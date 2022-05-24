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
	std::vector<std::vector<robin_hood::unordered_flat_map<int, std::string >>> formIDToEditorIDMap; //First index is mod index (first two digits of load id )
																								     //Second is based on form type
																									 //then map is for all forms of that mod and type
	robin_hood::unordered_flat_map<int, std::string > cachedEditorIDs; //Fetching Editor IDs is slow, and there are a lot of editor ids that need to be checked multiple times
																	   //(For example commonly used keywords/races used in a lot of ARMA etc ).
																	   //These are also a very small subset of the total number of editor ids, so we can store them after reading in for faster lookup
																       //with minimal memory consumption
};

