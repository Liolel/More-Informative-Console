#include "TESNPC.h"
#include "MoreInformativeConsole/globals.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "TESForm.h"

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

			CreateExtraInfoEntry(raceEntry, "Race", raceName);

			GetFormData(raceEntry, pRace, nullptr);

			resultArray->PushBack(raceEntry);

			_DMESSAGE("GetCharacterData: Ending Race");
			/*
			//Spells
			ExtraInfoEntry* allSpellsEntry;

			CreateExtraInfoEntry(allSpellsEntry, "Spells", "");

			DebugMessage("GetCharacterData: Starting Added Spells");

			if (pActor)
			{
				//Added Spells
				for (int i = 0; i < pActor->addedSpells.Length(); i++)
				{
					ExtraInfoEntry* spellEntry;

					SpellItem* spell = pActor->addedSpells.Get(i);
					GetSpellDataWrapper(spellEntry, spell, "Added Spell");

					allSpellsEntry->PushBack(spellEntry);
				}
			}

			DebugMessage("GetCharacterData: Starting Base Spells");

			//Actor Base Spells
			int numberOfBaseSpells = pActorBase->spellList.GetSpellCount();

			for (int i = 0; i < numberOfBaseSpells; i++)
			{
				ExtraInfoEntry* spellEntry;

				SpellItem* spell = pActorBase->spellList.GetNthSpell(i);
				GetSpellDataWrapper(spellEntry, spell, "Base Spell");

				allSpellsEntry->PushBack(spellEntry);
			}

			resultArray->PushBack(allSpellsEntry);

			DebugMessage("GetCharacterData: GetCharacter Done with spells");

			if (pActor)
			{
				// ActiveEffects as Array
				ExtraInfoEntry* activeEffectsEntry;

				CreateExtraInfoEntry(activeEffectsEntry, "Effects", "");

				tList<ActiveEffect>* effects = pActor->magicTarget.GetActiveEffects();

				DebugMessage("GetCharacterData: Active Effects Gotten");

				if (effects)
				{
					for (int i = 0; i < effects->Count(); i++)
					{
						DebugMessage("GetCharacterData: Starting Active Effect");

						ActiveEffect* pEffect = effects->GetNthItem(i);

						ExtraInfoEntry* effectEntry;

						if (pEffect->effect && pEffect->effect->mgef)
						{
							DebugMessage("GetCharacterData: Active Effect MGEF found");

							std::string effectName, effectActive;

							EffectSetting* mgef = pEffect->effect->mgef;

							if (mgef->fullName.name.data)
							{
								effectName = mgef->fullName.name.data;
							}

							else
							{
								effectName = "Unknown Effect";
							}

							if ((pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive)
							{
								effectActive = "Inactive";
							}

							else
							{
								effectActive = "Active";
							}

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

								GetMagicEffectData(effectEntry, effectBaseForm);
							}
						}

						else
						{
							CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "");
						}

						activeEffectsEntry->PushBack(effectEntry);


						DebugMessage("GetCharacterData: Ending Active Effect");

						/*if (pEffect->item)
						scaleformExtend::MagicItemData(effectView, pEffect->item, bRecursive ? bExtra : false, bRecursive); ??? */

						//RegisterBool(effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);

						// ActiveEffect
						//if (pEffect->effect && pEffect->effect->mgef)
						//	scaleformExtend::MagicItemData(effectView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);

						//activeEffects->PushBack(effect);
						/*
					}
				}

				resultArray->PushBack(activeEffectsEntry);

				DebugMessage("GetExtraData: Active Effects Done");

				ExtraInfoEntry * actorValueHealth;

				GetActorValue(actorValueHealth, pActor, actorValueHealthIndex);
				resultArray->PushBack(actorValueHealth);

				ExtraInfoEntry * actorValueMagicka;

				GetActorValue(actorValueMagicka, pActor, actorValueMagickaIndex);
				resultArray->PushBack(actorValueMagicka);

				ExtraInfoEntry * actorValueStamina;

				GetActorValue(actorValueStamina, pActor, actorValueStaminahIndex);
				resultArray->PushBack(actorValueStamina);

				//Get all actor values in a subarray
				ExtraInfoEntry * actorValueArray;
				CreateExtraInfoEntry(actorValueArray, "Actor Values", "");


				for (int i = 0; i < ActorValueList::kNumActorValues; i++)
				{

					ExtraInfoEntry * actorValue;
					GetActorValue(actorValue, pActor, i);
					actorValueArray->PushBack(actorValue);
				}

				resultArray->PushBack(actorValueArray);

				DebugMessage("GetExtraData: GetCharacter actor values gotten");

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

			} //end of a pActor Section

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