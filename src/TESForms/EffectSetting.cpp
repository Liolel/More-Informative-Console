#pragma once
#include "EffectSetting.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"
#include "globals.h"
#include <Util/GeneralUtil.h>
#include <math.h>

//4-24-2022: Checked for translations needed

void GetMagicEffectData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetExtraData: GetMagicEffectData Start");

	RE::EffectSetting* effectSetting = static_cast<RE::EffectSetting*>(baseForm);

	if (effectSetting) {
		logger::debug("GetExtraData: GetMagicEffectData Cast succeeded");

		//Magic School
		int skill = (int)effectSetting->data.associatedSkill;
		ExtraInfoEntry* skillEntry;

		CreateExtraInfoEntry(skillEntry, GetTranslation("$MagicSkill"), GetActorValueName(skill), priority_EffectSetting_Skill);
		resultArray->PushBack(skillEntry);

		//Minimum Skill Level

		int minimumSkill = effectSetting->data.minimumSkill;
		ExtraInfoEntry* minimumSkillEntry;

		CreateExtraInfoEntry(minimumSkillEntry, GetTranslation("$MinimumSkill"), std::to_string(minimumSkill), priority_EffectSetting_MinimumSkill);
		resultArray->PushBack(minimumSkillEntry);

		//Effect type

		int effectType = (int)effectSetting->data.archetype;
		ExtraInfoEntry* effectTypeEntry;

		CreateExtraInfoEntry(effectTypeEntry, GetTranslation("$EffectType"), GetEffectTypeName(effectType), priority_EffectSetting_EffectType);
		resultArray->PushBack(effectTypeEntry);

		//First AV
		int primaryAV = (int)effectSetting->data.primaryAV;
		ExtraInfoEntry* primaryAVEntry;

		CreateExtraInfoEntry(primaryAVEntry, GetTranslation("$PrimaryAV"), GetActorValueName(primaryAV), priority_EffectSetting_PrimaryAV);
		resultArray->PushBack(primaryAVEntry);

		//Second AV
		int secondaryAV = (int)effectSetting->data.secondaryAV;
		ExtraInfoEntry* secondaryAVEntry;

		CreateExtraInfoEntry(secondaryAVEntry, GetTranslation("$SecondaryAV"), GetActorValueName(secondaryAV), priority_EffectSetting_SecondaryAV);
		resultArray->PushBack(secondaryAVEntry);

		//Resistence
		int resistence = (int)effectSetting->data.resistVariable;
		ExtraInfoEntry* resistenceEntry;

		CreateExtraInfoEntry(resistenceEntry, GetTranslation("$Resistance"), GetActorValueName(resistence), priority_EffectSetting_Resistance);
		resultArray->PushBack(resistenceEntry);

		//delivery type
		ExtraInfoEntry* deliveryTypeEntry;

		CreateExtraInfoEntry(deliveryTypeEntry, GetTranslation("$DeliveryType"), GetDeliveryTypeName(effectSetting->data.delivery), priority_EffectSetting_DeliveryType);
		resultArray->PushBack(deliveryTypeEntry);

		//Hostile Flag
		ExtraInfoEntry* hostileEntry;

		std::string hostile = BooleanToYesNoString(HasFlag(effectSetting->data.flags.underlying(), (int)RE::EffectSetting::EffectSettingData::Flag::kHostile));
		CreateExtraInfoEntry(hostileEntry, GetTranslation("$FlagHostile"), hostile, priority_EffectSetting_Hostile);
		resultArray->PushBack(hostileEntry);
	}
	logger::debug("GetExtraData: GetMagicEffectData End");
}


