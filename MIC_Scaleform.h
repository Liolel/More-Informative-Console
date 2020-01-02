#pragma once

//#include "GameReferences.h"
#include "skse64/GameFormComponents.h"
#include "skse64/Hooks_Scaleform.h"
#include "skse64/ScaleformAPI.h"
#include "skse64/ScaleformCallbacks.h"
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformMovie.h"
#include "skse64/ScaleformState.h"
#include "skse64/ScaleformTypes.h"
#include "skse64/ScaleformValue.h"
#include "skse64/ScaleformVM.H"
#include "skse64/GameForms.h"
#include "skse64/GameData.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "ExtraInfoEntry.h"

namespace MICGlobals
{
	extern boolean readRaceSkins; //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	extern TESRace* filterARMAByRace; //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	extern boolean reducedMode; //Used to reduce the amount of info read to prevent crashes on opening chests with very large number of armors in certain circumstances
	extern int maxInventoryBeforeReducedMode; //Maximum inventory size before triggering reduced mode
	extern ExtraInfoEntry rootEntry;
}


namespace moreInformativeConsoleScaleForm
{
	bool InstallHooks(GFxMovieView * view, GFxValue * root);

}

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