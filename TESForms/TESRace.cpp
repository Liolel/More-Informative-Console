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
			CreateExtraInfoEntry(skinEntry, "Skin", skinName, priority_Race_Skin);
			GetFormData(skinEntry, skin, nullptr);

			resultArray->PushBack(skinEntry);

			_DMESSAGE("Done Getting Skin");
		}

		
		//Handle Flags
		UInt32 flags = (UInt32)race->data.flags;

		ExtraInfoEntry* playableEntry;
		std::string isPlayable = BooleanToYesNoString(HasFlag(flags, (int)RE::RACE_DATA::Flag::kPlayable));
		CreateExtraInfoEntry(playableEntry, "Playable", isPlayable, priority_Race_Playable);
		resultArray->PushBack(playableEntry);

		ExtraInfoEntry* childEntry;
		std::string isChild = BooleanToYesNoString(HasFlag(flags, (int)RE::RACE_DATA::Flag::kChild));
		CreateExtraInfoEntry(childEntry, "Child", isChild, priority_Race_Child);
		resultArray->PushBack(childEntry);
	}

	_DMESSAGE("Ending GetRaceEntry");
}