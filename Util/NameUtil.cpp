#include "NameUtil.h"

//Vector of all form types used to convert form types into readable strings
std::vector<std::string> FormTypes =
{
	"NONE",
	"TES4(Plugin info)",
	"GRUP(Form Group)",
	"GMST(Game Setting)",
	"KYWD(Keyword)",
	"LCRT(Location Reference Type)",
	"AACT(Action)",
	"TXST(Texture Set)",
	"MICN(Menu Item)",	//There doesn't seem to be an actual corrosponding form type to this
	"GLOB(Global Variable)",
	"CLAS(Class)",
	"FACT(Faction)",
	"HDPT(Head Part)",
	"EYES(Eyes)",
	"RACE(Race)",
	"SOUN(Sound)",
	"ASPC(Acoustic Space)",
	"SKIL(Skill)",	//This doesn't seem to corrospond to any objects in SKSE, much less a form type
	"MGEF(Magic Effect)",
	"SCPT(Script)", //There doesn't seem to be an actual corrosponding form type to this
	"LTEX(Land Texture)",
	"ENCH(Enchantment)",
	"SPEL(Spell)",
	"SCRL(Scroll)",
	"ACTI(Activator)",
	"TACT(Talking Activator)",
	"ARMO(Armor)",
	"BOOK(Book)",
	"CONT(Container)",
	"DOOR(Door)",
	"INGR(Ingredient)",
	"LIGH(Light)",
	"MISC(Misc. Item)",
	"APPA(Alchemical Apparatus)", //Actual form type but apparently a placeholder from Morrowind and Oblivion and not used in game
	"STAT(Static)",
	"SCOL(Static Collection)",
	"MSTT(Movable Static)",
	"GRAS(Grass)",
	"TREE(Tree)",
	"FLOR(Flora)",
	"FURN(Furniture)",
	"WEAP(Weapon)",
	"AMMO(Ammo)",
	"NPC_(Non-Player Character (Actor) )",
	"LVLN(Leveled Actor)",
	"KEYM(Key)",
	"ALCH(Ingestible)",
	"IDLM(Idle Marker)",
	"NOTE(Note)", //There doesn't seem to be an actual corrosponding form type to this
	"COBJ(Constructible Object (recipes))",
	"PROJ(Projectile)",
	"HAZD(Hazard)",
	"SLGM(Soul Gem)",
	"LVLI(Leveled Item)",
	"WTHR(Weather)",
	"CLMT(Climate)",
	"SPGD(Shader Particle Geometry)",
	"RFCT(Visual Effect)",
	"REGN(Region)",
	"NAVI(Navigation Mesh Info Map)",
	"CELL(Cell)",
	"REFR(Reference)",
	"ACHR(Actor Reference)",
	"PMIS(Placed Missle)",
	"PARW(Placed Arrow)",
	"PGRE(Placed Projectile)",
	"PBEA(Placed Beam)",
	"PFLA(Placed Flame)",
	"PCON(Placed Cone/Boice)",
	"PBAR(Placed Barrier)",
	"PHZD(Placed Hazard)",
	"WRLD(World Space)",
	"LAND(Landscape)",
	"NAVM(Navigation Mesh)",
	"TLOD(?)", //That's literally what this is called in GameForms.h. Even the SKSE developers don't seem to know what this is
	"DIAL(Dialog Topic)",
	"INFO(Dialog Response)",
	"QUST(Quest)",
	"IDLE(Idle Animation)",
	"PACK(Package)",
	"CSTY(Combat Style)",
	"LSCR(Load Screen)",
	"LVSP(Leveled Spell)",
	"ANIO(Animated Object)",
	"WATR(Water)",
	"EFSH(Effect Shader)",
	"TOFT(?)", //That's literally what this is called in GameForms.h. Even the SKSE developers don't seem to know what this is
	"EXPL(Explosion)",
	"DEBR(Debris)",
	"IMGS(Image Space)",
	"IMAD(Image Space Adapter",
	"FLST(FormID List)",
	"PERK(Perk)",
	"BPTD(Body Part Data)",
	"ADDN(Addon Node)",
	"AVIF(Actor Value Information)",
	"CAMS(Camera Shot)",
	"CPTH(Camera Path)",
	"VTYP(Voice Type)",
	"MATT(Material Type)",
	"IPCT(Impact)",
	"IPDS(Impact Data Set)",
	"ARMA(Armor Addon)",
	"ECZN(Encounter Zone)",
	"LCTN(Location)",
	"MESG(Message)",
	"RGDL(Ragdoll)",
	"DOBJ(Default Object Manager)",
	"LGTM(Lighting Template)",
	"MUSC(Music Type)",
	"FSTP(Footstep)",
	"FSTS(Footstep Set)",
	"SMBN(Story Manager Branch Node)",
	"SMQN(Story Manager Quest Node)",
	"SMEN(Story Manager Event Node)",
	"DLBR(Dialog Branch)",
	"MUST(Music Track)",
	"DLVW(Dialog View)",
	"WOOP(Word of Power)",
	"SHOU(Shout)",
	"EQUP(Equip Type)",
	"RELA(Relationship)",
	"SCEN(Scene)",
	"ASTP(Association Type)",
	"OTFT(Outfit)",
	"ARTO(Art Object)",
	"MATO(Material Object)",
	"MOVT(Movement Type)",
	"SNDR(Sound Descriptor)",
	"DUAL(Dual Cast Data)",
	"SNCT(Sound Category)",
	"SOPM(Sound Output Model)",
	"COLL(Collision Layer)",
	"CLFM(Color)",
	"REVB(Reverb Parameters)",
	"LENS(Lens Flare)",
	"LSPR(Unknown)",
	"VOLI(Volumetric Lighting)",
	"Unknown8A",
	"Alias",
	"ReferenceAlias",
	"LocAlias",
	"ActiveMagicEffect"
};

