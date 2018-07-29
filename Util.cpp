#pragma once

#include <sstream>
#include <vector>
#include <memory>
#include "GameObjects.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"

boolean printDebugMessages = true;

void DebugMessage(std::string message)
{
	if (printDebugMessages)
	{
		_MESSAGE(message.c_str());
	}
}


std::string GetActorValueName(int id)
{
	std::string actorValueName;

	switch (id)
	{
	case 0: actorValueName = "Aggression";
		break;
	case 1: actorValueName = "Confidence";
		break;
	case 2: actorValueName = "Energy";
		break;
	case 3: actorValueName = "Morality";
		break;
	case 4: actorValueName = "Mood";
		break;
	case 5: actorValueName = "Assistance";
		break;
	case 6: actorValueName = "OneHanded";
		break;
	case 7: actorValueName = "TwoHanded";
		break;
	case 8: actorValueName = "Marksman";
		break;
	case 9: actorValueName = "Block";
		break;
	case 10: actorValueName = "Smithing";
		break;
	case 11: actorValueName = "HeavyArmor";
		break;
	case 12: actorValueName = "LightArmor";
		break;
	case 13: actorValueName = "Pickpocket";
		break;
	case 14: actorValueName = "Lockpicking";
		break;
	case 15: actorValueName = "Sneak";
		break;
	case 16: actorValueName = "Alchemy";
		break;
	case 17: actorValueName = "Speechcraft";
		break;
	case 18: actorValueName = "Alteration";
		break;
	case 19: actorValueName = "Conjuration";
		break;
	case 20: actorValueName = "Destruction";
		break;
	case 21: actorValueName = "Illusion";
		break;
	case 22: actorValueName = "Restoration";
		break;
	case 23: actorValueName = "Enchanting";
		break;
	case 24: actorValueName = "Health";
		break;
	case 25: actorValueName = "Magicka";
		break;
	case 26: actorValueName = "Stamina";
		break;
	case 27: actorValueName = "HealRate";
		break;
	case 28: actorValueName = "MagickaRate";
		break;
	case 29: actorValueName = "StaminaRate";
		break;
	case 30: actorValueName = "SpeedMult";
		break;
	case 31: actorValueName = "InventoryWeight";
		break;
	case 32: actorValueName = "CarryWeight";
		break;
	case 33: actorValueName = "CritChance";
		break;
	case 34: actorValueName = "MeleeDamage";
		break;
	case 35: actorValueName = "UnarmedDamage";
		break;
	case 36: actorValueName = "Mass";
		break;
	case 37: actorValueName = "VoicePoints";
		break;
	case 38: actorValueName = "VoiceRate";
		break;
	case 39: actorValueName = "DamageResist";
		break;
	case 40: actorValueName = "PoisonResist";
		break;
	case 41: actorValueName = "FireResist";
		break;
	case 42: actorValueName = "ElectricResist";
		break;
	case 43: actorValueName = "FrostResist";
		break;
	case 44: actorValueName = "MagicResist";
		break;
	case 45: actorValueName = "DiseaseResist";
		break;
	case 46: actorValueName = "PerceptionCondition";
		break;
	case 47: actorValueName = "EnduranceCondition";
		break;
	case 48: actorValueName = "LeftAttackCondition";
		break;
	case 49: actorValueName = "RightAttackCondition";
		break;
	case 50: actorValueName = "LeftMobilityCondition";
		break;
	case 51: actorValueName = "RightMobilityCondition";
		break;
	case 52: actorValueName = "BrainCondition";
		break;
	case 53: actorValueName = "Paralysis";
		break;
	case 54: actorValueName = "Invisibility";
		break;
	case 55: actorValueName = "NightEye";
		break;
	case 56: actorValueName = "DetectLifeRange";
		break;
	case 57: actorValueName = "WaterBreathing";
		break;
	case 58: actorValueName = "WaterWalking";
		break;
	case 59: actorValueName = "IgnoreCrippledLimbs";
		break;
	case 60: actorValueName = "Fame";
		break;
	case 61: actorValueName = "Infamy";
		break;
	case 62: actorValueName = "JumpingBonus";
		break;
	case 63: actorValueName = "WardPower";
		break;
	case 64: actorValueName = "RightItemCharge/EquippedItemCharge";
		break;
	case 65: actorValueName = "ArmorPerks";
		break;
	case 66: actorValueName = "ShieldPerks";
		break;
	case 67: actorValueName = "WardDeflection";
		break;
	case 68: actorValueName = "Variable01";
		break;
	case 69: actorValueName = "Variable02";
		break;
	case 70: actorValueName = "Variable03";
		break;
	case 71: actorValueName = "Variable04";
		break;
	case 72: actorValueName = "Variable05";
		break;
	case 73: actorValueName = "Variable06";
		break;
	case 74: actorValueName = "Variable07";
		break;
	case 75: actorValueName = "Variable08";
		break;
	case 76: actorValueName = "Variable09";
		break;
	case 77: actorValueName = "Variable10";
		break;
	case 78: actorValueName = "BowSpeedBonus";
		break;
	case 79: actorValueName = "FavorActive";
		break;
	case 80: actorValueName = "FavorsPerDay";
		break;
	case 81: actorValueName = "FavorsPerDayTimer";
		break;
	case 82: actorValueName = "LeftItemCharge/EquippedStaffCharge";
		break;
	case 83: actorValueName = "AbsorbChance";
		break;
	case 84: actorValueName = "Blindness";
		break;
	case 85: actorValueName = "WeaponSpeedMult";
		break;
	case 86: actorValueName = "ShoutRecoveryMult";
		break;
	case 87: actorValueName = "BowStaggerBonus";
		break;
	case 88: actorValueName = "Telekinesis";
		break;
	case 89: actorValueName = "FavorPointsBonus";
		break;
	case 90: actorValueName = "LastBribedIntimidated";
		break;
	case 91: actorValueName = "LastFlattered";
		break;
	case 92: actorValueName = "MovementNoiseMult";
		break;
	case 93: actorValueName = "BypassVendorStolenCheck";
		break;
	case 94: actorValueName = "BypassVendorKeywordCheck";
		break;
	case 95: actorValueName = "WaitingForPlayer";
		break;
	case 96: actorValueName = "OneHandedMod";
		break;
	case 97: actorValueName = "TwoHandedMod";
		break;
	case 98: actorValueName = "MarksmanMod";
		break;
	case 99: actorValueName = "BlockMod";
		break;
	case 100: actorValueName = "SmithingMod";
		break;
	case 101: actorValueName = "HeavyArmorMod";
		break;
	case 102: actorValueName = "LightArmorMod";
		break;
	case 103: actorValueName = "PickPocketMod";
		break;
	case 104: actorValueName = "LockpickingMod";
		break;
	case 105: actorValueName = "SneakMod";
		break;
	case 106: actorValueName = "AlchemyMod";
		break;
	case 107: actorValueName = "SpeechcraftMod";
		break;
	case 108: actorValueName = "AlterationMod";
		break;
	case 109: actorValueName = "ConjurationMod";
		break;
	case 110: actorValueName = "DestructionMod";
		break;
	case 111: actorValueName = "IllusionMod";
		break;
	case 112: actorValueName = "RestorationMod";
		break;
	case 113: actorValueName = "EnchantingMod";
		break;
	case 114: actorValueName = "OneHandedSkillAdvance";
		break;
	case 115: actorValueName = "TwoHandedSkillAdvance";
		break;
	case 116: actorValueName = "MarksmanSkillAdvance";
		break;
	case 117: actorValueName = "BlockSkillAdvance";
		break;
	case 118: actorValueName = "SmithingSkillAdvance";
		break;
	case 119: actorValueName = "HeavyArmorSkillAdvance";
		break;
	case 120: actorValueName = "LightArmorSkillAdvance";
		break;
	case 121: actorValueName = "PickPocketSkillAdvance";
		break;
	case 122: actorValueName = "LockpickingSkillAdvance";
		break;
	case 123: actorValueName = "SneakSkillAdvance";
		break;
	case 124: actorValueName = "AlchemySkillAdvance";
		break;
	case 125: actorValueName = "SpeechcraftSkillAdvance";
		break;
	case 126: actorValueName = "AlterationSkillAdvance";
		break;
	case 127: actorValueName = "ConjurationSkillAdvance";
		break;
	case 128: actorValueName = "DestructionSkillAdvance";
		break;
	case 129: actorValueName = "IllusionSkillAdvance";
		break;
	case 130: actorValueName = "RestorationSkillAdvance";
		break;
	case 131: actorValueName = "EnchantingSkillAdvance";
		break;
	case 132: actorValueName = "LeftWeaponSpeedMult";
		break;
	case 133: actorValueName = "DragonSouls";
		break;
	case 134: actorValueName = "CombatHealthRegenMult";
		break;
	case 135: actorValueName = "OneHandedPowerMod";
		break;
	case 136: actorValueName = "TwoHandedPowerMod";
		break;
	case 137: actorValueName = "MarksmanPowerMod";
		break;
	case 138: actorValueName = "BlockPowerMod";
		break;
	case 139: actorValueName = "SmithingPowerMod";
		break;
	case 140: actorValueName = "HeavyArmorPowerMod";
		break;
	case 141: actorValueName = "LightArmorPowerMod";
		break;
	case 142: actorValueName = "PickPocketPowerMod";
		break;
	case 143: actorValueName = "LockpickingPowerMod";
		break;
	case 144: actorValueName = "SneakPowerMod";
		break;
	case 145: actorValueName = "AlchemyPowerMod";
		break;
	case 146: actorValueName = "SpeechcraftPowerMod";
		break;
	case 147: actorValueName = "AlterationPowerMod";
		break;
	case 148: actorValueName = "ConjurationPowerMod";
		break;
	case 149: actorValueName = "DestructionPowerMod";
		break;
	case 150: actorValueName = "IllusionPowerMod";
		break;
	case 151: actorValueName = "RestorationPowerMod";
		break;
	case 152: actorValueName = "EnchantingPowerMod";
		break;
	case 153: actorValueName = "DragonRend";
		break;
	case 154: actorValueName = "AttackDamageMult";
		break;
	case 155: actorValueName = "HealRateMult/CombatHealthRegenMultMod";
		break;
	case 156: actorValueName = "MagickaRateMult/CombatHealthRegenMultPowerMod";
		break;
	case 157: actorValueName = "StaminaRateMult";
		break;
	case 158: actorValueName = "WerewolfPerks";
		break;
	case 159: actorValueName = "VampirePerks";
		break;
	case 160: actorValueName = "GrabActorOffset";
		break;
	case 161: actorValueName = "Grabbed";
		break;
	case 162: actorValueName = "DEPRECATED05";
		break;
	case 163: actorValueName = "ReflectDamage";
		break;
	default: actorValueName = "unknown";
		break;
	}

	return actorValueName;
}


