#pragma once

struct Button
{
	bool IsDown;
	bool IsJustPressed;
	bool IsJustReleased;

	Button()
		: IsDown{ false }, IsJustPressed{ false }, IsJustReleased{ false }
	{

	}

	void Update(bool pressed)
	{
		IsJustPressed = !IsDown && pressed;
		IsJustReleased = IsDown && !pressed;
		IsDown = pressed;
	}
};