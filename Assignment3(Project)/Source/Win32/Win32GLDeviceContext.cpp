#include "..\Core\Window.h"
#include "..\Core\Utilities\Utilities.h"
#include "..\OpenGL\GLDeviceContext.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")

namespace Graphics
{
	bool GLDeviceContext::Create(const Window& window)
	{
		m_Window = &window;

		//Get window context
		m_SystemContext = GetDC((HWND)m_Window->GetHandle());
		if (m_SystemContext == nullptr)
		{
			std::cout << "GetDC() failed\n";
			return false;
		}


		PIXELFORMATDESCRIPTOR pfd;
		ZeroBlock(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

		//We don't care about depthbuffer for the window buffer since we will render to a seperate buffer
		//anyway
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		//Set format
		int pixelFormat = ChoosePixelFormat((HDC)m_SystemContext, &pfd);
		if (pixelFormat == 0)
		{
			std::cout << "ChoosePixelFormat() failed, see error\nError: " << system_get_last_error_string();
			return false;
		}

		if (!SetPixelFormat((HDC)m_SystemContext, pixelFormat, &pfd))
		{
			std::cout << "SetPixelFormat() failed, see error\nError: " << system_get_last_error_string();
			return false;
		}

		//Create context
		m_GLContext = wglCreateContext((HDC)m_SystemContext);
		if (m_GLContext == nullptr)
		{
			std::cout << "wglCreateContext() failed, see error\nError: " << system_get_last_error_string();
			return false;
		}

		//Set context
		if (!wglMakeCurrent((HDC)m_SystemContext, (HGLRC)m_GLContext))
		{
			std::cout << "wglMakeCurrent() failed, see error\nError: " << system_get_last_error_string();
			return false;
		}

		if (glewInit() != GLEW_OK)
		{
			std::cout << "glewInit() failed\n";
			return false;
		}

		//Check version
		GLint major = 0;
		GLint minor = 0;

		GL_Call(glGetIntegerv(GL_MAJOR_VERSION, &major));
		GL_Call(glGetIntegerv(GL_MINOR_VERSION, &minor));
		if (major < 4 && minor < 4)
		{
			std::cout << "ERROR: OpenGL 4.4 is not supported, Version supported is: " << major << '.' << minor << '\n';
			return false;
		}

#ifdef DEBUG
		//Enable debug
		GL_Call(glEnable(GL_DEBUG_OUTPUT));
#endif

		//Print API info
		std::cout << "Created Context (API = " << graphicsapi_to_string(GRAPHICS_API_OPENGL) << " | OpenGL " << glGetString(GL_VERSION) <<
			", GPU = " << glGetString(GL_VENDOR) << ' ' << glGetString(GL_RENDERER) << ")\n";

		return true;
	}

	void GLDeviceContext::Release()
	{
		// release OpenGL context
		wglMakeCurrent(0, 0);
		// release device context handle
		ReleaseDC((HWND)m_Window->GetHandle(), (HDC)m_SystemContext);
		// delete context
		wglDeleteContext((HGLRC)m_GLContext);
	}

	void GLDeviceContext::WindowResizeCallback(uint16 width, uint16 height)
	{
		//Set size to zero
		if (width < 1 || height < 1)
		{
			m_Viewport.x = 0;
			m_Viewport.y = 0;
			m_Viewport.width = 0;
			m_Viewport.height = 0;
			return;
		}

		//Do not print if size is the same
		if (m_TargetSize.x != width || m_TargetSize.y != height)
			std::cout << "Reszied target: (Width = " << width << ", height = " << height << ")\n";

		//Set size
		m_TargetSize.x = width;
		m_TargetSize.y = height;

		//Calculate size of viewport when we render to window
		if (m_Manager != nullptr)
			m_Manager->CalculateViewportSize(m_TargetSize, m_Viewport);
	}

	void GLDeviceContext::Present() const
	{
		SwapBuffers((HDC)m_SystemContext);
	}
}
#endif