std::string GetFormTypeName(int formType)
{
	return FormTypes[formType];
}


std::string GetName(RE::TESForm* pBaseForm)
{
	_DMESSAGE("GetExtraData: GetName Start");

	std::string name = "";

	switch (pBaseForm->GetFormType())
	{
		case RE::FormType::NPC:
		{
			_DMESSAGE("GetExtraData: GetName NPC");

			RE::TESNPC* pNPC = static_cast<RE::TESNPC*>(pBaseForm);
			if (pNPC)
			{
				name = pNPC->fullName.c_str();
			}

			break;
		}

	/*
	case kFormType_EffectSetting:
	{
		DebugMessage("GetExtraData: GetName Magic Effect");

		EffectSetting* pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
		if (pEffectSetting)
		{
			if (pEffectSetting->fullName.name.data)
			{
				name = pEffectSetting->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Spell:
	case kFormType_ScrollItem:
	case kFormType_Ingredient:
	case kFormType_Potion:
	case kFormType_Enchantment:
	{
		DebugMessage("GetExtraData: GetName Spell");

		MagicItem* pMagicItem = DYNAMIC_CAST(pBaseForm, TESForm, MagicItem);
		if (pMagicItem)
		{
			if (pMagicItem->fullName.name.data)
			{
				name = pMagicItem->fullName.name.data;
			}
		}


		break;
	}
	case kFormType_Armor:
	{
		DebugMessage("GetExtraData: GetName Armor");
		TESObjectARMO* pArmor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMO);

		if (pArmor)
		{
			if (pArmor->fullName.name.data)
			{
				name = pArmor->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Ammo:
	{
		DebugMessage("GetExtraData: GetName Ammo");
		TESAmmo* pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
		if (pAmmo)
		{
			if (pAmmo->fullName.name.data)
			{
				name = pAmmo->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Weapon:
	{
		DebugMessage("GetExtraData: GetName Weapon");
		TESObjectWEAP* pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			if (pWeapon->fullName.name.data)
			{
				name = pWeapon->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_SoulGem:
	{
		DebugMessage("GetExtraData: GetName Soul Gem");
		TESSoulGem* pSoul = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
		if (pSoul)
		{
			if (pSoul->fullName.name.data)
			{
				name = pSoul->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Book:
	{
		DebugMessage("GetExtraData: GetName Book");
		TESObjectBOOK* pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
		if (pBook)
		{
			if (pBook->fullName.name.data)
			{
				name = pBook->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Misc:
	{
		DebugMessage("GetExtraData: GetName Misc Item");
		TESObjectMISC* pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
		if (pMisc)
		{
			if (pMisc->fullName.name.data)
			{
				name = pMisc->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Key:
	{
		DebugMessage("GetExtraData: GetName Key Item");
		TESKey* pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
		if (pKey)
		{
			if (pKey->fullName.name.data)
			{
				name = pKey->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Container:
	{
		DebugMessage("GetExtraData: GetName Container");
		TESObjectCONT* pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			if (pContainer->fullName.name.data)
			{
				name = pContainer->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Light:
	{
		DebugMessage("GetExtraData: GetName Light");
		TESObjectLIGH* pLight = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectLIGH);
		if (pLight)
		{
			if (pLight->fullName.name.data)
			{
				name = pLight->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Shout:
	{
		DebugMessage("GetExtraData: GetName Shout");
		TESShout* pShout = DYNAMIC_CAST(pBaseForm, TESForm, TESShout);
		if (pShout)
		{
			if (pShout->fullName.name.data)
			{
				name = pShout->fullName.name.data;
			}
		}

		break;
	}

	case kFormType_Perk:
	{
		DebugMessage("GetExtraData: GetName Perk");
		BGSPerk* pPerk = DYNAMIC_CAST(pBaseForm, TESForm, BGSPerk);
		if (pPerk)
		{
			if (pPerk->fullName.name.data)
			{
				name = pPerk->fullName.name.data;
			}

			if (name == "")
			{
				name = FormIDToString(pBaseForm->formID);
			}
		}

		break;
	}

	case kFormType_Faction:
	{
		DebugMessage("GetExtraData: GetName Faction");
		TESFaction* pFaction = DYNAMIC_CAST(pBaseForm, TESForm, TESFaction);
		if (pFaction)
		{
			if (pFaction->fullName.name.data)
			{
				name = pFaction->fullName.name.data;
			}

			//if no name was found
			if (name == "")
			{
				name = FormIDToString(pBaseForm->formID);
			}
		}

		break;
	}

	case kFormType_Race:
	{
		DebugMessage("GetExtraData: GetName Race");
		TESRace* pRace = DYNAMIC_CAST(pBaseForm, TESForm, TESRace);
		if (pRace)
		{
			if (pRace->editorId)
			{
				name = pRace->editorId;
			}
		}

		break;
	}

	case kFormType_ARMA:
	{
		DebugMessage("GetExtraData: GetName Arma");
		TESObjectARMA* pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);
		if (pArma && pArma->race.race)
		{
			name = GetName(pArma->race.race);
		}

		break;
	}

	case kFormType_WorldSpace:
	{
		DebugMessage("GetExtraData: GetName Worldspace");
		TESWorldSpace* pWorld = DYNAMIC_CAST(pBaseForm, TESForm, TESWorldSpace);
		if (pWorld && pWorld->fullName.name.data)
		{
			name = pWorld->fullName.name.data;
		}

		break;
	}

	case kFormType_Cell:
	{
		DebugMessage("GetExtraData: GetName Worldspace");
		TESObjectCELL* pCell = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCELL);
		if (pCell && pCell->fullName.name.data)
		{
			name = pCell->fullName.name.data;
		}

		break;
	}

	//for objects with no name data show the formID
	case kFormType_Package:
	{
		name = FormIDToString(pBaseForm->formID);
		break;
	}
	*/
	}

	_DMESSAGE(("GetExtraData: GetName End: " + name).c_str());
	
	return name;
}
