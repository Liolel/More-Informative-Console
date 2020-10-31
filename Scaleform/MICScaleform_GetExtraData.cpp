#pragma once

#include "MICScaleform_GetExtraData.h"
#include "Util/ScaleformUtil.h"
#include "Util/NameUtil.h"
#include "globals.h"
#include "TESForms/TESForm.h"
#include "TESForms/TESObjectCELL.h"


void MICScaleform_GetExtraData::Call(Params& a_params)
{
	logger::debug("GetExtraData: Invoke Start");

	RE::GFxMovie* movie = a_params.movie;

	//Determine mode to use
	RE::GFxValue* modeGFX = &a_params.args[0];
	int modeInt = (int)modeGFX->GetNumber();

	if (modeInt == Constant_ModeConsoleHandle)
	{

		RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
		if (ref != nullptr)
		{
			logger::debug("GetExtraData: refFound");

			RE::TESBoundObject* baseForm = ref->data.objectReference;

			if (baseForm != nullptr)
			{
				logger::debug("GetExtraData: BaseFound");

				MICGlobals::rootEntry.Clear();
				GetFormData(&MICGlobals::rootEntry, baseForm, ref);

				logger::debug("Get Form Information done");
			}
		}
	}

	else if (modeInt == Constant_ModeWorldInformation)
	{
		MICGlobals::rootEntry.Clear();
		GetWorldData(&MICGlobals::rootEntry);
	}

	else if (modeInt == Constant_ModeMFG)
	{
		MICGlobals::rootEntry.Clear();
		ExtraInfoEntry* test;
		CreateExtraInfoEntry(test, "MFG", "", priority_Default);
		MICGlobals::rootEntry.PushBack(test);
	}

	//Sort the final results
	MICGlobals::rootEntry.Finalize();

	//RE::GFxValue returnValue;
	RE::GFxValue resultArray;

	//Convert the results into an array we can send to the swf
	MICGlobals::rootEntry.CreatePrimaryScaleformArray(&resultArray, movie);

	//Returning the desired results can crash the game if the method called takes too long to return the value. Invoking an method in the console.swf when we've finished running our code
	//Seems to prevent this crash
	RE::GFxValue root;
	movie->GetVariable(&root, "_root.consoleFader_mc.Console_mc");
	
	root.Invoke("AddExtraInfo", 0, &resultArray, 1);

	logger::debug("GetExtraData: Invoke End");
}

void GetWorldData(ExtraInfoEntry* resultArray)
{	
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();

	if (pc)
	{
		logger::debug("Starting Worldspace");

		RE::TESWorldSpace* currentWorldSpace = pc->GetWorldspace();;

		if (currentWorldSpace)
		{
			std::string worldSpaceName = GetName(currentWorldSpace);

			ExtraInfoEntry* worldSpaceEntry;
			CreateExtraInfoEntry(worldSpaceEntry, "World Space", worldSpaceName, priority_WorldData_WorldSpace);

			GetFormData(worldSpaceEntry, currentWorldSpace, nullptr);
			resultArray->PushBack(worldSpaceEntry);
		}

		GetCurrentCellForWorldData(resultArray, pc);
	}

	//Find the current music track
	/*
	DataHandler* dataHandler = DataHandler::GetSingleton();

	if (dataHandler)
	{
		DebugMessage("Starting Music");
		tArray<BGSMusicTrackFormWrapper*>* musicTrackArray = (tArray<BGSMusicTrackFormWrapper*>*)(&(dataHandler->arrMUST));

		int numberOfMusicTracks = musicTrackArray->count;

		for (int i = 0; i <= numberOfMusicTracks; i++)
		{
			BGSMusicTrackFormWrapper* wrapper;
			musicTrackArray->GetNthItem(i, wrapper);

			if (wrapper)
			{
				BSIMusicTrack* musicTrack = &(wrapper->track);

				MUSIC_STATUS musicTrackStatus = (MUSIC_STATUS)musicTrack->Unk_08();

				if (musicTrackStatus == MUSIC_STATUS::kPlaying)
				{
					std::string  formIDString = FormIDToString(wrapper->formID);
					ExtraInfoEntry* musicEntry;
					CreateExtraInfoEntry(musicEntry, "Music", formIDString);
					resultArray->PushBack(musicEntry);
				}
			}
		}
	}*/
}
