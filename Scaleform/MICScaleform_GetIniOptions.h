#pragma once

#include "RE/Skyrim.h"

class MICScaleform_GetIniOptions : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
};