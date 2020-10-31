#include "TESObjectCell.h"
#include "Util/NameUtil.h"
#include "TESForm.h"

void GetCellEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("Starting GetCellEntry");

	RE::TESObjectCELL* cell = static_cast<RE::TESObjectCELL*>(baseForm);
	if (cell)
	{
		bool isExterior = cell->IsExteriorCell();

		if (isExterior)
		{
			//Get the coordinates of the cell
			std::string coordinateString = GetCoordinateString(cell);
			ExtraInfoEntry* coordinateEntry;
			CreateExtraInfoEntry(coordinateEntry, "Coordinates", coordinateString, priority_Cell_Coordinate );
			resultArray->PushBack(coordinateEntry);

		}

		/*
		BSExtraData* extraData = pCell->unk048.extraData;

		while (extraData)
		{
			int extraDataType = extraData->GetType();
			std::string extraDataString = GetExtraDataTypeName(extraDataType);

			ExtraInfoEntry* extraDataTypeEntry;

			CreateExtraInfoEntry(extraDataTypeEntry, "Extra Data", extraDataString);
			resultArray->PushBack(extraDataTypeEntry);

			extraData = extraData->next;
		}*/

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

	if (currentCell)
	{
		std::string cellName = "";

		//For interior cells ge the name of the cell
		if (currentCell->IsInteriorCell())
		{
			cellName = GetName(currentCell);
		}

		//For exterior cells get the corrdinates instead
		else
		{
			cellName = GetCoordinateString(currentCell);
		}
		ExtraInfoEntry* cellEntry;
		CreateExtraInfoEntry(cellEntry, "Cell", cellName, priority_WorldData_Cell);

		GetFormData(cellEntry, currentCell, nullptr);

		//BSExtraData * extraData = currentCell->unk048.extraData;

		resultArray->PushBack(cellEntry);
	}
}