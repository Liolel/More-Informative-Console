#include "ExtraLocation.h"
#include "Util/NameUtil.h"
#include "TESForms/TESForm.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void ProcessLocationData(ExtraInfoEntry* resultArray, RE::BSExtraData* data)
{
	logger::debug("ProcessLocationData Start");
	RE::ExtraLocation* extraLocation = static_cast<RE::ExtraLocation*>(data);

	if (extraLocation)
	{
		auto bgsLocation = extraLocation->location;

		if (bgsLocation)
		{
			std::string locationName = GetName(bgsLocation);

			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry* locationEntry;

			CreateExtraInfoEntry(locationEntry, GetTranslation("$Location"), locationName, priority_ExtraData_Location);

			GetFormData(locationEntry, bgsLocation, nullptr);

			resultArray->PushBack(locationEntry);
		}
	}

	logger::debug("ProcessLocationData End");
}

