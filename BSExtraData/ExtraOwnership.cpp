#include "ExtraOwnership.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "MoreInformativeConsole/TESForms/TESForm.h"

void ProcessOwnership(ExtraInfoEntry* resultArray, RE::BSExtraData* data )
{
	_DMESSAGE("Starting ProcessOwnership");

	RE::ExtraOwnership* ownershipData = static_cast<RE::ExtraOwnership*>(data);

	if (ownershipData)
	{
		RE::TESForm* owner = ownershipData->owner;
		std::string ownerName = GetName(owner);

		//Placeholder for seeing what has editor IDs
		ExtraInfoEntry* ownershipEntry;

		CreateExtraInfoEntry(ownershipEntry, "Owner", ownerName, priority_ExtraData_Ownership);

		GetFormData(ownershipEntry, owner, nullptr);

		resultArray->PushBack(ownershipEntry);

	}

	_DMESSAGE("Ending ProcessOwnership");
}