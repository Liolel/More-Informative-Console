#pragma once

#include "MICScaleform_Log.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"

void MICScaleform_Log::Call(Params& a_params)
{
	_DMESSAGE(a_params.args[0].GetString());
}