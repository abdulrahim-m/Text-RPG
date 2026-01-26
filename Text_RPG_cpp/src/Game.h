#pragma once

#include "Console.h"
#include "inputs/Keyboard.h"
#include "states/StateMachine.h"

class Game
{
private:
	bool IsRunning;

	std::unique_ptr<Console> console;
	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<StateMachine> stateMachine;

	DWORD NumRead;
	INPUT_RECORD InRecBuf[128];
	HANDLE ConsoleIn;

	bool Init();

	void ProcessEvents();
	void ProcessInput();
	void Update();
	void Draw();

	void KeyEventProcess(KEY_EVENT_RECORD keyEvent);
public:
	Game();
	~Game();

	void Run();
};