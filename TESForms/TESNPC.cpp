#include "TESNPC.h"
#include "MoreInformativeConsole/globals.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "TESForm.h"
#include "MagicItem.h"

//Adapted from SKSE source code

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm)
{
	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	RE::TESNPC* rootNPC = npc->GetRootFaceNPC();

	return rootNPC;
}


void GetCharacterData(ExtraInfoEntry* resultArray, RE::TESForm* refForm, RE::TESForm* baseForm)
{
	_DMESSAGE("GetCharacterData: GetCharacter info start");

	//Get all of the various objects we need
	RE::Actor* actor = nullptr;

	if (refForm)
	{
		actor = static_cast<RE::Actor*>(refForm);
	}

	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	RE::PlayerCharacter* player = nullptr;

	if (baseForm->formID == playerBaseFormID)
	{
		player = RE::PlayerCharacter::GetSingleton();
	}

	RE::TESActorBase* actorBase = static_cast<RE::TESActorBase*>(npc);

	if (npc)
	{
		if (actorBase)
		{
			_DMESSAGE("GetCharacterData: GetCharacter info casts worked");

			_DMESSAGE("GetCharacterData: Starting Race");
			
			//Handle Race
			ExtraInfoEntry* raceEntry;

			RE::TESRace* pRace = npc->race;

			//Save the race so we can filter ARMA by the race later
			MICGlobals::filterARMAByRace = pRace;

			std::string raceName = GetName(pRace);

			CreateExtraInfoEntry(raceEntry, "Race", raceName, priority_Actor_Race);

			GetFormData(raceEntry, pRace, nullptr);

			resultArray->PushBack(raceEntry);

			_DMESSAGE("GetCharacterData: Ending Race");
			
			//Handle Spells
			GetSpellsForNPC(resultArray, actor, actorBase);

			//If we are looking at an actor there is additional data we can get specific to that fact
			if (actor)
			{
				GetActorData(resultArray, actor);
			}
		/*
			  //Handle Flags
			int essentialFlag = 0x02;
			int protectedFlag = 0x800;

			ExtraInfoEntry * protectionEntry;


			if ((pNPC->actorData.flags & essentialFlag) == essentialFlag)
			{
				CreateExtraInfoEntry(protectionEntry, "Protection", "Essential");
			}

			else if ((pNPC->actorData.flags & protectedFlag) == protectedFlag)
			{
				CreateExtraInfoEntry(protectionEntry, "Protection", "Protected");
			}

			else
			{
				CreateExtraInfoEntry(protectionEntry, "Protection", "None");
			}

			resultArray->PushBack(protectionEntry);

			//Level stuff

			if (pActor)
			{

				int level = CALL_MEMBER_FN(pActor, GetLevel)();

				ExtraInfoEntry * levelEntry;

				CreateExtraInfoEntry(levelEntry, "Level", IntToString(level));
				resultArray->PushBack(levelEntry);

				DebugMessage("GetExtraData: GetCharacter level gotten");
			}

			ExtraInfoEntry * isPcLeveledEntry;

			bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
			if (isLevelMult)
			{
				DebugMessage("GetExtraData: GetCharacter pc level mult set");


				CreateExtraInfoEntry(isPcLeveledEntry, "Is PC Level Mult", "True");

				double levelMult = (double)pNPC->actorData.level / 1000.0;
				int minLevel = pNPC->actorData.minLevel;
				int maxLevel = pNPC->actorData.maxLevel;

				ExtraInfoEntry * levelMultEntry, * minLevelEntry, * maxLevelEntry;

				CreateExtraInfoEntry(levelMultEntry, "Level Mult", DoubleToString(levelMult));
				isPcLeveledEntry->PushBack(levelMultEntry);

				CreateExtraInfoEntry(minLevelEntry, "Min level", IntToString(minLevel));
				isPcLeveledEntry->PushBack(minLevelEntry);

				CreateExtraInfoEntry(maxLevelEntry, "Max Level", IntToString(maxLevel));
				isPcLeveledEntry->PushBack(maxLevelEntry);
			}
			else
			{
				DebugMessage("GetExtraData: GetCharacter pc level mult not set");

				CreateExtraInfoEntry(isPcLeveledEntry, "Is PC Level Mult", "False");
			}

			resultArray->PushBack(isPcLeveledEntry);

			//Perks
			int numPerks = pActorBase->perkRanks.numPerkRanks;

			DebugMessage("GetExtraData: Starting Perks - Total Number" + IntToString(numPerks));

			ExtraInfoEntry * perks;
			CreateExtraInfoEntry(perks, "Perks", "");

			for (int i = 0; i < numPerks; i++)
			{
				DebugMessage("GetExtraData: Starting Perk num" + IntToString(i));

				BGSPerk *perk = pActorBase->perkRanks.perkRanks[i].perk;

				if (perk)
				{
					//int rank = pActorBase->perkRanks.perkRanks[i].rank;

					std::string name = GetName(perk);

					ExtraInfoEntry * perkEntry;

					CreateExtraInfoEntry(perkEntry, name, "");

					GetFormData(perkEntry, perk, nullptr);

					//ExtraInfoEntry * perkEntryRank;

					//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank) );
					//perkEntrySubArray->PushBack(perkEntryRank);

					perks->PushBack(perkEntry);
				}
			}

			if (pPlayer != nullptr)
			{
				int numPlayerPerks = pPlayer->addedPerks.count;

				DebugMessage("GetExtraData: Starting Player Perks - Total Number" + IntToString(numPlayerPerks));

				for (int i = 0; i < numPlayerPerks; i++)
				{
					DebugMessage("GetExtraData: Starting Player Perk num" + IntToString(i));

					BGSPerk *perk = pPlayer->addedPerks[i]->perk;

					if (perk)
					{
						//int rank = pPlayer->addedPerks[i]->rank;

						std::string name = GetName(perk);

						ExtraInfoEntry * perkEntry;

						CreateExtraInfoEntry(perkEntry, name, "");

						GetFormData(perkEntry, perk, nullptr);

						//ExtraInfoEntry * perkEntryRank;

						//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank));
						//perkEntrySubArray->PushBack(perkEntryRank);

						perks->PushBack(perkEntry);
					}
				}
			}

			resultArray->PushBack(perks);

			DebugMessage("GetExtraData: Done with perks");



			//apperance - currently height and weight
			DebugMessage("GetExtraData: appearance Started");

			ExtraInfoEntry * appearance;
			CreateExtraInfoEntry(appearance, "Appearance", "");

			float weight = pNPC->weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", FloatToString(weight));
			appearance->PushBack(weightEntry);

			float height = pNPC->height;

			ExtraInfoEntry * heightEntry;

			CreateExtraInfoEntry(heightEntry, "Height", FloatToString(height));
			appearance->PushBack(heightEntry);

			resultArray->PushBack(appearance);

			DebugMessage("GetExtraData: appearance Ended");

			DebugMessage("GetExtraData: factions start");

			ExtraInfoEntry * factionsEntry;

			CreateExtraInfoEntry(factionsEntry, "Factions", "");

			//Factions
			int numFactions = pActorBase->actorData.factions.count;

			if (numFactions > 0)
			{
				for (int i = 0; i < numFactions; i++)
				{
					TESActorBaseData::FactionInfo factionInfo = pActorBase->actorData.factions[i];
					TESFaction *faction = factionInfo.faction;

					if (faction)
					{
						ExtraInfoEntry * factionEntry;

						std::string factionName = GetName(faction);
						int rank = factionInfo.rank;

						CreateExtraInfoEntry(factionEntry, factionName, "Rank: " + IntToString(rank));

						GetFormData(factionEntry, faction, nullptr);

						factionsEntry->PushBack(factionEntry);
					}
				}
			}

			resultArray->PushBack(factionsEntry);

			//need to get factions off of reference as well

			DebugMessage("GetExtraData: factions ended");
			*/
		}
	}


	/*PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
	if (pPC)
	{
	RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
	}*/
	
	_DMESSAGE("GetExtraData: GetCharacter End");
}

