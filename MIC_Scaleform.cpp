#include "MIC_Scaleform.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
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

		/*
		TESObjectREFR* pRef = nullptr;
		TESForm* pForm = nullptr;

		LookupREFRByHandle(g_consoleHandle, &pRef);
		if (pRef != nullptr)
		{
			TESForm* pForm = pRef->baseForm;
		}

		
		if (!pForm )
			return;*/

		GFxValue fxValue1, fxValue2, fxValue3;
		fxValue1.SetString("a");
		fxValue2.SetString("b");
		fxValue3.SetString("c");

		args->result->PushBack(&fxValue1);
		args->result->PushBack(&fxValue2);
		args->result->PushBack(&fxValue3);

		/**
		scaleformExtend::CommonItemData(pFxVal, pForm);
		if (bExtra) {
			scaleformExtend::StandardItemData(pFxVal, pForm);
			scaleformExtend::MagicItemData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::ActorData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::MiscData(pFxVal, movieView, pForm, bExtra, bRecursive);
		}*/
		//scaleformExtend::FormListData(pFxVal, movieView, pForm, bExtra, bRecursive);
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