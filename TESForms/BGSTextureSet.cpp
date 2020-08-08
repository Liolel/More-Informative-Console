#include "BGSTextureSet.h"
#include "MoreInformativeConsole/Util/FilePathUtil.h"
#include "MoreInformativeConsole/Util/NameUtil.h"

void GetTextureSet(ExtraInfoEntry* resultArray, RE::TESForm* baseForm)
{
	_DMESSAGE("Starting AddTextureSetEntry");

	RE::BGSTextureSet* textureSet = static_cast<RE::BGSTextureSet*>(baseForm);

	if (textureSet)
	{
		ExtraInfoEntry* textureSetEntry;
		CreateExtraInfoEntry(textureSetEntry, "Texture Set", "", priority_TextureSet);

		int totalTextures = RE::BGSTextureSet::Textures::kTotal;

		for (int i = 0; i < totalTextures; i++)
		{
			RE::TESTexture* texture = &textureSet->textures[i];
			std::string texturePath = texture->textureName.c_str();

			std::string textureName = "";

			if (texturePath != "")
			{
				textureName = GetFileName(texturePath);
			}

			std::string textureType = GetTextureType(i);

			ExtraInfoEntry* textureEntry;

			CreateExtraInfoEntry(textureEntry, textureType, textureName, priority_TextureSet_TextureName);

			if (texturePath != "")
			{
				CreateFilePathSubarray(textureEntry, texturePath);
			}

			textureSetEntry->PushBack(textureEntry);
		}

		resultArray->PushBack(textureSetEntry);
	}

	_DMESSAGE("Ending AddTextureSetEntry");
}