#include "globals.h"

namespace MICGlobals
{
	bool readRaceSkins = true;                //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	bool minimizeFormDataRead = false;         //Enable/Disabling reading full form data in, for situations where the only important information is the actual form
	RE::TESRace* filterARMAByRace = nullptr;  //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	ExtraInfoEntry rootEntry("", "", priority_Default);
}

namespace MICOptions
{
	bool MICDebugMode = false;            //Enables debugging code. Currently this just activates the logging features
	bool ExperimentalFeatures = false;    //Enable experimental features
	double Transparency = 0.8;            //Transperency of the extra info window background
	extern int WindowWidth = 300;         //Width of the extra info window
	extern int WindowHeight = 600;        //Heigh of the extra info window
	extern int FontSizeExtraInfo = 16;    //Size of the font used in the extra info window
	extern int FontSizeBaseInfo = 16;     //Size of the font used in the base info
	extern int FontSizeConsoleText = 16;  //Size of the font used in the console text
	int BaseInfoFormat = 2;               //How much information to display in the base info window
}
