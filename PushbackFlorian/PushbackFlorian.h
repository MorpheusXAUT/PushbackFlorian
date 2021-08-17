#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "EuroScope/EuroScopePlugIn.h"

#include "constants.h"
#include "helpers.h"


class PushbackFlorian : public EuroScopePlugIn::CPlugIn
{
public:
	PushbackFlorian();
	virtual ~PushbackFlorian();

	bool OnCompileCommand(const char* sCommandLine);
	void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize);
	void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area);
	void OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan FlightPlan, int DataType);

private:
	static std::map<std::string, COLORREF> ScratchPadLineMap;

	bool debug;
	bool shouldSetPushGroundState;
	std::map<std::string, std::string> trackedDirection;
	std::map<std::string, COLORREF> trackedLine;

	void SetDirection(EuroScopePlugIn::CFlightPlan& fp, std::string direction, bool broadcast = false);
	void SetLine(EuroScopePlugIn::CFlightPlan& fp, COLORREF line, bool broadcast = false);
	void SetPushGroundState(EuroScopePlugIn::CFlightPlan& fp);
	void ClearDirection(EuroScopePlugIn::CFlightPlan& fp, bool broadcast = false);
	void ClearLine(EuroScopePlugIn::CFlightPlan& fp, bool broadcast = false);
	void BroadcastScratchPad(EuroScopePlugIn::CFlightPlan& fp, std::string msg);

	void LoadSettings();
	void SaveSettings();

	void LogMessage(std::string message);
	void LogMessage(std::string message, std::string type);
	void LogDebugMessage(std::string message);
	void LogDebugMessage(std::string message, std::string type);
};