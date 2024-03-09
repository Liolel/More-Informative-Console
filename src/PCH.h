#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <Util/GeneralUtil.h>
#include <robin_hood.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)
