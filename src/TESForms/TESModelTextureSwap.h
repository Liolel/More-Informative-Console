#pragma once

#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"
#include "TESModel.h"

void GetModelTextures(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm);
void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModelTextureSwap* modelTextureSwap);
