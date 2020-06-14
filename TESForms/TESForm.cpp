#include "TESForm.h"
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

	if( GetHasSourceFileArray(form) )
	{
		UInt32 numberOfSourceFiles = form->sourceFiles.array->size();
		formName = GetNthFormLocationName( form, numberOfSourceFiles - 1 );
	}

	return formName;
}


//general wrapper for all get form methods
void GetFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	_DMESSAGE( ( "GetExtraData: Get Form Data Start " + GetFormTypeName((int)baseForm->formType) + " " + FormIDToString(baseForm->formID) ).c_str() );

	GetCommonFormData(resultArray, baseForm, refForm);
	/*
	if (pBaseForm != nullptr
		&& pBaseForm->GetFormType() == kFormType_NPC
		&& (pRefForm == nullptr
			|| pRefForm->GetFormType() == kFormType_Character))
	{
		DebugMessage("GetExtraData: Get Form Data character found");
		GetCharacterData(resultArray, pRefForm, pBaseForm);
	}

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
	}

	else if (pBaseForm->GetFormType() == kFormType_Race)
	{
		DebugMessage("GetExtraData: Get Form Data Race found");
		GetRaceEntry(resultArray, pBaseForm);
	}

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
		
		RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
		RE::TESNPC* rootNPC = npc->GetRootFaceNPC();
		baseForm = static_cast<RE::TESForm*>(rootNPC);
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
	CreateExtraInfoEntry(nameArray, "Name", name);

	resultArray->PushBack(nameArray);

	const char* editorID = baseForm->GetFormEditorID();

	if (editorID)
	{
		ExtraInfoEntry* editorIDEntry;
		CreateExtraInfoEntry(editorIDEntry, "Editor ID", editorID);
		resultArray->PushBack(editorIDEntry);
	}

	//base formid
	std::string formID = FormIDToString(baseForm->formID);

	ExtraInfoEntry* formIDArray;
	CreateExtraInfoEntry(formIDArray, "Base form ID", formID);
	resultArray->PushBack(formIDArray);

	//base form type
	std::string baseFormType = GetFormTypeName((int)baseForm->formType);

	ExtraInfoEntry* formTypeEntry;
	CreateExtraInfoEntry(formTypeEntry, "Base Type", baseFormType);
	resultArray->PushBack(formTypeEntry);

	//ref formid
	if (refForm != nullptr)
	{
		std::string refFormID = FormIDToString(refForm->formID);

		ExtraInfoEntry* formIDArray;
		CreateExtraInfoEntry(formIDArray, "Ref form ID", refFormID);
		resultArray->PushBack(formIDArray);
	}

	//mod location info

	/*
	DebugMessage("GetCommonFormData: Get Form Location Start");

	ExtraInfoEntry* formLocationHolder;

	CreateExtraInfoEntry(formLocationHolder, "Form location information", "");
	GetFormLocationData(formLocationHolder, pBaseForm, pRefForm);
	resultArray->PushBack(formLocationHolder);

	if (pRefForm != nullptr)
	{
		GetPositionData(resultArray, pRefForm);
	}
	*/

	_DMESSAGE("GetCommonFormData: GetCommonFormData End");
}