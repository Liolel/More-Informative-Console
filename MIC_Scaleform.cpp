#include "MIC_Scaleform.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include <memory>
#include <vector>
#include <skse64\GameData.h>
#include <sstream>

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

		//_MESSAGE("Get Reference Info Called");
		ASSERT(args->numArgs >= 1);

		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
		if (pRef != nullptr)
		{
			//_MESSAGE("pRef passed");
			args->movie->CreateObject(args->result);
			std::unique_ptr<char[]>	sResult(new char[MAX_PATH]);

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				//_MESSAGE("pBase passed");
				sprintf_s(sResult.get(), MAX_PATH, "%08X", pRef->formID);
				RegisterString(args->result, args->movie, "refFormID", sResult.get());
				RegisterString(args->result, args->movie, "refFormType", FormTypes[pRef->formType].c_str());

				const char* pRefName = CALL_MEMBER_FN(pRef, GetReferenceName)();
				RegisterString(args->result, args->movie, "referenceName", pRefName);
				FormModInfoData* refFormModInfo = (FormModInfoData*)pRef->unk08;

				if (refFormModInfo != nullptr)
				{
					//_MESSAGE("Inside Ref modInfo");
					int numModsModifyingRef = refFormModInfo->size;

					std::string refFormFromMod = refFormModInfo->entries[0]->name;
					std::string refFormLastChangedBy = refFormModInfo->entries[numModsModifyingRef - 1]->name;

					RegisterString(args->result, args->movie, "refFormDefinedIn", refFormFromMod.c_str());
					RegisterString(args->result, args->movie, "refFormLastChangedBy", refFormLastChangedBy.c_str());


				}

				//_MESSAGE("Before FF actor if");
				if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
				{
					//_MESSAGE("Inside FF actor if");
					TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

					if (pBaseActor)
					{
						//_MESSAGE("Inside second FF actor if");
						TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

						if (pBaseActorTemplate)
						{
							pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
						}
					}
				}

				//_MESSAGE("In non FF actor base form");
				sprintf_s(sResult.get(), MAX_PATH, "%08X", pBaseForm->formID);
				RegisterString(args->result, args->movie, "baseFormID", sResult.get());
				RegisterString(args->result, args->movie, "baseFormType", FormTypes[pBaseForm->formType].c_str());



				//_MESSAGE("Before modInfo");
				FormModInfoData* baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

				if (baseFormModInfo != nullptr)
				{

					//_MESSAGE("Inside Base modInfo");
					int numModsModifyingBase = baseFormModInfo->size;

					std::string baseFormFromMod = baseFormModInfo->entries[0]->name;
					std::string baseFormLastChangedBy = baseFormModInfo->entries[numModsModifyingBase - 1]->name;

					RegisterString(args->result, args->movie, "baseFormDefinedIn", baseFormFromMod.c_str());
					RegisterString(args->result, args->movie, "baseFormLastChangedBy", baseFormLastChangedBy.c_str());


					//std::string modsModifying = "# Mods " + std::to_string(numModsModifyingBase) + " base # mods" + std::to_string(numModsModifyingRef) + " reference ";
					//std::string baseModifiedBy = "Base From " + baseFormFromMod + " last changed by" + baseFormLastChangedBy;
					//std::string refModifiedBy = "Ref From " + refFormFromMod + " last changed by" + refeFormLastChangedBy;


					//_MESSAGE(modsModifying.c_str());
					//_MESSAGE(baseModifiedBy.c_str());
					//_MESSAGE(refModifiedBy.c_str());

				}
				//_MESSAGE("After FF actor if");

			}
		}

		//_MESSAGE("Method End");
	}

};

