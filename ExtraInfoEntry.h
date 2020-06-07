#pragma once

#include <Vector>
#include "RE/Skyrim.h"
#include "Util/GeneralUtil.h"

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2);

	void Clear();
	void PushBack(ExtraInfoEntry * subArrayEntry);

	ExtraInfoEntry* GetChild(int index );
	void CreatePrimaryScaleformArray(RE::GFxValue * mainScaleFormArray, RE::GFxMovie * root);
	void CreateSecondaryScaleformArray(RE::GFxValue * scaleFormArray, RE::GFxMovie * root);

private:

	std::string entry1;
	std::string entry2;

	std::vector<ExtraInfoEntry*> subarray;
};

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents);