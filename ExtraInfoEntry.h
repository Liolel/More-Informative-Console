#pragma once

#include <Vector>
#include "RE/Skyrim.h"
#include "Util/GeneralUtil.h"

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2, int priority);

	void Clear();
	void PushBack(ExtraInfoEntry * subArrayEntry);

	ExtraInfoEntry* GetChild(int index );
	void CreatePrimaryScaleformArray(RE::GFxValue * mainScaleFormArray, RE::GFxMovie * root);
	void CreateSecondaryScaleformArray(RE::GFxValue * scaleFormArray, RE::GFxMovie * root);

	void Finalize();

	int priority; //Priority for use in sorting. Higher Priority entires move to the top of the list

private:

	std::string entry1;
	std::string entry2;

	std::vector<ExtraInfoEntry*> subarray;
};

enum priority
{
	//General items at the type of any list
	priority_Name,
	priority_EditorID,
	priority_FormID,
	priority_FormType,
	priority_FormLocation, //Mods form is found in
	priority_Model,
	priority_Texture,
	priority_TextureSet,
	//Actors
	priority_Actor_Health,
	priority_Actor_Magicka,
	priority_Actor_Stamina,
	priority_Actor_Level,
	priority_Actor_Race,
	priority_Actor_Spells,
	priority_Actor_Spells_AddedSpell,
	priority_Actor_Spells_BaseSpell,
	priority_Actor_Effects,
	priority_Actor_ActorValues,
	priority_Actor_ActorValues_ActorValue,
	priority_Actor_IsPCLeveleMult,
	priority_Actor_IsPCLeveleMult_LevelMult,
	priority_Actor_IsPCLeveleMult_LevelMin,
	priority_Actor_IsPCLeveleMult_LevelMax,
	priority_Actor_Protection,
	priority_Actor_CurrentPackage,
	//ActorValue
	priority_ActorValue_Base,
	priority_ActorValue_Current,
	priority_ActorValue_Max,
	//Armature
	priority_Arma_SkinMale,
	priority_Arma_SkinFemale,
	priority_Arma_PrimaryRace,
	priority_Arma_AddtionalRaces,
	//Armor
	priority_Armor_ArmorType,
	priority_Armor_ArmorRating,
	priority_Armor_Value,
	priority_Armor_Weight,
	priority_Armor_EquipSlots,
	priority_Armor_ArmorAddons,
	//Race
	priority_Race_Skin,
	priority_Race_SkeletonMale,
	priority_Race_SkeletonFemale,
	priority_Race_Child,
	priority_Race_Playable,
	//Reference Specific Items
	priority_Reference_Location, //XYZ position of reference
	priority_Reference_Enabled, //Is the reference enabled or disabled
	priority_Reference_ExtraData_EnableParent, //Enable Parent Information
	//Spell
	priority_Spell_SpellType,
	priority_Spell_BaseCost,
	priority_Spell_CastTime,
	priority_Spell_DeliveryType,
	//Magic Item (This is used in Spells and ... )
	priority_MagicItem_MagicEffects,
	priority_MagicItem_Effect,
	//Effects - This is one half of magic effect data. This is the magnitude/duration/area part
	priority_Effect_Magnitude,
	priority_Effect_Duration,
	priority_Effect_Area,
	//EffectSetting
	priority_EffectSetting_EffectType,
	priority_EffectSetting_Skill,
	priority_EffectSetting_PrimaryAV,
	priority_EffectSetting_SecondaryAV,
	priority_EffectSetting_Resistance,
	priority_EffectSetting_Hostile,
	priority_EffectSetting_MinimumSkill,
	priority_EffectSetting_DeliveryType,
	//Texture Set
	priority_TextureSet_TextureSet,
	priority_TextureSet_TextureName,
	//Enable Parent Extra Data
	priority_ExtraData_EnableParent_ParentForm,
	priority_ExtraData_EnableParent_StateOppositeParent,
	priority_ExtraData_EnableParent_Popin,
	//Form Location Information
	priority_FormLocation_BaseDefined,
	priority_FormLocation_BaseLastChanged,
	priority_FormLocation_BaseInMods,
	priority_FormLocation_ReferenceDefined,
	priority_FormLocation_ReferenceLastChanged,
	priority_FormLocation_ReferenceInMods,
	//Position
	priority_Position_XCoordinate,
	priority_Position_YCoordinate,
	priority_Position_ZCoordinate,
	priority_Position_XRotation,
	priority_Position_YRotation,
	priority_Position_ZRotation,
	//Bottom of the list objects
	priority_Default
};

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents, priority priority);

//Enum for priority order Highest up takes precedence in sorting

