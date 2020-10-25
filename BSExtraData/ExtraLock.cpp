#include "ExtraLock.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"

void ProcessLockData(ExtraInfoEntry* resultArray, RE::BSExtraData* data, RE::TESObjectREFR* refForm)
{
	logger::debug("Starting GetLockData");

	RE::ExtraLock* extraLock = static_cast<RE::ExtraLock*>(data);

	if (extraLock
		&& extraLock->lock)
	{
		RE::REFR_LOCK* lock = extraLock->lock;

		ExtraInfoEntry* lockEntry;

		CreateExtraInfoEntry(lockEntry, "Lock", "", priority_ExtraLock_Lock);

		/*
		bool isLocked = HasFlag((int)lock->flags, (int)RE::REFR_LOCK::Flag::kLocked);
		ExtraInfoEntry* isLockedEntry;
		CreateExtraInfoEntry(isLockedEntry, "Is locked", BooleanToYesNoString(isLocked), priority_Lock_IsLocked);
		lockEntry->PushBack(isLockedEntry);*/

		RE::LOCK_LEVEL lockLevel = lock->GetLockLevel( refForm );
		std::string lockLevelName = GetLockLevelName(lockLevel);

		ExtraInfoEntry* lockLevelEntry;
		CreateExtraInfoEntry(lockLevelEntry, "Lock level", lockLevelName, priority_ExtraLock_LockLevel);
		lockEntry->PushBack(lockLevelEntry);

		RE::TESForm* key = lock->key;
		ExtraInfoEntry* keyEntry;

		if (key)
		{
			std::string keyName = GetName(key);

			CreateExtraInfoEntry(keyEntry, "Key", keyName, priority_ExtraLock_Key);

			GetFormData(keyEntry, key, nullptr);

			lockEntry->PushBack(keyEntry);
		}

		else
		{
			CreateExtraInfoEntry(keyEntry, "Key", "None", priority_ExtraLock_Key);
			lockEntry->PushBack(keyEntry);
		}

		resultArray->PushBack(lockEntry);
	}

	logger::debug("Ending GetLockData");
}