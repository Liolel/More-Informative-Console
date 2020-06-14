#include "ExtraInfoEntry.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"

ExtraInfoEntry::ExtraInfoEntry(std::string entry1, std::string entry2)
{
	this->entry1 = entry1;
	this->entry2 = entry2;
	this->priority = priorityDefault;
}

ExtraInfoEntry::ExtraInfoEntry(std::string entry1, std::string entry2, int priority)
{
	this->entry1 = entry1;
	this->entry2 = entry2;
	this->priority = priority;
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

void ExtraInfoEntry::CreatePrimaryScaleformArray(RE::GFxValue * mainScaleFormArray, RE::GFxMovie * root)
{
	_DMESSAGE("Creating scaleform array");

	root->CreateArray(mainScaleFormArray);

	if (!subarray.empty())
	{
		for (int i = 0; i < subarray.size(); i++)
		{
			RE::GFxValue subArrayEntry;
			subarray[i]->CreateSecondaryScaleformArray(&subArrayEntry, root);
			mainScaleFormArray->PushBack(subArrayEntry);
		}
	}
}

void ExtraInfoEntry::CreateSecondaryScaleformArray(RE::GFxValue * scaleFormArray, RE::GFxMovie * root)
{
	root->CreateArray(scaleFormArray);

	RE::GFxValue GFxExtraInfoName, GFxExtraInfoContents, GFxExtraInfoCount;

	std::string arraySize = IntToString(subarray.size());

	GFxExtraInfoName.SetString(entry1.c_str());
	GFxExtraInfoContents.SetString(entry2.c_str());
	GFxExtraInfoCount.SetString(arraySize.c_str());

	scaleFormArray->PushBack(GFxExtraInfoName);
	scaleFormArray->PushBack(GFxExtraInfoContents);
	scaleFormArray->PushBack(GFxExtraInfoCount);

	//_DMESSAGE( (entry1 + " " + entry2 + " " + arraySize ).c_str()); //This causes crashes somehow?
}

bool comparePrioritys(ExtraInfoEntry * extraInfoEntryA, ExtraInfoEntry * extraInforEntryB)
{
	return extraInfoEntryA->priority > extraInforEntryB->priority;
}

//Sort each vector by priority
void ExtraInfoEntry::Finalize()
{
	if (!subarray.empty())
	{
		std::stable_sort(subarray.begin(), subarray.end(), comparePrioritys);
		
		for (int i = 0; i < subarray.size(); i++)
		{
			subarray[i]->Finalize();
		}
	}
}


void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents)
{
	extraInfoEntry = new ExtraInfoEntry(extraInfoName, extraInfoContents);
}
