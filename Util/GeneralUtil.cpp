#pragma once

#include "GeneralUtil.h"
#include "SKSE/Logger.h"
#include <sstream>
#include <vector>
#include <memory>

//MIC options

//const char deliminator = '\\';


/*

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
