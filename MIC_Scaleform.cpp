#include "MIC_Scaleform.h"
#include "Scaleform/MICScaleform_GetReferenceInfo.h"
#include "Scaleform/MICScaleform_GetIniOptions.h"
#include "Scaleform/MICScaleform_RetrieveExtraData.h"
#include "Scaleform/MICScaleform_GetExtraData.h"
#include "Scaleform/MICScaleform_Log.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include <Windows.h>

MICScaleform_GetReferenceInfo* getReferenceInfo = nullptr;
MICScaleform_GetIniOptions* getIniOptions = nullptr;
MICScaleform_RetrieveExtraData* retrieveExtraData = nullptr;
MICScaleform_GetExtraData* getExtraData = nullptr;
MICScaleform_Log* MICScaleformlog = nullptr;

/*

	void GetInventory(ExtraInfoEntry * resultArray,  EntryDataList * inventory, TESContainer * baseContainer)
	{
		DebugMessage("GetInventory: GetInventory Start");

		ExtraInfoEntry * inventoryEntry;

		CreateExtraInfoEntry(inventoryEntry, "Inventory", "");

		//go through the inventory (these are anything changed from the base form)
		/*
		if (inventory != nullptr
			&& inventory->Count() > 0)
		{
			for (EntryDataList::Iterator i = inventory->Begin(); !i.End(); ++i)
			{
				DebugMessage("GetInventory: Starting inventory item");

				InventoryEntryData * item = i.Get();


				int itemCount = item->countDelta;
				TESForm *itemForm = item->type;

				if (baseContainer != nullptr)
				{
					itemCount = itemCount + NumberOfItemInContainer(itemForm, baseContainer);
				}

				if (itemCount > 0)
				{
					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));

					GetFormData(inventoryItemEntry, itemForm, nullptr);

					inventoryEntry->PushBack(inventoryItemEntry);
				}

				DebugMessage("GetInventory: Ending inventory item");

			}
		}

		//go through the items in the base form
		if (baseContainer != nullptr)
		{
			for (int i = 0; i < baseContainer->numEntries; i++)
			{

				DebugMessage("GetInventory: Starting container item");

				RE::TESForm *itemForm = baseContainer->entries[i]->form;

				//only display items which have not changed from the base form
				bool displayItem = true;

				if (inventory != nullptr)
				{
					displayItem = !HasItem(inventory, itemForm);
				}

				if (displayItem
					&& itemForm->GetFormType() != kFormType_LeveledItem)
				{

					DebugMessage("GetInventory: Displaying container item");
					int itemCount = baseContainer->entries[i]->count;

					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					if (name != "")
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));
					}

					else
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, FormIDToString(itemForm->formID));
					}

					GetFormData(inventoryItemEntry, itemForm, nullptr);

					inventoryEntry->PushBack(inventoryItemEntry);
				}


				DebugMessage("GetInventory: Ending container item");
			}
		}

		resultArray->PushBack(inventoryEntry);

		//MICGlobals::reducedMode = false;

		DebugMessage("GetInventory: GetInventory End");
	}

	void GetWeaponData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectWEAP * pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			//animation type
			int animationType = pWeapon->type();

			ExtraInfoEntry * antimationTypeEntry;

			CreateExtraInfoEntry(antimationTypeEntry, "Animation type", GetWeaponAnimationTypeName(animationType));
			resultArray->PushBack(antimationTypeEntry);

			//damage
			int damage = pWeapon->damage.GetAttackDamage();

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", IntToString(damage));
			resultArray->PushBack(damageEntry);

			//speed
			float speed = pWeapon->speed();

			ExtraInfoEntry * speedEntry;

			CreateExtraInfoEntry(speedEntry, "Speed", FloatToString(speed));
			resultArray->PushBack(speedEntry);

			//reach
			float reach = pWeapon->reach();

			ExtraInfoEntry * reachEntry;

			CreateExtraInfoEntry(reachEntry, "Reach", FloatToString(reach));
			resultArray->PushBack(reachEntry);

			//stagger
			float stagger = pWeapon->stagger();

			ExtraInfoEntry * staggerEntry;

			CreateExtraInfoEntry(staggerEntry, "Stagger", FloatToString(stagger));
			resultArray->PushBack(staggerEntry);


			//crit damage
			int critDamage = pWeapon->critDamage();

			ExtraInfoEntry * critDamageEntry;

			CreateExtraInfoEntry(critDamageEntry, "Crit damage", IntToString(critDamage));
			resultArray->PushBack(critDamageEntry);

			/*RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
			RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange()); Data for npc AI*/
			/*
			BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
			if (equipSlot)
			{
				std::string equipSlotName = GetEquipTypeName(equipSlot->formID);

				ExtraInfoEntry * equipSlotEntry;

				CreateExtraInfoEntry(equipSlotEntry, "Equip slot", equipSlotName);
				resultArray->PushBack(equipSlotEntry);
			}

			//value
			int value = pWeapon->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

			//weight
			int weight = pWeapon->weight.weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", IntToString(weight));
			resultArray->PushBack(weightEntry);

		}
	}

	void GetAmmoData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESAmmo * pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
		if (pAmmo)
		{
			//damage
			float damage = pAmmo->settings.damage;

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", FloatToString(damage));
			resultArray->PushBack(damageEntry);


			//value
			int value = pAmmo->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

		}
	}

	void GetContainerData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectCONT * pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			int respawnsFlag = 0x02;

			ExtraInfoEntry * safeContainerEntry;

			if ((pContainer->unkB9 & respawnsFlag) == respawnsFlag)
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "No");
			}

			else
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "Yes");
			}

			resultArray->PushBack(safeContainerEntry);
		}
	}


	void GetLockData(ExtraInfoEntry* resultArray, ExtraLock* lockData)
	{		
		if (lockData->state)
		{
			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry* lockEntry;

			CreateExtraInfoEntry(lockEntry, "Lock", "");

			bool isLocked = (lockData->state->flags & LockState::Flag::kLocked) == LockState::Flag::kLocked;
			
			ExtraInfoEntry* isLockedEntry;
			CreateExtraInfoEntry(isLockedEntry, "Is locked", BooleanToYesNoString(isLocked));
			lockEntry->PushBack(isLockedEntry);

			int lockLevel = lockData->state->lockLevel;
			std::string lockLevelName = GetLockLevel(lockLevel);

			ExtraInfoEntry * lockLevelEntry;
			CreateExtraInfoEntry(lockLevelEntry, "Lock level", lockLevelName);
			lockEntry->PushBack(lockLevelEntry);

			TESForm* key = lockData->state->key;
			ExtraInfoEntry* keyEntry;

			if (key)
			{
				std::string keyName = GetName(key);

				CreateExtraInfoEntry(keyEntry, "Key", keyName);

				GetFormData(keyEntry, key, nullptr);

				lockEntry->PushBack(keyEntry);
			}

			else
			{
				CreateExtraInfoEntry(keyEntry, "Key", "None");
				lockEntry->PushBack(keyEntry);
			}

			/*
			int unknown1 = lockData->state->unk01;
			ExtraInfoEntry* unk01;
			CreateExtraInfoEntry(unk01, "unk01", IntToString( unknown1) );
			lockEntry->PushBack(unk01);

			int unknown2 = lockData->state->unk02;
			ExtraInfoEntry* unk02;
			CreateExtraInfoEntry(unk02, "unk02", IntToString(unknown2));
			lockEntry->PushBack(unk02);

			int unknown4 = lockData->state->unk04;
			ExtraInfoEntry* unk04;
			CreateExtraInfoEntry(unk04, "unk04", IntToString(unknown4));
			lockEntry->PushBack(unk04);

			int unknown11 = lockData->state->unk11;
			ExtraInfoEntry* unk11;
			CreateExtraInfoEntry(unk11, "unk11", IntToString(unknown11));
			lockEntry->PushBack(unk11);

			int unknown12 = lockData->state->unk12;
			ExtraInfoEntry* unk12;
			CreateExtraInfoEntry(unk12, "unk12", IntToString(unknown12));
			lockEntry->PushBack(unk12);

			int unknown14 = lockData->state->unk14;
			ExtraInfoEntry* unk14;
			CreateExtraInfoEntry(unk14, "unk14", IntToString(unknown14));
			lockEntry->PushBack(unk14);

			int unknown18 = lockData->state->unk18;
			ExtraInfoEntry* unk18;
			CreateExtraInfoEntry(unk18, "unk18", IntToString(unknown18));
			lockEntry->PushBack(unk18);

			int unknown1C = lockData->state->unk1C;
			ExtraInfoEntry* unk1C;
			CreateExtraInfoEntry(unk1C, "unk1C", IntToString(unknown1C));
			lockEntry->PushBack(unk1C);

			int flags = (UInt8)lockData->state->flags;
			ExtraInfoEntry* flagsEntry;
			CreateExtraInfoEntry(flagsEntry, "Flags", IntToString(flags));
			lockEntry->PushBack(flagsEntry); */
			/*
			resultArray->PushBack(lockEntry);
		}

	}

	void GetWorldData(ExtraInfoEntry* resultArray)
	{
		PlayerCharacter* pPC = (*g_thePlayer);

		if (pPC)
		{
			DebugMessage("Starting Worldspace");

			TESWorldSpace* currentWorldSpace = pPC->currentWorldSpace;

			if (currentWorldSpace)
			{
				std::string worldSpaceName = GetName(currentWorldSpace);

				ExtraInfoEntry* worldSpaceEntry;
				CreateExtraInfoEntry(worldSpaceEntry, "World Space", worldSpaceName);

				GetFormData(worldSpaceEntry, currentWorldSpace, nullptr);
				resultArray->PushBack(worldSpaceEntry);
			}

			DebugMessage("Starting Cell");

			TESObjectCELL* currentCell = pPC->parentCell;

			if (currentCell )
			{
				std::string cellName = GetName(currentCell);

				ExtraInfoEntry* cellEntry;
				CreateExtraInfoEntry(cellEntry, "Cell", cellName);

				GetFormData(cellEntry, currentCell, nullptr);

				//BSExtraData * extraData = currentCell->unk048.extraData;

				resultArray->PushBack(cellEntry);
			}
		}

		//Find the current music track
		
		DataHandler* dataHandler = DataHandler::GetSingleton();

		if (dataHandler)
		{
			DebugMessage("Starting Music");
			tArray<BGSMusicTrackFormWrapper*> *	musicTrackArray = (tArray<BGSMusicTrackFormWrapper*>*)(&(dataHandler->arrMUST));

			int numberOfMusicTracks = musicTrackArray->count;

			for (int i = 0; i <= numberOfMusicTracks; i++)
			{
				BGSMusicTrackFormWrapper* wrapper;
				musicTrackArray->GetNthItem(i, wrapper);

				if (wrapper)
				{
					BSIMusicTrack * musicTrack = &(wrapper->track);

					MUSIC_STATUS musicTrackStatus = (MUSIC_STATUS)musicTrack->Unk_08();

					if( musicTrackStatus == MUSIC_STATUS::kPlaying)
					{
						std::string  formIDString = FormIDToString(wrapper->formID);
						ExtraInfoEntry* musicEntry;
						CreateExtraInfoEntry(musicEntry, "Music", formIDString);
						resultArray->PushBack(musicEntry);
					}
				}
			}
		}
	}

	/*
	void StandardItemData(ExtraInfoEntry * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
	{
	if (!pForm || !pFxVal || !pFxVal->IsObject())
	return;

	switch (pForm->GetFormType())
	{

	break;

	case kFormType_SoulGem:
	{
	TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
	if (soulGem)
	{
	RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
	RegisterNumber(pFxVal, "soulSize", pEntry ? CALL_MEMBER_FN(pEntry, GetSoulLevel)() : soulGem->soulSize);
	}
	}
	break;

	case kFormType_Potion:
	{
	AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
	if (pAlchemy)
	{
	RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
	}
	}
	break;

	case kFormType_Book:
	{
	TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
	if (pBook)
	{
	RegisterNumber(pFxVal, "flags", pBook->data.flags);
	RegisterNumber(pFxVal, "bookType", pBook->data.type);
	switch (pBook->data.GetSanitizedType())
	{
	case TESObjectBOOK::Data::kType_Skill:
	RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
	break;

	case TESObjectBOOK::Data::kType_Spell:
	{
	double formID = -1;

	if (pBook->data.teaches.spell)
	formID = pBook->data.teaches.spell->formID;

	RegisterNumber(pFxVal, "teachesSpell", formID);
	}
	break;
	}
	}
	}
	break;

	default:
	break;
	}
	}*/
	/*

	void GetCellEntry(ExtraInfoEntry* resultArray, TESForm* pBaseForm)
	{
		DebugMessage("Starting GetCellEntry");

		TESObjectCELL* pCell = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCELL);
		if (pCell)
		{

			LightingCoordinates * lightningCoordinates = (LightingCoordinates*)(&(pCell->unk060));
			
			if (lightningCoordinates->coordinates)
			{
				
				Coordinates* coordinates = lightningCoordinates->coordinates;

				std::string coordinateString = IntToString(coordinates->cellX) + "," + IntToString(coordinates->cellY);

				ExtraInfoEntry* coordinateEntry;

				CreateExtraInfoEntry(coordinateEntry, "Coordinates", coordinateString);

				resultArray->PushBack(coordinateEntry);
			}

			BSExtraData * extraData = pCell->unk048.extraData;

			while (extraData)
			{
				int extraDataType = extraData->GetType();
				std::string extraDataString = GetExtraDataTypeName(extraDataType);

				ExtraInfoEntry* extraDataTypeEntry;

				CreateExtraInfoEntry(extraDataTypeEntry, "Extra Data", extraDataString);
				resultArray->PushBack(extraDataTypeEntry);

				extraData = extraData->next;
			}
			
		}
	}
};*/

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks( RE::GFxMovieView* a_view, RE::GFxValue* a_root )
{
	_DMESSAGE("Install Hooks Called");

	if (getReferenceInfo == nullptr)
	{
		getReferenceInfo = new MICScaleform_GetReferenceInfo;
	}

	if (getIniOptions == nullptr)
	{
		getIniOptions = new MICScaleform_GetIniOptions;
	}

	if (retrieveExtraData == nullptr)
	{
		retrieveExtraData = new MICScaleform_RetrieveExtraData;
	}

	if (getExtraData == nullptr)
	{
		getExtraData = new MICScaleform_GetExtraData;
	}

	
	if (MICScaleformlog == nullptr)
	{
		MICScaleformlog = new MICScaleform_Log;
	}

	RE::GFxValue globals;

	_DMESSAGE( a_view->GetMovieDef()->GetFileURL() );

	bool result = a_view->GetVariable(&globals, "_global");
	if (result)
	{
		//Create Object to hold new functions
		RE::GFxValue MIC;
		a_view->CreateObject(&MIC);

		RE::GFxValue	fnValue;
		a_view->CreateFunction(&fnValue, getReferenceInfo);
		MIC.SetMember("MICScaleform_GetReferenceInfo", fnValue);
		
		RE::GFxValue	fnValue2;
		a_view->CreateFunction(&fnValue2, getIniOptions);
		MIC.SetMember("MICScaleform_GetIniOptions", fnValue2);

		RE::GFxValue	fnValue3;
		a_view->CreateFunction(&fnValue3, retrieveExtraData);
		MIC.SetMember("MICScaleform_RetrieveExtraData", fnValue3);

		RE::GFxValue	fnValue4;
		a_view->CreateFunction(&fnValue4, getExtraData);
		MIC.SetMember("MICScaleform_GetExtraData", fnValue4);
		
		RE::GFxValue	fnValue5;
		a_view->CreateFunction(&fnValue5, MICScaleformlog);
		MIC.SetMember("MICScaleform_Log", fnValue5);

		//Store object with functions
		globals.SetMember("MIC", MIC);
	}

	return true;
}