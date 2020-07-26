#pragma once

#include <Vector>
#include "RE/Skyrim.h"
#include "Util/GeneralUtil.h"

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2, int priority);

	void Clear();
	void PushBack(ExtraInfoEntry * subArrayEntry);

	ExtraInfoEntry* GetChild(int index );
	void CreatePrimaryScaleformArray(RE::GFxValue * mainScaleFormArray, RE::GFxMovie * root);
	void CreateSecondaryScaleformArray(RE::GFxValue * scaleFormArray, RE::GFxMovie * root);

	void Finalize();

	int priority; //Priority for use in sorting. Higher Priority entires move to the top of the list

private:

	std::string entry1;
	std::string entry2;

	std::vector<ExtraInfoEntry*> subarray;
};

enum priority
{
	priority_Name,
	priority_EditorID,
	priority_FormID,
	priority_FormType,
	priority_FormLocation, //Mods form is found in
	priority_Reference_Location, //XYZ position of reference
	priority_Reference_Enabled, //Is the reference enabled or disabled
	priority_ExtraData_EnableParent, //Enable Parent Information
	priority_Default
};

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents);
void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents, priority priority);

//Enum for priority order Highest up takes precedence in sorting

