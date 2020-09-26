#include "MouseState.h"

MouseState::MouseState()
	: m_Buttons(0),
	m_Position()
{
}

MouseState::~MouseState()
{
}

bool MouseState::IsButtonDown(BUTTON button) const
{
	return (m_Buttons & button);
}

bool MouseState::IsButtonUp(BUTTON button) const
{
	return !(m_Buttons & button);
}

MouseState& MouseState::operator=(const MouseState& other)
{
	if (this != &other)
	{
		m_Buttons = other.m_Buttons;
		m_Position = other.m_Position;
	}

	return *this;
}
