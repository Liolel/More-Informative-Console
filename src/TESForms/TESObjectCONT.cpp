#include "TESObjectCONT.h"
#include <Util/GeneralUtil.h>
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void GetContainerData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESObjectCONT* container = static_cast<RE::TESObjectCONT*>(baseForm);
	if (container) {

		ExtraInfoEntry* safeContainerEntry;
		std::string isSafe = BooleanToYesNoString(!HasFlag(container->data.flags.underlying(), (int)RE::CONT_DATA::Flag::kRespawn));

		CreateExtraInfoEntry(safeContainerEntry, GetTranslation("$SafeContainer"), isSafe, priority_Container_IsSafe);

		resultArray->PushBack(safeContainerEntry);
	}
}
