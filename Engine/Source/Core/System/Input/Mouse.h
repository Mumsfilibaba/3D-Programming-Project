#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include "..\Window.h"

enum MOUSEBUTTON : uint8
{
	MOUSEBUTTON_LEFT = (1 << 0),
	MOUSEBUTTON_MIDDLE = (1 << 1),
	MOUSEBUTTON_RIGHT = (1 << 2),
	MOUSEBUTTON_BACK = (1 << 3),
	MOUSEBUTTON_FORWARD = (1 << 4),
	MOUSEBUTTON_UNKNOWN = (1 << 5)
};

namespace Input
{
	class Mouse
	{
	public:
		Mouse() = delete;
		~Mouse() = delete;

		static void Update();

		static float GetScrollWheelValue();
		static bool ScrolledUp();
		static bool ScrolledDown();
		static bool IsButtonDown(MOUSEBUTTON button);
		static bool IsButtonUp(MOUSEBUTTON button);
		static bool ButtonPressed(MOUSEBUTTON button);
		static bool ButtonDoubleClicked(MOUSEBUTTON button);

		static void SetPosition(const Math::Point& position, const Window& relativeWindow = Window::GetMainWindow());
		static void SetGlobalPosition(const Math::Point& position);

		static const Math::Point& GetPosition();
		static const Math::Point& GetGlobalPosition();
		static const Math::Point& GetLastPosition();

	private:
		static uint8 s_Buttons;
		static uint8 s_DoubleClicked;
		static uint8 s_LastButtons;
		static Math::Point s_PositionNorm;
		static Math::Point s_Position;
		static Math::Point s_LastPosition;
		static Math::Point s_GlobalPositon;
		static float s_Scroll;
		static float s_LastScroll;
		static MOUSEBUTTON s_LastButton;

		friend void mouse_callback(const Math::Point& position, MOUSEBUTTON button, float scrollDelta, bool down, bool doubleClicked);
	};
}
#endif