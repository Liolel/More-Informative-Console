#include "TESRace.h"

void GetRaceEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	_DMESSAGE("Starting GetRaceEntry");

	RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
	if (race)
	{
		_DMESSAGE("Getting Editor ID");
		/*
		//editor ID
		std::string editorID = pRace->editorId.Get();

		ExtraInfoEntry* editorIDEntry;

		CreateExtraInfoEntry(editorIDEntry, "EditorID", editorID);
		resultArray->PushBack(editorIDEntry);


		DebugMessage("Getting Models");
		//models
		TESModel* maleModel = &(pRace->models[0]);
		TESModel* femaleModel = &(pRace->models[1]);

		AddModelEntry(resultArray, "Male Skeleton", maleModel);
		AddModelEntry(resultArray, "Female Skeleton", femaleModel);

		//Skins
		if (MICGlobals::readRaceSkins
			&& pRace->skin.skin != nullptr)
		{
			DebugMessage("Getting Skin");
			TESObjectARMO* skin = pRace->skin.skin;

			std::string skinName = GetName(skin);

			ExtraInfoEntry* skinEntry;

			CreateExtraInfoEntry(skinEntry, "Skin", skinName);

			GetFormData(skinEntry, skin, nullptr);

			resultArray->PushBack(skinEntry);

			DebugMessage("Done Getting Skin");
		}

		//Handle Flags
		int playableFlag = 0x00000001;
		int childFlag = 0x00000004;

		ExtraInfoEntry* playableEntry;

		if ((pRace->data.raceFlags & playableFlag) == playableFlag)
		{
			CreateExtraInfoEntry(playableEntry, "Playable", "Yes");
		}

		else
		{
			CreateExtraInfoEntry(playableEntry, "Playable", "No");
		}

		resultArray->PushBack(playableEntry);

		ExtraInfoEntry* childEntry;

		if ((pRace->data.raceFlags & childFlag) == childFlag)
		{
			CreateExtraInfoEntry(childEntry, "Child", "Yes");
		}

		else
		{
			CreateExtraInfoEntry(childEntry, "Child", "No");
		}

		resultArray->PushBack(childEntry);*/
	}

	_DMESSAGE("Ending GetRaceEntry");
}