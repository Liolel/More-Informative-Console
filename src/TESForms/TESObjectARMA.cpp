#include "TESObjectARMA.h"
#include "TESForm.h"
#include "globals.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void GetArmaData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetArmaData: GetArmaData Start");

	RE::TESObjectARMA* arma = static_cast<RE::TESObjectARMA*>(baseForm);

	if (arma)
	{
		//Get the skin textures if this is for a body part

		RE::BGSTextureSet* maleSkin = arma->skinTextures[RE::SEX::kMale];

		if (maleSkin)
		{
			ExtraInfoEntry* maleSkinEntry;
			CreateExtraInfoEntry(maleSkinEntry, GetTranslation("$ArmaMaleSkin"), "", priority_Arma_SkinMale);

			GetFormData(maleSkinEntry, maleSkin, nullptr);

			resultArray->PushBack(maleSkinEntry);
		}

		RE::BGSTextureSet* femaleSkin = arma->skinTextures[RE::SEX::kFemale];

		if (femaleSkin)
		{
			ExtraInfoEntry* femaleSkinEntry;
			CreateExtraInfoEntry(femaleSkinEntry, GetTranslation("$ArmaFemaleSkin"), "", priority_Arma_SkinFemale);

			GetFormData(femaleSkinEntry, femaleSkin, nullptr);

			resultArray->PushBack(femaleSkinEntry);
		}

		//get the races this Arma is valid for

		MICGlobals::readRaceSkins = false; //disable reading race skin entries to avoid infinite loops
		
		//Get the primary race
		RE::TESRace* defaultRace = arma->race;

		if (defaultRace)
		{
			std::string defaultRaceName = GetName(defaultRace);

			ExtraInfoEntry* defaultRacesEntry;

			CreateExtraInfoEntry(defaultRacesEntry, GetTranslation("$ArmaPrimaryRace"), defaultRaceName, priority_Arma_PrimaryRace);

			GetFormData(defaultRacesEntry, defaultRace, nullptr);

			resultArray->PushBack(defaultRacesEntry);
		}

		//Get any additonal races this object covers
		int numberOfAdditionalRaces = arma->additionalRaces.size();

		if (numberOfAdditionalRaces > 0)
		{
			ExtraInfoEntry* additionalRacesEntry;

			CreateExtraInfoEntry(additionalRacesEntry, GetTranslation("$ArmaAdditonalRaces"), "", priority_Arma_AddtionalRaces);

			for (int i = 0; i < numberOfAdditionalRaces; i++)
			{
				RE::TESRace* additionalRace = arma->additionalRaces[i];

				if (additionalRace)
				{
					std::string additionalRaceName = GetName(additionalRace);

					ExtraInfoEntry* additionalRaceEntry;

					CreateExtraInfoEntry(additionalRaceEntry, additionalRaceName, "", priority_Default);

					GetFormData(additionalRaceEntry, additionalRace, nullptr);

					additionalRacesEntry->PushBack(additionalRaceEntry);
				}
			}

			resultArray->PushBack(additionalRacesEntry);
		}

		//Now taht we are done with this armarture we no longer need to prevent reading additional race skins
		MICGlobals::readRaceSkins = true;
	}

	logger::debug("GetArmaData: GetArmaData End");
}