void GetSpellsForNPC(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESActorBase* actorBase )
{
	//Spells
	ExtraInfoEntry* allSpellsEntry;

	CreateExtraInfoEntry(allSpellsEntry, "Spells", "", priority_Actor_Spells);

	_DMESSAGE("GetSpellsForNPC: Starting Added Spells");

	if (actor)
	{
		int numberOfAddedSpells = actor->addedSpells.size();

		//Added Spells
		for (int i = 0; i < numberOfAddedSpells; i++)
		{
			ExtraInfoEntry* spellEntry;

			RE::SpellItem* spell = actor->addedSpells[i];
			std::string spellName = GetName(spell);

			CreateExtraInfoEntry(spellEntry, spellName, "Added Spell", priority_Actor_Spells_AddedSpell);

			GetFormData(spellEntry, spell, nullptr);

			allSpellsEntry->PushBack(spellEntry);
		}
	}

	_DMESSAGE("GetSpellsForNPC: Starting Base Spells");

	if (actorBase->actorEffects)
	{
		//Actor Base Spells
		int numberOfBaseSpells = actorBase->actorEffects->numSpells;

		for (int i = 0; i < numberOfBaseSpells; i++)
		{
			ExtraInfoEntry* spellEntry;

			RE::SpellItem* spell = actorBase->actorEffects->spells[i];
			std::string spellName = GetName(spell);

			CreateExtraInfoEntry(spellEntry, spellName, "Base Spell", priority_Actor_Spells_BaseSpell);

			GetFormData(spellEntry, spell, nullptr);

			allSpellsEntry->PushBack(spellEntry);
		}
	}

	resultArray->PushBack(allSpellsEntry);

	_DMESSAGE("GetCharacterData:  Done with spells");
}

