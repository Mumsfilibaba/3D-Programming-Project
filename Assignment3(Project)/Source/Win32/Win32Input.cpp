#include "..\Core\Input\KeyboardState.h"
#include "..\Core\Input\MouseState.h"
#include "..\Core\Window.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

KeyboardState KeyboardState::GetCurrentState()
{
	KeyboardState currentState;

	for (int32 i = 0; i < 256; i++)
		currentState.m_Keys[i] = (GetAsyncKeyState(i) & 0x8000) ? true : false;

	return currentState;
}

MouseState MouseState::GetCurrentState()
{
	MouseState currentState;

	currentState.m_Buttons |= (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? BUTTON_MOUSE_LEFT : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? BUTTON_MOUSE_MIDDLE : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? BUTTON_MOUSE_RIGHT : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ? BUTTON_MOUSE_BACK : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ? BUTTON_MOUSE_FORWARD : 0;

	POINT pos;
	GetCursorPos(&pos);
	currentState.m_Position = Math::Point(pos.x, pos.y);

	return currentState;
}

MouseState MouseState::GetCurrentState(const Window& window)
{
	MouseState currentState;

	currentState.m_Buttons |= (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? BUTTON_MOUSE_LEFT : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? BUTTON_MOUSE_MIDDLE : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? BUTTON_MOUSE_RIGHT : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ? BUTTON_MOUSE_BACK : 0;
	currentState.m_Buttons |= (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ? BUTTON_MOUSE_FORWARD : 0;

	POINT pos;
	GetCursorPos(&pos);
	
	HWND hwnd = reinterpret_cast<HWND>(window.GetHandle());
	ScreenToClient(hwnd, &pos);

	currentState.m_Position = Math::Point(pos.x, pos.y);

	return currentState;
}

void MouseState::SetPosition(const Math::Point& position)
{
	SetCursorPos(position.x, position.y);
}

void MouseState::SetPosition(const Math::Point& position, const Window& window)
{
	POINT pos;
	pos.x = position.x;
	pos.y = position.y;

	HWND hwnd = reinterpret_cast<HWND>(window.GetHandle());
	ClientToScreen(hwnd, &pos);

	SetCursorPos(pos.x, pos.y);
}

#endif