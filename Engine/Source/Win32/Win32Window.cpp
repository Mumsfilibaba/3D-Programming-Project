#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS

#include "..\Core\System\Window.h"
#include "..\Core\System\Input\Keycode.h"
#include "..\Core\System\Input\Mouse.h"
#include "..\Core\System\Input\Keyboard.h"
#include "..\Core\Utilities\Utilities.h"
#include "Win32Keycode.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <iostream>


void message_procedure()
{
	MSG msg;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



LRESULT CALLBACK std_win_proc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);
LRESULT handle_mouse_event(uint32 message, WPARAM wParam, LPARAM lParam);
LRESULT handle_keyboard_event(uint32 message, WPARAM wParam, LPARAM lParam);
LRESULT handle_message(Window& window, uint32 message, WPARAM wParam, LPARAM lParam);
HICON set_icon(ICON icon);
HCURSOR set_cursor(CURSOR cursor);
KEYCODE translate_key(WPARAM wParam);



HICON set_icon(ICON icon)
{
	switch (icon)
	{
	case ICON_QUESTION: return LoadIcon(0, IDI_QUESTION); break;
	case ICON_WARNING: return LoadIcon(0, IDI_WARNING); break;
	case ICON_ERROR: return LoadIcon(0, IDI_ERROR); break;
	case ICON_INFORMATION: return LoadIcon(0, IDI_INFORMATION); break;
	case ICON_STANDARD:
	default: return LoadIcon(0, IDI_APPLICATION); break;
	}
}

HCURSOR set_cursor(CURSOR cursor)
{
	switch (cursor)
	{
	case CURSOR_WAIT: return LoadCursor(0, IDC_WAIT); break;
	case CURSOR_HELP: return LoadCursor(0, IDC_HELP); break;
	case CURSOR_HAND: return LoadCursor(0, IDC_HAND); break;
	case CURSOR_STANDARD:
	default: return LoadCursor(0, IDC_ARROW); break;
	}
}



void Window::Release()
{
	DeleteObject((HBRUSH)m_BackgroundHandle);
}



bool Window::Initialize()
{
	using namespace std;

	COLORREF color = RGB(m_Options.backgroundColor.r, m_Options.backgroundColor.g, m_Options.backgroundColor.b);
	m_BackgroundHandle = (void*)CreateSolidBrush(color);


	WNDCLASSEX wndClass;
	ZeroBlock(sizeof(WNDCLASSEX), &wndClass);
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(0);
	wndClass.lpfnWndProc = std_win_proc;
	wndClass.hbrBackground = (HBRUSH)m_BackgroundHandle;
	wndClass.hCursor = set_cursor(m_Options.cursor);
	wndClass.hIcon = set_icon(m_Options.icon);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.lpszClassName = L"DefWndClass";


	if (RegisterClassExW(&wndClass) == 0)
	{
		cout << "Could not register window:\nError: " << system_get_error_string();
		return false;
	}


	uint32 wndStyle = WS_OVERLAPPED;
	if (m_Options.style & WINDOW_STYLE_TITLE)
		wndStyle |= WS_CAPTION;
	if (m_Options.style & WINDOW_STYLE_RESIZEABLE)
		wndStyle |= WS_THICKFRAME;
	if (m_Options.style & WINDOW_STYLE_MINIMIZABLE)
		wndStyle |= WS_MINIMIZEBOX | WS_SYSMENU;
	if (m_Options.style & WINDOW_STYLE_MAXIMIZEABLE)
	{
		if (wndStyle & WS_SYSMENU)
			wndStyle |= WS_MAXIMIZEBOX;
		else
			wndStyle |= WS_MAXIMIZEBOX | WS_SYSMENU;
	}
	if (m_Options.style & WINDOW_STYLE_CLOSABLE)
		wndStyle |= WS_SYSMENU;
	if (m_Options.style & WINDOW_STYLE_BORDERLESS)
		wndStyle = WS_OVERLAPPED | WS_POPUP;


	RECT client = { 0, 0, (LONG)m_ClientSize.x, (LONG)m_ClientSize.y };
	AdjustWindowRect(&client, wndStyle, false);

	int posX = (GetSystemMetrics(SM_CXSCREEN) - (client.right - client.left)) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - (client.bottom - client.top)) / 2;


	m_Handle = CreateWindowEx(0, wndClass.lpszClassName, m_Title.c_str(), wndStyle, posX,
		posY, client.right - client.left, client.bottom - client.top, 0, 0, wndClass.hInstance, 0);

	if (!IsWindow((HWND)m_Handle))
	{
		cout << "Could not create window.\nError: " << system_get_error_string();
		return false;
	}


	SetWindowLongPtr((HWND)m_Handle, GWLP_USERDATA, (uintptr_t)this);


	cout << "Created Window (ClientWidth = " << to_string(m_ClientSize.x) << ", ClientHeight = " << to_string(m_ClientSize.y) << ")\n";
	return true;
}



