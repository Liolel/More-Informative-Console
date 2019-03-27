#pragma once
#include "skse64/PluginAPI.h"
#include "MIC_Scaleform.h"
#include "Util.h"
#include "Simpleini.h"
#include <shlobj.h>

IDebugLog					gLog;
const char*					kLogPath = "\\My Games\\Skyrim Special Edition\\Logs\\More Informative Console.log";
PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
SKSEScaleformInterface*		g_SKSEScaleformInterface = NULL;

extern "C"
{
	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, kLogPath);

		_MESSAGE("More Informative Console");
		_MESSAGE("Initalizing");

		//Populate the info strucutre
		info->infoVersion	= PluginInfo::kInfoVersion;
		info->name			= "More Informative Console";
		info->version		= 0.2;

		//Store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		//Runtime error checks
		if(skse->isEditor)
			{ _MESSAGE("Plugin loaded in editor"); return false; }

		// all is well
		return true;
	}

	void readINI()
	{
		//Read ini
		const std::string& iniPath = GetRuntimeDirectory() + R"(Data\SKSE\plugins\MoreInformativeConsole.ini)";

		CSimpleIniA ini;
		SI_Error iniError = ini.LoadFile(iniPath.c_str());

		if (iniError < 0)
		{
			_MESSAGE("Unable to read the ini file at path %s. Default values will be used", iniPath.c_str());
		}

		else
		{
			_MESSAGE("Reading in ini file");
			MICOptions::MICDebugMode = ini.GetBoolValue("Debug", "EnableDebugLogging", false);
			MICOptions::Transparency = (double)ini.GetLongValue("UI", "Transparency", false) / 100.0;
			MICOptions::FieldsToDisplay = ini.GetLongValue("UI", "FieldsToDisplay", false);
			MICOptions::Scale = (double)ini.GetLongValue("UI", "Scale", false) / 100.0;
			MICOptions::BaseInfoFormat = ini.GetLongValue("UI", "BaseInfoFormat", false);
		}
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{
		_MESSAGE("Establishing interfaces...");

		SKSEScaleformInterface::RegisterCallback callback = moreInformativeConsoleScaleForm::InstallHooks;

		g_SKSEScaleformInterface = (SKSEScaleformInterface *)skse->QueryInterface(kInterface_Scaleform);
		g_SKSEScaleformInterface->Register("MIC", callback);

		readINI();

		_MESSAGE("Plugin Initialization complete.");
		return true;
	}
};