#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"

void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModel* model, priority priority = priority_Model );