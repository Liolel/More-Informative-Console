#pragma once
#include "RE/Skyrim.h"

//Methods for converting various form data into user readable strings

std::string GetName(RE::TESForm* pBaseForm);
std::string GetFormTypeName(int formType);