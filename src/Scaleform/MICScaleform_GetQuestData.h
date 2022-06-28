#pragma once

#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

class MICScaleform_GetQuestData : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
};
