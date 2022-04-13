#include "TESForm.h"
#include "BGSLocation.h"
#include "BGSTextureSet.h"
#include "EffectSetting.h"
#include "MagicItem.h"
#include "SpellItem.h"
#include "TESAmmo.h"
#include "TESModelTextureSwap.h"
#include "TESObjectARMA.h"
#include "TESObjectARMO.h"
#include "TESObjectCELL.h"
#include "TESObjectCONT.h"
#include "TESObjectREFR.h"
#include "TESObjectWeap.h"
#include "TESRace.h"
#include "Util/NameUtil.h"
#include "globals.h"
#include "EditorIDCache.h"
#include "FormExtraInfoCache.h"

bool GetHasSourceFileArray(RE::TESForm* form)
{
	return form->sourceFiles.array;  //Check if the source files array exists
}

std::string GetNthFormLocationName(RE::TESForm* form, uint32_t n)
{
	std::string formName = "Unknown";

	if (GetHasSourceFileArray(form) && form->sourceFiles.array->size() > n) {
		RE::TESFile** sourceFiles = form->sourceFiles.array->data();
		formName = sourceFiles[n]->fileName;
	}

	return formName;
}

std::string GetFirstFormLocationName(RE::TESForm* form)
{
	return GetNthFormLocationName(form, 0);
}

std::string GetLastFormLocationName(RE::TESForm* form)
{
	std::string formName = "Unknown";
	int lastFileIndex = GetNumberOfSourceFiles(form) - 1;
	formName = GetNthFormLocationName(form, lastFileIndex);

	return formName;
}

int GetNumberOfSourceFiles(RE::TESForm* form)
{
	int numberOfFiles = 0;

	if (GetHasSourceFileArray(form)) {
		numberOfFiles = form->sourceFiles.array->size();
	}

	return numberOfFiles;
}

