#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "..\Math\Point.h"
#include "..\..\Defines.h"

void system_messagebox(const std::string& text, const std::string& caption, ICON icon);
void system_monitor_resolution(uint32& width, uint32& height);
Math::Point system_monitor_resolution();
std::string system_get_error_string();

std::string graphicsapi_to_string(GRAPHICS_API api);

uint32 read_file(const std::string& filename, const char** const buffer);
uint32 read_textfile(const std::string& filename, const char** const buffer);
#endif