#include "..\Core\Utilities\Utilities.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

std::string system_get_last_error_string()
{
	//Return the last error-code as a string
	using namespace std;

	string result;
	LPSTR buffer = nullptr;
	DWORD error = GetLastError();

	uint32 bufferSize = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

	result = std::string(buffer, bufferSize);
	result += "Code: " + std::to_string(error);

	LocalFree(buffer);
	return result;
}

bool system_directory_exists(const std::string& path)
{
	//Check if a directory exists by creating a handle for it
	bool result = true;
	HANDLE handle = CreateFileA(path.c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, 0,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);


	if (handle == 0)
		result = false;
	

	CloseHandle(handle);
	return result;
}

bool system_file_exists(const std::string& filename, const std::string& path)
{
	return system_directory_exists(filename + path);
}

#endif