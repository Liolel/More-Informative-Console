#include "BSExtraData.h"
#include "ExtraEnableStateParent.h"


//get data stored in the BSExtraData format
void GetBSExtraData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("Starting GetBSExtraData");

	RE::ExtraDataList* extraList = &refForm->extraList;

	ProcessExtraDataList(resultArray, extraList);

	_DMESSAGE("Ending GetBSExtraData");
}

void ProcessExtraDataList(ExtraInfoEntry* resultArray, RE::ExtraDataList* extraList)
{
	if (extraList)
	{
		if (extraList->HasType(RE::ExtraDataType::kEnableStateParent))
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kEnableStateParent);
			RE::ExtraEnableStateParent* enableParentInformation = static_cast<RE::ExtraEnableStateParent*>(data);

			ProcessEnableParentInformation(resultArray, enableParentInformation);
		}
		/*
		if (extraList->HasType(kExtraData_Ownership))
		{
			DebugMessage("Starting kExtraData_Ownership");

			BSExtraData* data = extraList->GetByType(kExtraData_Ownership);
			ExtraOwnership* ownershipData = DYNAMIC_CAST(data, BSExtraData, ExtraOwnership);

			if (ownershipData)
			{
				TESForm* owner = ownershipData->owner;
				std::string ownerName = GetName(owner);

				//Placeholder for seeing what has editor IDs
				ExtraInfoEntry* ownershipEntry;

				CreateExtraInfoEntry(ownershipEntry, "Owner", ownerName);


				DebugMessage("Before getting owner form data");

				GetFormData(ownershipEntry, owner, nullptr);

				resultArray->PushBack(ownershipEntry);

			}


			DebugMessage("Ending kExtraData_Ownership");
		}

		DebugMessage("Starting lock checks");
		//Handle locks
		ExtraLock* lockData = nullptr;

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

		//Extra Data Test
		ExtraInfoEntry* extraDataTypes;
		CreateExtraInfoEntry(extraDataTypes, "Extra Data", "");

		for (int i = 1; i <= 0xB3; i++)
		{

			//Placeholder for seeing what has editor IDs
			if (extraList->HasType(i))
			{
				ExtraInfoEntry* extraDataEntry;
				CreateExtraInfoEntry(extraDataEntry, GetExtraDataTypeName(i), "");
				extraDataTypes->PushBack(extraDataEntry);
			}

		}

		resultArray->PushBack(extraDataTypes);*/
		/**
		if (extraList->HasType(kExtraData_Package))
		{
		DebugMessage("Starting kExtraData_Package");

		BSExtraData * data = extraList->GetByType(kExtraData_Package);
		ExtraPackage * packageData = DYNAMIC_CAST(data, BSExtraData, ExtraPackage);

		if (packageData)
		{
		TESForm * currentPackage = packageData->currentPackage;
		std::string packageName = GetName(currentPackage);

		//Placeholder for seeing what has editor IDs
		ExtraInfoEntry * packageEntry;

		CreateExtraInfoEntry(packageEntry, "Current Package", packageName);


		DebugMessage("Before getting packag form data");

		ExtraInfoEntry * packageSubArray;
		movie->CreateArray(packageSubArray);
		GetFormData(packageSubArray, currentPackage, nullptr);

		packageEntry->PushBack(packageSubArray);
		resultArray->PushBack(packageEntry);

		}


		DebugMessage("Ending kExtraData_Package");
		}*/
		
	}
}