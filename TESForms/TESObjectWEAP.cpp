#include "TESObjectWEAP.h"
#include "TESForm.h"
#include "Util/NameUtil.h"

void GetWeaponData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetWeaponData start");

	RE::TESObjectWEAP* weapon = static_cast<RE::TESObjectWEAP*>(baseForm);
	if (weapon)
	{
		//animation type
		RE::WEAPON_TYPE animationType = weapon->weaponData.animationType.get();

		ExtraInfoEntry* antimationTypeEntry;

		CreateExtraInfoEntry(antimationTypeEntry, "Animation type", GetWeaponAnimationTypeName(animationType), priority_Weapon_AnimationType);
		resultArray->PushBack(antimationTypeEntry);

		//damage
		int damage = weapon->GetAttackDamage();

		ExtraInfoEntry* damageEntry;

		CreateExtraInfoEntry(damageEntry, "Damage", IntToString(damage), priority_Weapon_Damage);
		resultArray->PushBack(damageEntry);

		//speed
		float speed = weapon->weaponData.speed;

		ExtraInfoEntry* speedEntry;

		CreateExtraInfoEntry(speedEntry, "Speed", FloatToString(speed), priority_Weapon_Speed);
		resultArray->PushBack(speedEntry);

		//reach
		float reach = weapon->weaponData.reach;

		ExtraInfoEntry* reachEntry;

		CreateExtraInfoEntry(reachEntry, "Reach", FloatToString(reach), priority_Weapon_Reach );
		resultArray->PushBack(reachEntry);

		//stagger
		float stagger = weapon->weaponData.staggerValue;

		ExtraInfoEntry* staggerEntry;

		CreateExtraInfoEntry(staggerEntry, "Stagger", FloatToString(stagger), priority_Weapon_Stagger);
		resultArray->PushBack(staggerEntry);


		//crit damage
		int critDamage = weapon->criticalData.damage;

		ExtraInfoEntry* critDamageEntry;

		CreateExtraInfoEntry(critDamageEntry, "Crit damage", IntToString(critDamage), priority_Weapon_CritDamage);
		resultArray->PushBack(critDamageEntry);
		
		RE::BGSEquipSlot * equipSlot = weapon->GetEquipSlot();
		if (equipSlot)
		{
			RE::TESForm* equipSlotForm = static_cast<RE::TESForm*>(equipSlot);
			std::string equipSlotName = GetEquipTypeName(equipSlot->GetFormID());

			ExtraInfoEntry * equipSlotEntry;

			CreateExtraInfoEntry(equipSlotEntry, "Equip slot", equipSlotName, priority_Weapon_EquipSlotName);
			resultArray->PushBack(equipSlotEntry);
		}

		//value
		int value = weapon->value;

		ExtraInfoEntry * valueEntry;

		CreateExtraInfoEntry(valueEntry, "Value", IntToString(value), priority_Weapon_Value);
		resultArray->PushBack(valueEntry);

		//weight
		float weight = weapon->weight;

		ExtraInfoEntry * weightEntry;

		CreateExtraInfoEntry(weightEntry, "Weight", FloatToString(weight), priority_Weapon_Weight);
		resultArray->PushBack(weightEntry);

		GetKeywords(resultArray, weapon);
	}

	logger::debug("GetWeaponData End");
}
