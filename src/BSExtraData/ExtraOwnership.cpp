#include "ExtraOwnership.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"
#include "TranslationCache.h"
#include "globals.h"

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

		bool isFormDataCurrentMinimized = MICGlobals::minimizeFormDataRead;

		CreateExtraInfoEntry(ownershipEntry, GetTranslation("$Owner"), ownerName, priority_ExtraData_Ownership);

		MICGlobals::minimizeFormDataRead = true;

		GetFormData(ownershipEntry, owner, nullptr);

		MICGlobals::minimizeFormDataRead = isFormDataCurrentMinimized;

		resultArray->PushBack(ownershipEntry);

	}

	logger::debug("Ending ProcessOwnership");
}
