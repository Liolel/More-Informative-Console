#include "MIC_Scaleform.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include "Scaleform/MICScaleform_GetExtraData.h"
#include "Scaleform/MICScaleform_GetIniOptions.h"
#include "Scaleform/MICScaleform_GetReferenceInfo.h"
#include "Scaleform/MICScaleform_Log.h"
#include "Scaleform/MICScaleform_RetrieveExtraData.h"
#include "Scaleform/MICScaleform_GetQuestData.h"

MICScaleform_GetReferenceInfo* getReferenceInfo = nullptr;
MICScaleform_GetIniOptions* getIniOptions = nullptr;
MICScaleform_RetrieveExtraData* retrieveExtraData = nullptr;
MICScaleform_GetExtraData* getExtraData = nullptr;
MICScaleform_Log* MICScaleformlog = nullptr;
MICScaleform_GetQuestData* getQuestData = nullptr;

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks(RE::GFxMovieView* a_view, RE::GFxValue*)
{
	logger::debug("Install Hooks Called");

	if (getReferenceInfo == nullptr) {
		getReferenceInfo = new MICScaleform_GetReferenceInfo;
	}

	if (getIniOptions == nullptr) {
		getIniOptions = new MICScaleform_GetIniOptions;
	}

	if (retrieveExtraData == nullptr) {
		retrieveExtraData = new MICScaleform_RetrieveExtraData;
	}

	if (getExtraData == nullptr) {
		getExtraData = new MICScaleform_GetExtraData;
	}

	if (MICScaleformlog == nullptr) {
		MICScaleformlog = new MICScaleform_Log;
	}
	
	if (getQuestData == nullptr) {
		getQuestData = new MICScaleform_GetQuestData;
	}

	RE::GFxValue globals;

	logger::debug(a_view->GetMovieDef()->GetFileURL());
	bool result = a_view->GetVariable(&globals, "_global");
	if (result) {

		//Create Object to hold new functions
		RE::GFxValue MIC;
		a_view->CreateObject(&MIC);

		RE::GFxValue fnValue;
		a_view->CreateFunction(&fnValue, getReferenceInfo);
		MIC.SetMember("MICScaleform_GetReferenceInfo", fnValue);

		RE::GFxValue fnValue2;
		a_view->CreateFunction(&fnValue2, getIniOptions);
		MIC.SetMember("MICScaleform_GetIniOptions", fnValue2);

		RE::GFxValue fnValue3;
		a_view->CreateFunction(&fnValue3, retrieveExtraData);
		MIC.SetMember("MICScaleform_RetrieveExtraData", fnValue3);

		RE::GFxValue fnValue4;
		a_view->CreateFunction(&fnValue4, getExtraData);
		MIC.SetMember("MICScaleform_GetExtraData", fnValue4);

		RE::GFxValue fnValue5;
		a_view->CreateFunction(&fnValue5, MICScaleformlog);
		MIC.SetMember("MICScaleform_Log", fnValue5);

		RE::GFxValue fnValue6;
		a_view->CreateFunction(&fnValue6, getQuestData);
		MIC.SetMember("MICScaleform_GetQuestData", fnValue6);

		//Store object with functions
		globals.SetMember("MIC", MIC);
	}

	return true;
}
