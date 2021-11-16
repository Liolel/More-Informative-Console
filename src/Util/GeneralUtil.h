#pragma once

#include "RE/Skyrim.h"
#include "SKSE/Logger.h"

//void DebugMessage(std::string message);

/*

int NumberOfItemInContainer(RE::TESForm * item, RE::TESContainer * container);
bool HasItem(RE::EntryDataList * inventory, RE::TESForm * item);
*/
//Convert various data types to strings to display in UI
std::string IntToString(int number);
std::string IntAsDoubleToString(double number);  //This is for converting numbers that should be displayed as ints but are stored in double form due to how SKSE integrates with the console.swf file

std::string FloatToString(float number);
std::string DoubleToString(double number);
std::string FormIDToString(int formID);

std::string BooleanToYesNoString(bool boolean);

/*
int GetSmallestBitFlag(int flags);*/

bool HasFlag(int Flags, int flag);
