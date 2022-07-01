#include "MagicItem.h"
#include "TESForm.h"
#include "EffectSetting.h"
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

			ExtraInfoEntry* effectEntry;

			if (effect && effect->baseEffect) {
				CreateExtraInfoEntry(effectEntry, GetName( effect->baseEffect ), "", priority_MagicItem_Effect);
				GetEffectData(effectEntry, effect, nullptr, false);
			}

			else {
				CreateExtraInfoEntry(effectEntry, GetTranslation("$UnknownEffect"), "", priority_MagicItem_Effect);
			}
			magicEffectsEntry->PushBack(effectEntry);

			logger::debug("GetSpellData: Ending Active Effect");
		}

		resultArray->PushBack(magicEffectsEntry);

		logger::debug("GetMagicItemData: End");
	}
}
