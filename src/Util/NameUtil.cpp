#include "NameUtil.h"
#include "GeneralUtil.h"
#include "EditorIDCache.h"
#include "globals.h"
#include "TranslationCache.h"
#include "SKSE/Logger.h"

//4-29-2022: Checked for translations needed

//No translations used here as these only have meaning to me during development. These are not meant to be used by the player
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
	std::string formTypeKey = "$FormType" + IntToString(formType);
	return GetTranslation(formTypeKey);
}


std::string GetName(RE::TESForm* baseForm, RE::TESObjectREFR* refForm )
{
	logger::debug("GetExtraData: GetName Start");

	std::string name = "";

	if (refForm
		&& refForm->extraList.HasType(RE::ExtraDataType::kTextDisplayData) )
	{
		auto displayData = static_cast<RE::ExtraTextDisplayData*> (refForm->extraList.GetByType(RE::ExtraDataType::kTextDisplayData) );

		name = displayData->displayName.c_str();
	}
	else
	{
		auto fullName = baseForm ? baseForm->As<RE::TESFullName>() : nullptr;
		if (fullName)
		{
			name = fullName->fullName.c_str();
		}
		else
		{
			//Handle special cases
			switch (baseForm->GetFormType())
			{
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
		}

		}
	}

	//If the name is empty try getting the editor id
	if( name == "")
	{
		if (!MICOptions::DisableEditorIDs)
		{
			auto editorIDCache = EditorIDCache::GetSingleton();

			name = editorIDCache->GetEditorID(baseForm);

			//if the editor id was not found use the form id as a final backup
			if (name == "")
			{
				name = FormIDToString(baseForm->formID);
			}
		}
	}

	logger::debug( "GetExtraData: GetName End: {}", name );
	
	return name;
}

//Get the name of a texture type in a BGSTextureSet object
std::string GetTextureType(int textureType)
{
	std::string textureTypeName = "";

	switch (textureType)
	{
	case RE::BGSTextureSet::Texture::kDiffuse: textureTypeName = GetTranslation("$TextureSetDiffuse");
		break;
	case RE::BGSTextureSet::Texture::kNormal: textureTypeName = GetTranslation("$TextureSetNormalGloss");
		break;
	case RE::BGSTextureSet::Texture::kEnvironmentMask: textureTypeName = GetTranslation("$TextureSetEnvironmentMaskSubsurface");
		break;
	case RE::BGSTextureSet::Texture::kGlowMap: textureTypeName = GetTranslation("$TextureSetGlowDetail");
		break;
	case RE::BGSTextureSet::Texture::kHeight: textureTypeName = GetTranslation("$TextureSetHeight");
		break;
	case RE::BGSTextureSet::Texture::kEnvironment: textureTypeName = GetTranslation("$TextureSetEnvironment");
		break;
	case RE::BGSTextureSet::Texture::kMultilayer: textureTypeName = GetTranslation("$TextureSetMultilayer");
		break;
	case RE::BGSTextureSet::Texture::kBacklightMask: textureTypeName = GetTranslation("$TextureSetBacklitSpecular");
		break;
	default: textureTypeName = GetTranslation("$Unknown");
		break;
	}

	return textureTypeName;
}


std::string GetArmorTypeName(RE::TESObjectARMO::ArmorType armorType )
{
	std::string armorTypeName = "";

	switch (armorType)
	{
	case RE::TESObjectARMO::ArmorType::kClothing: armorTypeName = GetTranslation("$ArmorTypeClothes");
		break;
	case RE::TESObjectARMO::ArmorType::kLightArmor: armorTypeName = GetTranslation("$ArmorTypeLight");
		break;
	case RE::TESObjectARMO::ArmorType::kHeavyArmor: armorTypeName = GetTranslation("$ArmorTypeHeavy");
		break;
	default: armorTypeName = GetTranslation("$Unknown");
		break;
	}

	return armorTypeName;
}


//Get the name of a equip slot. slot is based on the bit flag for the slot
std::string GetEquipSlotName(int slot)
{
	std::string equipSlotKey = "$EquipSlot" + IntToString( slot );
	return GetTranslation(equipSlotKey);
}

