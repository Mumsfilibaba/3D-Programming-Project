#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "..\Math\Point.h"
#include "..\Graphics\Color.h"

void message_procedure();

enum WINDOW_STYLE : uint8
{
	//Styles
	WINDOW_STYLE_TITLE = (1 << 0),
	WINDOW_STYLE_RESIZEABLE = (1 << 1),
	WINDOW_STYLE_MINIMIZABLE = (1 << 2),
	WINDOW_STYLE_MAXIMIZEABLE = (1 << 3),
	WINDOW_STYLE_CLOSABLE = (1 << 4),
	//Standard styles
	WINDOW_STYLE_BORDERLESS = (1 << 5),
	WINDOW_STYLE_STANDARD = (WINDOW_STYLE_TITLE | WINDOW_STYLE_RESIZEABLE |
	WINDOW_STYLE_MINIMIZABLE | WINDOW_STYLE_MAXIMIZEABLE),
};

enum CURSOR : uint8
{
	CURSOR_STANDARD = 1,
	CURSOR_WAIT = 2,
	CURSOR_HELP = 3,
	CURSOR_CUSTOM = 4,
	CURSOR_HAND = 5,
};



struct WindowOptions
{
	ICON icon = ICON_STANDARD;
	CURSOR cursor = CURSOR_STANDARD;
	WINDOW_STYLE style = WINDOW_STYLE_STANDARD;
	Color backgroundColor = Color::SOFTGRAY;
};


class Window
{
public:
	Window(const Window& other) = delete;
	Window& operator=(const Window& other) = delete;

	Window(const std::wstring& title = std::wstring(L"Window"), uint16 width = 800, uint16 height = 600,
		const WindowOptions& options = WindowOptions());
	virtual ~Window();

	bool HasFocus() const;
	bool IsActive() const;
	bool Maximized() const;
	bool Minimized() const;
	void ShowMouse() const;
	void HideMouse() const;

	void Minimize() const;
	void ToggleMinimize() const;
	void Maximize() const;
	void ToggleMaximize() const;
	void Close() const;
	void Show() const;

	void Release();

	void SetTitle(const std::wstring& title);
	void SetWindowFocus(bool focus);
	void SetPosition(const Math::Point& position);
	void SetClientMaxSize(uint16 width, uint16 height);
	void SetClientMinSize(uint16 width, uint16 height);
	void SetClientSize(uint16 width, uint16 height);

	virtual void ResizeCallback(uint16 clientWidth, uint16 clientHeight)
	{
		m_ClientSize.x = clientWidth;
		m_ClientSize.y = clientHeight;
	}

	inline const std::wstring& GetTitle() const
	{
		return m_Title;
	}

	inline const Math::Point& GetClientSize() const
	{
		return m_ClientSize;
	}

	inline const Math::Point& GetMinClientSize() const
	{
		return m_MinClientSize;
	}

	inline const Math::Point& GetMaxClientSize() const
	{
		return m_MaxClientSize;
	}

	inline const Math::Point& GetPosition() const
	{
		return m_Position;
	}

	inline const WindowOptions& GetWindowDesc() const
	{
		return m_Options;
	}

	inline const void* GetHandle() const
	{
		return m_Handle;
	}

private:
	bool Initialize();

private:
	void* m_Handle;
	void* m_BackgroundHandle;

	std::wstring m_Title;
	WindowOptions m_Options;

	Math::Point m_ClientSize;
	Math::Point m_Position;
	Math::Point m_MinClientSize;
	Math::Point m_MaxClientSize;

public:
	inline static const Window& GetMainWindow()
	{
		return *s_MainWindow;
	}

	inline static void SetMainWindow(const Window& window)
	{
		s_MainWindow = (Window*)&window;
	}

private:
	static Window* s_MainWindow;
};
#endif