//general wrapper for all get form methods
void GetFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug(("GetExtraData: Get Form Data Start " + GetFormTypeName((int)baseForm->formType.underlying()) + " " + FormIDToString(baseForm->formID)).c_str());

	auto formExtraInfoCache = FormExtraInfoCache::GetSingleton();
	auto extraInfoEntryCached = formExtraInfoCache->GetExtraInfoEntry(baseForm);

	if (extraInfoEntryCached == nullptr)
	{
		GetCommonFormData(resultArray, baseForm, refForm);

		if (!MICGlobals::minimizeFormDataRead) //for the caster of an effect just getting the common form data should be enough information
		{
			if (refForm != nullptr) {
				GetReferenceFormData(resultArray, refForm);
			}

			RE::FormType baseFormType = baseForm->GetFormType();

			if (baseForm != nullptr && baseFormType == RE::FormType::NPC && (refForm == nullptr || refForm->GetFormType() == RE::FormType::ActorCharacter)) {
				logger::debug("GetExtraData: Get Form Data character found");
				GetCharacterData(resultArray, refForm, baseForm);
			}

			else if (baseFormType == RE::FormType::MagicEffect) {
				logger::debug("GetExtraData: Get Form Data magic effect found");
				GetMagicEffectData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Spell) {
				logger::debug("GetExtraData: Get Form Data spell found");
				GetSpellData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Armor) {
				logger::debug("GetExtraData: Get Form Data armor found");
				GetArmorData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Weapon) {
				logger::debug("GetExtraData: Get Form Data Weapon found");
				GetWeaponData(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Ammo) {
				logger::debug("GetExtraData: Get Form Data Ammo found");
				GetAmmoData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Container) {
				logger::debug("GetExtraData: Get Form Data Container found");
				GetContainerData(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Race) {
				logger::debug("GetExtraData: Get Form Data Race found");
				GetRaceEntry(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::TextureSet) {
				logger::debug("GetExtraData: Get Form Data Texture Set found");
				GetTextureSet(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Armature) {
				logger::debug("GetExtraData: Get Form Data ARMA found");
				GetArmaData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Cell) {
				logger::debug("GetExtraData: Get Form Data CELL found");
				GetCellEntry(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Location) {
				logger::debug("GetExtraData: Get Form Data LCTN found");
				GetLocationEntry(resultArray, baseForm);
			}

			//reset any filtering
			MICGlobals::filterARMAByRace = nullptr;

			formExtraInfoCache->CacheExtraInfoEntry(baseForm, resultArray);
		}
	}
	else
	{
		resultArray->CopyChildren(extraInfoEntryCached);
	}

	logger::debug("GetExtraData: Get Form Data End");
}

//get data common to all form types
void GetCommonFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug("GetCommonFormData Start");

	//If the base form was found in FF, get the template for that form which is going to be more relevant
	if (baseForm->formType == RE::FormType::NPC && baseForm->formID >= 0xFF000000) {
		logger::debug("Found actor with FF base form");
		baseForm = GetRootTemplate(baseForm);
	}

	std::string name = "";

	//The getName function returns the editor ID for race as that is more useful in identifying the actual race
	//but we want to display the real name when we are getting data for the form
	if (baseForm->formType == RE::FormType::Race) {
		RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
		if (race) {
			name = race->fullName.c_str();
		}
	}

	else {
		//name
		name = GetName(baseForm);
	}

	ExtraInfoEntry* nameArray;
	CreateExtraInfoEntry(nameArray, "Name", name, priority_Name);

	resultArray->PushBack(nameArray);

	
	//highly experimental code. This has a noticable hitch when selecting player character.
	//Requires Power of 3 tweaks to actually store all of this information
	auto editorIDCache = EditorIDCache::GetSingleton();

	std::string editorID = editorIDCache->GetEditorID(baseForm);

	if (editorID == "")
	{
		editorID = baseForm->GetFormEditorID();
	}

	if (editorID != "")
	{
		ExtraInfoEntry* editorIDEntry;
		CreateExtraInfoEntry(editorIDEntry, "Editor ID", editorID, priority_EditorID);
		resultArray->PushBack(editorIDEntry);
	}

	//base formid
	std::string formID = FormIDToString(baseForm->formID);

	ExtraInfoEntry* formIDArray;
	CreateExtraInfoEntry(formIDArray, "Base form ID", formID, priority_FormID);
	resultArray->PushBack(formIDArray);

	//base form type
	std::string baseFormType = GetFormTypeName((int)baseForm->formType.underlying());

	ExtraInfoEntry* formTypeEntry;
	CreateExtraInfoEntry(formTypeEntry, "Base Type", baseFormType, priority_FormType);
	resultArray->PushBack(formTypeEntry);

	//ref formid
	if (refForm != nullptr) {
		std::string refFormID = FormIDToString(refForm->formID);

		ExtraInfoEntry* formIDArrayReference;
		CreateExtraInfoEntry(formIDArrayReference, "Ref form ID", refFormID, priority_FormID);
		resultArray->PushBack(formIDArrayReference);
	}

	//mod location info
	GetFormLocationData(resultArray, baseForm, refForm);

	//Model information
	GetModelTextures(resultArray, baseForm );

	if (MICOptions::ExperimentalFeatures) {

		if (!MICGlobals::minimizeFormDataRead)
		{
			//Get scripts
			ExtraInfoEntry* scriptsEntry;
			CreateExtraInfoEntry(scriptsEntry, "Scripts", "", priority_Scripts_Scripts);

			GetScripts(scriptsEntry, baseForm, refForm);

			//There's no point showing an empty script entry as having no scripts is the standard and some formtypes can't even have scripts. So check if we found anything
			if (scriptsEntry->HasChildren()) {
				resultArray->PushBack(scriptsEntry);
			}

			else {
				delete (scriptsEntry);  //Free up the memory
			}
		}
	}

	logger::debug("GetCommonFormData: GetCommonFormData End");
}

//get information related to where mods the form is found in
void GetFormLocationData(ExtraInfoEntry*& resultArray, RE::TESForm* baseForm, RE::TESForm* refForm)
{
	logger::debug("GetExtraData: GetFormLocationData Start");

	ExtraInfoEntry* formLocationHolder;
	CreateExtraInfoEntry(formLocationHolder, "Form location information", "", priority_FormLocation);

	//this method may be called at a time we only have a base form, and in that case skip anything related to the reform
	if (refForm != nullptr && GetHasSourceFileArray(refForm)) {
		logger::debug("GetExtraData: GetFormLocationData ref mod info found");

		//Reference Form

		std::string refFirstDefinedIn = GetFirstFormLocationName(refForm);
		bool SkyrimESMNotDetectedBug = false;

		//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
		if (((refForm->formID & 0xFF000000) == 0) && refFirstDefinedIn != "Skyrim.esm") {
			refFirstDefinedIn = "Skyrim.esm";
			SkyrimESMNotDetectedBug = true;
		}

		ExtraInfoEntry* referenceDefinedIn;
		CreateExtraInfoEntry(referenceDefinedIn, "Reference defined in", refFirstDefinedIn, priority_FormLocation_ReferenceDefined);

		formLocationHolder->PushBack(referenceDefinedIn);

		std::string refLastDefinedIn = GetLastFormLocationName(refForm);

		ExtraInfoEntry* referenceLastChangedBy;
		CreateExtraInfoEntry(referenceLastChangedBy, "Reference last modified by", refLastDefinedIn, priority_FormLocation_ReferenceLastChanged);

		formLocationHolder->PushBack(referenceLastChangedBy);

		ExtraInfoEntry* allModsTouchingReferenceHolder;
		CreateExtraInfoEntry(allModsTouchingReferenceHolder, "Reference found in", "", priority_FormLocation_ReferenceInMods);

		GetModInfoData(allModsTouchingReferenceHolder, refForm, SkyrimESMNotDetectedBug);

		logger::debug("GetExtraData: Ref Last Modified By " + refLastDefinedIn);

		formLocationHolder->PushBack(allModsTouchingReferenceHolder);
	}
	//Base Form

	logger::debug("GetExtraData: GetFormLocationData at pBaseSection section");

	if (GetHasSourceFileArray(baseForm)) {
		logger::debug("GetExtraData: GetFormLocationData baseFormModInfo found");

		std::string baseFirstDefinedIn = GetFirstFormLocationName(baseForm);

		ExtraInfoEntry* baseDefinedIn;
		CreateExtraInfoEntry(baseDefinedIn, "Base defined in", baseFirstDefinedIn, priority_FormLocation_BaseDefined);

		formLocationHolder->PushBack(baseDefinedIn);

		std::string baseLastDefinedIn = GetLastFormLocationName(baseForm);

		ExtraInfoEntry* baseLastChangedBy;
		CreateExtraInfoEntry(baseLastChangedBy, "Base last modified by", baseLastDefinedIn, priority_FormLocation_BaseLastChanged);

		formLocationHolder->PushBack(baseLastChangedBy);

		ExtraInfoEntry* allModsTouchingBaseHolder;
		CreateExtraInfoEntry(allModsTouchingBaseHolder, "Base found in", "", priority_FormLocation_BaseInMods);

		GetModInfoData(allModsTouchingBaseHolder, baseForm, false);

		logger::debug("GetExtraData: Base Last Modified By " + baseLastDefinedIn);

		formLocationHolder->PushBack(allModsTouchingBaseHolder);
	}

	resultArray->PushBack(formLocationHolder);

	logger::debug("GetExtraData: GetFormLocationData End");
}

void GetModInfoData(ExtraInfoEntry*& resultArray, RE::TESForm* form, bool SkyrimESMNotDetectedBug)
{
	logger::debug("GetExtraData: GetModInfoData start");

	int numMods = GetNumberOfSourceFiles(form);

	if (SkyrimESMNotDetectedBug) {
		ExtraInfoEntry* modEntry;

		CreateExtraInfoEntry(modEntry, "Mod", "Skyrim.esm", priority_Default);
		resultArray->PushBack(modEntry);
	}

	for (int i = 0; i < numMods; i++) {
		ExtraInfoEntry* modEntry;

		std::string modName = GetNthFormLocationName(form, i);

		CreateExtraInfoEntry(modEntry, "Mod", modName, priority_Default);
		resultArray->PushBack(modEntry);
	}

	logger::debug("GetExtraData: GetModInfoData end");
}

void GetScripts(ExtraInfoEntry*& resultArray, RE::TESForm* baseForm, RE::TESForm* refForm)
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
			GetScriptsForHandle(resultArray, vm, policy, handle, baseForm, nullptr);
		}

		if (refForm)
		{
			RE::VMHandle handle = policy->GetHandleForObject(refForm->GetFormType(), baseForm);
			GetScriptsForHandle(resultArray, vm, policy, handle, refForm, nullptr);

			if (refForm->GetFormType() == RE::FormType::ActorCharacter)
			{
				//Check active effects if this is an actor
				RE::Actor* actor = nullptr;
				actor = static_cast<RE::Actor*>(refForm);

				if (actor)
				{
#ifndef SKYRIMVR
					RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = actor->GetActiveEffectList();
					logger::debug("GetCharacterData: Active Effects Gotten");

					if (activeEffects)
					{
						RE::BSSimpleList<RE::ActiveEffect*>::iterator itrEnd = activeEffects->end();

						for (RE::BSSimpleList<RE::ActiveEffect*>::iterator itr = activeEffects->begin(); itr != itrEnd; ++itr)
						{
							//logger::debug("GetCharacterData: Starting Active Effect");

							RE::ActiveEffect* activeEffect = *(itr);
							auto handleActiveEffect = policy->GetHandleForObject(RE::ActiveEffect::VMTYPEID, activeEffect);
							GetScriptsForHandle(resultArray, vm, policy, handleActiveEffect, nullptr, activeEffect);
						}
					}
#endif
				}
			}
		}
	}

	MICGlobals::minimizeFormDataRead = false;

	logger::debug("GetScript End");
}

