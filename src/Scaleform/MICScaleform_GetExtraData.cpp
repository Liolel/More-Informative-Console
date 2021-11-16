#pragma once

#include "MICScaleform_GetExtraData.h"
#include "TESForms/BGSMusicTrack.h"
#include "TESForms/TESForm.h"
#include "TESForms/TESNPC.h"
#include "TESForms/TESObjectCELL.h"
#include "Util/NameUtil.h"
#include "Util/ScaleformUtil.h"
#include "globals.h"

void MICScaleform_GetExtraData::Call(Params& a_params)
{
	logger::debug("GetExtraData: Invoke Start");

	RE::GFxMovie* movie = a_params.movie;

	bool skipUsualEndCode = false;

	//Determine mode to use
	RE::GFxValue* modeGFX = &a_params.args[0];
	int modeInt = (int)modeGFX->GetNumber();

	if (modeInt == Constant_ModeConsoleHandle) {
		RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
		if (ref != nullptr) {
			logger::debug("GetExtraData: refFound");

			RE::TESBoundObject* baseForm = ref->data.objectReference;

			if (baseForm != nullptr) {
				logger::debug("GetExtraData: BaseFound");

				MICGlobals::rootEntry.Clear();
				GetFormData(&MICGlobals::rootEntry, baseForm, ref);

				logger::debug("Get Form Information done");
			}
		}
	}

	else if (modeInt == Constant_ModeWorldInformation) {
		MICGlobals::rootEntry.Clear();
		GetWorldData(&MICGlobals::rootEntry);
	}

	else if (modeInt == Constant_ModeMFG) {
		MICGlobals::rootEntry.Clear();

		RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
		if (ref != nullptr && ref->GetFormType() == RE::FormType::ActorCharacter) {
			logger::debug("GetExtraData MFG: Character found");

			skipUsualEndCode = true;  //We need to return the mfg information slightly differently from the other types of information so skip the usual end code

			ExtraInfoEntry expressionsRoot("", "", priority_Default);
			ExtraInfoEntry modifierRoot("", "", priority_Default);
			ExtraInfoEntry phenomeRoot("", "", priority_Default);

			RE::Actor* actor = static_cast<RE::Actor*>(ref);

			GetMFGInformation(&expressionsRoot, &modifierRoot, &phenomeRoot, actor);

			expressionsRoot.Finalize();
			modifierRoot.Finalize();
			phenomeRoot.Finalize();

			RE::GFxValue expressionsArray, modifierArray, phenomeArray;
			expressionsRoot.CreatePrimaryScaleformArray(&expressionsArray, movie);
			modifierRoot.CreatePrimaryScaleformArray(&modifierArray, movie);
			phenomeRoot.CreatePrimaryScaleformArray(&phenomeArray, movie);

			RE::GFxValue root;
			movie->GetVariable(&root, "_root.consoleFader_mc.Console_mc");

			//Send the expressions
			root.Invoke("AddExtraInfo", 0, &expressionsArray, 1);

			//Send the modifier
			root.Invoke("AddExtraInfo", 0, &modifierArray, 1);

			//Send the phenome
			root.Invoke("AddExtraInfo", 0, &phenomeArray, 1);

			logger::debug("Get MFG information done");
		}

		else {
			ExtraInfoEntry* noNPCSelectedWarning;
			CreateExtraInfoEntry(noNPCSelectedWarning, "There is no npc selected", "", priority_Warning);
			MICGlobals::rootEntry.PushBack(noNPCSelectedWarning);
		}
	}

	if (!skipUsualEndCode) {
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
}

void GetWorldData(ExtraInfoEntry* resultArray)
{
	RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();

	if (pc) {
		logger::debug("Starting Worldspace");

		RE::TESWorldSpace* currentWorldSpace = pc->GetWorldspace();
		;

		if (currentWorldSpace) {
			std::string worldSpaceName = GetName(currentWorldSpace);

			ExtraInfoEntry* worldSpaceEntry;
			CreateExtraInfoEntry(worldSpaceEntry, "World Space", worldSpaceName, priority_WorldData_WorldSpace);

			GetFormData(worldSpaceEntry, currentWorldSpace, nullptr);
			resultArray->PushBack(worldSpaceEntry);
		}

		GetCurrentCellForWorldData(resultArray, pc);
	}

	GetCurrentMusic(resultArray);

	//Get the weather
	RE::Sky* sky = RE::Sky::GetSingleton();

	if (sky && sky->currentWeather) {
		RE::TESWeather* weather = sky->currentWeather;

		std::string weatherName = GetName(weather);

		ExtraInfoEntry* weatherEntry;
		CreateExtraInfoEntry(weatherEntry, "Weather", weatherName, priority_WorldData_Weather);

		GetFormData(weatherEntry, weather, nullptr);
		resultArray->PushBack(weatherEntry);
	}
}