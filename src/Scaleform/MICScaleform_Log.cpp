#pragma once

#include "MICScaleform_Log.h"
#include "Util/GeneralUtil.h"

void MICScaleform_Log::Call(Params& a_params)
{
	logger::debug(a_params.args[0].GetString());
}