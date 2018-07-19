#include "MIC_Scaleform.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include "Util.h"
#include <memory>
#include <vector>
#include <skse64\GameData.h>


std::vector<std::string> FormTypes =
{
	"NONE",
	"TES4",
	"GRUP",
	"GMST",
	"KYWD",
	"LCRT",
	"AACT",
	"TXST",
	"MICN",
	"GLOB",
	"CLAS",
	"FACT",
	"HDPT",
	"EYES",
	"RACE",
	"SOUN",
	"ASPC",
	"SKIL",
	"MGEF",
	"SCPT",
	"LTEX",
	"ENCH",
	"SPEL",
	"SCRL",
	"ACTI",
	"TACT",
	"ARMO",
	"BOOK",
	"CONT",
	"DOOR",
	"INGR",
	"LIGH",
	"MISC",
	"APPA",
	"STAT",
	"SCOL",
	"MSTT",
	"GRAS",
	"TREE",
	"FLOR",
	"FURN",
	"WEAP",
	"AMMO",
	"NPC_",
	"LVLN",
	"KEYM",
	"ALCH",
	"IDLM",
	"NOTE",
	"COBJ",
	"PROJ",
	"HAZD",
	"SLGM",
	"LVLI",
	"WTHR",
	"CLMT",
	"SPGD",
	"RFCT",
	"REGN",
	"NAVI",
	"CELL",
	"REFR",
	"ACHR",
	"PMIS",
	"PARW",
	"PGRE",
	"PBEA",
	"PFLA",
	"PCON",
	"PBAR",
	"PHZD",
	"WRLD",
	"LAND",
	"NAVM",
	"TLOD",
	"DIAL",
	"INFO",
	"QUST",
	"IDLE",
	"PACK",
	"CSTY",
	"LSCR",
	"LVSP",
	"ANIO",
	"WATR",
	"EFSH",
	"TOFT",
	"EXPL",
	"DEBR",
	"IMGS",
	"IMAD",
	"FLST",
	"PERK",
	"BPTD",
	"ADDN",
	"AVIF",
	"CAMS",
	"CPTH",
	"VTYP",
	"MATT",
	"IPCT",
	"IPDS",
	"ARMA",
	"ECZN",
	"LCTN",
	"MESH",
	"RGDL",
	"DOBJ",
	"LGTM",
	"MUSC",
	"FSTP",
	"FSTS",
	"SMBN",
	"SMQN",
	"SMEN",
	"DLBR",
	"MUST",
	"DLVW",
	"WOOP",
	"SHOU",
	"EQUP",
	"RELA",
	"SCEN",
	"ASTP",
	"OTFT",
	"ARTO",
	"MATO",
	"MOVT",
	"SNDR",
	"DUAL",
	"SNCT",
	"SOPM",
	"COLL",
	"CLFM",
	"REVB",
	"LENS",
	"LSPR",
	"VOLI",
	"Unknown8A",
	"Alias",
	"ReferenceAlias",
	"LocAlias",
	"ActiveMagicEffect"
};

const int actorValueHealthIndex = 24;
const int actorValueMagickaIndex = 25;
const int actorValueStaminahIndex = 26;

class MICScaleform_GetReferenceInfo : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{

		std::string refFormFromMod, refFormLastChangedBy;
		int numModsModifyingRef;

		DebugMessage("GetReferenceInfo: Called");
		ASSERT(args->numArgs >= 1);

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
				RegisterString(args->result, args->movie, "refFormType", FormTypes[pRef->formType].c_str());

				const char* pRefName = CALL_MEMBER_FN(pRef, GetReferenceName)();
				RegisterString(args->result, args->movie, "referenceName", pRefName);
				FormModInfoData* refFormModInfo = (FormModInfoData*)pRef->unk08;

