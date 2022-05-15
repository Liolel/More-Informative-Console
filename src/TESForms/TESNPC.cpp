#include "TESNPC.h"
#include "MagicItem.h"
#include "TESForm.h"
#include "Util/NameUtil.h"
#include "globals.h"
#include "TranslationCache.h"
#include <Util/GeneralUtil.h>

//Adapted from SKSE source code

//4-30-2022: Checked for translations needed

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm)
{
	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	auto npcRoot = npc;
	while (npcRoot->faceNPC
		   && npcRoot->formID >= 0xFF000000)
	{
		npcRoot = npcRoot->faceNPC;
	}

	return npcRoot;
}

void GetCharacterData(ExtraInfoEntry* resultArray, RE::TESForm* refForm, RE::TESForm* baseForm)
{
	logger::debug("GetCharacterData: GetCharacter info start");

	//Get all of the various objects we need
	RE::Actor* actor = nullptr;

	if (refForm) {
		actor = static_cast<RE::Actor*>(refForm);
	}

	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	RE::PlayerCharacter* player = nullptr;

	if (baseForm->formID == playerBaseFormID) {
		player = RE::PlayerCharacter::GetSingleton();
	}

	RE::TESActorBase* actorBase = static_cast<RE::TESActorBase*>(npc);

	if (npc) {
		if (actorBase) {
			logger::debug("GetCharacterData: GetCharacter info casts worked");

			logger::debug("GetCharacterData: Starting Race");

			//Handle Race
			ExtraInfoEntry* raceEntry;

			RE::TESRace* pRace = npc->race;

			//Save the race so we can filter ARMA by the race later
			MICGlobals::filterARMAByRace = pRace;

			std::string raceName = GetName(pRace);

			CreateExtraInfoEntry(raceEntry, GetTranslation("$Race"), raceName, priority_Actor_Race);

			GetFormData(raceEntry, pRace, nullptr);

			resultArray->PushBack(raceEntry);

			logger::debug("GetCharacterData: Ending Race");

			//Handle Spells
			GetSpellsForNPC(resultArray, actor, actorBase);

			//If we are looking at an actor there is additional data we can get specific to that fact
			if (actor) {
				GetActorData(resultArray, actor);
			}

			GetLevelData(resultArray, actor, npc);

			//Perks
			GetPerksForNPC(resultArray, actorBase, player);

			//Appearance
			GetNPCAppearanceData(resultArray, npc);

			GetFactionsForNPC(resultArray, actor, actorBase);
		}
	}

	logger::debug("GetExtraData: GetCharacter End");
}

void GetSpellsForNPC(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESActorBase* actorBase)
{
	//Spells
	ExtraInfoEntry* allSpellsEntry;

	CreateExtraInfoEntry(allSpellsEntry, GetTranslation("$Spells"), "", priority_Actor_Spells);

	logger::debug("GetSpellsForNPC: Starting Added Spells");

	if (actor) {
		int numberOfAddedSpells = actor->addedSpells.size();

		//Added Spells
		for (int i = 0; i < numberOfAddedSpells; i++) {
			ExtraInfoEntry* spellEntry;

			RE::SpellItem* spell = actor->addedSpells[i];

			if (spell)
			{
				std::string spellName = GetName(spell);

				CreateExtraInfoEntry(spellEntry, spellName, GetTranslation("$AddedSpell"), priority_Actor_Spells_AddedSpell);

				GetFormData(spellEntry, spell, nullptr);

				allSpellsEntry->PushBack(spellEntry);
			}
		}
	}

	logger::debug("GetSpellsForNPC: Starting Base Spells");

	if (actorBase->actorEffects) {
		//Actor Base Spells
		int numberOfBaseSpells = actorBase->actorEffects->numSpells;

		for (int i = 0; i < numberOfBaseSpells; i++) {
			ExtraInfoEntry* spellEntry;

			RE::SpellItem* spell = actorBase->actorEffects->spells[i];

			if (spell)
			{
				std::string spellName = GetName(spell);

				CreateExtraInfoEntry(spellEntry, spellName, GetTranslation("$BaseSpell"), priority_Actor_Spells_BaseSpell);

				GetFormData(spellEntry, spell, nullptr);

				allSpellsEntry->PushBack(spellEntry);
			}
		}
	}

	resultArray->PushBack(allSpellsEntry);

	logger::debug("GetCharacterData:  Done with spells");
}

