#pragma once

#include "MICScaleform_Log.h"
#include "Util/GeneralUtil.h"

//4-23-2022: Checked for translations needed

void MICScaleform_Log::Call(Params& a_params)
{
	std::string logString = a_params.args[0].GetString();
	logger::debug("%s", logString);
}
