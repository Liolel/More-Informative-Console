#include "TESRace.h"
#include "TESForm.h"
#include "TESModel.h"
#include "Util/NameUtil.h"
#include "globals.h"
#include "TranslationCache.h"
#include <Util/GeneralUtil.h>

//4-30-2022: Checked for translations needed

void GetRaceEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("Starting GetRaceEntry");

	RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
	if (race) 
	{

		logger::debug("Getting Models");
		//models
		RE::TESModel* maleModelSkeleton = &(race->skeletonModels[RE::SEXES::kMale]);
		RE::TESModel* femaleModelSkelelton = &(race->skeletonModels[RE::SEXES::kFemale]);

		AddModelEntry(resultArray, GetTranslation("$RaceMaleSkeleton"), maleModelSkeleton, priority_Race_SkeletonMale);
		AddModelEntry(resultArray, GetTranslation("$RaceFemaleSkeleton"), femaleModelSkelelton, priority_Race_SkeletonFemale);

		//Get the skin for this race
		//We need to check that MICGlobals::readRaceSkins is true, because the armor object will look at the Armature objects which will look at the races that the Armature applies too.
		if (MICGlobals::readRaceSkins && race->skin != nullptr) {
			logger::debug("Getting Skin");
			RE::TESObjectARMO* skin = race->skin;
			std::string skinName = GetName(skin);

			ExtraInfoEntry* skinEntry;
			CreateExtraInfoEntry(skinEntry, GetTranslation("$RaceSkin"), skinName, priority_Race_Skin);
			GetFormData(skinEntry, skin, nullptr);

			resultArray->PushBack(skinEntry);

			logger::debug("Done Getting Skin");
		}

		//Handle Flags
		int flags = race->data.flags.underlying();

		ExtraInfoEntry* playableEntry;
		std::string isPlayable = BooleanToYesNoString(HasFlag(flags, (int)RE::RACE_DATA::Flag::kPlayable));
		CreateExtraInfoEntry(playableEntry, GetTranslation("$RaceSkin"), isPlayable, priority_Race_Playable);
		resultArray->PushBack(playableEntry);

		ExtraInfoEntry* childEntry;
		std::string isChild = BooleanToYesNoString(HasFlag(flags, (int)RE::RACE_DATA::Flag::kChild));
		CreateExtraInfoEntry(childEntry, GetTranslation("$RaceChild"), isChild, priority_Race_Child);
		resultArray->PushBack(childEntry);
	}

	logger::debug("Ending GetRaceEntry");
}
