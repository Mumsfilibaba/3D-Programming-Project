#include "Window.h"
#include "..\Utilities\Utilities.h"

Window* Window::s_MainWindow = nullptr;

Window::Window(const std::wstring& title, uint16 width, uint16 height, const WindowOptions& options)
	: m_Handle(nullptr),
	m_BackgroundHandle(nullptr),
	m_Title(title),
	m_ClientSize(width, height),
	m_Position((system_monitor_resolution() / 2) - (m_ClientSize / 2)),
	m_MinClientSize(MINWINDOWWIDTH, MINWINDOWHEIGHT),
	m_MaxClientSize(system_monitor_resolution()),
	m_Options(options)
{
	if (s_MainWindow == nullptr)
		s_MainWindow = this;


	Initialize();
}

Window::~Window()
{
	Release();
}

void Window::SetClientMaxSize(uint16 width, uint16 height)
{
	Math::Point size(system_monitor_resolution());

	if (m_MaxClientSize.y > size.y)
		m_MaxClientSize.y = size.y;
	else if (m_MaxClientSize.y < MINWINDOWHEIGHT)
		m_MaxClientSize.y = MINWINDOWHEIGHT;
	else
		m_MaxClientSize.y = height;


	if (m_MaxClientSize.x > size.x)
		m_MaxClientSize.x = size.x;
	else if (m_MaxClientSize.x < MINWINDOWWIDTH)
		m_MaxClientSize.x = MINWINDOWWIDTH;
	else
		m_MaxClientSize.x = width;
}

void Window::SetClientMinSize(uint16 width, uint16 height)
{
	Math::Point size(system_monitor_resolution());

	if (m_MinClientSize.y < MINWINDOWHEIGHT)
		m_MinClientSize.y = MINWINDOWHEIGHT;
	else if (m_MinClientSize.y > size.y)
		m_MinClientSize.y = size.y;
	else
		m_MinClientSize.y = height;


	if (m_MinClientSize.x < MINWINDOWWIDTH)
		m_MinClientSize.x = MINWINDOWWIDTH;
	if (m_MinClientSize.x > size.x)
		m_MinClientSize.x = size.x;
	else
		m_MinClientSize.x = width;
}