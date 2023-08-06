#include "TranslationCache.h"
#include "SimpleIni.h"

TranslationCache* TranslationCache::GetSingleton()
{
	static TranslationCache singleton;
	return std::addressof(singleton);
}

void TranslationCache::CacheTranslations()
{
	//Read ini
	constexpr auto path = L"Data/SKSE/Plugins/MoreInformativeConsoleTranslations.ini";

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey( false );

	SI_Error iniError = ini.LoadFile(path);

	if (iniError < 0) {
		logger::info("Unable to read the translation file");
	}
	else
	{
		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		for (auto itrSection = sections.begin(); itrSection != sections.end(); itrSection++)
		{
			std::string section = itrSection->pItem;

			CSimpleIniA::TNamesDepend keys;
			ini.GetAllKeys(section.c_str(), keys);

			for (auto itrKey = keys.begin(); itrKey != keys.end(); itrKey++)
			{
				std::string key = itrKey->pItem;

				std::string translation = ini.GetValue(section.c_str(), key.c_str() );

				this->keyToTranslationMap.emplace(key, translation);

			}
		}
	}

	ini.Reset();
}

std::string TranslationCache::GetTranslation(std::string key)
{
	auto translationItr = keyToTranslationMap.find(key);
	std::string translation = translationItr != keyToTranslationMap.end() ? translationItr->second : "";

	if (translation == "")
	{
		logger::info("Translation Key {} not found", key );
	}

	return translation;
}


std::string GetTranslation(std::string key)
{
	auto translationCache = TranslationCache::GetSingleton();

	return translationCache->GetTranslation(key);
}