void GetEffectData(ExtraInfoEntry* resultArray, RE::Effect* effect, RE::Actor* caster, bool isFromActiveEffect, float durationFloat, float elapsedTime, bool isActive, float magnitude )
{
	logger::debug("GetEffectData Start");

	//Get data for the actual effect
	RE::EffectSetting* effectSetting = effect->baseEffect;
	GetFormData(resultArray, effectSetting, nullptr);

	//Magnitude
	ExtraInfoEntry* areaEntry;

	int area = effect->effectItem.area;
	CreateExtraInfoEntry(areaEntry, GetTranslation("$Area"), IntToString(area), priority_Effect_Area);
	areaEntry->SetMayCopy(false);
	resultArray->PushBack(areaEntry);

	if (isFromActiveEffect)
	{
		//Magnitude
		ExtraInfoEntry* magnitudeEntry;

		magnitude = std::abs(magnitude);

		CreateExtraInfoEntry(magnitudeEntry, GetTranslation("$Magnitude"), FloatToString(magnitude), priority_Effect_Magnitude);
		magnitudeEntry->SetMayCopy(false);
		resultArray->PushBack(magnitudeEntry);

		if (durationFloat > 0.0)
		{
			durationFloat = durationFloat - elapsedTime;
		}

		//Duration
		ExtraInfoEntry* durationEntry;

		CreateExtraInfoEntry(durationEntry, GetTranslation("$Duration"), FloatToString(durationFloat), priority_Effect_Duration);
		durationEntry->SetMayCopy(false);
		resultArray->PushBack(durationEntry);

		//Active
		ExtraInfoEntry* activeEntry;
		CreateExtraInfoEntry(activeEntry, GetTranslation("$EffectActive"), BooleanToYesNoString(isActive), priority_Effect_Active);
		activeEntry->SetMayCopy(false);
		resultArray->PushBack(activeEntry);
	}
	else
	{
		//Magnitude
		ExtraInfoEntry* magnitudeEntry;

		float effectMagnitude = effect->effectItem.magnitude;
		CreateExtraInfoEntry(magnitudeEntry, GetTranslation("$Magnitude"), FloatToString(effectMagnitude), priority_Effect_Magnitude);
		magnitudeEntry->SetMayCopy(false);
		resultArray->PushBack(magnitudeEntry);

		//Duration
		ExtraInfoEntry* durationEntry;

		int durationInt = effect->effectItem.duration;
		CreateExtraInfoEntry(durationEntry, GetTranslation("$Duration"), IntToString(durationInt), priority_Effect_Duration);
		durationEntry->SetMayCopy(false);
		resultArray->PushBack(durationEntry);
	}

	//Caster
	if (!MICGlobals::minimizeFormDataRead)
	{
		if (caster)
		{
			auto baseFormCaster = caster->GetActorBase();

			if (baseFormCaster)
			{
				MICGlobals::minimizeFormDataRead = true;
				ExtraInfoEntry* casterEntry;
				std::string casterName = GetName(baseFormCaster);
				CreateExtraInfoEntry(casterEntry, GetTranslation("$Caster"), casterName, priority_Effect_Caster);
				GetFormData(casterEntry, baseFormCaster, caster);
				resultArray->PushBack(casterEntry);
				MICGlobals::minimizeFormDataRead = false;
			}
		}
	}
}


void GetActiveEffectData(ExtraInfoEntry* resultArray, RE::ActiveEffect* activeEffect)
{
	logger::debug("EffectSetting: Active Effect MGEF found");

	std::string effectActive;

	RE::Effect* effect = activeEffect->effect;
	priority priorityToUse;

	bool active = IsEffectActive(activeEffect);

	if (!active)
	{
		effectActive = GetTranslation("$EffectInactive");
		priorityToUse = priority_MagicItem_Effect_Inactive;
	}
	else
	{
		effectActive = GetTranslation("$EffectActive");
		priorityToUse = priority_MagicItem_Effect_Active;
	}

	auto caster = activeEffect->GetCasterActor().get();

	ExtraInfoEntry* effectEntry;

	RE::EffectSetting* effectSetting = effect->baseEffect;
	std::string effectName = GetName(effectSetting);

	CreateExtraInfoEntry(effectEntry, effectName, effectActive, priorityToUse);

	GetEffectData(effectEntry, effect, caster, true, activeEffect->duration, activeEffect->elapsedSeconds, active, activeEffect->magnitude );

	resultArray->PushBack(effectEntry);
}

bool IsEffectActive(RE::ActiveEffect* activeEffect)
{
	return !(HasFlag(activeEffect->flags.underlying(), (int)RE::ActiveEffect::Flag::kInactive));
}
