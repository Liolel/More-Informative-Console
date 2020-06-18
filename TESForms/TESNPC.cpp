#include "TESNPC.h"

//Adapted from SKSE source code

RE::TESBoundObject* GetRootTemplate(RE::TESForm* baseForm)
{
	RE::TESNPC* npc = static_cast<RE::TESNPC*>(baseForm);
	RE::TESNPC* rootNPC = npc->GetRootFaceNPC();

	return rootNPC;
}