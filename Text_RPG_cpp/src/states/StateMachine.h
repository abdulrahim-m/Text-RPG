#pragma once

#include <memory>
#include <stack>
#include "IState.h"

typedef std::unique_ptr<IState> StatePtr;

class StateMachine
{
private:
	std::stack<StatePtr> States;

public:
	StateMachine();
	~StateMachine();

	void PushState(StatePtr newState);
	StatePtr PopState();
	const bool Empty() const { return States.empty(); }
	StatePtr& GetCurrentState();

};