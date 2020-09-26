#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS

#include "..\Core\Utilities\Utilities.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::string system_get_error_string()
{
	using namespace std;

	static string result;
	LPSTR buffer = nullptr;
	DWORD error = GetLastError();

	uint32 bufferSize = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

	result = std::string(buffer, bufferSize);
	result += "Code: " + std::to_string(error);

	LocalFree(buffer);
	return result;
}

void system_messagebox(const std::string& text, const std::string& caption, ICON icon)
{
	uint32 type = MB_OK;
	if (ICON_ERROR)
		type |= MB_ICONERROR;
	else if (ICON_QUESTION)
		type |= MB_ICONQUESTION;
	else if (ICON_WARNING)
		type |= MB_ICONWARNING;
	else if (ICON_INFORMATION)
		type |= MB_ICONINFORMATION;

	MessageBoxA(0, text.c_str(), caption.c_str(), type);
}

void system_monitor_resolution(uint32& width, uint32& height)
{
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
}

Math::Point system_monitor_resolution()
{
	return Math::Point(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

#endif