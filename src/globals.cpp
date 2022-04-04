#include "globals.h"

namespace MICGlobals
{
	bool readRaceSkins = true;                //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	bool readEffectCaster = true;              //Enable/Disabling reading the caster entry for active effects. Used to prevent an infinite loop of reading Effect->Caster->Effect->Caster
	RE::TESRace* filterARMAByRace = nullptr;  //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	bool useEditorIDs=true; //True if power of 3 tweaks is installed allowing editor ids to be read in from memory
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
