#include "TESObjectCell.h"
#include "TESForm.h"
#include "BSExtraData/BSExtraData.h"
#include "Util/NameUtil.h"

void GetCellEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("Starting GetCellEntry");

	RE::TESObjectCELL* cell = static_cast<RE::TESObjectCELL*>(baseForm);
	if (cell) {
		bool isExterior = cell->IsExteriorCell();

		if (isExterior) {
			//Get the coordinates of the cell
			std::string coordinateString = GetCoordinateString(cell);
			ExtraInfoEntry* coordinateEntry;
			CreateExtraInfoEntry(coordinateEntry, "Coordinates", coordinateString, priority_Cell_Coordinate);
			resultArray->PushBack(coordinateEntry);
		}

		//it seems that ownership is both attached to the cell and stored in an extraOwnership object. The code for parsing extraOwnership is used in multiple locations so I can't really disable that, so only check
		//the ownership attached to the cell if it is somehow missing from the extra data
		if (!cell->extraList.HasType(RE::ExtraDataType::kOwnership))
		{

			auto ownerFaction = cell->GetFactionOwner();

			if (ownerFaction != nullptr)
			{
				std::string factionName = GetName(ownerFaction);
				ExtraInfoEntry* factionOwnerEntry;
				CreateExtraInfoEntry(factionOwnerEntry, "Owner", factionName, priority_Cell_Owner);
				GetFormData(factionOwnerEntry, ownerFaction, nullptr);

				resultArray->PushBack(factionOwnerEntry);
			}
			else
			{
				auto ownerActor = cell->GetActorOwner();

				if (ownerActor != nullptr)
				{
					std::string actorName = GetName(ownerActor);
					ExtraInfoEntry* ownerActorEntry;
					CreateExtraInfoEntry(ownerActorEntry, "Owner", actorName, priority_Cell_Owner);
					GetFormData(ownerActorEntry, ownerActor, nullptr);
					resultArray->PushBack(ownerActorEntry);
				}
			}
		}

		
		auto extraData = &(cell->extraList);
		ProcessExtraDataList(resultArray, extraData, nullptr);
	}
}

std::string GetCoordinateString(RE::TESObjectCELL* cell)
{
	std::string coordinates = "";

	RE::EXTERIOR_DATA* exteriorData = cell->GetCoordinates();

	if (exteriorData)
	{
		coordinates = IntToString(exteriorData->cellX) + "," + IntToString(exteriorData->cellY);
	}

	return coordinates;
}

void GetCurrentCellForWorldData(ExtraInfoEntry* resultArray, RE::PlayerCharacter* pc)
{
	logger::debug("Starting GetCurrentCellForWorldData");

	RE::TESObjectCELL* currentCell = pc->parentCell;

	if (currentCell) {
		std::string cellName = "";

		//For interior cells ge the name of the cell
		if (currentCell->IsInteriorCell()) {
			cellName = GetName(currentCell);
		}

		//For exterior cells get the corrdinates instead
		else {
			cellName = GetCoordinateString(currentCell);
		}
		ExtraInfoEntry* cellEntry;
		CreateExtraInfoEntry(cellEntry, "Cell", cellName, priority_WorldData_Cell);

		GetFormData(cellEntry, currentCell, nullptr);

		resultArray->PushBack(cellEntry);
	}
}
