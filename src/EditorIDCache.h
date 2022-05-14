#pragma once
#include <vector>
#include "robin_hood.h"
#include "RE/Skyrim.h"
class EditorIDCache
{
public:
	static EditorIDCache* GetSingleton();

	void CacheEditorIDs();
	std::string GetEditorID(RE::TESForm* form);

private:
	std::vector<robin_hood::unordered_flat_map<RE::FormID, std::string>> formIDToEditorIDMap;
};

