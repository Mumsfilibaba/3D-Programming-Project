#pragma once

#include "..\Defines.h"
#include "Math\Point.h"
#include "Graphics\API\IDeviceContext.h"

class Window
{
public:
	Window();
	Window(const wchar_t* title, uint16 width = 800, uint16 height = 600);
	~Window();

	bool IsActive() const;
	bool HasFocus() const;
	bool Create(const wchar_t* title, uint16 width = 800, uint16 height = 600);
	bool CreateContext(GRAPHICS_API api, const Graphics::RenderSettings& manager);
	void Show() const;
	void Release();

	void OnResize(uint16 width, uint16 height);

	inline void* GetHandle() const 
	{
		return m_Handle;
	}

	inline const Math::Point& GetSize() const
	{
		return m_Size;
	}

	inline const Graphics::IDeviceContext* GetDeviceContext() const
	{
		return m_Context;
	}

private:
	void* m_Handle;
	Graphics::IDeviceContext* m_Context;
	Math::Point m_Size;
};