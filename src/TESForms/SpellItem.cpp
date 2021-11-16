#include "SpellItem.h"
#include "MagicItem.h"
#include "TESForm.h"
#include "Util/NameUtil.h"

void GetSpellData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	/*
	case kFormType_Spell:
	case kFormType_ScrollItem:
	case kFormType_Ingredient:
	case kFormType_Potion:
	case kFormType_Enchantment:
	{ */
	/*

	*/
	RE::SpellItem* spellItem = static_cast<RE::SpellItem*>(baseForm);
	if (spellItem) {
		logger::debug("GetSpellData: Starting spell item code");

		//spell type
		ExtraInfoEntry* spellTypeEntry;

		CreateExtraInfoEntry(spellTypeEntry, "Spell Type", GetSpellTypeName(spellItem->data.spellType), priority_Spell_SpellType);
		resultArray->PushBack(spellTypeEntry);

		//casting type
		ExtraInfoEntry* castingTypeEntry;

		CreateExtraInfoEntry(castingTypeEntry, "Cast Type", GetCastingTypeName(spellItem->data.castingType), priority_Spell_BaseCost);
		resultArray->PushBack(castingTypeEntry);

		//spell cost
		ExtraInfoEntry* spellCostEntry;

		CreateExtraInfoEntry(spellCostEntry, "Base Cost", IntToString(spellItem->data.costOverride), priority_Spell_CastTime);
		resultArray->PushBack(spellCostEntry);

		//cast time
		ExtraInfoEntry* castTimeEntry;

		CreateExtraInfoEntry(castTimeEntry, "Cast Time", FloatToString(spellItem->data.chargeTime), priority_Spell_DeliveryType);
		resultArray->PushBack(castTimeEntry);

		//Get Data for the magic effects for the spell
		GetMagicItemData(resultArray, baseForm);

		GetKeywords(resultArray, spellItem);

		/*
		BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
		if (equipSlot)
		RegisterNumber(pFxVal, "equipSlot", equipSlot->formID); */

		logger::debug("GetSpellData: Ending spell item code");
	}
	/*
	AlchemyItem * pAlchemyItem = DYNAMIC_CAST(pMagicItem, MagicItem, AlchemyItem);
	if (pAlchemyItem)
	{
	if (pAlchemyItem->itemData.useSound) {
	ExtraInfoEntry * useSound;
	movieView->CreateObject(useSound);
	scaleformExtend::FormData(useSoundView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
	pFxVal->SetMember("useSound", &useSound);
	}
	}

	EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
	if (pEnchantItem)
	{
	RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

	ExtraInfoEntry * baseEnchant;
	movieView->CreateObject(baseEnchant);
	scaleformExtend::FormData(baseEnchantView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
	pFxVal->SetMember("baseEnchant", &baseEnchant);

	ExtraInfoEntry * restrictions;
	movieView->CreateObject(restrictions);
	scaleformExtend::FormData(restrictionsView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
	pFxVal->SetMember("restrictions", &restrictions);
	}
	}	*/
}
