#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"


const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminaIndex = 26;
const int playerBaseFormID = 0x7;

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm);
void GetCharacterData(ExtraInfoEntry* resultArray, RE::TESForm* refForm, RE::TESForm* baseForm);