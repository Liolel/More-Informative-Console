#pragma once
#include "TESModelTextureSwap.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "MoreInformativeConsole/Util/FilePathUtil.h"
#include "TESForm.h"

void GetModelTextures(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	_DMESSAGE( ("Starting GetModelTextures " + GetFormTypeName((int)baseForm->formType) ).c_str() );
	if (baseForm->formType == RE::FormType::Static)
	{
		RE::TESObjectSTAT* staticForm = static_cast<RE::TESObjectSTAT*>(baseForm);

		if (staticForm)
		{
			RE::TESModelTextureSwap* textSwap = staticForm->GetAsModelTextureSwap();
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (baseForm->formType == RE::FormType::MovableStatic)
	{
		RE::BGSMovableStatic* movableStatic = static_cast<RE::BGSMovableStatic*>(baseForm);

		if (movableStatic)
		{
			RE::TESModelTextureSwap* textSwap = movableStatic->GetAsModelTextureSwap();
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (baseForm->formType == RE::FormType::Activator)
	{
		RE::TESObjectACTI* activator = static_cast<RE::TESObjectACTI*>(baseForm);

		if (activator)
		{
			RE::TESModelTextureSwap* textSwap = activator->GetAsModelTextureSwap();
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	/*


	else if (pBaseForm->formType == kFormType_Tree)
	{
		TESObjectTREE* pTree = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectTREE);

		if (pTree)
		{
			TESModel* model = &(pTree->model);
			AddModelEntry(resultArray, "Model", model);
		}
	}

	else if (pBaseForm->formType == kFormType_Flora)
	{
		TESFlora* pFlora = DYNAMIC_CAST(pBaseForm, TESForm, TESFlora);

		if (pFlora)
		{
			TESModelTextureSwap* textSwap = &(pFlora->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Furniture)
	{
		TESFurniture* pFurniture = DYNAMIC_CAST(pBaseForm, TESForm, TESFurniture);

		if (pFurniture)
		{
			TESModelTextureSwap* textSwap = &(pFurniture->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Door)
	{
		TESObjectDOOR* pDoor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectDOOR);
		if (pDoor)
		{
			TESModelTextureSwap* textSwap = &(pDoor->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Container)
	{
		TESObjectCONT* pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			TESModelTextureSwap* textSwap = &(pContainer->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Misc)
	{
		TESObjectMISC* pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
		if (pMisc)
		{
			TESModelTextureSwap* textSwap = &(pMisc->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Book)
	{
		TESObjectBOOK* pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
		if (pBook)
		{
			TESModelTextureSwap* textSwap = &(pBook->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Key)
	{
		TESKey* pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
		if (pKey)
		{
			TESModelTextureSwap* textSwap = &(pKey->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_SoulGem)
	{
		TESSoulGem* pSoulGem = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
		if (pSoulGem)
		{
			TESModelTextureSwap* textSwap = &(pSoulGem->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Ingredient)
	{
		IngredientItem* pIngredient = DYNAMIC_CAST(pBaseForm, TESForm, IngredientItem);
		if (pIngredient)
		{
			TESModelTextureSwap* textSwap = &(pIngredient->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}

	else if (pBaseForm->formType == kFormType_Potion)
	{
		AlchemyItem* pAlchemy = DYNAMIC_CAST(pBaseForm, TESForm, AlchemyItem);
		if (pAlchemy)
		{
			TESModelTextureSwap* textSwap = &(pAlchemy->texSwap);
			AddModelEntry(resultArray, "Model", textSwap);
		}
	}


	else if (pBaseForm->formType == kFormType_Weapon)
	{
		TESObjectWEAP* pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			TESModelTextureSwap* textSwap = &(pWeapon->texSwap);

			if (textSwap)
			{
				AddModelEntry(resultArray, "Model", textSwap);
			}

			TESObjectSTAT* firstPersonModel = pWeapon->model;

			if (firstPersonModel)
			{
				TESModelTextureSwap* textSwapFirstPerson = &(firstPersonModel->texSwap);

				if (textSwapFirstPerson)
				{
					AddModelEntry(resultArray, "First Person Model", textSwapFirstPerson);
				}
			}
		}
	}

	else if (pBaseForm->formType == kFormType_ARMA)
	{
		TESObjectARMA* pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);
		if (pArma)
		{
			//Texture entries are theortical
			TESModelTextureSwap* maleModel = &(pArma->models[0][0]);

			if (maleModel)
			{
				AddModelEntry(resultArray, "Model Male", maleModel);
			}

			TESModelTextureSwap* femaleModel = &(pArma->models[0][1]);
			if (femaleModel)
			{
				AddModelEntry(resultArray, "Model Female", femaleModel);
			}

			TESModelTextureSwap* maleFirstPerson = &(pArma->models[1][0]);
			if (maleFirstPerson)
			{
				AddModelEntry(resultArray, "Model Male 1st Person", maleFirstPerson);
			}

			TESModelTextureSwap* femaleFirstPerson = &(pArma->models[1][1]);
			if (femaleFirstPerson)
			{
				AddModelEntry(resultArray, "Model Female 1st Person", femaleFirstPerson);
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
	

	_DMESSAGE("Ending GetModelTextures");
}


void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModelTextureSwap* modelTextureSwap)
{
	_DMESSAGE("Starting AddModelEntry for modelTextureSwap");

	if (modelTextureSwap)
	{
		//Get the model and any TextureSet if a texture set exists
		_DMESSAGE("Past modelTextureSwap");
		RE::TESModel* model = modelTextureSwap;

		_DMESSAGE("Past conversion");

		RE::BGSTextureSet* textureSet = nullptr;

		if (modelTextureSwap->alternateTextures)
		{
			_DMESSAGE("Inside swaps check");
			textureSet = modelTextureSwap->alternateTextures->textureSet;
		}

		_DMESSAGE("Past swaps check");

		if (textureSet)
		{
			_DMESSAGE("Starting Texture Set Branch");

			ExtraInfoEntry* modelTextureEntry;

			std::string modelPath = model->GetModel();
			std::string modelName = GetFileName(modelPath);

			CreateExtraInfoEntry(modelTextureEntry, modelType, modelName, priority_Model);

			AddModelEntry(modelTextureEntry, "Model", model);

			_DMESSAGE("Starting Texture Set Info");
			ExtraInfoEntry* textureSetEntry;

			CreateExtraInfoEntry(textureSetEntry, "Texture Set", "", priority_TextureSet);

			GetFormData(textureSetEntry, textureSet, nullptr);

			modelTextureEntry->PushBack(textureSetEntry);

			resultArray->PushBack(modelTextureEntry);
		}

		else //If no texture set exists call code for just a model
		{
			_DMESSAGE("Starting No texture set branch");
			AddModelEntry(resultArray, modelType, model);
		}
	}

	_DMESSAGE("Ending AddModelEntry for modelTextureSwap");
}