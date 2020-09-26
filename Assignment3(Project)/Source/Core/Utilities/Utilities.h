#pragma once

#include <string>
#include "..\..\Defines.h"

std::string system_get_last_error_string();
bool system_directory_exists(const std::string& path);
bool system_file_exists(const std::string& filename, const std::string& path);

std::string graphicsapi_to_string(GRAPHICS_API api);
std::string shadertype_to_string(SHADER shader);
std::string key_to_string(KEY key);

uint32 read_file(const std::string& file, const char** const buffer);
uint32 read_textfile(const std::string& file, const char** const buffer);

uint32 get_attribute_stride(ATTRIBUTETYPE attribute);
uint32 get_attribute_components(ATTRIBUTETYPE attribute);