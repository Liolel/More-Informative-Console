#pragma once

void DebugMessage(std::string message);

std::string GetActorValueName(int id);
std::string GetEffectTypeName(int id);
std::string GetSpellTypeName(int spellType);
std::string GetCastingTypeName(int castingType);
std::string GetDeliveryTypeName(int deliveryType);

std::string GetName(TESForm* pBaseForm);
int NumberOfItemInContainer(TESForm * item, TESContainer * container);
bool HasItem(EntryDataList * inventory, TESForm * item);


std::string IntToString(int number);
std::string FloatToString(float number);
std::string DoubleToString(double number);
std::string FormIDToString(int formID);
