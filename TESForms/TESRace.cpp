#include "TESRace.h"
#include "TESModel.h"
#include "TESForm.h"
#include "MoreInformativeConsole/globals.h"
#include "MoreInformativeConsole/Util/NameUtil.h"

void GetRaceEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	_DMESSAGE("Starting GetRaceEntry");

	RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
	if (race)
	{		
		/* Currently this is covered by the GetCommonData method
		//editor ID
		std::string editorID = race->GetFormEditorID();

		ExtraInfoEntry* editorIDEntry;

		CreateExtraInfoEntry(editorIDEntry, "EditorID", editorID, priority_EditorID );
		resultArray->PushBack(editorIDEntry);
		*/
		
		_DMESSAGE("Getting Models");
		//models
		RE::TESModel* maleModelSkeleton = &(race->skeletonModels[RE::SEXES::kMale]);
		RE::TESModel *femaleModelSkelelton = &(race->skeletonModels[RE::SEXES::kFemale]);

		AddModelEntry(resultArray, "Male Skeleton", maleModelSkeleton, priority_Race_SkeletonMale);
		AddModelEntry(resultArray, "Female Skeleton", femaleModelSkelelton, priority_Race_SkeletonFemale);
		

		//Get the skin for this race
		//We need to check that MICGlobals::readRaceSkins is true, because the armor object will look at the Armature objects which will look at the races that the Armature applies too.
		if (MICGlobals::readRaceSkins
			&& race->skin != nullptr)
		{
			_DMESSAGE("Getting Skin");
			RE::TESObjectARMO* skin = race->skin;
			std::string skinName = GetName(skin);

			ExtraInfoEntry* skinEntry;
			CreateExtraInfoEntry(skinEntry, "Skin", skinName);
			GetFormData(skinEntry, skin, nullptr);

			resultArray->PushBack(skinEntry);

			_DMESSAGE("Done Getting Skin");
		}

		/*
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