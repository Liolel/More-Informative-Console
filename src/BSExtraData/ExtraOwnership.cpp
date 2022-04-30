#include "ExtraOwnership.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void ProcessOwnership(ExtraInfoEntry* resultArray, RE::BSExtraData* data )
{
	logger::debug("Starting ProcessOwnership");

	RE::ExtraOwnership* ownershipData = static_cast<RE::ExtraOwnership*>(data);

	if (ownershipData)
	{
		RE::TESForm* owner = ownershipData->owner;
		std::string ownerName = GetName(owner);

		//Placeholder for seeing what has editor IDs
		ExtraInfoEntry* ownershipEntry;

		CreateExtraInfoEntry(ownershipEntry, GetTranslation("$Owner"), ownerName, priority_ExtraData_Ownership);

		GetFormData(ownershipEntry, owner, nullptr);

		resultArray->PushBack(ownershipEntry);

	}

	logger::debug("Ending ProcessOwnership");
}
