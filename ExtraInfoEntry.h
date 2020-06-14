#pragma once

#include <Vector>
#include "RE/Skyrim.h"
#include "Util/GeneralUtil.h"

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2);
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

//Constants for priorities for easy modification

const int priorityDefault = 0;
