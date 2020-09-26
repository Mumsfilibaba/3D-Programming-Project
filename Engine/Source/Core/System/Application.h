#ifndef APPLICATION_H
#define APPLICATION_H

#include "..\..\Defines.h"
#include "Context.h"

enum APPLICATION_FLAGS : byte
{
	APPLICATION_FLAGS_UNKNOWN = (1 << 0),
	APPLICATION_FLAGS_NO_CONSOLE = (1 << 1),
};


class Window;

class Application
{
public:
#ifdef _DEBUG
	Application(GRAPHICS_API api = GRAPHICS_API_FIRST_AVAILABLE, APPLICATION_FLAGS flags = APPLICATION_FLAGS_UNKNOWN);
#else
	Application(GRAPHICS_API api = GRAPHICS_API_FIRST_AVAILABLE, APPLICATION_FLAGS flags = APPLICATION_FLAGS_NO_CONSOLE);
#endif
	virtual ~Application();

	bool Initialize(GRAPHICS_API api, APPLICATION_FLAGS flags);
	int Run();
	void Exit(int returnCode = 0);

	
	virtual bool Load();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	inline const Graphics::Context& GetContext()
	{
		return *m_Context;
	}

	inline const Window& GetWIndow()
	{
		return *m_Window;
	}

private:
	Graphics::Context* m_Context;
	Window* m_Window;
	
	mutable bool m_Run;
	mutable int m_ReturnCode;
};
#endif