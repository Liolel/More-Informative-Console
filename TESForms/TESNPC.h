#pragma once
#include "RE/Skyrim.h"
#include "MoreInformativeConsole/ExtraInfoEntry.h"


const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminaIndex = 26;
const int playerBaseFormID = 0x7;

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm); //Give a npc form return the root form used as a template by that form. Mostly used to get from FF indexed forms to the form in a mod they are taking their data from
void GetCharacterData(ExtraInfoEntry* resultArray, RE::TESForm* refForm, RE::TESForm* baseForm); //Get the general data 
void GetSpellsForNPC(ExtraInfoEntry* resultArray, RE::Actor* actor, RE::TESActorBase* actorBase); //Get all spells for a npc, both those assigned in their base form, and those added later in game
void GetActorData(ExtraInfoEntry* resultArray, RE::Actor* actor); //Get data that is specific to actors current state instead of coming from their base form