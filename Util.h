#pragma once

#include "RE/Skyrim.h"

namespace MICOptions
{
	extern boolean MICDebugMode; //Enables debugging code. Currently this just activates the logging features

	//extern int BackgroundColor; //Background color of the extra info window
	extern double Transparency; //Transperency of the extra info window background
	extern double Scale; //Scale of the extra info window
	extern int FieldsToDisplay; //Number of text fields to display

	extern int BaseInfoFormat; //How much information to display in the base info window
}


void DebugMessage(std::string message);

/*
std::string GetActorValueName(int id);
std::string GetEffectTypeName(int id);
std::string GetSpellTypeName(int spellType);
std::string GetCastingTypeName(int castingType);
std::string GetDeliveryTypeName(int deliveryType);
std::string GetTextureType(int textureType);


std::string GetName(RE::TESForm* pBaseForm);
int NumberOfItemInContainer(RE::TESForm * item, RE::TESContainer * container);
bool HasItem(RE::EntryDataList * inventory, RE::TESForm * item);
std::string GetFormTypeName(int formType);
std::string GetExtraDataTypeName(int extraDataType);
std::string GetArmorWeightClassName(int weightClass);
std::string GetEquipSlotName(int slot);
std::string GetWeaponAnimationTypeName(int weaponType);
std::string GetEquipTypeName(int formID);
std::string GetLockLevel(int lockLevel);
*/
std::string IntToString(int number);
/*
std::string FloatToString(float number);
std::string DoubleToString(double number);
std::string FormIDToString(int formID);
std::string BooleanToYesNoString(bool boolean);

int GetSmallestBitFlag(int flags);
std::string GetFileName(std::string filePath);

const int Constant_ModeConsoleHandle = 0;
const int Constant_ModeWorldInformation = 1;
const int Constant_ModeMFG = 2; */