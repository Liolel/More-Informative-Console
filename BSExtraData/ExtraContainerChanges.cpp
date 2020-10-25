#include "ExtraContainerChanges.h"
#include "Util/NameUtil.h"
#include "Util/GeneralUtil.h"
#include "TESForms/TESForm.h"

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
	GetInventory(resultArray, containerChanges, container);
}


void GetEquipment(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::Actor* actor)
{
	logger::debug("GetEquipment Start");

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
		logger::debug("GetEquipment: Starting EquipSlot item");

		int mask = 1 << i;

		RE::TESForm* equipedItem = FindEquipedItemInSlot(mask, containerChanges);

		if ( equipedItem )
		{
			logger::debug("GetEquipment: EquipSlot item Found");

			ExtraInfoEntry* equipedItemEntry;
			std::string name = GetName(equipedItem);
			std::string slotName = GetEquipSlotName(i);

			CreateExtraInfoEntry(equipedItemEntry, slotName, name, priority_ExtraContainerChanges_Item);

			GetFormData(equipedItemEntry, equipedItem, nullptr);
			equipmentEntry->PushBack(equipedItemEntry);

		}

		logger::debug("GetEquipment: Ending EquipSlot item");
	}

	resultArray->PushBack(equipmentEntry);

	logger::debug("GetEquipment: GetEquipment End");
}

RE::TESForm* FindEquipedItemInSlot( int slotMask, RE::ExtraContainerChanges* containerChanges )
{
	logger::debug("FindEquipedItemInSlot Start");
	RE::TESForm* equipedSlot = nullptr;
	RE::InventoryChanges* inventoryChanges = containerChanges->changes;
	RE::BSSimpleList<RE::InventoryEntryData*> ::iterator itr, itrEnd;
	itrEnd = inventoryChanges->entryList->end();

	for ( itr = inventoryChanges->entryList->begin(); itr != itrEnd; itr++)
	{
		//logger::debug("FindEquipedItemInSlot Item found");

		RE::InventoryEntryData* inventoryEntryData = *itr;
		RE::TESForm* item = inventoryEntryData->object;

		if (item->GetFormType() == RE::FormType::Armor)
		{
			//logger::debug("FindEquipedItemInSlot Armor Found");
			RE::TESObjectARMO* armor = static_cast<RE::TESObjectARMO*>(item);

			//Check if the armor is for the equip slot we're checking for
			if (HasFlag(armor->bipedModelData.bipedObjectSlots.underlying(), slotMask) )
			{
				//logger::debug("FindEquipedItemInSlot Matching Slot Found");
				
				//If there is extra data associated with the item found
				if (inventoryEntryData->extraLists)
				{
					RE::BSSimpleList<RE::ExtraDataList*> ::iterator itrExtraList, itrExtraListEnd;
					itrExtraListEnd  = inventoryEntryData->extraLists->end();
					for ( itrExtraList = inventoryEntryData->extraLists->begin(); itrExtraList != itrExtraListEnd; itrExtraList++)
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

	logger::debug("FindEquipedItemInSlot End");
	
	return equipedSlot;
}


void GetInventory(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::TESContainer* baseContainer)
{
	logger::debug("GetInventory: Start");

	ExtraInfoEntry* inventoryEntry;
	CreateExtraInfoEntry(inventoryEntry, "Inventory", "", priority_ExtraContainerChanges_Inventory);

	RE::InventoryChanges* inventoryChanges = containerChanges->changes;

	//Get items in containerChanges extra data
	RE::BSSimpleList<RE::InventoryEntryData*>::iterator itr, itrEnd;
	itrEnd = inventoryChanges->entryList->end();

	for ( itr = inventoryChanges->entryList->begin(); itr != itrEnd; itr++)
	{
		//logger::debug("FindEquipedItemInSlot Item found");

		RE::InventoryEntryData* inventoryEntryData = *itr;
		RE::TESForm* item = inventoryEntryData->object;
		int itemCount = inventoryEntryData->countDelta;

		if (baseContainer)
		{
			itemCount = itemCount + GetNumberOfItemInContainer(baseContainer, item);
		}

		if (itemCount > 0)
		{
			std::string name = GetName(item);

			ExtraInfoEntry * inventoryItemEntry;

			CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount), priority_ExtraContainerChanges_Item);

			GetFormData(inventoryItemEntry, item, nullptr);

			inventoryEntry->PushBack(inventoryItemEntry);
		}

			//logger::debug("GetInventory: Ending inventory item");
	}
	
	//go through the items in the base form
	if (baseContainer != nullptr)
	{
		for (uint32_t i = 0; i < baseContainer->numContainerObjects; i++)
		{
			//DebugMessage("GetInventory: Starting container item");

			RE::ContainerObject* containerObject = baseContainer->containerObjects[i];
			RE::TESForm* item = containerObject->obj;

			//only display items which have not changed from the base form
			bool displayItem = !InventoryChangesContainsItem(inventoryChanges, item);

			if (displayItem
				&& item->GetFormType() != RE::FormType::LeveledItem ) //Don't display leved items in the inventory for the moment
			{
				//DebugMessage("GetInventory: Displaying container item");
				int itemCount = containerObject->count;

				std::string name = GetName(item);

				ExtraInfoEntry * inventoryItemEntry;

				CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount), priority_ExtraContainerChanges_Item);

				GetFormData(inventoryItemEntry, item, nullptr);

				inventoryEntry->PushBack(inventoryItemEntry);
			}

			//DebugMessage("GetInventory: Ending container item");
		}
	}

	resultArray->PushBack(inventoryEntry);

	logger::debug("GetInventory: End");
}

int GetNumberOfItemInContainer(RE::TESContainer* container, RE::TESForm* item)
{
	logger::debug("GetNumberOfItemInContainer: Start");

	int count = 0;

	for (uint32_t i = 0; i < container->numContainerObjects; i++)
	{
		RE::ContainerObject* containerObject = container->containerObjects[i];

		if (containerObject->obj == item)
		{
			count += containerObject->count;
		}
	}

	logger::debug("GetNumberOfItemInContainer: End");

	return count;
}


bool InventoryChangesContainsItem(RE::InventoryChanges* inventoryChanges, RE::TESForm* item)
{
	logger::debug("InventoryChangesContainsItem Start");

	bool containsItem = false;
	RE::BSSimpleList<RE::InventoryEntryData*>::iterator itr, itrEnd;
	itr = inventoryChanges->entryList->begin();
	itrEnd = inventoryChanges->entryList->end();

	while( itr != itrEnd
		   && !containsItem )
	{
		//logger::debug("FindEquipedItemInSlot Item found");

		RE::InventoryEntryData* inventoryEntryData = *itr;
		RE::TESForm* itemInInventory = inventoryEntryData->object;

		if (itemInInventory == item)
		{
			containsItem = true;
		}

		itr++;
	}

	logger::debug("InventoryChangesContainsItem End");

	return containsItem;
}