void Window::Show() const
{
	ShowWindow((HWND)m_Handle, SW_SHOWNORMAL);
}

void Window::Minimize() const
{
	if (!Minimized())
		CloseWindow((HWND)m_Handle);
}

void Window::ToggleMinimize() const
{
	if (!Minimized())
		CloseWindow((HWND)m_Handle);
	else
		ShowWindow((HWND)m_Handle, SW_RESTORE);
}

void Window::Maximize() const
{
	if (!Maximized())
		ShowWindow((HWND)m_Handle, SW_SHOWMAXIMIZED);
}

void Window::ToggleMaximize() const
{
	if (!Maximized())
		ShowWindow((HWND)m_Handle, SW_SHOWMAXIMIZED);
	else
		ShowWindow((HWND)m_Handle, SW_RESTORE);
}

void Window::Close() const
{
	DestroyWindow((HWND)m_Handle);
}



void Window::ShowMouse() const
{
	if (ShowCursor(true) > 0)
		ShowCursor(false);
}

void Window::HideMouse() const
{
	if (ShowCursor(false) < -1)
		ShowCursor(true);
}



bool Window::HasFocus() const
{
	return (m_Handle == GetFocus());
}

bool Window::IsActive() const
{
	return (IsWindow((HWND)m_Handle) != 0);
}

bool Window::Maximized() const
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);

	GetWindowPlacement((HWND)m_Handle, &wp);

	return wp.showCmd == SW_SHOWMAXIMIZED;
}

bool Window::Minimized() const
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);

	GetWindowPlacement((HWND)m_Handle, &wp);

	return wp.showCmd == SW_SHOWMINIMIZED;
}



void Window::SetWindowFocus(bool focus)
{
	focus ? SetFocus((HWND)m_Handle) : SetFocus(0);
}

void Window::SetPosition(const Math::Point& position)
{
	MoveWindow((HWND)m_Handle, position.x, position.y, GetClientSize().x, GetClientSize().y, true);
}

void Window::SetTitle(const std::wstring& title)
{
	SetWindowTextW((HWND)m_Handle, title.c_str());
}

void Window::SetClientSize(uint16 width, uint16 height)
{
	MoveWindow((HWND)m_Handle, GetPosition().x, GetPosition().y, width, height, true);
}



