#include "BSExtraData.h"
#include "ExtraOwnership.h"
#include "ExtraEnableStateParent.h"
#include "ExtraContainerChanges.h"
#include "ExtraLock.h"
#include "MoreInformativeConsole/globals.h"
#include "MoreInformativeConsole/Util/NameUtil.h"

const int numberOfExtraDataTypes = 0xBF + 1; //The plus 1 is because there are extra data types from 0 up to and including BF

//get data stored in the BSExtraData format
void GetBSExtraData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("Starting GetBSExtraData");

	RE::ExtraDataList* extraList = &refForm->extraList;

	ProcessExtraDataList(resultArray, extraList, refForm);

	_DMESSAGE("Ending GetBSExtraData");
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

		/*
		//Check if the selected reference has a lock
		if (extraList->HasType(kExtraData_Lock))
		{
			BSExtraData* data = extraList->GetByType(kExtraData_Lock);
			lockData = DYNAMIC_CAST(data, BSExtraData, ExtraLock);
		}

		//if the reference doesn't have a lock check if there is a linked reference with a lock
		if (!lockData
			&& extraList->HasType(kExtraData_Teleport))
		{
			DebugMessage("Starting checking linked reference");
			BSExtraData* data = extraList->GetByType(kExtraData_Teleport);
			ExtraTeleport* teleportData = DYNAMIC_CAST(data, BSExtraData, ExtraTeleport);

			UInt32 linkedReferenceHandle = teleportData->data->dest;



#if SKSE_VERSION_INTEGER_BETA <= 12
			TESObjectREFR* linkedRef = nullptr;
			LookupREFRByHandle(linkedReferenceHandle, &linkedRef);
#else
			NiPointer<TESObjectREFR> linkedRef;
			LookupREFRByHandle(linkedReferenceHandle, linkedRef);
#endif

			if (linkedRef)
			{
				BaseExtraList* linkedRefExtraList = &linkedRef->extraData;

				if (linkedRefExtraList->HasType(kExtraData_Lock))
				{
					BSExtraData* data = linkedRefExtraList->GetByType(kExtraData_Lock);
					lockData = DYNAMIC_CAST(data, BSExtraData, ExtraLock);
				}
			}
			DebugMessage("Ending checking linked reference");
		}

		if (lockData)
		{
			GetLockData(resultArray, lockData);
		}

		DebugMessage("Ending lock checks");
		*/
		//Extra Data Test

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