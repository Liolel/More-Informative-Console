#include "globals.h"


namespace MICGlobals
{
	boolean readRaceSkins = true; //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	RE::TESRace* filterARMAByRace = nullptr; //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	boolean reducedMode = false; //Used to reduce the amount of info read to prevent crashes on opening chests with very large number of armors in certain circumstancesv
	int maxInventoryBeforeReducedMode = 750; //Maximum inventory size before triggering reduced mode
	ExtraInfoEntry rootEntry("", "", priority_Default);
}

namespace MICOptions
{
	boolean MICDebugMode = false; //Enables debugging code. Currently this just activates the logging features
	boolean ExperimentalFeatures = false; //Enable experimental features
	double Transparency = 0.8; //Transperency of the extra info window background
	extern int WindowWidth = 300; //Width of the extra info window
	extern int WindowHeight = 600; //Heigh of the extra info window
	extern int FontSizeExtraInfo = 16; //Size of the font used in the extra info window
	extern int FontSizeBaseInfo = 16; //Size of the font used in the base info
	extern int FontSizeConsoleText = 16; //Size of the font used in the console text
	int BaseInfoFormat = 2; //How much information to display in the base info window
}