std::string GetEffectTypeName(int id)
{
	std::string effectTypeName;

	switch (id)
	{
	case 0: effectTypeName = "Value Mod";
		break;
	case 1: effectTypeName = "Script";
		break;
	case 2: effectTypeName = "Dispel";
		break;
	case 3: effectTypeName = "Cure Disease";
		break;
	case 4: effectTypeName = "Absorb";
		break;
	case 5: effectTypeName = "Dual Value Mod";
		break;
	case 6: effectTypeName = "Calm";
		break;
	case 7: effectTypeName = "Demoralize";
		break;
	case 8: effectTypeName = "Frenzy";
		break;
	case 9: effectTypeName = "Disarm";
		break;
	case 10: effectTypeName = "Command Summoned";
		break;
	case 11: effectTypeName = "Invisibility";
		break;
	case 12: effectTypeName = "Light";
		break;
	case 15: effectTypeName = "Lock";
		break;
	case 16: effectTypeName = "Open";
		break;
	case 17: effectTypeName = "Bound Weapon";
		break;
	case 18: effectTypeName = "Summon Creature";
		break;
	case 19: effectTypeName = "Detect Life";
		break;
	case 20: effectTypeName = "Telekinesis";
		break;
	case 21: effectTypeName = "Paralysis";
		break;
	case 22: effectTypeName = "Reanimate";
		break;
	case 23: effectTypeName = "Soul Trap";
		break;
	case 24: effectTypeName = "Turn Undead";
		break;
	case 25: effectTypeName = "Guide";
		break;
	case 26: effectTypeName = "Werewolf Feed";
		break;
	case 27: effectTypeName = "Cure Paralysis";
		break;
	case 28: effectTypeName = "Cure Addiction";
		break;
	case 29: effectTypeName = "Cure Poison";
		break;
	case 30: effectTypeName = "Concussion";
		break;
	case 31: effectTypeName = "Value and Parts";
		break;
	case 32: effectTypeName = "Accumulate Magnitude";
		break;
	case 33: effectTypeName = "Stagger";
		break;
	case 34: effectTypeName = "Peak Value Mod";
		break;
	case 35: effectTypeName = "Cloak";
		break;
	case 36: effectTypeName = "Werewolf";
		break;
	case 37: effectTypeName = "Slow Time";
		break;
	case 38: effectTypeName = "Rally";
		break;
	case 39: effectTypeName = "Enchance Weapon";
		break;
	case 40: effectTypeName = "Spawn Hazard";
		break;
	case 41: effectTypeName = "Etherealize";
		break;
	case 42: effectTypeName = "Banish";
		break;
	case 43: effectTypeName = "Spawn Scripted Ref";
		break;
	case 44: effectTypeName = "Disguise";
		break;
	case 45: effectTypeName = "Grab Actor";
		break;
	case 46: effectTypeName = "Vampire Lord";
		break;
	default: effectTypeName = "Unknown";
		break;
	}

	return effectTypeName;
}

