#pragma once

#include "GeneralUtil.h"
#include "SKSE/Logger.h"
#include <sstream>
#include <vector>
#include <memory>

//MIC options

//const char deliminator = '\\';


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
