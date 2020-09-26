#include "KeyboardState.h"

KeyboardState::KeyboardState()
	: m_Keys()
{
	ZeroBlock(m_Keys, sizeof(bool) * 256);
}

KeyboardState::KeyboardState(const KeyboardState& other)
	: m_Keys()
{
	memcpy(m_Keys, other.m_Keys, sizeof(bool) * 256);
}

KeyboardState::~KeyboardState()
{
}

bool KeyboardState::IsKeyDown(KEY key)
{
	return m_Keys[key];
}

bool KeyboardState::IsKeyUp(KEY key)
{
	return !m_Keys[key];
}

KeyboardState& KeyboardState::operator=(const KeyboardState & other)
{
	if (this != &other)
		memcpy(m_Keys, other.m_Keys, sizeof(bool) * 256);

	return *this;
}