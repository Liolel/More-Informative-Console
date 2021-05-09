#pragma once
#include "SKSE/API.h"
#include "MIC_Scaleform.h"
#include "globals.h"
#include "Simpleini.h"
#include "Util/FilePathUtil.h"
#include <shlobj.h>

//					gLog;
const char*					kLogPath = "\\My Games\\Skyrim Special Edition\\SKSE\\More Informative Console.log";
//PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
//SKSEScaleformInterface*		g_SKSEScaleformInterface = NULL;


void readINI()
{
	//Read ini
	const std::string& iniPath = GetRuntimeDirectory() + "Data\\SKSE\\plugins\\MoreInformativeConsole.ini";

	CSimpleIniA ini;
	SI_Error iniError = ini.LoadFile(iniPath.c_str());

	if (iniError < 0)
	{
		logger::info("Unable to read the ini file at path %s. Default values will be used", iniPath.c_str());
	}

	else
	{
		logger::info("Reading in ini file");
		MICOptions::MICDebugMode = ini.GetBoolValue("Debug", "EnableDebugLogging", false);
		MICOptions::ExperimentalFeatures = ini.GetBoolValue("Experimental", "EnableExperimentalFeatures", false);
		MICOptions::Transparency = (double)ini.GetLongValue("UI", "Transparency", false) / 100.0;
		MICOptions::WindowHeight = ini.GetLongValue("UI", "WindowHeight", false);
		MICOptions::WindowWidth = ini.GetLongValue("UI", "WindowWidth", false);
		MICOptions::FontSizeExtraInfo = ini.GetLongValue("UI", "FontSizeExtraInfo", false);
		MICOptions::FontSizeBaseInfo = ini.GetLongValue("UI", "FontSizeBaseInfo", false);
		MICOptions::FontSizeConsoleText = ini.GetLongValue("UI", "FontSizeConsoleText", false);
		MICOptions::BaseInfoFormat = ini.GetLongValue("UI", "BaseInfoFormat", false);
	}
}

extern "C" DLLEXPORT bool SKSEAPI  SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info)
{
	
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "MoreInformationConsole.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	
	readINI(); //Get the INI now because I need values from that to finish setting up the logging

	//Set the logger print level based on if debug mode is enabled
	if (MICOptions::MICDebugMode)
	{
		log->set_level(spdlog::level::debug);
		log->flush_on(spdlog::level::debug);
	}

	else
	{
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
	}

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info("More Informative Console");
	logger::info("Initalizing");
	
	//Populate the info strucutre
	
	info->infoVersion	= SKSE::PluginInfo::kVersion;
	info->name			= "More Informative Console";
	info->version		= 4;

	//Store plugin handle so we can identify ourselves later
	//g_pluginHandle = skse->GetPluginHandle();

	//Runtime error checks
	if(skse->IsEditor())
		{ logger::info("Plugin loaded in editor"); return false; }

	// all is well
	return true;
}

extern "C" DLLEXPORT bool SKSEAPI  SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("Establishing interfaces...");

	SKSE::Init(a_skse);
		
	const auto scaleform = SKSE::GetScaleformInterface();

	scaleform->Register(moreInformativeConsoleScaleForm::InstallHooks, "MIC");

	logger::info("Plugin Initialization complete.");

	return true;
}
