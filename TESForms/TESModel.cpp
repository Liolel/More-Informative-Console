#include "TESModel.h"
#include "MoreInformativeConsole/Util/FilePathUtil.h"

void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModel* model)
{
	_DMESSAGE("Starting AddModelEntry for model");

	if (model)
	{
		std::string modelPath = model->GetModel();

		if (modelPath != "")
		{
			_DMESSAGE("Get Model path");

			std::string modelName = GetFileName(modelPath);

			_DMESSAGE("Get Model name");

			ExtraInfoEntry* modelEntry;
			CreateExtraInfoEntry(modelEntry, modelType, modelName, priority_Model);

			//Create an entry for the model path
			_DMESSAGE("Splitting Model Path");

			ExtraInfoEntry* modelPathEntry;
			CreateExtraInfoEntry(modelPathEntry, "Model Path", "", priority_Model);

			CreateFilePathSubarray(modelPathEntry, modelPath);
			modelEntry->PushBack(modelPathEntry);

			_DMESSAGE("Done Splitting Model Path");

			//Add textures
			int numberOfTextures = model->numTextures;

			if (numberOfTextures > 0)
			{
				ExtraInfoEntry* modelPathEntry;
				CreateExtraInfoEntry(modelPathEntry, "Model Path", "", priority_Texture);

				for (int i = 0; i < numberOfTextures; i++)
				{

				}
			}

			resultArray->PushBack(modelEntry);

			RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
			
			_DMESSAGE(IntToString( handler->compiledFileCollection.smallFiles.size()).c_str() ) ;
			//_DMESSAGE(IntToString(handler->files.end).c_str());
		}
	}

	_DMESSAGE("Ending AddModelEntry for model");
}
/*
void AddTextureEntry(ExtraInfoEntry* resultArray, RE::TESModel* model, int textureNumber)
{
	RE::BSResource::ID* texture = model->textures[textureNumber];
}*/