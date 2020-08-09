#pragma once

#include "GeneralUtil.h"
#include "SKSE/Logger.h"
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

std::string IntAsDoubleToString(double number)
{
	int numberInt = (int)number;

	return IntToString(numberInt);
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
*/

bool HasFlag(int Flags, int flag) 
{
	return ( Flags & flag ) == flag; 
}
