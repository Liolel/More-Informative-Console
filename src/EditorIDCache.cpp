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

	
	this->formIDToEditorIDMap.resize(numFormTypes);

	const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
	const RE::BSReadLockGuard locker{ lock };
	if (map) {
		for (auto& [id, form] : *map) {
			auto formType = form->GetFormType();
			auto editorID = id.c_str();
			auto formID = form->GetFormID();

			this->formIDToEditorIDMap[(int)formType].emplace(formID, editorID);
		}
	}
}

std::string EditorIDCache::GetEditorID(RE::TESForm* form)
{
	auto formType = form->GetFormType();
	auto formID = form->GetFormID();

	auto mapForFormType = this->formIDToEditorIDMap[(int)formType];

	auto editorIDItr = mapForFormType.find(formID);
	std::string editorID = editorIDItr != mapForFormType.end() ? editorIDItr->second : "";

	return editorID;
}