std::string GetSpellTypeName(int spellType)
{
	std::string spellTypeName = "";

	switch (spellType)
	{
	case SpellItem::kTypeSpell:	spellTypeName = "Spell";
		break;
	case SpellItem::kTypeDisease: spellTypeName = "Disease";
		break;
	case SpellItem::kTypePower:	spellTypeName = "Power";
		break;
	case SpellItem::kTypeLesserPower: spellTypeName = "Lesser Power";
		break;
	case SpellItem::kTypeAbility: spellTypeName = "Ability";
		break;
	case SpellItem::kTypePoison: spellTypeName = "Poison";
		break;
	case SpellItem::kTypeAddition: spellTypeName = "Addiction";
		break;
	case SpellItem::kTypeVoice: spellTypeName = "Voice";
		break;
	default: spellTypeName = "Unknown type";
		break;
	}

	return spellTypeName;
}

std::string GetCastingTypeName(int castingType)
{
	std::string castingTypeName = "";

	switch (castingType)
	{
	case EffectSetting::Properties::kCastingType_Concentration: castingTypeName = "Concentration";
		break;
	case EffectSetting::Properties::kCastingType_ConstantEffect: castingTypeName = "Constant Effect";
		break;
	case EffectSetting::Properties::kCastingType_FireAndForget: castingTypeName = "Fire and Forget";
		break;
	default: castingTypeName = "Unknown type";
		break;
	}

	return castingTypeName;
}

