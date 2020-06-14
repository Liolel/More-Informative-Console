#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"

//Functions for getting the name of the mods a form is located in
bool GetHasSourceFileArray(RE::TESForm* form);
std::string GetNthFormLocationName(RE::TESForm* form, UInt32 n);
std::string GetFirstFormLocationName(RE::TESForm* form);
std::string GetLastFormLocationName(RE::TESForm* form);

//Methods for retrieving data common toall forms
void GetFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm);
void GetCommonFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESForm* refForm);