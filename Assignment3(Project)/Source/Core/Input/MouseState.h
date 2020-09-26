#pragma once

#include "..\Math\Point.h"

class Window;

class MouseState
{
public:
	MouseState();
	~MouseState();

	bool IsButtonDown(BUTTON button) const;
	bool IsButtonUp(BUTTON button) const;
	
	inline const Math::Point& GetPosition() const
	{
		return m_Position;
	}

	MouseState& operator=(const MouseState& other);

private:
	Math::Point m_Position;
	uint8 m_Buttons;

public:
	static MouseState GetCurrentState();
	static MouseState GetCurrentState(const Window& window);
	static void SetPosition(const Math::Point& position);
	static void SetPosition(const Math::Point& position, const Window& window);
};