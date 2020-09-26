#include "Window.h"

Window::Window()
	: m_Handle(nullptr)
{
}

Window::Window(const wchar_t* title, uint16 width, uint16 height)
	: m_Handle(nullptr)
{
	Create(title, width, height);
}

Window::~Window()
{
	Release();
}