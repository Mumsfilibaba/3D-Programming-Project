#include "..\Core\Window.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "..\Core\Utilities\Utilities.h"
#include <iostream>
#include <string>
#include <windows.h>
#include "..\D3D11\D3D11DeviceContext.h"
#include "..\OpenGL\GLDeviceContext.h"


LRESULT CALLBACK std_win_proc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);
LRESULT handle_message(Window& window, uint32 message, WPARAM wParam, LPARAM lParam);


bool Window::IsActive() const
{
	return (IsWindow((HWND)m_Handle) != FALSE);
}

bool Window::HasFocus() const
{
	return (GetFocus() == m_Handle);
}

bool Window::Create(const wchar_t* title, uint16 width, uint16 height)
{
	using namespace std;

	WNDCLASSEX wndClass;
	ZeroBlock(&wndClass, sizeof(WNDCLASSEX));
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(0);
	wndClass.lpfnWndProc = std_win_proc;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = (HCURSOR)LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = (HICON)LoadIcon(0, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpszClassName = L"WND_CLASS";


	if (RegisterClassExW(&wndClass) == 0)
	{
		cout << "Could not register window:\nError: " << system_get_last_error_string();
		return false;
	}


	m_Size = Math::Point(width, height);
	RECT client = { 0, 0, (LONG)m_Size.x, (LONG)m_Size.y };
	AdjustWindowRect(&client, WS_OVERLAPPEDWINDOW, false);

	int posX = (GetSystemMetrics(SM_CXSCREEN) - (client.right - client.left)) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - (client.bottom - client.top)) / 2;


	m_Handle = CreateWindowEx(0, wndClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, posX,
		posY, client.right - client.left, client.bottom - client.top, 0, 0, wndClass.hInstance, 0);


	if (!IsWindow((HWND)m_Handle))
	{
		cout << "Could not create window.\nError: " << system_get_last_error_string();
		return false;
	}


	SetWindowLongPtr((HWND)m_Handle, GWLP_USERDATA, (uintptr_t)this);


	cout << "Created Window (ClientWidth = " << to_string(m_Size.x) << ", ClientHeight = " << to_string(m_Size.y) << ")\n";
	return true;
}

bool Window::CreateContext(GRAPHICS_API api, const Graphics::RenderSettings& manager)
{
	using namespace Graphics;

	switch (api)
	{
	case GRAPHICS_API_D3D11: m_Context = new D3D11DeviceContext(*this, manager); break;
	case GRAPHICS_API_OPENGL: m_Context = new GLDeviceContext(*this, manager); break;
	}

	return true;
}

void Window::Show() const
{
	ShowWindow((HWND)m_Handle, SW_NORMAL);
}

void Window::Release()
{
	ReleaseObject_S(m_Context);

	if (m_Handle != nullptr)
	{
		DestroyWindow((HWND)m_Handle);
		m_Handle = nullptr;
	}
}

void Window::OnResize(uint16 width, uint16 height)
{
	m_Size.x = width;
	m_Size.y = height;

	m_Context->WindowResizeCallback(width, height);
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
	case WM_SIZE:
		window.OnResize(LOWORD(lParam), HIWORD(lParam));
	default:
		return DefWindowProc((HWND)window.GetHandle(), message, wParam, lParam);
	}
}

#endif