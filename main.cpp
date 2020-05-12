#pragma once
#include "SKSE/API.h"
#include "SKSE/Logger.h"
#include "skse64_common/Utilities.h"
#include "MIC_Scaleform.h"
#include "Util.h"
#include "Simpleini.h"
#include <shlobj.h>

//					gLog;
const char*					kLogPath = "\\My Games\\Skyrim Special Edition\\SKSE\\More Informative Console.log";
//PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
//SKSEScaleformInterface*		g_SKSEScaleformInterface = NULL;

extern "C"
{
	bool SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, kLogPath);

		_MESSAGE("More Informative Console");
		_MESSAGE("Initalizing");

		//Populate the info strucutre
		info->infoVersion	= SKSE::PluginInfo::kVersion;
		info->name			= "More Informative Console";
		info->version		= 0.4;

		//Store plugin handle so we can identify ourselves later
		//g_pluginHandle = skse->GetPluginHandle();

		//Runtime error checks
		if(skse->IsEditor())
			{ _MESSAGE("Plugin loaded in editor"); return false; }

		// all is well
		return true;
	}

	void readINI()
	{
		//Read ini
		const std::string& iniPath = GetRuntimeDirectory() + "\\Data\\SKSE\\plugins\\MoreInformativeConsole.ini";

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

	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("Establishing interfaces...");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		
		const auto scaleform = SKSE::GetScaleformInterface();

		scaleform->Register(moreInformativeConsoleScaleForm::InstallHooks, "MIC");
		
		/*
		RE::UI::GetSingleton()->Register

		scaleform->Register("MIC", moreInformativeConsoleScaleForm::InstallHooks)

		//SKSEScaleformInterface::RegisterCallback callback = moreInformativeConsoleScaleForm::InstallHooks;

		/*
		_MESSAGE("Establishing interfaces 2..." );

		g_SKSEScaleformInterface = (SKSEScaleformInterface *)skse->QueryInterface(kInterface_Scaleform);


		_MESSAGE("Establishing interfaces 3...");
		g_SKSEScaleformInterface->Register("MIC", callback);*/
		readINI();

		//Console::Register();

		_MESSAGE("Plugin Initialization complete.");
		return true;
	}
};