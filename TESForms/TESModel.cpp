#include "TESModel.h"
#include "Util/FilePathUtil.h"

void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModel* model, priority priority)
{
	logger::debug("Starting AddModelEntry for model");

	if (model)
	{
		std::string modelPath = model->GetModel();

		if (modelPath != "")
		{
			logger::debug("Get Model path");

			std::string modelName = GetFileName(modelPath);

			logger::debug("Get Model name");

			ExtraInfoEntry* modelEntry;
			CreateExtraInfoEntry(modelEntry, modelType, modelName, priority);

			//Create an entry for the model path
			logger::debug("Splitting Model Path");


			CreateFilePathSubarray(modelEntry, modelPath);

			/*
			ExtraInfoEntry* modelPathEntry;
			CreateExtraInfoEntry(modelPathEntry, "Model Path", "", priority_Model);

			
			CreateFilePathSubarray(modelPathEntry, modelPath);
			modelEntry->PushBack(modelPathEntry);

			logger::debug("Done Splitting Model Path");

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
			*/
			resultArray->PushBack(modelEntry);

			RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
			
			logger::debug(IntToString( handler->compiledFileCollection.smallFiles.size()).c_str() ) ;
			//logger::debug(IntToString(handler->files.end).c_str());
		}
	}

	logger::debug("Ending AddModelEntry for model");
}
/*
void AddTextureEntry(ExtraInfoEntry* resultArray, RE::TESModel* model, int textureNumber)
{
	RE::BSResource::ID* texture = model->textures[textureNumber];
}*/