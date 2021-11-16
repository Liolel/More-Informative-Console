#include "ExtraTeleport.h"
#include "ExtraLock.h"

void ProcessTeleportData(ExtraInfoEntry* resultArray, RE::BSExtraData* data)
{
	logger::debug("ProcessTeleportData Start");
	RE::ExtraTeleport* extraTeleport = static_cast<RE::ExtraTeleport*>(data);

	if (extraTeleport)
	{
		RE::TESObjectREFR* destination = extraTeleport->teleportData->linkedDoor.get().get();
		
		if( destination )
		{
			

			//Check specifically for lock data. Don't put code getting the full form data without adding code to prevent finding the extra Teleport information of the destination and creating an infinite loop

			RE::ExtraDataList* extraList = &destination->extraList;

			if (extraList)
			{
				if (extraList->HasType(RE::ExtraDataType::kLock))
				{
					RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kLock);
					ProcessLockData(resultArray, data, destination);
				}
			}
		}
	}

	logger::debug("ProcessTeleportData End");
}
