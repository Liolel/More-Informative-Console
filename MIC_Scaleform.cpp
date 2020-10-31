#include "MIC_Scaleform.h"
#include "Scaleform/MICScaleform_GetReferenceInfo.h"
#include "Scaleform/MICScaleform_GetIniOptions.h"
#include "Scaleform/MICScaleform_RetrieveExtraData.h"
#include "Scaleform/MICScaleform_GetExtraData.h"
#include "Scaleform/MICScaleform_Log.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include <Windows.h>

MICScaleform_GetReferenceInfo* getReferenceInfo = nullptr;
MICScaleform_GetIniOptions* getIniOptions = nullptr;
MICScaleform_RetrieveExtraData* retrieveExtraData = nullptr;
MICScaleform_GetExtraData* getExtraData = nullptr;
MICScaleform_Log* MICScaleformlog = nullptr;

/*

	/*
	void StandardItemData(ExtraInfoEntry * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
	{
	if (!pForm || !pFxVal || !pFxVal->IsObject())
	return;

	switch (pForm->GetFormType())
	{

	break;

	case kFormType_SoulGem:
	{
	TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
	if (soulGem)
	{
	RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
	RegisterNumber(pFxVal, "soulSize", pEntry ? CALL_MEMBER_FN(pEntry, GetSoulLevel)() : soulGem->soulSize);
	}
	}
	break;

	case kFormType_Potion:
	{
	AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
	if (pAlchemy)
	{
	RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
	}
	}
	break;

	case kFormType_Book:
	{
	TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
	if (pBook)
	{
	RegisterNumber(pFxVal, "flags", pBook->data.flags);
	RegisterNumber(pFxVal, "bookType", pBook->data.type);
	switch (pBook->data.GetSanitizedType())
	{
	case TESObjectBOOK::Data::kType_Skill:
	RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
	break;

	case TESObjectBOOK::Data::kType_Spell:
	{
	double formID = -1;

	if (pBook->data.teaches.spell)
	formID = pBook->data.teaches.spell->formID;

	RegisterNumber(pFxVal, "teachesSpell", formID);
	}
	break;
	}
	}
	}
	break;

	default:
	break;
	}
	}*/
	/*

	void GetCellEntry(ExtraInfoEntry* resultArray, TESForm* pBaseForm)
	{
		DebugMessage("Starting GetCellEntry");

		TESObjectCELL* pCell = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCELL);
		if (pCell)
		{

			LightingCoordinates * lightningCoordinates = (LightingCoordinates*)(&(pCell->unk060));
			
			if (lightningCoordinates->coordinates)
			{
				
				Coordinates* coordinates = lightningCoordinates->coordinates;

				std::string coordinateString = IntToString(coordinates->cellX) + "," + IntToString(coordinates->cellY);

				ExtraInfoEntry* coordinateEntry;

				CreateExtraInfoEntry(coordinateEntry, "Coordinates", coordinateString);

				resultArray->PushBack(coordinateEntry);
			}

			BSExtraData * extraData = pCell->unk048.extraData;

			while (extraData)
			{
				int extraDataType = extraData->GetType();
				std::string extraDataString = GetExtraDataTypeName(extraDataType);

				ExtraInfoEntry* extraDataTypeEntry;

				CreateExtraInfoEntry(extraDataTypeEntry, "Extra Data", extraDataString);
				resultArray->PushBack(extraDataTypeEntry);

				extraData = extraData->next;
			}
			
		}
	}
};*/

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks( RE::GFxMovieView* a_view, RE::GFxValue* a_root )
{
	logger::debug("Install Hooks Called");

	if (getReferenceInfo == nullptr)
	{
		getReferenceInfo = new MICScaleform_GetReferenceInfo;
	}

	if (getIniOptions == nullptr)
	{
		getIniOptions = new MICScaleform_GetIniOptions;
	}

	if (retrieveExtraData == nullptr)
	{
		retrieveExtraData = new MICScaleform_RetrieveExtraData;
	}

	if (getExtraData == nullptr)
	{
		getExtraData = new MICScaleform_GetExtraData;
	}

	
	if (MICScaleformlog == nullptr)
	{
		MICScaleformlog = new MICScaleform_Log;
	}

	RE::GFxValue globals;

	logger::debug( a_view->GetMovieDef()->GetFileURL() );

	bool result = a_view->GetVariable(&globals, "_global");
	if (result)
	{
		//Create Object to hold new functions
		RE::GFxValue MIC;
		a_view->CreateObject(&MIC);

		RE::GFxValue	fnValue;
		a_view->CreateFunction(&fnValue, getReferenceInfo);
		MIC.SetMember("MICScaleform_GetReferenceInfo", fnValue);
		
		RE::GFxValue	fnValue2;
		a_view->CreateFunction(&fnValue2, getIniOptions);
		MIC.SetMember("MICScaleform_GetIniOptions", fnValue2);

		RE::GFxValue	fnValue3;
		a_view->CreateFunction(&fnValue3, retrieveExtraData);
		MIC.SetMember("MICScaleform_RetrieveExtraData", fnValue3);

		RE::GFxValue	fnValue4;
		a_view->CreateFunction(&fnValue4, getExtraData);
		MIC.SetMember("MICScaleform_GetExtraData", fnValue4);
		
		RE::GFxValue	fnValue5;
		a_view->CreateFunction(&fnValue5, MICScaleformlog);
		MIC.SetMember("MICScaleform_Log", fnValue5);

		//Store object with functions
		globals.SetMember("MIC", MIC);
	}

	return true;
}