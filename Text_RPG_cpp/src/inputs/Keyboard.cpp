#include "keyboard.h"
#include "../Logger.h"

Keyboard::Keyboard()
{
	// Nothing here
}

void Keyboard::Update()
{
	for (int i = 0; i < KEY_LAST; i++)
	{
		Keys[i].IsJustPressed = false;
		Keys[i].IsJustReleased = false;
	}
}

void Keyboard::OnKeyDown(int key)
{
	// Check to see if key is defined
	if (key > KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	Keys[key].Update(true);
}

void Keyboard::OnKeyUp(int key)
{
	// Check to see if key is defined
	if (key > KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	Keys[key].Update(false);
}

bool Keyboard::IsKeyHeld(int key) const
{
	// Check to see if key is defined
	if (key > KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return Keys[key].IsDown;
}

bool Keyboard::IsKeyJustPressed(int key) const
{
	// Check to see if key is defined
	if (key > KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return Keys[key].IsJustPressed;
}

bool Keyboard::IsKeyJustReleased(int key) const
{
	// Check to see if key is defined
	if (key > KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return Keys[key].IsJustReleased;
}