void GetActorData(ExtraInfoEntry* resultArray, RE::Actor* actor )
{
	//Create a subarray to hold all the active effects for an actor
	ExtraInfoEntry* activeEffectsEntry;

	CreateExtraInfoEntry(activeEffectsEntry, "Effects", "", priority_Actor_Effects);

	RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = actor->GetActiveEffectList();

	_DMESSAGE("GetCharacterData: Active Effects Gotten");

	if (activeEffects)
	{
		for (RE::BSSimpleList<RE::ActiveEffect*>::iterator itr = activeEffects->begin(); itr != activeEffects->end(); (void)itr++)
		{
			_DMESSAGE("GetCharacterData: Starting Active Effect");

			RE::ActiveEffect* activeEffect = *(itr);

			ExtraInfoEntry* effectEntry;

			if (activeEffect && activeEffect->effect)
			{
				_DMESSAGE("GetCharacterData: Active Effect MGEF found");

				std::string effectActive;

				RE::Effect* effect = activeEffect->effect;

				if (HasFlag((UInt32)activeEffect->flags, (UInt32)RE::ActiveEffect::Flag::kInactive))
				{
					effectActive = "Inactive";
				}
				else
				{
					effectActive = "Active";
				}

				GetEffectData(activeEffectsEntry, effect, effectActive);
				/*
				CreateExtraInfoEntry(effectEntry, effectName, effectActive);

				TESForm* effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

				if (effectBaseForm)
				{
					DebugMessage("GetCharacterData: Active Effect MGEF base form found");

					GetCommonFormData(effectEntry, effectBaseForm, nullptr);

					//Magnitude
					ExtraInfoEntry* magnitudeEntry;

					float magnitude = pEffect->magnitude;
					CreateExtraInfoEntry(magnitudeEntry, "Magnitude", FloatToString(magnitude));
					effectEntry->PushBack(magnitudeEntry);

					//Duration
					ExtraInfoEntry* durationEntry;

					float duration = pEffect->duration;
					CreateExtraInfoEntry(durationEntry, "Duration", FloatToString(duration));
					effectEntry->PushBack(durationEntry);


					//Magnitude
					ExtraInfoEntry* elapsedEntry;

					float elapsed = pEffect->elapsed;
					CreateExtraInfoEntry(elapsedEntry, "Elapsed", FloatToString(elapsed));
					effectEntry->PushBack(elapsedEntry);

					etMagicEffectData(effectEntry, effectBaseForm);
				} */
			}

			//This is only reached if there is an active effect without a actual corrosponding effect. Probally impossible but here's some code to handle it just in case
			else
			{
				CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "", priority_MagicItem_Effect);
				activeEffectsEntry->PushBack(effectEntry);
			}

			_DMESSAGE("GetCharacterData: Ending Active Effect");

		}
	}

	resultArray->PushBack(activeEffectsEntry);

	_DMESSAGE("GetActorData: Active Effects Done");
	
	//Add Health/Magicka/Stamina to the main subarray
	GetActorValue(resultArray, actor, actorValueHealthIndex, priority_Actor_Health);
	GetActorValue(resultArray, actor, actorValueMagickaIndex, priority_Actor_Magicka);
	GetActorValue(resultArray, actor, actorValueStaminaIndex, priority_Actor_Stamina);

	//Get all actor values in a subarray
	ExtraInfoEntry * actorValueArray;
	CreateExtraInfoEntry(actorValueArray, "Actor Values", "", priority_Actor_ActorValues);

	for (int i = 0; i < totalNumberOfActorValues; i++)
	{
		GetActorValue(actorValueArray, actor, i, priority_Actor_ActorValues_ActorValue);
	}

	resultArray->PushBack(actorValueArray);

	_DMESSAGE("GetActorData: actor values gotten");
	/*
	DebugMessage("Before package");

	ActorProcessManager * pProcess = pActor->processManager;

	if (pProcess)
	{
		TESForm * currentPackage = pProcess->unk18.package;

		if (currentPackage)
		{
			DebugMessage("Before getting package name");


			//TESForm * currentPackage = packageData->currentPackage;
			std::string packageName = GetName(currentPackage);

			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry * packageEntry;

			CreateExtraInfoEntry(packageEntry, "Current Package", packageName);


			DebugMessage("Before getting package form data");

			GetFormData(packageEntry, currentPackage, nullptr);

			resultArray->PushBack(packageEntry);

		}
	}

	DebugMessage("After package");
	*/
}

