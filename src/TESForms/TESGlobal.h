#pragma once
#include "ExtraInfoEntry.h"
#include "RE/Skyrim.h"
#include "TranslationCache.h"

void GetGlobalInformation(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);

float GetGlobalValue(RE::TESForm* baseForm);
