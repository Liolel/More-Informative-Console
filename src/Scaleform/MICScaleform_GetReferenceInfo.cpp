#include "MICScaleform_GetReferenceInfo.h"
#include "TESForms/TESForm.h"
#include "TESForms/TESNPC.h"
#include "Util/GeneralUtil.h"
#include "Util/NameUtil.h"
#include "Util/ScaleformUtil.h"
#include <RE/T/TESForm.h>

void MICScaleform_GetReferenceInfo::Call(Params& a_params)
{
	logger::debug("GetReferenceInfo: Called");

	//Retrieve the various scaleform objects needed from the parameters
	RE::GFxValue* results = a_params.retVal;
	RE::GFxMovie* movie = a_params.movie;

	movie->CreateObject(results);

	//Get the reference selected in the console
	RE::TESObjectREFR* ref = RE::Console::GetSelectedRef().get();
	if (ref != nullptr) {
		logger::debug("GetReferenceInfo: ref found");

		//Get the associated base form
		RE::TESBoundObject* baseForm = ref->data.objectReference;

		if (baseForm != nullptr) {
			logger::debug("GetReferenceInfo: baseForm found");

			//If we found both the base form and the reference form we can start retrieving the necessary information.

			// if the base form is a npc with mod index FF try and get the root template for the npc as that contains more useful information
			// TODO: Do I want to actually get the root template or do I want to just go up to the first non FF level?
			if (baseForm->formType == RE::FormType::NPC && baseForm->formID >= 0xFF000000) {
				baseForm = GetRootTemplate(baseForm);
			}

			//get the form ids
			RegisterString(results, movie, "refFormID", FormIDToString(ref->formID));
			RegisterString(results, movie, "baseFormID", FormIDToString(baseForm->formID));

			//Get the reference name
			const char* referenceName = ref->GetName();
			RegisterString(results, movie, "referenceName", referenceName);

			//Get the location info  the reference was defined in
			logger::debug("GetReferenceInfo: Getting refernce form location");
			std::string refDefinedIn = GetFirstFormLocationName(ref);
			std::string refFormLastChangedBy = GetLastFormLocationName(ref);

			//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
			if (((ref->formID & 0xFF000000) == 0)  //Refs from Skyrim.ESM will have 00 for the first two hexidecimal digits
				&& refDefinedIn != "Skyrim.esm")   //And refs from all other mods will have a non zero value, so a bitwise && of those two digits with FF will be nonzero for all non Skyrim.ESM mods
			{
				refDefinedIn = "Skyrim.esm";
			}

			RegisterString(results, movie, "refFormDefinedIn", refDefinedIn);
			RegisterString(results, movie, "refFormLastChangedBy", refFormLastChangedBy);

			//Get base form information

			std::string baseDefinedIn = GetFirstFormLocationName(baseForm);
			std::string baseFormLastChangedBy = GetLastFormLocationName(baseForm);

			RegisterString(results, movie, "baseFormDefinedIn", baseDefinedIn);
			RegisterString(results, movie, "baseFormLastChangedBy", baseFormLastChangedBy);

			//Get the form type
			std::string baseFormTypeName = GetFormTypeName(baseForm->formType.underlying());
			RegisterString(results, movie, "baseFormType", baseFormTypeName);
		}
	}

	logger::debug("GetReferenceInfo: Finished");
}