std::string GetDeliveryTypeName(int deliveryType)
{
	std::string deliveryTypeName = "";

	switch (deliveryType)
	{
	case EffectSetting::Properties::kDeliveryType_Aimed: deliveryTypeName = "Aimed";
		break;
	case EffectSetting::Properties::kDeliveryType_Contact: deliveryTypeName = "Contact";
		break;
	case EffectSetting::Properties::kDeliveryType_Self: deliveryTypeName = "Self";
		break;
	case EffectSetting::Properties::kDeliveryType_TargetActor: deliveryTypeName = "Target Actor";
		break;
	case EffectSetting::Properties::kDeliveryType_TargetLocation: deliveryTypeName = "Target Location";
		break;
	default: deliveryTypeName = "Unknown type";
		break;
	}

	return deliveryTypeName;
}


std::string GetName(TESForm* pBaseForm)
{
	DebugMessage("GetExtraData: GetName Start");

	std::string name = "";

	switch (pBaseForm->GetFormType())
	{
		case kFormType_NPC:
		{
			DebugMessage("GetExtraData: GetName NPC");

			TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);
			if (pNPC)
			{
				if (pNPC->fullName.name.data)
				{
					name = pNPC->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_EffectSetting:
		{
			DebugMessage("GetExtraData: GetName Magic Effect");

			EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
			if (pEffectSetting)
			{
				if (pEffectSetting->fullName.name.data)
				{
					name = pEffectSetting->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Spell:
		case kFormType_ScrollItem:
		case kFormType_Ingredient:
		case kFormType_Potion:
		case kFormType_Enchantment:
		{
			DebugMessage("GetExtraData: GetName Spell");

			MagicItem * pMagicItem = DYNAMIC_CAST(pBaseForm, TESForm, MagicItem);
			if (pMagicItem)
			{
				if (pMagicItem->fullName.name.data)
				{
					name = pMagicItem->fullName.name.data;
				}
			}


			break;
		}
		case kFormType_Armor:
		{
			DebugMessage("GetExtraData: GetName Armor");
			TESObjectARMO * pArmor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMO);

			if (pArmor)
			{
				if (pArmor->fullName.name.data)
				{
					name = pArmor->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Ammo:
		{
			DebugMessage("GetExtraData: GetName Ammo");
			TESAmmo * pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
			if (pAmmo)
			{
				if (pAmmo->fullName.name.data)
				{
					name = pAmmo->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Weapon:
		{
			TESObjectWEAP * pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
			DebugMessage("GetExtraData: GetName Weapon");
			if (pWeapon)
			{
				if (pWeapon->fullName.name.data)
				{
					name = pWeapon->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_SoulGem:
		{
			TESSoulGem * pSoul = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
			DebugMessage("GetExtraData: GetName Soul Gem");
			if (pSoul)
			{
				if (pSoul->fullName.name.data)
				{
					name = pSoul->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Book:
		{
			TESObjectBOOK * pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
			DebugMessage("GetExtraData: GetName Book");
			if (pBook)
			{
				if (pBook->fullName.name.data)
				{
					name = pBook->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Misc:
		{
			TESObjectMISC * pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
			DebugMessage("GetExtraData: GetName Misc Item");
			if (pMisc)
			{
				if (pMisc->fullName.name.data)
				{
					name = pMisc->fullName.name.data;
				}
			}

			break;
		}

		case kFormType_Key:
		{
			TESKey * pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
			DebugMessage("GetExtraData: GetName Key Item");
			if (pKey)
			{
				if (pKey->fullName.name.data)
				{
					name = pKey->fullName.name.data;
				}
			}

			break;
		}
		
	}

	DebugMessage("GetExtraData: GetName End");

	return name;
}

//returns the total amount the given item stored in the given container
int NumberOfItemInContainer(TESForm * item, TESContainer * container)
{
	int numberOfItemInContainer = 0;

	for (int i = 0; i < container->numEntries; i++)
	{
		TESForm *itemForm = container->entries[i]->form;

		if (itemForm == item)
		{
			numberOfItemInContainer += container->entries[i]->count;
		}
	}

	return numberOfItemInContainer;
}

//returns true if the given item is present in the EntryDataList
bool HasItem(EntryDataList * inventory, TESForm * item)
{
	bool hasItem = false;

	for (EntryDataList::Iterator it = inventory->Begin(); !it.End(); ++it)
	{
		InventoryEntryData * e = it.Get();
		if (e && e->type == item)
		{
			hasItem = true;
		}
	}

	return hasItem;
}

std::string IntToString(int number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

std::string FloatToString(float number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

std::string DoubleToString(double number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

std::string FormIDToString(int formID)
{
	std::ostringstream ss;
	std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);
	sprintf_s(sResult.get(), MAX_PATH, "%08X", formID);
	return sResult.get();
}