#include "ScriptUtil.h"
#include "TESForms/TESForm.h"
#include "TranslationCache.h"
#include "globals.h"
#include "NameUtil.h"
#include "GeneralUtil.h"

void GetScripts(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug("GetScript start");

	//Get the VM handle for the form. Based on the HasVMAD method that is part of CommonLibSSEs implementation of TESFORM
	RE::BSScript::Internal::VirtualMachine* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

	MICGlobals::minimizeFormDataRead = true;

	if (vm)
	{
		RE::BSScript::IObjectHandlePolicy* policy = vm->GetObjectHandlePolicy();
		if (policy)
		{
			RE::VMHandle handle = policy->GetHandleForObject(baseForm->GetFormType(), baseForm);
			GetScriptsForHandle(resultArray, vm, policy, handle, baseForm, nullptr, nullptr);
		}

		if (refForm)
		{
			RE::VMHandle handle = policy->GetHandleForObject(refForm->GetFormType(), refForm);
			GetScriptsForHandle(resultArray, vm, policy, handle, refForm, nullptr, nullptr);

			if (refForm->GetFormType() == RE::FormType::ActorCharacter)
			{
				//Check active effects if this is an actor
				RE::Actor* actor = nullptr;
				actor = static_cast<RE::Actor*>(refForm);

				if (actor)
				{
#ifndef SKYRIMVR
					RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = actor->GetActiveEffectList();
					logger::debug("GetScripts: Active Effects Gotten");

					if (activeEffects)
					{
						RE::BSSimpleList<RE::ActiveEffect*>::iterator itrEnd = activeEffects->end();

						for (RE::BSSimpleList<RE::ActiveEffect*>::iterator itr = activeEffects->begin(); itr != itrEnd; ++itr)
						{
							//logger::debug("GetCharacterData: Starting Active Effect");

							RE::ActiveEffect* activeEffect = *(itr);
							auto handleActiveEffect = policy->GetHandleForObject(RE::ActiveEffect::VMTYPEID, activeEffect);
							GetScriptsForHandle(resultArray, vm, policy, handleActiveEffect, nullptr, activeEffect, nullptr);
						}
					}
#else
					int total = 0;
					logger::debug("GetScripts: Starting Active Effect");

					actor->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
						logger::debug("GetScripts: Visiting Active Effect {}", total++);
						if (activeEffect) {
							auto handleActiveEffect = policy->GetHandleForObject(RE::ActiveEffect::VMTYPEID, activeEffect);
							GetScriptsForHandle(resultArray, vm, policy, handleActiveEffect, nullptr, activeEffect, nullptr);
						}
						return RE::BSContainer::ForEachResult::kContinue;
					});

#endif
				}
			}

			RE::ExtraDataList* extraList = &refForm->extraList;

			//Need ini setting to skip this part for player. Check 

			if (extraList->HasType(RE::ExtraDataType::kAliasInstanceArray))
			{
				RE::ExtraAliasInstanceArray* aliasInstanceArray = static_cast<RE::ExtraAliasInstanceArray*>(extraList->GetByType(RE::ExtraDataType::kAliasInstanceArray));
				int numberOfAliases = aliasInstanceArray->aliases.size();

				for (int i = 0; i < numberOfAliases; i++)
				{
					auto alias = aliasInstanceArray->aliases[i]->alias;
					auto handleAlias = policy->GetHandleForObject(alias->GetVMTypeID(), alias);
					GetScriptsForHandle(resultArray, vm, policy, handleAlias, nullptr, nullptr, alias);
				}

			}
		}
	}

	MICGlobals::minimizeFormDataRead = false;
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
						ExtraInfoEntry* sourceEntry;
						std::string sourceName = GetName(sourceForm);
						CreateExtraInfoEntry(sourceEntry, GetTranslation("$Source"), sourceName, priority_Scripts_Source);

						GetFormData(sourceEntry, sourceForm, nullptr);

						scriptEntry->PushBack(sourceEntry);
					}

					else if (alias)
					{
						//add information on quest later
						ExtraInfoEntry* sourceEntry;
						CreateExtraInfoEntry(sourceEntry, GetTranslation("$Source"), alias->aliasName.c_str(), priority_Scripts_Source);

						GetAliasInformation(sourceEntry, alias);

						scriptEntry->PushBack(sourceEntry);
					}

					GetVariablesAndPropertiesForScript(scriptEntry, script);

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

	
	if (objectTypeInfo)
	{
		ExtraInfoEntry* numProperties;
		ExtraInfoEntry* numVariables;
		ExtraInfoEntry* totalVariables;

		CreateExtraInfoEntry(numProperties, "Properties", IntToString( objectTypeInfo->GetNumProperties() ), priority_Scripts_Script); //this gives number of properties
		CreateExtraInfoEntry(numVariables, "Variables", IntToString(objectTypeInfo->GetNumVariables()), priority_Scripts_Script); //this gives number of properties + variables in script
		CreateExtraInfoEntry(totalVariables, "Total", IntToString(objectTypeInfo->GetTotalNumVariables()), priority_Scripts_Script); //this gives number of properties and variables in script and parents

		resultArray->PushBack(numProperties);
		resultArray->PushBack(numVariables);
		resultArray->PushBack(totalVariables);

		//logger::info(script->type->name.c_str());

		ExtraInfoEntry* variablesAndProperties;
		CreateExtraInfoEntry(variablesAndProperties, GetTranslation("$VariablesProperties") , "", priority_Scripts_Script); //this gives number of properties

		const auto vars = objectTypeInfo->GetVariableIter();
		if (vars) {
			for (std::uint32_t i = 0; i < objectTypeInfo->GetNumVariables(); ++i) {
				const auto& varTypeInfo = vars[i];
				std::string variableName = varTypeInfo.name.c_str();

				auto variable = &(script->variables[i]);

				std::string cleanVariableName = CleanVariableName(variableName);
				GetVariableValue(variablesAndProperties, variable, cleanVariableName);
			}
		}

		resultArray->PushBack(variablesAndProperties);
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

std::string GetVariableValue(ExtraInfoEntry* resultArray, RE::BSScript::Variable * variable, std::string variableName)
{
	RE::TESForm * form = nullptr;
	RE::BGSBaseAlias* alias = nullptr;

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

		if (object)
		{
			auto vmHandle = object->handle;

			auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			auto policy = vm ? vm->GetObjectHandlePolicy() : nullptr;
			if (policy)
			{
				int vmHandleType = 139; //Handle types 0-138 are forms and can be handled with the same code so we don't need specific checks
				bool vmHandleTypeFound = false;

				while (!vmHandleTypeFound
					&& vmHandleType <= 142)
				{
					vmHandleTypeFound = policy->HandleIsType(vmHandleType, vmHandle);

					if (!vmHandleTypeFound)
					{
						vmHandleType++;
					}
				}

				if (vmHandleTypeFound)
				{
					if (vmHandleType == 139) //Active Effect
					{

					}
					else //all remaining vmHandle types are subclasses of BGSBaseAlias
					{
						alias = static_cast<RE::BGSBaseAlias*>(object->Resolve(vmHandleType));
						value = alias->aliasName;
					}
				}
				else
				{
					//logger::info("Type Found");

					form = static_cast<RE::TESForm*>(object->Resolve(0)); //0 seems to always resolve regardless of the handle type

					if (form)
					{
						value = GetName(form);
					}
				}


			}
		}

		else
		{
			value = GetTranslation("$PropertyNone");
		}
	}

	//object type
	//get vm handle. VM handle can be used to get a form

	ExtraInfoEntry* variableEntry;
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
		GetAliasInformation(variableEntry, alias);
	}

	resultArray->PushBack(variableEntry);

	return value;
}

void GetAliasInformation(ExtraInfoEntry* resultArray, const RE::BGSBaseAlias * alias)
{
	ExtraInfoEntry* aliasNameEntry;

	CreateExtraInfoEntry(aliasNameEntry, GetTranslation("$AliasName"), alias->aliasName.c_str() , priority_Scripts_AliasName); //this gives number of properties
	resultArray->PushBack(aliasNameEntry);

	RE::TESQuest* owningQuest = alias->owningQuest;

	if (owningQuest)
	{
		ExtraInfoEntry* aliasQuestEntry;
		CreateExtraInfoEntry(aliasQuestEntry, GetTranslation("$AliasQuest"), GetName(owningQuest), priority_Scripts_AliasQuest);
		GetFormData(aliasQuestEntry, owningQuest, nullptr);
		resultArray->PushBack(aliasQuestEntry);
	}
}
