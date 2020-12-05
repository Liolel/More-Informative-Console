#include "BSExtraData.h"
#include "ExtraOwnership.h"
#include "ExtraEnableStateParent.h"
#include "ExtraContainerChanges.h"
#include "ExtraLock.h"
#include "ExtraTeleport.h"
#include "globals.h"
#include "Util/NameUtil.h"

const int numberOfExtraDataTypes = 0xBF + 1; //The plus 1 is because there are extra data types from 0 up to and including BF

//get data stored in the BSExtraData format
void GetBSExtraData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	logger::debug("Starting GetBSExtraData");

	RE::ExtraDataList* extraList = &refForm->extraList;

	ProcessExtraDataList(resultArray, extraList, refForm);

	logger::debug("Ending GetBSExtraData");
}

void ProcessExtraDataList(ExtraInfoEntry* resultArray, RE::ExtraDataList* extraList, RE::TESObjectREFR* refForm)
{
	if (extraList)
	{
		if (extraList->HasType(RE::ExtraDataType::kEnableStateParent) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kEnableStateParent);
			ProcessEnableParentInformation(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kOwnership) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kOwnership);
			ProcessOwnership(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kContainerChanges))
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kContainerChanges);
			ProcessContainerChanges(resultArray, data, refForm);
		}
		
		if (extraList->HasType(RE::ExtraDataType::kLock))
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kLock);
			ProcessLockData(resultArray, data, refForm);
		}

		if (extraList->HasType(RE::ExtraDataType::kTeleport))
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kTeleport);
			ProcessTeleportData(resultArray, data);
		}

		if (MICOptions::MICDebugMode)
		{
				
			ExtraInfoEntry* extraDataTypes;
			CreateExtraInfoEntry(extraDataTypes, "Extra Data", "", priority_ExtraData);

			for (int i = 1; i <= numberOfExtraDataTypes; i++)
			{

				//Placeholder for seeing what has editor IDs
				if (extraList->HasType( (RE::ExtraDataType)i ) )
				{
					ExtraInfoEntry* extraDataEntry;
					CreateExtraInfoEntry(extraDataEntry, GetExtraDataTypeName(i), "", priority_ExtraData);
					extraDataTypes->PushBack(extraDataEntry);
				}

			}

			resultArray->PushBack(extraDataTypes);
		}		
	}
}