				if (refFormModInfo != nullptr)
				{
					DebugMessage("GetReferenceInfo: Inside Ref modInfo");
					numModsModifyingRef = refFormModInfo->size;

					refFormFromMod = refFormModInfo->entries[0]->name;
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
				RegisterString(args->result, args->movie, "baseFormType", FormTypes[pBaseForm->formType].c_str());



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

		movie->CreateArray(args->result);

		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
		if (pRef != nullptr)
		{
			DebugMessage("GetExtraData: pRefFound");

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				DebugMessage("GetExtraData: BaseFound");
				GetFormData(args->result, movie, pBaseForm, pRef);
			}
		}

		DebugMessage("GetExtraData: Invoke End");
	}

	//general wrapper for all get form methods
	void GetFormData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm, TESForm* pRefForm)
	{
		DebugMessage("GetExtraData: Get Form Data Start");
		
		GetCommonFormData(resultArray, movie, pBaseForm, pRefForm);

		if (pBaseForm != nullptr 
			&& pBaseForm->GetFormType() == kFormType_NPC
			&& pRefForm != nullptr
			&& pRefForm->GetFormType() == kFormType_Character)
		{
			DebugMessage("GetExtraData: Get Form Data character found");
			GetCharacterData(resultArray, movie, pRefForm, pBaseForm);
		}

		if (pBaseForm->GetFormType() == kFormType_EffectSetting )
		{
			DebugMessage("GetExtraData: Get Form Data magic effect found");
			GetCharacterData(resultArray, movie, pRefForm, pBaseForm);
		}

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
		DebugMessage("GetExtraData: GetCommonFormData Start");

		std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);

		//name
		std::string name = GetName(pBaseForm);

		GFxValue nameArray;
		CreateExtraInfoEntry(&nameArray, movie, "Name", name);

		resultArray->PushBack(&nameArray);

		//formid
		sprintf_s(sResult.get(), MAX_PATH, "%08X", pBaseForm->formID);
		std::string formID = sResult.get();

		GFxValue  formIDArray;
		CreateExtraInfoEntry(&formIDArray, movie, "Base form ID", formID);
		resultArray->PushBack(&formIDArray);

		//mod location info

		GFxValue formLocationHolder, formLocationData;

		CreateExtraInfoEntry(&formLocationHolder, movie, "Form location information", "");
		GetFormLocationData(&formLocationData, movie, pBaseForm, pRefForm);
		formLocationHolder.PushBack(&formLocationData);
		resultArray->PushBack(&formLocationHolder);

		DebugMessage("GetExtraData: GetCommonFormData End");

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

				GFxValue referenceDefinedIn;
				CreateExtraInfoEntry(&referenceDefinedIn, movie, "Reference defined in", refFormModInfo->entries[0]->name);

				resultArray->PushBack(&referenceDefinedIn);

				GFxValue referenceLastChangedBy;
				CreateExtraInfoEntry(&referenceLastChangedBy, movie, "Reference last modified by", refFormModInfo->entries[numModsModifyingRef - 1]->name);

				resultArray->PushBack(&referenceLastChangedBy);

				GFxValue allModsTouchingReferenceHolder;
				CreateExtraInfoEntry(&allModsTouchingReferenceHolder, movie, "Reference found in", "");

				GFxValue allModsTouchingReference;
				GetModInfoData(&allModsTouchingReference, movie, refFormModInfo);
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
			GetModInfoData(&allModsTouchingBase, movie, baseFormModInfo);
			allModsTouchingBaseHolder.PushBack(&allModsTouchingBase);

			resultArray->PushBack(&allModsTouchingBaseHolder);
		}

		DebugMessage("GetExtraData: GetFormLocationData End");
	}

	std::string GetName(TESForm* pBaseForm)
	{
		DebugMessage("GetExtraData: GetName Start");

		std::string name = "";

		switch (pBaseForm->GetFormType())
		{
			case kFormType_NPC:
			{
				DebugMessage("GetExtraData: GetName NPC");

				TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);
				if (pNPC)
				{
					if (pNPC->fullName.name.data)
					{
						name = pNPC->fullName.name.data;
					}
				}

				break;
			}

			case kFormType_EffectSetting:
			{
				DebugMessage("GetExtraData: GetName Magic Effect");

				EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
				if (pEffectSetting)
				{
					if (pEffectSetting->fullName.name.data)
					{
						name = pEffectSetting->fullName.name.data;
					}
				}
			}

			case kFormType_Spell:
			case kFormType_ScrollItem:
			case kFormType_Ingredient:
			case kFormType_Potion:
			case kFormType_Enchantment:
			{
				MagicItem * pMagicItem = DYNAMIC_CAST(pBaseForm, TESForm, MagicItem);
				if (pMagicItem)
				{
					if (pMagicItem->fullName.name.data)
					{
						name = pMagicItem->fullName.name.data;
					}
				}

			}
		}

		DebugMessage("GetExtraData: GetName End");

		return name;
	}

	void GetModInfoData(GFxValue * resultArray, GFxMovieView * movie, FormModInfoData* modInfoData)
	{
		DebugMessage("GetExtraData: GetModInfoData start");

		movie->CreateArray(resultArray);

		int numMods = modInfoData->size;

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

		Actor * pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);
		TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);
		if (pActor && pNPC)
		{
			TESActorBase *pActorBase = DYNAMIC_CAST(pNPC, TESNPC, TESActorBase);

			if (pActorBase)
			{
				DebugMessage("GetCharacterData: GetCharacter info casts worked");

				//Spells
				GFxValue allSpellsEntry;

				CreateExtraInfoEntry(&allSpellsEntry, movie, "Spells", "");

				GFxValue SpellsArray;
				movie->CreateArray(&SpellsArray);

				DebugMessage("GetCharacterData: Starting Added Spells");

				//Added Spells
				for (int i = 0; i < pActor->addedSpells.Length(); i++)
				{
					GFxValue spellEntry;

					SpellItem* spell = pActor->addedSpells.Get(i);
					GetSpellDataWrapper(&spellEntry, movie, spell, "Added Spell");

					SpellsArray.PushBack(&spellEntry);
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


				//Level stuff

				const int level = CALL_MEMBER_FN(pActor, GetLevel)();

				GFxValue levelEntry;

				CreateExtraInfoEntry(&levelEntry, movie, "Level", IntToString(level));
				resultArray->PushBack(&levelEntry);

				DebugMessage("GetExtraData: GetCharacter level gotten");


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
		DebugMessage("GetExtraData: GetActover Value Start" );

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

			CreateExtraInfoEntry( &baseValueEntry, movie, "Base", FloatToString(baseValue) );
			CreateExtraInfoEntry( &currentValueEntry, movie, "Current", FloatToString(currentValue) );
			CreateExtraInfoEntry( &maxValueEntry, movie, "Max", FloatToString(maxValue) );

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

	void GetMagicEffectData( GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm )
	{
		DebugMessage("GetExtraData: GetMagicEffectData Start");


		EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
		if (pEffectSetting)
		{
			DebugMessage("GetExtraData: GetMagicEffectData Cast succeeded");

			//Magic School
			int skill = pEffectSetting->school();
			GFxValue skillEntry;

			CreateExtraInfoEntry(&skillEntry, movie, "Magic Skill", GetActorValueName(skill) );
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

	void GetSpellData( GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm )
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

					}
					/*
					SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
					if (pSpellItem)
					{
						RegisterNumber(pFxVal, "spellType", pSpellItem->data.type);
						RegisterNumber(pFxVal, "trueCost", pSpellItem->GetMagickaCost());

						BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
						if (equipSlot)
							RegisterNumber(pFxVal, "equipSlot", equipSlot->formID);
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
};


//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks(GFxMovieView * view, GFxValue * root)
{
	DebugMessage("Install Hooks Called");

	RegisterFunction <MICScaleform_GetReferenceInfo>(root, view, "MICScaleform_GetReferenceInfo");
	RegisterFunction <MICScaleform_GetExtraData>(root, view, "MICScaleform_GetExtraData");

	return true;
}