#include "ExtraRegionList.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"

void ProcessRegionList(ExtraInfoEntry* resultArray, RE::BSExtraData* data)
{
	logger::debug("ProcessRegionList Start");
	RE::ExtraRegionList* extraRegionList = static_cast<RE::ExtraRegionList*>(data);

	if (extraRegionList)
	{
		ExtraInfoEntry* regionsEntry;

		CreateExtraInfoEntry(regionsEntry, "Regions", "", priority_ExtraData_RegionList);

		auto itrEnd = extraRegionList->list->end();

		for( auto itr = extraRegionList->list->begin(); itr != itrEnd; ++itr )
		{
			RE::TESRegion *region = *(itr);
			std::string regionName = GetName(region);

			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry* regionEntry;

			CreateExtraInfoEntry(regionEntry, regionName, "", priority_ExtraData_Region);

			GetFormData(regionEntry, region, nullptr);

			regionsEntry->PushBack(regionEntry);
		}

		resultArray->PushBack(regionsEntry);
	}

	logger::debug("ProcessRegionList End");
}

