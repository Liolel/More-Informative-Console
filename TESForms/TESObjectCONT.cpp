#include "TESObjectCONT.h"

void GetContainerData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESObjectCONT* container = static_cast<RE::TESObjectCONT*>(baseForm);
	if (container)
	{
		int respawnsFlag = 0x02;

		ExtraInfoEntry* safeContainerEntry;
		std::string isSafe = BooleanToYesNoString(!HasFlag( container->data.flags.underlying()
														  , (int)RE::CONT_DATA::Flag::kRespawn) );

		CreateExtraInfoEntry(safeContainerEntry, "Safe Container", isSafe, priority_Container_IsSafe);

		resultArray->PushBack(safeContainerEntry);
	}
}
