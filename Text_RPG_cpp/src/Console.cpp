#include "Console.h"
#include "Logger.h"
#include <string>
#include <vector>

bool Console::SetTextColor(int size, int x, int y, HANDLE handle, WORD color)
{
	COORD pos = { x , y };

	std::vector<WORD> write(size, color);

	DWORD written = 0;
	LPDWORD lpNumWritten = &written;

	if (!WriteConsoleOutputAttribute(handle, &write[0], size, pos, lpNumWritten))
	{
		TRPG_ERROR("Unable to change color!");
		return false;
	}

	return true;
}

Console::Console()
	:Screen{nullptr}
{
	// Initialize Screen Buffer
	Screen = std::make_unique<wchar_t[]>(BUFFER_SIZE);

	// Get a handle to the console window
	ConsoleWindow = GetConsoleWindow();

	if (!GetWindowRect(ConsoleWindow, &ConsoleWindowRect))
		throw("Failed to get the window Rect when creating the console!");

	// Get the font size
	CONSOLE_FONT_INFO font_info;
	if (!GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info))
		throw("Failed to get the console font!");

	COORD font_size = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), font_info.nFont);

	int actual_screen_x = SCREEN_WIDTH * font_size.X + 5 * font_size.X;
	int actual_screen_y = SCREEN_HEIGHT * font_size.Y + 8 * font_size.Y;

	int pos_x = GetSystemMetrics(SM_CXSCREEN) / 2 - (actual_screen_x / 2);
	int pos_y = GetSystemMetrics(SM_CYSCREEN) / 2 - (actual_screen_y / 2);

	 // Set the size and position of the window
	if (!MoveWindow(ConsoleWindow, pos_x, pos_y, actual_screen_x, actual_screen_y, TRUE))
		throw("Failed to set and move the console window!");

	// Clear the screen buffer
	ClearBuffer();

	// Create the screen buffer
	WinConsole = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	if (!WinConsole)
		throw("Failed to create the console screen buffer");

	// Set the buffer to be active
	if (!SetConsoleActiveScreenBuffer(WinConsole))
		throw("Failed to set the active screen buffer!");

	// Hide the cursor 
	if (!ShowConsoleCursor(false))
		throw("Failed to hide the console cursor!");
}

Console::~Console()
{
}

void Console::ClearBuffer()
{
	// Set all the values of the buffer to empty space
	for (int i = 0; i < BUFFER_SIZE; i++)
		Screen[i] = L' ';
}

void Console::Write(int x, int y, const std::wstring& text, WORD color)
{
	SetTextColor(text.size(), x, y, WinConsole, color);
	int pos = y * SCREEN_WIDTH + x;
	swprintf(&Screen[pos], BUFFER_SIZE, text.c_str());
}

void Console::Draw()
{
	// Handles all console drawing
	WriteConsoleOutputCharacter(WinConsole, Screen.get(), BUFFER_SIZE, { 0, 0 }, &BytesWritten);
}

bool Console::ShowConsoleCursor(bool show)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(WinConsole, &cursorInfo))
	{
		TRPG_ERROR("Failed to get cursor info!");
		return false;
	}
	cursorInfo.bVisible = show;
	return SetConsoleCursorInfo(WinConsole, &cursorInfo);
}
