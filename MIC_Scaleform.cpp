#include "MIC_Scaleform.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "PapyrusActor.cpp"
#include "Util.h"
#include "GameMenus.h"
#include <memory>
#include <vector>
#include <skse64\GameData.h>
#include <skse64_common\skse_version.h>
#include "PapyrusActorValueInfo.h"
#include <Windows.h>

const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminahIndex = 26;
const int playerBaseFormID = 0x7;
const char deliminator = '\\';

namespace MICGlobals
{
	boolean readRaceSkins = true; //Enable/Disabling reading the skin entires for races. Used to prevent an infinite loop of reading Race->Armor-Arma-Race
	TESRace* filterARMAByRace = nullptr; //Used to filter ARMA forms for armors to only show ARMAs that can be used by a specific race.
	boolean reducedMode = false; //Used to reduce the amount of info read to prevent crashes on opening chests with very large number of armors in certain circumstancesv
	int maxInventoryBeforeReducedMode = 750; //Maximum inventory size before triggering reduced mode
	ExtraInfoEntry rootEntry("", "");
}

class MICScaleform_GetReferenceInfo : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{

		std::string refFormFromMod, refFormLastChangedBy;
		int numModsModifyingRef;

		DebugMessage("GetReferenceInfo: Called");
		//ASSERT(args->numArgs >= 1);

#if SKSE_VERSION_INTEGER_BETA <= 12
		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
#else
		UInt32 handle = (*g_consoleHandle);

		NiPointer<TESObjectREFR> pRef;
		LookupREFRByHandle(handle, pRef);
#endif

		
		if (pRef != nullptr)
		{
			DebugMessage("GetReferenceInfo: pRef passed");
			args->movie->CreateObject(args->result);
			std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				DebugMessage("GetReferenceInfo: pBase passed");
				sprintf_s(sResult.get(), MAX_PATH, "%08X", pRef->formID);
				RegisterString(args->result, args->movie, "refFormID", sResult.get());
				RegisterString(args->result, args->movie, "refFormType", GetFormTypeName(pRef->formType).c_str());

				const char* pRefName = CALL_MEMBER_FN(pRef, GetReferenceName)();
				RegisterString(args->result, args->movie, "referenceName", pRefName);
				FormModInfoData* refFormModInfo = (FormModInfoData*)pRef->unk08;

				if (refFormModInfo != nullptr)
				{
					DebugMessage("GetReferenceInfo: Inside Ref modInfo");
					numModsModifyingRef = refFormModInfo->size;

					refFormFromMod = refFormModInfo->entries[0]->name;

					//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
					if ( ( (pRef->formID & 0xFF000000 ) == 0)
						&& refFormFromMod != "Skyrim.esm")
					{
						refFormFromMod = "Skyrim.esm";
					}

					refFormLastChangedBy = refFormModInfo->entries[numModsModifyingRef - 1]->name;

					RegisterString(args->result, args->movie, "refFormDefinedIn", refFormFromMod.c_str());
					RegisterString(args->result, args->movie, "refFormLastChangedBy", refFormLastChangedBy.c_str());


				}

				DebugMessage("GetReferenceInfo: Before FF actor if");
				if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
				{
					DebugMessage("GetReferenceInfo: Inside FF actor if");
					TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

					if (pBaseActor)
					{
						DebugMessage("GetReferenceInfo: Inside second FF actor if");
						TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

						if (pBaseActorTemplate)
						{
							pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
						}
					}
				}

				DebugMessage("GetReferenceInfo: In non FF actor base form");
				sprintf_s(sResult.get(), MAX_PATH, "%08X", pBaseForm->formID);
				RegisterString(args->result, args->movie, "baseFormID", sResult.get());
				RegisterString(args->result, args->movie, "baseFormType", GetFormTypeName(pBaseForm->formType).c_str());



				DebugMessage("GetReferenceInfo: Before base modInfo");
				FormModInfoData* baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

				if (baseFormModInfo != nullptr)
				{

					DebugMessage("GetReferenceInfo: Inside Base modInfo");
					int numModsModifyingBase = baseFormModInfo->size;

					std::string baseFormFromMod = baseFormModInfo->entries[0]->name;
					std::string baseFormLastChangedBy = baseFormModInfo->entries[numModsModifyingBase - 1]->name;

					RegisterString(args->result, args->movie, "baseFormDefinedIn", baseFormFromMod.c_str());
					RegisterString(args->result, args->movie, "baseFormLastChangedBy", baseFormLastChangedBy.c_str());


					std::string modsModifying = "GetReferenceInfo: # Mods " + std::to_string(numModsModifyingBase) + " base # mods" + std::to_string(numModsModifyingRef) + " reference ";
					std::string baseModifiedBy = "GetReferenceInfo: Base From " + baseFormFromMod + " last changed by" + baseFormLastChangedBy;
					std::string refModifiedBy = "GetReferenceInfo: Ref From " + refFormFromMod + " last changed by" + refFormLastChangedBy;


					DebugMessage(modsModifying.c_str());
					DebugMessage(baseModifiedBy.c_str());
					DebugMessage(refModifiedBy.c_str());

				}
				DebugMessage("GetReferenceInfo: After FF actor if");

			}
		}

		DebugMessage("GetReferenceInfo: Method End");
	}

};

class MICScaleform_GetExtraData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{

		DebugMessage("GetExtraData: Invoke Start");

		GFxMovieView * movie = args->movie;

		GFxValue result;

		movie->CreateArray(&result);

#if SKSE_VERSION_INTEGER_BETA <= 12
		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
#else
		UInt32 handle = (*g_consoleHandle);

