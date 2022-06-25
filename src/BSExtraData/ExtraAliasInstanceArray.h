#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void ProcessExtraAliasInstanceArray(ExtraInfoEntry* resultArray, RE::BSExtraData* data);
void GetAliasInformation(ExtraInfoEntry* resultArray, const RE::BGSBaseAlias* alias, bool getOwningQuest );
