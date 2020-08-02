#include "TESObjectARMA.h"
#include "TESForm.h"
#include "MoreInformativeConsole/globals.h"
#include "MoreInformativeConsole/Util/NameUtil.h"

void GetArmaData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	_DMESSAGE("GetArmaData: GetArmaData Start");

	RE::TESObjectARMA* arma = static_cast<RE::TESObjectARMA*>(baseForm);

	if (arma)
	{
		//Get the skin textures if this is for a body part

		RE::BGSTextureSet* maleSkin = arma->skinTextures[RE::SEX::kMale];

		if (maleSkin)
		{
			ExtraInfoEntry* maleSkinEntry;
			CreateExtraInfoEntry(maleSkinEntry, "Male skin", "", priority_Arma_SkinMale);

			GetFormData(maleSkinEntry, maleSkin, nullptr);

			resultArray->PushBack(maleSkinEntry);
		}

		RE::BGSTextureSet* femaleSkin = arma->skinTextures[RE::SEX::kFemale];

		if (femaleSkin)
		{
			ExtraInfoEntry* femaleSkinEntry;
			CreateExtraInfoEntry(femaleSkinEntry, "Female skin", "", priority_Arma_SkinFemale);

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

			CreateExtraInfoEntry(defaultRacesEntry, "Primary Race", defaultRaceName, priority_Arma_PrimaryRace);

			GetFormData(defaultRacesEntry, defaultRace, nullptr);

			resultArray->PushBack(defaultRacesEntry);
		}

		//Get any additonal races this object covers
		int numberOfAdditionalRaces = arma->additionalRaces.size();

		if (numberOfAdditionalRaces > 0)
		{
			ExtraInfoEntry* additionalRacesEntry;

			CreateExtraInfoEntry(additionalRacesEntry, "Additional Races", "", priority_Arma_AddtionalRaces);

			for (int i = 0; i < numberOfAdditionalRaces; i++)
			{
				RE::TESRace* additionalRace = arma->additionalRaces[i];

				if (additionalRace)
				{
					std::string additionalRaceName = GetName(additionalRace);

					ExtraInfoEntry* additionalRaceEntry;

					CreateExtraInfoEntry(additionalRaceEntry, additionalRaceName, "");

					GetFormData(additionalRaceEntry, additionalRace, nullptr);

					additionalRacesEntry->PushBack(additionalRaceEntry);
				}
			}

			resultArray->PushBack(additionalRacesEntry);
		}

		//Now taht we are done with this armarture we no longer need to prevent reading additional race skins
		MICGlobals::readRaceSkins = true;
	}

	_DMESSAGE("GetArmaData: GetArmaData End");
}