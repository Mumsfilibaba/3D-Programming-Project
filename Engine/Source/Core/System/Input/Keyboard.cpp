#include <cstring>
#include "Keyboard.h"

namespace Input
{
	bool Keyboard::s_Keys[];
	bool Keyboard::s_LastKeys[];
	char Keyboard::s_LastChar;
	uint8 Keyboard::s_Toggle;
	KEYCODE Keyboard::s_LastPressed;
	KEYCODE Keyboard::s_LastReleased;



	void character_callback(char character)
	{
		Keyboard::s_LastChar = character;
	}



	void keyboard_callback(KEYCODE key, uint8 specialkeys, bool down)
	{
		Keyboard::s_Keys[key] = down;

		if (specialkeys & SPECIALKEYS_LEFT_SHIFT)
			Keyboard::s_Keys[KEYCODE_LEFT_SHIFT] = true;
		else
			Keyboard::s_Keys[KEYCODE_LEFT_SHIFT] = false;

		if (specialkeys & SPECIALKEYS_RIGHT_SHIFT)
			Keyboard::s_Keys[KEYCODE_RIGHT_SHIFT] = true;
		else
			Keyboard::s_Keys[KEYCODE_RIGHT_SHIFT] = false;


		if (specialkeys & SPECIALKEYS_LEFT_CTRL)
			Keyboard::s_Keys[KEYCODE_LEFT_CTRL] = true;
		else
			Keyboard::s_Keys[KEYCODE_LEFT_CTRL] = false;

		if (specialkeys & SPECIALKEYS_RIGHT_CTRL)
			Keyboard::s_Keys[KEYCODE_RIGHT_CTRL] = true;
		else
			Keyboard::s_Keys[KEYCODE_RIGHT_CTRL] = false;


		if (specialkeys & SPECIALKEYS_LEFT_ALT)
			Keyboard::s_Keys[KEYCODE_LEFT_ALT] = true;
		else
			Keyboard::s_Keys[KEYCODE_LEFT_ALT] = false;

		if (specialkeys & SPECIALKEYS_ALTGR)
			Keyboard::s_Keys[KEYCODE_ALTGR] = true;
		else
			Keyboard::s_Keys[KEYCODE_ALTGR] = false;
	}



	bool Keyboard::IsKeyDown(KEYCODE key)
	{
		return s_Keys[key];
	}

	bool Keyboard::IsKeyUp(KEYCODE key)
	{
		return !s_Keys[key];
	}

	bool Keyboard::IsToggled(KEYCODE key)
	{
		if (key == KEYCODE_CAPSLOCK)
			return s_Toggle & TOGGLEKEY_CAPSLOCK;
		else if (key == KEYCODE_NUMLOCK)
			return s_Toggle & TOGGLEKEY_NUMLOCK;
		else if (key == KEYCODE_SCROLLOCK)
			return s_Toggle & TOGGLEKEY_SCROLLOCK;
		else
			return false;
	}

	bool Keyboard::KeyPressed(KEYCODE key)
	{
		return (s_Keys[key] && !s_LastKeys[key]);
	}



	char Keyboard::GetLastChar()
	{
		return s_LastChar;
	}

	KEYCODE Keyboard::GetLastKeyPressed()
	{
		return s_LastPressed;
	}

	KEYCODE Keyboard::GetLastKeyReleased()
	{
		return Keyboard::s_LastReleased;
	}



	void Keyboard::Update()
	{
		memcpy(s_LastKeys, s_Keys, KEYCODE_UNKNOWN);

		PlatformUpdate();
	}
}