#include "ScaleformUtil.h"

void RegisterString(RE::GFxValue* object, RE::GFxMovie* movie, std::string stringName, std::string stringContents)
{
	RE::GFxValue gfxString;
	movie->CreateString(&gfxString, stringContents.c_str() );

	object->SetMember(stringName.c_str(), gfxString);
}


void RegisterNumber(RE::GFxValue* object, std::string numberName, double number)
{
	RE::GFxValue gfxNumber;
	gfxNumber.SetNumber(number);

	object->SetMember(numberName.c_str(), gfxNumber);
}