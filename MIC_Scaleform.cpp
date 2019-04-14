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
#include "PapyrusActorValueInfo.h"
#include <Windows.h>

const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminahIndex = 26;
const int playerBaseFormID = 0x7;
const char deliminator = '\\';

class MICScaleform_GetReferenceInfo : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{

		std::string refFormFromMod, refFormLastChangedBy;
		int numModsModifyingRef;

		DebugMessage("GetReferenceInfo: Called");
		//ASSERT(args->numArgs >= 1);

		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
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

		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
		if (pRef != nullptr)
		{

			DebugMessage("GetExtraData: pRefFound");

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				DebugMessage("GetExtraData: BaseFound");

				GetFormData(&result, movie, pBaseForm, pRef);

				DebugMessage("Get Form Information done");

				//GFxValue * FormLocationData;

				//result.GetElement(4, FormLocationData);

				//Send the data we created back to the console
				GFxValue returnValue;
				GFxValue arguments;

				UIStringHolder* stringHolder = UIStringHolder::GetSingleton();
				MenuManager* mm = MenuManager::GetSingleton();

				IMenu* consoleMenu = mm->GetMenu(&stringHolder->console);

				consoleMenu->view->Invoke("_root.consoleFader_mc.Console_mc.StartExtraInfoData", &returnValue, &arguments, 0);

				TraverseArray(&result, consoleMenu->view, 0, -1, -1,-1, -1);

				//int arraySize;
	
				//DebugMessage(IntToString(result.GetArraySize()));

				consoleMenu->view->Invoke("_root.consoleFader_mc.Console_mc.FinishExtraInfoData", &returnValue, &arguments, 0);
			}
		}

		DebugMessage("GetExtraData: Invoke End");
	}

	//general wrapper for all get form methods
	void GetFormData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm, TESObjectREFR* pRefForm)
	{
		DebugMessage("GetExtraData: Get Form Data Start " + GetFormTypeName(pBaseForm->formType) + " " + FormIDToString(pBaseForm->formID) );

		GetCommonFormData(resultArray, movie, pBaseForm, pRefForm);

		if (pBaseForm != nullptr
			&& pBaseForm->GetFormType() == kFormType_NPC
			&& ( pRefForm == nullptr
			     || pRefForm->GetFormType() == kFormType_Character ) )
		{
			DebugMessage("GetExtraData: Get Form Data character found");
			GetCharacterData(resultArray, movie, pRefForm, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_EffectSetting)
		{
			DebugMessage("GetExtraData: Get Form Data magic effect found");
			GetMagicEffectData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Spell)
		{
			DebugMessage("GetExtraData: Get Form Data spell found");
			GetSpellData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Armor)
		{
			DebugMessage("GetExtraData: Get Form Data armor found");
			GetArmorData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Weapon)
		{
			DebugMessage("GetExtraData: Get Form Data Weapon found");
			GetWeaponData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Ammo)
		{
			DebugMessage("GetExtraData: Get Form Data Ammo found");
			GetAmmoData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Container)
		{
			DebugMessage("GetExtraData: Get Form Data Container found");
			GetContainerData(resultArray, movie, pBaseForm);
		}
		else if (pBaseForm->GetFormType() == kFormType_Faction)
		{
			DebugMessage("GetExtraData: Get Form Data Faction found");
			//GetContainerData(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_Race)
		{
			DebugMessage("GetExtraData: Get Form Data Race found");
			GetRaceEntry(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_TextureSet )
		{
			DebugMessage("GetExtraData: Get Form Data Texture Set found");
			GetTextureSet(resultArray, movie, pBaseForm);
		}

		else if (pBaseForm->GetFormType() == kFormType_ARMA)
		{
			DebugMessage("GetExtraData: Get Form Data ARMA found");
			GetArmaData(resultArray, movie, pBaseForm);
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
					GetEquipment(resultArray, movie, inventoryExtraData, pActor);
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

			GetInventory(resultArray, movie, inventory, pContainer);

		}

		GetModelTextures(resultArray, movie, pBaseForm);

		//Handle BSExtra data
		if (pRefForm != nullptr)
		{
			GetBSExtraData(resultArray, movie, pRefForm);
		}

		//reset any filtering
		MICGlobals::filterARMAByRace = nullptr;

		DebugMessage("GetExtraData: Get Form Data End");
	}

	void CreateExtraInfoEntry(GFxValue * valueArray, GFxMovieView * movie, std::string extraInfoName, std::string extraInfoContents)
	{
		movie->CreateArray(valueArray);

		GFxValue GFxExtraInfoName, GFxExtraInfoContents;

		GFxExtraInfoName.SetString(extraInfoName.c_str());
		GFxExtraInfoContents.SetString(extraInfoContents.c_str());

		valueArray->PushBack(&GFxExtraInfoName);
		valueArray->PushBack(&GFxExtraInfoContents);

	}

	//get data common to all form types
	void GetCommonFormData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm, TESForm* pRefForm)
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

		GFxValue nameArray;
		CreateExtraInfoEntry(&nameArray, movie, "Name", name);

		resultArray->PushBack(&nameArray);

		DebugMessage("GetCommonFormData: Get FormID Start");

		//base formid
		sprintf_s(sResult.get(), MAX_PATH, "%08X", pBaseForm->formID);
		std::string formID = sResult.get();

		GFxValue  formIDArray;
		CreateExtraInfoEntry(&formIDArray, movie, "Base form ID", formID);
		resultArray->PushBack(&formIDArray);

		//base form type
		std::string baseFormType = GetFormTypeName(pBaseForm->GetFormType());

		GFxValue  formTypeEntry;
		CreateExtraInfoEntry(&formTypeEntry, movie, "Base Type", baseFormType);
		resultArray->PushBack(&formTypeEntry);

		//ref formid
		if (pRefForm != nullptr)
		{
			sprintf_s(sResult.get(), MAX_PATH, "%08X", pRefForm->formID);
			std::string refFormID = sResult.get();

			GFxValue  formIDArray;
			CreateExtraInfoEntry(&formIDArray, movie, "Ref form ID", refFormID);
			resultArray->PushBack(&formIDArray);
		}

		//mod location info


		DebugMessage("GetCommonFormData: Get Form Location Start");

		GFxValue formLocationHolder, formLocationData;

		CreateExtraInfoEntry(&formLocationHolder, movie, "Form location information", "");
		GetFormLocationData(&formLocationData, movie, pBaseForm, pRefForm);
		formLocationHolder.PushBack(&formLocationData);
		resultArray->PushBack(&formLocationHolder);

		if (pRefForm != nullptr)
		{
			GetPositionData(resultArray, movie, pRefForm);
		}

		DebugMessage("GetCommonFormData: GetCommonFormData End");

	}

	//get information related to where mods the form is found in
	void GetFormLocationData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm, TESForm* pRefForm)
	{
		DebugMessage("GetExtraData: GetFormLocationData Start");

		movie->CreateArray(resultArray);

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
				if ( ( ( pRefForm->formID & 0xFF000000 ) == 0)
					&& refFirstDefinedIn != "Skyrim.esm")
				{
					refFirstDefinedIn = "Skyrim.esm";
					SkyrimESMNotDetectedBug = true;
				}

				GFxValue referenceDefinedIn;
				CreateExtraInfoEntry(&referenceDefinedIn, movie, "Reference defined in", refFirstDefinedIn);

				resultArray->PushBack(&referenceDefinedIn);

				GFxValue referenceLastChangedBy;
				CreateExtraInfoEntry(&referenceLastChangedBy, movie, "Reference last modified by", refFormModInfo->entries[numModsModifyingRef - 1]->name);

				resultArray->PushBack(&referenceLastChangedBy);

				GFxValue allModsTouchingReferenceHolder;
				CreateExtraInfoEntry(&allModsTouchingReferenceHolder, movie, "Reference found in", "");

				GFxValue allModsTouchingReference;
				GetModInfoData(&allModsTouchingReference, movie, refFormModInfo, SkyrimESMNotDetectedBug);
				allModsTouchingReferenceHolder.PushBack(&allModsTouchingReference);

				resultArray->PushBack(&allModsTouchingReferenceHolder);
			}
		}
		//Base Form

		DebugMessage("GetExtraData: GetFormLocationData at pBaseSection section");

		FormModInfoData *baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

		if (baseFormModInfo != nullptr)
		{
			DebugMessage("GetExtraData: GetFormLocationData baseFormModInfo found");

			int numModsModifyingBase = baseFormModInfo->size;

			GFxValue baseDefinedIn;
			CreateExtraInfoEntry(&baseDefinedIn, movie, "Base defined in", baseFormModInfo->entries[0]->name);

			resultArray->PushBack(&baseDefinedIn);

			GFxValue baseLastChangedBy;
			CreateExtraInfoEntry(&baseLastChangedBy, movie, "Base last modified by", baseFormModInfo->entries[numModsModifyingBase - 1]->name);

			resultArray->PushBack(&baseLastChangedBy);

			GFxValue allModsTouchingBaseHolder;
			CreateExtraInfoEntry(&allModsTouchingBaseHolder, movie, "Base found in", "");

			GFxValue allModsTouchingBase;
			GetModInfoData(&allModsTouchingBase, movie, baseFormModInfo, false);
			allModsTouchingBaseHolder.PushBack(&allModsTouchingBase);

			resultArray->PushBack(&allModsTouchingBaseHolder);
		}

		DebugMessage("GetExtraData: GetFormLocationData End");
	}

	void GetModInfoData(GFxValue * resultArray, GFxMovieView * movie, FormModInfoData* modInfoData, boolean SkyrimESMNotDetectedBug)
	{
		DebugMessage("GetExtraData: GetModInfoData start");

		movie->CreateArray(resultArray);

		int numMods = modInfoData->size;

		if (SkyrimESMNotDetectedBug)
		{
			GFxValue modEntry;

			CreateExtraInfoEntry(&modEntry, movie, "Mod", "Skyrim.esm");
			resultArray->PushBack(&modEntry);
		}

		for (int i = 0; i < numMods; i++)
		{
			GFxValue modEntry;

			CreateExtraInfoEntry(&modEntry, movie, "Mod", modInfoData->entries[i]->name);
			resultArray->PushBack(&modEntry);
		}

		DebugMessage("GetExtraData: GetModInfoData end");
	}

	void GetCharacterData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pRefForm, TESForm* pBaseForm)
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
				GFxValue raceEntry;

				TESRace * pRace = pNPC->race.race;

				MICGlobals::filterARMAByRace = pRace;

				std::string raceName = GetName(pRace);

				CreateExtraInfoEntry( &raceEntry, movie, "Race", raceName);

				GFxValue raceSubEntry;
				movie->CreateArray(&raceSubEntry);
				GetFormData(&raceSubEntry, movie, pRace, nullptr);

				raceEntry.PushBack(&raceSubEntry);
				resultArray->PushBack(&raceEntry);

				DebugMessage("GetCharacterData: Ending Race");

				//Spells
				GFxValue allSpellsEntry;

				CreateExtraInfoEntry(&allSpellsEntry, movie, "Spells", "");

				GFxValue SpellsArray;
				movie->CreateArray(&SpellsArray);

				DebugMessage("GetCharacterData: Starting Added Spells");

				if (pActor)
				{
					//Added Spells
					for (int i = 0; i < pActor->addedSpells.Length(); i++)
					{
						GFxValue spellEntry;

						SpellItem* spell = pActor->addedSpells.Get(i);
						GetSpellDataWrapper(&spellEntry, movie, spell, "Added Spell");

						SpellsArray.PushBack(&spellEntry);
					}
				}

				DebugMessage("GetCharacterData: Starting Base Spells");

				//Actor Base Spells
				int numberOfBaseSpells = pActorBase->spellList.GetSpellCount();

				for (int i = 0; i < numberOfBaseSpells; i++)
				{
					GFxValue spellEntry;

					SpellItem* spell = pActorBase->spellList.GetNthSpell(i);
					GetSpellDataWrapper(&spellEntry, movie, spell, "Base Spell");

					SpellsArray.PushBack(&spellEntry);
				}

				allSpellsEntry.PushBack(&SpellsArray);

				resultArray->PushBack(&allSpellsEntry);

				DebugMessage("GetCharacterData: GetCharacter Done with spells");

				if (pActor)
				{
					// ActiveEffects as Array
					GFxValue activeEffectsEntry;

					CreateExtraInfoEntry(&activeEffectsEntry, movie, "Effects", "");

					GFxValue activeEffectsArray;
					movie->CreateArray(&activeEffectsArray);

					tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();

					DebugMessage("GetCharacterData: Active Effects Gotten");

					if (effects)
					{
						for (int i = 0; i < effects->Count(); i++)
						{
							DebugMessage("GetCharacterData: Starting Active Effect");

							ActiveEffect * pEffect = effects->GetNthItem(i);

							GFxValue effectEntry;

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

								CreateExtraInfoEntry(&effectEntry, movie, effectName, effectActive);

								GFxValue effectEntrySubArray;

								movie->CreateArray(&effectEntrySubArray);

								TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

								if (effectBaseForm)
								{
									DebugMessage("GetCharacterData: Active Effect MGEF base form found");

									GetCommonFormData(&effectEntrySubArray, movie, effectBaseForm, nullptr);

									//Magnitude
									GFxValue magnitudeEntry;

									float magnitude = pEffect->magnitude;
									CreateExtraInfoEntry(&magnitudeEntry, movie, "Magnitude", FloatToString(magnitude));
									effectEntrySubArray.PushBack(&magnitudeEntry);

									//Duration
									GFxValue durationEntry;

									float duration = pEffect->duration;
									CreateExtraInfoEntry(&durationEntry, movie, "Duration", FloatToString(duration));
									effectEntrySubArray.PushBack(&durationEntry);


									//Magnitude
									GFxValue elapsedEntry;

									float elapsed = pEffect->elapsed;
									CreateExtraInfoEntry(&elapsedEntry, movie, "Elapsed", FloatToString(elapsed));
									effectEntrySubArray.PushBack(&elapsedEntry);

									GetMagicEffectData(&effectEntrySubArray, movie, effectBaseForm);

									effectEntry.PushBack(&effectEntrySubArray);
								}
							}

							else
							{
								CreateExtraInfoEntry(&effectEntry, movie, "Unknown Effect Type", "");
							}

							activeEffectsArray.PushBack(&effectEntry);


							DebugMessage("GetCharacterData: Ending Active Effect");

							/*if (pEffect->item)
							scaleformExtend::MagicItemData(&effect, movieView, pEffect->item, bRecursive ? bExtra : false, bRecursive); ??? */

							//RegisterBool(&effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);

							// ActiveEffect
							//if (pEffect->effect && pEffect->effect->mgef)
							//	scaleformExtend::MagicItemData(&effect, movieView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);

							//activeEffects.PushBack(&effect);
						}
					}

					activeEffectsEntry.PushBack(&activeEffectsArray);
					resultArray->PushBack(&activeEffectsEntry);

					DebugMessage("GetExtraData: Active Effects Done");


					GFxValue actorValueHealth;

					GetActorValue(&actorValueHealth, movie, pActor, actorValueHealthIndex);
					resultArray->PushBack(&actorValueHealth);

					GFxValue actorValueMagicka;

					GetActorValue(&actorValueMagicka, movie, pActor, actorValueMagickaIndex);
					resultArray->PushBack(&actorValueMagicka);

					GFxValue actorValueStamina;

					GetActorValue(&actorValueStamina, movie, pActor, actorValueStaminahIndex);
					resultArray->PushBack(&actorValueStamina);

					//Get all actor values in a subarray
					GFxValue actorValueArrayEntry;
					CreateExtraInfoEntry(&actorValueArrayEntry, movie, "Actor Values", "");

					GFxValue actorValueArray;
					movie->CreateArray(&actorValueArray);

					for (int i = 0; i < ActorValueList::kNumActorValues; i++)
					{

						GFxValue actorValue;
						GetActorValue(&actorValue, movie, pActor, i);
						actorValueArray.PushBack(&actorValue);
					}


					actorValueArrayEntry.PushBack(&actorValueArray);
					resultArray->PushBack(&actorValueArrayEntry);

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
							GFxValue packageEntry;

							CreateExtraInfoEntry(&packageEntry, movie, "Current Package", packageName);


							DebugMessage("Before getting package form data");

							GFxValue packageSubArray;
							movie->CreateArray(&packageSubArray);
							GetFormData(&packageSubArray, movie, currentPackage, nullptr);

							packageEntry.PushBack(&packageSubArray);
							resultArray->PushBack(&packageEntry);

						}
					}
					
					DebugMessage("After package");

				} //end of a pActor Section

				//Handle Flags
				int essentialFlag = 0x02;
				int protectedFlag = 0x800;

				GFxValue protectionEntry;


				if ((pNPC->actorData.flags & essentialFlag) == essentialFlag)
				{
					CreateExtraInfoEntry(&protectionEntry, movie, "Protection", "Essential");
				}

				else if ((pNPC->actorData.flags & protectedFlag) == protectedFlag)
				{
					CreateExtraInfoEntry(&protectionEntry, movie, "Protection", "Protected");
				}

				else
				{
					CreateExtraInfoEntry(&protectionEntry, movie, "Protection", "None");
				}

				resultArray->PushBack(&protectionEntry);

				//Level stuff

				if (pActor)
				{

					int level = CALL_MEMBER_FN(pActor, GetLevel)();

					GFxValue levelEntry;

					CreateExtraInfoEntry(&levelEntry, movie, "Level", IntToString(level));
					resultArray->PushBack(&levelEntry);

					DebugMessage("GetExtraData: GetCharacter level gotten");
				}

				GFxValue isPcLeveledEntry;

				bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
				if (isLevelMult)
				{
					DebugMessage("GetExtraData: GetCharacter pc level mult set");


					CreateExtraInfoEntry(&isPcLeveledEntry, movie, "Is PC Level Mult", "True");

					GFxValue levelMultSubArray;
					movie->CreateArray(&levelMultSubArray);

					double levelMult = (double)pNPC->actorData.level / 1000.0;
					int minLevel = pNPC->actorData.minLevel;
					int maxLevel = pNPC->actorData.maxLevel;

					GFxValue levelMultEntry, minLevelEntry, maxLevelEntry;

					CreateExtraInfoEntry(&levelMultEntry, movie, "Level Mult", DoubleToString(levelMult));
					levelMultSubArray.PushBack(&levelMultEntry);

					CreateExtraInfoEntry(&minLevelEntry, movie, "Min level", IntToString(minLevel));
					levelMultSubArray.PushBack(&minLevelEntry);

					CreateExtraInfoEntry(&maxLevelEntry, movie, "Max Level", IntToString(maxLevel));
					levelMultSubArray.PushBack(&maxLevelEntry);

					isPcLeveledEntry.PushBack(&levelMultSubArray);
				}
				else
				{
					DebugMessage("GetExtraData: GetCharacter pc level mult not set");

					CreateExtraInfoEntry(&isPcLeveledEntry, movie, "Is PC Level Mult", "False");
				}

				resultArray->PushBack(&isPcLeveledEntry);

				//Perks
				int numPerks = pActorBase->perkRanks.numPerkRanks;

				DebugMessage("GetExtraData: Starting Perks - Total Number" + IntToString(numPerks));

				GFxValue perks;
				CreateExtraInfoEntry(&perks, movie, "Perks", "");

				GFxValue perkSubArray;
				movie->CreateArray(&perkSubArray);

				for (int i = 0; i < numPerks; i++)
				{
					DebugMessage("GetExtraData: Starting Perk num" + IntToString(i));

					BGSPerk *perk = pActorBase->perkRanks.perkRanks[i].perk;

					if (perk)
					{
						//int rank = pActorBase->perkRanks.perkRanks[i].rank;

						std::string name = GetName(perk);

						GFxValue perkEntry;

						CreateExtraInfoEntry(&perkEntry, movie, name, "");

						GFxValue perkEntrySubArray;
						movie->CreateArray(&perkEntrySubArray);

						GetFormData(&perkEntrySubArray, movie, perk, nullptr);

						//GFxValue perkEntryRank;

						//CreateExtraInfoEntry(&perkEntryRank, movie, "Rank", IntToString(rank) );
						//perkEntrySubArray.PushBack(&perkEntryRank);

						perkEntry.PushBack(&perkEntrySubArray);

						perkSubArray.PushBack(&perkEntry);
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

							GFxValue perkEntry;

							CreateExtraInfoEntry(&perkEntry, movie, name, "");

							GFxValue perkEntrySubArray;
							movie->CreateArray(&perkEntrySubArray);

							GetFormData(&perkEntrySubArray, movie, perk, nullptr);

							//GFxValue perkEntryRank;

							//CreateExtraInfoEntry(&perkEntryRank, movie, "Rank", IntToString(rank));
							//perkEntrySubArray.PushBack(&perkEntryRank);

							perkEntry.PushBack(&perkEntrySubArray);

							perkSubArray.PushBack(&perkEntry);
						}
					}
				}

				perks.PushBack(&perkSubArray);

				resultArray->PushBack(&perks);

				DebugMessage("GetExtraData: Done with perks");



				//apperance - currently height and weight
				DebugMessage("GetExtraData: appearance Started");

				GFxValue appearance;
				CreateExtraInfoEntry(&appearance, movie, "Appearance", "");

				GFxValue appearanceSubArray;
				movie->CreateArray(&appearanceSubArray);

				float weight = pNPC->weight;

				GFxValue weightEntry;

				CreateExtraInfoEntry(&weightEntry, movie, "Weight", FloatToString(weight));
				appearanceSubArray.PushBack(&weightEntry);

				float height = pNPC->height;

				GFxValue heightEntry;

				CreateExtraInfoEntry(&heightEntry, movie, "Height", FloatToString(height));
				appearanceSubArray.PushBack(&heightEntry);

				appearance.PushBack(&appearanceSubArray);

				resultArray->PushBack(&appearance);

				DebugMessage("GetExtraData: appearance Ended");

				DebugMessage("GetExtraData: factions start");

				GFxValue factionsEntry;

				CreateExtraInfoEntry(&factionsEntry, movie, "Factions", "");

				//Factions
				int numFactions = pActorBase->actorData.factions.count;

				if (numFactions > 0)
				{
					GFxValue factionsSubArray;

					movie->CreateArray(&factionsSubArray);

					for (int i = 0; i < numFactions; i++)
					{
						TESActorBaseData::FactionInfo factionInfo = pActorBase->actorData.factions[i];
						TESFaction *faction = factionInfo.faction;

						if (faction)
						{
							GFxValue factionEntry;

							std::string factionName = GetName(faction);
							int rank = factionInfo.rank;

							CreateExtraInfoEntry(&factionEntry, movie, factionName, "Rank: " + IntToString(rank));

							GFxValue factionEntrySubarrray;
							movie->CreateArray(&factionEntrySubarrray);
							GetFormData(&factionEntrySubarrray, movie, faction, nullptr);
							factionEntry.PushBack(&factionEntrySubarrray);

							factionsSubArray.PushBack(&factionEntry);
						}
					}

					factionsEntry.PushBack(&factionsSubArray);
				}

				resultArray->PushBack(&factionsEntry);

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

	void GetActorValue(GFxValue * resultArray, GFxMovieView * movie, Actor * pActor, int id)
	{
		DebugMessage("GetExtraData: GetActover Value Start");

		if (id < ActorValueList::kNumActorValues)
		{
			std::string valueName = GetActorValueName(id);
			float baseValue = pActor->actorValueOwner.GetBase(id);
			float currentValue = pActor->actorValueOwner.GetCurrent(id);
			float maxValue = pActor->actorValueOwner.GetCurrent(id);

			CreateExtraInfoEntry(resultArray, movie, valueName, FloatToString(currentValue));

			//create a subarray for the base  current and maximum

			GFxValue subArray;

			movie->CreateArray(&subArray);

			GFxValue baseValueEntry, currentValueEntry, maxValueEntry;

			CreateExtraInfoEntry(&baseValueEntry, movie, "Base", FloatToString(baseValue));
			CreateExtraInfoEntry(&currentValueEntry, movie, "Current", FloatToString(currentValue));
			CreateExtraInfoEntry(&maxValueEntry, movie, "Max", FloatToString(maxValue));

			subArray.PushBack(&baseValueEntry);
			subArray.PushBack(&currentValueEntry);
			subArray.PushBack(&maxValueEntry);

			resultArray->PushBack(&subArray);
		}

		else
		{
			//the Program will probally crash if we don't create an entry, so add an entry reporting the error in this case
			CreateExtraInfoEntry(resultArray, movie, "Unknown AV id", IntToString(id));
		}

		DebugMessage("GetExtraData: GetActover Value End");
	}

	void GetMagicEffectData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		DebugMessage("GetExtraData: GetMagicEffectData Start");


		EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
		if (pEffectSetting)
		{
			DebugMessage("GetExtraData: GetMagicEffectData Cast succeeded");

			//Magic School
			int skill = pEffectSetting->school();
			GFxValue skillEntry;

			CreateExtraInfoEntry(&skillEntry, movie, "Magic Skill", GetActorValueName(skill));
			resultArray->PushBack(&skillEntry);

			//Minimum Skill Level

			int minimumSkill = pEffectSetting->level();
			GFxValue minimumSkillEntry;

			CreateExtraInfoEntry(&minimumSkillEntry, movie, "Minimum Skill", IntToString(minimumSkill));
			resultArray->PushBack(&minimumSkillEntry);

			//Effect type 

			int effectType = pEffectSetting->properties.archetype;
			GFxValue effectTypeEntry;

			CreateExtraInfoEntry(&effectTypeEntry, movie, "Effect type", GetEffectTypeName(effectType));
			resultArray->PushBack(&effectTypeEntry);

			//First AV
			int primaryType = pEffectSetting->properties.primaryValue;
			GFxValue primaryAVEntry;

			CreateExtraInfoEntry(&primaryAVEntry, movie, "Primary AV", GetActorValueName(primaryType));
			resultArray->PushBack(&primaryAVEntry);

			//Second AV
			int secondaryType = pEffectSetting->properties.secondaryValue;
			GFxValue secondaryAVEntry;

			CreateExtraInfoEntry(&secondaryAVEntry, movie, "Secondary AV", GetActorValueName(secondaryType));
			resultArray->PushBack(&secondaryAVEntry);

			//Resistence
			int resistence = pEffectSetting->properties.resistance;
			GFxValue resistenceEntry;

			CreateExtraInfoEntry(&resistenceEntry, movie, "Primary AV", GetActorValueName(resistence));
			resultArray->PushBack(&resistenceEntry);


			//delivery type
			GFxValue deliveryTypeEntry;

			CreateExtraInfoEntry(&deliveryTypeEntry, movie, "Delivery Type", GetDeliveryTypeName(pEffectSetting->properties.deliveryType));
			resultArray->PushBack(&deliveryTypeEntry);

			//Hostile Flag
			GFxValue hostileEntry;

			bool hostile = (pEffectSetting->properties.flags & EffectSetting::Properties::kEffectType_Hostile) == EffectSetting::Properties::kEffectType_Hostile;

			if (hostile)
			{
				CreateExtraInfoEntry(&hostileEntry, movie, "Hostile", "true");
			}

			else
			{
				CreateExtraInfoEntry(&hostileEntry, movie, "Hostile", "false");
			}
			resultArray->PushBack(&hostileEntry);
			/*



			//RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags); //I added the hostile flag. Need to check what else to add
			RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType); //do these next 3 when I add spells
			RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
			RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
			RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);*/
		}

		DebugMessage("GetExtraData: GetMagicEffectData End");
	}

	void GetSpellData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
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

			GFxValue magicEffectsEntry;

			CreateExtraInfoEntry(&magicEffectsEntry, movie, "Magic Effects", "");

			GFxValue magicEffectsArray;
			movie->CreateArray(&magicEffectsArray);

			int numEffects = pMagicItem->effectItemList.count;

			for (int i = 0; i < numEffects; i++)
			{


				MagicItem::EffectItem * pEffect = pMagicItem->effectItemList[i];

				GFxValue effectEntry;

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

					CreateExtraInfoEntry(&effectEntry, movie, effectName, effectActive);

					GFxValue effectEntrySubArray;

					movie->CreateArray(&effectEntrySubArray);

					TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

					if (effectBaseForm)
					{
						DebugMessage("GetExtraData: Active Effect MGEF base form found");

						GetCommonFormData(&effectEntrySubArray, movie, effectBaseForm, nullptr);

						//Magnitude
						GFxValue magnitudeEntry;

						float magnitude = pEffect->magnitude;
						CreateExtraInfoEntry(&magnitudeEntry, movie, "Magnitude", FloatToString(magnitude));
						effectEntrySubArray.PushBack(&magnitudeEntry);

						//Duration
						GFxValue durationEntry;

						float duration = pEffect->duration;
						CreateExtraInfoEntry(&durationEntry, movie, "Duration", FloatToString(duration));
						effectEntrySubArray.PushBack(&durationEntry);


						//Magnitude
						GFxValue areaEntry;

						float area = pEffect->area;
						CreateExtraInfoEntry(&areaEntry, movie, "Area", FloatToString(area));
						effectEntrySubArray.PushBack(&areaEntry);

						GetMagicEffectData(&effectEntrySubArray, movie, effectBaseForm);

						effectEntry.PushBack(&effectEntrySubArray);
					}
				}

				else
				{
					CreateExtraInfoEntry(&effectEntry, movie, "Unknown Effect Type", "");
				}

				magicEffectsArray.PushBack(&effectEntry);


				DebugMessage("GetSpellData: Ending Active Effect");


			}

			magicEffectsEntry.PushBack(&magicEffectsArray);
			resultArray->PushBack(&magicEffectsEntry);

			DebugMessage("GetSpellData: Done general magic item code");
		}

		SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
		if (pSpellItem)
		{
			DebugMessage("GetSpellData: Starting spell item code");

			//spell type
			GFxValue spellTypeEntry;

			CreateExtraInfoEntry(&spellTypeEntry, movie, "Spell Type", GetSpellTypeName(pSpellItem->data.type));
			resultArray->PushBack(&spellTypeEntry);

			//casting type
			GFxValue castingTypeEntry;

			CreateExtraInfoEntry(&castingTypeEntry, movie, "Cast Type", GetCastingTypeName(pSpellItem->data.castType));
			resultArray->PushBack(&castingTypeEntry);

			//spell cost
			GFxValue spellCostEntry;

			CreateExtraInfoEntry(&spellCostEntry, movie, "True Cost", IntToString(pSpellItem->GetMagickaCost()));
			resultArray->PushBack(&spellCostEntry);

			//cast time
			GFxValue castTimeEntry;

			CreateExtraInfoEntry(&castTimeEntry, movie, "Cast Time", FloatToString(pSpellItem->data.castTime));
			resultArray->PushBack(&castTimeEntry);

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
		GFxValue useSound;
		movieView->CreateObject(&useSound);
		scaleformExtend::FormData(&useSound, movieView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("useSound", &useSound);
		}
		}

		EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
		if (pEnchantItem)
		{
		RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

		GFxValue baseEnchant;
		movieView->CreateObject(&baseEnchant);
		scaleformExtend::FormData(&baseEnchant, movieView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("baseEnchant", &baseEnchant);

		GFxValue restrictions;
		movieView->CreateObject(&restrictions);
		scaleformExtend::FormData(&restrictions, movieView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("restrictions", &restrictions);
		}
		}	*/
	}

	//wrapper for getting both the common form data and the spell data for a spell
	void GetSpellDataWrapper(GFxValue * spellEntry, GFxMovieView * movie, SpellItem* spell, std::string source)
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

			CreateExtraInfoEntry(spellEntry, movie, spellName, source);

			GFxValue spellEntrySubArray;
			movie->CreateArray(&spellEntrySubArray);

			GetCommonFormData(&spellEntrySubArray, movie, spellBaseForm, nullptr);
			GetSpellData(&spellEntrySubArray, movie, spellBaseForm);

			spellEntry->PushBack(&spellEntrySubArray);
		}

		else
		{
			CreateExtraInfoEntry(spellEntry, movie, "Unknown Spell", source);
		}

		DebugMessage("GetSpellDataWrapper: Finished spell");
	}

	void GetEquipment(GFxValue * resultArray, GFxMovieView * movie, ExtraContainerChanges* pContainerChanges, Actor * pActor)
	{
		DebugMessage("GetEquipment: GetEquipment Start");

		GFxValue equipmentEntry;

		CreateExtraInfoEntry(&equipmentEntry, movie, "Equipment", "");

		GFxValue equipmentSubArray;

		movie->CreateArray(&equipmentSubArray);

		//weapons and shouts

		//left hand
		TESForm * leftHand = pActor->GetEquippedObject(true);

		if (leftHand != nullptr)
		{
			GFxValue leftHandEntry;
			std::string name = GetName(leftHand);

			CreateExtraInfoEntry(&leftHandEntry, movie, "Left Hand:", name);

			GFxValue leftHandSubArray;
			movie->CreateArray(&leftHandSubArray);

			GetFormData(&leftHandSubArray, movie, leftHand, nullptr);
			leftHandEntry.PushBack(&leftHandSubArray);
			equipmentSubArray.PushBack(&leftHandEntry);
		}

		//right hand
		TESForm * rightHand = pActor->GetEquippedObject(false);

		if (rightHand != nullptr)
		{
			GFxValue rightHandEntry;
			std::string name = GetName(rightHand);

			CreateExtraInfoEntry(&rightHandEntry, movie, "Right Hand:", name);

			GFxValue rightHandSubArray;
			movie->CreateArray(&rightHandSubArray);

			GetFormData(&rightHandSubArray, movie, rightHand, nullptr);
			rightHandEntry.PushBack(&rightHandSubArray);
			equipmentSubArray.PushBack(&rightHandEntry);
		}

		//shout
		TESForm * shout = pActor->equippedShout;

		if (shout != nullptr)
		{
			GFxValue ShoutEntry;
			std::string name = GetName(shout);

			CreateExtraInfoEntry(&ShoutEntry, movie, "Shout:", name);

			GFxValue shoutSubArray;
			movie->CreateArray(&shoutSubArray);

			GetFormData(&shoutSubArray, movie, shout, nullptr);
			ShoutEntry.PushBack(&shoutSubArray);
			equipmentSubArray.PushBack(&ShoutEntry);
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

				GFxValue equipedItemEntry;
				std::string name = GetName(equipedItem);
				std::string slotName = GetEquipSlotName(i);

				CreateExtraInfoEntry(&equipedItemEntry, movie, slotName, name);

				GFxValue equipedItemEntrySubArray;
				movie->CreateArray(&equipedItemEntrySubArray);

				GetFormData(&equipedItemEntrySubArray, movie, equipedItem, nullptr);
				equipedItemEntry.PushBack(&equipedItemEntrySubArray);
				equipmentSubArray.PushBack(&equipedItemEntry);

			}

			DebugMessage("GetEquipment: Ending EquipSlot item");
		}


		equipmentEntry.PushBack(&equipmentSubArray);

		resultArray->PushBack(&equipmentEntry);

		DebugMessage("GetEquipment: GetEquipment End");
	}

	void GetInventory(GFxValue * resultArray, GFxMovieView * movie, EntryDataList * inventory, TESContainer * baseContainer)
	{
		DebugMessage("GetInventory: GetInventory Start");

		GFxValue inventoryEntry;

		CreateExtraInfoEntry(&inventoryEntry, movie, "Inventory", "");

		GFxValue inventorySubArray;

		movie->CreateArray(&inventorySubArray);

		if (inventory != nullptr
			&& inventory->Count() > MICGlobals::maxInventoryBeforeReducedMode)
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

					GFxValue inventoryItemEntry;

					CreateExtraInfoEntry(&inventoryItemEntry, movie, name, IntToString(itemCount));

					GFxValue inventoryItemEntrySubArray;
					movie->CreateArray(&inventoryItemEntrySubArray);

					GetFormData(&inventoryItemEntrySubArray, movie, itemForm, nullptr);

					inventoryItemEntry.PushBack(&inventoryItemEntrySubArray);

					inventorySubArray.PushBack(&inventoryItemEntry);
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

					GFxValue inventoryItemEntry;

					if (name != "")
					{
						CreateExtraInfoEntry(&inventoryItemEntry, movie, name, IntToString(itemCount));
					}

					else
					{
						CreateExtraInfoEntry(&inventoryItemEntry, movie, name, FormIDToString(itemForm->formID));
					}

					GFxValue inventoryItemEntrySubArray;
					movie->CreateArray(&inventoryItemEntrySubArray);

					GetFormData(&inventoryItemEntrySubArray, movie, itemForm, nullptr);

					inventoryItemEntry.PushBack(&inventoryItemEntrySubArray);

					inventorySubArray.PushBack(&inventoryItemEntry);
				}


				DebugMessage("GetInventory: Ending container item");
			}
		}

		inventoryEntry.PushBack(&inventorySubArray);

		resultArray->PushBack(&inventoryEntry);

		MICGlobals::reducedMode = false;

		DebugMessage("GetInventory: GetInventory End");
	}

	void GetArmaData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		DebugMessage("GetArmaData: GetArmaData Start");

		TESObjectARMA * pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);

		if (pArma)
		{
			DebugMessage("GetArmaData: Before Cast");
			TESForm * maleSkin = (TESForm *) pArma->unk130;
			
			DebugMessage("GetArmaData: After Cast");
			if (maleSkin)
			{
				GFxValue maleSkinEntry;
				CreateExtraInfoEntry(&maleSkinEntry, movie, "Male skin", "");

				GFxValue maleSkinSubEntry;
				movie->CreateArray(&maleSkinSubEntry);

				GetFormData(&maleSkinSubEntry, movie, maleSkin, nullptr);

				maleSkinEntry.PushBack(&maleSkinSubEntry);
				resultArray->PushBack(&maleSkinEntry);
			}


			DebugMessage("GetArmaData: Before Cast");
			TESForm * femaleSkin = (TESForm *)pArma->unk138;

			DebugMessage("GetArmaData: After Cast");
			if (femaleSkin)
			{
				GFxValue maleSkinEntry;
				CreateExtraInfoEntry(&maleSkinEntry, movie, "Female skin", "");

				GFxValue maleSkinSubEntry;
				movie->CreateArray(&maleSkinSubEntry);

				GetFormData(&maleSkinSubEntry, movie, femaleSkin, nullptr);

				maleSkinEntry.PushBack(&maleSkinSubEntry);
				resultArray->PushBack(&maleSkinEntry);
			}

			//races
			
			MICGlobals::readRaceSkins = false; //disable reading race skin entries to avoid infinite loops

			//Primary race
			TESRace * defaultRace = pArma->race.race;
			std::string defaultRaceName = GetName(defaultRace);

			GFxValue defaultRacesEntry;

			CreateExtraInfoEntry(&defaultRacesEntry, movie, "Primary Race", defaultRaceName);

			GFxValue defaultRacesEntrySubArray;

			movie->CreateArray(&defaultRacesEntrySubArray);

			GetFormData(&defaultRacesEntrySubArray, movie, defaultRace, nullptr);
			  
			defaultRacesEntry.PushBack(&defaultRacesEntrySubArray);
			resultArray->PushBack(&defaultRacesEntry);

			//Additional races

			if (pArma->additionalRaces.count > 0)
			{
				GFxValue additionalRacesEntry;

				CreateExtraInfoEntry(&additionalRacesEntry, movie, "Additional Races", "");

				GFxValue additionalRacesEntrySubArray;

				movie->CreateArray(&additionalRacesEntrySubArray);

				for (int i = 0; i < pArma->additionalRaces.count; i++)
				{
					TESRace * additionalRace = pArma->additionalRaces[i];

					if (additionalRace)
					{
						std::string additionalRaceName = GetName(additionalRace);

						GFxValue additionalRaceEntry;

						CreateExtraInfoEntry(&additionalRaceEntry, movie, additionalRaceName, "");

						GFxValue additionalRaceEntrySubArray;

						movie->CreateArray(&additionalRaceEntrySubArray);

						GetFormData(&additionalRaceEntrySubArray, movie, additionalRace, nullptr);

						additionalRaceEntry.PushBack(&additionalRaceEntrySubArray);
						additionalRacesEntrySubArray.PushBack(&additionalRaceEntry);
					}
				}

				additionalRacesEntry.PushBack(&additionalRacesEntrySubArray);

				resultArray->PushBack(&additionalRacesEntry);
			}


			MICGlobals::readRaceSkins = true;

			//resultArray->PushBack(&racesEntry);

			//Skin Textures
			
		}

		DebugMessage("GetArmaData: GetArmaData End");
	}

	void GetArmorData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		TESObjectARMO * pArmor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMO);

		if (pArmor)
		{
			//value
			int armorRating = pArmor->armorValTimes100 / 100;

			GFxValue armorRatingEntry;

			CreateExtraInfoEntry(&armorRatingEntry, movie, "Armor Rating", IntToString(armorRating));
			resultArray->PushBack(&armorRatingEntry);

			//value
			int value = pArmor->value.value;

			GFxValue valueEntry;

			CreateExtraInfoEntry(&valueEntry, movie, "Value", IntToString(value));
			resultArray->PushBack(&valueEntry);

			//weight
			int weight = pArmor->weight.weight;

			GFxValue weightEntry;

			CreateExtraInfoEntry(&weightEntry, movie, "Weight", IntToString(weight));
			resultArray->PushBack(&weightEntry);

			//weight class
			int weightClass = pArmor->bipedObject.data.weightClass;

			GFxValue weightClassEntry;

			CreateExtraInfoEntry(&weightClassEntry, movie, "Armor Type", GetArmorWeightClassName(weightClass));
			resultArray->PushBack(&weightClassEntry);

			//Equip slots
			int parts = pArmor->bipedObject.data.parts;

			GFxValue equipSlotsEntry;

			CreateExtraInfoEntry(&equipSlotsEntry, movie, "Equip Slots", "");

			GFxValue equipSlotsEntrySubArray;
			movie->CreateArray(&equipSlotsEntrySubArray);

			for (int i = 0; i <= 31; i++)
			{
				int mask = 1 << i;

				if ((parts & mask) == mask)
				{

					std::string slotName = GetEquipSlotName(i);

					GFxValue equipSlotEntry;

					CreateExtraInfoEntry(&equipSlotEntry, movie, slotName, "");
					equipSlotsEntrySubArray.PushBack(&equipSlotEntry);
				}
			}

			equipSlotsEntry.PushBack(&equipSlotsEntrySubArray);
			resultArray->PushBack(&equipSlotsEntry);

			if (!MICGlobals::reducedMode)
			{
				GFxValue armorAddonsEntry;

				CreateExtraInfoEntry(&armorAddonsEntry, movie, "Armor Entry", "");

				GFxValue armorAddonsSubArray;
				movie->CreateArray(&armorAddonsSubArray);

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
						GFxValue armorAddonEntry;

						std::string armorAddonName = GetName(pArmor->armorAddons[i]);

						CreateExtraInfoEntry(&armorAddonEntry, movie, armorAddonName, "");

						GFxValue armorAddonSubArray;
						movie->CreateArray(&armorAddonSubArray);

						GetFormData(&armorAddonSubArray, movie, pArmor->armorAddons[i], nullptr);

						armorAddonEntry.PushBack(&armorAddonSubArray);
						armorAddonsSubArray.PushBack(&armorAddonEntry);
					}
				}
			
				armorAddonsEntry.PushBack(&armorAddonsSubArray);
				resultArray->PushBack(&armorAddonsEntry);
			}
		}
	}

	void GetWeaponData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		TESObjectWEAP * pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			//animation type
			int animationType = pWeapon->type();

			GFxValue antimationTypeEntry;

			CreateExtraInfoEntry(&antimationTypeEntry, movie, "Animation type", GetWeaponAnimationTypeName(animationType));
			resultArray->PushBack(&antimationTypeEntry);

			//damage
			int damage = pWeapon->damage.GetAttackDamage();

			GFxValue damageEntry;

			CreateExtraInfoEntry(&damageEntry, movie, "Damage", IntToString(damage));
			resultArray->PushBack(&damageEntry);

			//speed
			float speed = pWeapon->speed();

			GFxValue speedEntry;

			CreateExtraInfoEntry(&speedEntry, movie, "Speed", FloatToString(speed));
			resultArray->PushBack(&speedEntry);

			//reach
			float reach = pWeapon->reach();

			GFxValue reachEntry;

			CreateExtraInfoEntry(&reachEntry, movie, "Reach", FloatToString(reach));
			resultArray->PushBack(&reachEntry);

			//stagger
			float stagger = pWeapon->stagger();

			GFxValue staggerEntry;

			CreateExtraInfoEntry(&staggerEntry, movie, "Stagger", FloatToString(stagger));
			resultArray->PushBack(&staggerEntry);


			//crit damage
			int critDamage = pWeapon->critDamage();

			GFxValue critDamageEntry;

			CreateExtraInfoEntry(&critDamageEntry, movie, "Crit damage", IntToString(critDamage));
			resultArray->PushBack(&critDamageEntry);

			/*RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
			RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange()); Data for npc AI*/

			BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
			if (equipSlot)
			{
				std::string equipSlotName = GetEquipTypeName(equipSlot->formID);

				GFxValue equipSlotEntry;

				CreateExtraInfoEntry(&equipSlotEntry, movie, "Equip slot", equipSlotName);
				resultArray->PushBack(&equipSlotEntry);
			}

			//value
			int value = pWeapon->value.value;

			GFxValue valueEntry;

			CreateExtraInfoEntry(&valueEntry, movie, "Value", IntToString(value));
			resultArray->PushBack(&valueEntry);

			//weight
			int weight = pWeapon->weight.weight;

			GFxValue weightEntry;

			CreateExtraInfoEntry(&weightEntry, movie, "Weight", IntToString(weight));
			resultArray->PushBack(&weightEntry);

		}
	}

	void GetAmmoData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		TESAmmo * pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
		if (pAmmo)
		{
			//damage
			float damage = pAmmo->settings.damage;

			GFxValue damageEntry;

			CreateExtraInfoEntry(&damageEntry, movie, "Damage", FloatToString(damage));
			resultArray->PushBack(&damageEntry);


			//value
			int value = pAmmo->value.value;

			GFxValue valueEntry;

			CreateExtraInfoEntry(&valueEntry, movie, "Value", IntToString(value));
			resultArray->PushBack(&valueEntry);

		}
	}

	void GetContainerData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		TESObjectCONT * pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			int respawnsFlag = 0x02;

			GFxValue safeContainerEntry;

			if ((pContainer->unkB9 & respawnsFlag) == respawnsFlag)
			{
				CreateExtraInfoEntry(&safeContainerEntry, movie, "Safe Container", "No");
			}

			else
			{
				CreateExtraInfoEntry(&safeContainerEntry, movie, "Safe Container", "Yes");
			}

			resultArray->PushBack(&safeContainerEntry);
		}
	}

	void GetPositionData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pRefForm)
	{
		DebugMessage("Starting GetPositionData");

		TESObjectREFR * pRef = DYNAMIC_CAST(pRefForm, TESForm, TESObjectREFR);

		if (pRef)
		{
			GFxValue positionEntry;
			CreateExtraInfoEntry(&positionEntry, movie, "Position", "");

			GFxValue positionSubArray;
			movie->CreateArray(&positionSubArray);

			//position
			float xPos = pRef->pos.x;
			GFxValue xPositionEntry;
			CreateExtraInfoEntry(&xPositionEntry, movie, "X Position", FloatToString(xPos));
			positionSubArray.PushBack(&xPositionEntry);

			float yPos = pRef->pos.y;
			GFxValue yPositionEntry;
			CreateExtraInfoEntry(&yPositionEntry, movie, "Y Position", FloatToString(yPos));
			positionSubArray.PushBack(&yPositionEntry);

			float zPos = pRef->pos.z;
			GFxValue zPositionEntry;
			CreateExtraInfoEntry(&zPositionEntry, movie, "Z Position", FloatToString(zPos));
			positionSubArray.PushBack(&zPositionEntry);

			//rotation
			float xRot = pRef->rot.x;
			GFxValue xRotationEntry;
			CreateExtraInfoEntry(&xRotationEntry, movie, "X Rotation", FloatToString(xRot));
			positionSubArray.PushBack(&xRotationEntry);

			float yRot = pRef->rot.y;
			GFxValue yRotationEntry;
			CreateExtraInfoEntry(&yRotationEntry, movie, "Y Rotation", FloatToString(yRot));
			positionSubArray.PushBack(&yRotationEntry);

			float zRot = pRef->rot.z;
			GFxValue zRotationEntry;
			CreateExtraInfoEntry(&zRotationEntry, movie, "Z Rotation", FloatToString(zRot));
			positionSubArray.PushBack(&zRotationEntry);

			positionEntry.PushBack(&positionSubArray);
			resultArray->PushBack(&positionEntry);
		}

		DebugMessage("Ending GetPositionData");
	}


	//get data stored in the BSExtraData format
	void GetBSExtraData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pRefForm)
	{
		DebugMessage("Starting GetBSExtraData");

		TESObjectREFR * pRef = DYNAMIC_CAST(pRefForm, TESForm, TESObjectREFR);

		if (pRef)
		{
			BaseExtraList *extraList = &pRef->extraData;

			if(MICOptions::MICDebugMode)
			{
				//Placeholder for seeing what has editor IDs
				GFxValue editorIDEntry;

				if (extraList->HasType(kExtraData_EditorID))
				{
					CreateExtraInfoEntry(&editorIDEntry, movie, "Editor ID", "Yes");
				}
				else
				{
					CreateExtraInfoEntry(&editorIDEntry, movie, "Editor ID", "No");
				}

				resultArray->PushBack(&editorIDEntry);
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
					GFxValue ownershipEntry;

					CreateExtraInfoEntry(&ownershipEntry, movie, "Owner", ownerName);


					DebugMessage("Before getting owner form data");

					GFxValue ownershipSubarray;
					movie->CreateArray(&ownershipSubarray);
					GetFormData(&ownershipSubarray, movie, owner, nullptr);

					ownershipEntry.PushBack(&ownershipSubarray);
					resultArray->PushBack(&ownershipEntry);

				}


				DebugMessage("Ending kExtraData_Ownership");
			}
			

			if (MICOptions::MICDebugMode)
			{
				//Placeholder for seeing what has editor IDs
				GFxValue editorIDEntry;

				if (extraList->HasType(kExtraData_Package))
				{
					CreateExtraInfoEntry(&editorIDEntry, movie, "Extra Data Package", "Yes");
				}
				else
				{
					CreateExtraInfoEntry(&editorIDEntry, movie, "Extra Data Package", "No");
				}

				resultArray->PushBack(&editorIDEntry);
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
					GFxValue packageEntry;

					CreateExtraInfoEntry(&packageEntry, movie, "Current Package", packageName);


					DebugMessage("Before getting packag form data");

					GFxValue packageSubArray;
					movie->CreateArray(&packageSubArray);
					GetFormData(&packageSubArray, movie, currentPackage, nullptr);

					packageEntry.PushBack(&packageSubArray);
					resultArray->PushBack(&packageEntry);

				}


				DebugMessage("Ending kExtraData_Package");
			}*/
		}




		DebugMessage("Ending GetBSExtraData");
	}
	/*
	void StandardItemData(GFxValue * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
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

	void GetModelTextures(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm)
	{
		DebugMessage("Starting GetModelTextures " + GetFormTypeName( pBaseForm->formType) );
		if (pBaseForm->formType == kFormType_Static)
		{
			TESObjectSTAT *pStatic = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectSTAT);

			if (pStatic)
			{
				TESModelTextureSwap * textSwap = &(pStatic->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_MovableStatic)
		{
			BGSMovableStatic *pMoveStatic = DYNAMIC_CAST(pBaseForm, TESForm, BGSMovableStatic);

			if (pMoveStatic)
			{
				TESModelTextureSwap * textSwap = &(pMoveStatic->staticObj.texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Activator)
		{
			TESObjectACTI *pActivate = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectACTI);

			if (pActivate)
			{
				TESModelTextureSwap * textSwap = &(pActivate->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Tree)
		{
			TESObjectTREE *pTree = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectTREE);

			if (pTree)
			{
				TESModel * model = &(pTree->model);
				AddModelEntry(resultArray, movie, "Model", model);
			}
		}

		else if (pBaseForm->formType == kFormType_Flora)
		{
			TESFlora *pFlora = DYNAMIC_CAST(pBaseForm, TESForm, TESFlora);

			if (pFlora)
			{
				TESModelTextureSwap * textSwap = &(pFlora->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Furniture)
		{
			TESFurniture *pFurniture = DYNAMIC_CAST(pBaseForm, TESForm, TESFurniture);

			if (pFurniture)
			{
				TESModelTextureSwap * textSwap = &(pFurniture->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Door)
		{
			TESObjectDOOR *pDoor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectDOOR);
			if (pDoor)
			{
				TESModelTextureSwap * textSwap = &(pDoor->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Container)
		{
			TESObjectCONT *pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
			if (pContainer)
			{
				TESModelTextureSwap * textSwap = &(pContainer->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}
		
		else if (pBaseForm->formType == kFormType_Misc)
		{
			TESObjectMISC *pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
			if (pMisc)
			{
				TESModelTextureSwap * textSwap = &(pMisc->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Book)
		{
			TESObjectBOOK *pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
			if (pBook)
			{
				TESModelTextureSwap * textSwap = &(pBook->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Key )
		{
			TESKey *pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
			if (pKey)
			{
				TESModelTextureSwap * textSwap = &(pKey->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_SoulGem)
		{
			TESSoulGem *pSoulGem = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
			if (pSoulGem)
			{
				TESModelTextureSwap * textSwap = &(pSoulGem->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Ingredient)
		{
			IngredientItem *pIngredient = DYNAMIC_CAST(pBaseForm, TESForm, IngredientItem);
			if (pIngredient)
			{
				TESModelTextureSwap * textSwap = &(pIngredient->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Potion)
		{
			AlchemyItem *pAlchemy = DYNAMIC_CAST(pBaseForm, TESForm, AlchemyItem);
			if (pAlchemy)
			{
				TESModelTextureSwap * textSwap = &(pAlchemy->texSwap);
				AddModelEntry(resultArray, movie, "Model", textSwap);
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
					AddModelEntry(resultArray, movie, "Model", textSwap);
				}

				TESObjectSTAT * firstPersonModel = pWeapon->model;

				if (firstPersonModel)
				{
					TESModelTextureSwap * textSwapFirstPerson = &(firstPersonModel->texSwap);

					if (textSwapFirstPerson)
					{
						AddModelEntry(resultArray, movie, "First Person Model", textSwapFirstPerson);
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
					AddModelEntry(resultArray, movie, "Model Male", maleModel);
				}

				TESModelTextureSwap * femaleModel = &(pArma->models[0][1]);
				if (femaleModel)
				{
					AddModelEntry(resultArray, movie, "Model Female", femaleModel);
				}

				TESModelTextureSwap * maleFirstPerson = &(pArma->models[1][0]);
				if (maleFirstPerson)
				{
					AddModelEntry(resultArray, movie, "Model Male 1st Person", maleFirstPerson);
				}

				TESModelTextureSwap * femaleFirstPerson = &(pArma->models[1][1]);
				if (femaleFirstPerson)
				{
					AddModelEntry(resultArray, movie, "Model Female 1st Person", femaleFirstPerson);
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

	void AddModelEntry(GFxValue * resultArray, GFxMovieView * movie, std::string modelType, TESModelTextureSwap * modelTextureSwap)
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

				GFxValue modelTextureEntry;

				std::string modelPath = model->GetModelName();
				std::string modelName = GetFileName(modelPath);

				CreateExtraInfoEntry(&modelTextureEntry, movie, modelType, modelName);

				GFxValue modelTextureSubArray;
				movie->CreateArray(&modelTextureSubArray);

				AddModelEntry(&modelTextureSubArray, movie, "Model", model);

				DebugMessage("Starting Texture Set Info");
				GFxValue textureSetEntry;

				CreateExtraInfoEntry(&textureSetEntry, movie, "Texture Set", "");

				GFxValue textureSetEntrySubArray;
				movie->CreateArray(&textureSetEntrySubArray);

				GetFormData(&textureSetEntrySubArray, movie, textureSet, nullptr);

				textureSetEntry.PushBack(&textureSetEntrySubArray);

				modelTextureSubArray.PushBack(&textureSetEntry);

				modelTextureEntry.PushBack(&modelTextureSubArray);

				resultArray->PushBack(&modelTextureEntry);
			}

			else
			{
				DebugMessage("Starting No texture set branch");
				AddModelEntry(resultArray, movie, modelType, model);
			}
		}

		DebugMessage("Ending AddModelEntry for modelTextureSwap");
	}

	void AddModelEntry(GFxValue * resultArray, GFxMovieView * movie, std::string modelType, TESModel * model )
	{
		DebugMessage("Starting AddModelEntry for model");

		if (model)
		{
			std::string modelPath;
			modelPath.assign( model->GetModelName() );

			if (modelPath != "")
			{
				DebugMessage("Get Model path");

				std::string modelName = GetFileName(modelPath);

				DebugMessage("Get Model name");

				GFxValue modelEntry;
				CreateExtraInfoEntry(&modelEntry, movie, modelType, modelName);

				DebugMessage("Splitting Model Path");


				CreateFilePathSubarray(&modelEntry, movie, modelPath);

				DebugMessage("Done Splitting Model Path");

				resultArray->PushBack(&modelEntry);
			}
		}

		DebugMessage("Ending AddModelEntry for model");
	}

	void GetTextureSet(GFxValue * resultArray, GFxMovieView * movie, TESForm * pBaseForm )
	{
		DebugMessage("Starting AddTextureSetEntry");

		BGSTextureSet * pTextureSet = DYNAMIC_CAST(pBaseForm, TESForm, BGSTextureSet);

		if (pTextureSet)
		{

			GFxValue textureSetEntry;
			CreateExtraInfoEntry(&textureSetEntry, movie, "Texture Set", "");

			GFxValue textureSetSubArray;

			movie->CreateArray(&textureSetSubArray);

			for (int i = 0; i < BGSTextureSet::kNumTextures; i++ )
			{
				TESTexture * texture = &pTextureSet->texturePaths[i];
				std::string texturePath = texture->str;

				std::string textureName = "";

				if (texturePath != "")
				{
					textureName = GetFileName(texturePath);
				}

				std::string textureType = GetTextureType(i);

				GFxValue textureEntry;

				CreateExtraInfoEntry(&textureEntry, movie, textureType, textureName);

				if (texturePath != "")
				{
					CreateFilePathSubarray(&textureEntry, movie, texturePath);
				}

				textureSetSubArray.PushBack(&textureEntry);
			}

			textureSetEntry.PushBack(&textureSetSubArray);
			resultArray->PushBack(&textureSetEntry);
		}

		DebugMessage("Ending AddTextureSetEntry");
	}

	void CreateFilePathSubarray(GFxValue * mainEntry, GFxMovieView * movie, std::string filePath)
	{
		DebugMessage("Starting CreateFilePathSubarray " + filePath);

		GFxValue filePathSubArray;
		movie->CreateArray(&filePathSubArray);

		//loop through the string until the last slash is going
		int firstSlash = filePath.find_first_of(deliminator);
		while (firstSlash != std::string::npos)
		{
			//DebugMessage("Splitting with index: " + IntToString(firstSlash) + " with remaining path " + modelPath);

			GFxValue pathEntry;
			std::string path = filePath.substr(0, firstSlash + 1);

			CreateExtraInfoEntry(&pathEntry, movie, path, "");
			filePathSubArray.PushBack(&pathEntry);

			filePath = filePath.substr(firstSlash + 1); //get everything after the first slash
			firstSlash = filePath.find_first_of(deliminator); //refind the first slash
		}

		GFxValue pathEntry;
		//add everything after the last slash
		CreateExtraInfoEntry(&pathEntry, movie, filePath, "");

		filePathSubArray.PushBack(&pathEntry);

		mainEntry->PushBack(&filePathSubArray);

		DebugMessage("Ending CreateFilePathSubarray");
	}

	void GetRaceEntry(GFxValue * resultArray, GFxMovieView * movie, TESForm * pBaseForm)
	{
		DebugMessage("Starting GetRaceEntry");

		TESRace * pRace = DYNAMIC_CAST(pBaseForm, TESForm, TESRace);
		if (pRace)
		{
			DebugMessage("Getting Editor ID");

			//editor ID
			std::string editorID = pRace->editorId.Get();

			GFxValue editorIDEntry;

			CreateExtraInfoEntry(&editorIDEntry, movie, "EditorID", editorID);
			resultArray->PushBack(&editorIDEntry);


			DebugMessage("Getting Models");
			//models
			TESModel * maleModel = &(pRace->models[0]);
			TESModel * femaleModel = &(pRace->models[1]);

			AddModelEntry(resultArray, movie, "Male Skeleton", maleModel);
			AddModelEntry(resultArray, movie, "Female Skeleton", femaleModel);

			//Skins
			if (MICGlobals::readRaceSkins)
			{
				DebugMessage("Getting Skin");
				TESObjectARMO *skin = pRace->skin.skin;

				std::string skinName = GetName(skin);

				GFxValue skinEntry;

				CreateExtraInfoEntry(&skinEntry, movie, "Skin", skinName);

				GFxValue skinSubEntry;

				movie->CreateArray(&skinSubEntry);

				GetFormData(&skinSubEntry, movie, skin, nullptr);

				skinEntry.PushBack(&skinSubEntry);
				resultArray->PushBack(&skinEntry);

				DebugMessage("Done Getting Skin");
			}

			//Handle Flags
			int playableFlag = 0x00000001;
			int childFlag = 0x00000004;

			GFxValue playableEntry;

			if ((pRace->data.raceFlags & playableFlag) == playableFlag)
			{
				CreateExtraInfoEntry(&playableEntry, movie, "Playable", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(&playableEntry, movie, "Playable", "No");
			}

			resultArray->PushBack(&playableEntry);

			GFxValue childEntry;

			if ((pRace->data.raceFlags & childFlag) == childFlag)
			{
				CreateExtraInfoEntry(&childEntry, movie, "Child", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(&childEntry, movie, "Child", "No");
			}

			resultArray->PushBack(&childEntry);
		}

		DebugMessage("Ending GetRaceEntry");
	}

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

				resultArray.PushBack(&zeroIndex);

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

					resultArray.PushBack(&oneIndex);

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

						resultArray.PushBack(&twoIndex);

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

							resultArray.PushBack(&threeIndex);

							if (level == 4)
							{

								GFxValue fourIndex;
								threeIndex.SetNumber(i);
								resultArray.PushBack(&fourIndex);
							}
						}
					}
				}

				resultArray.PushBack(&arrayElement);

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
	}
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

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks(GFxMovieView * view, GFxValue * root)
{
	DebugMessage("Install Hooks Called");

	RegisterFunction <MICScaleform_GetReferenceInfo>(root, view, "MICScaleform_GetReferenceInfo");
	RegisterFunction <MICScaleform_GetExtraData>(root, view, "MICScaleform_GetExtraData");
	RegisterFunction <MICScaleform_GetIniOptions>(root, view, "MICScaleform_GetIniOptions");

	return true;
}