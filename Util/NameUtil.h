#pragma once
#include "RE/Skyrim.h"

//Methods for converting various form data into user readable strings

std::string GetName(RE::TESForm* pBaseForm);
std::string GetFormTypeName(int formType);
std::string GetTextureType(int textureType);

std::string GetArmorTypeName(RE::TESObjectARMO::ArmorType armorType);
std::string GetEquipSlotName(int slot);
std::string GetWeaponAnimationTypeName(RE::WEAPON_TYPE weaponType);
std::string GetEquipTypeName(int formID);

std::string GetActorValueName(int id);
std::string GetEffectTypeName(int id);
std::string GetSpellTypeName(RE::MagicSystem::SpellType spellType);
std::string GetCastingTypeName(RE::MagicSystem::CastingType castingType);
std::string GetDeliveryTypeName(RE::MagicSystem::Delivery deliveryType);

std::string GetExtraDataTypeName(int extraDataType);
std::string GetLockLevelName(RE::LOCK_LEVEL lockLevel);

std::string GetMFGExpressionName(int expressionIndex);
std::string GetMFGModiferName(int modifierIndex);
std::string GetMFGPhenomeName(int phonemeIndex);