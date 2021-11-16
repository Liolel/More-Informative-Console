#pragma once

#include "MICScaleform_GetIniOptions.h"
#include "Util/ScaleformUtil.h"
#include "globals.h"


void MICScaleform_GetIniOptions::Call(Params& a_params)
{
	logger::debug("GetIniOptions:: Called");

	//Retrieve the various scaleform objects needed from the parameters
	RE::GFxValue* results = a_params.retVal;
	RE::GFxMovie* movie = a_params.movie;

	movie->CreateObject(results);

	RegisterNumber(results, "Transparency", MICOptions::Transparency);
	RegisterNumber(results, "FontSizeExtraInfo", MICOptions::FontSizeExtraInfo);
	RegisterNumber(results, "FontSizeBaseInfo", MICOptions::FontSizeBaseInfo);
	RegisterNumber(results, "FontSizeConsoleText", MICOptions::FontSizeConsoleText);
	RegisterNumber(results, "WindowHeight", MICOptions::WindowHeight);
	RegisterNumber(results, "WindowWidth", MICOptions::WindowWidth);
	RegisterNumber(results, "BaseInfoFormat", MICOptions::BaseInfoFormat);
}