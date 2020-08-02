#pragma once

#include "MoreInformativeConsole/ExtraInfoEntry.h"

#include "RE/Skyrim.h"
#include "SKSE/Logger.h"

const char deliminator = '\\';

std::string GetFileName(std::string filePath);
void CreateFilePathSubarray(ExtraInfoEntry* mainEntry, std::string filePath);

std::string GetRuntimePath();
const std::string& GetRuntimeDirectory();

