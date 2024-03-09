#include "TESObjectCell.h"
#include "TESForm.h"
#include "BSExtraData/BSExtraData.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

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
			CreateExtraInfoEntry(coordinateEntry, GetTranslation("$Coordinates"), coordinateString, priority_Cell_Coordinate);
			resultArray->PushBack(coordinateEntry);

			auto landscape = cell->GetRuntimeData().cellLand;

			//get landscape textures
			if (landscape != nullptr
				and landscape->loadedData != nullptr )
			{
				std::set<RE::TESLandTexture*> landscapeTextures;

				auto loadedData = landscape->loadedData;

				for (int i = 0; i < 4; i++)
				{
					if ( loadedData->defQuadTextures[i] != nullptr
						 && loadedData->defQuadTextures[i]->formID != 0 ) //0 seems to be used to represent entires with no texture assigned						
					{
						landscapeTextures.insert(loadedData->defQuadTextures[i]);
					}

					for (int j = 0; j < 6; j++)
					{
						if (loadedData->quadTextures[i][j] != nullptr 
							&& loadedData->quadTextures[i][j]->formID != 0 )
						{
							landscapeTextures.insert(loadedData->quadTextures[i][j]);
						}
					}
				}

				//if at least one landscape texture was found
				if (!landscapeTextures.empty())
				{
					ExtraInfoEntry* landscapeTexturesEntry;

					CreateExtraInfoEntry(landscapeTexturesEntry, GetTranslation("$LandscapeTextures"), "", priority_Cell_LandscapeTextures);

					for (auto landscapeTexture : landscapeTextures )
					{
						ExtraInfoEntry* landscapeTextureEntry;	
						std::string landscapeTextureName = GetName(landscapeTexture, nullptr);

						CreateExtraInfoEntry(landscapeTextureEntry, landscapeTextureName, "", priority_Cell_LandscapeTextures);
						GetFormData(landscapeTextureEntry, landscapeTexture, nullptr);
						landscapeTexturesEntry->PushBack(landscapeTextureEntry);
					}    

					resultArray->PushBack(landscapeTexturesEntry);
				}

			}
		}

		RE::BGSLightingTemplate* lightingTemplate = cell->GetRuntimeData().lightingTemplate;

		if (lightingTemplate != nullptr)
		{
			std::string templateName = GetName(lightingTemplate);
			ExtraInfoEntry* lightingTemplateEntry;

			CreateExtraInfoEntry(lightingTemplateEntry, GetTranslation("$LightingTemplate"), templateName, priority_Cell_LightningTemplate);

			GetFormData(lightingTemplateEntry, lightingTemplate, nullptr);

			resultArray->PushBack(lightingTemplateEntry);
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
				CreateExtraInfoEntry(factionOwnerEntry, GetTranslation("$Owner"), factionName, priority_Cell_Owner);
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
					CreateExtraInfoEntry(ownerActorEntry, GetTranslation("$Owner"), actorName, priority_Cell_Owner);
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
		CreateExtraInfoEntry(cellEntry, GetTranslation("$Cell"), cellName, priority_WorldData_Cell);

		GetFormData(cellEntry, currentCell, nullptr);

		resultArray->PushBack(cellEntry);
	}
}

void GetLandscapeTextureAtReference(ExtraInfoEntry* resultArray, RE::PlayerCharacter* pc)
{
	logger::debug("Starting GetCellEntry");

	RE::TESObjectCELL* cell = pc->parentCell;
	RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
	if (cell && ref)
	{
		bool isExterior = cell->IsExteriorCell();

		if (isExterior)
		{
			auto tes = RE::TES::GetSingleton();

			if (tes)
			{
				auto landscapeTexture = tes->GetLandTexture(ref->GetPosition() );

				if (landscapeTexture)
				{
					ExtraInfoEntry* landscapeTextureEntry;
					std::string landscapeTextureName = GetName(landscapeTexture, nullptr);

					CreateExtraInfoEntry(landscapeTextureEntry, GetTranslation("$LandscapeTextureAtReference"), landscapeTextureName, priority_WorldData_LandscapeTextureAtReference);
					GetFormData(landscapeTextureEntry, landscapeTexture, nullptr);
					resultArray->PushBack(landscapeTextureEntry);
				}
			}
		}
	}
}
