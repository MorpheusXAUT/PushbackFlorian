#include "pch.h"
#include "PushbackFlorian.h"

PushbackFlorian* pPlugin;

PushbackFlorian::PushbackFlorian() : EuroScopePlugIn::CPlugIn(
	EuroScopePlugIn::COMPATIBILITY_CODE,
	PLUGIN_NAME,
	PLUGIN_VERSION,
	PLUGIN_AUTHOR,
	PLUGIN_LICENSE
)
{
	std::ostringstream msg;
	msg << "Version " << PLUGIN_VERSION << " loaded.";

	this->LogMessage(msg.str());

	this->RegisterTagItemType("Direction", TAG_ITEM_FP_DIRECTION);
	this->RegisterTagItemType("Line", TAG_ITEM_FP_LINE);

	this->RegisterTagItemFunction("Direction Menu", TAG_FUNC_DIRECTION_MENU);
	this->RegisterTagItemFunction("Direction north", TAG_FUNC_DIRECTION_NORTH);
	this->RegisterTagItemFunction("Direction east", TAG_FUNC_DIRECTION_EAST);
	this->RegisterTagItemFunction("Direction south", TAG_FUNC_DIRECTION_SOUTH);
	this->RegisterTagItemFunction("Direction west", TAG_FUNC_DIRECTION_WEST);
	this->RegisterTagItemFunction("Direction Clear", TAG_FUNC_DIRECTION_CLEAR);
	this->RegisterTagItemFunction("Line Menu", TAG_FUNC_LINE_MENU);
	this->RegisterTagItemFunction("Line blue", TAG_FUNC_LINE_BLUE);
	this->RegisterTagItemFunction("Line orange", TAG_FUNC_LINE_ORANGE);
	this->RegisterTagItemFunction("Line Clear", TAG_FUNC_LINE_CLEAR);

	this->debug = false;
	this->shouldSetPushGroundState = true;

	this->LoadSettings();
}

PushbackFlorian::~PushbackFlorian()
{
}

std::map<std::string, COLORREF> PushbackFlorian::ScratchPadLineMap = {
	{"B", TAG_COLOR_LINE_BLUE},
	{"O", TAG_COLOR_LINE_ORANGE}
};

bool PushbackFlorian::OnCompileCommand(const char* sCommandLine)
{
	std::vector<std::string> args = split(sCommandLine);

	if (args[0] == ".pbf") {
		if (args.size() == 1) {
			std::ostringstream msg;
			msg << "Version " << PLUGIN_VERSION << " loaded. Available commands: debug, push, reset";

			this->LogMessage(msg.str());

			return true;
		}

		if (args[1] == "debug") {
			if (this->debug) {
				this->LogMessage("Disabling debug mode", "Config");
			}
			else {
				this->LogMessage("Enabling debug mode", "Config");
			}

			this->debug = !this->debug;

			this->SaveSettings();

			return true;
		}
		else if (args[1] == "push") {
			if (this->shouldSetPushGroundState) {
				this->LogMessage("No longer setting push ground state on direction or line assignment", "Config");
			}
			else {
				this->LogMessage("Setting push ground state on direction or line assignment", "Config");
			}

			this->shouldSetPushGroundState = !this->shouldSetPushGroundState;

			this->SaveSettings();

			return true;
		}
		else if (args[1] == "reset") {
			this->LogMessage("Resetting plugin state", "Config");
			
			this->trackedDirection.clear();
			this->trackedLine.clear();

			return true;
		}
	}

	return false;
}

void PushbackFlorian::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize)
{
	if (!FlightPlan.IsValid()) {
		return;
	}

	switch (ItemCode) {
	case TAG_ITEM_FP_DIRECTION: {
		auto it = this->trackedDirection.find(FlightPlan.GetCallsign());
		if (it != this->trackedDirection.end()) {
			strcpy_s(sItemString, 16, it->second.c_str());
		}

		break;
	}
	case TAG_ITEM_FP_LINE: {
		auto it = this->trackedLine.find(FlightPlan.GetCallsign());
		if (it != this->trackedLine.end()) {
			strcpy_s(sItemString, 16, "===");
			*pColorCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;
			*pRGB = it->second;
		}

		break;
	}
	}
}

