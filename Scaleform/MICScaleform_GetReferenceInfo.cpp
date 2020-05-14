#include "MICScaleform_GetReferenceInfo.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"
#include "MoreInformativeConsole/Util/ScaleformUtil.h"

void MICScaleform_GetReferenceInfo::Call(Params& a_params)
{
	_DMESSAGE("GetReferenceInfo: Called");

	//Retrieve the various scaleform objects needed from the parameters
	RE::GFxValue* results = a_params.retVal;
	RE::GFxMovie* movie = a_params.movie;

	movie->CreateObject(results);

	std::string refFormFromMod, refFormLastChangedBy;
	int numModsModifyingRef;

	//Get the reference selected in the console
	RE::TESObjectREFR* pRef = RE::Console::GetSelectedRef().get();
	if (pRef != nullptr)
	{
		_DMESSAGE("GetReferenceInfo: PRef found");

		//Get the associated base form
		RE::TESBoundObject * pBaseForm = pRef->data.objectReference;

		if (pBaseForm != nullptr)
		{
			_DMESSAGE("GetReferenceInfo: pBaseForm found");

			//If we found both the base form and the reference form we can start retrieving the necessary information.

			//get the form ids
			RegisterString(results, movie, "refFormID", FormIDToString(pRef->formID));
			RegisterString(results, movie, "baseFormID", FormIDToString(pBaseForm->formID));
		}
	}
}


/*
void MICScaleform_GetReferenceInfo::Call(Params& a_params)
{
			const char* pRefName = CALL_MEMBER_FN(pRef, GetReferenceName)();
			RegisterString(args->result, args->movie, "referenceName", pRefName);
			FormModInfoData* refFormModInfo = (FormModInfoData*)pRef->unk08;

			if (refFormModInfo != nullptr)
			{
				DebugMessage("GetReferenceInfo: Inside Ref modInfo");
				numModsModifyingRef = refFormModInfo->size;

				refFormFromMod = refFormModInfo->entries[0]->name;

				//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
				if ( ( (pRef->formID & 0xFF000000 ) == 0)
					&& refFormFromMod != "Skyrim.esm")
				{
					refFormFromMod = "Skyrim.esm";
				}

				refFormLastChangedBy = refFormModInfo->entries[numModsModifyingRef - 1]->name;

				RegisterString(args->result, args->movie, "refFormDefinedIn", refFormFromMod.c_str());
				RegisterString(args->result, args->movie, "refFormLastChangedBy", refFormLastChangedBy.c_str());


			}

			DebugMessage("GetReferenceInfo: Before FF actor if");
			if (pBaseForm->formType == kFormType_NPC && pBaseForm->formID >= 0xFF000000)
			{
				DebugMessage("GetReferenceInfo: Inside FF actor if");
				TESNPC* pBaseActor = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);

				if (pBaseActor)
				{
					DebugMessage("GetReferenceInfo: Inside second FF actor if");
					TESNPC* pBaseActorTemplate = pBaseActor->GetRootTemplate();

					if (pBaseActorTemplate)
					{
						pBaseForm = DYNAMIC_CAST(pBaseActorTemplate, TESNPC, TESForm);
					}
				}
			}

			RegisterString(args->result, args->movie, "baseFormType", GetFormTypeName(pBaseForm->formType).c_str());



			DebugMessage("GetReferenceInfo: Before base modInfo");
			FormModInfoData* baseFormModInfo = (FormModInfoData*)pBaseForm->unk08;

			if (baseFormModInfo != nullptr)
			{

				DebugMessage("GetReferenceInfo: Inside Base modInfo");
				int numModsModifyingBase = baseFormModInfo->size;

				std::string baseFormFromMod = baseFormModInfo->entries[0]->name;
				std::string baseFormLastChangedBy = baseFormModInfo->entries[numModsModifyingBase - 1]->name;

				RegisterString(args->result, args->movie, "baseFormDefinedIn", baseFormFromMod.c_str());
				RegisterString(args->result, args->movie, "baseFormLastChangedBy", baseFormLastChangedBy.c_str());


				std::string modsModifying = "GetReferenceInfo: # Mods " + std::to_string(numModsModifyingBase) + " base # mods" + std::to_string(numModsModifyingRef) + " reference ";
				std::string baseModifiedBy = "GetReferenceInfo: Base From " + baseFormFromMod + " last changed by" + baseFormLastChangedBy;
				std::string refModifiedBy = "GetReferenceInfo: Ref From " + refFormFromMod + " last changed by" + refFormLastChangedBy;


				DebugMessage(modsModifying.c_str());
				DebugMessage(baseModifiedBy.c_str());
				DebugMessage(refModifiedBy.c_str());

			}
			DebugMessage("GetReferenceInfo: After FF actor if");

		}
	}
	*
	/*
	DebugMessage("GetReferenceInfo: Method End");
}*/