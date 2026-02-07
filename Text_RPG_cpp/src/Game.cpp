#include "game.h"
#include <iostream>
#include "logger.h"
#include "states/GameState.h"

bool Game::Init()
{
	try
	{
		console = std::make_unique<Console>();
	}
	catch(std::exception& e)
	{
		std::string error = e.what();
		TRPG_ERROR(error);
		return false;
	}

	ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	keyboard = std::make_unique<Keyboard>();
	stateMachine = std::make_unique<StateMachine>();
	
	stateMachine->PushState(std::make_unique<GameState>(*console, *keyboard, *stateMachine));

	return true;
}

void Game::ProcessEvents()
{
	// Get the number of console input
	if (!GetNumberOfConsoleInputEvents(ConsoleIn, &NumRead))
	{
		DWORD error = GetLastError();
		TRPG_ERROR("Failed to get number on console input events!" + error)
			return;
	}

	if (NumRead <= 0)
		return;

	if (!PeekConsoleInput(ConsoleIn, InRecBuf, 128, &NumRead))
	{
		DWORD error = GetLastError();
		TRPG_ERROR("Failed to peek events!" + error)
			return;
	}

	for (int i = 0; i < NumRead; i++)
	{
		switch (InRecBuf[i].EventType)
		{
		case KEY_EVENT:
			KeyEventProcess(InRecBuf[i].Event.KeyEvent);
			break;
		default:
			break;
		}
	}

	// Flush other input events
	FlushConsoleInputBuffer(ConsoleIn);
}

void Game::ProcessInput()
{
	if (keyboard->IsKeyJustPressed(KEY_ESCAPE))
		IsRunning = false;

	if (stateMachine->Empty())
	{
		TRPG_ERROR("No states in state machine to process inputs!");
		IsRunning = false;
		return;
	}

	stateMachine->GetCurrentState()->ProcessInput();
}

void Game::Update()
{
	if (stateMachine->Empty())
	{
		TRPG_ERROR("No states in state machine to update!");
		IsRunning = false;
		return;
	}

	stateMachine->GetCurrentState()->Update();
	keyboard->Update();
}

void Game::Draw()
{
	if (stateMachine->Empty())
	{
		TRPG_ERROR("No states in state machine to draw!");
		IsRunning = false;
		return;
	}

	stateMachine->GetCurrentState()->Draw();
	console->Draw();
}

void Game::KeyEventProcess(KEY_EVENT_RECORD keyEvent)
{
	if (keyEvent.bKeyDown)
		keyboard->OnKeyDown(keyEvent.wVirtualKeyCode);
	else
		keyboard->OnKeyUp(keyEvent.wVirtualKeyCode);
}

Game::Game()
	: IsRunning{ true }
	, console{ nullptr }
	, keyboard{ nullptr }
	, stateMachine{ nullptr }
{
	
}

Game::~Game()
{
}

void Game::Run()
{
	if (!Init())
		IsRunning = false;

	while (IsRunning)
	{
		ProcessEvents();
		ProcessInput(); 
		Update();
		Draw();
	}
}
