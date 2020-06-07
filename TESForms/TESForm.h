#pragma once
#include "RE/Skyrim.h"

//Functions for getting the name of the mods a form is located in
bool GetHasSourceFileArray(RE::TESForm* form);
std::string GetNthFormLocationName(RE::TESForm* form, UInt32 n);
std::string GetFirstFormLocationName(RE::TESForm* form);
std::string GetLastFormLocationName(RE::TESForm* form);