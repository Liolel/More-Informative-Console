#include "ExtraInfoEntry.h"

ExtraInfoEntry::ExtraInfoEntry(std::string entry1, std::string entry2)
{
	this->entry1 = entry1;
	this->entry2 = entry2;
}

void ExtraInfoEntry::Clear()
{
	//DebugMessage("Clear Start " + entry1 + " " + entry2 + " " + IntToString( subarray.size() ) );

	for (std::vector< ExtraInfoEntry * >::iterator it = subarray.begin(); it != subarray.end(); ++it)
	{
		//DebugMessage("In Clear Loop");
		(*it)->Clear();
		delete (*it);
	}

	subarray.clear();
}

void ExtraInfoEntry::PushBack(ExtraInfoEntry * subArrayEntry)
{
	subarray.push_back(subArrayEntry);
}

ExtraInfoEntry* ExtraInfoEntry::GetChild(int index)
{
	return subarray[index];
}

void ExtraInfoEntry::CreatePrimaryScaleformArray(GFxValue * mainScaleFormArray, GFxMovieView * root)
{
	root->CreateArray(mainScaleFormArray);

	if (!subarray.empty())
	{
		
		for (int i = 0; i < subarray.size(); i++)
		{
			GFxValue subArrayEntry; 
			subarray[i]->CreateSecondaryScaleformArray(&subArrayEntry, root);
			mainScaleFormArray->PushBack(&subArrayEntry);
		}
	}
}

void ExtraInfoEntry::CreateSecondaryScaleformArray(GFxValue * scaleFormArray, GFxMovieView * root)
{
	root->CreateArray(scaleFormArray);

	GFxValue GFxExtraInfoName, GFxExtraInfoContents, GFxExtraInfoCount;

	std::string arraySize = IntToString(subarray.size());

	GFxExtraInfoName.SetString(entry1.c_str());
	GFxExtraInfoContents.SetString(entry2.c_str());
	GFxExtraInfoCount.SetString(arraySize.c_str());

	scaleFormArray->PushBack(&GFxExtraInfoName);
	scaleFormArray->PushBack(&GFxExtraInfoContents);
	scaleFormArray->PushBack(&GFxExtraInfoCount);

	DebugMessage(entry1 + " " + entry2 + " " + IntToString(subarray.size()));
}