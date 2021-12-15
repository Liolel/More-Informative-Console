cmake_minimum_required(VERSION 3.20)
set(NAME "MoreInformativeConsole")
set(VERSION 0.43.0)

# ---- Options ----

option(COPY_BUILD "Copy the build output to the Skyrim directory." OFF)
option(BUILD_SKYRIMVR "Build for Skyrim VR" OFF)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(VCPKG_ROOT)

macro(find_commonlib_path)
	if (CommonLibName AND NOT ${CommonLibName} STREQUAL "")
		# Check extern
		find_path(CommonLibPath
		include/REL/Relocation.h
		PATHS extern/${CommonLibName})
		if (${CommonLibPath} STREQUAL "CommonLibPath-NOTFOUND")
			#Check path
			set_from_environment(${CommonLibName}Path)
			set(CommonLibPath ${${CommonLibName}Path})
		endif()
	endif()
endmacro()

if(BUILD_SKYRIMVR)
	add_compile_definitions(SKYRIMVR)
	set(CommonLibName "CommonLibVR")
	set_from_environment(SkyrimVRPath)
	set(SkyrimPath ${SkyrimVRPath})
	set(SkyrimVersion "Skyrim VR")
	set(VERSION "${VERSION}.1")
else()
	set(CommonLibName "CommonLibSSE")
	set_from_environment(Skyrim64Path)
	set(SkyrimPath ${Skyrim64Path})
	set(SkyrimVersion "Skyrim SSE")
endif()
find_commonlib_path()
message(
	STATUS
	"Building for ${SkyrimVersion} at ${SkyrimPath} with ${CommonLibName} at ${CommonLibPath}."
)


if (DEFINED VCPKG_ROOT)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
else ()
	message(
		WARNING
		"Variable VCPKG_ROOT is not set. Continuing without vcpkg."
	)
endif ()

set(Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# ---- Globals ----

if (MSVC)
	add_compile_definitions(
		_UNICODE
	)

	if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
		add_compile_options(
			/MP	# Build with Multiple Processes
		)
	endif ()
endif ()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_STATIC_LIBS ON)


# ---- Dependencies ----
if (DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${CommonLibPath})
	add_subdirectory(${CommonLibPath} ${CommonLibName})
else ()
	message(
		FATAL_ERROR
		"Variable ${CommonLibName}Path is not set or in extern/."
	)
endif()

find_package(spdlog REQUIRED)
find_package(binary_io REQUIRED CONFIG)

# ---- Add source files ----

