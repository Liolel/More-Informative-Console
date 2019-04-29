#pragma once

#include "GameObjects.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"

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

std::string GetActorValueName(int id);
std::string GetEffectTypeName(int id);
std::string GetSpellTypeName(int spellType);
std::string GetCastingTypeName(int castingType);
std::string GetDeliveryTypeName(int deliveryType);
std::string GetTextureType(int textureType);


std::string GetName(TESForm* pBaseForm);
int NumberOfItemInContainer(TESForm * item, TESContainer * container);
bool HasItem(EntryDataList * inventory, TESForm * item);
std::string GetFormTypeName(int formType);
std::string GetArmorWeightClassName(int weightClass);
std::string GetEquipSlotName(int slot);
std::string GetWeaponAnimationTypeName(int weaponType);
std::string GetEquipTypeName(int formID);

std::string IntToString(int number);
std::string FloatToString(float number);
std::string DoubleToString(double number);
std::string FormIDToString(int formID);

int GetSmallestBitFlag(int flags);
std::string GetFileName(std::string filePath);