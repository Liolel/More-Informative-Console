#pragma once

class TranslationCache
{
public:
	static TranslationCache* GetSingleton();

	void CacheTranslations();
	std::string GetTranslation(std::string key);

private:
	robin_hood::unordered_flat_map<std::string, std::string> keyToTranslationMap;
};

std::string GetTranslation(std::string key);
