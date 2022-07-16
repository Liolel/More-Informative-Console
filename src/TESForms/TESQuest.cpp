#include "TESQuest.h"

void GetQuestInformation(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESQuest* quest = static_cast<RE::TESQuest*>(baseForm);
	if (quest) 
	{
		ExtraInfoEntry* statusEntry;

		std::string status = "";

		if (!(quest->alreadyRun))
		{
			status = GetTranslation("$QuestStatusInactive");
		}
		else if (quest->IsRunning() ) 
		{
			status = GetTranslation("$QuestStatusRunning");
		}
		else if (quest->IsCompleted() ) 
		{
			status = GetTranslation("$QuestStatusCompleted");
		}
		else if (quest->IsStopped() ) 
		{
			status = GetTranslation("$QuestStatusStopped");
		} 

		CreateExtraInfoEntry(statusEntry, GetTranslation("$QuestStatus"), status, priority_Quest_Status);
		resultArray->PushBack(statusEntry);

		ExtraInfoEntry* currentStageEntry;
		CreateExtraInfoEntry(currentStageEntry, GetTranslation("$QuestCurrentStage"), IntToString(quest->currentStage), priority_Quest_CurrentStage);
		resultArray->PushBack(currentStageEntry);

		ExtraInfoEntry* aliasesEntry;
		CreateExtraInfoEntry(aliasesEntry, GetTranslation("$Aliases"), "", priorty_Aliases);

		
		logger::debug("GetQuestInformation: Starting Aliases");

		for (std::uint32_t i = 0; i < quest->aliases.size(); i++) 
		{
			RE::BGSBaseAlias* alias = quest->aliases[i];

			if (alias) 
			{
				ExtraInfoEntry* aliasEntry;
				CreateExtraInfoEntry(aliasEntry, alias->aliasName.c_str(), "", priorty_Aliases);

				GetAliasInformation(aliasEntry
								   , alias
					               , false //getOwningQuest
								   );

				aliasesEntry->PushBack(aliasEntry);
			}
		}

		logger::debug("GetQuestInformation: Ending Aliases");

		resultArray->PushBack(aliasesEntry);
	}
}