void GetActorData(ExtraInfoEntry* resultArray, RE::Actor* actor)
{
	//Create a subarray to hold all the active effects for an actor
	ExtraInfoEntry* activeEffectsEntry;

	CreateExtraInfoEntry(activeEffectsEntry, GetTranslation("$Effects"), "", priority_Actor_Effects);

#ifndef SKYRIMVR
	RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = actor->GetActiveEffectList();
	logger::debug("GetCharacterData: Active Effects Gotten");

	if (activeEffects) {
		RE::BSSimpleList<RE::ActiveEffect*>::iterator itrEnd = activeEffects->end();

		for (RE::BSSimpleList<RE::ActiveEffect*>::iterator itr = activeEffects->begin(); itr != itrEnd; ++itr) {
			logger::debug("GetCharacterData: Starting Active Effect");

			RE::ActiveEffect* activeEffect = *(itr);

			ExtraInfoEntry* effectEntry;

			if (activeEffect && activeEffect->effect) {
				logger::debug("GetCharacterData: Active Effect MGEF found");

				std::string effectActive;

				RE::Effect* effect = activeEffect->effect;
				priority priorityToUse;

				if (HasFlag(activeEffect->flags.underlying(), (int)RE::ActiveEffect::Flag::kInactive)) 
				{
					effectActive = GetTranslation("$EffectInactive");
					priorityToUse = priority_MagicItem_Effect_Inactive;
				} else {
					effectActive = GetTranslation("$EffectActive");
					priorityToUse = priority_MagicItem_Effect_Active;
				}

				auto caster = activeEffect->GetCasterActor().get();

				GetEffectData(activeEffectsEntry, effect, effectActive, priorityToUse, caster );				
			}

			//This is only reached if there is an active effect without a actual corrosponding effect. Probally impossible but here's some code to handle it just in case
			else {
				CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "", priority_MagicItem_Effect);
				activeEffectsEntry->PushBack(effectEntry);
			}

			logger::debug("GetCharacterData: Ending Active Effect");
		}
	}
#else
	int total = 0;
	logger::debug("GetCharacterData: Starting Active Effect");

	actor->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
		logger::debug("GetCharacterData: Visiting Active Effect {}", total++);
		if (activeEffect) {
			ExtraInfoEntry* effectEntry;
			if (activeEffect->effect) {
				logger::debug("GetCharacterData: Active Effect MGEF found");
				std::string effectActive;
				priority priorityToUse;

				RE::Effect* effect = activeEffect->effect;

				if (HasFlag(activeEffect->flags.underlying(), (int)RE::ActiveEffect::Flag::kInactive)) {
					effectActive = "Inactive";

					priorityToUse = priority_MagicItem_Effect_Inactive;
				} else {
					effectActive = "Active";

					priorityToUse = priority_MagicItem_Effect_Active;
				}

				logger::debug("ActiveEffect: {}, magnitude: {}, duration:{}, formID: {}", activeEffect->effect->baseEffect->GetFullName(), activeEffect->magnitude, activeEffect->duration, activeEffect->effect->baseEffect->formID);

				auto caster = activeEffect->GetCasterActor().get();

				GetEffectData(activeEffectsEntry, effect, effectActive, priorityToUse, caster );
			}

			//This is only reached if there is an active effect without a actual corrosponding effect. Probally impossible but here's some code to handle it just in case
			else {
				CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "", priority_MagicItem_Effect);
				activeEffectsEntry->PushBack(effectEntry);
			}
			logger::debug("GetCharacterData: Ending Active Effect");
		}
		return RE::BSContainer::ForEachResult::kContinue;
	});