KEYCODE translate_key(WPARAM wParam)
{
	switch (wParam)
	{
	//Arrowkeys
	case WIN32KEYCODE_LEFT: return KEYCODE_LEFT;
	case WIN32KEYCODE_UP: return KEYCODE_UP;
	case WIN32KEYCODE_RIGHT: return KEYCODE_RIGHT;
	case WIN32KEYCODE_DOWN: return KEYCODE_DOWN;
	//0-9
	case WIN32KEYCODE_0: return KEYCODE_0;
	case WIN32KEYCODE_1: return KEYCODE_1;
	case WIN32KEYCODE_2: return KEYCODE_2;
	case WIN32KEYCODE_3: return KEYCODE_3;
	case WIN32KEYCODE_4: return KEYCODE_4;
	case WIN32KEYCODE_5: return KEYCODE_5;
	case WIN32KEYCODE_6: return KEYCODE_6;
	case WIN32KEYCODE_7: return KEYCODE_7;
	case WIN32KEYCODE_8: return KEYCODE_8;
	case WIN32KEYCODE_9: return KEYCODE_9;
	//A-Z
	case WIN32KEYCODE_A: return KEYCODE_A;
	case WIN32KEYCODE_B: return KEYCODE_B;
	case WIN32KEYCODE_C: return KEYCODE_C;
	case WIN32KEYCODE_D: return KEYCODE_D;
	case WIN32KEYCODE_E: return KEYCODE_E;
	case WIN32KEYCODE_F: return KEYCODE_F;
	case WIN32KEYCODE_G: return KEYCODE_G;
	case WIN32KEYCODE_H: return KEYCODE_H;
	case WIN32KEYCODE_I: return KEYCODE_I;
	case WIN32KEYCODE_J: return KEYCODE_J;
	case WIN32KEYCODE_K: return KEYCODE_K;
	case WIN32KEYCODE_L: return KEYCODE_L;
	case WIN32KEYCODE_M: return KEYCODE_M;
	case WIN32KEYCODE_N: return KEYCODE_N;
	case WIN32KEYCODE_O: return KEYCODE_O;
	case WIN32KEYCODE_P: return KEYCODE_P;
	case WIN32KEYCODE_Q: return KEYCODE_Q;
	case WIN32KEYCODE_R: return KEYCODE_R;
	case WIN32KEYCODE_S: return KEYCODE_S;
	case WIN32KEYCODE_T: return KEYCODE_T;
	case WIN32KEYCODE_U: return KEYCODE_U;
	case WIN32KEYCODE_V: return KEYCODE_V;
	case WIN32KEYCODE_W: return KEYCODE_W;
	case WIN32KEYCODE_X: return KEYCODE_X;
	case WIN32KEYCODE_Y: return KEYCODE_Y;
	case WIN32KEYCODE_Z: return KEYCODE_Z;
	//Numpad 0-9
	case WIN32KEYCODE_NUMPAD_0: return KEYCODE_NUMPAD_0;
	case WIN32KEYCODE_NUMPAD_1: return KEYCODE_NUMPAD_1;
	case WIN32KEYCODE_NUMPAD_2: return KEYCODE_NUMPAD_2;
	case WIN32KEYCODE_NUMPAD_3: return KEYCODE_NUMPAD_3;
	case WIN32KEYCODE_NUMPAD_4: return KEYCODE_NUMPAD_4;
	case WIN32KEYCODE_NUMPAD_5: return KEYCODE_NUMPAD_5;
	case WIN32KEYCODE_NUMPAD_6: return KEYCODE_NUMPAD_6;
	case WIN32KEYCODE_NUMPAD_7: return KEYCODE_NUMPAD_7;
	case WIN32KEYCODE_NUMPAD_8: return KEYCODE_NUMPAD_8;
	case WIN32KEYCODE_NUMPAD_9: return KEYCODE_NUMPAD_9;
	//F1-F12 
	case WIN32KEYCODE_F1: return KEYCODE_F1;
	case WIN32KEYCODE_F2: return KEYCODE_F2;
	case WIN32KEYCODE_F3: return KEYCODE_F3;
	case WIN32KEYCODE_F4: return KEYCODE_F4;
	case WIN32KEYCODE_F5: return KEYCODE_F5;
	case WIN32KEYCODE_F6: return KEYCODE_F6;
	case WIN32KEYCODE_F7: return KEYCODE_F7;
	case WIN32KEYCODE_F8: return KEYCODE_F8;
	case WIN32KEYCODE_F9: return KEYCODE_F9;
	case WIN32KEYCODE_F10: return KEYCODE_F10;
	case WIN32KEYCODE_F11: return KEYCODE_F11;
	case WIN32KEYCODE_F13: return KEYCODE_F13;
	case WIN32KEYCODE_F14: return KEYCODE_F14;
	case WIN32KEYCODE_F15: return KEYCODE_F15;
	case WIN32KEYCODE_F16: return KEYCODE_F16;
	case WIN32KEYCODE_F17: return KEYCODE_F17;
	case WIN32KEYCODE_F18: return KEYCODE_F18;
	case WIN32KEYCODE_F19: return KEYCODE_F19;
	case WIN32KEYCODE_F20: return KEYCODE_F20;
	case WIN32KEYCODE_F21: return KEYCODE_F21;
	case WIN32KEYCODE_F22: return KEYCODE_F22;
	case WIN32KEYCODE_F23: return KEYCODE_F23;
	case WIN32KEYCODE_F24: return KEYCODE_F24;
	//OS-Keys
	case WIN32KEYCODE_PAGE_UP: return KEYCODE_PAGE_UP;
	case WIN32KEYCODE_PAGE_DOWN: return KEYCODE_PAGE_DOWN;
	case WIN32KEYCODE_END: return KEYCODE_END;
	case WIN32KEYCODE_HOME: return KEYCODE_HOME;
	case WIN32KEYCODE_LEFT_WIN_CMD: return KEYCODE_LEFT_WIN_CMD;
	case WIN32KEYCODE_RIGHT_WIN_CMD: return KEYCODE_RIGHT_WIN_CMD;
	case WIN32KEYCODE_APPS: return KEYCODE_APPS;
	case WIN32KEYCODE_HELP: return KEYCODE_HELP;
	case WIN32KEYCODE_PAUSE: return KEYCODE_PAUSE;
	case WIN32KEYCODE_ESCAPE: return KEYCODE_ESCAPE;
	//Textmodifiers
	case WIN32KEYCODE_INSERT: return KEYCODE_INSERT;
	case WIN32KEYCODE_DELETE: return KEYCODE_DELETE;
	case WIN32KEYCODE_BACKSPACE: return KEYCODE_BACKSPACE;
	case WIN32KEYCODE_TAB: return KEYCODE_TAB;
	case WIN32KEYCODE_CLEAR: return KEYCODE_CLEAR;
	case WIN32KEYCODE_ENTER: return KEYCODE_ENTER;
	case WIN32KEYCODE_SPACE: return KEYCODE_SPACE;
	case WIN32KEYCODE_COMMA: return KEYCODE_COMMA;
	case WIN32KEYCODE_PERIOD: return KEYCODE_PERIOD;
	case WIN32KEYCODE_TILDE: return KEYCODE_TILDE;
	case WIN32KEYCODE_QUOTE: return KEYCODE_QUOTE;
	case WIN32KEYCODE_SLASH: return KEYCODE_SLASH;
	case WIN32KEYCODE_BACKSLASH: return KEYCODE_BACKSLASH;
	case WIN32KEYCODE_SEMI_COLON: return KEYCODE_SEMI_COLON;
	case WIN32KEYCODE_DECIMAL: return KEYCODE_DECIMAL;
	//Math
	case WIN32KEYCODE_MINUS: return KEYCODE_MINUS;
	case WIN32KEYCODE_PLUS: return KEYCODE_PLUS;
	case WIN32KEYCODE_DIVIDE: return KEYCODE_DIVIDE;
	case WIN32KEYCODE_MULTIPLY: return KEYCODE_MULTIPLY;
	case WIN32KEYCODE_SUBTRACT: return KEYCODE_SUBTRACT;
	case WIN32KEYCODE_ADD: return KEYCODE_ADD;
	//Brackets
	case WIN32KEYCODE_LEFT_BRACKET: return KEYCODE_LEFT_BRACKET;
	case WIN32KEYCODE_RIGHT_BRACKET: return KEYCODE_RIGHT_BRACKET;
	case WIN32KEYCODE_ANGLE_BRACKET: return KEYCODE_ANGLE_BRACKET;
	//Modifiers
	case WIN32KEYCODE_CTRL: return KEYCODE_CTRL;
	case WIN32KEYCODE_SHIFT: return KEYCODE_SHIFT;
	case WIN32KEYCODE_ALT: return KEYCODE_ALT;
	//Toggle
	case WIN32KEYCODE_NUMLOCK: return KEYCODE_NUMLOCK;
	case WIN32KEYCODE_CAPSLOCK: return KEYCODE_CAPSLOCK;
	case WIN32KEYCODE_SCROLLOCK: return KEYCODE_SCROLLOCK;
	default: return KEYCODE_UNKNOWN;
	}
}



