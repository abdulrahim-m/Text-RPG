#pragma once

#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <type_traits>
#include "Console.h"
#include "Logger.h"
#include "inputs/Keyboard.h"

struct SelectorParams
{
	int x, y, columns, currentX, currentY, spacingX, spacingY;
	std::wstring cursor;

	SelectorParams(
		int x = 5, int y = 10, int columns = 1, int current_x = 0, int current_y = 0, 
		int spacing_x = 10, int spaing_y = 2, std::wstring curser = L"->"
	)
	{
		this->x = x;
		this->y = y;
		this->columns = columns;
		this->currentX = current_x;
		this->currentY = current_y;
		this->spacingX = spacing_x;
		this->spacingY = spaing_y;
		this->cursor = curser;
	}
};

template <typename T = std::wstring> class Selector
{
private:
	Console& console;
	Keyboard& keyboard;
	std::function<void(int, std::vector<T>)> onSelection;
	std::function<void(int, int, T)> onDrawItem;
	std::vector<T> data;
	SelectorParams params;
	bool showCursor;
	int rows;

public:
	Selector(Console& console, Keyboard& keyboard, std::vector<T> data, SelectorParams params = SelectorParams());
	Selector(Console& console, Keyboard& keyboard, std::function<void(int, std::vector<T>)> on_selected,
		std::function<void(int, int, T)> on_draw_item, std::vector<T> data, SelectorParams params = SelectorParams());
	~Selector();

	void SetData(std::vector<T> data) { this->data = data; }
	const std::vector<T>& GetData() { return data; }
	void SetSelectionFont(std::function<void(int, std::vector<T>)> on_selection) { onSelection = on_selection; }
	void SetDrawFunc(std::function<void(int, int, T)> on_draw_item) { onDrawItem = on_draw_item; }
	void ShowCursor() { showCursor = true; }
	void HideCursor() { showCursor = false; }
	const int GetIndex() const { return params.currentX + (params.currentY * params.columns); }

	void ProcessInputs();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void OnAction();

	void OnDrawItem(int x, int y, T item);
	void OnSelection(int index, std::vector<T> data);

	void Draw();
};

template<typename T>
inline Selector<T>::Selector(Console& console, Keyboard& keyboard, std::vector<T> data, SelectorParams params)
	: Selector(
		console, 
		keyboard, 
		[this](int index, std::vector<T> data) { Selector::OnSelection(index, data); },
		[this](int x, int y, T item) { Selector::OnDrawItem(x, y, item); },
		data,
		params)
{
}

template<typename T>
inline Selector<T>::Selector(Console& console, Keyboard& keyboard, std::function<void(int, std::vector<T>)> on_selection,
	std::function<void(int, int, T)> on_draw_item, std::vector<T> data, SelectorParams params)
	: console(console)
	, keyboard(keyboard)
	, onSelection(on_selection)
	, onDrawItem(on_draw_item)
	, data(data)
	, params(params)
	, showCursor(true)
{
	// Initialaize the number of rows
	rows = std::ceil(data.size() / (params.columns == 0 ? 1 : params.columns));

	// Check to see if rows are less than 1
	if (rows < 1) rows = 1;

}

template<typename T>
inline Selector<T>::~Selector()
{
}

template<typename T>
inline void Selector<T>::ProcessInputs()
{
	if (keyboard.IsKeyJustPressed(KEY_W)) MoveUp();
	else if (keyboard.IsKeyJustPressed(KEY_S)) MoveDown();
	else if (keyboard.IsKeyJustPressed(KEY_A)) MoveLeft();
	else if (keyboard.IsKeyJustPressed(KEY_D)) MoveRight();
	else if (keyboard.IsKeyJustPressed(KEY_SPACE)) OnAction();
}

template<typename T>
inline void Selector<T>::MoveUp()
{
	params.currentY = std::max(params.currentY - 1, 0);
}

template<typename T>
inline void Selector<T>::MoveDown()
{
	params.currentY = std::min(params.currentY + 1, rows - 1);
}

template<typename T>
inline void Selector<T>::MoveLeft()
{
	params.currentX = std::max(params.currentX - 1, 0);
}

template<typename T>
inline void Selector<T>::MoveRight()
{
	params.currentX = std::min(params.currentX + 1, params.columns - 1);
}

template<typename T>
inline void Selector<T>::OnAction()
{
	int index = GetIndex();
	OnSelection(index, data);
}

template<typename T>
inline void Selector<T>::OnDrawItem(int x, int y, T item)
{
	// Check to see if the type is WString
	if constexpr (std::is_same<T, std::wstring>::value)
	{
		console.Write(x, y, item);
	}

	// Get the type and let user know to override this
	std::string type = typeid(item).name();
	TRPG_ERROR("Data type [" + type + "] - needs to be a wstring, please create the draw item function to override!");
}

// This function should be overridden
template<typename T>
inline void Selector<T>::OnSelection(int index, std::vector<T> data)
{
	console.Write(50, 20, L"index is: " + std::to_wstring(index));
}

template<typename T>
inline void Selector<T>::Draw()
{
	// If there is no data, nothing to draw
	if (data.empty()) return;

	int itemIndex = 0;
	int x = params.x;
	int y = params.y;
	int rowHeight = params.spacingY;
	int spacingX = params.spacingX;

	int maxData = data.size();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < params.columns; j++)
		{
			if (i == params.currentY && j == params.currentX)
			{
				if (showCursor)
				{
					// Reset the areas behind the cursor as it moves
					if (i != 0) console.Write(x - (x == 0 ? 0 : 2), y - rowHeight, L" ");
					
					console.Write(x - (x == 0 ? 0 : 2), y + rowHeight, L" ");
					console.Write(x - (x == 0 ? 0 : 2) - spacingX, y, L" ");
					console.Write(x - (x == 0 ? 0 : 2) + spacingX, y, L" ");

					// Draw the cursor
					console.Write(x - (x == 0 ? 0 : 2), y, params.cursor, RED);
				}
				else
				{
					// Clear the cursor
					console.Write(x - (x == 0 ? 0 : 2), y, L" ");
				}
			}

			if (itemIndex < maxData)
			{
				T item = data[itemIndex];
				OnDrawItem(x, y, item);
				x += spacingX;
				itemIndex++;
			}
		}

		y += rowHeight;
		x = params.x;
	}
}
