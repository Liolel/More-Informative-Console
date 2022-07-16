#include "TESGlobal.h"

void GetGlobalInformation(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESGlobal* global = static_cast<RE::TESGlobal*>(baseForm);

	if (global) 
	{
		ExtraInfoEntry* globalTypeEntry;
		std::string type = "";

		switch ( global->type.get() ) 
		{
			case RE::TESGlobal::Type::kFloat:
				type = GetTranslation("$GlobalTypeFloat");
				break;
			case RE::TESGlobal::Type::kShort:
				type = GetTranslation("$GlobalTypeShort");
				break;
			case RE::TESGlobal::Type::kLong:
				type = GetTranslation("$GlobalTypeLong");
				break;
		}

		CreateExtraInfoEntry(globalTypeEntry, GetTranslation("$GlobalType"), type, priorty_Global_Type);
		resultArray->PushBack(globalTypeEntry);

		ExtraInfoEntry* globalValueEntry;

		CreateExtraInfoEntry(globalValueEntry, GetTranslation("$GlobalValue"), FloatToString(global->value), priorty_Global_Value);
		resultArray->PushBack(globalValueEntry);
	}
}

float GetGlobalValue(RE::TESForm* baseForm)
{
	float value = -9999.9F;

	if (baseForm->formType = RE::FormType::Global) 
	{
		RE::TESGlobal* global = static_cast<RE::TESGlobal*>(baseForm);

		if (global) 
		{
			value = global->value;
		}
	}

	return value;
}
