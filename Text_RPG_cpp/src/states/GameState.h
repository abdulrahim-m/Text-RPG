#pragma once

#include "IState.h"

class Keyboard;
class StateMachine;

class GameState : public IState
{
private:
	Keyboard& keyboard;
	StateMachine& stateMachine;
public:
	GameState(Keyboard& keyboard, StateMachine& stateMachine);
	~GameState();

	void OnEnter() override;
	void OnExit() override;
	void Update() override;
	void Draw() override;
	void ProcessInput() override;

	bool Exit() override;
};