#pragma once

#include "RE/Skyrim.h"

//void DebugMessage(std::string message);

/*
std::string GetActorValueName(int id);
std::string GetEffectTypeName(int id);
std::string GetSpellTypeName(int spellType);
std::string GetCastingTypeName(int castingType);
std::string GetDeliveryTypeName(int deliveryType);
std::string GetTextureType(int textureType);

int NumberOfItemInContainer(RE::TESForm * item, RE::TESContainer * container);
bool HasItem(RE::EntryDataList * inventory, RE::TESForm * item);
std::string GetExtraDataTypeName(int extraDataType);
std::string GetArmorWeightClassName(int weightClass);
std::string GetEquipSlotName(int slot);
std::string GetWeaponAnimationTypeName(int weaponType);
std::string GetEquipTypeName(int formID);
std::string GetLockLevel(int lockLevel);
*/
//Convert various data types to strings to display in UI
std::string IntToString(int number);

std::string FloatToString(float number);
/*std::string DoubleToString(double number);*/
std::string FormIDToString(int formID);
/*
std::string BooleanToYesNoString(bool boolean);

int GetSmallestBitFlag(int flags);
std::string GetFileName(std::string filePath);
*/
