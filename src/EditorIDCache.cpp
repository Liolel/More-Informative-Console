#include "EditorIDCache.h"
#include <memory>

EditorIDCache* EditorIDCache::GetSingleton()
{
	static EditorIDCache singleton;
	return std::addressof(singleton);
}

void EditorIDCache::CacheEditorIDs()
{
	int numFormTypes = (int)RE::FormType::Max;

	this->formIDToEditorIDMap.resize(256);

	for (int i = 0; i < 256; i++)
	{
		this->formIDToEditorIDMap[i].resize(numFormTypes);
	}

	const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
	const RE::BSReadLockGuard locker{ lock };
	if (map) {
		for (auto& [id, form] : *map) {
			auto formType = form->GetFormType();
			auto editorID = id.c_str();
			int formID = form->GetFormID();
			std::uint32_t modIndex = (std::uint32_t)formID >>24;

			this->formIDToEditorIDMap[modIndex][(int)formType].emplace(formID, editorID);
		}
	}
}

std::string EditorIDCache::GetEditorID(RE::TESForm* form)
{
	auto formType = form->GetFormType();
	int formID = form->GetFormID();

	auto editorIDItr = this->cachedEditorIDs.find(formID);
	std::string editorID = editorIDItr != cachedEditorIDs.end() ? editorIDItr->second : "";

	if (editorIDItr == cachedEditorIDs.end() )
	{
		std::uint32_t modIndex = (std::uint32_t)formID >> 24;

		auto mapForFormType = this->formIDToEditorIDMap[modIndex][(int)formType];

		editorIDItr = mapForFormType.find(formID);
		editorID = editorIDItr != mapForFormType.end() ? editorIDItr->second : "";

		this->cachedEditorIDs.emplace(formID, editorID);
	}


	return editorID;
}