		NiPointer<TESObjectREFR> pRef;
		LookupREFRByHandle(handle, pRef);
#endif
		if (pRef != nullptr)
		{

			DebugMessage("GetExtraData: pRefFound");

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				DebugMessage("GetExtraData: BaseFound");

				MICGlobals::rootEntry.Clear();

				GetFormData(&MICGlobals::rootEntry, pBaseForm, pRef);

				DebugMessage("Get Form Information done");

				//GFxValue * FormLocationData;

				//result.GetElement(4, FormLocationData);

				//Send the data we created back to the console
				GFxValue returnValue;
				//MICGlobals::rootEntry.CreatePrimaryScaleformArray(&returnValue, movie);
				GFxValue arguments;

				//UIStringHolder* stringHolder = UIStringHolder::GetSingleton();
				//MenuManager* mm = MenuManager::GetSingleton();

				//IMenu* consoleMenu = mm->GetMenu(&stringHolder->console);

				//consoleMenu->view->Invoke("_root.consoleFader_mc.Console_mc.StartExtraInfoData", &returnValue, &arguments, 0);

				//TraverseArray(&result, consoleMenu->view, 0, -1, -1,-1, -1);

				//int arraySize;
	
				//DebugMessage(IntToString(result.GetArraySize()));

				//consoleMenu->view->Invoke("_root.consoleFader_mc.Console_mc.FinishExtraInfoData", &returnValue, &arguments, 0);
			}
		}

		DebugMessage("GetExtraData: Invoke End");
	}

	//general wrapper for all get form methods
	void GetFormData(ExtraInfoEntry * resultArray, TESForm* pBaseForm, TESObjectREFR* pRefForm)
	{
		DebugMessage("GetExtraData: Get Form Data Start " + GetFormTypeName(pBaseForm->formType) + " " + FormIDToString(pBaseForm->formID));

		GetCommonFormData(resultArray, pBaseForm, pRefForm);

		if (pBaseForm != nullptr
			&& pBaseForm->GetFormType() == kFormType_NPC
			&& (pRefForm == nullptr
				|| pRefForm->GetFormType() == kFormType_Character))
		{
			DebugMessage("GetExtraData: Get Form Data character found");
			GetCharacterData(resultArray, pRefForm, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_EffectSetting)
		{
			DebugMessage("GetExtraData: Get Form Data magic effect found");
			GetMagicEffectData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Spell)
		{
			DebugMessage("GetExtraData: Get Form Data spell found");
			GetSpellData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Armor)
		{
			DebugMessage("GetExtraData: Get Form Data armor found");
			GetArmorData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Weapon)
		{
			DebugMessage("GetExtraData: Get Form Data Weapon found");
			GetWeaponData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Ammo)
		{
			DebugMessage("GetExtraData: Get Form Data Ammo found");
			GetAmmoData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Container)
		{
			DebugMessage("GetExtraData: Get Form Data Container found");
			GetContainerData(resultArray, pBaseForm);
		}
		else if (pBaseForm->GetFormType() == kFormType_Faction)
		{
			DebugMessage("GetExtraData: Get Form Data Faction found");
			//GetContainerData(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Race)
		{
			DebugMessage("GetExtraData: Get Form Data Race found");
			GetRaceEntry(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_TextureSet)
		{
			DebugMessage("GetExtraData: Get Form Data Texture Set found");
			GetTextureSet(resultArray, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_ARMA)
		{
			DebugMessage("GetExtraData: Get Form Data ARMA found");
			GetArmaData(resultArray, pBaseForm);
		}

		//get inventory
		if (pRefForm != nullptr
			&& pRefForm->extraData.HasType(kExtraData_ContainerChanges))
		{
			DebugMessage("GetFormData: Found Inventory Start");


			ExtraContainerChanges * inventoryExtraData = static_cast<ExtraContainerChanges*> (pRefForm->extraData.GetByType(kExtraData_ContainerChanges));
			EntryDataList * inventory = inventoryExtraData->data->objList;

			DebugMessage("GetFormData: Got Inventory");

			TESContainer * pContainer = nullptr;

			if (pBaseForm->GetFormType() == kFormType_NPC)
			{
				DebugMessage("GetFormData: Inventory npc");

				Actor* pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);

				if (pActor)
				{
					GetEquipment(resultArray, inventoryExtraData, pActor);
				}

				TESActorBase *pActorBase = DYNAMIC_CAST(pBaseForm, TESForm, TESActorBase);

				if (pActorBase)
				{
					pContainer = &pActorBase->container;
					DebugMessage("GetFormData: Inventory npc container gotten");

				}
			}

			if (pBaseForm->GetFormType() == kFormType_Container)
			{
				DebugMessage("GetFormData: Inventory container");

				TESObjectCONT *pContainerForm = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);

				if (pContainerForm)
				{
					DebugMessage("GetFormData: Inventory cpntainer container gotten");
					pContainer = &pContainerForm->container;
				}
			}

			GetInventory(resultArray, inventory, pContainer);

		}

		GetModelTextures(resultArray, pBaseForm);

		//Handle BSExtra data
		if (pRefForm != nullptr)
		{
			GetBSExtraData(resultArray, pRefForm);
		}

		//reset any filtering
		MICGlobals::filterARMAByRace = nullptr;

		DebugMessage("GetExtraData: Get Form Data End");
	}

	void CreateExtraInfoEntry(ExtraInfoEntry * & extraInfoEntry, std::string extraInfoName, std::string extraInfoContents)
	{
		extraInfoEntry = new ExtraInfoEntry(extraInfoName, extraInfoContents);
	}
	/*
	void CreateExtraInfoEntry(ExtraInfoEntry * valueArray,  std::string extraInfoName, std::string extraInfoContents)
	{
	movie->CreateArray(valueArray);

	ExtraInfoEntry * GFxExtraInfoName, GFxExtraInfoContents;

	GFxExtraInfoName.SetString(extraInfoName.c_str());
	GFxExtraInfoContents.SetString(extraInfoContents.c_str());

	valueArray->PushBack(GFxExtraInfoName);
	valueArray->PushBack(GFxExtraInfoContents);

	}*/

	//get data common to all form types
	void GetCommonFormData(ExtraInfoEntry * resultArray, TESForm* pBaseForm, TESForm* pRefForm)
	{
		DebugMessage("GetCommonFormData: GetCommonFormData Start");

		std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);

		if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
		{
			DebugMessage("GetCommonFormData: Inside FF actor if");
			TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

			if (pBaseActor)
			{
				DebugMessage("GetCommonFormData: Inside second FF actor if");
				TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

				if (pBaseActorTemplate)
				{
					pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
				}
			}
		}

		std::string name = "";

		//The getName function returns the editor ID for race as that is more useful in identifying the actual race
		//but we want to display the real name when we are getting data for the form
		if (pBaseForm->formType == kFormType_Race)
		{
			TESRace * pRace = DYNAMIC_CAST(pBaseForm, TESForm, TESRace);
			if (pRace)
			{
				if (pRace->fullName.name.data)
				{
					name = pRace->fullName.name.data;
				}
			}
		}

		else
		{
			//name
			name = GetName(pBaseForm);
		}

		ExtraInfoEntry * nameArray;
		CreateExtraInfoEntry(nameArray, "Name", name);

		resultArray->PushBack(nameArray);

		DebugMessage("GetCommonFormData: Get FormID Start");

		//base formid
		sprintf_s(sResult.get(), MAX_PATH, "%08X", pBaseForm->formID);
		std::string formID = sResult.get();

		ExtraInfoEntry *  formIDArray;
		CreateExtraInfoEntry(formIDArray, "Base form ID", formID);
		resultArray->PushBack(formIDArray);

		//base form type
		std::string baseFormType = GetFormTypeName(pBaseForm->GetFormType());

		ExtraInfoEntry *  formTypeEntry;
		CreateExtraInfoEntry(formTypeEntry, "Base Type", baseFormType);
		resultArray->PushBack(formTypeEntry);

		//ref formid
		if (pRefForm != nullptr)
		{
			sprintf_s(sResult.get(), MAX_PATH, "%08X", pRefForm->formID);
			std::string refFormID = sResult.get();

			ExtraInfoEntry *  formIDArray;
			CreateExtraInfoEntry(formIDArray, "Ref form ID", refFormID);
			resultArray->PushBack(formIDArray);
		}

		//mod location info


		DebugMessage("GetCommonFormData: Get Form Location Start");

		ExtraInfoEntry *formLocationHolder, *formLocationData;

		CreateExtraInfoEntry(formLocationHolder, "Form location information", "");
		GetFormLocationData(formLocationData, pBaseForm, pRefForm);
		formLocationHolder->PushBack(formLocationData);
		resultArray->PushBack(formLocationHolder);

		if (pRefForm != nullptr)
		{
			GetPositionData(resultArray, pRefForm);
		}

		DebugMessage("GetCommonFormData: GetCommonFormData End");

	}

	//get information related to where mods the form is found in
	void GetFormLocationData(ExtraInfoEntry * & resultArray, TESForm* pBaseForm, TESForm* pRefForm)
	{
		DebugMessage("GetExtraData: GetFormLocationData Start");

		CreateExtraInfoEntry(resultArray, "", "");

		//if the target is a npc that has been created dynamically we want info on the base form the npc is created from
		if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
		{
			DebugMessage("GetExtraData: GetFormLocationData Getting base form template");

			TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

			if (pBaseActor)
			{
				TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

				if (pBaseActorTemplate)
				{
					pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
					DebugMessage("GetExtraData: GetFormLocationData Got base form template");

				}
			}
		}

		DebugMessage("GetExtraData: GetFormLocationData After check for base form template");

		//this method may be called at a time we only have a base form, and in that case skip anything related to the reform
		if (pRefForm != nullptr)
		{
			DebugMessage("GetExtraData: GetFormLocationData inside pRef section");


			FormModInfoData *refFormModInfo = (FormModInfoData*)pRefForm->unk08;

			if (refFormModInfo != nullptr)
			{
				DebugMessage("GetExtraData: GetFormLocationData pRef mod info found");

				//Reference Form
				int numModsModifyingRef = refFormModInfo->size;

				std::string refFirstDefinedIn = refFormModInfo->entries[0]->name;

				boolean SkyrimESMNotDetectedBug = false;

				//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
				if (((pRefForm->formID & 0xFF000000) == 0)
					&& refFirstDefinedIn != "Skyrim.esm")
				{
					refFirstDefinedIn = "Skyrim.esm";
					SkyrimESMNotDetectedBug = true;
				}

				ExtraInfoEntry * referenceDefinedIn;
				CreateExtraInfoEntry(referenceDefinedIn, "Reference defined in", refFirstDefinedIn);

				resultArray->PushBack(referenceDefinedIn);

				ExtraInfoEntry * referenceLastChangedBy;
				CreateExtraInfoEntry(referenceLastChangedBy, "Reference last modified by", refFormModInfo->entries[numModsModifyingRef - 1]->name);

				resultArray->PushBack(referenceLastChangedBy);

				ExtraInfoEntry * allModsTouchingReferenceHolder;
				CreateExtraInfoEntry(allModsTouchingReferenceHolder, "Reference found in", "");

				ExtraInfoEntry * allModsTouchingReference;
				GetModInfoData(allModsTouchingReference, refFormModInfo, SkyrimESMNotDetectedBug);
				allModsTouchingReferenceHolder->PushBack(allModsTouchingReference);

				resultArray->PushBack(allModsTouchingReferenceHolder);
			}
		}
		//Base Form

		DebugMessage("GetExtraData: GetFormLocationData at pBaseSection section");

		FormModInfoData *baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

		if (baseFormModInfo != nullptr)
		{
			DebugMessage("GetExtraData: GetFormLocationData baseFormModInfo found");

			int numModsModifyingBase = baseFormModInfo->size;

			ExtraInfoEntry * baseDefinedIn;
			CreateExtraInfoEntry(baseDefinedIn, "Base defined in", baseFormModInfo->entries[0]->name);

			resultArray->PushBack(baseDefinedIn);

			ExtraInfoEntry * baseLastChangedBy;
			CreateExtraInfoEntry(baseLastChangedBy, "Base last modified by", baseFormModInfo->entries[numModsModifyingBase - 1]->name);

			resultArray->PushBack(baseLastChangedBy);

			ExtraInfoEntry * allModsTouchingBaseHolder;
			CreateExtraInfoEntry(allModsTouchingBaseHolder, "Base found in", "");

			ExtraInfoEntry * allModsTouchingBase;
			GetModInfoData(allModsTouchingBase, baseFormModInfo, false);
			allModsTouchingBaseHolder->PushBack(allModsTouchingBase);

			resultArray->PushBack(allModsTouchingBaseHolder);
		}

		DebugMessage("GetExtraData: GetFormLocationData End");
	}

	void GetModInfoData(ExtraInfoEntry * resultArray,  FormModInfoData* modInfoData, boolean SkyrimESMNotDetectedBug)
	{
		DebugMessage("GetExtraData: GetModInfoData start");

		CreateExtraInfoEntry(resultArray, "", "");

		int numMods = modInfoData->size;

		if (SkyrimESMNotDetectedBug)
		{
			ExtraInfoEntry * modEntry;

			CreateExtraInfoEntry(modEntry, "Mod", "Skyrim.esm");
			resultArray->PushBack(modEntry);
		}

		for (int i = 0; i < numMods; i++)
		{
			ExtraInfoEntry * modEntry;

			CreateExtraInfoEntry(modEntry, "Mod", modInfoData->entries[i]->name);
			resultArray->PushBack(modEntry);
		}

		DebugMessage("GetExtraData: GetModInfoData end");
	}

	void GetCharacterData(ExtraInfoEntry * resultArray,  TESForm* pRefForm, TESForm* pBaseForm)
	{
		DebugMessage("GetCharacterData: GetCharacter info start");

		Actor * pActor = nullptr;

		if (pRefForm)
		{
			pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);
		}

		TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);
		PlayerCharacter* pPlayer = nullptr;

		if (pBaseForm->formID == playerBaseFormID)
		{
			pPlayer = DYNAMIC_CAST(pRefForm, TESForm, PlayerCharacter);
		}

		TESActorBase *pActorBase = DYNAMIC_CAST(pNPC, TESNPC, TESActorBase);

		if (pNPC)
		{
			if (pActorBase)
			{
				DebugMessage("GetCharacterData: GetCharacter info casts worked");

				DebugMessage("GetCharacterData: Starting Race");

				//Race
				ExtraInfoEntry * raceEntry;

				TESRace * pRace = pNPC->race.race;

				MICGlobals::filterARMAByRace = pRace;

				std::string raceName = GetName(pRace);

				CreateExtraInfoEntry(raceEntry, "Race", raceName);

				ExtraInfoEntry * raceSubEntry;
				CreateExtraInfoEntry(raceSubEntry, "", "");
				GetFormData(raceSubEntry, pRace, nullptr);

				raceEntry->PushBack(raceSubEntry);
				resultArray->PushBack(raceEntry);

				DebugMessage("GetCharacterData: Ending Race");

				//Spells
				ExtraInfoEntry * allSpellsEntry;

				CreateExtraInfoEntry(allSpellsEntry, "Spells", "");

				ExtraInfoEntry * SpellsArray;
				CreateExtraInfoEntry(SpellsArray, "", "");

				DebugMessage("GetCharacterData: Starting Added Spells");

				if (pActor)
				{
					//Added Spells
					for (int i = 0; i < pActor->addedSpells.Length(); i++)
					{
						ExtraInfoEntry * spellEntry;

						SpellItem* spell = pActor->addedSpells.Get(i);
						GetSpellDataWrapper(spellEntry, spell, "Added Spell");

						SpellsArray->PushBack(spellEntry);
					}
				}

				DebugMessage("GetCharacterData: Starting Base Spells");

				//Actor Base Spells
				int numberOfBaseSpells = pActorBase->spellList.GetSpellCount();

				for (int i = 0; i < numberOfBaseSpells; i++)
				{
					ExtraInfoEntry * spellEntry;

					SpellItem* spell = pActorBase->spellList.GetNthSpell(i);
					GetSpellDataWrapper(spellEntry, spell, "Base Spell");

					SpellsArray->PushBack(spellEntry);
				}

				allSpellsEntry->PushBack(SpellsArray);

				resultArray->PushBack(allSpellsEntry);

				DebugMessage("GetCharacterData: GetCharacter Done with spells");

				if (pActor)
				{
					// ActiveEffects as Array
					ExtraInfoEntry * activeEffectsEntry;

					CreateExtraInfoEntry(activeEffectsEntry, "Effects", "");

					ExtraInfoEntry * activeEffectsArray;

					CreateExtraInfoEntry(activeEffectsArray, "", "");;

					tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();

					DebugMessage("GetCharacterData: Active Effects Gotten");

					if (effects)
					{
						for (int i = 0; i < effects->Count(); i++)
						{
							DebugMessage("GetCharacterData: Starting Active Effect");

							ActiveEffect * pEffect = effects->GetNthItem(i);

							ExtraInfoEntry * effectEntry;

							if (pEffect->effect && pEffect->effect->mgef)
							{
								DebugMessage("GetCharacterData: Active Effect MGEF found");

								std::string effectName, effectActive;

								EffectSetting * mgef = pEffect->effect->mgef;

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

								ExtraInfoEntry * effectEntrySubArray;

								CreateExtraInfoEntry(effectEntrySubArray, "", "");

								TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

								if (effectBaseForm)
								{
									DebugMessage("GetCharacterData: Active Effect MGEF base form found");

									GetCommonFormData(effectEntrySubArray, effectBaseForm, nullptr);

									//Magnitude
									ExtraInfoEntry * magnitudeEntry;

									float magnitude = pEffect->magnitude;
									CreateExtraInfoEntry(magnitudeEntry, "Magnitude", FloatToString(magnitude));
									effectEntrySubArray->PushBack(magnitudeEntry);

									//Duration
									ExtraInfoEntry * durationEntry;

									float duration = pEffect->duration;
									CreateExtraInfoEntry(durationEntry, "Duration", FloatToString(duration));
									effectEntrySubArray->PushBack(durationEntry);


									//Magnitude
									ExtraInfoEntry * elapsedEntry;

									float elapsed = pEffect->elapsed;
									CreateExtraInfoEntry(elapsedEntry, "Elapsed", FloatToString(elapsed));
									effectEntrySubArray->PushBack(elapsedEntry);

									GetMagicEffectData(effectEntrySubArray, effectBaseForm);

									effectEntry->PushBack(effectEntrySubArray);
								}
							}

							else
							{
								CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "");
							}

							activeEffectsArray->PushBack(effectEntry);


							DebugMessage("GetCharacterData: Ending Active Effect");

							/*if (pEffect->item)
							scaleformExtend::MagicItemData(effectView, pEffect->item, bRecursive ? bExtra : false, bRecursive); ??? */

							//RegisterBool(effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);

							// ActiveEffect
							//if (pEffect->effect && pEffect->effect->mgef)
							//	scaleformExtend::MagicItemData(effectView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);

							//activeEffects->PushBack(effect);
						}
					}

					activeEffectsEntry->PushBack(activeEffectsArray);
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
					ExtraInfoEntry * actorValueArrayEntry;
					CreateExtraInfoEntry(actorValueArrayEntry, "Actor Values", "");

					ExtraInfoEntry * actorValueArray;

					CreateExtraInfoEntry(actorValueArray, "", "");

					for (int i = 0; i < ActorValueList::kNumActorValues; i++)
					{

						ExtraInfoEntry * actorValue;
						GetActorValue(actorValue, pActor, i);
						actorValueArray->PushBack(actorValue);
					}


					actorValueArrayEntry->PushBack(actorValueArray);
					resultArray->PushBack(actorValueArrayEntry);

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

							ExtraInfoEntry * packageSubArray;
							CreateExtraInfoEntry(packageSubArray, "", "");
							GetFormData(packageSubArray, currentPackage, nullptr);

							packageEntry->PushBack(packageSubArray);
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

					ExtraInfoEntry * levelMultSubArray;
					CreateExtraInfoEntry(levelMultSubArray, "", "");
					double levelMult = (double)pNPC->actorData.level / 1000.0;
					int minLevel = pNPC->actorData.minLevel;
					int maxLevel = pNPC->actorData.maxLevel;

					ExtraInfoEntry * levelMultEntry, * minLevelEntry, * maxLevelEntry;

					CreateExtraInfoEntry(levelMultEntry, "Level Mult", DoubleToString(levelMult));
					levelMultSubArray->PushBack(levelMultEntry);

					CreateExtraInfoEntry(minLevelEntry, "Min level", IntToString(minLevel));
					levelMultSubArray->PushBack(minLevelEntry);

					CreateExtraInfoEntry(maxLevelEntry, "Max Level", IntToString(maxLevel));
					levelMultSubArray->PushBack(maxLevelEntry);

					isPcLeveledEntry->PushBack(levelMultSubArray);
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

				ExtraInfoEntry * perkSubArray;
				CreateExtraInfoEntry(perkSubArray, "", "");

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

						ExtraInfoEntry * perkEntrySubArray;
						CreateExtraInfoEntry(perkEntrySubArray, "", "");

						GetFormData(perkEntrySubArray, perk, nullptr);

						//ExtraInfoEntry * perkEntryRank;

						//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank) );
						//perkEntrySubArray->PushBack(perkEntryRank);

						perkEntry->PushBack(perkEntrySubArray);

						perkSubArray->PushBack(perkEntry);
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

							ExtraInfoEntry * perkEntrySubArray;
							CreateExtraInfoEntry(perkEntrySubArray, "", "");

							GetFormData(perkEntrySubArray, perk, nullptr);

							//ExtraInfoEntry * perkEntryRank;

							//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank));
							//perkEntrySubArray->PushBack(perkEntryRank);

							perkEntry->PushBack(perkEntrySubArray);

							perkSubArray->PushBack(perkEntry);
						}
					}
				}

				perks->PushBack(perkSubArray);

				resultArray->PushBack(perks);

				DebugMessage("GetExtraData: Done with perks");



				//apperance - currently height and weight
				DebugMessage("GetExtraData: appearance Started");

				ExtraInfoEntry * appearance;
				CreateExtraInfoEntry(appearance, "Appearance", "");

				ExtraInfoEntry * appearanceSubArray;
				CreateExtraInfoEntry(appearanceSubArray, "", "");

				float weight = pNPC->weight;

				ExtraInfoEntry * weightEntry;

				CreateExtraInfoEntry(weightEntry, "Weight", FloatToString(weight));
				appearanceSubArray->PushBack(weightEntry);

				float height = pNPC->height;

				ExtraInfoEntry * heightEntry;

				CreateExtraInfoEntry(heightEntry, "Height", FloatToString(height));
				appearanceSubArray->PushBack(heightEntry);

				appearance->PushBack(appearanceSubArray);

				resultArray->PushBack(appearance);

				DebugMessage("GetExtraData: appearance Ended");

				DebugMessage("GetExtraData: factions start");

				ExtraInfoEntry * factionsEntry;

				CreateExtraInfoEntry(factionsEntry, "Factions", "");

				//Factions
				int numFactions = pActorBase->actorData.factions.count;

				if (numFactions > 0)
				{
					ExtraInfoEntry * factionsSubArray;

					CreateExtraInfoEntry(factionsSubArray, "", "");

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

							ExtraInfoEntry * factionEntrySubarrray;
							CreateExtraInfoEntry(factionEntrySubarrray, "", "");;
							GetFormData(factionEntrySubarrray, faction, nullptr);
							factionEntry->PushBack(factionEntrySubarrray);

							factionsSubArray->PushBack(factionEntry);
						}
					}

					factionsEntry->PushBack(factionsSubArray);
				}

				resultArray->PushBack(factionsEntry);

				//need to get factions off of reference as well

				DebugMessage("GetExtraData: factions ended");

			}
		}


		/*PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
		if (pPC)
		{
		RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
		}*/

		DebugMessage("GetExtraData: GetCharacter End");
	}

	void GetActorValue(ExtraInfoEntry * resultArray,  Actor * pActor, int id)
	{
		DebugMessage("GetExtraData: GetActover Value Start");

		if (id < ActorValueList::kNumActorValues)
		{
			std::string valueName = GetActorValueName(id);
			float baseValue = pActor->actorValueOwner.GetBase(id);
			float currentValue = pActor->actorValueOwner.GetCurrent(id);
			float maxValue = pActor->actorValueOwner.GetCurrent(id);

			CreateExtraInfoEntry(resultArray, valueName, FloatToString(currentValue));

			//create a subarray for the base  current and maximum

			ExtraInfoEntry * subArray;

			CreateExtraInfoEntry(subArray, "", "");

			ExtraInfoEntry * baseValueEntry, * currentValueEntry, * maxValueEntry;

			CreateExtraInfoEntry(baseValueEntry, "Base", FloatToString(baseValue));
			CreateExtraInfoEntry(currentValueEntry, "Current", FloatToString(currentValue));
			CreateExtraInfoEntry(maxValueEntry, "Max", FloatToString(maxValue));

			subArray->PushBack(baseValueEntry);
			subArray->PushBack(currentValueEntry);
			subArray->PushBack(maxValueEntry);

			resultArray->PushBack(subArray);
		}

		else
		{
			//the Program will probally crash if we don't create an entry, so add an entry reporting the error in this case
			CreateExtraInfoEntry(resultArray, "Unknown AV id", IntToString(id));
		}

		DebugMessage("GetExtraData: GetActover Value End");
	}

	void GetMagicEffectData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("GetExtraData: GetMagicEffectData Start");


		EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
		if (pEffectSetting)
		{
			DebugMessage("GetExtraData: GetMagicEffectData Cast succeeded");

			//Magic School
			int skill = pEffectSetting->school();
			ExtraInfoEntry * skillEntry;

			CreateExtraInfoEntry(skillEntry, "Magic Skill", GetActorValueName(skill));
			resultArray->PushBack(skillEntry);

			//Minimum Skill Level

			int minimumSkill = pEffectSetting->level();
			ExtraInfoEntry * minimumSkillEntry;

			CreateExtraInfoEntry(minimumSkillEntry, "Minimum Skill", IntToString(minimumSkill));
			resultArray->PushBack(minimumSkillEntry);

			//Effect type 

			int effectType = pEffectSetting->properties.archetype;
			ExtraInfoEntry * effectTypeEntry;

			CreateExtraInfoEntry(effectTypeEntry, "Effect type", GetEffectTypeName(effectType));
			resultArray->PushBack(effectTypeEntry);

			//First AV
			int primaryType = pEffectSetting->properties.primaryValue;
			ExtraInfoEntry * primaryAVEntry;

			CreateExtraInfoEntry(primaryAVEntry, "Primary AV", GetActorValueName(primaryType));
			resultArray->PushBack(primaryAVEntry);

			//Second AV
			int secondaryType = pEffectSetting->properties.secondaryValue;
			ExtraInfoEntry * secondaryAVEntry;

			CreateExtraInfoEntry(secondaryAVEntry, "Secondary AV", GetActorValueName(secondaryType));
			resultArray->PushBack(secondaryAVEntry);

			//Resistence
			int resistence = pEffectSetting->properties.resistance;
			ExtraInfoEntry * resistenceEntry;

			CreateExtraInfoEntry(resistenceEntry, "Primary AV", GetActorValueName(resistence));
			resultArray->PushBack(resistenceEntry);


			//delivery type
			ExtraInfoEntry * deliveryTypeEntry;

			CreateExtraInfoEntry(deliveryTypeEntry, "Delivery Type", GetDeliveryTypeName(pEffectSetting->properties.deliveryType));
			resultArray->PushBack(deliveryTypeEntry);

			//Hostile Flag
			ExtraInfoEntry * hostileEntry;

			bool hostile = (pEffectSetting->properties.flags & EffectSetting::Properties::kEffectType_Hostile) == EffectSetting::Properties::kEffectType_Hostile;

			if (hostile)
			{
				CreateExtraInfoEntry(hostileEntry, "Hostile", "true");
			}

			else
			{
				CreateExtraInfoEntry(hostileEntry, "Hostile", "false");
			}
			resultArray->PushBack(hostileEntry);
			/*



			//RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags); //I added the hostile flag. Need to check what else to add
			RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType); //do these next 3 when I add spells
			RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
			RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
			RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);*/
		}

		DebugMessage("GetExtraData: GetMagicEffectData End");
	}

	void GetSpellData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		/*
		case kFormType_Spell:
		case kFormType_ScrollItem:
		case kFormType_Ingredient:
		case kFormType_Potion:
		case kFormType_Enchantment:
		{ */
		MagicItem * pMagicItem = DYNAMIC_CAST(pBaseForm, TESForm, MagicItem);
		if (pMagicItem)
		{
			DebugMessage("GetSpellData: Starting Effect Data Effect");

			ExtraInfoEntry * magicEffectsEntry;

			CreateExtraInfoEntry(magicEffectsEntry, "Magic Effects", "");

			ExtraInfoEntry * magicEffectsArray;
			CreateExtraInfoEntry(magicEffectsArray, "", "");

			int numEffects = pMagicItem->effectItemList.count;

			for (int i = 0; i < numEffects; i++)
			{


				MagicItem::EffectItem * pEffect = pMagicItem->effectItemList[i];

				ExtraInfoEntry * effectEntry;

				if (pEffect && pEffect->mgef)
				{
					DebugMessage("GetSpellData: Active Effect MGEF found");

					std::string effectName, effectActive;

					EffectSetting * mgef = pEffect->mgef;

					if (mgef->fullName.name.data)
					{
						effectName = mgef->fullName.name.data;
					}

					else
					{
						effectName = "Unknown Effect";
					}

					CreateExtraInfoEntry(effectEntry, effectName, effectActive);

					ExtraInfoEntry * effectEntrySubArray;

					CreateExtraInfoEntry(effectEntrySubArray, "", "");

					TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

					if (effectBaseForm)
					{
						DebugMessage("GetExtraData: Active Effect MGEF base form found");

						GetCommonFormData(effectEntrySubArray, effectBaseForm, nullptr);

						//Magnitude
						ExtraInfoEntry * magnitudeEntry;

						float magnitude = pEffect->magnitude;
						CreateExtraInfoEntry(magnitudeEntry, "Magnitude", FloatToString(magnitude));
						effectEntrySubArray->PushBack(magnitudeEntry);

						//Duration
						ExtraInfoEntry * durationEntry;

						float duration = pEffect->duration;
						CreateExtraInfoEntry(durationEntry, "Duration", FloatToString(duration));
						effectEntrySubArray->PushBack(durationEntry);


						//Magnitude
						ExtraInfoEntry * areaEntry;

						float area = pEffect->area;
						CreateExtraInfoEntry(areaEntry, "Area", FloatToString(area));
						effectEntrySubArray->PushBack(areaEntry);

						GetMagicEffectData(effectEntrySubArray, effectBaseForm);

						effectEntry->PushBack(effectEntrySubArray);
					}
				}

				else
				{
					CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "");
				}

				magicEffectsArray->PushBack(effectEntry);


				DebugMessage("GetSpellData: Ending Active Effect");


			}

			magicEffectsEntry->PushBack(magicEffectsArray);
			resultArray->PushBack(magicEffectsEntry);

			DebugMessage("GetSpellData: Done general magic item code");
		}

		SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
		if (pSpellItem)
		{
			DebugMessage("GetSpellData: Starting spell item code");

			//spell type
			ExtraInfoEntry * spellTypeEntry;

			CreateExtraInfoEntry(spellTypeEntry, "Spell Type", GetSpellTypeName(pSpellItem->data.type));
			resultArray->PushBack(spellTypeEntry);

			//casting type
			ExtraInfoEntry * castingTypeEntry;

			CreateExtraInfoEntry(castingTypeEntry, "Cast Type", GetCastingTypeName(pSpellItem->data.castType));
			resultArray->PushBack(castingTypeEntry);

			//spell cost
			ExtraInfoEntry * spellCostEntry;

			CreateExtraInfoEntry(spellCostEntry, "True Cost", IntToString(pSpellItem->GetMagickaCost()));
			resultArray->PushBack(spellCostEntry);

			//cast time
			ExtraInfoEntry * castTimeEntry;

			CreateExtraInfoEntry(castTimeEntry, "Cast Time", FloatToString(pSpellItem->data.castTime));
			resultArray->PushBack(castTimeEntry);

			//RegisterNumber(pFxVal, "spellType", pSpellItem->data.type);
			//RegisterNumber(pFxVal, "trueCost", pSpellItem->GetMagickaCost());

			/*
			BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
			if (equipSlot)
			RegisterNumber(pFxVal, "equipSlot", equipSlot->formID); */

			DebugMessage("GetSpellData: Ending spell item code");
		}
		/*
		AlchemyItem * pAlchemyItem = DYNAMIC_CAST(pMagicItem, MagicItem, AlchemyItem);
		if (pAlchemyItem)
		{
		if (pAlchemyItem->itemData.useSound) {
		ExtraInfoEntry * useSound;
		movieView->CreateObject(useSound);
		scaleformExtend::FormData(useSoundView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("useSound", &useSound);
		}
		}

		EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
		if (pEnchantItem)
		{
		RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

		ExtraInfoEntry * baseEnchant;
		movieView->CreateObject(baseEnchant);
		scaleformExtend::FormData(baseEnchantView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("baseEnchant", &baseEnchant);

		ExtraInfoEntry * restrictions;
		movieView->CreateObject(restrictions);
		scaleformExtend::FormData(restrictionsView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("restrictions", &restrictions);
		}
		}	*/
	}

	//wrapper for getting both the common form data and the spell data for a spell
	void GetSpellDataWrapper(ExtraInfoEntry * spellEntry,  SpellItem* spell, std::string source)
	{
		DebugMessage("GetSpellDataWrapper: Starting spell");

		TESForm *spellBaseForm = DYNAMIC_CAST(spell, SpellItem, TESForm);

		if (spellBaseForm)
		{
			std::string spellName = "Unknown";

			if (spell->fullName.name.data)
			{
				spellName = spell->fullName.name.data;
			}

			CreateExtraInfoEntry(spellEntry, spellName, source);

			ExtraInfoEntry * spellEntrySubArray;
			CreateExtraInfoEntry(spellEntrySubArray, "", "");

			GetCommonFormData(spellEntrySubArray, spellBaseForm, nullptr);
			GetSpellData(spellEntrySubArray, spellBaseForm);

			spellEntry->PushBack(spellEntrySubArray);
		}

		else
		{
			CreateExtraInfoEntry(spellEntry, "Unknown Spell", source);
		}

		DebugMessage("GetSpellDataWrapper: Finished spell");
	}

	void GetEquipment(ExtraInfoEntry * resultArray,  ExtraContainerChanges* pContainerChanges, Actor * pActor)
	{
		DebugMessage("GetEquipment: GetEquipment Start");

		ExtraInfoEntry * equipmentEntry;

		CreateExtraInfoEntry(equipmentEntry, "Equipment", "");

		ExtraInfoEntry * equipmentSubArray;

		CreateExtraInfoEntry(equipmentSubArray, "", "");

		//weapons and shouts

		//left hand
		TESForm * leftHand = pActor->GetEquippedObject(true);

		if (leftHand != nullptr)
		{
			ExtraInfoEntry * leftHandEntry;
			std::string name = GetName(leftHand);

			CreateExtraInfoEntry(leftHandEntry, "Left Hand:", name);

			ExtraInfoEntry * leftHandSubArray;
			CreateExtraInfoEntry(leftHandSubArray, "", "");

			GetFormData(leftHandSubArray, leftHand, nullptr);
			leftHandEntry->PushBack(leftHandSubArray);
			equipmentSubArray->PushBack(leftHandEntry);
		}

		//right hand
		TESForm * rightHand = pActor->GetEquippedObject(false);

		if (rightHand != nullptr)
		{
			ExtraInfoEntry * rightHandEntry;
			std::string name = GetName(rightHand);

			CreateExtraInfoEntry(rightHandEntry, "Right Hand:", name);

			ExtraInfoEntry * rightHandSubArray;
			CreateExtraInfoEntry(rightHandSubArray, "", "");

			GetFormData(rightHandSubArray, rightHand, nullptr);
			rightHandEntry->PushBack(rightHandSubArray);
			equipmentSubArray->PushBack(rightHandEntry);
		}

		//shout
		TESForm * shout = pActor->equippedShout;

		if (shout != nullptr)
		{
			ExtraInfoEntry * ShoutEntry;
			std::string name = GetName(shout);

			CreateExtraInfoEntry(ShoutEntry, "Shout:", name);

			ExtraInfoEntry * shoutSubArray;
			CreateExtraInfoEntry(shoutSubArray, "", "");

			GetFormData(shoutSubArray, shout, nullptr);
			ShoutEntry->PushBack(shoutSubArray);
			equipmentSubArray->PushBack(ShoutEntry);
		}

		//check each equip slot
		for (int i = 0; i < 32; i++)
		{
			DebugMessage("GetEquipment: Starting EquipSlot item");

			int mask = 1 << i;

			MatchBySlot matcher(mask);
			EquipData eqD = pContainerChanges->FindEquipped(matcher);

			if (eqD.pForm != nullptr)
			{
				DebugMessage("GetEquipment: EquipSlot item Found");

				TESForm * equipedItem = eqD.pForm;

				ExtraInfoEntry * equipedItemEntry;
				std::string name = GetName(equipedItem);
				std::string slotName = GetEquipSlotName(i);

				CreateExtraInfoEntry(equipedItemEntry, slotName, name);

				ExtraInfoEntry * equipedItemEntrySubArray;
				CreateExtraInfoEntry(equipedItemEntrySubArray, "", "");

				GetFormData(equipedItemEntrySubArray, equipedItem, nullptr);
				equipedItemEntry->PushBack(equipedItemEntrySubArray);
				equipmentSubArray->PushBack(equipedItemEntry);

			}

			DebugMessage("GetEquipment: Ending EquipSlot item");
		}


		equipmentEntry->PushBack(equipmentSubArray);

		resultArray->PushBack(equipmentEntry);

		DebugMessage("GetEquipment: GetEquipment End");
	}

	void GetInventory(ExtraInfoEntry * resultArray,  EntryDataList * inventory, TESContainer * baseContainer)
	{
		DebugMessage("GetInventory: GetInventory Start");

		ExtraInfoEntry * inventoryEntry;

		CreateExtraInfoEntry(inventoryEntry, "Inventory", "");

		ExtraInfoEntry * inventorySubArray;

		CreateExtraInfoEntry(inventorySubArray, "", "");

		if (inventory != nullptr)
		{
			MICGlobals::reducedMode = true;
		}

		//go through the inventory (these are anything changed from the base form)
		if (inventory != nullptr
			&& inventory->Count() > 0)
		{
			for (EntryDataList::Iterator i = inventory->Begin(); !i.End(); ++i)
			{
				DebugMessage("GetInventory: Starting inventory item");

				InventoryEntryData * item = i.Get();


				int itemCount = item->countDelta;
				TESForm *itemForm = item->type;

				if (baseContainer != nullptr)
				{
					itemCount = itemCount + NumberOfItemInContainer(itemForm, baseContainer);
				}

				if (itemCount > 0)
				{
					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));

					ExtraInfoEntry * inventoryItemEntrySubArray;
					CreateExtraInfoEntry(inventoryItemEntrySubArray, "", "");

					GetFormData(inventoryItemEntrySubArray, itemForm, nullptr);

					inventoryItemEntry->PushBack(inventoryItemEntrySubArray);

					inventorySubArray->PushBack(inventoryItemEntry);
				}

				DebugMessage("GetInventory: Ending inventory item");

			}
		}

		//go through the items in the base form
		if (baseContainer != nullptr)
		{
			for (int i = 0; i < baseContainer->numEntries; i++)
			{

				DebugMessage("GetInventory: Starting container item");

				TESForm *itemForm = baseContainer->entries[i]->form;

				//only display items which have not changed from the base form
				bool displayItem = true;

				if (inventory != nullptr)
				{
					displayItem = !HasItem(inventory, itemForm);
				}

				if (displayItem
					&& itemForm->GetFormType() != kFormType_LeveledItem)
				{

					DebugMessage("GetInventory: Displaying container item");
					int itemCount = baseContainer->entries[i]->count;

					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					if (name != "")
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));
					}

					else
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, FormIDToString(itemForm->formID));
					}

					ExtraInfoEntry * inventoryItemEntrySubArray;
					CreateExtraInfoEntry(inventoryItemEntrySubArray, "", "");

					GetFormData(inventoryItemEntrySubArray, itemForm, nullptr);

					inventoryItemEntry->PushBack(inventoryItemEntrySubArray);

					inventorySubArray->PushBack(inventoryItemEntry);
				}


				DebugMessage("GetInventory: Ending container item");
			}
		}

		inventoryEntry->PushBack(inventorySubArray);

		resultArray->PushBack(inventoryEntry);

		MICGlobals::reducedMode = false;

		DebugMessage("GetInventory: GetInventory End");
	}

	void GetArmaData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("GetArmaData: GetArmaData Start");

		TESObjectARMA * pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);

		if (pArma)
		{
			DebugMessage("GetArmaData: Before Cast");
			TESForm * maleSkin = (TESForm *)pArma->unk130;

			DebugMessage("GetArmaData: After Cast");
			if (maleSkin)
			{
				ExtraInfoEntry * maleSkinEntry;
				CreateExtraInfoEntry(maleSkinEntry, "Male skin", "");

				ExtraInfoEntry * maleSkinSubEntry;
				CreateExtraInfoEntry(maleSkinSubEntry, "", "");

				GetFormData(maleSkinSubEntry, maleSkin, nullptr);

				maleSkinEntry->PushBack(maleSkinSubEntry);
				resultArray->PushBack(maleSkinEntry);
			}


			DebugMessage("GetArmaData: Before Cast");
			TESForm * femaleSkin = (TESForm *)pArma->unk138;

			DebugMessage("GetArmaData: After Cast");
			if (femaleSkin)
			{
				ExtraInfoEntry * maleSkinEntry;
				CreateExtraInfoEntry(maleSkinEntry, "Female skin", "");

				ExtraInfoEntry * maleSkinSubEntry;
				CreateExtraInfoEntry(maleSkinSubEntry, "", "");

				GetFormData(maleSkinSubEntry, femaleSkin, nullptr);

				maleSkinEntry->PushBack(maleSkinSubEntry);
				resultArray->PushBack(maleSkinEntry);
			}

			//races

			MICGlobals::readRaceSkins = false; //disable reading race skin entries to avoid infinite loops

											   //Primary race
			TESRace * defaultRace = pArma->race.race;
			std::string defaultRaceName = GetName(defaultRace);

			ExtraInfoEntry * defaultRacesEntry;

			CreateExtraInfoEntry(defaultRacesEntry, "Primary Race", defaultRaceName);

			ExtraInfoEntry * defaultRacesEntrySubArray;

			CreateExtraInfoEntry(defaultRacesEntrySubArray, "", "");

			GetFormData(defaultRacesEntrySubArray, defaultRace, nullptr);

			defaultRacesEntry->PushBack(defaultRacesEntrySubArray);
			resultArray->PushBack(defaultRacesEntry);

			//Additional races

			if (pArma->additionalRaces.count > 0)
			{
				ExtraInfoEntry * additionalRacesEntry;

				CreateExtraInfoEntry(additionalRacesEntry, "Additional Races", "");

				ExtraInfoEntry * additionalRacesEntrySubArray;

				CreateExtraInfoEntry(additionalRacesEntrySubArray, "", "");

				for (int i = 0; i < pArma->additionalRaces.count; i++)
				{
					TESRace * additionalRace = pArma->additionalRaces[i];

					if (additionalRace)
					{
						std::string additionalRaceName = GetName(additionalRace);

						ExtraInfoEntry * additionalRaceEntry;

						CreateExtraInfoEntry(additionalRaceEntry, additionalRaceName, "");

						ExtraInfoEntry * additionalRaceEntrySubArray;

						CreateExtraInfoEntry(additionalRaceEntrySubArray, "", "");

						GetFormData(additionalRaceEntrySubArray, additionalRace, nullptr);

						additionalRaceEntry->PushBack(additionalRaceEntrySubArray);
						additionalRacesEntrySubArray->PushBack(additionalRaceEntry);
					}
				}

				additionalRacesEntry->PushBack(additionalRacesEntrySubArray);

				resultArray->PushBack(additionalRacesEntry);
			}


			MICGlobals::readRaceSkins = true;

			//resultArray->PushBack(racesEntry);

			//Skin Textures

		}

		DebugMessage("GetArmaData: GetArmaData End");
	}

	void GetArmorData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectARMO * pArmor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMO);

		if (pArmor)
		{
			//value
			int armorRating = pArmor->armorValTimes100 / 100;

			ExtraInfoEntry * armorRatingEntry;

			CreateExtraInfoEntry(armorRatingEntry, "Armor Rating", IntToString(armorRating));
			resultArray->PushBack(armorRatingEntry);

			//value
			int value = pArmor->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

			//weight
			int weight = pArmor->weight.weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", IntToString(weight));
			resultArray->PushBack(weightEntry);

			//weight class
			int weightClass = pArmor->bipedObject.data.weightClass;

			ExtraInfoEntry * weightClassEntry;

			CreateExtraInfoEntry(weightClassEntry, "Armor Type", GetArmorWeightClassName(weightClass));
			resultArray->PushBack(weightClassEntry);

			//Equip slots
			int parts = pArmor->bipedObject.data.parts;

			ExtraInfoEntry * equipSlotsEntry;

			CreateExtraInfoEntry(equipSlotsEntry, "Equip Slots", "");

			ExtraInfoEntry * equipSlotsEntrySubArray;
			CreateExtraInfoEntry(equipSlotsEntrySubArray, "", "");

			for (int i = 0; i <= 31; i++)
			{
				int mask = 1 << i;

				if ((parts & mask) == mask)
				{

					std::string slotName = GetEquipSlotName(i);

					ExtraInfoEntry * equipSlotEntry;

					CreateExtraInfoEntry(equipSlotEntry, slotName, "");
					equipSlotsEntrySubArray->PushBack(equipSlotEntry);
				}
			}

			equipSlotsEntry->PushBack(equipSlotsEntrySubArray);
			resultArray->PushBack(equipSlotsEntry);

			if (!MICGlobals::reducedMode)
			{
				ExtraInfoEntry * armorAddonsEntry;

				CreateExtraInfoEntry(armorAddonsEntry, "Armor Entry", "");

				ExtraInfoEntry * armorAddonsSubArray;
				CreateExtraInfoEntry(armorAddonsSubArray, "", "");

				for (int i = 0; i < pArmor->armorAddons.count; i++)
				{
					bool addEntry = true;

					//if we are filtering by race
					if (MICGlobals::filterARMAByRace != nullptr)
					{
						addEntry = pArmor->armorAddons[i]->isValidRace(MICGlobals::filterARMAByRace);
					}

					if (addEntry)
					{
						ExtraInfoEntry * armorAddonEntry;

						std::string armorAddonName = GetName(pArmor->armorAddons[i]);

						CreateExtraInfoEntry(armorAddonEntry, armorAddonName, "");

						ExtraInfoEntry * armorAddonSubArray;
						CreateExtraInfoEntry(armorAddonSubArray, "", "");

						GetFormData(armorAddonSubArray, pArmor->armorAddons[i], nullptr);

						armorAddonEntry->PushBack(armorAddonSubArray);
						armorAddonsSubArray->PushBack(armorAddonEntry);
					}
				}

				armorAddonsEntry->PushBack(armorAddonsSubArray);
				resultArray->PushBack(armorAddonsEntry);
			}
		}
	}

	void GetWeaponData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectWEAP * pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			//animation type
			int animationType = pWeapon->type();

			ExtraInfoEntry * antimationTypeEntry;

			CreateExtraInfoEntry(antimationTypeEntry, "Animation type", GetWeaponAnimationTypeName(animationType));
			resultArray->PushBack(antimationTypeEntry);

			//damage
			int damage = pWeapon->damage.GetAttackDamage();

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", IntToString(damage));
			resultArray->PushBack(damageEntry);

			//speed
			float speed = pWeapon->speed();

			ExtraInfoEntry * speedEntry;

			CreateExtraInfoEntry(speedEntry, "Speed", FloatToString(speed));
			resultArray->PushBack(speedEntry);

			//reach
			float reach = pWeapon->reach();

			ExtraInfoEntry * reachEntry;

			CreateExtraInfoEntry(reachEntry, "Reach", FloatToString(reach));
			resultArray->PushBack(reachEntry);

			//stagger
			float stagger = pWeapon->stagger();

			ExtraInfoEntry * staggerEntry;

			CreateExtraInfoEntry(staggerEntry, "Stagger", FloatToString(stagger));
			resultArray->PushBack(staggerEntry);


			//crit damage
			int critDamage = pWeapon->critDamage();

			ExtraInfoEntry * critDamageEntry;

			CreateExtraInfoEntry(critDamageEntry, "Crit damage", IntToString(critDamage));
			resultArray->PushBack(critDamageEntry);

			/*RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
			RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange()); Data for npc AI*/

			BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
			if (equipSlot)
			{
				std::string equipSlotName = GetEquipTypeName(equipSlot->formID);

				ExtraInfoEntry * equipSlotEntry;

				CreateExtraInfoEntry(equipSlotEntry, "Equip slot", equipSlotName);
				resultArray->PushBack(equipSlotEntry);
			}

			//value
			int value = pWeapon->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

			//weight
			int weight = pWeapon->weight.weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", IntToString(weight));
			resultArray->PushBack(weightEntry);

		}
	}

	void GetAmmoData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESAmmo * pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
		if (pAmmo)
		{
			//damage
			float damage = pAmmo->settings.damage;

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", FloatToString(damage));
			resultArray->PushBack(damageEntry);


			//value
			int value = pAmmo->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

		}
	}

	void GetContainerData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectCONT * pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			int respawnsFlag = 0x02;

			ExtraInfoEntry * safeContainerEntry;

			if ((pContainer->unkB9 & respawnsFlag) == respawnsFlag)
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "No");
			}

			else
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "Yes");
			}

			resultArray->PushBack(safeContainerEntry);
		}
	}

	void GetPositionData(ExtraInfoEntry * resultArray,  TESForm* pRefForm)
	{
		DebugMessage("Starting GetPositionData");

		TESObjectREFR * pRef = DYNAMIC_CAST(pRefForm, TESForm, TESObjectREFR);

		if (pRef)
		{
			ExtraInfoEntry * positionEntry;
			CreateExtraInfoEntry(positionEntry, "Position", "");

			ExtraInfoEntry * positionSubArray;
			CreateExtraInfoEntry(positionSubArray, "", "");

			//position
			float xPos = pRef->pos.x;
			ExtraInfoEntry * xPositionEntry;
			CreateExtraInfoEntry(xPositionEntry, "X Position", FloatToString(xPos));
			positionSubArray->PushBack(xPositionEntry);

			float yPos = pRef->pos.y;
			ExtraInfoEntry * yPositionEntry;
			CreateExtraInfoEntry(yPositionEntry, "Y Position", FloatToString(yPos));
			positionSubArray->PushBack(yPositionEntry);

			float zPos = pRef->pos.z;
			ExtraInfoEntry * zPositionEntry;
			CreateExtraInfoEntry(zPositionEntry, "Z Position", FloatToString(zPos));
			positionSubArray->PushBack(zPositionEntry);

			//rotation
			float xRot = pRef->rot.x;
			ExtraInfoEntry * xRotationEntry;
			CreateExtraInfoEntry(xRotationEntry, "X Rotation", FloatToString(xRot));
			positionSubArray->PushBack(xRotationEntry);

			float yRot = pRef->rot.y;
			ExtraInfoEntry * yRotationEntry;
			CreateExtraInfoEntry(yRotationEntry, "Y Rotation", FloatToString(yRot));
			positionSubArray->PushBack(yRotationEntry);

			float zRot = pRef->rot.z;
			ExtraInfoEntry * zRotationEntry;
			CreateExtraInfoEntry(zRotationEntry, "Z Rotation", FloatToString(zRot));
			positionSubArray->PushBack(zRotationEntry);

			positionEntry->PushBack(positionSubArray);
			resultArray->PushBack(positionEntry);
		}

		DebugMessage("Ending GetPositionData");
	}


	//get data stored in the BSExtraData format
	void GetBSExtraData(ExtraInfoEntry * resultArray,  TESForm* pRefForm)
	{
		DebugMessage("Starting GetBSExtraData");

		TESObjectREFR * pRef = DYNAMIC_CAST(pRefForm, TESForm, TESObjectREFR);

		if (pRef)
		{
			BaseExtraList *extraList = &pRef->extraData;

			if (MICOptions::MICDebugMode)
			{
				//Placeholder for seeing what has editor IDs
				ExtraInfoEntry * editorIDEntry;

				if (extraList->HasType(kExtraData_EditorID))
				{
					CreateExtraInfoEntry(editorIDEntry, "Editor ID", "Yes");
				}
				else
				{
					CreateExtraInfoEntry(editorIDEntry, "Editor ID", "No");
				}

				resultArray->PushBack(editorIDEntry);
			}

			if (extraList->HasType(kExtraData_Ownership))
			{
				DebugMessage("Starting kExtraData_Ownership");

				BSExtraData * data = extraList->GetByType(kExtraData_Ownership);
				ExtraOwnership * ownershipData = DYNAMIC_CAST(data, BSExtraData, ExtraOwnership);

				if (ownershipData)
				{
					TESForm * owner = ownershipData->owner;
					std::string ownerName = GetName(owner);

					//Placeholder for seeing what has editor IDs
					ExtraInfoEntry * ownershipEntry;

					CreateExtraInfoEntry(ownershipEntry, "Owner", ownerName);


					DebugMessage("Before getting owner form data");

					ExtraInfoEntry * ownershipSubarray;
					CreateExtraInfoEntry(ownershipSubarray, "", "");
					GetFormData(ownershipSubarray, owner, nullptr);

					ownershipEntry->PushBack(ownershipSubarray);
					resultArray->PushBack(ownershipEntry);

				}


				DebugMessage("Ending kExtraData_Ownership");
			}


			if (MICOptions::MICDebugMode)
			{
				//Placeholder for seeing what has editor IDs
				ExtraInfoEntry * editorIDEntry;

				if (extraList->HasType(kExtraData_Package))
				{
					CreateExtraInfoEntry(editorIDEntry, "Extra Data Package", "Yes");
				}
				else
				{
					CreateExtraInfoEntry(editorIDEntry, "Extra Data Package", "No");
				}

				resultArray->PushBack(editorIDEntry);
			}

			/**
			if (extraList->HasType(kExtraData_Package))
			{
			DebugMessage("Starting kExtraData_Package");

			BSExtraData * data = extraList->GetByType(kExtraData_Package);
			ExtraPackage * packageData = DYNAMIC_CAST(data, BSExtraData, ExtraPackage);

			if (packageData)
			{
			TESForm * currentPackage = packageData->currentPackage;
			std::string packageName = GetName(currentPackage);

			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry * packageEntry;

			CreateExtraInfoEntry(packageEntry, "Current Package", packageName);


			DebugMessage("Before getting packag form data");

			ExtraInfoEntry * packageSubArray;
			movie->CreateArray(packageSubArray);
			GetFormData(packageSubArray, currentPackage, nullptr);

			packageEntry->PushBack(packageSubArray);
			resultArray->PushBack(packageEntry);

			}


			DebugMessage("Ending kExtraData_Package");
			}*/
		}




		DebugMessage("Ending GetBSExtraData");
	}
	/*
	void StandardItemData(ExtraInfoEntry * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
	{
	if (!pForm || !pFxVal || !pFxVal->IsObject())
	return;

	switch (pForm->GetFormType())
	{

	break;

	case kFormType_SoulGem:
	{
	TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
	if (soulGem)
	{
	RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
	RegisterNumber(pFxVal, "soulSize", pEntry ? CALL_MEMBER_FN(pEntry, GetSoulLevel)() : soulGem->soulSize);
	}
	}
	break;

	case kFormType_Potion:
	{
	AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
	if (pAlchemy)
	{
	RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
	}
	}
	break;

	case kFormType_Book:
	{
	TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
	if (pBook)
	{
	RegisterNumber(pFxVal, "flags", pBook->data.flags);
	RegisterNumber(pFxVal, "bookType", pBook->data.type);
	switch (pBook->data.GetSanitizedType())
	{
	case TESObjectBOOK::Data::kType_Skill:
	RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
	break;

	case TESObjectBOOK::Data::kType_Spell:
	{
	double formID = -1;

	if (pBook->data.teaches.spell)
	formID = pBook->data.teaches.spell->formID;

	RegisterNumber(pFxVal, "teachesSpell", formID);
	}
	break;
	}
	}
	}
	break;

	default:
	break;
	}
	}*/

	void GetModelTextures(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("Starting GetModelTextures " + GetFormTypeName(pBaseForm->formType));
		if (pBaseForm->formType == kFormType_Static)
		{
			TESObjectSTAT *pStatic = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectSTAT);

			if (pStatic)
			{
				TESModelTextureSwap * textSwap = &(pStatic->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_MovableStatic)
		{
			BGSMovableStatic *pMoveStatic = DYNAMIC_CAST(pBaseForm, TESForm, BGSMovableStatic);

			if (pMoveStatic)
			{
				TESModelTextureSwap * textSwap = &(pMoveStatic->staticObj.texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Activator)
		{
			TESObjectACTI *pActivate = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectACTI);

			if (pActivate)
			{
				TESModelTextureSwap * textSwap = &(pActivate->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Tree)
		{
			TESObjectTREE *pTree = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectTREE);

			if (pTree)
			{
				TESModel * model = &(pTree->model);
				AddModelEntry(resultArray, "Model", model);
			}
		}

		else if (pBaseForm->formType == kFormType_Flora)
		{
			TESFlora *pFlora = DYNAMIC_CAST(pBaseForm, TESForm, TESFlora);

			if (pFlora)
			{
				TESModelTextureSwap * textSwap = &(pFlora->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Furniture)
		{
			TESFurniture *pFurniture = DYNAMIC_CAST(pBaseForm, TESForm, TESFurniture);

			if (pFurniture)
			{
				TESModelTextureSwap * textSwap = &(pFurniture->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Door)
		{
			TESObjectDOOR *pDoor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectDOOR);
			if (pDoor)
			{
				TESModelTextureSwap * textSwap = &(pDoor->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Container)
		{
			TESObjectCONT *pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
			if (pContainer)
			{
				TESModelTextureSwap * textSwap = &(pContainer->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Misc)
		{
			TESObjectMISC *pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
			if (pMisc)
			{
				TESModelTextureSwap * textSwap = &(pMisc->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Book)
		{
			TESObjectBOOK *pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
			if (pBook)
			{
				TESModelTextureSwap * textSwap = &(pBook->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Key)
		{
			TESKey *pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
			if (pKey)
			{
				TESModelTextureSwap * textSwap = &(pKey->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_SoulGem)
		{
			TESSoulGem *pSoulGem = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
			if (pSoulGem)
			{
				TESModelTextureSwap * textSwap = &(pSoulGem->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Ingredient)
		{
			IngredientItem *pIngredient = DYNAMIC_CAST(pBaseForm, TESForm, IngredientItem);
			if (pIngredient)
			{
				TESModelTextureSwap * textSwap = &(pIngredient->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Potion)
		{
			AlchemyItem *pAlchemy = DYNAMIC_CAST(pBaseForm, TESForm, AlchemyItem);
			if (pAlchemy)
			{
				TESModelTextureSwap * textSwap = &(pAlchemy->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}


		else if (pBaseForm->formType == kFormType_Weapon)
		{
			TESObjectWEAP *pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
			if (pWeapon)
			{
				TESModelTextureSwap * textSwap = &(pWeapon->texSwap);

				if (textSwap)
				{
					AddModelEntry(resultArray, "Model", textSwap);
				}

				TESObjectSTAT * firstPersonModel = pWeapon->model;

				if (firstPersonModel)
				{
					TESModelTextureSwap * textSwapFirstPerson = &(firstPersonModel->texSwap);

					if (textSwapFirstPerson)
					{
						AddModelEntry(resultArray, "First Person Model", textSwapFirstPerson);
					}
				}
			}
		}

		else if (pBaseForm->formType == kFormType_ARMA)
		{
			TESObjectARMA *pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);
			if (pArma)
			{
				//Texture entries are theortical
				TESModelTextureSwap * maleModel = &(pArma->models[0][0]);

				if (maleModel)
				{
					AddModelEntry(resultArray, "Model Male", maleModel);
				}

				TESModelTextureSwap * femaleModel = &(pArma->models[0][1]);
				if (femaleModel)
				{
					AddModelEntry(resultArray, "Model Female", femaleModel);
				}

				TESModelTextureSwap * maleFirstPerson = &(pArma->models[1][0]);
				if (maleFirstPerson)
				{
					AddModelEntry(resultArray, "Model Male 1st Person", maleFirstPerson);
				}

				TESModelTextureSwap * femaleFirstPerson = &(pArma->models[1][1]);
				if (femaleFirstPerson)
				{
					AddModelEntry(resultArray, "Model Female 1st Person", femaleFirstPerson);
				}
			}
		}

		//_MESSAGE( textSwap->GetModelName() ); //example output Architecture\Winterhold\WinterholdExtTowerRing01.nif

		/*
		int count = textSwap->count;

		_MESSAGE(IntToString(count).c_str());

		for (int i = 0; i < count; i++)
		{
		BGSTextureSet * textureSet = textSwap->swaps[i].textureSet;

		for (int i = 0; i < textureSet->kNumTextures; i++)
		{
		_MESSAGE(textureSet->texturePaths[i].str);
		}
		}*/


		DebugMessage("Ending GetModelTextures");
	}

	void AddModelEntry(ExtraInfoEntry * resultArray,  std::string modelType, TESModelTextureSwap * modelTextureSwap)
	{
		DebugMessage("Starting AddModelEntry for modelTextureSwap");

		if (modelTextureSwap)
		{
			DebugMessage("Past modelTextureSwap");
			TESModel * model = modelTextureSwap;

			DebugMessage("Past conversion");

			BGSTextureSet * textureSet = nullptr;

			if (modelTextureSwap->swaps)
			{
				DebugMessage("Inside swaps check");
				textureSet = modelTextureSwap->swaps->textureSet;
			}

			DebugMessage("Past swaps check");

			if (textureSet)
			{
				DebugMessage("Starting Texture Set Branch");

				ExtraInfoEntry * modelTextureEntry;

				std::string modelPath = model->GetModelName();
				std::string modelName = GetFileName(modelPath);

				CreateExtraInfoEntry(modelTextureEntry, modelType, modelName);

				ExtraInfoEntry * modelTextureSubArray;
				CreateExtraInfoEntry(modelTextureSubArray, "", "");

				AddModelEntry(modelTextureSubArray, "Model", model);

				DebugMessage("Starting Texture Set Info");
				ExtraInfoEntry * textureSetEntry;

				CreateExtraInfoEntry(textureSetEntry, "Texture Set", "");

				ExtraInfoEntry * textureSetEntrySubArray;
				CreateExtraInfoEntry(textureSetEntrySubArray, "", "");

				GetFormData(textureSetEntrySubArray, textureSet, nullptr);

				textureSetEntry->PushBack(textureSetEntrySubArray);

				modelTextureSubArray->PushBack(textureSetEntry);

				modelTextureEntry->PushBack(modelTextureSubArray);

				resultArray->PushBack(modelTextureEntry);
			}

			else
			{
				DebugMessage("Starting No texture set branch");
				AddModelEntry(resultArray, modelType, model);
			}
		}

		DebugMessage("Ending AddModelEntry for modelTextureSwap");
	}

	void AddModelEntry(ExtraInfoEntry * resultArray,  std::string modelType, TESModel * model)
	{
		DebugMessage("Starting AddModelEntry for model");

		if (model)
		{
			std::string modelPath;
			modelPath.assign(model->GetModelName());

			if (modelPath != "")
			{
				DebugMessage("Get Model path");

				std::string modelName = GetFileName(modelPath);

				DebugMessage("Get Model name");

				ExtraInfoEntry * modelEntry;
				CreateExtraInfoEntry(modelEntry, modelType, modelName);

				DebugMessage("Splitting Model Path");


				CreateFilePathSubarray(modelEntry, modelPath);

				DebugMessage("Done Splitting Model Path");

				resultArray->PushBack(modelEntry);
			}
		}

		DebugMessage("Ending AddModelEntry for model");
	}

	void GetTextureSet(ExtraInfoEntry * resultArray,  TESForm * pBaseForm)
	{
		DebugMessage("Starting AddTextureSetEntry");

		BGSTextureSet * pTextureSet = DYNAMIC_CAST(pBaseForm, TESForm, BGSTextureSet);

		if (pTextureSet)
		{

			ExtraInfoEntry * textureSetEntry;
			CreateExtraInfoEntry(textureSetEntry, "Texture Set", "");

			ExtraInfoEntry * textureSetSubArray;

			CreateExtraInfoEntry(textureSetSubArray, "", "");

			for (int i = 0; i < BGSTextureSet::kNumTextures; i++)
			{
				TESTexture * texture = &pTextureSet->texturePaths[i];
				std::string texturePath = texture->str;

				std::string textureName = "";

				if (texturePath != "")
				{
					textureName = GetFileName(texturePath);
				}

				std::string textureType = GetTextureType(i);

				ExtraInfoEntry * textureEntry;

				CreateExtraInfoEntry(textureEntry, textureType, textureName);

				if (texturePath != "")
				{
					CreateFilePathSubarray(textureEntry, texturePath);
				}

				textureSetSubArray->PushBack(textureEntry);
			}

			textureSetEntry->PushBack(textureSetSubArray);
			resultArray->PushBack(textureSetEntry);
		}

		DebugMessage("Ending AddTextureSetEntry");
	}

	void CreateFilePathSubarray(ExtraInfoEntry * mainEntry,  std::string filePath)
	{
		DebugMessage("Starting CreateFilePathSubarray " + filePath);

		ExtraInfoEntry * filePathSubArray;
		CreateExtraInfoEntry(filePathSubArray, "", "");

		//loop through the string until the last slash is going
		int firstSlash = filePath.find_first_of(deliminator);
		while (firstSlash != std::string::npos)
		{
			//DebugMessage("Splitting with index: " + IntToString(firstSlash) + " with remaining path " + modelPath);

			ExtraInfoEntry * pathEntry;
			std::string path = filePath.substr(0, firstSlash + 1);

			CreateExtraInfoEntry(pathEntry, path, "");
			filePathSubArray->PushBack(pathEntry);

			filePath = filePath.substr(firstSlash + 1); //get everything after the first slash
			firstSlash = filePath.find_first_of(deliminator); //refind the first slash
		}

		ExtraInfoEntry * pathEntry;
		//add everything after the last slash
		CreateExtraInfoEntry(pathEntry, filePath, "");

		filePathSubArray->PushBack(pathEntry);

		mainEntry->PushBack(filePathSubArray);

		DebugMessage("Ending CreateFilePathSubarray");
	}

	void GetRaceEntry(ExtraInfoEntry * resultArray,  TESForm * pBaseForm)
	{
		DebugMessage("Starting GetRaceEntry");

		TESRace * pRace = DYNAMIC_CAST(pBaseForm, TESForm, TESRace);
		if (pRace)
		{
			DebugMessage("Getting Editor ID");

			//editor ID
			std::string editorID = pRace->editorId.Get();

			ExtraInfoEntry * editorIDEntry;

			CreateExtraInfoEntry(editorIDEntry, "EditorID", editorID);
			resultArray->PushBack(editorIDEntry);


			DebugMessage("Getting Models");
			//models
			TESModel * maleModel = &(pRace->models[0]);
			TESModel * femaleModel = &(pRace->models[1]);

			AddModelEntry(resultArray, "Male Skeleton", maleModel);
			AddModelEntry(resultArray, "Female Skeleton", femaleModel);

			//Skins
			if (MICGlobals::readRaceSkins
				&& pRace->skin.skin != nullptr)
			{
				DebugMessage("Getting Skin");
				TESObjectARMO *skin = pRace->skin.skin;

				std::string skinName = GetName(skin);

				ExtraInfoEntry * skinEntry;

				CreateExtraInfoEntry(skinEntry, "Skin", skinName);

				ExtraInfoEntry * skinSubEntry;

				CreateExtraInfoEntry(skinSubEntry, "", "");

				GetFormData(skinSubEntry, skin, nullptr);

				skinEntry->PushBack(skinSubEntry);
				resultArray->PushBack(skinEntry);

				DebugMessage("Done Getting Skin");
			}

			//Handle Flags
			int playableFlag = 0x00000001;
			int childFlag = 0x00000004;

			ExtraInfoEntry * playableEntry;

			if ((pRace->data.raceFlags & playableFlag) == playableFlag)
			{
				CreateExtraInfoEntry(playableEntry, "Playable", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(playableEntry, "Playable", "No");
			}

			resultArray->PushBack(playableEntry);

			ExtraInfoEntry * childEntry;

			if ((pRace->data.raceFlags & childFlag) == childFlag)
			{
				CreateExtraInfoEntry(childEntry, "Child", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(childEntry, "Child", "No");
			}

			resultArray->PushBack(childEntry);
		}

		DebugMessage("Ending GetRaceEntry");
	}

	/*
	void TraverseArray(GFxValue * scaleformArray, GFxMovieView * movie, int level, int level0Index, int level1Index, int level2Index, int level3Index)
	{
		int arrayLength = scaleformArray->GetArraySize();

		GFxValue returnValue;

		for (int i = 0; i < arrayLength; i++)
		{
			GFxValue arrayElement; 
			scaleformArray->GetElement(i, &arrayElement);

			if ( level >= 2 || arrayElement.GetType() == GFxValue::kType_String)
			{
				//send result
				GFxValue resultArray;

				movie->CreateArray(&resultArray);

				GFxValue zeroIndex;

				if (level == 0)
				{
					zeroIndex.SetNumber(i);
				}
				else
				{
					zeroIndex.SetNumber(level0Index);
				}

				resultArray->PushBack(&zeroIndex);

				if (level >= 1)
				{
					GFxValue oneIndex;

					if (level == 1)
					{
						oneIndex.SetNumber(i);
					}
					else
					{
						oneIndex.SetNumber(level1Index);
					}

					resultArray->PushBack(&oneIndex);

					if (level >= 2)
					{
						GFxValue twoIndex;

						if (level == 2)
						{
							twoIndex.SetNumber(i);
						}
						else
						{
							twoIndex.SetNumber(level2Index);
						}

						resultArray->PushBack(&twoIndex);

						if (level >= 3)
						{
							GFxValue threeIndex;

							if (level == 2)
							{
								threeIndex.SetNumber(i);
							}
							else
							{
								threeIndex.SetNumber(level3Index);
							}

							resultArray->PushBack(&threeIndex);

							if (level == 4)
							{

								GFxValue fourIndex;
								threeIndex.SetNumber(i);
								resultArray->PushBack(&fourIndex);
							}
						}
					}
				}

				resultArray->PushBack(&arrayElement);

				movie->Invoke("_root.consoleFader_mc.Console_mc.AddExtraInfo", &returnValue, &resultArray, 1);
			}

			else if (arrayElement.GetType() == GFxValue::kType_Array)
			{
				if (level <= 1)
				{
					if (level == 0)
					{
						level0Index = i;
					}

					else if (level == 1)
					{
						level1Index = i;
					}

					else if (level == 2)
					{
						level2Index = i;
					}

					else if (level == 3)
					{
						level3Index = i;
					}


					TraverseArray(&arrayElement, movie, level + 1, level0Index, level1Index, level2Index, level3Index );
				}
			}
		}
	}*/
};



class MICScaleform_GetIniOptions : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		args->movie->CreateObject(args->result);
		RegisterNumber(args->result, "Transparency", MICOptions::Transparency);
		RegisterNumber(args->result, "Scale", MICOptions::Scale);
		RegisterNumber(args->result, "FieldsToDisplay", MICOptions::FieldsToDisplay);
		RegisterNumber(args->result, "BaseInfoFormat", MICOptions::BaseInfoFormat);
	}
};


class MICScaleform_RetrieveExtraData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{

		DebugMessage("RetrieveExtraData: Invoke Start");

		GFxMovieView * movie = args->movie;
		GFxValue indexArray = args->args[0];

		ExtraInfoEntry * extrainfoEntryToRetrieve = TraverseExtraInfoEntries(&MICGlobals::rootEntry, &indexArray, 0);

		DebugMessage("RetrieveExtraData: Invoke End");
	}

	ExtraInfoEntry * TraverseExtraInfoEntries(ExtraInfoEntry * currentEntry, GFxValue * indexArray, int currentIndex)
	{
		GFxValue * indexToSelect;
		indexArray->GetElement(currentIndex, indexToSelect);

		ExtraInfoEntry * nextEntry = currentEntry->GetChild(indexToSelect->GetNumber());

		if (currentIndex + 1 == indexArray->GetArraySize())
		{
			return nextEntry;
		}

		else
		{
			return TraverseExtraInfoEntries(nextEntry, indexArray, currentIndex + 1);
		}
	}
};

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks(GFxMovieView * view, GFxValue * root)
{
	DebugMessage("Install Hooks Called");

	RegisterFunction <MICScaleform_GetReferenceInfo>(root, view, "MICScaleform_GetReferenceInfo");
	RegisterFunction <MICScaleform_GetExtraData>(root, view, "MICScaleform_GetExtraData");
	RegisterFunction <MICScaleform_GetIniOptions>(root, view, "MICScaleform_GetIniOptions");
	RegisterFunction <MICScaleform_RetrieveExtraData>(root, view, "MICScaleform_RetrieveExtraData");
	return true;
}