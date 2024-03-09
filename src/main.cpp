#pragma once
#include "EditorIDCache.h"
#include "MIC_Scaleform.h"
#include "SKSE/API.h"
#include "Simpleini.h"
#include "TranslationCache.h"
#include "globals.h"
#include <stddef.h>
#include <SKSE/SKSE.h>

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace
{
	void readINI()
	{
		//Read ini
		constexpr auto path = L"Data/SKSE/Plugins/MoreInformativeConsole.ini";

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.SetMultiKey();

		SI_Error iniError = ini.LoadFile(path);

		if (iniError < 0)
		{
			logger::info("Unable to read the ini file. Default values will be used");
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
			MICOptions::DisableEditorIDs = ini.GetBoolValue("Performance", "DisableEditorIDs", false);
			MICOptions::DisableScriptsAliases = ini.GetBoolValue("Performance", "DisableScriptsAliases", false);
			MICOptions::DisableScriptsAliasesPlayerOnly = ini.GetBoolValue("Performance", "DisableScriptsAliasesPlayerOnly", false);
		}
	}

	void MessageHandler(SKSE::MessagingInterface::Message* a_message)
	{
		logger::info("Processed message");
		if (a_message->type == SKSE::MessagingInterface::kDataLoaded)
		{
			auto editorIDCache = EditorIDCache::GetSingleton();
			editorIDCache->CacheEditorIDs();
			logger::info("Cached editor ids");
		}
	}
}


namespace Plugin
{
	using namespace std::literals;

	inline constexpr REL::Version VERSION
	{
		// clang-format off
		1,
		2,
		0,
		// clang-format on
	};

	inline constexpr auto NAME = "@PROJECT_NAME@"sv;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion( Plugin::VERSION );
	v.PluginName("More Informative Console");
	v.AuthorName("Linthar");
	v.UsesAddressLibrary();
	v.HasNoStructUse();
	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
		readINI();

		auto path = log_directory();
		if (!path) {
			report_and_fail("Unable to lookup SKSE logs directory.");
		}
		*path /= PluginDeclaration::GetSingleton()->GetName();
		*path += L".log";

		std::shared_ptr<spdlog::logger> log;
		if (IsDebuggerPresent()) {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
		} else {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
		}

		//Set the logger print level based on if debug mode is enabled
		if (MICOptions::MICDebugMode) {
			log->set_level(spdlog::level::level_enum::debug);
			log->flush_on(spdlog::level::level_enum::debug);
		}

		else {
			log->set_level(spdlog::level::level_enum::info);
			log->flush_on(spdlog::level::level_enum::info);
		}

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");

		auto* plugin = PluginDeclaration::GetSingleton();
		auto version = plugin->GetVersion();
		log::info("{} {} is loading...", plugin->GetName(), version);
		
		if (a_skse->IsEditor()) {
			logger::critical("Loaded in editor, marking as incompatible"sv);
			return false;
		}

		logger::info("Before Translation");

		auto translationCache = TranslationCache::GetSingleton();
		translationCache->CacheTranslations();

		logger::info("Establishing interfaces...");

		SKSE::Init(a_skse);
		const auto scaleform = SKSE::GetScaleformInterface();

		scaleform->Register(moreInformativeConsoleScaleForm::InstallHooks, "MIC");

		if (!MICOptions::DisableEditorIDs) {
			auto messaging = SKSE::GetMessagingInterface();
			messaging->RegisterListener(MessageHandler);
		}
		logger::info("Plugin Initialization complete.");

		return true;
}
