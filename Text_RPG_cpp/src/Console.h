#pragma once

#include "utilities/Colors.h"
#include <windows.h>
#include <memory>
#include <string>

class Console
{
private:
	const int SCREEN_WIDTH = 128;
	const int SCREEN_HEIGHT = 48;

	const int BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

	HANDLE WinConsole;
	HWND ConsoleWindow;
	RECT ConsoleWindowRect;

	DWORD BytesWritten;
	std::unique_ptr<wchar_t[]> Screen;

	bool SetTextColor(int size, int x, int y, HANDLE handle, WORD color);

public:
	Console();
	~Console();

	void ClearBuffer();
	void Write(int x, int y, const std::wstring& text, WORD color = WHITE);
	void Draw();
	bool ShowConsoleCursor(bool show);


};