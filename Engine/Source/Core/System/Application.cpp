#include "..\Utilities\Utilities.h"
#include "Application.h"
#include "Window.h"
#include "Context.h"
#include "Input\Input.h"
#include <iostream>

Application::Application(GRAPHICS_API api, APPLICATION_FLAGS flags)
	: m_Context(nullptr),
	m_Window(nullptr),
	m_Run(true),
	m_ReturnCode(0)
{
	Initialize(api, flags);
}

Application::~Application()
{
	Release();
}

bool Application::Initialize(GRAPHICS_API api, APPLICATION_FLAGS flags)
{
	bool result = true;

	if (m_Window == nullptr)
		m_Window = new Window();

	if (m_Context == nullptr)
	{
		m_Context = Graphics::Context::Create(api, *m_Window);

		if (m_Context == nullptr)
		{
			std::string message("Could not create Context");

			system_messagebox(message, "ERROR", ICON_ERROR);
			std::cout << message << '\n';

			result = false;
		}
	}


	if (!result)
		Exit(-1);
	
	return result;
}

bool Application::Load()
{
	return false;
}

void Application::Update()
{
}

void Application::Render()
{
}

void Application::Release()
{
	ReleaseObject_S(m_Window);
	ReleaseObject_S(m_Context);
}

int Application::Run()
{
	Load();


	//Show window after everything is loaded
	if (m_Window != nullptr)
		m_Window->Show();


	//Program loop
	while (m_Run)
	{
		//Handle OS-Messages
		message_procedure();
		
		m_Context->SetViewport((float)m_Window->GetClientSize().x, (float)m_Window->GetClientSize().y, 0, 0);

		//Update game
		Update();
		
		//Render game
		Render();


		m_Context->Present();


		if (!m_Window->IsActive())
			Exit(0);
	}

	Release();

	return m_ReturnCode;
}

void Application::Exit(int returnCode)
{
	m_Run = false;
	m_ReturnCode = returnCode;
}