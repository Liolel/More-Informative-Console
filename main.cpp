#include "skse64/PluginAPI.h"
#include "MIC_Scaleform.h"
//#include "skse64/skse_version.h"
#include <shlobj.h>

IDebugLog					gLog;
const char*					kLogPath = "\\My Games\\Skyrim Special Edition\\Logs\\More Informative Console.log";
PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
SKSEScaleformInterface*		g_SKSEScaleformInterface = NULL;

boolean	MICDebugMode = false;

extern "C"
{
	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, kLogPath);

		_MESSAGE("More Informative Console");
		_MESSAGE("Loading...");

		//Populate info structure
		info->infoVersion	= PluginInfo::kInfoVersion;
		info->name			= "More Informative Console";
		info->version		= 1;

		//Store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		//Runtime error checks
		if(skse->isEditor)
			{ _MESSAGE("Loaded In Editor, Marking As Incompatible"); return false; }

		// all is well
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{
		_MESSAGE("Establishing interfaces...");

		SKSEScaleformInterface::RegisterCallback callback = moreInformativeConsoleScaleForm::InstallHooks;

		g_SKSEScaleformInterface = (SKSEScaleformInterface *)skse->QueryInterface(kInterface_Scaleform);
		g_SKSEScaleformInterface->Register("MIC", callback);

		_MESSAGE("Plugin Initialization complete.");
		return true;
	}
};