#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"


const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminaIndex = 26;
const int playerBaseFormID = 0x7;
const int totalNumberOfActorValues = (int)RE::ActorValue::kTotal;

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm); //Give a npc form return the root form used as a template by that form. Mostly used to get from FF indexed forms to the form in a mod they are taking their data from
void GetCharacterData(ExtraInfoEntry* resultArray, RE::TESForm* refForm, RE::TESForm* baseForm); //Get the general data 
void GetSpellsForNPC(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESActorBase* actorBase); //Get all spells for a npc, both those assigned in their base form, and those added later in game
void GetActorData(ExtraInfoEntry* resultArray, RE::Actor* actor); //Get data that is specific to actors current state instead of coming from their base form
void GetActorValue(ExtraInfoEntry*& resultArray, RE::Actor* actor, int id, priority actorValuePriority); //Get the value for a specific actor value
void GetLevelData(ExtraInfoEntry*& resultArray, RE::Actor* actor, RE::TESNPC* npc); //Get data on a npcs level and possible leveled multiple
void GetPerksForNPC(ExtraInfoEntry*& resultArray, RE::TESActorBase* actorBase, RE::PlayerCharacter* player); //Get all perks the npc has
void GetNPCAppearanceData(ExtraInfoEntry*& resultArray, RE::TESNPC* npc); //Get data related to the npcs appearance
void GetFactionsForNPC(ExtraInfoEntry*& resultArray, RE::Actor* actor, RE::TESActorBase* actorBase);