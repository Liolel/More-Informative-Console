#include "TESObjectREFR.h"
#include "BSExtraData/BSExtraData.h"
#include "Util/GeneralUtil.h"
#include "EditorIDCache.h"
#include "Util/FilePathUtil.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void GetReferenceFormData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	logger::debug("GetReferenceFormData Start");

	//get the editor id if there is one
	auto editorIDCache = EditorIDCache::GetSingleton();
	std::string editorID = editorIDCache->GetEditorID(refForm);

	if (editorID != "" )
	{
		ExtraInfoEntry* referenceEditorIDEntry;
		CreateExtraInfoEntry(referenceEditorIDEntry, GetTranslation("$ReferenceEditorID"), editorID, priority_EditorIDReference);
		resultArray->PushBack(referenceEditorIDEntry);
	}

	GetPositionData(resultArray, refForm);
	GetTextures(resultArray, refForm);

	//Check if the refrence is Enabled
	bool isDisabled = refForm->IsDisabled();

	ExtraInfoEntry* enabledStatusEntry;
	std::string enabledStatus = BooleanToYesNoString(!isDisabled);
	CreateExtraInfoEntry(enabledStatusEntry, GetTranslation("$ReferenceIsEnabled"), enabledStatus, priority_Reference_Enabled);

	resultArray->PushBack(enabledStatusEntry);

	GetBSExtraData(resultArray, refForm);
}

void GetPositionData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	logger::debug("Starting GetPositionData");

	ExtraInfoEntry* positionEntry;
	CreateExtraInfoEntry(positionEntry, GetTranslation("$ReferencePosition"), "", priority_Reference_Location);

	//position
	float xPos = refForm->data.location.x;
	ExtraInfoEntry* xPositionEntry;
	CreateExtraInfoEntry(xPositionEntry, GetTranslation("$ReferencePositionX"), FloatToString(xPos), priority_Position_XCoordinate);
	positionEntry->PushBack(xPositionEntry);

	float yPos = refForm->data.location.y;
	ExtraInfoEntry* yPositionEntry;
	CreateExtraInfoEntry(yPositionEntry, GetTranslation("$ReferencePositionY"), FloatToString(yPos), priority_Position_YCoordinate);
	positionEntry->PushBack(yPositionEntry);

	float zPos = refForm->data.location.z;
	ExtraInfoEntry* zPositionEntry;
	CreateExtraInfoEntry(zPositionEntry, GetTranslation("$ReferencePositionZ"), FloatToString(zPos), priority_Position_ZCoordinate);
	positionEntry->PushBack(zPositionEntry);

	//rotation
	float xRot = refForm->data.angle.x;
	ExtraInfoEntry* xRotationEntry;
	CreateExtraInfoEntry(xRotationEntry, GetTranslation("$ReferenceRotationX"), FloatToString(xRot), priority_Position_XRotation);
	positionEntry->PushBack(xRotationEntry);

	float yRot = refForm->data.angle.y;
	ExtraInfoEntry* yRotationEntry;
	CreateExtraInfoEntry(yRotationEntry, GetTranslation("$ReferenceRotationY"), FloatToString(yRot), priority_Position_YRotation);
	positionEntry->PushBack(yRotationEntry);

	float zRot = refForm->data.angle.z;
	ExtraInfoEntry* zRotationEntry;
	CreateExtraInfoEntry(zRotationEntry, GetTranslation("$ReferenceRotationZ"), FloatToString(zRot), priority_Position_ZRotation);
	positionEntry->PushBack(zRotationEntry);

	resultArray->PushBack(positionEntry);

	logger::debug("Ending GetPositionData");
}


void GetTextures(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	logger::debug("Starting GetTextures");
	auto reference3d = refForm->Get3D();

	if (reference3d)
	{
		std::set<std::string> uniqueFilePaths;
		//vist each geometry
		RE::BSVisit::TraverseScenegraphGeometries(reference3d, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl
		{
			//go from the geometry object to the texture set if it has one
			const auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
			const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
			if (lightingShader)
			{
				const auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material)
				{
					if (const auto textureSet = material->textureSet; textureSet)
					{
						//there are 8 distinct texture categories
						for (int i = 0; i < 8; i++)
						{
							auto sourcePath = textureSet->GetTexturePath((RE::BSTextureSet::Texture)i);

							if (sourcePath)
							{
								uniqueFilePaths.insert(sourcePath);
							}
						}
					}
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});

		if (!uniqueFilePaths.empty())
		{
			ExtraInfoEntry* texturesEntry;
			CreateExtraInfoEntry(texturesEntry, GetTranslation("$Textures"), "", priority_TextureSet);

			for (auto itr = uniqueFilePaths.begin(); itr != uniqueFilePaths.end(); itr++)
			{
				ExtraInfoEntry* textureEntry;
				std::string filePath = *itr;
				std::string fileName = GetFileName(filePath);

				CreateExtraInfoEntry(textureEntry, fileName, "", priority_Texture);
				CreateFilePathSubarray(textureEntry, filePath);
				texturesEntry->PushBack(textureEntry);
			}

			resultArray->PushBack(texturesEntry);
		}
	}

	logger::debug("Ending GetTextures");
}
