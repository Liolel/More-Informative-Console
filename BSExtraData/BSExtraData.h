#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"

void GetBSExtraData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm);
void ProcessExtraDataList(ExtraInfoEntry* resultArray, RE::ExtraDataList*, RE::TESObjectREFR* refForm);
