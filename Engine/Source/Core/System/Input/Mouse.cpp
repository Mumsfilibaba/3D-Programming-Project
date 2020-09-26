#include "Mouse.h"
#include "..\Window.h"

namespace Input
{
	uint8 Mouse::s_Buttons;
	uint8 Mouse::s_DoubleClicked;
	uint8 Mouse::s_LastButtons;
	float Mouse::s_Scroll;
	float Mouse::s_LastScroll;
	MOUSEBUTTON Mouse::s_LastButton;
	Math::Point Mouse::s_Position;
	Math::Point Mouse::s_PositionNorm;
	Math::Point Mouse::s_LastPosition;
	Math::Point Mouse::s_GlobalPositon;



	void mouse_callback(const Math::Point& position, MOUSEBUTTON button, float scrollDelta,
		bool down, bool doubleClicked)
	{
		Mouse::s_Position = position;
		Mouse::s_Scroll += scrollDelta;


		if (down)
			Mouse::s_Buttons |= button;
		else
			Mouse::s_Buttons &= ~button;


		if (doubleClicked)
			Mouse::s_DoubleClicked |= button;
	}



	void Mouse::Update()
	{
		s_LastScroll = s_Scroll;

		s_LastPosition = s_Position;

		s_LastButtons = s_Buttons;

		s_DoubleClicked = 0;
	}



	bool Mouse::IsButtonDown(MOUSEBUTTON button)
	{
		return (s_Buttons & button);
	}

	bool Mouse::IsButtonUp(MOUSEBUTTON button)
	{
		return !(s_Buttons & button);
	}

	bool Mouse::ButtonPressed(MOUSEBUTTON button)
	{
		return ((s_Buttons & button) && !(s_LastButtons & button));
	}

	bool Mouse::ButtonDoubleClicked(MOUSEBUTTON button)
	{
		return s_DoubleClicked & button;
	}



	const Math::Point& Mouse::GetPosition()
	{
		return s_Position;
	}

	const Math::Point& Mouse::GetLastPosition()
	{
		return s_LastPosition;
	}

	float Mouse::GetScrollWheelValue()
	{
		return s_Scroll;
	}



	bool Mouse::ScrolledUp()
	{
		return (s_Scroll > s_LastScroll);
	}

	bool Mouse::ScrolledDown()
	{
		return (s_Scroll < s_LastScroll);
	}
}