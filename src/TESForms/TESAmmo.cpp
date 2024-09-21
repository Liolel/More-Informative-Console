#include "TESAmmo.h"
#include "TESForm.h"
#include "TranslationCache.h"

//4-29-2022: Checked for translations needed

void GetAmmoData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESAmmo* ammo = static_cast<RE::TESAmmo*>(baseForm);
	if (ammo) 
	{		
		//damage
		float damage = ammo->data.damage;

		ExtraInfoEntry* damageEntry;

		CreateExtraInfoEntry(damageEntry, GetTranslation("$Damage"), FloatToString(damage), priority_Ammo_Damage);
		resultArray->PushBack(damageEntry);

		//value
		int value = ammo->value;

		ExtraInfoEntry* valueEntry;

		CreateExtraInfoEntry(valueEntry, GetTranslation("$Value"), IntToString(value), priority_Ammo_Value);
		resultArray->PushBack(valueEntry);

		GetKeywords(resultArray, ammo->AsKeywordForm() );
	}
}