class MICScaleform_GetExtraData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		GFxMovieView * movie = args->movie;

		movie->CreateArray(args->result);

		TESObjectREFR* pRef = nullptr;
		LookupREFRByHandle(g_consoleHandle, &pRef);
		if (pRef != nullptr)
		{

			TESForm* pBaseForm = pRef->baseForm;

			if (pBaseForm != nullptr)
			{
				GetCommonFormData(args->result, movie, pBaseForm, pRef);

			}

			if (pBaseForm->GetFormType() == kFormType_NPC
				&& pRef->GetFormType() == kFormType_Character)
			{
				GetCharacterData(args->result, movie, pRef, pBaseForm);
			}
		}
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
	}

	//get information related to where mods the form is found in
	void GetFormLocationData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pBaseForm, TESForm* pRefForm)
	{
		movie->CreateArray(resultArray);

		//if the target is a npc that has been created dynamically we want info on the base form the npc is created from
		if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
		{
			TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

			if (pBaseActor)
			{
				TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

				if (pBaseActorTemplate)
				{
					pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
				}
			}
		}

		//this method may be called at a time we only have a base form, and in that case skip anything related to the reform
		if (pRefForm != nullptr)
		{
			FormModInfoData *refFormModInfo = (FormModInfoData*)pRefForm->unk08;

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
		//Base Form
		FormModInfoData *baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

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

	std::string GetName(TESForm* pBaseForm)
	{
		std::string name = "";

		switch (pBaseForm->GetFormType())
		{
			case kFormType_NPC:
			{
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
		}

		return name;
	}

	void GetModInfoData(GFxValue * resultArray, GFxMovieView * movie, FormModInfoData* modInfoData)
	{
		movie->CreateArray(resultArray);

		int numMods = modInfoData->size;

		for (int i = 0; i < numMods; i++)
		{
			GFxValue modEntry;

			CreateExtraInfoEntry(&modEntry, movie, "Mod", modInfoData->entries[i]->name);
			resultArray->PushBack(&modEntry);
		}
	}

	void GetCharacterData(GFxValue * resultArray, GFxMovieView * movie, TESForm* pRefForm, TESForm* pBaseForm)
	{
		Actor * pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);
		TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

		if (pActor && pNPC)
		{

			/*
			// Spells as Array
			GFxValue addedSpells;
			movieView->CreateArray(&addedSpells);

			for (int i = 0; i < pActor->addedSpells.Length(); i++)
			{
				GFxValue spell;
				movieView->CreateObject(&spell);
				scaleformExtend::FormData(&spell, movieView, pActor->addedSpells.Get(i), bRecursive ? bExtra : false, bRecursive);
				addedSpells.PushBack(&spell);
			}

			pFxVal->SetMember("spells", &addedSpells);*/

			/*
			// ActiveEffects as Array
			GFxValue activeEffects;
			movieView->CreateArray(&activeEffects);

			tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();
			if (effects)
			{
				for (int i = 0; i < effects->Count(); i++)
				{
					GFxValue effect;
					movieView->CreateObject(&effect);

					ActiveEffect * pEffect = effects->GetNthItem(i);

					if (pEffect->item)
						scaleformExtend::MagicItemData(&effect, movieView, pEffect->item, bRecursive ? bExtra : false, bRecursive);

					RegisterNumber(&effect, "elapsed", pEffect->elapsed);
					RegisterNumber(&effect, "duration", pEffect->duration);
					RegisterNumber(&effect, "magnitude", pEffect->magnitude);
					RegisterBool(&effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);

					// ActiveEffect
					if (pEffect->effect && pEffect->effect->mgef)
						scaleformExtend::MagicItemData(&effect, movieView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);

					activeEffects.PushBack(&effect);
				}
			}
			
			pFxVal->SetMember("activeEffects", &activeEffects); */

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

			//Level stuff

			const int level = CALL_MEMBER_FN(pActor, GetLevel)();

			GFxValue levelEntry;

			CreateExtraInfoEntry(&levelEntry, movie, "Level", IntToString(level));
			resultArray->PushBack(&levelEntry);

			GFxValue isPcLeveledEntry;

			bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
			if (isLevelMult)
			{
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
				CreateExtraInfoEntry(&isPcLeveledEntry, movie, "Is PC Level Mult", "False");
			}

			resultArray->PushBack(&isPcLeveledEntry);

		}
		/*PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
		if (pPC)
		{
			RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
		}*/
	}

	void GetActorValue(GFxValue * resultArray, GFxMovieView * movie, Actor * pActor, int id)
	{
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
	}

	std::string GetActorValueName( int id )
	{
		std::string actorValueName;

		switch (id)
		{
		case 0: actorValueName = "Aggression";
			break;
		case 1: actorValueName = "Confidence";
			break;
		case 2: actorValueName = "Energy";
			break;
		case 3: actorValueName = "Morality";
			break;
		case 4: actorValueName = "Mood";
			break;
		case 5: actorValueName = "Assistance";
			break;
		case 6: actorValueName = "OneHanded";
			break;
		case 7: actorValueName = "TwoHanded";
			break;
		case 8: actorValueName = "Marksman";
			break;
		case 9: actorValueName = "Block";
			break;
		case 10: actorValueName = "Smithing";
			break;
		case 11: actorValueName = "HeavyArmor";
			break;
		case 12: actorValueName = "LightArmor";
			break;
		case 13: actorValueName = "Pickpocket";
			break;
		case 14: actorValueName = "Lockpicking";
			break;
		case 15: actorValueName = "Sneak";
			break;
		case 16: actorValueName = "Alchemy";
			break;
		case 17: actorValueName = "Speechcraft";
			break;
		case 18: actorValueName = "Alteration";
			break;
		case 19: actorValueName = "Conjuration";
			break;
		case 20: actorValueName = "Destruction";
			break;
		case 21: actorValueName = "Illusion";
			break;
		case 22: actorValueName = "Restoration";
			break;
		case 23: actorValueName = "Enchanting";
			break;
		case 24: actorValueName = "Health";
			break;
		case 25: actorValueName = "Magicka";
			break;
		case 26: actorValueName = "Stamina";
			break;
		case 27: actorValueName = "HealRate";
			break;
		case 28: actorValueName = "MagickaRate";
			break;
		case 29: actorValueName = "StaminaRate";
			break;
		case 30: actorValueName = "SpeedMult";
			break;
		case 31: actorValueName = "InventoryWeight";
			break;
		case 32: actorValueName = "CarryWeight";
			break;
		case 33: actorValueName = "CritChance";
			break;
		case 34: actorValueName = "MeleeDamage";
			break;
		case 35: actorValueName = "UnarmedDamage";
			break;
		case 36: actorValueName = "Mass";
			break;
		case 37: actorValueName = "VoicePoints";
			break;
		case 38: actorValueName = "VoiceRate";
			break;
		case 39: actorValueName = "DamageResist";
			break;
		case 40: actorValueName = "PoisonResist";
			break;
		case 41: actorValueName = "FireResist";
			break;
		case 42: actorValueName = "ElectricResist";
			break;
		case 43: actorValueName = "FrostResist";
			break;
		case 44: actorValueName = "MagicResist";
			break;
		case 45: actorValueName = "DiseaseResist";
			break;
		case 46: actorValueName = "PerceptionCondition";
			break;
		case 47: actorValueName = "EnduranceCondition";
			break;
		case 48: actorValueName = "LeftAttackCondition";
			break;
		case 49: actorValueName = "RightAttackCondition";
			break;
		case 50: actorValueName = "LeftMobilityCondition";
			break;
		case 51: actorValueName = "RightMobilityCondition";
			break;
		case 52: actorValueName = "BrainCondition";
			break;
		case 53: actorValueName = "Paralysis";
			break;
		case 54: actorValueName = "Invisibility";
			break;
		case 55: actorValueName = "NightEye";
			break;
		case 56: actorValueName = "DetectLifeRange";
			break;
		case 57: actorValueName = "WaterBreathing";
			break;
		case 58: actorValueName = "WaterWalking";
			break;
		case 59: actorValueName = "IgnoreCrippledLimbs";
			break;
		case 60: actorValueName = "Fame";
			break;
		case 61: actorValueName = "Infamy";
			break;
		case 62: actorValueName = "JumpingBonus";
			break;
		case 63: actorValueName = "WardPower";
			break;
		case 64: actorValueName = "RightItemCharge/EquippedItemCharge";
			break;
		case 65: actorValueName = "ArmorPerks";
			break;
		case 66: actorValueName = "ShieldPerks";
			break;
		case 67: actorValueName = "WardDeflection";
			break;
		case 68: actorValueName = "Variable01";
			break;
		case 69: actorValueName = "Variable02";
			break;
		case 70: actorValueName = "Variable03";
			break;
		case 71: actorValueName = "Variable04";
			break;
		case 72: actorValueName = "Variable05";
			break;
		case 73: actorValueName = "Variable06";
			break;
		case 74: actorValueName = "Variable07";
			break;
		case 75: actorValueName = "Variable08";
			break;
		case 76: actorValueName = "Variable09";
			break;
		case 77: actorValueName = "Variable10";
			break;
		case 78: actorValueName = "BowSpeedBonus";
			break;
		case 79: actorValueName = "FavorActive";
			break;
		case 80: actorValueName = "FavorsPerDay";
			break;
		case 81: actorValueName = "FavorsPerDayTimer";
			break;
		case 82: actorValueName = "LeftItemCharge/EquippedStaffCharge";
			break;
		case 83: actorValueName = "AbsorbChance";
			break;
		case 84: actorValueName = "Blindness";
			break;
		case 85: actorValueName = "WeaponSpeedMult";
			break;
		case 86: actorValueName = "ShoutRecoveryMult";
			break;
		case 87: actorValueName = "BowStaggerBonus";
			break;
		case 88: actorValueName = "Telekinesis";
			break;
		case 89: actorValueName = "FavorPointsBonus";
			break;
		case 90: actorValueName = "LastBribedIntimidated";
			break;
		case 91: actorValueName = "LastFlattered";
			break;
		case 92: actorValueName = "MovementNoiseMult";
			break;
		case 93: actorValueName = "BypassVendorStolenCheck";
			break;
		case 94: actorValueName = "BypassVendorKeywordCheck";
			break;
		case 95: actorValueName = "WaitingForPlayer";
			break;
		case 96: actorValueName = "OneHandedMod";
			break;
		case 97: actorValueName = "TwoHandedMod";
			break;
		case 98: actorValueName = "MarksmanMod";
			break;
		case 99: actorValueName = "BlockMod";
			break;
		case 100: actorValueName = "SmithingMod";
			break;
		case 101: actorValueName = "HeavyArmorMod";
			break;
		case 102: actorValueName = "LightArmorMod";
			break;
		case 103: actorValueName = "PickPocketMod";
			break;
		case 104: actorValueName = "LockpickingMod";
			break;
		case 105: actorValueName = "SneakMod";
			break;
		case 106: actorValueName = "AlchemyMod";
			break;
		case 107: actorValueName = "SpeechcraftMod";
			break;
		case 108: actorValueName = "AlterationMod";
			break;
		case 109: actorValueName = "ConjurationMod";
			break;
		case 110: actorValueName = "DestructionMod";
			break;
		case 111: actorValueName = "IllusionMod";
			break;
		case 112: actorValueName = "RestorationMod";
			break;
		case 113: actorValueName = "EnchantingMod";
			break;
		case 114: actorValueName = "OneHandedSkillAdvance";
			break;
		case 115: actorValueName = "TwoHandedSkillAdvance";
			break;
		case 116: actorValueName = "MarksmanSkillAdvance";
			break;
		case 117: actorValueName = "BlockSkillAdvance";
			break;
		case 118: actorValueName = "SmithingSkillAdvance";
			break;
		case 119: actorValueName = "HeavyArmorSkillAdvance";
			break;
		case 120: actorValueName = "LightArmorSkillAdvance";
			break;
		case 121: actorValueName = "PickPocketSkillAdvance";
			break;
		case 122: actorValueName = "LockpickingSkillAdvance";
			break;
		case 123: actorValueName = "SneakSkillAdvance";
			break;
		case 124: actorValueName = "AlchemySkillAdvance";
			break;
		case 125: actorValueName = "SpeechcraftSkillAdvance";
			break;
		case 126: actorValueName = "AlterationSkillAdvance";
			break;
		case 127: actorValueName = "ConjurationSkillAdvance";
			break;
		case 128: actorValueName = "DestructionSkillAdvance";
			break;
		case 129: actorValueName = "IllusionSkillAdvance";
			break;
		case 130: actorValueName = "RestorationSkillAdvance";
			break;
		case 131: actorValueName = "EnchantingSkillAdvance";
			break;
		case 132: actorValueName = "LeftWeaponSpeedMult";
			break;
		case 133: actorValueName = "DragonSouls";
			break;
		case 134: actorValueName = "CombatHealthRegenMult";
			break;
		case 135: actorValueName = "OneHandedPowerMod";
			break;
		case 136: actorValueName = "TwoHandedPowerMod";
			break;
		case 137: actorValueName = "MarksmanPowerMod";
			break;
		case 138: actorValueName = "BlockPowerMod";
			break;
		case 139: actorValueName = "SmithingPowerMod";
			break;
		case 140: actorValueName = "HeavyArmorPowerMod";
			break;
		case 141: actorValueName = "LightArmorPowerMod";
			break;
		case 142: actorValueName = "PickPocketPowerMod";
			break;
		case 143: actorValueName = "LockpickingPowerMod";
			break;
		case 144: actorValueName = "SneakPowerMod";
			break;
		case 145: actorValueName = "AlchemyPowerMod";
			break;
		case 146: actorValueName = "SpeechcraftPowerMod";
			break;
		case 147: actorValueName = "AlterationPowerMod";
			break;
		case 148: actorValueName = "ConjurationPowerMod";
			break;
		case 149: actorValueName = "DestructionPowerMod";
			break;
		case 150: actorValueName = "IllusionPowerMod";
			break;
		case 151: actorValueName = "RestorationPowerMod";
			break;
		case 152: actorValueName = "EnchantingPowerMod";
			break;
		case 153: actorValueName = "DragonRend";
			break;
		case 154: actorValueName = "AttackDamageMult";
			break;
		case 155: actorValueName = "HealRateMult/CombatHealthRegenMultMod";
			break;
		case 156: actorValueName = "MagickaRateMult/CombatHealthRegenMultPowerMod";
			break;
		case 157: actorValueName = "StaminaRateMult";
			break;
		case 158: actorValueName = "WerewolfPerks";
			break;
		case 159: actorValueName = "VampirePerks";
			break;
		case 160: actorValueName = "GrabActorOffset";
			break;
		case 161: actorValueName = "Grabbed";
			break;
		case 162: actorValueName = "DEPRECATED05";
			break;
		case 163: actorValueName = "ReflectDamage";
			break;
		default: actorValueName = "unknown";
			break;
		}

		return actorValueName;
	}

	std::string IntToString(int number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}

	std::string FloatToString(float number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}

	std::string DoubleToString(double number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}

};


//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks(GFxMovieView * view, GFxValue * root)
{
	//_MESSAGE("Install Hooks Called");

	RegisterFunction <MICScaleform_GetReferenceInfo>(root, view, "MICScaleform_GetReferenceInfo");
	RegisterFunction <MICScaleform_GetExtraData>(root, view, "MICScaleform_GetExtraData");

	return true;
}