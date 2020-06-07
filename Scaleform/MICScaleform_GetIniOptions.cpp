#pragma once

#include "MICScaleform_GetIniOptions.h"
#include "MoreInformativeConsole/Util/ScaleformUtil.h"
#include "MoreInformativeConsole/globals.h"


void MICScaleform_GetIniOptions::Call(Params& a_params)
{
	_DMESSAGE("GetIniOptions:: Called");

	//Retrieve the various scaleform objects needed from the parameters
	RE::GFxValue* results = a_params.retVal;
	RE::GFxMovie* movie = a_params.movie;

	movie->CreateObject(results);

	RegisterNumber(results, "Transparency", MICOptions::Transparency);
	RegisterNumber(results, "Scale", MICOptions::Scale);
	RegisterNumber(results, "FieldsToDisplay", MICOptions::FieldsToDisplay);
	RegisterNumber(results, "BaseInfoFormat", MICOptions::BaseInfoFormat);
}