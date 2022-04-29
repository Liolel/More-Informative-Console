#pragma once

#include "FilePathUtil.h"

//4-29-2022: Checked for translations needed

std::string GetFileName(std::string filePath)
{
	//get the name of the file

	long lastSlash = (long)filePath.find_last_of(deliminator);

	std::string fileName = "";

	if (lastSlash != std::string::npos) {
		fileName = filePath.substr((long)lastSlash + 1);
	}
	//its unlikely but if the model is not in any folder its name is the same as the path
	else {
		fileName = filePath;
	}

	return fileName;
}

void CreateFilePathSubarray(ExtraInfoEntry* mainEntry, std::string filePath)
{
	logger::debug(("Starting CreateFilePathSubarray " + filePath).c_str());

	//loop through the string until the last slash is found
	long firstSlash = (long)filePath.find_first_of(deliminator);
	while (firstSlash != std::string::npos) {
		ExtraInfoEntry* pathEntry;
		std::string path = filePath.substr(0, (long)firstSlash + 1);

		CreateExtraInfoEntry(pathEntry, path, "", priority_Default);
		mainEntry->PushBack(pathEntry);

		filePath = filePath.substr((long)firstSlash + 1);        //get everything after the first slash
		firstSlash = (long)filePath.find_first_of(deliminator);  //refind the first slash
	}

	ExtraInfoEntry* pathEntry;
	//add everything after the last slash
	CreateExtraInfoEntry(pathEntry, filePath, "", priority_Default);

	mainEntry->PushBack(pathEntry);

	logger::debug("Ending CreateFilePathSubarray");
}
