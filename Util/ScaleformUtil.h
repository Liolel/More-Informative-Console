#pragma once
#include "CommonLibSSE/include/RE/Skyrim.h"

//Register a string as part of a scaleform object
void RegisterString(RE::GFxValue* object, RE::GFxMovie* movie, std::string stringName, std::string stringContents);
void RegisterNumber(RE::GFxValue* object, std::string numberName, double number);