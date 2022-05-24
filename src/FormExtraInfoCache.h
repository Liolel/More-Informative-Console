#pragma once
#include "ExtraInfoEntry.h"
#include "robin_hood.h"

class FormExtraInfoCache
{
public:
	static FormExtraInfoCache* GetSingleton();

	void CacheExtraInfoEntry(RE::TESForm* form, ExtraInfoEntry* extraInfoEntry);
	ExtraInfoEntry* GetExtraInfoEntry(RE::TESForm* form); 

	void ClearCache();

private:
	robin_hood::unordered_flat_map<RE::FormID, ExtraInfoEntry*> formIDToExtraInfoEntry;
};
