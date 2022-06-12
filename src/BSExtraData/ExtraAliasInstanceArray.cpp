#include "ExtraAliasInstanceArray.h"
#include "TranslationCache.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"

void ProcessExtraAliasInstanceArray(ExtraInfoEntry* resultArray, RE::BSExtraData* data)
{
	RE::ExtraAliasInstanceArray* aliasInstanceArray = static_cast<RE::ExtraAliasInstanceArray*>(data);
	ExtraInfoEntry * aliasesEntry;

	CreateExtraInfoEntry(aliasesEntry, GetTranslation("$Aliases"), "", priorty_Aliases);

	int numberOfAliases = aliasInstanceArray->aliases.size();

	for (int i = 0; i < numberOfAliases; i++)
	{
		auto alias = aliasInstanceArray->aliases[i]->alias;
		auto owningQuest = alias->owningQuest;

		if (owningQuest)
		{
			ExtraInfoEntry* aliasEntry;
			CreateExtraInfoEntry(aliasEntry, GetName( owningQuest ), alias->aliasName.c_str(), priorty_Alias);
			GetAliasInformation(aliasEntry, alias);

			aliasesEntry->PushBack(aliasEntry);
		}
	}

	resultArray->PushBack(aliasesEntry);
}

void GetAliasInformation(ExtraInfoEntry* resultArray, const RE::BGSBaseAlias* alias)
{
	ExtraInfoEntry* aliasNameEntry;

	CreateExtraInfoEntry(aliasNameEntry, GetTranslation("$AliasName"), alias->aliasName.c_str(), priority_Alias_AliasName); //this gives number of properties
	resultArray->PushBack(aliasNameEntry);

	RE::TESQuest* owningQuest = alias->owningQuest;

	if (owningQuest)
	{
		ExtraInfoEntry* aliasQuestEntry;
		CreateExtraInfoEntry(aliasQuestEntry, GetTranslation("$AliasQuest"), GetName(owningQuest), priority_Alias_AliasQuest);
		GetFormData(aliasQuestEntry, owningQuest, nullptr);
		resultArray->PushBack(aliasQuestEntry);
	}
}
