#include "TESObjectREFR.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"

void GetReferenceFormData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("GetReferenceFormData Start");

	GetPositionData(resultArray, refForm);

	//Check if the refrence is Enabled
	boolean isDisabled = refForm->IsDisabled();

	ExtraInfoEntry* enabledStatusEntry;
	std::string enabledStatus = !isDisabled ? "Yes" : "No";
	CreateExtraInfoEntry(enabledStatusEntry, "Is Enabled", enabledStatus, priority_Reference_Enabled);

}


void GetPositionData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("Starting GetPositionData");

	ExtraInfoEntry* positionEntry;
	CreateExtraInfoEntry(positionEntry, "Position", "", priority_Reference_Location);

	//position
	float xPos = refForm->data.location.x;
	ExtraInfoEntry* xPositionEntry;
	CreateExtraInfoEntry(xPositionEntry, "X Position", FloatToString(xPos));
	positionEntry->PushBack(xPositionEntry);

	float yPos = refForm->data.location.y;
	ExtraInfoEntry* yPositionEntry;
	CreateExtraInfoEntry(yPositionEntry, "Y Position", FloatToString(yPos));
	positionEntry->PushBack(yPositionEntry);

	float zPos = refForm->data.location.z;
	ExtraInfoEntry* zPositionEntry;
	CreateExtraInfoEntry(zPositionEntry, "Z Position", FloatToString(zPos));
	positionEntry->PushBack(zPositionEntry);

	//rotation
	float xRot = refForm->data.angle.x;
	ExtraInfoEntry* xRotationEntry;
	CreateExtraInfoEntry(xRotationEntry, "X Rotation", FloatToString(xRot));
	positionEntry->PushBack(xRotationEntry);

	float yRot = refForm->data.angle.y;
	ExtraInfoEntry* yRotationEntry;
	CreateExtraInfoEntry(yRotationEntry, "Y Rotation", FloatToString(yRot));
	positionEntry->PushBack(yRotationEntry);

	float zRot = refForm->data.angle.z;
	ExtraInfoEntry* zRotationEntry;
	CreateExtraInfoEntry(zRotationEntry, "Z Rotation", FloatToString(zRot));
	positionEntry->PushBack(zRotationEntry);

	resultArray->PushBack(positionEntry);

	_DMESSAGE("Ending GetPositionData");
}