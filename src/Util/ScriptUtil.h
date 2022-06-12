#pragma once

#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void GetScripts(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm);
void GetScriptsForHandle(ExtraInfoEntry* resultArray, RE::BSScript::Internal::VirtualMachine* vm, RE::BSScript::IObjectHandlePolicy* policy, RE::VMHandle handle, RE::TESForm* form, RE::ActiveEffect* activeEffect, const RE::BGSBaseAlias* alias);
void GetVariablesAndPropertiesForScript(ExtraInfoEntry* resultArray, RE::BSScript::Object * script);
std::string CleanVariableName(std::string variableName);
std::string GetVariableValue(ExtraInfoEntry* resultArray, RE::BSScript::Variable * variable, std::string variableName);
void GetAliasInformation(ExtraInfoEntry* resultArray, const RE::BGSBaseAlias* alias);
