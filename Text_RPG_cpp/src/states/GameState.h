#pragma once

#include "IState.h"
#include "../Selector.h"

class Console;
class Keyboard;
class StateMachine;

class GameState : public IState
{
private:
	Console& console;
	Keyboard& keyboard;
	StateMachine& stateMachine;

	Selector<> selector;
public:
	GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine);
	~GameState();

	void OnEnter() override;
	void OnExit() override;
	void Update() override;
	void Draw() override;
	void ProcessInput() override;

	bool Exit() override;
};