LRESULT CALLBACK std_win_proc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam)
{
	Window* win = (Window*)(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	if (win != nullptr)
		return handle_message(*win, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}



LRESULT handle_message(Window& window, uint32 message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_GETMINMAXINFO:
		if (!window.Maximized())
		{
			LONG style = (LONG)GetWindowLongPtr((HWND)window.GetHandle(), GWL_STYLE);

			RECT client = { 0, 0, (LONG)window.GetMaxClientSize().x, (LONG)window.GetMaxClientSize().y };
			AdjustWindowRect(&client, style, false);

			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = client.right - client.left;
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = client.bottom - client.top;


			client = { 0, 0, (LONG)window.GetMinClientSize().x, (LONG)window.GetMinClientSize().y };
			AdjustWindowRect(&client, style, false);


			((MINMAXINFO*)lParam)->ptMinTrackSize.x = client.right - client.left;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = client.bottom - client.top;
		}

		return 0;
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		return handle_keyboard_event(message, wParam, lParam);
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
		return handle_mouse_event(message, wParam, lParam);
	case WM_SIZE:
		window.ResizeCallback(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	return DefWindowProc((HWND)window.GetHandle(), message, wParam, lParam);
}



LRESULT handle_keyboard_event(uint32 message, WPARAM wParam, LPARAM lParam)
{
	bool down = false;
	uint8 specialKeys = 0;

	switch (message)
	{
	case WM_CHAR:
		Input::character_callback((char)wParam);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		down = true;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		if ((GetKeyState(WIN32KEYCODE_LEFT_CTRL) & 0x8000))
			specialKeys |= SPECIALKEYS_LEFT_CTRL;

		if ((GetKeyState(WIN32KEYCODE_RIGHT_CTRL) & 0x8000))
			specialKeys |= SPECIALKEYS_RIGHT_CTRL;


		if ((GetKeyState(WIN32KEYCODE_LEFT_SHIFT) & 0x8000))
			specialKeys |= SPECIALKEYS_LEFT_SHIFT;

		if ((GetKeyState(WIN32KEYCODE_RIGHT_SHIFT) & 0x8000))
			specialKeys |= SPECIALKEYS_RIGHT_SHIFT;


		if ((GetKeyState(WIN32KEYCODE_ALTGR) & 0x8000))
			specialKeys |= SPECIALKEYS_ALTGR;

		if ((GetKeyState(WIN32KEYCODE_LEFT_ALT) & 0x8000))
			specialKeys |= SPECIALKEYS_LEFT_ALT;

		Input::keyboard_callback(translate_key(wParam), specialKeys, down);

	default: return 0;
	}
}



LRESULT handle_mouse_event(uint32 message, WPARAM wParam, LPARAM lParam)
{
	using namespace Math;

	MOUSEBUTTON button = MOUSEBUTTON_UNKNOWN;
	bool down = false;
	bool doubleClicked = false;
	float scrollDelta = 0.0f;

	uint32 t = 0;
	switch (message)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_MOUSEWHEEL:
		scrollDelta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		break;
	case WM_LBUTTONUP:
		button = MOUSEBUTTON_LEFT;
		break;
	case WM_MBUTTONUP:
		button = MOUSEBUTTON_MIDDLE;
		break;
	case WM_RBUTTONUP:
		button = MOUSEBUTTON_RIGHT;
		break;
	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			button = MOUSEBUTTON_BACK;
		else
			button = MOUSEBUTTON_FORWARD;
		break;
	case WM_LBUTTONDOWN:
		button = MOUSEBUTTON_LEFT;
		down = true;
		break;
	case WM_MBUTTONDOWN:
		button = MOUSEBUTTON_MIDDLE;
		down = true;
		break;
	case WM_RBUTTONDOWN:
		button = MOUSEBUTTON_RIGHT;
		down = true;
		break;
	case WM_XBUTTONDOWN:
		down = true;
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			button = MOUSEBUTTON_BACK;
		else
			button = MOUSEBUTTON_FORWARD;
		break;
	case WM_LBUTTONDBLCLK:
		doubleClicked = true;
		button = MOUSEBUTTON_LEFT;
		break;
	case WM_MBUTTONDBLCLK:
		doubleClicked = true;
		button = MOUSEBUTTON_MIDDLE;
		break;
	case WM_RBUTTONDBLCLK:
		doubleClicked = true;
		button = MOUSEBUTTON_RIGHT;
		break;
	case WM_XBUTTONDBLCLK:
		doubleClicked = true;
		if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			button = MOUSEBUTTON_BACK;
		else
			button = MOUSEBUTTON_FORWARD;
		break;
	}

	Input::mouse_callback(Math::Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), button, scrollDelta, down, doubleClicked);
	return 0;
}

#endif