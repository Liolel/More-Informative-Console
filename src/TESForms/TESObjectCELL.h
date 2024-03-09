#pragma once

#include "ExtraInfoEntry.h"

void GetCellEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);
std::string GetCoordinateString(RE::TESObjectCELL* cell);
void GetCurrentCellForWorldData(ExtraInfoEntry* resultArray, RE::PlayerCharacter* pc);
void GetLandscapeTextureAtReference(ExtraInfoEntry* resultArray, RE::PlayerCharacter* pc);
