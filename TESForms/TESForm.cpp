#include "TESForm.h"
#include "TESObjectREFR.h"
#include "TESRace.h"
#include "MoreInformativeConsole/Util/NameUtil.h"

bool GetHasSourceFileArray(RE::TESForm* form)
{
	return form->sourceFiles.array;	//Check if the source files array exists
}

std::string GetNthFormLocationName(RE::TESForm* form, UInt32 n)
{
	std::string formName = "Unknown";

	if (GetHasSourceFileArray( form )
		&& form->sourceFiles.array->size() > n)
	{
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
	formName = GetNthFormLocationName( form, lastFileIndex);

	return formName;
}

int GetNumberOfSourceFiles(RE::TESForm* form )
{
	int numberOfFiles = 0;

	if( GetHasSourceFileArray(form) )
	{
		numberOfFiles = form->sourceFiles.array->size();
	}

	return numberOfFiles;
}


//general wrapper for all get form methods
void GetFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	_DMESSAGE( ( "GetExtraData: Get Form Data Start " + GetFormTypeName((int)baseForm->formType) + " " + FormIDToString(baseForm->formID) ).c_str() );

	GetCommonFormData(resultArray, baseForm, refForm);

	if (refForm != nullptr)
	{
		GetReferenceFormData(resultArray, refForm);
	}

	
	if (baseForm != nullptr
		&& baseForm->GetFormType() == RE::FormType::NPC
		&& (refForm == nullptr
			|| refForm->GetFormType() == RE::FormType::ActorCharacter))
	{
		_DMESSAGE("GetExtraData: Get Form Data character found");
		GetCharacterData(resultArray, refForm, baseForm);
	}
	/*
	else if (pBaseForm->GetFormType() == kFormType_EffectSetting)
	{
		DebugMessage("GetExtraData: Get Form Data magic effect found");
		GetMagicEffectData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Spell)
	{
		DebugMessage("GetExtraData: Get Form Data spell found");
		GetSpellData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Armor)
	{
		DebugMessage("GetExtraData: Get Form Data armor found");
		GetArmorData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Weapon)
	{
		DebugMessage("GetExtraData: Get Form Data Weapon found");
		GetWeaponData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Ammo)
	{
		DebugMessage("GetExtraData: Get Form Data Ammo found");
		GetAmmoData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Container)
	{
		DebugMessage("GetExtraData: Get Form Data Container found");
		GetContainerData(resultArray, pBaseForm);
	}
	else if (pBaseForm->GetFormType() == kFormType_Faction)
	{
		DebugMessage("GetExtraData: Get Form Data Faction found");
		//GetContainerData(resultArray, pBaseForm);
	}*/

	else if (baseForm->GetFormType() == RE::FormType::Race)
	{
		_DMESSAGE("GetExtraData: Get Form Data Race found");
		GetRaceEntry(resultArray, baseForm);
	}
	/*
	else if (pBaseForm->GetFormType() == kFormType_TextureSet)
	{
		DebugMessage("GetExtraData: Get Form Data Texture Set found");
		GetTextureSet(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_ARMA)
	{
		DebugMessage("GetExtraData: Get Form Data ARMA found");
		GetArmaData(resultArray, pBaseForm);
	}

	else if (pBaseForm->GetFormType() == kFormType_Cell)
	{
		DebugMessage("GetExtraData: Get Form Data CELL found");
		GetCellEntry(resultArray, pBaseForm);
	}

	//get inventory
	if (pRefForm != nullptr
		&& pRefForm->extraData.HasType(kExtraData_ContainerChanges))
	{
		DebugMessage("GetFormData: Found Inventory Start");


		ExtraContainerChanges* inventoryExtraData = static_cast<ExtraContainerChanges*> (pRefForm->extraData.GetByType(kExtraData_ContainerChanges));
		EntryDataList* inventory = inventoryExtraData->data->objList;

		DebugMessage("GetFormData: Got Inventory");

		TESContainer* pContainer = nullptr;

		if (pBaseForm->GetFormType() == kFormType_NPC)
		{
			DebugMessage("GetFormData: Inventory npc");

			Actor* pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);

			if (pActor)
			{
				GetEquipment(resultArray, inventoryExtraData, pActor);
			}

			TESActorBase* pActorBase = DYNAMIC_CAST(pBaseForm, TESForm, TESActorBase);

			if (pActorBase)
			{
				pContainer = &pActorBase->container;
				DebugMessage("GetFormData: Inventory npc container gotten");

			}
		}

		if (pBaseForm->GetFormType() == kFormType_Container)
		{
			DebugMessage("GetFormData: Inventory container");

			TESObjectCONT* pContainerForm = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);

			if (pContainerForm)
			{
				DebugMessage("GetFormData: Inventory cpntainer container gotten");
				pContainer = &pContainerForm->container;
			}
		}

		GetInventory(resultArray, inventory, pContainer);

	}

	GetModelTextures(resultArray, pBaseForm);

	//Handle BSExtra data
	if (pRefForm != nullptr)
	{
		GetBSExtraData(resultArray, pRefForm);
	}

	//reset any filtering
	MICGlobals::filterARMAByRace = nullptr;
	*/
	_DMESSAGE("GetExtraData: Get Form Data End");
}

