#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void GetMagicEffectData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);
void GetActiveEffectData(ExtraInfoEntry* resultArray, RE::ActiveEffect* activeEffect);
void GetEffectData(ExtraInfoEntry* resultArray, RE::Effect* effect, RE::Actor* caster, bool isFromActiveEffect, float durationFloat = 0.0, float elapsedTime = 0.0, bool isActive = false, float magnitude = 0.0);
bool IsEffectActive(RE::ActiveEffect* activeEffect);
