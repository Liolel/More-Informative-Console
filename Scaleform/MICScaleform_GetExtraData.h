#pragma once

#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

const int Constant_ModeConsoleHandle = 0;
const int Constant_ModeWorldInformation = 1;
const int Constant_ModeMFG = 2;

class MICScaleform_GetExtraData : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
};

void GetWorldData(ExtraInfoEntry* resultArray);