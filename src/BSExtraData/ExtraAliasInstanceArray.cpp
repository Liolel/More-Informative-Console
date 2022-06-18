#include "ExtraAliasInstanceArray.h"
#include "TranslationCache.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"
#include "globals.h"

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

		if (alias->GetVMTypeID() == RE::BGSLocAlias::VMTYPEID) 
		{
			//const RE::BGSLocAlias * locAlias = static_cast<const RE::BGSLocAlias*>(alias);

			//currently no one has reverse engineered getting the contents of a location alias. So all we can do is report that it is a location alias
			ExtraInfoEntry* aliasTypeEntry;
			CreateExtraInfoEntry(aliasTypeEntry, GetTranslation("$AliasType"), GetTranslation("$AliasTypeLocationAlias"), priority_Alias_AliasType);

			resultArray->PushBack(aliasTypeEntry);
		} 
		else 
		{
			ExtraInfoEntry* aliasTypeEntry;
			CreateExtraInfoEntry(aliasTypeEntry, GetTranslation("$AliasType"), GetTranslation("$AliasTypeReferenceAlias"), priority_Alias_AliasType);

			resultArray->PushBack(aliasTypeEntry);

			const RE::BGSRefAlias* refAlias = static_cast<const RE::BGSRefAlias*>(alias);

			if (refAlias) {
				ExtraInfoEntry* aliasTargetEntry;
				RE::TESObjectREFR* target = refAlias->GetActorReference();

				if (target) 
				{
					RE::TESForm* baseForm = target->data.objectReference;
					CreateExtraInfoEntry(aliasTargetEntry, GetTranslation("$AliasTarget"), GetName(target), priority_Alias_AliasTarget);

					bool isminimizeFormDataRead = MICGlobals::minimizeFormDataRead;

					MICGlobals::minimizeFormDataRead = true;

					GetFormData(aliasTargetEntry, baseForm, target);
					resultArray->PushBack(aliasTargetEntry);

					MICGlobals::minimizeFormDataRead = isminimizeFormDataRead;
				} 
				else 
				{
					CreateExtraInfoEntry(aliasTargetEntry, GetTranslation("$AliasTarget"), GetTranslation("$AliasTargetEmpty"), priority_Alias_AliasTarget);
					resultArray->PushBack(aliasTargetEntry);
				}


				//auto location = locAlias->
			}
		}

	}
}