void GetScriptsForHandle(ExtraInfoEntry*& resultArray, RE::BSScript::Internal::VirtualMachine* vm, RE::BSScript::IObjectHandlePolicy* policy, RE::VMHandle handle, RE::TESForm* form, RE::ActiveEffect* activeEffect)
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

				ExtraInfoEntry* scriptEntry;

				CreateExtraInfoEntry(scriptEntry, scriptName, "", priority_Scripts_Script);

				RE::TESForm* sourceForm = nullptr;

				if (form)
				{
					sourceForm = form;
				}
				else if( activeEffect )
				{
					sourceForm = activeEffect->GetBaseObject();
				}

				if (sourceForm)
				{
					ExtraInfoEntry* sourceEntry;
					std::string sourceName = GetName(sourceForm);
					CreateExtraInfoEntry(sourceEntry, "Source", sourceName, priority_Scripts_Source);

					GetFormData(sourceEntry, sourceForm, nullptr);

					scriptEntry->PushBack(sourceEntry);
				}

				resultArray->PushBack(scriptEntry);
			}
		}

		handle = policy->GetHandleScriptsMovedFrom(handle);
	}
}

//Get all keywords for forms that store keywords in the normal location
void GetKeywords(ExtraInfoEntry*& resultArray, RE::BGSKeywordForm* keywordForm)
{
	logger::debug("GetKeywords Start");

	if (keywordForm) {
		ExtraInfoEntry* keywordsEntry;

		CreateExtraInfoEntry(keywordsEntry, "Keywords", "", priority_Keywords);

		logger::debug("GetKeywords Before crash");

		for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
			RE::BGSKeyword* keyword = *(keywordForm->GetKeywordAt(i));

			if (keyword && ((keyword->formID & 0xFF000000) != 0xFF000000))  //There was a strange Keyword with a formid starting with FF that is causing a crash. There must be some skse plugin reponsible for this
																			//as it can't be created by normal methods. For now I want to filter those keywords out
			{
				ExtraInfoEntry* keywordEntry;
				std::string keywordName = GetName(keyword);

				CreateExtraInfoEntry(keywordEntry, keywordName, "", priority_Keywords);

				GetFormData(keywordEntry, keyword, nullptr);

				keywordsEntry->PushBack(keywordEntry);
			}
		}

		resultArray->PushBack(keywordsEntry);
	}

	logger::debug("GetKeywords End");
}
