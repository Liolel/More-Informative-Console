#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void GetEnchantmentInfomation(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);
void GetCharge(ExtraInfoEntry* resultArray, RE::ExtraDataList* extraList, RE::TESEnchantableForm* item, RE::ExtraEnchantment* extraEnchantment);
