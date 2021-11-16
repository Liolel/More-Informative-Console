#include "NameUtil.h"
#include "GeneralUtil.h"
#include "SKSE/Logger.h"

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

std::vector<std::string> ExtraDataTypes =
{
"NONE",
"kHavok",
"kCell3D",
"kCellWaterType",
"kRegionList",
"kSeenData",
"kEditorID",
"kCellMusicType",
"kCellSkyRegion",
"kProcessMiddleLow",
"kDetachTime",
"kPersistentCell",
"unknown12",
"kAction",
"kStartingPosition",
"unknown15",
"kAnimGraphManager",
"kBiped",
"kUsedMarkers",
"kDistantData",
"kRagDollData",
"kContainerChanges",
"kWorn",
"kWornLeft",
"kPackageStartLocation",
"kPackage",
"kTresPassPackage",
"kRunOncePacks",
"kReferenceHandle",
"kFollower",
"kLevCreaModifier",
"kGhost",
"kOriginalReference",
"kOwnership",
"kGlobal",
"kRank",
"kCount",
"kHealth",
"unknown38",
"kTimeLeft",
"kCharge",
"kLight",
"kLock",
"kTeleport",
"kMapMarker",
"kLeveledCreature",
"kLeveledItem",
"kScale",
"kSeed",
"kMagicCaster",
"kNonActorMagicTarget",
"unknown51",
"kPlayerCrimeList",
"unknown53",
"kEnableStateParent",
"kEnableStateChildren",
"kItemDropper",
"kDroppedItemList",
"kRandomTeleportMarker",
"unknown59",
"kSavedHavokData",
"kCannotWear",
"kPoison",
"kMagicLight",
"kLastFinishedSequence",
"kSavedAnimation",
"kNorthRotation",
"kSpawnContainer",
"kFriendHits",
"kHeadingTarget",
"unknown70",
"kRefractionProperty",
"kStartingWorldOrCell",
"kHotkey",
"kEditorRef3DData",
"kEditiorRefMoveData",
"kInfoGeneralTopic",
"kHasNoRumors",
"kSound",
"kTerminalState",
"kLinkedRef",
"kLinkedRefChildren",
"kActivateRef",
"kActivateRefChildren",
"kCanTalkToPlayer",
"kObjectHealth",
"kCellImageSpace",
"kNavMeshPortal",
"kModelSwap",
"kRadius",
"unknown90",
"kFactionChanges",
"kDismemberedLimbs",
"kActorCause",
"kMultiBound",
"kMultiBoundData",
"kMultiBoundRef",
"kReflectedRefs",
"kReflectorRefs",
"kEmittanceSource",
"kUnk64",
"kCombatStyle",
"unknown102",
"kPrimitive",
"kOpenCloseActivateRef",
"kAnimNoteReceiver",
"kAmmo",
"kPatrolRefData",
"kPackageData",
"kOcclusionShape",
"kCollisionData",
"kSayTopicInfoOnceADay",
"kEncounterZone",
"kSayTopicInfo",
"kOcclusionPlaneRefData",
"kPortalRefData",
"kPortal",
"kRoom",
"kHealthPerc",
"kRoomRefData",
"kGuardedRefData",
"kCreatureAwakeSound",
"unknown122",
"kHorse",
"kIgnoredBySandbox",
"kCellAcousticSpace",
"kReservedMarkers",
"kWeaponIdleSound",
"kWaterLightRefs",
"kLitWaterRefs",
"kWeaponAttackSound",
"kActivateLoopSound",
"kPatrolRefInUseData",
"kAshPileRef",
"kCreatureMovementSound",
"kFollowerSwimBreadcrumbs",
"kAliasInstanceArray",
"kLocation",
"unknown138",
"kLocationRefType",
"kPromotedRef",
"kAnimationSequencer",
"kOutfitItem",
"unknown143",
"kLeveledItemBase",
"kLightData",
"kSceneData",
"kBadPosition",
"kHeadTrackingWeight",
"kFromAlias",
"kShouldWear",
"kFavorCost",
"kAttachedArrows3D",
"kTextDisplayData",
"kAlphaCutoff",
"kEnchantment",
"kSoul",
"kForcedTarget",
"unknown158",
"kUniqueID",
"kFlags",
"kRefrPath",
"kDecalGroup",
"kLockList",
"kForcedLandingMarker",
"kLargeRefOwnerCells",
"kCellWaterEnvMap",
"kCellGrassData",
"kTeleportName",
"kInteraction",
"kWaterData",
"kWaterCurrentZoneData",
"kAttachRef",
"kAttachRefChildren",
"kGroupConstraint",
"kScriptedAnimDependence",
"kCachedScale",
"kRaceData",
"kGIDBuffer",
"kMissingRefIDs",
"kUnkB4",
"kResourcesPreload",
"kUnkB6",
"kUnkB7",
"kUnkB8",
"kUnkB9",
"kUnkBA",
"kUnkBB",
"kUnkBC",
"kUnkBD",
"kUnkBE",
"kUnkBF"
};