//get data common to all form types
void GetCommonFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESForm* refForm)
{
	_DMESSAGE("GetCommonFormData Start");
	
	//If the base form was found in FF, get the template for that form which is going to be more relevant
	if (baseForm->formType == RE::FormType::NPC && baseForm->formID >= 0xFF000000)
	{
		_DMESSAGE("Found actor with FF base form");
		baseForm = GetRootTemplate(baseForm);
	}

	std::string name = "";

	//The getName function returns the editor ID for race as that is more useful in identifying the actual race
	//but we want to display the real name when we are getting data for the form
	if (baseForm->formType == RE::FormType::Race )
	{
		RE::TESRace* pRace = static_cast<RE::TESRace*>(baseForm);
		if (pRace)
		{
			name = pRace->fullName.c_str();
		}
	}

	else
	{
		//name
		name = GetName(baseForm);
	}

	ExtraInfoEntry* nameArray;
	CreateExtraInfoEntry(nameArray, "Name", name, priority_Name);

	resultArray->PushBack(nameArray);
	
	std::string editorID = baseForm->GetFormEditorID();

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
	std::string baseFormType = GetFormTypeName((int)baseForm->formType);

	ExtraInfoEntry* formTypeEntry;
	CreateExtraInfoEntry(formTypeEntry, "Base Type", baseFormType, priority_FormType);
	resultArray->PushBack(formTypeEntry);

	//ref formid
	if (refForm != nullptr)
	{
		std::string refFormID = FormIDToString(refForm->formID);

		ExtraInfoEntry* formIDArray;
		CreateExtraInfoEntry(formIDArray, "Ref form ID", refFormID, priority_FormID);
		resultArray->PushBack(formIDArray);
	}

	//mod location info

	GetFormLocationData(resultArray, baseForm, refForm);

	_DMESSAGE("GetCommonFormData: GetCommonFormData End");
}

//get information related to where mods the form is found in
void GetFormLocationData(ExtraInfoEntry*& resultArray, RE::TESForm* baseForm, RE::TESForm* refForm)
{
	_DMESSAGE("GetExtraData: GetFormLocationData Start");

	ExtraInfoEntry* formLocationHolder;
	CreateExtraInfoEntry(formLocationHolder, "Form location information", "", priority_FormLocation);

	//this method may be called at a time we only have a base form, and in that case skip anything related to the reform
	if (refForm != nullptr 
		&& GetHasSourceFileArray( refForm ) )
	{
		_DMESSAGE("GetExtraData: GetFormLocationData ref mod info found");

		//Reference Form
		int numModsModifyingRef = GetNumberOfSourceFiles(refForm);

		std::string refFirstDefinedIn = GetFirstFormLocationName(refForm);
		boolean SkyrimESMNotDetectedBug = false;

		//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
		if( ( (refForm->formID & 0xFF000000) == 0)
			&& refFirstDefinedIn != "Skyrim.esm")
		{
			refFirstDefinedIn = "Skyrim.esm";
			SkyrimESMNotDetectedBug = true;
		}

		ExtraInfoEntry* referenceDefinedIn;
		CreateExtraInfoEntry(referenceDefinedIn, "Reference defined in", refFirstDefinedIn);

		formLocationHolder->PushBack(referenceDefinedIn);

		std::string refLastDefinedIn = GetLastFormLocationName(refForm);

		ExtraInfoEntry* referenceLastChangedBy;
		CreateExtraInfoEntry(referenceLastChangedBy, "Reference last modified by", refLastDefinedIn);

		formLocationHolder->PushBack(referenceLastChangedBy);

		ExtraInfoEntry* allModsTouchingReferenceHolder;
		CreateExtraInfoEntry(allModsTouchingReferenceHolder, "Reference found in", "");

		GetModInfoData(allModsTouchingReferenceHolder, refForm, SkyrimESMNotDetectedBug);

		formLocationHolder->PushBack(allModsTouchingReferenceHolder);
	}
	//Base Form

	_DMESSAGE("GetExtraData: GetFormLocationData at pBaseSection section");
	
	if (GetHasSourceFileArray(baseForm) )
	{
		_DMESSAGE("GetExtraData: GetFormLocationData baseFormModInfo found");

		int numModsModifyingBase = GetNumberOfSourceFiles(baseForm);

		std::string baseFirstDefinedIn = GetFirstFormLocationName(baseForm);

		ExtraInfoEntry* baseDefinedIn;
		CreateExtraInfoEntry(baseDefinedIn, "Base defined in", baseFirstDefinedIn);

		formLocationHolder->PushBack(baseDefinedIn);

		std::string baseLastDefinedIn = GetLastFormLocationName(baseForm);

		ExtraInfoEntry* baseLastChangedBy;
		CreateExtraInfoEntry(baseLastChangedBy, "Base last modified by", baseLastDefinedIn);

		formLocationHolder->PushBack(baseLastChangedBy);

		ExtraInfoEntry* allModsTouchingBaseHolder;
		CreateExtraInfoEntry(allModsTouchingBaseHolder, "Base found in", "");

		GetModInfoData(allModsTouchingBaseHolder, baseForm, false);

		formLocationHolder->PushBack(allModsTouchingBaseHolder);
	}

	resultArray->PushBack(formLocationHolder);

	_DMESSAGE("GetExtraData: GetFormLocationData End");
}

void GetModInfoData(ExtraInfoEntry*& resultArray, RE::TESForm * form, boolean SkyrimESMNotDetectedBug)
{
	_DMESSAGE("GetExtraData: GetModInfoData start");

	int numMods = GetNumberOfSourceFiles(form);

	if (SkyrimESMNotDetectedBug)
	{
		ExtraInfoEntry* modEntry;

		CreateExtraInfoEntry(modEntry, "Mod", "Skyrim.esm");
		resultArray->PushBack(modEntry);
	}

	for (int i = 0; i < numMods; i++)
	{
		ExtraInfoEntry* modEntry;

		std::string modName = GetNthFormLocationName(form, i);

		CreateExtraInfoEntry(modEntry, "Mod", modName);
		resultArray->PushBack(modEntry);
	}

	_DMESSAGE("GetExtraData: GetModInfoData end");
}
