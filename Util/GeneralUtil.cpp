#pragma once

#include "GeneralUtil.h"
#include <sstream>
#include <vector>
#include <memory>

//MIC options


/*


std::vector<std::string> ExtraDataTypes =
{
"NONE",
"kExtraData_Havok",
"kExtraData_Cell3D",
"kExtraData_CellWaterType",
"kExtraData_RegionList",
"kExtraData_SeenData",
"kExtraData_EditorID",
"kExtraData_CellMusicType",
"kExtraData_CellSkyRegion",
"kExtraData_ProcessMiddleLow",
"kExtraData_DetachTime",
"kExtraData_PersistentCell",
"unknown12",
"kExtraData_Action",
"kExtraData_StartingPosition",
"unknown15",
"kExtraData_AnimGraphManager",
"unknown17",
"kExtraData_UsedMarkers",
"kExtraData_DistantData",
"kExtraData_RagDollData",
"kExtraData_ContainerChanges",
"kExtraData_Worn",
"kExtraData_WornLeft",
"kExtraData_PackageStartLocation",
"kExtraData_Package",
"kExtraData_TresPassPackage",
"kExtraData_RunOncePacks",
"kExtraData_ReferenceHandle",
"kExtraData_Follower",
"kExtraData_LevCreaModifier",
"kExtraData_Ghost",
"kExtraData_OriginalReference",
"kExtraData_Ownership",
"kExtraData_Global",
"kExtraData_Rank",
"kExtraData_Count",
"kExtraData_Health",
"unknown38",
"kExtraData_TimeLeft",
"kExtraData_Charge",
"kExtraData_Light",
"kExtraData_Lock",
"kExtraData_Teleport",
"kExtraData_MapMarker",
"kExtraData_LeveledCreature",
"kExtraData_LeveledItem",
"kExtraData_Scale",
"kExtraData_Seed",
"kExtraData_MagicCaster",
"unknown50",
"unknown51",
"kExtraData_PlayerCrimeList",
"unknown53",
"kExtraData_EnableStateParent",
"kExtraData_EnableStateChildren",
"kExtraData_ItemDropper",
"kExtraData_DroppedItemList",
"kExtraData_RandomTeleportMarker",
"unknown59",
"kExtraData_SavedHavokData",
"kExtraData_CannotWear",
"kExtraData_Poison",
"unknown63",
"kExtraData_LastFinishedSequence",
"kExtraData_SavedAnimation",
"kExtraData_NorthRotation",
"kExtraData_SpawnContainer",
"kExtraData_FriendHits",
"kExtraData_HeadingTarget",
"unknown70",
"kExtraData_RefractionProperty",
"kExtraData_StartingWorldOrCell",
"kExtraData_Hotkey",
"unknown74",
"kExtraData_EditiorRefMoveData",
"kExtraData_InfoGeneralTopic",
"kExtraData_HasNoRumors",
"kExtraData_Sound",
"kExtraData_TerminalState",
"kExtraData_LinkedRef",
"kExtraData_LinkedRefChildren",
"kExtraData_ActivateRef",
"kExtraData_ActivateRefChildren",
"kExtraData_CanTalkToPlayer",
"kExtraData_ObjectHealth",
"kExtraData_CellImageSpace",
"kExtraData_NavMeshPortal",
"kExtraData_ModelSwap",
"kExtraData_Radius",
"unknown90",
"kExtraData_FactionChanges",
"kExtraData_DismemberedLimbs",
"kExtraData_ActorCause",
"kExtraData_MultiBound",
"kExtraData_MultiBoundData",
"kExtraData_MultiBoundRef",
"kExtraData_ReflectedRefs",
"kExtraData_ReflectorRefs",
"kExtraData_EmittanceSource",
"kExtraData_RadioData",
"kExtraData_CombatStyle",
"unknown102",
"kExtraData_Primitive",
"kExtraData_OpenCloseActivateRef",
"kExtraData_AnimNoteReceiver",
"kExtraData_Ammo",
"kExtraData_PatrolRefData",
"kExtraData_PackageData",
"kExtraData_OcclusionShape",
"kExtraData_CollisionData",
"kExtraData_SayTopicInfoOnceADay",
"kExtraData_EncounterZone",
"kExtraData_SayTopicInfo",
"kExtraData_OcclusionPlaneRefData",
"kExtraData_PortalRefData",
"kExtraData_Portal",
"kExtraData_Room",
"kExtraData_HealthPerc",
"kExtraData_RoomRefData",
"kExtraData_GuardedRefData",
"kExtraData_CreatureAwakeSound",
"unknown122",
"kExtraData_Horse",
"kExtraData_IgnoredBySandbox",
"kExtraData_CellAcousticSpace",
"kExtraData_ReservedMarkers",
"kExtraData_WeaponIdleSound",
"kExtraData_WaterLightRefs",
"kExtraData_LitWaterRefs",
"kExtraData_WeaponAttackSound",
"kExtraData_ActivateLoopSound",
"kExtraData_PatrolRefInUseData",
"kExtraData_AshPileRef",
"unknown134",
"kExtraData_FollowerSwimBreadcrumbs",
"kExtraData_AliasInstanceArray",
"kExtraData_Location",
"unknown138",
"kExtraData_LocationRefType",
"kExtraData_PromotedRef",
"unknown141",
"kExtraData_OutfitItem",
"unknown143",
"kExtraData_LeveledItemBase",
"kExtraData_LightData",
"kExtraData_SceneData",
"kExtraData_BadPosition",
"kExtraData_HeadTrackingWeight",
"kExtraData_FromAlias",
"kExtraData_ShouldWear",
"kExtraData_FavorCost",
"kExtraData_AttachedArrows3D",
"kExtraData_TextDisplayData",
"kExtraData_AlphaCutoff",
"kExtraData_Enchantment",
"kExtraData_Soul",
"kExtraData_ForcedTarget",
"unknown158",
"kExtraData_UniqueID",
"kExtraData_Flags",
"kExtraData_RefrPath",
"kExtraData_DecalGroup",
"kExtraData_LockList",
"kExtraData_ForcedLandingMarker",
"kExtraData_LargeRefOwnerCells",
"kExtraData_CellWaterEnvMap",
"kExtraData_CellGrassData",
"kExtraData_TeleportName",
"kExtraData_Interaction",
"kExtraData_WaterData",
"kExtraData_WaterCurrentZoneData",
"kExtraData_AttachRef",
"kExtraData_AttachRefChildren",
"kExtraData_GroupConstraint",
"kExtraData_ScriptedAnimDependence",
"kExtraData_CachedScale",
"kExtraData_RaceData",
"kExtraData_GIDBuffer",
"kExtraData_MissingRefIDs"
};


const char deliminator = '\\';
*/

