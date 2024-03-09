#include "ScriptUtil.h"
#include "TESForms/TESForm.h"
#include "BSExtraData/ExtraAliasInstanceArray.h"
#include "TranslationCache.h"
#include "globals.h"
#include "TESForms/TESGlobal.h"
#include "NameUtil.h"
#include "GeneralUtil.h"
#include "TESForms/EffectSetting.h"

void GetScripts(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug("GetScript start");

	//if getting scripts is allowed
	if (!MICOptions::DisableScriptsAliases 
		&& (baseForm->formID != 0x0000007 
			|| !MICOptions::DisableScriptsAliasesPlayerOnly) ) 
	{
		//Get the VM handle for the form. Based on the HasVMAD method that is part of CommonLibSSEs implementation of TESFORM
		RE::BSScript::Internal::VirtualMachine* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

		if (vm) {
			RE::BSScript::IObjectHandlePolicy* policy = vm->GetObjectHandlePolicy();
			if (policy) {
				RE::VMHandle handle = policy->GetHandleForObject(baseForm->GetFormType(), baseForm);
				GetScriptsForHandle(resultArray, vm, policy, handle, baseForm, nullptr, nullptr);

				if (refForm) {
					handle = policy->GetHandleForObject(refForm->GetFormType(), refForm);
					GetScriptsForHandle(resultArray, vm, policy, handle, refForm, nullptr, nullptr);

					if (refForm->GetFormType() == RE::FormType::ActorCharacter) {
						//Check active effects if this is an actor
						RE::Actor* actor = nullptr;
						actor = static_cast<RE::Actor*>(refForm);

						if (actor) {
							RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = actor->AsMagicTarget()->GetActiveEffectList();
							logger::debug("GetScripts: Active Effects Gotten");

							if (activeEffects) {
								RE::BSSimpleList<RE::ActiveEffect*>::iterator itrEnd = activeEffects->end();

								for (RE::BSSimpleList<RE::ActiveEffect*>::iterator itr = activeEffects->begin(); itr != itrEnd; ++itr) {
									//logger::debug("GetCharacterData: Starting Active Effect");

									RE::ActiveEffect* activeEffect = *(itr);
									auto handleActiveEffect = policy->GetHandleForObject(RE::ActiveEffect::VMTYPEID, activeEffect);
									GetScriptsForHandle(resultArray, vm, policy, handleActiveEffect, nullptr, activeEffect, nullptr);
								}
							}
						}
					}

					RE::ExtraDataList* extraList = &refForm->extraList;

					//Need ini setting to skip this part for player. Check

					if (extraList->HasType(RE::ExtraDataType::kAliasInstanceArray)) {
						RE::ExtraAliasInstanceArray* aliasInstanceArray = static_cast<RE::ExtraAliasInstanceArray*>(extraList->GetByType(RE::ExtraDataType::kAliasInstanceArray));
						int numberOfAliases = aliasInstanceArray->aliases.size();

						for (int i = 0; i < numberOfAliases; i++) {
							auto alias = aliasInstanceArray->aliases[i]->alias;
							auto handleAlias = policy->GetHandleForObject(alias->GetVMTypeID(), alias);
							GetScriptsForHandle(resultArray, vm, policy, handleAlias, nullptr, nullptr, alias);
						}
					}
				}
			}
		}
	}
	
	logger::debug("GetScript End");
}

