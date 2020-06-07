#pragma once

#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"

class MICScaleform_RetrieveExtraData : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
private:
	ExtraInfoEntry* TraverseExtraInfoEntries(ExtraInfoEntry* currentEntry, RE::GFxValue* indexArray, int currentIndex);
};
