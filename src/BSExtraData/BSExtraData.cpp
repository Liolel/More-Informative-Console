#include "BSExtraData.h"
#include "ExtraOwnership.h"
#include "ExtraEnableStateParent.h"
#include "ExtraContainerChanges.h"
#include "ExtraLocation.h"
#include "ExtraLock.h"
#include "ExtraRegionList.h"
#include "ExtraTeleport.h"
#include "ExtraAliasInstanceArray.h"
#include "TESForms/TESForm.h"
#include "TESForms/EnchantmentItem.h"
#include "globals.h"
#include "TranslationCache.h"
#include "Util/NameUtil.h"


//4-30-2022: Checked for translations needed

const int numberOfExtraDataTypes = 0xBF + 1; //The plus 1 is because there are extra data types from 0 up to and including BF

//get data stored in the BSExtraData format
void GetBSExtraData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	logger::debug("Starting GetBSExtraData");

	RE::ExtraDataList* extraList = &refForm->extraList;

	ProcessExtraDataList(resultArray, extraList, refForm);

	logger::debug("Ending GetBSExtraData");
}

void ProcessExtraDataList(ExtraInfoEntry* resultArray, RE::ExtraDataList* extraList, RE::TESObjectREFR* refForm)
{
	if (extraList)
	{
		if (extraList->HasType(RE::ExtraDataType::kEnableStateParent) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kEnableStateParent);
			ProcessEnableParentInformation(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kOwnership) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kOwnership);
			ProcessOwnership(resultArray, data);
		}

		if (extraList->HasType( RE::ExtraDataType::kContainerChanges )
			&& refForm != nullptr )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kContainerChanges);
			ProcessContainerChanges(resultArray, data, refForm);
		}
		
		if (extraList->HasType( RE::ExtraDataType::kLock )
			&& refForm != nullptr )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kLock);
			ProcessLockData(resultArray, data, refForm);
		}

		if (extraList->HasType( RE::ExtraDataType::kTeleport )
			&& refForm != nullptr )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kTeleport);
			ProcessTeleportData(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kLocation) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kLocation);
			ProcessLocationData(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kRegionList))
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kRegionList);
			ProcessRegionList(resultArray, data);
		}

		if ( extraList->HasType(RE::ExtraDataType::kAliasInstanceArray )
			 && ( !MICOptions::DisableScriptsAliases  //if getting aliases are allowed
				  && ( refForm->formID != 0x0000014 || 
					   !MICOptions::DisableScriptsAliasesPlayerOnly) ) )
		{
			RE::BSExtraData* data = extraList->GetByType(RE::ExtraDataType::kAliasInstanceArray);
			ProcessExtraAliasInstanceArray(resultArray, data);
		}

		if (extraList->HasType(RE::ExtraDataType::kEnchantment))
		{
			auto enchantmentExtra = extraList->GetByType<RE::ExtraEnchantment>();
			if (enchantmentExtra)
			{
				auto enchantmentForm = enchantmentExtra->enchantment;

				if (enchantmentForm)
				{
					ExtraInfoEntry* enchantmentEntry;
					std::string enchantmentName = GetName(enchantmentForm);

					CreateExtraInfoEntry(enchantmentEntry, GetTranslation("$Enchantment"), enchantmentName, priority_Enchantment);
					GetFormData(enchantmentEntry, enchantmentForm, nullptr);
					GetCharge(enchantmentEntry, &(refForm->extraList), nullptr, enchantmentExtra);

					enchantmentEntry->SetMayCopy(false);
					resultArray->PushBack(enchantmentEntry);
				}
			}
		}

		if (extraList->HasType(RE::ExtraDataType::kCellAcousticSpace))
		{
			auto acousticSpaceExtra = extraList->GetByType<RE::ExtraCellAcousticSpace>();
			if (acousticSpaceExtra)
			{
				auto acousticSpace = acousticSpaceExtra->space;

				if (acousticSpace )
				{
					ExtraInfoEntry* acousticSpaceEntry;
					std::string acousticSpaceName = GetName(acousticSpace);

					CreateExtraInfoEntry(acousticSpaceEntry, GetTranslation("$AcousticSpace"), acousticSpaceName, priority_Enchantment);
					GetFormData(acousticSpaceEntry, acousticSpace, nullptr);

					resultArray->PushBack(acousticSpaceEntry);
				}
			}
		}


		if ( MICOptions::ExperimentalFeatures )
		{
				
			ExtraInfoEntry* extraDataTypes;
			CreateExtraInfoEntry(extraDataTypes, "$ExtraData", "", priority_ExtraData);

			for (int i = 1; i <= numberOfExtraDataTypes; i++)
			{

				//Placeholder for seeing what has editor IDs
				if (extraList->HasType( (RE::ExtraDataType)i ) )
				{
					ExtraInfoEntry* extraDataEntry;
					CreateExtraInfoEntry(extraDataEntry, GetExtraDataTypeName(i), "", priority_ExtraData);
					extraDataTypes->PushBack(extraDataEntry);
				}

			}

			resultArray->PushBack(extraDataTypes);
		}		
	}
}
