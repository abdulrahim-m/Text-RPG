#include "GameState.h"
#include "../Logger.h"
#include "../inputs/Keyboard.h"
#include "StateMachine.h"

GameState::GameState(Keyboard& keyboard, StateMachine& stateMachine)
	: keyboard(keyboard)
	, stateMachine(stateMachine)
{
}

GameState::~GameState()
{
}

void GameState::OnEnter()
{
	TRPG_LOG("ENTER GAME STATE!");
}

void GameState::OnExit()
{
	TRPG_LOG("EXIT GAME STATE!");
}

void GameState::Update()
{
	TRPG_LOG("UPDATE GAME STATE!");
}

void GameState::Draw()
{
}

void GameState::ProcessInput()
{
	if (keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		stateMachine.PopState();
	}
}

bool GameState::Exit()
{
	return false;
}
