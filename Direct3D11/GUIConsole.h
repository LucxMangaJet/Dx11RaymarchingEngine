#pragma once
#include <vector>
#include "imgui.h"
#include <ctype.h>
#include "GUIBase.h"

//From ImGUI Example
class GUIConsole : public GUIBase
{
	char                  InputBuf[256];
	std::vector<char*>       Items;
	std::vector<const char*> Commands;
	std::vector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool                  AutoScroll;
	bool                  ScrollToBottom;
	

public:
	GUIConsole();
	~GUIConsole();

	void Draw(const AppInfo& info) override;
	void Draw(const char* title, bool* p_open);
	void AddLog(const char* fmt, ...) IM_FMTARGS(2);

private:
	void ClearLog();
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	void ExecCommand(const char* command_line);


	// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		GUIConsole* console = (GUIConsole*)data->UserData;
		return console->TextEditCallback(data);
	}

};
