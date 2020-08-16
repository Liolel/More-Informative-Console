#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"

void ProcessContainerChanges(ExtraInfoEntry* resultArray, RE::BSExtraData* data, RE::TESObjectREFR* refForm);
void GetEquipment(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::Actor* actor);
RE::TESForm* FindEquipedItemInSlot(int slotMask, RE::ExtraContainerChanges* containerChanges);