void GetScriptsForHandle(ExtraInfoEntry* resultArray, RE::BSScript::Internal::VirtualMachine* vm, RE::BSScript::IObjectHandlePolicy* policy, RE::VMHandle handle, RE::TESForm* form, RE::ActiveEffect* activeEffect, const RE::BGSBaseAlias* alias )
{
	//if (handle != policy->EmptyHandle())
	while (handle != policy->EmptyHandle())
	{
		//If we have a handle for the object the next step is to look if there are any scripts attached

		auto attachedScriptsIterator = vm->attachedScripts.find(handle);

		if (attachedScriptsIterator != vm->attachedScripts.end()) {
			RE::BSTSmallSharedArray<RE::BSScript::Internal::AttachedScript>* scripts = &(*attachedScriptsIterator).second;
			int numberOfScripts = scripts->size();

			for (int i = 0; i < numberOfScripts; i++) {
				auto script = (*scripts)[i].get();

				std::string scriptName = script->type->name.c_str();
				//std::string scriptName = script->type->G
				logger::debug("Found Script {}", scriptName );

				if (GetShouldDisplayScript(scriptName))
				{
					ExtraInfoEntry* scriptEntry;

					CreateExtraInfoEntry(scriptEntry, scriptName, "", priority_Scripts_Script);

					RE::TESForm* sourceForm = nullptr;

					if (form)
					{
						sourceForm = form;
					}
					else if (activeEffect)
					{
						sourceForm = activeEffect->GetBaseObject();
					}

					if (sourceForm)
					{
						bool isInMinimizeMode = MICGlobals::minimizeFormDataRead;

						MICGlobals::minimizeFormDataRead = true;

						ExtraInfoEntry* sourceEntry;
						std::string sourceName = GetName(sourceForm);
						CreateExtraInfoEntry(sourceEntry, GetTranslation("$Source"), sourceName, priority_Scripts_Source);

						GetFormData(sourceEntry, sourceForm, nullptr);

						scriptEntry->PushBack(sourceEntry);

						MICGlobals::minimizeFormDataRead = isInMinimizeMode;
					}

					else if (alias)
					{
						//add information on quest later
						ExtraInfoEntry* sourceEntry;
						CreateExtraInfoEntry(sourceEntry, GetTranslation("$Source"), GetTranslation("$Alias") + " : " + alias->aliasName.c_str(), priority_Scripts_Source);

						GetAliasInformation(sourceEntry, alias, true);

						scriptEntry->PushBack(sourceEntry);
					}

					ExtraInfoEntry* variablesAndPropertiesEntry;
					CreateExtraInfoEntry(variablesAndPropertiesEntry, GetTranslation("$VariablesProperties"), "", priority_Scripts_Script);  //this gives number of properties

					RE::BSScript::ObjectTypeInfo* objectTypeInfo = script->GetTypeInfo();

					if (objectTypeInfo) {
						variablesAndPropertiesEntry->needsExpansion = true;
						variablesAndPropertiesEntry->subarrayCountOverride = objectTypeInfo->GetTotalNumVariables();
						variablesAndPropertiesEntry->scriptToExpand = script;
					}

					scriptEntry->PushBack(variablesAndPropertiesEntry);

					//GetVariablesAndPropertiesForScript(scriptEntry, script);

					resultArray->PushBack(scriptEntry);
				}
			}
		}

		handle = policy->GetHandleScriptsMovedFrom(handle);
	}
}

void GetVariablesAndPropertiesForScript(ExtraInfoEntry* resultArray, RE::BSScript::Object * script )
{
	RE::BSScript::ObjectTypeInfo * objectTypeInfo = script->GetTypeInfo();

	bool isInMinimizeMode = MICGlobals::minimizeFormDataRead;
	MICGlobals::minimizeFormDataRead = true;
	
	int j = 0;

	//variables are ordered from the topmost level down, but the object we get is for the lowest level child. 
	//so we need to work our way up in levels from said child to the parent scripts, and then process them in reverse order
	//which is why we are storing all of the levels in a stack
	std::stack<RE::BSScript::ObjectTypeInfo*> objectTypeInfoStack;

	while (objectTypeInfo) 
	{
		objectTypeInfoStack.push(objectTypeInfo);
		objectTypeInfo = objectTypeInfo->GetParent();
	}

	while( !objectTypeInfoStack.empty() ) 
	{
		objectTypeInfo = objectTypeInfoStack.top();
		objectTypeInfoStack.pop();
		logger::info("{}", objectTypeInfo->GetName());
		
		const auto vars = objectTypeInfo->GetVariableIter();
		if (vars) {
			for (std::uint32_t i = 0; i < objectTypeInfo->GetNumVariables(); ++i) {
				const auto& varTypeInfo = vars[i];
				std::string variableName = varTypeInfo.name.c_str();

				auto variable = &(script->variables[j]);

				std::string cleanVariableName = CleanVariableName(variableName);

				GetVariableValue(resultArray, variable, cleanVariableName, false);

				j++;
			}
		}
	}
	
	MICGlobals::minimizeFormDataRead = isInMinimizeMode;
}

std::string CleanVariableName(std::string variableName)
{
	//properties are given variable names starting with :: and ending with _var which need to be removed
	if (variableName.size() > 6
		&& variableName.substr(0, 2) == "::"
		&& variableName.substr(variableName.size() - 4) == "_var")
	{
		//remove the last 4 characters and the first two
		variableName = variableName.substr(0, variableName.size() - 4).substr(2);
	}

	return variableName;
}

