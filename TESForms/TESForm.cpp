#include "TESForm.h"


bool GetHasSourceFileArray(RE::TESForm* form)
{
	return form->sourceFiles.array;	//Check if the source files array exists
}

std::string GetNthFormLocationName(RE::TESForm* form, UInt32 n)
{
	std::string formName = "Unknown";

	if (GetHasSourceFileArray( form )
		&& form->sourceFiles.array->size() > n)
	{
		RE::TESFile** sourceFiles = form->sourceFiles.array->data();
		formName = sourceFiles[n]->fileName;
	}

	return formName;
}

std::string GetFirstFormLocationName(RE::TESForm* form)
{
	return GetNthFormLocationName(form, 0);
}

std::string GetLastFormLocationName(RE::TESForm* form)
{
	std::string formName = "Unknown";

	if( GetHasSourceFileArray(form) )
	{
		UInt32 numberOfSourceFiles = form->sourceFiles.array->size();
		formName = GetNthFormLocationName( form, numberOfSourceFiles - 1 );
	}

	return formName;
}