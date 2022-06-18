#include "ExtraInfoEntry.h"
#include "Util/ScriptUtil.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include <algorithm>

ExtraInfoEntry::ExtraInfoEntry(std::string entry1, std::string entry2, int priority)
{
	this->entry1 = entry1;
	this->entry2 = entry2;
	this->priority = priority;
	this->parents = 0;
	this->isFinalized = false;
	this->mayCopy = true;
	this->disableSortingByName = false;
	this->needsExpansion = false;
	this->subarrayCountOverride = 0;
	this->scriptToExpand = nullptr;
}

void ExtraInfoEntry::Clear()
{
	//DebugMessage("Clear Start " + entry1 + " " + entry2 + " " + IntToString( subarray.size() ) );

	for (std::vector<ExtraInfoEntry*>::iterator it = subarray.begin(); it != subarray.end(); ++it) 
	{
		//if this is the last parent of the entry remove the entry
		if ( (*it)->parents <= 1)
		{
			(*it)->Clear();
			delete (*it);
		}
		else
		{
			(*it)->parents--;
		}
	}

	subarray.clear();
	isFinalized = false;
}

void ExtraInfoEntry::PushBack(ExtraInfoEntry* subArrayEntry)
{
	subarray.push_back(subArrayEntry);
	subArrayEntry->parents++;
}

void ExtraInfoEntry::CopyChildren(ExtraInfoEntry* entryToCopyFrom)
{
	for ( auto extraInfoEntryChild = entryToCopyFrom->subarray.begin(); extraInfoEntryChild != entryToCopyFrom->subarray.end(); extraInfoEntryChild++ )
	{
		if( (*extraInfoEntryChild)->mayCopy )
		{
			this->PushBack(*extraInfoEntryChild);
		}
	}
}

bool ExtraInfoEntry::HasChildren()
{
	return !subarray.empty();
}

ExtraInfoEntry* ExtraInfoEntry::GetChild(int index)
{
	return index < subarray.size() ? subarray[index] : nullptr;
}

void ExtraInfoEntry::SetMayCopy(bool valueToSet )
{
	this->mayCopy = valueToSet;
}


void ExtraInfoEntry::CreatePrimaryScaleformArray(RE::GFxValue* mainScaleFormArray, RE::GFxMovie* root)
{
	logger::debug("Creating scaleform array " + IntToString( (int)subarray.size() ) );

	root->CreateArray(mainScaleFormArray);

	if (needsExpansion && scriptToExpand ) 
	{
		GetVariablesAndPropertiesForScript(this, scriptToExpand);
		Finalize();
	}

	if (!subarray.empty()) {
		for (int i = 0; i < subarray.size(); i++) {
			RE::GFxValue subArrayEntry;
			subarray[i]->CreateSecondaryScaleformArray(&subArrayEntry, root);
			mainScaleFormArray->PushBack(subArrayEntry);
		}
	}
}

void ExtraInfoEntry::CreateSecondaryScaleformArray(RE::GFxValue* scaleFormArray, RE::GFxMovie* root)
{
	root->CreateArray(scaleFormArray);

	RE::GFxValue GFxExtraInfoName, GFxExtraInfoContents, GFxExtraInfoCount;

	std::string arraySize = "";

	if( needsExpansion ) 
	{
		arraySize = IntToString(subarrayCountOverride);
	} 
	else 
	{
		arraySize = IntToString((int)subarray.size());
	}

	GFxExtraInfoName.SetString(entry1.c_str());
	GFxExtraInfoContents.SetString(entry2.c_str());
	GFxExtraInfoCount.SetString(arraySize.c_str());

	scaleFormArray->PushBack(GFxExtraInfoName);
	scaleFormArray->PushBack(GFxExtraInfoContents);
	scaleFormArray->PushBack(GFxExtraInfoCount);

	//logger::debug( (entry1 + " " + entry2 + " " + arraySize ).c_str()); //This causes crashes somehow?
}

bool comparePrioritys(ExtraInfoEntry* extraInfoEntryA, ExtraInfoEntry* extraInforEntryB)
{
	return extraInfoEntryA->priority < extraInforEntryB->priority;
}

bool comparePrioritysFullSorting(ExtraInfoEntry* extraInfoEntryA, ExtraInfoEntry* extraInforEntryB)
{
	return  extraInfoEntryA->priority == extraInforEntryB->priority 
			? extraInfoEntryA->entry1.compare( extraInforEntryB->entry1 ) < 0
			: extraInfoEntryA->priority < extraInforEntryB->priority;
}

//Sort each vector by priority
void ExtraInfoEntry::Finalize()
{
	if (!this->isFinalized)
	{
		if (!subarray.empty()) {

			if (this->disableSortingByName)
			{
				std::stable_sort(subarray.begin(), subarray.end(), comparePrioritys);
			}
			else
			{
				std::stable_sort(subarray.begin(), subarray.end(), comparePrioritysFullSorting);
			}

			for (int i = 0; i < subarray.size(); i++) {
				subarray[i]->Finalize();
			}
		}
	}

	this->isFinalized = true;
}

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents, priority priority)
{
	extraInfoEntry = new ExtraInfoEntry(extraInfoName, extraInfoContents, priority);
}
