#include "BGSTextureSet.h"
#include "Util/FilePathUtil.h"
#include "Util/NameUtil.h"
#include "TranslationCache.h"

//4-24-2022: Checked for translations needed

void GetTextureSet(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	logger::debug("Starting AddTextureSetEntry");

	RE::BGSTextureSet* textureSet = static_cast<RE::BGSTextureSet*>(baseForm);

	if (textureSet) {
		ExtraInfoEntry* textureSetEntry;
		CreateExtraInfoEntry(textureSetEntry, GetTranslation("$TextureSet"), "", priority_TextureSet);

		int totalTextures = RE::BGSTextureSet::Textures::kUsedTotal;

		for (int i = 0; i < totalTextures; i++) {
			RE::TESTexture* texture = &textureSet->textures[i];
			std::string texturePath = texture->textureName.c_str();

			std::string textureName = "";

			if (texturePath != "") {
				textureName = GetFileName(texturePath);
			}

			std::string textureType = GetTextureType(i);

			ExtraInfoEntry* textureEntry;

			CreateExtraInfoEntry(textureEntry, textureType, textureName, priority_TextureSet_TextureName);

			if (texturePath != "") {
				CreateFilePathSubarray(textureEntry, texturePath);
			}

			textureSetEntry->PushBack(textureEntry);
		}

		resultArray->PushBack(textureSetEntry);
	}

	logger::debug("Ending AddTextureSetEntry");
}
