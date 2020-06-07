#pragma once

#include "MICScaleform_RetrieveExtraData.h"
#include "MoreInformativeConsole/Util/ScaleformUtil.h"
#include "MoreInformativeConsole/globals.h"


void MICScaleform_RetrieveExtraData::Call(Params& a_params)
{
	_DMESSAGE( ("RetrieveExtraData: Invoke Start, NumArgs " + IntToString(a_params.argCount) ).c_str() );

	RE::GFxMovie * movie = a_params.movie;
	RE::GFxValue * indexArray = &a_params.args[0];

	ExtraInfoEntry* extrainfoEntryToRetrieve = TraverseExtraInfoEntries(&MICGlobals::rootEntry, indexArray, 0);

	RE::GFxValue returnValue;
	RE::GFxValue resultArray;
	extrainfoEntryToRetrieve->CreatePrimaryScaleformArray(&resultArray, movie);

	//Returning the desired results can crash the game if the method called takes too long to return the value. Invoking an method in the console.swf when we've finished running our code
	//Seems to prevent this crash
	movie->Invoke("_root.consoleFader_mc.Console_mc.AddExtraInfo", &returnValue, &resultArray, 1);

	_DMESSAGE("RetrieveExtraData: End");
}

//Recursively travel through the extra info structure to find the desired entry 
ExtraInfoEntry* MICScaleform_RetrieveExtraData::TraverseExtraInfoEntries(ExtraInfoEntry* currentEntry, RE::GFxValue* indexArray, int currentIndex)
{
	_DMESSAGE( ("Traverse Current Index " + IntToString(currentIndex) ).c_str() );

	RE::GFxValue indexToSelect;
	indexArray->GetElement(currentIndex, &indexToSelect);

	_DMESSAGE( ("Traverse Current Index " + IntToString(currentIndex) + " indexToSelect " + IntToString(indexToSelect.GetNumber() ) ).c_str() );

	ExtraInfoEntry * nextEntry = currentEntry->GetChild(indexToSelect.GetNumber());

	if (currentIndex + 1 == indexArray->GetArraySize())
	{
		return nextEntry;
	}

	else
	{
		return TraverseExtraInfoEntries(nextEntry, indexArray, currentIndex + 1);
	}
}