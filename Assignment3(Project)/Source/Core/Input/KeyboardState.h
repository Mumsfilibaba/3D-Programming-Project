#pragma once

#include "..\..\Defines.h"

class KeyboardState
{
public:
	KeyboardState();
	KeyboardState(const KeyboardState& other);
	~KeyboardState();

	bool IsKeyDown(KEY key);
	bool IsKeyUp(KEY key);

	KeyboardState& operator=(const KeyboardState& other);

private:
	bool m_Keys[256];

public:
	static KeyboardState GetCurrentState();
};