set(SOURCES
	src/main.cpp
	src/PCH.h
	src/ExtraInfoEntry.cpp
	src/ExtraInfoEntry.h
	src/globals.cpp
	src/globals.h
	src/MIC_Scaleform.cpp
	src/MIC_Scaleform.h
	src/Scaleform/MICScaleform_GetExtraData.cpp
	src/Scaleform/MICScaleform_GetExtraData.h
	src/Scaleform/MICScaleform_GetIniOptions.cpp
	src/Scaleform/MICScaleform_GetIniOptions.h
	src/Scaleform/MICScaleform_GetReferenceInfo.cpp
	src/Scaleform/MICScaleform_GetReferenceInfo.h
	src/Scaleform/MICScaleform_Log.cpp
	src/Scaleform/MICScaleform_Log.h
	src/Scaleform/MICScaleform_RetrieveExtraData.cpp
	src/Scaleform/MICScaleform_RetrieveExtraData.h
	src/Util/FilePathUtil.h
	src/Util/FilePathUtil.cpp
	src/Util/GeneralUtil.h
	src/Util/GeneralUtil.cpp
	src/Util/NameUtil.h
	src/Util/NameUtil.cpp
	src/Util/ScaleFormUtil.h
	src/Util/ScaleFormUtil.cpp
	src/TESForms/BGSMusicTrack.cpp
	src/TESForms/BGSMusicTrack.h
	src/TESForms/BGSTextureSet.cpp
	src/TESForms/BGSTextureSet.h
	src/TESForms/EffectSetting.cpp
	src/TESForms/EffectSetting.h
	src/TESForms/MagicItem.cpp
	src/TESForms/MagicItem.h
	src/TESForms/SpellItem.cpp
	src/TESForms/SpellItem.h
	src/TESForms/TESAmmo.cpp
	src/TESForms/TESAmmo.h
	src/TESForms/TESForm.cpp
	src/TESForms/TESForm.h
	src/TESForms/TESModel.cpp
	src/TESForms/TESModel.h
	src/TESForms/TESModelTextureSwap.cpp
	src/TESForms/TESModelTextureSwap.h
	src/TESForms/TESNPC.cpp
	src/TESForms/TESNPC.h
	src/TESForms/TESObjectARMA.cpp
	src/TESForms/TESObjectARMA.h
	src/TESForms/TESObjectARMO.cpp
	src/TESForms/TESObjectARMO.h
	src/TESForms/TESObjectCELL.cpp
	src/TESForms/TESObjectCELL.h
	src/TESForms/TESObjectCONT.cpp
	src/TESForms/TESObjectCONT.h
	src/TESForms/TESObjectREFR.cpp
	src/TESForms/TESObjectREFR.h
	src/TESForms/TESObjectWEAP.cpp
	src/TESForms/TESObjectWEAP.h
	src/TESForms/TESRace.cpp
	src/TESForms/TESRace.h
	src/BSExtraData/BSExtraData.cpp
	src/BSExtraData/BSExtraData.h
	src/BSExtraData/ExtraContainerChanges.cpp
	src/BSExtraData/ExtraContainerChanges.h
	src/BSExtraData/ExtraEnableStateParent.cpp
	src/BSExtraData/ExtraEnableStateParent.h
	src/BSExtraData/ExtraLock.cpp
	src/BSExtraData/ExtraLock.h
	src/BSExtraData/ExtraOwnership.cpp
	src/BSExtraData/ExtraOwnership.h
	src/BSExtraData/ExtraTeleport.cpp
	src/BSExtraData/ExtraTeleport.h
)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES}
)

source_group(
	TREE
		${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# ---- Create DLL ----

add_library(
	${PROJECT_NAME}
	SHARED
	${SOURCES}
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_20
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		${CommonLibName}::${CommonLibName}
		spdlog::spdlog
		binary_io::binary_io
)

if (MSVC)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/sdl	# Enable Additional Security Checks
			/utf-8	# Set Source and Executable character sets to UTF-8
			/Zi	# Debug Information Format

			/permissive-	# Standards conformance

			/Zc:alignedNew	# C++17 over-aligned allocation
			/Zc:auto	# Deduce Variable Type
			/Zc:char8_t
			/Zc:__cplusplus	# Enable updated __cplusplus macro
			/Zc:externC
			/Zc:externConstexpr	# Enable extern constexpr variables
			/Zc:forScope	# Force Conformance in for Loop Scope
			/Zc:hiddenFriend
			/Zc:implicitNoexcept	# Implicit Exception Specifiers
			/Zc:lambda
			/Zc:noexceptTypes	# C++17 noexcept rules
			/Zc:preprocessor	# Enable preprocessor conformance mode
			/Zc:referenceBinding	# Enforce reference binding rules
			/Zc:rvalueCast	# Enforce type conversion rules
			/Zc:sizedDealloc	# Enable Global Sized Deallocation Functions
			/Zc:strictStrings	# Disable string literal type conversion
			/Zc:ternary	# Enforce conditional operator rules
			/Zc:threadSafeInit	# Thread-safe Local Static Initialization
			/Zc:tlsGuards
			/Zc:trigraphs	# Trigraphs Substitution
			/Zc:wchar_t	# wchar_t Is Native Type

			/external:anglebrackets
			/external:W0

			/W4	# Warning level
			/WX	# Warning level (warnings are errors)

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			/WX	# Treat Linker Warnings as Errors

			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif ()

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

# ---- Post build ----

if (COPY_BUILD)
	if (DEFINED SkyrimPath)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${SkyrimPath}/Data/SKSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${SkyrimPath}/Data/SKSE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable ${SkyrimPath} is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
