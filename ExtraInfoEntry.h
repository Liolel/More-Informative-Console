#pragma once

#include <Vector>
#include "ScaleformValue.h"
#include "ScaleformMovie.h"
#include "Util.h"

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2);

	void Clear();
	void PushBack(ExtraInfoEntry * subArrayEntry);

	ExtraInfoEntry* GetChild(int index );
	void CreatePrimaryScaleformArray(GFxValue * mainScaleFormArray, GFxMovieView * root);
	void CreateSecondaryScaleformArray(GFxValue * scaleFormArray, GFxMovieView * root);

private:

	std::string entry1;
	std::string entry2;

	std::vector<ExtraInfoEntry*> subarray;
};
