#pragma once

#include "MICScaleform_GetQuestData.h"
#include "TESForms/TESForm.h"
#include "Util/ScaleformUtil.h"
#include "globals.h"
#include "FormExtraInfoCache.h"
#include "TranslationCache.h"
#include "Util/GeneralUtil.h"

//4-23-2022: Checked for translations needed

void MICScaleform_GetQuestData::Call(Params& a_params)
{
	logger::debug("GetQuestData: Invoke Start");

	RE::GFxMovie* movie = a_params.movie;

	auto formExtraInfoCache = FormExtraInfoCache::GetSingleton();

	//Determine mode to use
	RE::GFxValue* modeGFX = &a_params.args[0];
	std::string questEditorIdToFind = modeGFX->GetString();
	questEditorIdToFind = ToLower(questEditorIdToFind);

	logger::debug("{}", questEditorIdToFind);

	RE::TESDataHandler * datahandler = RE::TESDataHandler::GetSingleton();

	RE::TESQuest* matchingQuest = nullptr;

	RE::BSTArray<RE::TESForm*> questArray = datahandler->GetFormArray(RE::FormType::Quest);

	UINT32 i = 0;

	while (i < questArray.size()
		   && !matchingQuest) 
	{
		RE::TESQuest* quest = static_cast<RE::TESQuest*>(questArray[i]);

		logger::debug("{}", quest->formEditorID.c_str());

		if (quest )
		{
			std::string questEditorId = quest->formEditorID.c_str();
			questEditorId = ToLower(questEditorId);
		
			if (questEditorId == questEditorIdToFind) 
			{
				matchingQuest = quest;
			}
		}

		i++;
	}

	if( matchingQuest ) 
	{
		logger::debug("GetQuestData: Quest found");
		MICGlobals::rootEntry.Clear();
		formExtraInfoCache->ClearCache();

		GetFormData(&MICGlobals::rootEntry, matchingQuest, nullptr);
		logger::debug("GetQuestData: Quest data finished");

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
	}

	else
	{
		RE::ConsoleLog* consoleLog = RE::ConsoleLog::GetSingleton();
		std::string errorMessage = GetTranslation("$QuestNotFound");
		consoleLog->Print(errorMessage.c_str());
	}
	
	logger::debug("GetQuestData: Invoke End");
}
