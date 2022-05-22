#pragma once
#include "MIC_Scaleform.h"
#include "SKSE/API.h"
#include "Simpleini.h"
#include "globals.h"
#include "EditorIDCache.h"
#include "TranslationCache.h"

void readINI()
{
	//Read ini
	constexpr auto path = L"Data/SKSE/Plugins/MoreInformativeConsole.ini";

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey();

	SI_Error iniError = ini.LoadFile(path);

	if (iniError < 0) {
		logger::info("Unable to read the ini file. Default values will be used");
	}

	else {
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
		MICOptions::DisableEditorIDs = ini.GetBoolValue("Experimental", "DisableEditorIDs", false);
		MICOptions::DisableScripts = ini.GetBoolValue("Experimental", "DisableTextures", false);
		MICOptions::DisableTextures = ini.GetBoolValue("Experimental", "DisableScripts", false);
	}
}

void MessageHandler(SKSE::MessagingInterface::Message* a_message)
{
	logger::info("Processed message");
	if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
		auto editorIDCache = EditorIDCache::GetSingleton();
		editorIDCache->CacheEditorIDs();
		logger::info("Cached editor ids");
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
	readINI();
	//Set the logger print level based on if debug mode is enabled
	if (MICOptions::MICDebugMode) {
		log->set_level(spdlog::level::debug);
		log->flush_on(spdlog::level::debug);
	}

	else {
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
	}
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
	logger::info("Initalizing");

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver <
#ifndef SKYRIMVR
		SKSE::RUNTIME_1_5_39
#else
		SKSE::RUNTIME_VR_1_4_15
#endif
	) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"sv), ver.string());
		return false;
	}

	auto translationCache = TranslationCache::GetSingleton();
	translationCache->CacheTranslations();

	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("Establishing interfaces...");

	readINI();

	SKSE::Init(a_skse);
	const auto scaleform = SKSE::GetScaleformInterface();

	scaleform->Register(moreInformativeConsoleScaleForm::InstallHooks, "MIC");
	
	if (!MICOptions::DisableEditorIDs)
	{
		auto messaging = SKSE::GetMessagingInterface();
		messaging->RegisterListener(MessageHandler);
	}
	logger::info("Plugin Initialization complete.");

	return true;
}
