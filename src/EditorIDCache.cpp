#include "EditorIDCache.h"
#include <memory>

EditorIDCache* EditorIDCache::GetSingleton()
{
	static EditorIDCache singleton;
	return std::addressof(singleton);
}

std::string EditorIDCache::GetEditorID(RE::TESForm* form)
{
	return clib_util::editorID::get_editorID(form);
}