void GetActorValue(ExtraInfoEntry*& resultArray, RE::Actor* actor, int id, priority actorValuePriority)
{
	_DMESSAGE("GetExtraData: GetActover Value Start");

	RE::ActorValue actorValue = (RE::ActorValue)id;
	ExtraInfoEntry* actorValueEntry;

	if (id < totalNumberOfActorValues)
	{
		std::string valueName = GetActorValueName(id);
		float baseValue = actor->GetBaseActorValue(actorValue);
		float currentValue = actor->GetActorValue(actorValue);
		float maxValue = actor->GetPermanentActorValue(actorValue);

		CreateExtraInfoEntry(actorValueEntry, valueName, FloatToString(currentValue), actorValuePriority);

		//create a subarray for the base  current and maximum

		ExtraInfoEntry* baseValueEntry, * currentValueEntry, * maxValueEntry;

		CreateExtraInfoEntry(baseValueEntry, "Base", FloatToString(baseValue), priority_ActorValue_Base);
		CreateExtraInfoEntry(currentValueEntry, "Current", FloatToString(currentValue), priority_ActorValue_Current);
		CreateExtraInfoEntry(maxValueEntry, "Max", FloatToString(maxValue), priority_ActorValue_Max);

		actorValueEntry->PushBack(baseValueEntry);
		actorValueEntry->PushBack(currentValueEntry);
		actorValueEntry->PushBack(maxValueEntry);

		resultArray->PushBack(actorValueEntry);
	}

	_DMESSAGE("GetExtraData: GetActover Value End");
}