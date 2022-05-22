#pragma once

#include "ExtraInfoEntry.h"

namespace MICGlobals
{
	extern bool minimizeFormDataRead;     //Enable/Disabling reading the caster entry for active effects. Used to prevent an infinite loop of reading Effect->Caster->Effect->Caster
	extern RE::TESRace* filterARMAByRace;  //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	extern ExtraInfoEntry rootEntry;
}

namespace MICOptions
{
	extern bool MICDebugMode; //Enables debugging code. Currently this just activates the logging features
	extern bool ExperimentalFeatures; //Enable experimental features
	//extern int BackgroundColor; //Background color of the extra info window
	extern double Transparency; //Transperency of the extra info window background
	extern int BaseInfoFormat; //How much information to display in the base info window
	extern int WindowWidth; //Width of the extra info window
	extern int WindowHeight; //Heigh of the extra info window	
	extern int FontSizeExtraInfo; //Size of the font used in the extra info window
	extern int FontSizeBaseInfo; //Size of the font used in the base info
	extern int FontSizeConsoleText; //Size of the font used in the console text
	extern int DisableEditorIDs;
	extern int DisableScripts;
	extern int DisableTextures;
}
