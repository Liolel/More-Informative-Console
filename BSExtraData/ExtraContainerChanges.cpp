#include "ExtraContainerChanges.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"
#include "MoreInformativeConsole/TESForms/TESForm.h"

void ProcessContainerChanges(ExtraInfoEntry* resultArray, RE::BSExtraData* data, RE::TESObjectREFR* refForm)
{
	//get inventory
	RE::ExtraContainerChanges* containerChanges = static_cast<RE::ExtraContainerChanges*>(data);

	//get container object from the base form if a reference was passed of the right type
	RE::TESContainer* container = nullptr;

	if (refForm)
	{
		RE::TESForm* baseForm = refForm->data.objectReference;

		if (baseForm->GetFormType() == RE::FormType::NPC)
		{
			RE::Actor* actor = static_cast<RE::Actor*>(refForm);
			RE::TESActorBase* actorBase = static_cast<RE::TESActorBase*>(baseForm);

			//Put code to get equipment here
			GetEquipment(resultArray, containerChanges, actor);

			container = static_cast<RE::TESContainer*>(actorBase);
		}

		else if( baseForm->GetFormType() == RE::FormType::Container)
		{
			RE::TESObjectCONT* objectCont = static_cast<RE::TESObjectCONT*>(baseForm);
			container = static_cast<RE::TESContainer*>(objectCont);
		}
	}

	//Get inventory here
}


void GetEquipment(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::Actor* actor)
{
	_DMESSAGE("GetEquipment Start");

	ExtraInfoEntry* equipmentEntry;

	CreateExtraInfoEntry(equipmentEntry, "Equipment", "", priority_ExtraContainerChanges_Equipment);

	//weapons and shouts

	//left hand
	RE::TESForm* leftHand = actor->GetEquippedObject(true);

	if (leftHand != nullptr)
	{
		ExtraInfoEntry* leftHandEntry;
		std::string name = GetName(leftHand);

		CreateExtraInfoEntry(leftHandEntry, "Left Hand:", name, priority_ExtraContainerChanges_Item);

		GetFormData(leftHandEntry, leftHand, nullptr);
		equipmentEntry->PushBack(leftHandEntry);
	}

	//right hand
	RE::TESForm* rightHand = actor->GetEquippedObject(false);

	if (rightHand != nullptr)
	{
		ExtraInfoEntry* rightHandEntry;
		std::string name = GetName(rightHand);

		CreateExtraInfoEntry(rightHandEntry, "Right Hand:", name, priority_ExtraContainerChanges_Item);

		GetFormData(rightHandEntry, rightHand, nullptr);
		equipmentEntry->PushBack(rightHandEntry);
	}

	//shout
	RE::TESForm* shout = actor->selectedPower;

	if (shout != nullptr)
	{
		ExtraInfoEntry* ShoutEntry;
		std::string name = GetName(shout);

		CreateExtraInfoEntry(ShoutEntry, "Power/Shout:", name, priority_ExtraContainerChanges_Item);

		GetFormData(ShoutEntry, shout, nullptr);
		equipmentEntry->PushBack(ShoutEntry);
	}

	//check each equip slot
	for (int i = 0; i < 32; i++)
	{
		_DMESSAGE("GetEquipment: Starting EquipSlot item");

		int mask = 1 << i;

		RE::TESForm* equipedItem = FindEquipedItemInSlot(mask, containerChanges);

		if ( equipedItem )
		{
			_DMESSAGE("GetEquipment: EquipSlot item Found");

			ExtraInfoEntry* equipedItemEntry;
			std::string name = GetName(equipedItem);
			std::string slotName = GetEquipSlotName(i);

			CreateExtraInfoEntry(equipedItemEntry, slotName, name, priority_ExtraContainerChanges_Item);

			GetFormData(equipedItemEntry, equipedItem, nullptr);
			equipmentEntry->PushBack(equipedItemEntry);

		}

		_DMESSAGE("GetEquipment: Ending EquipSlot item");
	}

	resultArray->PushBack(equipmentEntry);

	_DMESSAGE("GetEquipment: GetEquipment End");
}

RE::TESForm* FindEquipedItemInSlot( int slotMask, RE::ExtraContainerChanges* containerChanges )
{
	_DMESSAGE("FindEquipedItemInSlot Start");
	RE::TESForm* equipedSlot = nullptr;
	RE::InventoryChanges* inventoryChanges = containerChanges->changes;

	for (RE::BSSimpleList<RE::InventoryEntryData*>::iterator itr = inventoryChanges->entryList->begin(); itr != inventoryChanges->entryList->end(); itr++)
	{
		//_DMESSAGE("FindEquipedItemInSlot Item found");

		RE::InventoryEntryData* inventoryEntryData = *itr;
		RE::TESForm* item = inventoryEntryData->object;

		if (item->GetFormType() == RE::FormType::Armor)
		{
			//_DMESSAGE("FindEquipedItemInSlot Armor Found");
			RE::TESObjectARMO* armor = static_cast<RE::TESObjectARMO*>(item);

			//Check if the armor is for the equip slot we're checking for
			if (HasFlag((UInt32)armor->bipedModelData.bipedObjectSlots, slotMask))
			{
				//_DMESSAGE("FindEquipedItemInSlot Matching Slot Found");
				
				//If there is extra data associated with the item found
				if (inventoryEntryData->extraLists)
				{
					for (RE::BSSimpleList<RE::ExtraDataList*>::iterator itrExtraList = inventoryEntryData->extraLists->begin(); itrExtraList != inventoryEntryData->extraLists->end(); itrExtraList++)
					{
						RE::ExtraDataList* extraDataList = *itrExtraList;

						if (extraDataList->HasType(RE::ExtraDataType::kWorn) || extraDataList->HasType(RE::ExtraDataType::kWornLeft))
						{
							equipedSlot = item;
						}
					}
				}
			}
		}
	}

	_DMESSAGE("FindEquipedItemInSlot End");
	
	return equipedSlot;
}

/*
void GetInventory(ExtraInfoEntry* resultArray, EntryDataList* inventory, TESContainer* baseContainer)
{
	DebugMessage("GetInventory: GetInventory Start");

	ExtraInfoEntry* inventoryEntry;

	CreateExtraInfoEntry(inventoryEntry, "Inventory", "");

	/*if (inventory != nullptr)
	{
		MICGlobals::reducedMode = true;
	}*/

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
}*/