#pragma once

#include "RE//Skyrim.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "ExtraInfoEntry.h"

namespace moreInformativeConsoleScaleForm
{
	bool InstallHooks(RE::GFxMovieView* a_view, RE::GFxValue* a_root);

}

/*
struct FormModInfoData
{
	ModInfo ** entries;     // array of ModInfo* - mods that change this form.
	UInt64  size;
};

//https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/88daa12f2e20a5e7cd545c922e2449f3bfcafb63/include/RE/ExtraLock.h
struct LockState
{
	enum Flag
	{
		kNone = 0,
		kLocked = 1,
		kLevelScaling = 1 << 4
	};

	// members
	UInt8	lockLevel;	// 00
	UInt8	unk01;		// 01
	UInt16	unk02;		// 02
	UInt32	unk04;		// 04
	TESKey* key;		// 08
	UInt8	flags;		// 10
	UInt8	unk11;		// 11
	UInt16	unk12;		// 12
	UInt32	unk14;		// 14
	UInt32	unk18;		// 18
	UInt32	unk1C;		// 1C
};
STATIC_ASSERT(sizeof(LockState) == 0x20);

class ExtraLock : public BSExtraData
{
public:

	enum { kExtraTypeID = kExtraData_Lock};

	// members
	LockState* state;  // 10
};
STATIC_ASSERT(sizeof(ExtraLock) == 0x18);

class ExtraTeleport : public BSExtraData
{
public:
	enum { kExtraTypeID = kExtraData_Teleport};


	struct Data
	{
		UInt32   	dest;	// 00
		float		unk04;	// 04
		float		unk08;	// 08
		float		unk0C;	// 0C
		float		unk10;	// 10
		float		unk14;	// 14
		float		unk18;	// 18
		UInt8		unk1C;	// 1C
		UInt8		pad1D;	// 1D
		UInt16		pad1E;	// 1E
	};
	STATIC_ASSERT(sizeof(Data) == 0x20);


	//virtual ~ExtraTeleport();														// 00

	// override (BSExtraData)
	//virtual ExtraDataType	GetType() const override;								// 01 - { return kTeleport; }
	//virtual bool			IsNotEqual(const BSExtraData* a_rhs) const override;	// 02


	// members
	Data* data;	// 10
};
STATIC_ASSERT(sizeof(ExtraTeleport) == 0x18);

//https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/TESObjectCELL.h

struct Coordinates	// XCLC
{
	SInt32	cellX;			// 00
	SInt32	cellY;			// 04
	char* maxHeightData;	// 08
	UInt64	unk18;			// 10
	float	worldX;			// 18
	float	worldY;			// 1C
	UInt64	unk20;			// 20
};
STATIC_ASSERT(sizeof(Coordinates) == 0x28);


union LightingCoordinates
{
	UInt32* lighting;		// XCLL
	Coordinates* coordinates;	// XCLC
};
STATIC_ASSERT(sizeof(LightingCoordinates) == 0x8);

//https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/BSIMusicTrack.h
enum class MUSIC_STATUS : UInt32
{
	kInactive = 0,
	kPlaying = 1,
	kPaused = 2,
	kFinishing = 3,
	kFinished = 4
};

/*
class BGSMusicTrack : public BSIMusicTrack
{
public:
	
	virtual ~BGSMusicTrack();						// 00

	// override (BSIMusicTrack)
	virtual bool	TestCanPlay();			// 07 - { return conditions ? conditions->Run(g_thePlayer, g_thePlayer) : true; }

	// add
	virtual bool	InitTrack(TESFile* a_mod);		// 0B
	virtual bool	LoadTrack(TESFile* a_mod) = 0;	// 0C


	// members
	Condition *	conditions;	// 10
	UInt64		unk18;		// 18
};
STATIC_ASSERT(sizeof(BGSMusicTrack) == 0x20);*/
