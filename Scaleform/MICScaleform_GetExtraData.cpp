#pragma once

#include "MICScaleform_GetExtraData.h"
#include "MoreInformativeConsole/Util/ScaleformUtil.h"
#include "MoreInformativeConsole/globals.h"


void MICScaleform_GetExtraData::Call(Params& a_params)
{
	_DMESSAGE("GetExtraData: Invoke Start");

	RE::GFxMovie* movie = a_params.movie;

	//Determine mode to use
	RE::GFxValue* modeGFX = &a_params.args[0];
	int modeInt = modeGFX->GetNumber();

	if (modeInt == Constant_ModeConsoleHandle)
	{

		RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
		if (ref != nullptr)
		{

			_DMESSAGE("GetExtraData: pRefFound");

			RE::TESBoundObject* baseForm = ref->data.objectReference;

			if (baseForm != nullptr)
			{
				_DMESSAGE("GetExtraData: BaseFound");

				MICGlobals::rootEntry.Clear();

				ExtraInfoEntry* test;
				CreateExtraInfoEntry(test, "Object", "");
				MICGlobals::rootEntry.PushBack(test);

				//GetFormData(&MICGlobals::rootEntry, pBaseForm, pRef);

				//DebugMessage("Get Form Information done");
			}
		}
	}

	else if (modeInt == Constant_ModeWorldInformation)
	{
		MICGlobals::rootEntry.Clear();
		//GetWorldData(&MICGlobals::rootEntry);

		ExtraInfoEntry* test;
		CreateExtraInfoEntry(test, "World", "");
		MICGlobals::rootEntry.PushBack(test);
	}

	else if (modeInt == Constant_ModeMFG)
	{
		MICGlobals::rootEntry.Clear();
		ExtraInfoEntry* test;
		CreateExtraInfoEntry(test, "MFG", "");
		MICGlobals::rootEntry.PushBack(test);
	}

	//else if( modeInt == Constant_Mod)

	//RE::GFxValue returnValue;
	RE::GFxValue resultArray;

	//Convert the results into an array we can send to the swf
	MICGlobals::rootEntry.CreatePrimaryScaleformArray(&resultArray, movie);

	//Returning the desired results can crash the game if the method called takes too long to return the value. Invoking an method in the console.swf when we've finished running our code
	//Seems to prevent this crash
	RE::GFxValue root;
	movie->GetVariable(&root, "_root.consoleFader_mc.Console_mc");
	
	root.Invoke("AddExtraInfo", 0, &resultArray, 1);

	_DMESSAGE("GetExtraData: Invoke End");
}
