#include "SpellItem.h"
#include "MagicItem.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-29-2022: Checked for translations needed

void GetSpellData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::SpellItem* spellItem = static_cast<RE::SpellItem*>(baseForm);
	if (spellItem) {
		logger::debug("GetSpellData: Starting spell item code");

		//spell type
		ExtraInfoEntry* spellTypeEntry;

		CreateExtraInfoEntry(spellTypeEntry, GetTranslation("$SpellType"), GetSpellTypeName(spellItem->data.spellType), priority_Spell_SpellType);
		resultArray->PushBack(spellTypeEntry);

		//casting type
		ExtraInfoEntry* castingTypeEntry;

		CreateExtraInfoEntry(castingTypeEntry, GetTranslation("$CastingType"), GetCastingTypeName(spellItem->data.castingType), priority_Spell_BaseCost);
		resultArray->PushBack(castingTypeEntry);

		//spell cost
		ExtraInfoEntry* spellCostEntry;

		CreateExtraInfoEntry(spellCostEntry, GetTranslation("$BaseCost"), IntToString(spellItem->data.costOverride), priority_Spell_CastTime);
		resultArray->PushBack(spellCostEntry);

		//cast time
		ExtraInfoEntry* castTimeEntry;

		CreateExtraInfoEntry(castTimeEntry, GetTranslation("$CastTime"), FloatToString(spellItem->data.chargeTime), priority_Spell_DeliveryType);
		resultArray->PushBack(castTimeEntry);

		logger::debug("GetSpellData: Ending spell item code");
	}
}