#endif

	resultArray->PushBack(activeEffectsEntry);

	logger::debug("GetActorData: Active Effects Done");

	//Add Health/Magicka/Stamina to the main subarray
	GetActorValue(resultArray, actor, actorValueHealthIndex, priority_Actor_Health);
	GetActorValue(resultArray, actor, actorValueMagickaIndex, priority_Actor_Magicka);
	GetActorValue(resultArray, actor, actorValueStaminaIndex, priority_Actor_Stamina);

	//Get all actor values in a subarray
	ExtraInfoEntry* actorValueArray;
	CreateExtraInfoEntry(actorValueArray, GetTranslation("$ActorValues"), "", priority_Actor_ActorValues);
	actorValueArray->disableSortingByName = true; //for actor values specifically we want to keep the sorting of the index numbers

	for (int i = 0; i < totalNumberOfActorValues; i++) {
		GetActorValue(actorValueArray, actor, i, priority_Actor_ActorValues_ActorValue);
	}

	resultArray->PushBack(actorValueArray);

	logger::debug("GetActorData: actor values gotten");

	RE::AIProcess* aiProcess = actor->currentProcess;

	if (aiProcess) {
		RE::TESForm* currentPackage = aiProcess->currentPackage.package;

		if (currentPackage) {
			logger::debug("GetActorData: Found current package");

			std::string packageName = GetName(currentPackage);

			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry* packageEntry;

			CreateExtraInfoEntry(packageEntry, GetTranslation("$CurrentPackage"), packageName, priority_Actor_CurrentPackage);

			GetFormData(packageEntry, currentPackage, nullptr);

			resultArray->PushBack(packageEntry);
		}
	}

	//Check if a npc is protected/essential
	ExtraInfoEntry* protectionEntry;
	std::string protectionStatus;

	if (HasFlag(actor->boolFlags.underlying(), (int)RE::Actor::BOOL_FLAGS::kEssential)) {
		protectionStatus = GetTranslation("$ProtectionEssential");
	}

	else if (HasFlag(actor->boolFlags.underlying(), (int)RE::Actor::BOOL_FLAGS::kProtected)) {
		protectionStatus = GetTranslation("$ProtectionProtected");
	}

	else {
		protectionStatus = GetTranslation("$ProtectionNone");
	}
	CreateExtraInfoEntry(protectionEntry, GetTranslation("$Protection"), protectionStatus, priority_Actor_Protection);

	resultArray->PushBack(protectionEntry);

	logger::debug("GetActorData: End");
}

void GetActorValue(ExtraInfoEntry* resultArray, RE::Actor* actor, int id, priority actorValuePriority)
{
	logger::debug("GetExtraData: GetActover Value Start");

	RE::ActorValue actorValue = (RE::ActorValue)id;
	ExtraInfoEntry* actorValueEntry;

	if (id < totalNumberOfActorValues) {
		std::string valueName = GetActorValueName(id);
		float baseValue = actor->GetBaseActorValue(actorValue);
		float currentValue = actor->GetActorValue(actorValue);
		float maxValue = actor->GetPermanentActorValue(actorValue);

		CreateExtraInfoEntry(actorValueEntry, valueName, FloatToString(currentValue), actorValuePriority);

		//create a subarray for the base  current and maximum

		ExtraInfoEntry *baseValueEntry, *currentValueEntry, *maxValueEntry;

		CreateExtraInfoEntry(baseValueEntry, GetTranslation("$ActorValueBase"), FloatToString(baseValue), priority_ActorValue_Base);
		CreateExtraInfoEntry(currentValueEntry, GetTranslation("$ActorValueCurrent"), FloatToString(currentValue), priority_ActorValue_Current);
		CreateExtraInfoEntry(maxValueEntry, GetTranslation("$ActorValueMax"), FloatToString(maxValue), priority_ActorValue_Max);

		actorValueEntry->PushBack(baseValueEntry);
		actorValueEntry->PushBack(currentValueEntry);
		actorValueEntry->PushBack(maxValueEntry);

		resultArray->PushBack(actorValueEntry);
	}

	logger::debug("GetExtraData: GetActover Value End");
}

