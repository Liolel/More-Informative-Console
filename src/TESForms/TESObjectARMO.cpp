#include "TESObjectARMO.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"
#include "globals.h"

//4-30-2022: Checked for translations needed

void GetArmorData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("Starting GetArmorData");

	RE::TESObjectARMO* armor = static_cast<RE::TESObjectARMO*>(baseForm);

	if (armor) {
		//armor rating
		int armorRating = armor->armorRating / 100;  //The armor rating in memory is 100 times the value shown to the user for some reason so we need to convert

		ExtraInfoEntry* armorRatingEntry;
		CreateExtraInfoEntry(armorRatingEntry, GetTranslation("$ArmorRating"), IntToString(armorRating), priority_Armor_ArmorRating);
		resultArray->PushBack( armorRatingEntry );

		//value
		int value = armor->value;

		ExtraInfoEntry* valueEntry;
		CreateExtraInfoEntry(valueEntry, GetTranslation("$Value"), IntToString(value), priority_Armor_Value );
		resultArray->PushBack( valueEntry );

		//weight
		float weight = armor->weight;

		ExtraInfoEntry* weightEntry;
		CreateExtraInfoEntry(weightEntry, GetTranslation("$Weight"), FloatToString(weight), priority_Armor_Weight);
		resultArray->PushBack(weightEntry);

		//armor type
		RE::TESObjectARMO::ArmorType armorType = armor->GetArmorType();

		ExtraInfoEntry* weightClassEntry;
		CreateExtraInfoEntry(weightClassEntry, GetTranslation("$ArmorType"), GetArmorTypeName(armorType), priority_Armor_ArmorType);
		resultArray->PushBack(weightClassEntry);

		//Equip slots
		int parts = (int)armor->GetSlotMask();

		ExtraInfoEntry* equipSlotsEntry;
		CreateExtraInfoEntry(equipSlotsEntry, GetTranslation("$EquipSlots"), "", priority_Armor_EquipSlots);

		//The equip slots are stored in a mask, so extract each individual bit to check if a slot is used
		for (int i = 0; i <= 31; i++) {
			int mask = 1 << i;

			if ((parts & mask) == mask) {
				std::string slotName = GetEquipSlotName(i);

				ExtraInfoEntry* equipSlotEntry;

				CreateExtraInfoEntry(equipSlotEntry, slotName, "", priority_Default);
				equipSlotsEntry->PushBack(equipSlotEntry);
			}
		}

		resultArray->PushBack(equipSlotsEntry);

		//Armature
		ExtraInfoEntry* armorAddonsEntry;
		CreateExtraInfoEntry(armorAddonsEntry, GetTranslation("$Arma"), "", priority_Armor_ArmorAddons);

		int numberOfArmorAddons = armor->armorAddons.size();

		for (int i = 0; i < numberOfArmorAddons; i++) {
			RE::TESObjectARMA* arma = armor->armorAddons[i];
			bool addEntry = true;

			//if we are filtering by race skip all addons the race is not valid for
			if (MICGlobals::filterARMAByRace != nullptr) {
				addEntry = arma->IsValidRace(MICGlobals::filterARMAByRace);
			}

			if (addEntry) {
				ExtraInfoEntry* armorAddonEntry;
				std::string armorAddonName = GetName(arma);
				std::string raceName = "";
			
				if ( arma->race )
				{
					raceName = GetName(arma->race);
				}

				CreateExtraInfoEntry(armorAddonEntry, armorAddonName, raceName, priority_Default);
				GetFormData(armorAddonEntry, arma, nullptr);
				armorAddonsEntry->PushBack(armorAddonEntry);
			}
		}

		resultArray->PushBack(armorAddonsEntry);
	}

	logger::debug("Ending GetArmorData");
}
