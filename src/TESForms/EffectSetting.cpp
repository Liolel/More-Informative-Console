#pragma once
#include "EffectSetting.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include <Util/GeneralUtil.h>

void GetMagicEffectData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetExtraData: GetMagicEffectData Start");

	RE::EffectSetting* effectSetting = static_cast<RE::EffectSetting*>(baseForm);

	if (effectSetting) {
		logger::debug("GetExtraData: GetMagicEffectData Cast succeeded");

		//Magic School
		int skill = (int)effectSetting->data.associatedSkill;
		ExtraInfoEntry* skillEntry;

		CreateExtraInfoEntry(skillEntry, "Magic Skill", GetActorValueName(skill), priority_EffectSetting_Skill);
		resultArray->PushBack(skillEntry);

		//Minimum Skill Level

		int minimumSkill = effectSetting->data.minimumSkill;
		ExtraInfoEntry* minimumSkillEntry;

		CreateExtraInfoEntry(minimumSkillEntry, "Minimum Skill", std::to_string(minimumSkill), priority_EffectSetting_MinimumSkill);
		resultArray->PushBack(minimumSkillEntry);

		//Effect type

		int effectType = (int)effectSetting->data.archetype;
		ExtraInfoEntry* effectTypeEntry;

		CreateExtraInfoEntry(effectTypeEntry, "Effect type", GetEffectTypeName(effectType), priority_EffectSetting_EffectType);
		resultArray->PushBack(effectTypeEntry);

		//First AV
		int primaryAV = (int)effectSetting->data.primaryAV;
		ExtraInfoEntry* primaryAVEntry;

		CreateExtraInfoEntry(primaryAVEntry, "Primary AV", GetActorValueName(primaryAV), priority_EffectSetting_PrimaryAV);
		resultArray->PushBack(primaryAVEntry);

		//Second AV
		int secondaryAV = (int)effectSetting->data.secondaryAV;
		ExtraInfoEntry* secondaryAVEntry;

		CreateExtraInfoEntry(secondaryAVEntry, "Secondary AV", GetActorValueName(secondaryAV), priority_EffectSetting_SecondaryAV);
		resultArray->PushBack(secondaryAVEntry);

		//Resistence
		int resistence = (int)effectSetting->data.resistVariable;
		ExtraInfoEntry* resistenceEntry;

		CreateExtraInfoEntry(resistenceEntry, "Resistance", GetActorValueName(resistence), priority_EffectSetting_Resistance);
		resultArray->PushBack(resistenceEntry);

		//delivery type
		ExtraInfoEntry* deliveryTypeEntry;

		CreateExtraInfoEntry(deliveryTypeEntry, "Delivery Type", GetDeliveryTypeName(effectSetting->data.delivery), priority_EffectSetting_DeliveryType);
		resultArray->PushBack(deliveryTypeEntry);

		//Hostile Flag
		ExtraInfoEntry* hostileEntry;

		std::string hostile = BooleanToYesNoString(HasFlag(effectSetting->data.flags.underlying(), (int)RE::EffectSetting::EffectSettingData::Flag::kHostile));
		CreateExtraInfoEntry(hostileEntry, "Hostile", hostile, priority_EffectSetting_Hostile);
		resultArray->PushBack(hostileEntry);
		/*



		//RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags); //I added the hostile flag. Need to check what else to add
		RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType); //do these next 3 when I add spells
		RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
		RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
		RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);*/

		GetKeywords(resultArray, effectSetting);
	}
	logger::debug("GetExtraData: GetMagicEffectData End");
}