void PushbackFlorian::OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area)
{
	EuroScopePlugIn::CFlightPlan fp = this->FlightPlanSelectASEL();
	if (!fp.IsValid()) {
		return;
	}

	switch (FunctionId) {
	case TAG_FUNC_DIRECTION_MENU:
		this->OpenPopupList(Area, "Direction", 1);
		this->AddPopupListElement("North", NULL, TAG_FUNC_DIRECTION_NORTH, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("East", NULL, TAG_FUNC_DIRECTION_EAST, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("South", NULL, TAG_FUNC_DIRECTION_SOUTH, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("West", NULL, TAG_FUNC_DIRECTION_WEST, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("    ", NULL, TAG_FUNC_DIRECTION_CLEAR, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, true);
		break;
	case TAG_FUNC_DIRECTION_NORTH:
		this->SetDirection(fp, "N", true);
		break;
	case TAG_FUNC_DIRECTION_EAST:
		this->SetDirection(fp, "E", true);
		break;
	case TAG_FUNC_DIRECTION_SOUTH:
		this->SetDirection(fp, "S", true);
		break;
	case TAG_FUNC_DIRECTION_WEST:
		this->SetDirection(fp, "W", true);
		break;
	case TAG_FUNC_DIRECTION_CLEAR:
		this->ClearDirection(fp, true);
		break;
	case TAG_FUNC_LINE_MENU:
		this->OpenPopupList(Area, "Line", 1);
		this->AddPopupListElement("Blue", NULL, TAG_FUNC_LINE_BLUE, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("Orange", NULL, TAG_FUNC_LINE_ORANGE, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, false);
		this->AddPopupListElement("    ", NULL, TAG_FUNC_LINE_CLEAR, false, EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, false, true);
		break;
	case TAG_FUNC_LINE_BLUE:
		this->SetLine(fp, TAG_COLOR_LINE_BLUE, true);
		break;
	case TAG_FUNC_LINE_ORANGE:
		this->SetLine(fp, TAG_COLOR_LINE_ORANGE, true);
		break;
	case TAG_FUNC_LINE_CLEAR:
		this->ClearLine(fp, true);
	}
}

void PushbackFlorian::OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan FlightPlan, int DataType)
{
	if (!FlightPlan.IsValid()) {
		return;
	}

	switch (DataType) {
	case EuroScopePlugIn::CTR_DATA_TYPE_SCRATCH_PAD_STRING:
	{
		auto cad = FlightPlan.GetControllerAssignedData();
		std::vector<std::string> scratch = split(cad.GetScratchPadString(), SCRATCH_PAD_DELIMITER);

		if (scratch.size() >= 3 && scratch[0] == SCRATCH_PAD_PREFIX) {
			if (scratch[1] == SCRATCH_PAD_DIRECTION) {
				if (scratch[2] == SCRATCH_PAD_CLEAR) {
					this->ClearDirection(FlightPlan);
				}
				else {
					this->SetDirection(FlightPlan, scratch[2]);
				}
			}
			else if (scratch[1] == SCRATCH_PAD_LINE) {
				if (scratch[2] == SCRATCH_PAD_CLEAR) {
					this->ClearLine(FlightPlan);
				}
				else {
					auto it = this->ScratchPadLineMap.find(scratch[2]);
					if (it != this->ScratchPadLineMap.end()) {
						this->SetLine(FlightPlan, it->second);
					}
				}
			}
		}
		break;
	}
	case EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE:
		if (strcmp(FlightPlan.GetGroundState(), "TAXI") == 0) {
			this->ClearDirection(FlightPlan);
			this->ClearLine(FlightPlan);
		}
	}
}

void PushbackFlorian::SetDirection(EuroScopePlugIn::CFlightPlan& fp, std::string direction, bool broadcast)
{
	this->trackedDirection[fp.GetCallsign()] = direction;

	if (broadcast) {
		this->SetPushGroundState(fp);

		std::ostringstream msg;
		msg << SCRATCH_PAD_PREFIX << SCRATCH_PAD_DELIMITER
			<< SCRATCH_PAD_DIRECTION << SCRATCH_PAD_DELIMITER
			<< direction;

		this->BroadcastScratchPad(fp, msg.str());
	}
}

void PushbackFlorian::SetLine(EuroScopePlugIn::CFlightPlan& fp, COLORREF line, bool broadcast)
{
	this->trackedLine[fp.GetCallsign()] = line;

	if (broadcast) {
		this->SetPushGroundState(fp);

		for (auto it = this->ScratchPadLineMap.begin(); it != this->ScratchPadLineMap.end(); ++it) {
			if (it->second == line) {
				std::ostringstream msg;
				msg << SCRATCH_PAD_PREFIX << SCRATCH_PAD_DELIMITER
					<< SCRATCH_PAD_LINE << SCRATCH_PAD_DELIMITER
					<< it->first;

				this->BroadcastScratchPad(fp, msg.str());

				break;
			}
		}
	}
}

void PushbackFlorian::SetPushGroundState(EuroScopePlugIn::CFlightPlan& fp)
{
	if (!this->shouldSetPushGroundState) {
		return;
	}

	if (!fp.IsValid()) {
		return;
	}

	if (!fp.GetTrackingControllerIsMe() && strcmp(fp.GetTrackingControllerId(), "") != 0) {
		return;
	}

	std::string gs = fp.GetGroundState();

	if (gs == "PUSH") {
		return;
	}
	else if (gs != "" && gs != "ST-UP") {
		std::ostringstream msg;
		msg << "Aircraft already has ground state \"" << gs << "\", not overwriting with push state";
		this->LogDebugMessage(msg.str(), fp.GetCallsign());

		return;
	}

	this->BroadcastScratchPad(fp, "PUSH");
}

void PushbackFlorian::ClearDirection(EuroScopePlugIn::CFlightPlan& fp, bool broadcast)
{
	this->trackedDirection.erase(fp.GetCallsign());

	if (broadcast) {
		std::ostringstream msg;
		msg << SCRATCH_PAD_PREFIX << SCRATCH_PAD_DELIMITER
			<< SCRATCH_PAD_DIRECTION << SCRATCH_PAD_DELIMITER
			<< SCRATCH_PAD_CLEAR;

		this->BroadcastScratchPad(fp, msg.str());
	}
}

void PushbackFlorian::ClearLine(EuroScopePlugIn::CFlightPlan& fp, bool broadcast)
{
	this->trackedLine.erase(fp.GetCallsign());

	if (broadcast) {
		std::ostringstream msg;
		msg << SCRATCH_PAD_PREFIX << SCRATCH_PAD_DELIMITER
			<< SCRATCH_PAD_LINE << SCRATCH_PAD_DELIMITER
			<< SCRATCH_PAD_CLEAR;

		this->BroadcastScratchPad(fp, msg.str());
	}
}

void PushbackFlorian::BroadcastScratchPad(EuroScopePlugIn::CFlightPlan& fp, std::string msg)
{
	if (!fp.IsValid()) {
		return;
	}

	if (!fp.GetTrackingControllerIsMe() && strcmp(fp.GetTrackingControllerId(), "") != 0) {
		return;
	}

	auto cad = fp.GetControllerAssignedData();
	std::string scratch = cad.GetScratchPadString();

	if (!cad.SetScratchPadString(msg.c_str())) {
		this->LogMessage("Failed to set broadcast message in scratch pad", fp.GetCallsign());
	}

	if (!cad.SetScratchPadString(scratch.c_str())) {
		this->LogMessage("Failed to reset scratch pad after setting broadcast message", fp.GetCallsign());
	}
}

void PushbackFlorian::LoadSettings()
{
	const char* settings = this->GetDataFromSettings(PLUGIN_NAME);
	if (settings) {
		std::vector<std::string> splitSettings = split(settings, SETTINGS_DELIMITER);

		if (splitSettings.size() < 2) {
			this->LogMessage("Invalid saved settings found, reverting to default.");

			this->SaveSettings();

			return;
		}

		std::istringstream(splitSettings[0]) >> this->debug;
		std::istringstream(splitSettings[1]) >> this->shouldSetPushGroundState;

		this->LogDebugMessage("Successfully loaded settings.");
	}
	else {
		this->LogMessage("No saved settings found, using defaults.");
	}
}

void PushbackFlorian::SaveSettings()
{
	std::ostringstream ss;
	ss << this->debug << SETTINGS_DELIMITER
		<< this->shouldSetPushGroundState;


	this->SaveDataToSettings(PLUGIN_NAME, "Settings", ss.str().c_str());
}

void PushbackFlorian::LogMessage(std::string message)
{
	this->DisplayUserMessage("Message", PLUGIN_NAME, message.c_str(), true, true, true, false, false);
}

void PushbackFlorian::LogMessage(std::string message, std::string type)
{
	this->DisplayUserMessage(PLUGIN_NAME, type.c_str(), message.c_str(), true, true, true, false, false);
}

void PushbackFlorian::LogDebugMessage(std::string message)
{
	if (this->debug) {
		this->LogMessage(message);
	}
}

void PushbackFlorian::LogDebugMessage(std::string message, std::string type)
{
	if (this->debug) {
		this->LogMessage(message, type);
	}
}

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
	*ppPlugInInstance = pPlugin = new PushbackFlorian();
}

void __declspec (dllexport) EuroScopePlugInExit(void)
{
	delete pPlugin;
}