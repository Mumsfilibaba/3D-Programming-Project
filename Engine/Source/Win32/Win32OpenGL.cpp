#include "..\Defines.h"
#include "..\Core\Utilities\Utilities.h"
#include "..\Core\Graphics\ColorF.h"
#include "..\Core\System\Window.h"
#include "..\OpenGL\GLContext.h"
#include <iostream>

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib, "opengl32.lib")

namespace Graphics
{
	bool GLContext::Initialize(const Window& window)
	{
		//Set WindowHandle
		m_WindowHandle = window.GetHandle();

		
		PIXELFORMATDESCRIPTOR pfd;
		ZeroBlock(sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;


		//Get window device context
		m_SysContext = GetDC((HWND)m_WindowHandle);
		if (m_SysContext == nullptr)
		{
			std::cout << "GLContext: Invalid Context";
			return false;
		}

		//Set pixelformat
		int pixelformat = ChoosePixelFormat((HDC)m_SysContext, &pfd);

		if (!SetPixelFormat((HDC)m_SysContext, pixelformat, &pfd))
		{
			std::cout << "GLContext: Invalid pixelformat. Error: " << system_get_error_string();
			return false;
		}

		//Create OpenGL context
		m_GLContext = wglCreateContext((HDC)m_SysContext);
		if (m_GLContext == nullptr)
		{
			std::cout << "GLContext: Invalid GLContext";
			return false;
		}


		if (!wglMakeCurrent((HDC)m_SysContext, (HGLRC)m_GLContext))
		{
			std::cout << "GLContext: Could not make context to current";
			return false;
		}

		//Init OpenGL function pointers
		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLContext: Could not initialize OpenGL extension library";
			return false;
		}

		//Print info on creation
		std::cout << "Created Context (API = " << graphicsapi_to_string(GRAPHICS_API_OPENGL) << " | OpenGL " << glGetString(GL_VERSION) << 
			", GPU = " << glGetString(GL_VENDOR) << ' ' << glGetString(GL_RENDERER) << ")\n";

		return true;
	}

	void GLContext::Release()
	{
		//Set current Context to null
		wglMakeCurrent(0, 0);

		//Release SystemContext
		ReleaseDC((HWND)m_WindowHandle, (HDC)m_SysContext);

		//delete GLContext
		wglDeleteContext((HGLRC)m_GLContext);
	}


	void GLContext::Present() const
	{
		SwapBuffers((HDC)m_SysContext);

		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClearDepthf(m_ClearDepth);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void GLContext::BindBackBuffers() const
	{
	}
}
#endif