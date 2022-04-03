#include "BGSLocation.h"
#include "TESForm.h"
#include "Util/NameUtil.h"

void GetLocationEntry(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::BGSLocation* bgsLocation = static_cast<RE::BGSLocation*>(baseForm);
	if (bgsLocation) {
		
		//cleared
		bool cleared = bgsLocation->cleared;

		ExtraInfoEntry* clearedEntry;

		CreateExtraInfoEntry(clearedEntry, "Cleared", BooleanToYesNoString(cleared), priority_Location_Cleared);
		resultArray->PushBack(clearedEntry);

		//parent location
		auto bgsLocationParent = bgsLocation->parentLoc;

		if (bgsLocationParent)
		{
			ExtraInfoEntry* parentLocEntry;
			std::string parentLocName = GetName(bgsLocationParent);

			CreateExtraInfoEntry(parentLocEntry, "Parent", parentLocName, priority_Location_ParentLocation);
			GetFormData(parentLocEntry, bgsLocationParent, nullptr);
			resultArray->PushBack(parentLocEntry);

		}

		GetKeywords(resultArray, bgsLocation);
	}
}
