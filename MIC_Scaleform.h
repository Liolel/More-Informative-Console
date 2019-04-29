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
#include <vector>
#include "ExtraInfoEntry.h"

namespace MICGlobals
{
	extern boolean readRaceSkins; //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	extern TESRace* filterARMAByRace; //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	extern boolean reducedMode; //Used to reduce the amount of info read to prevent crashes on opening chests with very large number of armors in certain circumstances
	extern int maxInventoryBeforeReducedMode; //Maximum inventory size before triggering reduced mode
	extern ExtraInfoEntry rootEntry;
}


namespace moreInformativeConsoleScaleForm
{
	bool InstallHooks(GFxMovieView * view, GFxValue * root);

}

struct FormModInfoData
{
	ModInfo ** entries;     // array of ModInfo* - mods that change this form.
	UInt64  size;
};