#ifndef KEYBOARDSTATE_H
#define KEYBOARDSTATE_H

#include "Keycode.h"

namespace Input
{
	class Keyboard
	{
	public:
		Keyboard() = delete;
		~Keyboard() = delete;

		static bool IsKeyDown(KEYCODE key);
		static bool IsKeyUp(KEYCODE key);
		static bool IsToggled(KEYCODE key);
		static bool KeyPressed(KEYCODE key);

		static char GetLastChar();
		static KEYCODE GetLastKeyPressed();
		static KEYCODE GetLastKeyReleased();

		static void Update();

	private:
		static void PlatformUpdate();

	private:
		static bool s_Keys[KEYCODE_UNKNOWN];
		static bool s_LastKeys[KEYCODE_UNKNOWN];
		static uint8 s_Toggle;
		static char s_LastChar;
		static KEYCODE s_LastPressed;
		static KEYCODE s_LastReleased;

		friend void character_callback(char character);
		friend void keyboard_callback(KEYCODE key, uint8 specialkeys, bool down);
	};
}
#endif