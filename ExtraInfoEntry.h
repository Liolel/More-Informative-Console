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

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents);
void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents, int priority);

//Constants for priorities for easy modification

const int priority_Default = 0;

//Basic form information should have the highest priority
const int priority_Name = 100000;
const int priority_EditorID = 99990;
const int priority_FormID = 99980;
const int priority_FormType = 99970;
const int priority_FormLocation = 99960; //Mods form is found in

const int priority_Reference_Location = 50000; //XYZ position of reference
const int priority_Reference_Enabled = 10000; //Is the reference enabled or disabled
const int priority_ExtraData_EnableParent = 9999; //Enable Parent Information