std::string GetActorValueName(int id)
{
	//its expected that sometimes we will be passed a -1 id, for cases like effects that have no actor value defined for the primary av
	std::string actorValueKey = id >= 0 ? "$ActorValue" + IntToString(id) : "$Unknown";
	return GetTranslation(actorValueKey);
}


std::string GetEffectTypeName(int id)
{
	std::string effectTypeKey = "$MagicEffectType" + IntToString(id);
	return GetTranslation(effectTypeKey);
}

std::string GetSpellTypeName(RE::MagicSystem::SpellType spellType)
{
	std::string spellTypeName = "";

	switch (spellType)
	{
	case RE::MagicSystem::SpellType::kSpell: spellTypeName = GetTranslation("$SpellTypeSpell");
		break;
	case RE::MagicSystem::SpellType::kDisease: spellTypeName = GetTranslation("$SpellTypeDisease");
		break;
	case RE::MagicSystem::SpellType::kPower: spellTypeName = GetTranslation("$SpellTypePower");
		break;
	case RE::MagicSystem::SpellType::kLesserPower: spellTypeName = GetTranslation("$SpellTypeLesserPower");
		break;
	case RE::MagicSystem::SpellType::kAbility: spellTypeName = GetTranslation("$SpellTypeAbility");
		break;
	case RE::MagicSystem::SpellType::kPoison: spellTypeName = GetTranslation("$SpellTypePoison");
		break;
	case RE::MagicSystem::SpellType::kEnchantment: spellTypeName = GetTranslation("$SpellTypeEnchantment");
		break;
	case RE::MagicSystem::SpellType::kIngredient: spellTypeName = GetTranslation("$SpellTypeIngredient");
		break;
	case RE::MagicSystem::SpellType::kLeveledSpell: spellTypeName = GetTranslation("$SpellTypeLeveledSpell");
		break;
	case RE::MagicSystem::SpellType::kAddiction: spellTypeName = GetTranslation("$SpellTypeAddiction");
		break;
	case RE::MagicSystem::SpellType::kVoicePower: spellTypeName = GetTranslation("$SpellTypeVoice");
		break;
	case RE::MagicSystem::SpellType::kStaffEnchantment: spellTypeName = GetTranslation("$SpellTypeStaffEnchantment");
		break;
	case RE::MagicSystem::SpellType::kScroll: spellTypeName = GetTranslation("$SpellTypeScroll");
		break;
	default: spellTypeName = GetTranslation("$Unknown");
		break;
	}

	return spellTypeName;
}

std::string GetCastingTypeName(RE::MagicSystem::CastingType castingType)
{
	std::string castingTypeName = "";

	switch (castingType)
	{
	case RE::MagicSystem::CastingType::kConcentration: castingTypeName = GetTranslation("$CastingTypeConcentration");
		break;
	case RE::MagicSystem::CastingType::kConstantEffect: castingTypeName = GetTranslation("$CastingTypeConstantEffect");
		break;
	case RE::MagicSystem::CastingType::kFireAndForget: castingTypeName = GetTranslation("$CastingTypeFireForget");
		break;
	case RE::MagicSystem::CastingType::kScroll: castingTypeName = GetTranslation("$CastingTypeScroll");
		break;
	default: castingTypeName = GetTranslation("$Unknown");
		break;
	}

	return castingTypeName;
}

