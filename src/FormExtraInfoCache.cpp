#include "FormExtraInfoCache.h"

FormExtraInfoCache* FormExtraInfoCache::GetSingleton()
{
	static FormExtraInfoCache singleton;
	return std::addressof(singleton);
}


void FormExtraInfoCache::CacheExtraInfoEntry(RE::TESForm* form, ExtraInfoEntry* extraInfoEntry)
{
	auto formID = form->GetFormID();
	this->formIDToExtraInfoEntry.emplace(formID, extraInfoEntry);

}

ExtraInfoEntry* FormExtraInfoCache::GetExtraInfoEntry(RE::TESForm* form)
{
	auto formID = form->GetFormID();

	auto extraInfoEntryItr = this->formIDToExtraInfoEntry.find(formID);
	ExtraInfoEntry* extraInfoEntry = extraInfoEntryItr != this->formIDToExtraInfoEntry.end() ? extraInfoEntryItr->second : nullptr;

	return extraInfoEntry;
}

void FormExtraInfoCache::ClearCache()
{
	this->formIDToExtraInfoEntry.clear();
}
