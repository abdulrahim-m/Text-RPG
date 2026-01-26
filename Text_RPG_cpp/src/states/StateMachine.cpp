#include "StateMachine.h"

StateMachine::StateMachine()
	:States()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::PushState(StatePtr newState)
{
	States.push(std::move(newState));
	States.top()->OnEnter();
}

StatePtr StateMachine::PopState()
{
	if (States.empty())
		return nullptr;

	auto oldState = std::move(States.top());

	States.pop();

	oldState->OnExit();

	return oldState;
}

StatePtr& StateMachine::GetCurrentState()
{
	return States.top();
}
