#pragma once

#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"
#include "TESModel.h"

void GetModelTextures(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);
void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModelTextureSwap* modelTextureSwap);
