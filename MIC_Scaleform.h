#pragma once

//#include "GameReferences.h"
#include "skse64/GameFormComponents.h"
#include "skse64/Hooks_Scaleform.h"
#include "skse64/ScaleformAPI.h"
#include "skse64/ScaleformCallbacks.h"
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformMovie.h"
#include "skse64/ScaleformState.h"
#include "skse64/ScaleformTypes.h"
#include "skse64/ScaleformValue.h"
#include "skse64/ScaleformVM.H"
#include "skse64/GameForms.h"
#include "skse64/GameData.h"
#include <string>
#include <iomanip>
#include <sstream>

namespace moreInformativeConsoleScaleForm
{
	bool InstallHooks(GFxMovieView * view, GFxValue * root);

}

struct FormModInfoData
{
	ModInfo ** entries;     // array of ModInfo* - mods that change this form.
	UInt64  size;
};