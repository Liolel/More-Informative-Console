#include "TESObjectWEAP.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void GetWeaponData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetWeaponData start");

	RE::TESObjectWEAP* weapon = static_cast<RE::TESObjectWEAP*>(baseForm);
	if (weapon) {
		//animation type
		RE::WEAPON_TYPE animationType = weapon->weaponData.animationType.get();

		ExtraInfoEntry* antimationTypeEntry;

		CreateExtraInfoEntry(antimationTypeEntry, GetTranslation("$WeaponAnimationType"), GetWeaponAnimationTypeName(animationType), priority_Weapon_AnimationType);
		resultArray->PushBack(antimationTypeEntry);

		//damage
		int damage = weapon->GetAttackDamage();

		ExtraInfoEntry* damageEntry;

		CreateExtraInfoEntry(damageEntry, GetTranslation("$Damage"), IntToString(damage), priority_Weapon_Damage);
		resultArray->PushBack(damageEntry);

		//speed
		float speed = weapon->weaponData.speed;

		ExtraInfoEntry* speedEntry;

		CreateExtraInfoEntry(speedEntry, GetTranslation("$WeaponSpeed"), FloatToString(speed), priority_Weapon_Speed);
		resultArray->PushBack(speedEntry);

		//reach
		float reach = weapon->weaponData.reach;

		ExtraInfoEntry* reachEntry;

		CreateExtraInfoEntry(reachEntry, GetTranslation("$WeaponReach"), FloatToString(reach), priority_Weapon_Reach );
		resultArray->PushBack(reachEntry);

		//stagger
		float stagger = weapon->weaponData.staggerValue;

		ExtraInfoEntry* staggerEntry;

		CreateExtraInfoEntry(staggerEntry, GetTranslation("$WeaponStagger"), FloatToString(stagger), priority_Weapon_Stagger);
		resultArray->PushBack(staggerEntry);

		//crit damage
		int critDamage = weapon->criticalData.damage;

		ExtraInfoEntry* critDamageEntry;

		CreateExtraInfoEntry(critDamageEntry, GetTranslation("$WeaponCrit"), IntToString(critDamage), priority_Weapon_CritDamage);
		resultArray->PushBack(critDamageEntry);

		RE::BGSEquipSlot* equipSlot = weapon->GetEquipSlot();
		if (equipSlot) {
			std::string equipSlotName = GetEquipTypeName(equipSlot->GetFormID());

			ExtraInfoEntry* equipSlotEntry;

			CreateExtraInfoEntry(equipSlotEntry, GetTranslation("$EquipSlot"), equipSlotName, priority_Weapon_EquipSlotName);
			resultArray->PushBack(equipSlotEntry);
		}

		//value
		int value = weapon->value;

		ExtraInfoEntry* valueEntry;

		CreateExtraInfoEntry(valueEntry, GetTranslation("$Value"), IntToString(value), priority_Weapon_Value);
		resultArray->PushBack(valueEntry);

		//weight
		float weight = weapon->weight;

		ExtraInfoEntry* weightEntry;

		CreateExtraInfoEntry(weightEntry, GetTranslation("$Weight"), FloatToString(weight), priority_Weapon_Weight);
		resultArray->PushBack(weightEntry);
	}

	logger::debug("GetWeaponData End");
}
