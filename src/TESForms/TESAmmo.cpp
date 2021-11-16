#include "TESAmmo.h"
#include "TESForm.h"

void GetAmmoData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	RE::TESAmmo* ammo = static_cast<RE::TESAmmo*>(baseForm);
	if (ammo) {
		//damage
		float damage = ammo->data.damage;

		ExtraInfoEntry* damageEntry;

		CreateExtraInfoEntry(damageEntry, "Damage", FloatToString(damage), priority_Ammo_Damage);
		resultArray->PushBack(damageEntry);

		//value
		int value = ammo->value;

		ExtraInfoEntry* valueEntry;

		CreateExtraInfoEntry(valueEntry, "Value", IntToString(value), priority_Ammo_Value);
		resultArray->PushBack(valueEntry);

		GetKeywords(resultArray, ammo);
	}
}