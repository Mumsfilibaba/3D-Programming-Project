#include "Utilities.h"
#include <iostream>

std::string graphicsapi_to_string(GRAPHICS_API api)
{
	using namespace std;

	switch (api)
	{
	case GRAPHICS_API_D3D11: return string("GRAPHICS_API_D3D11");
	case GRAPHICS_API_OPENGL: return string("GRAPHICS_API_OPENGL");
	default: return string("GRAPHICS_API_UNKNOWN");
	}
}

uint32 read_file(const std::string& filename, const char** const buffer)
{
	FILE* file = nullptr;
	file = fopen(filename.c_str(), "rb");
	if (file == nullptr)
	{
		std::cout << "'read_file': Could not load file '" << filename << '\'';
		return 0;
	}

	fseek(file, 0, SEEK_END);
	int64 filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*buffer = (const char*)calloc(filesize, sizeof(char));
	uint32 bytesRead = (uint32)fread((void*)*buffer, sizeof(uint8), filesize, file);

	fclose(file);
	return bytesRead;
}

uint32 read_textfile(const std::string& filename, const char** const buffer)
{
	FILE* file = nullptr;
	file = fopen(filename.c_str(), "rt");
	if (file == nullptr)
	{
		std::cout << "'read_textfile': Could not load file '" << filename << '\'';
		return 0;
	}

	fseek(file, 0, SEEK_END);
	int64 filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*buffer = (const char*)calloc(filesize, sizeof(char));
	uint32 bytesRead = (uint32)fread((void*)*buffer, sizeof(uint8), filesize, file);

	fclose(file);
	return bytesRead;
}