/*
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

std::string GetTextureType(int textureType)
{
	std::string textureTypeName = "";

	switch (textureType)
	{
	case BGSTextureSet::kTextureDiffuse: textureTypeName = "Diffuse";
		break;
	case BGSTextureSet::kTextureNormal: textureTypeName = "Normal";
		break;
	case BGSTextureSet::kTextureEnvironmentMask: textureTypeName = "Enviroment Mask/Subsurface Tint";
		break;
	case BGSTextureSet::kTextureGlowMap: textureTypeName = "Glow/Detail Map";
		break;
	case BGSTextureSet::kTextureHeight: textureTypeName = "Height";
		break;
	case BGSTextureSet::kTextureEnvironment: textureTypeName = "Enviroment";
		break;
	case BGSTextureSet::kTextureMultilayer: textureTypeName = "Multilayer";
		break;
	case BGSTextureSet::kTextureBacklightMask: textureTypeName = "Backlight Mask/Specular";
		break;
	default: textureTypeName = "Unknown type";
		break;
	}

	return textureTypeName;
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

//Get the name of a equip slot. slot is based on the bit flag for the slot
std::string GetEquipSlotName( int slot )
{
	std::string equipSlotName = "";

	switch (slot)
	{
		case 0: 
		{
			equipSlotName = "Head";
			break;
		}
		case 1: 
		{
			equipSlotName = "Hair";
			break;
		}
		case 2:
		{
			equipSlotName = "Body";
			break;
		}
		case 3:
		{
			equipSlotName = "Hands";
			break;
		}
		case 4:
		{
			equipSlotName = "Forearms";
			break;
		}
		case 5:
		{
			equipSlotName = "Amulet";
			break;
		}
		case 6:
		{
			equipSlotName = "Ring";
			break;
		}
		case 7:
		{
			equipSlotName = "Feet";
			break;
		}
		case 8:
		{
			equipSlotName = "Calves";
			break;
		}
		case 9:
		{
			equipSlotName = "Shield";
			break;
		}
		case 10:
		{
			equipSlotName = "Tail";
			break;
		}
		case 11:
		{
			equipSlotName = "Long hair";
			break;
		}
		case 12:
		{
			equipSlotName = "Circlet";
			break;
		}
		case 13:
		{
			equipSlotName = "Ears";
			break;
		}
		case 14:
		{
			equipSlotName = "Slot 44";
			break;
		}
		case 15:
		{
			equipSlotName = "Slot 45";
			break;
		}
		case 16:
		{
			equipSlotName = "Slot 46";
			break;
		}
		case 17:
		{
			equipSlotName = "Slot 47";
			break;
		}
		case 18:
		{
			equipSlotName = "Slot 48";
			break;
		}
		case 19:
		{
			equipSlotName = "Slot 49";
			break;
		}
		case 20:
		{
			equipSlotName = "Decapitated head";
			break;
		}
		case 21:
		{
			equipSlotName = "Decapitate";
			break;
		}
		case 22:
		{
			equipSlotName = "Slot 52";
			break;
		}
		case 23:
		{
			equipSlotName = "Slot 53";
			break;
		}
		case 24:
		{
			equipSlotName = "Slot 54";
			break;
		}
		case 25:
		{
			equipSlotName = "Slot 55";
			break;
		}
		case 26:
		{
			equipSlotName = "Slot 56";
			break;
		}
		case 27:
		{
			equipSlotName = "Slot 57";
			break;
		}
		case 28:
		{
			equipSlotName = "Slot 58";
			break;
		}
		case 29:
		{
			equipSlotName = "Slot 59";
			break;
		}
		case 30:
		{
			equipSlotName = "Slot 60";
			break;
		}
		case 31:
		{
			equipSlotName = "FX01";
			break;
		}
	}

	return equipSlotName;
}

//Get the name of a equip slot. slot is based on the bit flag for the slot
std::string GetWeaponAnimationTypeName(int weaponType)
{
	std::string weaponTypeName = "";

	switch (weaponType)
	{
		case TESObjectWEAP::GameData::kType_HandToHandMelee:
		{
			weaponTypeName = "Hand to Hand";
			break;
		}

		case TESObjectWEAP::GameData::kType_OneHandSword:
		{
			weaponTypeName = "One handed sword";
			break;
		}

		case TESObjectWEAP::GameData::kType_OneHandDagger:
		{
			weaponTypeName = "Dagger";
			break;
		}

		case TESObjectWEAP::GameData::kType_OneHandAxe:
		{
			weaponTypeName = "One handed axe";
			break;
		}

		case TESObjectWEAP::GameData::kType_TwoHandSword:
		{
			weaponTypeName = "Two handed sword";
			break;
		}

		case TESObjectWEAP::GameData::kType_TwoHandAxe:
		{
			weaponTypeName = "Two handed axe";
			break;
		}

		case TESObjectWEAP::GameData::kType_Bow:
		{
			weaponTypeName = "Bow";
			break;
		}

		case TESObjectWEAP::GameData::kType_Staff:
		{
			weaponTypeName = "Staff";
			break;
		}

		case TESObjectWEAP::GameData::kType_CrossBow:
		{
			weaponTypeName = "Crossbow";
			break;
		}
	}
	return weaponTypeName;
}

//Get the name of a equip type 
std::string GetEquipTypeName(int formID)
{
	std::string equipTypeName = "";

	switch (formID)
	{
		case 0x00013F42:
		{
			equipTypeName = "Right Hand";
			break;
		}
		case 0x00013F43:
		{
			equipTypeName = "Left Hand";
			break;
		}
		case 0x00013F44:
		{
			equipTypeName = "Either Hand";
			break;
		}
		case 0x00013F45:
		{
			equipTypeName = "Both Hands";
			break;
		}
		case 0x000141E8:
		{
			equipTypeName = "Shield";
			break;
		}
		case 0x00025BEE:
		{
			equipTypeName = "Voice";
			break;
		}
		case 0x00035698:
		{
			equipTypeName = "Potion";
			break;
		}
	}
	return equipTypeName;
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

std::string GetExtraDataTypeName(int extraDataType)
{
	return ExtraDataTypes[extraDataType];
}

std::string GetArmorWeightClassName(int weightClass)
{
	std::string weightClassName = "";

	switch (weightClass)
	{
	case BGSBipedObjectForm::kWeight_None: weightClassName = "Clothing";
		break;
	case BGSBipedObjectForm::kWeight_Light: weightClassName = "Light";
		break;
	case BGSBipedObjectForm::kWeight_Heavy: weightClassName = "Heavy";
		break;
	default: weightClassName = "Unkown";
		break;
	}

	return weightClassName;
}

std::string GetLockLevel(int lockLevel)
{
	std::string lockLevelString = "";

	switch (lockLevel)
	{
	case 0: lockLevelString = "Novice";
		break;
	case 25: lockLevelString = "Apprentice";
		break;
	case 50: lockLevelString = "Adept";
		break;
	case 75: lockLevelString = "Expert";
		break;
	case 100: lockLevelString = "Master";
		break;
	case 255: lockLevelString = "Unpickable";
		break;
	default: lockLevelString = "Unkown";
		break;
	}

	return lockLevelString;
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
*/
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
/*
std::string DoubleToString(double number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

*/
std::string FormIDToString(int formID)
{
	std::ostringstream ss;
	std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);
	sprintf_s(sResult.get(), MAX_PATH, "%08X", formID);
	return sResult.get();
}

std::string BooleanToYesNoString(bool boolean)
{
	std::string output = "";

	if (boolean)
	{
		output = "Yes";
	}

	else
	{
		output = "No";
	}

	return output;
}
/*
//get the smallest bit that is 1 in the flags passed
int GetSmallestBitFlag(int flags)
{
	int smallestFlag = -1;

	int i = 0;

	while (i < 32 && smallestFlag == -1)
	{
		int mask = 1 << i;

		if ( (flags & mask) == mask )
		{
			smallestFlag = i;
		}

		i++;
	}

	return smallestFlag;
}

std::string GetFileName(std::string filePath)
{
	//get the name of the file

	int lastSlash = filePath.find_last_of(deliminator);

	std::string fileName = "";

	if (lastSlash != std::string::npos)
	{
		fileName = filePath.substr(lastSlash + 1);
	}
	//its unlikely but if the model is not in any folder its name is the same as the path
	else
	{
		fileName = filePath;
	}

	return fileName;
}*/

bool HasFlag(int Flags, int flag) 
{
	return ( Flags & flag ) == flag; 
}