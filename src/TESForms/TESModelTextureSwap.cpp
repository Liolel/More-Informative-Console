#pragma once
#include "TESModelTextureSwap.h"
#include "Util/NameUtil.h"
#include "Util/GeneralUtil.h"
#include "Util/FilePathUtil.h"
#include "TranslationCache.h"
#include "TESForm.h"

//4-30-2022: Checked for translations needed

void GetModelTextures(ExtraInfoEntry* resultArray, RE::TESForm* baseForm )
{
	//logger::debug(("Starting GetModelTextures " + GetFormTypeName((int)baseForm->formType.underlying())).c_str());
	switch (baseForm->GetFormType())
	{
		case RE::FormType::Static:
		{
			RE::TESObjectSTAT* staticForm = static_cast<RE::TESObjectSTAT*>(baseForm);

			if (staticForm)
			{
				RE::TESModelTextureSwap* textSwap = staticForm->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::MovableStatic:
		{
			RE::BGSMovableStatic* movableStatic = static_cast<RE::BGSMovableStatic*>(baseForm);

			if (movableStatic)
			{
				RE::TESModelTextureSwap* textSwap = movableStatic->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Activator:
		{
			RE::TESObjectACTI* activator = static_cast<RE::TESObjectACTI*>(baseForm);

			if (activator)
			{
				RE::TESModelTextureSwap* textSwap = activator->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Tree:
		{
			RE::TESModel* tree = static_cast<RE::TESObjectTREE*>(baseForm);

			if (tree)
			{
				AddModelEntry(resultArray, GetTranslation("$Model"), tree); //For some reason trees don't have a ModelTextureSwap
			}

			break;
		}

		case RE::FormType::Flora:
		{
			RE::TESFlora* flora = static_cast<RE::TESFlora*>(baseForm);

			if (flora)
			{
				RE::TESModelTextureSwap* textSwap = flora->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Furniture:
		{
			RE::TESFurniture* funiture = static_cast<RE::TESFurniture*>(baseForm);

			if (funiture)
			{
				RE::TESModelTextureSwap* textSwap = funiture->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Door:
		{
			RE::TESObjectDOOR* door = static_cast<RE::TESObjectDOOR*>(baseForm);

			if (door)
			{
				RE::TESModelTextureSwap* textSwap = door->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Container:
		{
			RE::TESObjectCONT* container = static_cast<RE::TESObjectCONT*>(baseForm);

			if (container)
			{
				RE::TESModelTextureSwap* textSwap = container->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Misc:
		{
			RE::TESObjectMISC* misc = static_cast<RE::TESObjectMISC*>(baseForm);

			if (misc)
			{
				RE::TESModelTextureSwap* textSwap = misc->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Book:
		{
			RE::TESObjectBOOK* book = static_cast<RE::TESObjectBOOK*>(baseForm);

			if (book)
			{
				RE::TESModelTextureSwap* textSwap = book->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::KeyMaster:
		{
			RE::TESKey* key = static_cast<RE::TESKey*>(baseForm);

			if (key)
			{
				RE::TESModelTextureSwap* textSwap = key->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::SoulGem:
		{
			RE::TESSoulGem* soulgem = static_cast<RE::TESSoulGem*>(baseForm);

			if (soulgem)
			{
				RE::TESModelTextureSwap* textSwap = soulgem->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Ingredient:
		{
			RE::IngredientItem* ingredient = static_cast<RE::IngredientItem*>(baseForm);

			if (ingredient)
			{
				RE::TESModelTextureSwap* textSwap = ingredient->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::AlchemyItem:
		{
			RE::AlchemyItem* alchemyItem = static_cast<RE::AlchemyItem*>(baseForm);

			if (alchemyItem)
			{
				RE::TESModelTextureSwap* textSwap = alchemyItem->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Light:
		{
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(baseForm);

			if (light)
			{
				RE::TESModelTextureSwap* textSwap = light->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwap);
			}

			break;
		}

		case RE::FormType::Weapon:
		{
			RE::TESObjectWEAP* weapon = static_cast<RE::TESObjectWEAP*>(baseForm);

			if (weapon)
			{
				RE::TESModelTextureSwap* textSwapModel = weapon->GetAsModelTextureSwap();
				AddModelEntry(resultArray, GetTranslation("$Model"), textSwapModel);

				//Get first person model
				RE::TESObjectSTAT* firstPersonModel = weapon->firstPersonModelObject;
				if (firstPersonModel)
				{
					RE::TESModelTextureSwap* textSwapFirstPersonModel = firstPersonModel->GetAsModelTextureSwap();
					AddModelEntry(resultArray, GetTranslation("$ModelFirstPerson"), textSwapFirstPersonModel);
				}

			}

			break;
		}

		case RE::FormType::Armature:
		{
			RE::TESObjectARMA* arma = static_cast<RE::TESObjectARMA*>(baseForm);
			if (arma)
			{
				RE::TESModelTextureSwap* maleModel = &(arma->bipedModels[RE::SEXES::kMale]);

				if (maleModel)
				{
					AddModelEntry(resultArray, GetTranslation("$ModelMale"), maleModel);
				}

				RE::TESModelTextureSwap* femaleModel = &(arma->bipedModels[RE::SEXES::kFemale]);
				if (femaleModel)
				{
					AddModelEntry(resultArray, GetTranslation("$ModelFemale"), femaleModel);
				}

				RE::TESModelTextureSwap* maleFirstPerson = &(arma->bipedModel1stPersons[RE::SEXES::kMale]);
				if (maleFirstPerson)
				{
					AddModelEntry(resultArray, GetTranslation("$ModelMaleFirstPerson"), maleFirstPerson);
				}

				RE::TESModelTextureSwap* femaleFirstPerson = &(arma->bipedModel1stPersons[RE::SEXES::kFemale]);
				if (femaleFirstPerson)
				{
					AddModelEntry(resultArray, GetTranslation("$ModelFemaleFirstPerson"), femaleFirstPerson);
				}
			}

			break;
		}
	}
	logger::debug("Ending GetModelTextures");
}


void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModelTextureSwap* modelTextureSwap)
{
	logger::debug("Starting AddModelEntry for modelTextureSwap");

	if (modelTextureSwap)
	{
		//Get the model and any TextureSet if a texture set exists
		logger::debug("Past modelTextureSwap");
		RE::TESModel* model = modelTextureSwap;

		logger::debug("Past conversion");

		RE::BGSTextureSet* textureSet = nullptr;

		if (modelTextureSwap->alternateTextures)
		{
			logger::debug("Inside swaps check");
			textureSet = modelTextureSwap->alternateTextures->textureSet;
		}

		logger::debug("Past swaps check");

		//If there is a texture set we need to retrieve the textures in the set
		if (textureSet)
		{
			logger::debug("Starting Texture Set Branch");

			ExtraInfoEntry* modelTextureEntry;

			std::string modelPath = model->GetModel();
			std::string modelName = GetFileName(modelPath);

			CreateExtraInfoEntry(modelTextureEntry, modelType, modelName, priority_Model);

			AddModelEntry(modelTextureEntry, GetTranslation("$Model"), model);

			logger::debug("Starting Texture Set Info");
			ExtraInfoEntry* textureSetEntry;

			CreateExtraInfoEntry(textureSetEntry, GetTranslation("$TextureSet"), "", priority_TextureSet);

			GetFormData(textureSetEntry, textureSet, nullptr);

			modelTextureEntry->PushBack(textureSetEntry);

			resultArray->PushBack(modelTextureEntry);
		}

		else //If no texture set exists call code for just a model
		{
			logger::debug("Starting No texture set branch");
			AddModelEntry(resultArray, modelType, model);
		}
	}

	logger::debug("Ending AddModelEntry for modelTextureSwap");
}