void GetLevelData(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESNPC* npc)
{
	//Level stuff
	logger::debug("GetLevelData: Start");

	if (actor) {
		int level = actor->GetLevel();

		ExtraInfoEntry* levelEntry;

		CreateExtraInfoEntry(levelEntry, GetTranslation("$Level"), IntToString(level), priority_Actor_Level);
		resultArray->PushBack(levelEntry);
	}

	ExtraInfoEntry* isPcLeveledEntry;

	bool isLevelMult = HasFlag(npc->actorData.actorBaseFlags.underlying(), (int)RE::ACTOR_BASE_DATA::Flag::kPCLevelMult);
	CreateExtraInfoEntry(isPcLeveledEntry, GetTranslation("$LevelIsPcLevelMult"), BooleanToYesNoString(isLevelMult), priority_Actor_IsPCLeveleMult);

	if (isLevelMult) {
		logger::debug("GetLevelData: GetCharacter pc level mult");

		double levelMult = (double)npc->actorData.level / 1000.0;  //I don't know why the level mult is stored in memory as 1000 times what the value you'd see in TESEdit is, but this division is needed to fix that
		int minLevel = npc->actorData.calcLevelMin;
		int maxLevel = npc->actorData.calcLevelMax;

		ExtraInfoEntry *levelMultEntry, *minLevelEntry, *maxLevelEntry;

		CreateExtraInfoEntry(levelMultEntry, GetTranslation("$LevelMult"), DoubleToString(levelMult), priority_Actor_IsPCLeveleMult_LevelMult);
		isPcLeveledEntry->PushBack(levelMultEntry);

		CreateExtraInfoEntry(minLevelEntry, GetTranslation("$LevelMin"), IntToString(minLevel), priority_Actor_IsPCLeveleMult_LevelMin);
		isPcLeveledEntry->PushBack(minLevelEntry);

		CreateExtraInfoEntry(maxLevelEntry, GetTranslation("$LevelMax"), IntToString(maxLevel), priority_Actor_IsPCLeveleMult_LevelMax);
		isPcLeveledEntry->PushBack(maxLevelEntry);
	}

	resultArray->PushBack(isPcLeveledEntry);

	logger::debug("GetLevelData: End");
}

#ifdef SKYRIMVR
#pragma warning(disable:4100) //the player parameter is unused in the VR branch, but we still need it defined as this codebase is shared with the SSE and AE branches that do use that parameter
#endif

void GetPerksForNPC(ExtraInfoEntry* resultArray, RE::TESActorBase* actorBase, RE::PlayerCharacter* player)
{
	logger::debug("Starting GetPerks");
	int numPerks = actorBase->perkCount;

	ExtraInfoEntry* perks;
	CreateExtraInfoEntry(perks, GetTranslation("$Perks"), "", priority_Actor_Perks);

	for (int i = 0; i < numPerks; i++) {
		RE::BGSPerk* perk = actorBase->perks[i].perk;

		if (perk) {
			std::string name = GetName(perk);

			ExtraInfoEntry* perkEntry;

			CreateExtraInfoEntry(perkEntry, name, "", priority_Actor_Perks_Perk);

			GetFormData(perkEntry, perk, nullptr);

			perks->PushBack(perkEntry);
		}
	}

#ifndef SKYRIMVR  // player->addedPerks isn't an iteratable in VR and is still undiscovered
	if (player != nullptr) {
		logger::debug(" GetPerks: Starting Player Perks ");
		int numPlayerPerks = player->addedPerks.size();

		for (int i = 0; i < numPlayerPerks; i++) {
			RE::BGSPerk* perk = player->addedPerks[i]->perk;

			if (perk) {
				std::string name = GetName(perk);

				ExtraInfoEntry* perkEntry;

				CreateExtraInfoEntry(perkEntry, name, "", priority_Actor_Perks_Perk);

				GetFormData(perkEntry, perk, nullptr);

				perks->PushBack(perkEntry);
			}
		}
	}
#endif

	resultArray->PushBack(perks);

	logger::debug("Ending GetPerks");
}

#ifdef SKYRIMVR
#pragma warning(default:4100)
#endif

void GetNPCAppearanceData(ExtraInfoEntry* resultArray, RE::TESNPC* npc)
{
	//apperance - currently height and weight
	logger::debug("GetNPCAppearanceData Started");

	ExtraInfoEntry* appearance;
	CreateExtraInfoEntry(appearance, GetTranslation("$Appearance"), "", priority_Actor_Appearance);

	float weight = npc->weight;

	ExtraInfoEntry* weightEntry;

	CreateExtraInfoEntry(weightEntry, GetTranslation("$NPCWeight"), FloatToString(weight), priority_Actor_Appearance_Weight);
	appearance->PushBack(weightEntry);

	float height = npc->height;

	ExtraInfoEntry* heightEntry;

	CreateExtraInfoEntry(heightEntry, GetTranslation("$Height"), FloatToString(height), priority_Actor_Appearance_Height);
	appearance->PushBack(heightEntry);

	resultArray->PushBack(appearance);

	logger::debug("GetNPCAppearanceData Ended");
}