std::string GetFormTypeName(int formType)
{
	return FormTypes[formType];
}


std::string GetName(RE::TESForm* baseForm)
{
	logger::debug("GetExtraData: GetName Start");

	std::string name = "";

	switch (baseForm->GetFormType())
	{
		case RE::FormType::NPC:
		{
			logger::debug("GetExtraData: GetName NPC");

			RE::TESNPC* pNPC = static_cast<RE::TESNPC*>(baseForm);
			if (pNPC)
			{
				name = pNPC->fullName.c_str();
			}

			break;
		}

	
		case RE::FormType::MagicEffect:
		{
			logger::debug("GetExtraData: GetName Magic Effect");

			RE::EffectSetting* effectSetting = static_cast<RE::EffectSetting*>(baseForm);
			if (effectSetting)
			{			
				name = effectSetting->fullName.c_str();
			}

			break;
		}

		case RE::FormType::Spell:
		case RE::FormType::Scroll:
		case RE::FormType::Ingredient:
		case RE::FormType::AlchemyItem:
		case RE::FormType::Enchantment:
	{
		logger::debug("GetExtraData: GetName MagicItem");

		RE::MagicItem* magicItem = static_cast<RE::MagicItem*>(baseForm);
		if (magicItem)
		{
			name = magicItem->fullName.c_str();
		}


		break;
	}
	
		case RE::FormType::Armor:
		{
			logger::debug("GetExtraData: GetName Armor");
			RE::TESObjectARMO* armor = static_cast<RE::TESObjectARMO*>(baseForm);

			if (armor)
			{
				name = armor->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Ammo:
		{
			logger::debug("GetExtraData: GetName Ammo");
			RE::TESAmmo* ammo = static_cast<RE::TESAmmo*>(baseForm);

			if (ammo)
			{
				name = ammo->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Weapon:
		{
			logger::debug("GetExtraData: GetName Weapon");
			RE::TESObjectWEAP* weapon = static_cast<RE::TESObjectWEAP*>(baseForm);

			if (weapon)
			{
				name = weapon->fullName.c_str();
			}
			break;
		}

		case RE::FormType::SoulGem:
		{
			logger::debug("GetExtraData: GetName Soul gem");
			RE::TESSoulGem* soulgem = static_cast<RE::TESSoulGem*>(baseForm);

			if (soulgem)
			{
				name = soulgem->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Book:
		{
			logger::debug("GetExtraData: GetName Book");
			RE::TESObjectBOOK* book = static_cast<RE::TESObjectBOOK*>(baseForm);

			if (book)
			{
				name = book->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Misc:
		{
			logger::debug("GetExtraData: GetName Misc");
			RE::TESObjectMISC* misc = static_cast<RE::TESObjectMISC*>(baseForm);

			if (misc)
			{
				name = misc->fullName.c_str();
			}
			break;
		}

		case RE::FormType::KeyMaster:
		{
			logger::debug("GetExtraData: GetName Key");
			RE::TESKey* key = static_cast<RE::TESKey*>(baseForm);

			if (key)
			{
				name = key->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Container:
		{
			logger::debug("GetExtraData: GetName Container");
			RE::TESObjectCONT* container = static_cast<RE::TESObjectCONT*>(baseForm);

			if (container)
			{
				name = container->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Light:
		{
			logger::debug("GetExtraData: GetName Light");
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(baseForm);

			if (light)
			{
				name = light->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Shout:
		{
			logger::debug("GetExtraData: GetName Shout");
			RE::TESShout* shout = static_cast<RE::TESShout*>(baseForm);

			if (shout)
			{
				name = shout->fullName.c_str();
			}
			break;
		}

		case RE::FormType::Perk:
		{
			logger::debug("GetExtraData: GetName Perk");
			RE::BGSPerk* perk = static_cast<RE::BGSPerk*>(baseForm);

			if (perk)
			{
				name = perk->fullName.c_str();

				//If the name is empty show the formID as a backup
				if (name == "")
				{
					name = FormIDToString(baseForm->formID);
				}
			}
			break;
		}

		case RE::FormType::Faction:
		{
			logger::debug("GetExtraData: GetName Faction");
			RE::TESFaction* faction = static_cast<RE::TESFaction*>(baseForm);

			if (faction)
			{
				name = faction->fullName.c_str();

				//If the name is empty show the formID as a backup
				if (name == "")
				{
					name = FormIDToString(baseForm->formID);
				}
			}
			break;
		}

		case RE::FormType::Race:
		{
			logger::debug("GetExtraData: GetName Race");
			RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
			if (race)
			{
				name = race->GetFormEditorID();
			}

			break;
		}
	
		case RE::FormType::Armature:
		{
			logger::debug("GetExtraData: GetName Arma");
			RE::TESObjectARMA* arma = static_cast<RE::TESObjectARMA*>(baseForm);
			if (arma && arma->race)
			{
				name = GetName(arma->race);
			}

			break;
		}

		case RE::FormType::WorldSpace:
		{
			logger::debug("GetExtraData: GetName Worldspace");
			RE::TESWorldSpace* worldspace = static_cast<RE::TESWorldSpace*>(baseForm);
			if (worldspace)
			{
				name = worldspace->fullName.c_str();
			}

			break;
		}

		case RE::FormType::Cell:
		{
			logger::debug("GetExtraData: GetName Cell");
			RE::TESObjectCELL* cell = static_cast<RE::TESObjectCELL*>(baseForm);
			if (cell)
			{
				name = cell->fullName.c_str();
			}

			break;
		}

		case RE::FormType::MusicType:
		{
			logger::debug("GetExtraData: GetName MusicTrack");
			RE::BGSMusicType* musicType = static_cast<RE::BGSMusicType*>(baseForm);
			if (musicType)
			{
				name = musicType->formEditorID.c_str();
			}

			break;
		}

		case RE::FormType::Keyword:
		{
			logger::debug("GetExtraData: GetName Keyword");
			RE::BGSKeyword* keyword = static_cast<RE::BGSKeyword*>(baseForm);
			if (keyword)
			{
				name = keyword->formEditorID.c_str();
			}

			break;
		}

		//for objects with no name data show the formID
		case RE::FormType::Package:
		case RE::FormType::MusicTrack:
		case RE::FormType::Weather:
		{
			name = FormIDToString(baseForm->formID);
			break;
		}
	
	}

	logger::debug(("GetExtraData: GetName End: " + name).c_str());
	
	return name;
}

//Get the name of a texture type in a BGSTextureSet object
std::string GetTextureType(int textureType)
{
	std::string textureTypeName = "";

	switch (textureType)
	{
	case RE::BGSTextureSet::Texture::kDiffuse: textureTypeName = "Diffuse";
		break;
	case RE::BGSTextureSet::Texture::kNormal: textureTypeName = "Normal/Gloss";
		break;
	case RE::BGSTextureSet::Texture::kEnvironmentMask: textureTypeName = "Enviroment Mask/Subsurface Tint";
		break;
	case RE::BGSTextureSet::Texture::kGlowMap: textureTypeName = "Glow/Detail Map";
		break;
	case RE::BGSTextureSet::Texture::kHeight: textureTypeName = "Height";
		break;
	case RE::BGSTextureSet::Texture::kEnvironment: textureTypeName = "Enviroment";
		break;
	case RE::BGSTextureSet::Texture::kMultilayer: textureTypeName = "Multilayer";
		break;
	case RE::BGSTextureSet::Texture::kBacklightMask: textureTypeName = "Backlight Mask/Specular";
		break;
	default: textureTypeName = "Unknown type";
		break;
	}

	return textureTypeName;
}


std::string GetArmorTypeName(RE::TESObjectARMO::ArmorType armorType )
{
	std::string armorTypeName = "";

	switch (armorType)
	{
	case RE::TESObjectARMO::ArmorType::kClothing: armorTypeName = "Clothing";
		break;
	case RE::TESObjectARMO::ArmorType::kLightArmor: armorTypeName = "Light";
		break;
	case RE::TESObjectARMO::ArmorType::kHeavyArmor: armorTypeName = "Heavy";
		break;
	default: armorTypeName = "Unkown";
		break;
	}

	return armorTypeName;
}


//Get the name of a equip slot. slot is based on the bit flag for the slot
std::string GetEquipSlotName(int slot)
{
	std::string equipSlotName = "";

	switch (slot)
	{
	case 0:
	{
		equipSlotName = "Head";
		break;
	}
	case 1:
	{
		equipSlotName = "Hair";
		break;
	}
	case 2:
	{
		equipSlotName = "Body";
		break;
	}
	case 3:
	{
		equipSlotName = "Hands";
		break;
	}
	case 4:
	{
		equipSlotName = "Forearms";
		break;
	}
	case 5:
	{
		equipSlotName = "Amulet";
		break;
	}
	case 6:
	{
		equipSlotName = "Ring";
		break;
	}
	case 7:
	{
		equipSlotName = "Feet";
		break;
	}
	case 8:
	{
		equipSlotName = "Calves";
		break;
	}
	case 9:
	{
		equipSlotName = "Shield";
		break;
	}
	case 10:
	{
		equipSlotName = "Tail";
		break;
	}
	case 11:
	{
		equipSlotName = "Long hair";
		break;
	}
	case 12:
	{
		equipSlotName = "Circlet";
		break;
	}
	case 13:
	{
		equipSlotName = "Ears";
		break;
	}
	case 14:
	{
		equipSlotName = "Slot 44";
		break;
	}
	case 15:
	{
		equipSlotName = "Slot 45";
		break;
	}
	case 16:
	{
		equipSlotName = "Slot 46";
		break;
	}
	case 17:
	{
		equipSlotName = "Slot 47";
		break;
	}
	case 18:
	{
		equipSlotName = "Slot 48";
		break;
	}
	case 19:
	{
		equipSlotName = "Slot 49";
		break;
	}
	case 20:
	{
		equipSlotName = "Decapitated head";
		break;
	}
	case 21:
	{
		equipSlotName = "Decapitate";
		break;
	}
	case 22:
	{
		equipSlotName = "Slot 52";
		break;
	}
	case 23:
	{
		equipSlotName = "Slot 53";
		break;
	}
	case 24:
	{
		equipSlotName = "Slot 54";
		break;
	}
	case 25:
	{
		equipSlotName = "Slot 55";
		break;
	}
	case 26:
	{
		equipSlotName = "Slot 56";
		break;
	}
	case 27:
	{
		equipSlotName = "Slot 57";
		break;
	}
	case 28:
	{
		equipSlotName = "Slot 58";
		break;
	}
	case 29:
	{
		equipSlotName = "Slot 59";
		break;
	}
	case 30:
	{
		equipSlotName = "Slot 60";
		break;
	}
	case 31:
	{
		equipSlotName = "FX01";
		break;
	}
	}

	return equipSlotName;
}

std::string GetActorValueName(int id)
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


std::string GetEffectTypeName(int id)
{
	std::string effectTypeName;

	switch (id)
	{
	case 0: effectTypeName = "Value Mod";
		break;
	case 1: effectTypeName = "Script";
		break;
	case 2: effectTypeName = "Dispel";
		break;
	case 3: effectTypeName = "Cure Disease";
		break;
	case 4: effectTypeName = "Absorb";
		break;
	case 5: effectTypeName = "Dual Value Mod";
		break;
	case 6: effectTypeName = "Calm";
		break;
	case 7: effectTypeName = "Demoralize";
		break;
	case 8: effectTypeName = "Frenzy";
		break;
	case 9: effectTypeName = "Disarm";
		break;
	case 10: effectTypeName = "Command Summoned";
		break;
	case 11: effectTypeName = "Invisibility";
		break;
	case 12: effectTypeName = "Light";
		break;
	case 15: effectTypeName = "Lock";
		break;
	case 16: effectTypeName = "Open";
		break;
	case 17: effectTypeName = "Bound Weapon";
		break;
	case 18: effectTypeName = "Summon Creature";
		break;
	case 19: effectTypeName = "Detect Life";
		break;
	case 20: effectTypeName = "Telekinesis";
		break;
	case 21: effectTypeName = "Paralysis";
		break;
	case 22: effectTypeName = "Reanimate";
		break;
	case 23: effectTypeName = "Soul Trap";
		break;
	case 24: effectTypeName = "Turn Undead";
		break;
	case 25: effectTypeName = "Guide";
		break;
	case 26: effectTypeName = "Werewolf Feed";
		break;
	case 27: effectTypeName = "Cure Paralysis";
		break;
	case 28: effectTypeName = "Cure Addiction";
		break;
	case 29: effectTypeName = "Cure Poison";
		break;
	case 30: effectTypeName = "Concussion";
		break;
	case 31: effectTypeName = "Value and Parts";
		break;
	case 32: effectTypeName = "Accumulate Magnitude";
		break;
	case 33: effectTypeName = "Stagger";
		break;
	case 34: effectTypeName = "Peak Value Mod";
		break;
	case 35: effectTypeName = "Cloak";
		break;
	case 36: effectTypeName = "Werewolf";
		break;
	case 37: effectTypeName = "Slow Time";
		break;
	case 38: effectTypeName = "Rally";
		break;
	case 39: effectTypeName = "Enchance Weapon";
		break;
	case 40: effectTypeName = "Spawn Hazard";
		break;
	case 41: effectTypeName = "Etherealize";
		break;
	case 42: effectTypeName = "Banish";
		break;
	case 43: effectTypeName = "Spawn Scripted Ref";
		break;
	case 44: effectTypeName = "Disguise";
		break;
	case 45: effectTypeName = "Grab Actor";
		break;
	case 46: effectTypeName = "Vampire Lord";
		break;
	default: effectTypeName = "Unknown";
		break;
	}

	return effectTypeName;
}



std::string GetSpellTypeName(RE::MagicSystem::SpellType spellType)
{
	std::string spellTypeName = "";

	switch (spellType)
	{
	case RE::MagicSystem::SpellType::kSpell: spellTypeName = "Spell";
		break;
	case RE::MagicSystem::SpellType::kDisease: spellTypeName = "Disease";
		break;
	case RE::MagicSystem::SpellType::kPower: spellTypeName = "Power";
		break;
	case RE::MagicSystem::SpellType::kLesserPower: spellTypeName = "Lesser Power";
		break;
	case RE::MagicSystem::SpellType::kAbility: spellTypeName = "Ability";
		break;
	case RE::MagicSystem::SpellType::kPoison: spellTypeName = "Poison";
		break;
	case RE::MagicSystem::SpellType::kEnchantment: spellTypeName = "Enchantment";
		break;
	case RE::MagicSystem::SpellType::kIngredient: spellTypeName = "Ingredient";
		break;
	case RE::MagicSystem::SpellType::kLeveledSpell: spellTypeName = "Leveled Spell";
		break;
	case RE::MagicSystem::SpellType::kAddiction: spellTypeName = "Addiction";
		break;
	case RE::MagicSystem::SpellType::kVoicePower: spellTypeName = "Voice";
		break;
	case RE::MagicSystem::SpellType::kStaffEnchantment: spellTypeName = "Staff Enchantment";
		break;
	case RE::MagicSystem::SpellType::kScroll: spellTypeName = "Scroll";
		break;
	default: spellTypeName = "Unknown type";
		break;
	}

	return spellTypeName;
}

std::string GetCastingTypeName(RE::MagicSystem::CastingType castingType)
{
	std::string castingTypeName = "";

	switch (castingType)
	{
	case RE::MagicSystem::CastingType::kConcentration: castingTypeName = "Concentration";
		break;
	case RE::MagicSystem::CastingType::kConstantEffect: castingTypeName = "Constant Effect";
		break;
	case RE::MagicSystem::CastingType::kFireAndForget: castingTypeName = "Fire and Forget";
		break;
	case RE::MagicSystem::CastingType::kScroll: castingTypeName = "Scroll";
		break;
	default: castingTypeName = "Unknown type";
		break;
	}

	return castingTypeName;
}

std::string GetDeliveryTypeName(RE::MagicSystem::Delivery deliveryType)
{
	std::string deliveryTypeName = "";

	switch (deliveryType)
	{
	case RE::MagicSystem::Delivery::kAimed: deliveryTypeName = "Aimed";
		break;
	case RE::MagicSystem::Delivery::kTouch: deliveryTypeName = "Contact";
		break;
	case RE::MagicSystem::Delivery::kSelf: deliveryTypeName = "Self";
		break;
	case RE::MagicSystem::Delivery::kTargetActor: deliveryTypeName = "Target Actor";
		break;
	case RE::MagicSystem::Delivery::kTargetLocation: deliveryTypeName = "Target Location";
		break;
	default: deliveryTypeName = "Unknown type";
		break;
	}

	return deliveryTypeName;
}

std::string GetExtraDataTypeName(int extraDataType)
{
	return ExtraDataTypes[extraDataType];
}

//Get the name of a equip slot. slot is based on the bit flag for the slot
std::string GetWeaponAnimationTypeName(RE::WEAPON_TYPE weaponType)
{
	std::string weaponTypeName = "";

	switch (weaponType)
	{
		case RE::WEAPON_TYPE::kHandToHandMelee:
		{
			weaponTypeName = "Hand to Hand";
			break;
		}

		case RE::WEAPON_TYPE::kOneHandSword:
		{
			weaponTypeName = "One handed sword";
			break;
		}

		case RE::WEAPON_TYPE::kOneHandDagger:
		{
			weaponTypeName = "Dagger";
			break;
		}

		case RE::WEAPON_TYPE::kOneHandAxe:
		{
			weaponTypeName = "One handed axe";
			break;
		}

		case RE::WEAPON_TYPE::kTwoHandSword:
		{
			weaponTypeName = "Two handed sword";
			break;
		}

		case RE::WEAPON_TYPE::kTwoHandAxe:
		{
			weaponTypeName = "Two handed axe";
			break;
		}

		case RE::WEAPON_TYPE::kBow:
		{
			weaponTypeName = "Bow";
			break;
		}

		case RE::WEAPON_TYPE::kStaff:
		{
			weaponTypeName = "Staff";
			break;
		}

		case RE::WEAPON_TYPE::kCrossbow:
		{
			weaponTypeName = "Crossbow";
			break;
		}
	}
	return weaponTypeName;
}

//Get the name of a equip type 
std::string GetEquipTypeName(int formID)
{
	std::string equipTypeName = "";

	switch (formID)
	{
	case 0x00013F42:
	{
		equipTypeName = "Right Hand";
		break;
	}
	case 0x00013F43:
	{
		equipTypeName = "Left Hand";
		break;
	}
	case 0x00013F44:
	{
		equipTypeName = "Either Hand";
		break;
	}
	case 0x00013F45:
	{
		equipTypeName = "Both Hands";
		break;
	}
	case 0x000141E8:
	{
		equipTypeName = "Shield";
		break;
	}
	case 0x00025BEE:
	{
		equipTypeName = "Voice";
		break;
	}
	case 0x00035698:
	{
		equipTypeName = "Potion";
		break;
	}
	}
	return equipTypeName;
}



std::string GetLockLevelName(RE::LOCK_LEVEL lockLevel)
{
	std::string lockLevelString = "";

	switch (lockLevel)
	{
	case RE::LOCK_LEVEL::kUnlocked:
		lockLevelString = "Unlocked";
		break;
	case RE::LOCK_LEVEL::kVeryEasy:
		lockLevelString = "Novice";
		break;
	case RE::LOCK_LEVEL::kEasy:
		lockLevelString = "Apprentice";
		break;
	case RE::LOCK_LEVEL::kAverage:
		lockLevelString = "Adept";
		break;
	case RE::LOCK_LEVEL::kHard:
		lockLevelString = "Expert";
		break;
	case RE::LOCK_LEVEL::kVeryHard:
		lockLevelString = "Master";
		break;
	case RE::LOCK_LEVEL::kRequiresKey:
		lockLevelString = "Requires Key";
		break;
	default: 
		lockLevelString = "Unkown";
		break;
	}

	return lockLevelString;
}

std::string GetMFGExpressionName(int expressionIndex)
{
	std::string expressionName;

	switch (expressionIndex)
	{
	case 0: expressionName = "Dialog Anger";
		break;
	case 1: expressionName = "Dialog Fear";
		break;
	case 2: expressionName = "Dialog Happy";
		break;
	case 3: expressionName = "Dialog Sad";
		break;
	case 4: expressionName = "Dialog Surprise";
		break;
	case 5: expressionName = "Dialog Puzzled";
		break;
	case 6: expressionName = "Dialog Disgust";
		break;
	case 7: expressionName = "Mood Neutral";
		break;
	case 8: expressionName = "Mood Anger";
		break;
	case 9: expressionName = "Mood Fear";
		break;
	case 10: expressionName = "Mood Happy";
		break;
	case 11: expressionName = "Mood Sad";
		break;
	case 12: expressionName = "Mood Surprise";
		break;
	case 13: expressionName = "Mood Puzzled";
		break;
	case 14: expressionName = "Mood Disgust";
		break;
	case 15: expressionName = "Combat Anger";
		break;
	case 16: expressionName = "Combat Shout";
		break;
	default: expressionName = "Unknown";
		break;
	}

	return expressionName;
}

std::string GetMFGModiferName(int modifierIndex)
{
	std::string modifierName;

	switch (modifierIndex)
	{
	case 0: modifierName = "BlinkLeft";
		break;
	case 1: modifierName = "BlinkRight";
		break;
	case 2: modifierName = "BrowDownLeft";
		break;
	case 3: modifierName = "BrowDownRight";
		break;
	case 4: modifierName = "BrowInLeft";
		break;
	case 5: modifierName = "BrowInRight";
		break;
	case 6: modifierName = "BrowUpLeft";
		break;
	case 7: modifierName = "BrowUpRight";
		break;
	case 8: modifierName = "LookDown";
		break;
	case 9: modifierName = "LookLeft";
		break;
	case 10: modifierName = "LookRight";
		break;
	case 11: modifierName = "LookUp";
		break;
	case 12: modifierName = "SquintLeft";
		break;
	case 13: modifierName = "SquintRight";
		break;
	case 14: modifierName = "HeadPitch";
		break;
	case 15: modifierName = "HeadRoll";
		break;
	case 16: modifierName = "HeadYaw";
		break;
	default: modifierName = "Unknown";
		break;
	}

	return modifierName;
}

std::string GetMFGPhenomeName(int phenomeIndex)
{
	std::string phenomeName;

	switch (phenomeIndex)
	{
	case 0: phenomeName = "Aah";
		break;
	case 1: phenomeName = "BigAah";
		break;
	case 2: phenomeName = "BMP";
		break;
	case 3: phenomeName = "ChJSh";
		break;
	case 4: phenomeName = "DST";
		break;
	case 5: phenomeName = "Eee";
		break;
	case 6: phenomeName = "Eh";
		break;
	case 7: phenomeName = "FV";
		break;
	case 8: phenomeName = "I";
		break;
	case 9: phenomeName = "K";
		break;
	case 10: phenomeName = "N";
		break;
	case 11: phenomeName = "Oh";
		break;
	case 12: phenomeName = "OohQ";
		break;
	case 13: phenomeName = "R";
		break;
	case 14: phenomeName = "Th";
		break;
	case 15: phenomeName = "W";
		break;
	default: phenomeName = "Unknown";
		break;
	}

	return phenomeName;
}