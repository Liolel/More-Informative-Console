#include "ScaleformUtil.h"

void RegisterString(RE::GFxValue* object, RE::GFxMovie* movie, std::string stringName, std::string stringContents)
{
	RE::GFxValue gfxString;
	movie->CreateString(&gfxString, stringContents.c_str() );

	object->SetMember(stringName.c_str(), gfxString);
}