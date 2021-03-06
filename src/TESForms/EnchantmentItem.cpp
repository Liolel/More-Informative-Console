#include "EnchantmentItem.h"
#include "MagicItem.h"
#include "TranslationCache.h"

//4-24-2022: Checked for translations needed

void GetCharge(ExtraInfoEntry* resultArray, RE::ExtraDataList* extraList , RE::TESEnchantableForm* item, RE::ExtraEnchantment * extraEnchantment)
{
	double charge = 0.0;
	double maximumCharge = 0.0;

	if (item)
	{
		maximumCharge = item->amountofEnchantment;
		charge = maximumCharge;
	}
	else if (extraEnchantment)
	{
		maximumCharge = extraEnchantment->charge;
		charge = maximumCharge;
	}
	if (extraList
		&& extraList->HasType(RE::ExtraDataType::kCharge) )
	{
		auto chargeData = extraList->GetByType<RE::ExtraCharge>();
		
		if( chargeData )
		{
			charge = chargeData->charge;
		}
	}
	if (maximumCharge > 0.0 )
	{
		ExtraInfoEntry* chargeEntry;
		CreateExtraInfoEntry(chargeEntry, GetTranslation("$Charge"), DoubleToString( charge ), priority_Enchantment_Charge);
		chargeEntry->SetMayCopy(false);
		resultArray->PushBack(chargeEntry);
		ExtraInfoEntry* maximumChargeEntry;
		CreateExtraInfoEntry(maximumChargeEntry, GetTranslation("$MaximumCharge"), DoubleToString(maximumCharge), priority_Enchantment_MaximumCharge);
		maximumChargeEntry->SetMayCopy(false);
		resultArray->PushBack(maximumChargeEntry);
	}
}
