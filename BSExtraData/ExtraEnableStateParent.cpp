#include "ExtraEnableStateParent.h"
#include "MoreInformativeConsole/TESForms/TESForm.h"
#include "MoreInformativeConsole/Util/NameUtil.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"

void ProcessEnableParentInformation(ExtraInfoEntry* resultArray, RE::ExtraEnableStateParent* enableParentInformation)
{
	_DMESSAGE("Starting ProcessEnableParentInformation");
	ExtraInfoEntry* enableParentEntry;

	CreateExtraInfoEntry(enableParentEntry, "Enable Parent", "", priority_ExtraData_EnableParent);

	if (enableParentInformation->parent.get())
	{
		RE::TESObjectREFR* parentRefForm = enableParentInformation->parent.get().get(); //One get to get a NIPointer, one get to get the object reference
		RE::TESBoundObject* parentBaseForm = parentRefForm->data.objectReference;
		std::string parentFormName = GetName(parentBaseForm);

		ExtraInfoEntry* enableParentFormEntry;
		
		CreateExtraInfoEntry(enableParentFormEntry, "Parent Reference", parentFormName);
		GetFormData(enableParentFormEntry, parentBaseForm, parentRefForm);

		enableParentEntry->PushBack(enableParentFormEntry);
	}

	bool hasPopin = HasFlag(enableParentInformation->flags, flag_PopIn);
	ExtraInfoEntry* popinEntry;
	std::string popinString = BooleanToYesNoString(hasPopin);
	CreateExtraInfoEntry(popinEntry, "Popin", popinString);
	enableParentEntry->PushBack(popinEntry);

	bool enableStateOppositeParent = HasFlag(enableParentInformation->flags, flag_EnableStateOppositeParent);
	ExtraInfoEntry* enableStateOppositeParentEntry;
	std::string enableStateOppositeParentString = BooleanToYesNoString(enableStateOppositeParent);
	CreateExtraInfoEntry(enableStateOppositeParentEntry, "State Opposite Parent", enableStateOppositeParentString);
	enableParentEntry->PushBack(enableStateOppositeParentEntry);

	resultArray->PushBack(enableParentEntry);

	_DMESSAGE("Ending ProcessEnableParentInformation");
}