std::string GetDeliveryTypeName(RE::MagicSystem::Delivery deliveryType)
{
	std::string deliveryTypeName = "";

	switch (deliveryType)
	{
	case RE::MagicSystem::Delivery::kAimed: deliveryTypeName = GetTranslation("$DeliveryTypeAimed");
		break;
	case RE::MagicSystem::Delivery::kTouch: deliveryTypeName = GetTranslation("$DeliveryTypeContact");
		break;
	case RE::MagicSystem::Delivery::kSelf: deliveryTypeName = GetTranslation("$DeliveryTypeSelf");
		break;
	case RE::MagicSystem::Delivery::kTargetActor: deliveryTypeName = GetTranslation("$DeliveryTypeTargetActor");
		break;
	case RE::MagicSystem::Delivery::kTargetLocation: deliveryTypeName = GetTranslation("$DeliveryTypeTargetLocation");
		break;
	default: deliveryTypeName = GetTranslation("$Unknown");
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
			weaponTypeName = GetTranslation("$WeaponTypeHandToHand");
			break;
		}

		case RE::WEAPON_TYPE::kOneHandSword:
		{
			weaponTypeName = GetTranslation("$WeaponTypeOneHandedSword");
			break;
		}

		case RE::WEAPON_TYPE::kOneHandDagger:
		{
			weaponTypeName = GetTranslation("$WeaponTypeDagger");
			break;
		}

		case RE::WEAPON_TYPE::kOneHandAxe:
		{
			weaponTypeName = GetTranslation("$WeaponTypeOneHandedAxe");
			break;
		}

		case RE::WEAPON_TYPE::kTwoHandSword:
		{
			weaponTypeName = GetTranslation("$WeaponTypeTwoHandedSword");
			break;
		}

		case RE::WEAPON_TYPE::kTwoHandAxe:
		{
			weaponTypeName = GetTranslation("$WeaponTypeTwoHandedAxe");
			break;
		}

		case RE::WEAPON_TYPE::kBow:
		{
			weaponTypeName = GetTranslation("$WeaponTypeBow");
			break;
		}

		case RE::WEAPON_TYPE::kStaff:
		{
			weaponTypeName = GetTranslation("$WeaponTypeStaff");
			break;
		}

		case RE::WEAPON_TYPE::kCrossbow:
		{
			weaponTypeName = GetTranslation("$WeaponTypeCrossbow");
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
		equipTypeName = GetTranslation("$EquipTypeRightHand");
		break;
	}
	case 0x00013F43:
	{
		equipTypeName = GetTranslation("$EquipTypeLeftHand");
		break;
	}
	case 0x00013F44:
	{
		equipTypeName = GetTranslation("$EquipTypeEitherHand");
		break;
	}
	case 0x00013F45:
	{
		equipTypeName = GetTranslation("$EquipTypeBothHand");
		break;
	}
	case 0x000141E8:
	{
		equipTypeName = GetTranslation("$EquipTypeShield");
		break;
	}
	case 0x00025BEE:
	{
		equipTypeName = GetTranslation("$EquipTypeVoice");
		break;
	}
	case 0x00035698:
	{
		equipTypeName = GetTranslation("$EquipTypePotion");
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
		lockLevelString = GetTranslation("$LockUnlocked");
		break;
	case RE::LOCK_LEVEL::kVeryEasy:
		lockLevelString = GetTranslation("$Novice");
		break;
	case RE::LOCK_LEVEL::kEasy:
		lockLevelString = GetTranslation("$Apprentice");
		break;
	case RE::LOCK_LEVEL::kAverage:
		lockLevelString = GetTranslation("$Adept");
		break;
	case RE::LOCK_LEVEL::kHard:
		lockLevelString = GetTranslation("$Expert");
		break;
	case RE::LOCK_LEVEL::kVeryHard:
		lockLevelString = GetTranslation("$Master");
		break;
	case RE::LOCK_LEVEL::kRequiresKey:
		lockLevelString = GetTranslation("$LockRequiresKey");
		break;
	default: 
		lockLevelString = GetTranslation("$Unknown");
		break;
	}

	return lockLevelString;
}

std::string GetMFGExpressionName(int expressionIndex)
{
	std::string expressionKey = "$mfgExpression" + IntToString(expressionIndex);
	return GetTranslation(expressionKey);
}

std::string GetMFGModiferName(int modifierIndex)
{
	std::string modifierKey = "$mfgModifier" + IntToString(modifierIndex);
	return GetTranslation(modifierKey);
}

std::string GetMFGPhenomeName(int phenomeIndex)
{
	std::string phenomeKey = "$mfgPhoneme" + IntToString(phenomeIndex);
	return GetTranslation(phenomeKey);
}
