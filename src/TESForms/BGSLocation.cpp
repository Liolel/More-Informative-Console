#include "BGSLocation.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-23-2022: Checked for translations needed

void GetLocationEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::BGSLocation* bgsLocation = static_cast<RE::BGSLocation*>(baseForm);
	if (bgsLocation) {
		
		//cleared
		bool cleared = bgsLocation->cleared;

		ExtraInfoEntry* clearedEntry;

		CreateExtraInfoEntry(clearedEntry, GetTranslation("$Cleared"), BooleanToYesNoString(cleared), priority_Location_Cleared);
		resultArray->PushBack(clearedEntry);

		//parent location
		auto bgsLocationParent = bgsLocation->parentLoc;

		if (bgsLocationParent)
		{
			ExtraInfoEntry* parentLocEntry;
			std::string parentLocName = GetName(bgsLocationParent);

			CreateExtraInfoEntry(parentLocEntry, GetTranslation("$Parent"), parentLocName, priority_Location_ParentLocation);
			GetFormData(parentLocEntry, bgsLocationParent, nullptr);
			resultArray->PushBack(parentLocEntry);
		}
	}
}