void GetFactionsForNPC(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESActorBase* actorBase)
{
	logger::debug("GetFactionsForNPC start");

	ExtraInfoEntry* factionsEntry;

	CreateExtraInfoEntry(factionsEntry, GetTranslation("$Factions"), "", priority_Actor_Factions_Faction);

	//Check base factions
	int numFactionsBase = actorBase->factions.size();

	if (numFactionsBase > 0) {
		for (int i = 0; i < numFactionsBase; i++) {
			RE::FACTION_RANK factionInfo = actorBase->factions[i];
			RE::TESFaction* faction = factionInfo.faction;

			if (faction) {
				ExtraInfoEntry* factionEntry;

				std::string factionName = GetName(faction);
				int rank = factionInfo.rank;

				CreateExtraInfoEntry(factionEntry, factionName, GetTranslation("$FactionRank") + IntToString(rank), priority_Actor_Factions_Faction);

				GetFormData(factionEntry, faction, nullptr);

				factionsEntry->PushBack(factionEntry);
			}
		}
	}

	if (actor) {
		RE::ExtraFactionChanges* factionChanges = actor->extraList.GetByType<RE::ExtraFactionChanges>();

		if (factionChanges) {
			int numFactionsChanged = factionChanges->factionChanges.size();

			if (numFactionsChanged > 0) {
				for (int i = 0; i < numFactionsChanged; i++) {
					RE::FACTION_RANK factionInfo = factionChanges->factionChanges[i];
					RE::TESFaction* faction = factionInfo.faction;

					if (faction) {
						ExtraInfoEntry* factionEntry;

						std::string factionName = GetName(faction);
						int rank = factionInfo.rank;

						CreateExtraInfoEntry(factionEntry, factionName, GetTranslation("$FactionRank") + IntToString(rank), priority_Actor_Factions_Faction);

						GetFormData(factionEntry, faction, nullptr);

						factionsEntry->PushBack(factionEntry);
					}
				}
			}
		}
	}

	resultArray->PushBack(factionsEntry);
}

void GetMFGInformation(ExtraInfoEntry* expressionsRoot, ExtraInfoEntry* modifierRoot, ExtraInfoEntry* phenomeRoot, RE::Actor* actor)
{
	RE::BSFaceGenAnimationData* bsFaceGenAnimationData = actor->GetFaceGenAnimationData();

	if (bsFaceGenAnimationData) {
		RE::BSFaceGenKeyframeMultiple* expressions = &bsFaceGenAnimationData->expressionKeyFrame;

		for (int i = 0; i < numberOfMFGExpressions; i++) {
			ExtraInfoEntry* expressionEntry;
			std::string expressionName = GetTranslation("$mfgExpression") + IntToString(i) + " " + GetMFGExpressionName(i);
			std::string value = FloatToString(expressions->values[i] * 100);

			CreateExtraInfoEntry(expressionEntry, expressionName, value, priority_MFG_Expression);
			expressionsRoot->PushBack(expressionEntry);
		}

		RE::BSFaceGenKeyframeMultiple* modifiers = &bsFaceGenAnimationData->modifierKeyFrame;

		for (int i = 0; i < numberOfMFGModifiers; i++) {
			ExtraInfoEntry* modifierEntry;
			std::string modifierName = GetTranslation("$mfgModifier") + IntToString(i) + " " + GetMFGModiferName(i);
			std::string value = FloatToString(modifiers->values[i] * 100);

			CreateExtraInfoEntry(modifierEntry, modifierName, value, priority_MFG_Modifier);
			modifierRoot->PushBack(modifierEntry);
		}

		RE::BSFaceGenKeyframeMultiple* phenomes = &bsFaceGenAnimationData->phenomeKeyFrame;

		for (int i = 0; i < numberOfMFGPhenomes; i++) {
			ExtraInfoEntry* phenomeEntry;
			std::string modifierName = GetTranslation("$mfgPhoneme") + IntToString(i) + " " + GetMFGPhenomeName(i);
			std::string value = FloatToString(phenomes->values[i] * 100);

			CreateExtraInfoEntry(phenomeEntry, modifierName, value, priority_MFG_Phenome);
			phenomeRoot->PushBack(phenomeEntry);
		}
	}
}
