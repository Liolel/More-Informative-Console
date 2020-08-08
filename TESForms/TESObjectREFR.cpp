#include "TESObjectREFR.h"
#include "MoreInformativeConsole/Util/GeneralUtil.h"
#include "MoreInformativeConsole/BSExtraData/BSExtraData.h"

void GetReferenceFormData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("GetReferenceFormData Start");

	GetPositionData(resultArray, refForm);

	//Check if the refrence is Enabled
	boolean isDisabled = refForm->IsDisabled();

	ExtraInfoEntry* enabledStatusEntry;
	std::string enabledStatus = BooleanToYesNoString(!isDisabled);
	CreateExtraInfoEntry(enabledStatusEntry, "Is Enabled", enabledStatus, priority_Reference_Enabled);

	resultArray->PushBack(enabledStatusEntry);

	GetBSExtraData(resultArray, refForm);
}


void GetPositionData(ExtraInfoEntry* resultArray, RE::TESObjectREFR* refForm)
{
	_DMESSAGE("Starting GetPositionData");

	ExtraInfoEntry* positionEntry;
	CreateExtraInfoEntry(positionEntry, "Position", "", priority_Reference_Location);

	//position
	float xPos = refForm->data.location.x;
	ExtraInfoEntry* xPositionEntry;
	CreateExtraInfoEntry(xPositionEntry, "X Position", FloatToString(xPos), priority_Position_XCoordinate);
	positionEntry->PushBack(xPositionEntry);

	float yPos = refForm->data.location.y;
	ExtraInfoEntry* yPositionEntry;
	CreateExtraInfoEntry(yPositionEntry, "Y Position", FloatToString(yPos), priority_Position_YCoordinate);
	positionEntry->PushBack(yPositionEntry);

	float zPos = refForm->data.location.z;
	ExtraInfoEntry* zPositionEntry;
	CreateExtraInfoEntry(zPositionEntry, "Z Position", FloatToString(zPos), priority_Position_ZCoordinate);
	positionEntry->PushBack(zPositionEntry);

	//rotation
	float xRot = refForm->data.angle.x;
	ExtraInfoEntry* xRotationEntry;
	CreateExtraInfoEntry(xRotationEntry, "X Rotation", FloatToString(xRot), priority_Position_XRotation);
	positionEntry->PushBack(xRotationEntry);

	float yRot = refForm->data.angle.y;
	ExtraInfoEntry* yRotationEntry;
	CreateExtraInfoEntry(yRotationEntry, "Y Rotation", FloatToString(yRot), priority_Position_YRotation);
	positionEntry->PushBack(yRotationEntry);

	float zRot = refForm->data.angle.z;
	ExtraInfoEntry* zRotationEntry;
	CreateExtraInfoEntry(zRotationEntry, "Z Rotation", FloatToString(zRot), priority_Position_ZRotation);
	positionEntry->PushBack(zRotationEntry);

	resultArray->PushBack(positionEntry);

	_DMESSAGE("Ending GetPositionData");
}