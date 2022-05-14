#include "MagicItem.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"
#include "globals.h"

//4-29-2022: Checked for translations needed

void GetMagicItemData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("GetMagicItemData: Start");

	RE::MagicItem* magicItem = static_cast<RE::MagicItem*>(baseForm);

	if (magicItem) {
		ExtraInfoEntry* magicEffectsEntry;

		CreateExtraInfoEntry(magicEffectsEntry, GetTranslation("$MagicEffects"), "", priority_MagicItem_MagicEffects);

		int numEffects = magicItem->effects.size();

		for (int i = 0; i < numEffects; i++) {
			RE::Effect* effect = magicItem->effects[i];

			if (effect && effect->baseEffect) {
				GetEffectData(magicEffectsEntry, effect, "", priority_MagicItem_Effect, nullptr);
			}

			else {
				ExtraInfoEntry* effectEntry;
				CreateExtraInfoEntry(effectEntry, GetTranslation("$UnknownEffect"), "", priority_MagicItem_Effect);
				magicEffectsEntry->PushBack(effectEntry);
			}

			logger::debug("GetSpellData: Ending Active Effect");
		}

		resultArray->PushBack(magicEffectsEntry);

		logger::debug("GetMagicItemData: End");
	}
}

void GetEffectData(ExtraInfoEntry* resultArray, RE::Effect* effect, std::string effectActiveString, priority priorityToUse, RE::Actor* caster )
{
	logger::debug("GetEffectData Start");

	ExtraInfoEntry* effectEntry;

	RE::EffectSetting* effectSetting = effect->baseEffect;
	std::string effectName = GetName(effectSetting);

	CreateExtraInfoEntry(effectEntry, effectName, effectActiveString, priorityToUse);

	//Get data for the actual effect
	GetFormData(effectEntry, effectSetting, nullptr);

	//Magnitude
	ExtraInfoEntry* magnitudeEntry;

	float magnitude = effect->effectItem.magnitude;
	CreateExtraInfoEntry(magnitudeEntry, GetTranslation("$Magnitude"), FloatToString(magnitude), priority_Effect_Magnitude);
	magnitudeEntry->SetMayCopy(false);
	effectEntry->PushBack(magnitudeEntry);

	//Duration
	ExtraInfoEntry* durationEntry;

	int duration = effect->effectItem.duration;
	CreateExtraInfoEntry(durationEntry, GetTranslation("$Duration"), IntToString(duration), priority_Effect_Duration);
	durationEntry->SetMayCopy(false);
	effectEntry->PushBack(durationEntry);

	//Magnitude
	ExtraInfoEntry* areaEntry;

	int area = effect->effectItem.area;
	CreateExtraInfoEntry(areaEntry, GetTranslation("$Area"), IntToString(area), priority_Effect_Area);
	areaEntry->SetMayCopy(false);
	effectEntry->PushBack(areaEntry);

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
				effectEntry->PushBack(casterEntry);
				MICGlobals::minimizeFormDataRead = false;
			}
		}
	}

	resultArray->PushBack(effectEntry);
}