std::string GetVariableValue(ExtraInfoEntry* resultArray, RE::BSScript::Variable * variable, std::string variableName, bool isInArray)
{
	RE::TESForm * form = nullptr;
	RE::BGSBaseAlias* alias = nullptr;
	RE::BSScript::Array* arrayVarible = nullptr;
	RE::ActiveEffect* activeEffect = nullptr;

	std::string value = "";
	if (variable->IsBool())
	{
		value = BooleanToTrueFalseString(variable->GetBool());
	}
	else if (variable->IsFloat() )
	{
		value = FloatToString(variable->GetFloat());
	}
	else if (variable->IsInt())
	{
		value = IntToString(variable->GetSInt());
	}
	else if (variable->IsString())
	{
		value = variable->GetString();
	} 
	else if (variable->IsObject()) 
	{
		auto object = variable->GetObject();

		if (object && object->handle) {
			auto vmHandle = object->handle;

			auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			auto policy = vm ? vm->GetObjectHandlePolicy() : nullptr;
			if (policy) {
				int vmHandleType = 139;  //Handle types 0-138 are forms and can be handled with the same code so we don't need specific checks
				bool vmHandleTypeFound = false;

				while (!vmHandleTypeFound && vmHandleType <= 142)
				{
					vmHandleTypeFound = policy->HandleIsType(vmHandleType, vmHandle);

					if (!vmHandleTypeFound) 
					{
						vmHandleType++;
					}
				}

				if (vmHandleTypeFound) {
					if (vmHandleType == 142)  //Active Effect
					{
						activeEffect = static_cast<RE::ActiveEffect*>(object->Resolve(vmHandleType));
						value = GetName(activeEffect->GetBaseObject());
					} 
					else  //all remaining vmHandle types are subclasses of BGSBaseAlias
					{
						alias = static_cast<RE::BGSBaseAlias*>(object->Resolve(vmHandleType));
						value = GetTranslation("$Alias") + " : " + alias->aliasName.c_str();
					}
				} 
				else 
				{
					//logger::info("Type Found");

					form = static_cast<RE::TESForm*>(object->Resolve(0));  //0 seems to always resolve regardless of the handle type
					RE::TESObjectREFR* refForm = form ? form->As<RE::TESObjectREFR>() : nullptr;

					if (refForm) 
					{
						form = refForm->data.objectReference;
					}

					if (form) 
					{
						if (form->formType != RE::FormType::Global) 
						{
							value = GetName(form, refForm);
						} 
						else 
						{
							value = FloatToString(GetGlobalValue(form));
						}
					}
				}

			} 
			else 
			{
				value = GetTranslation("$PropertyNone");
			}
		}
	}
	else if ( variable->IsArray() )
	{
		//I'm not sure if the above checks include NoneArrays so add a safety check here just in case
		if( variable->IsNoneArray() ) 
		{
			value = GetTranslation("$PropertyNone");
		}
		else 
		{
			auto arrayVariablePointer = variable->GetArray();
			if (arrayVariablePointer) 
			{
				arrayVarible = arrayVariablePointer.get();
			}

			value = GetTranslation("$PropertyArray");
		}
	}
	else
	{
		value = GetTranslation("$PropertyNone");
	}	

	//object type
	//get vm handle. VM handle can be used to get a form

	ExtraInfoEntry* variableEntry;

	//rerannage the values inside array to look better
	if (isInArray) 
	{
		variableName = value;
		value = "";
	}

	CreateExtraInfoEntry(variableEntry, variableName, value, priority_Scripts_VariableProperty); //this gives number of properties

	if (form)
	{
		RE::TESObjectREFR * refForm = nullptr;

		if( form->formType == RE::FormType::Reference
			|| form->formType == RE::FormType::ActorCharacter )
		{
			refForm = static_cast<RE::TESObjectREFR*>(form);
			form = refForm->data.objectReference;
		}

		//In the unlikely case that a ref form has no base form associated with it skip the rest of the code
		//as that code is not meant to handle this edge case
		if( form )
		{
			GetFormData(variableEntry, form, refForm);
		}
	}
	else if (alias)
	{
		GetAliasInformation(variableEntry, alias, true);
	} 
	
	else if (activeEffect)
	{
		//get the information we need to call the GetEffectData method
		bool active = IsEffectActive(activeEffect);
		RE::Effect* effect = activeEffect->effect;
		auto caster = activeEffect->GetCasterActor().get();

		GetEffectData(variableEntry, effect, caster, true, activeEffect->duration, activeEffect->elapsedSeconds, active, activeEffect->magnitude);
	}
	else if ( arrayVarible ) 
	{
		for (auto i = arrayVarible->begin(); i != arrayVarible->end(); i++ )
		{
			RE::BSScript::Variable* variableInArray = i;

			if (variableInArray ) 
			{
				GetVariableValue(variableEntry, variableInArray, "", true);
			} 
			else //I don't think this is possible but add an empty entry if the pointer somehow doesn't point to a variable
			{
				ExtraInfoEntry* variableInArrayEntry;
				CreateExtraInfoEntry(variableInArrayEntry, "", "", priority_Scripts_VariableProperty); 
				variableEntry->PushBack(variableInArrayEntry);
			}

		}
	}

	resultArray->PushBack(variableEntry);

	return value;
}
