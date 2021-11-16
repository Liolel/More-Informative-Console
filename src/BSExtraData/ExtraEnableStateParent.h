#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void ProcessEnableParentInformation(ExtraInfoEntry* resultArray, RE::BSExtraData* data);

const int flag_EnableStateOppositeParent = 1 << 0;
const int flag_PopIn = 1 << 1;