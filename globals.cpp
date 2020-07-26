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
	//int BackgroundColor = 0x3d3d3d; //Background color of the extra info window
	double Transparency = 0.8; //Transperency of the extra info window background
	double Scale = 1; //Scale of the extra info window
	int FieldsToDisplay = 18; //Number of text fields to display
	int BaseInfoFormat = 2; //How much information to display in the base info window
}