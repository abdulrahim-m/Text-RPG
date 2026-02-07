#include "GameState.h"
#include "../Logger.h"
#include "../Console.h"
#include "../inputs/Keyboard.h"
#include "StateMachine.h"

GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine)
	: console(console)
	, keyboard(keyboard)
	, stateMachine(stateMachine)
	, selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
{
}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	console.ClearBuffer();
}

void GameState::OnExit()
{
	console.ClearBuffer();
}

void GameState::Update()
{
	
}

void GameState::Draw()
{
	selector.Draw();
	console.Draw();
}

void GameState::ProcessInput()
{
	if (keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		stateMachine.PopState();
	}

	selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
