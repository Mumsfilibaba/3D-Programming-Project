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

std::string shadertype_to_string(SHADER shader)
{
	using namespace std;

	switch (shader)
	{
	case SHADER_PIXEL: return string("SHADER_PIXEL");
	case SHADER_VERTEX: return string("SHADER_VERTEX");
	case SHADER_GEOMETRY: return string("SHADER_GEOMETRY");
	case SHADER_HULL: return string("SHADER_HULL");
	case SHADER_DOMAIN: return string("SHADER_DOMAIN");
	case SHADER_UNKOWN:
	default: return string("SHADER_UNKNOWN");
	}
}

std::string key_to_string(KEY key)
{
	using namespace std;

	switch (key)
	{
	case KEY_LEFT: return string("KEY_LEFT");
	case KEY_UP: return string("KEY_UP");
	case KEY_RIGHT: return string("KEY_RIGHT");
	case KEY_DOWN: return string("KEY_DOWN");
	case KEY_0: return string("KEY_0");
	case KEY_1: return string("KEY_1");
	case KEY_2: return string("KEY_2");
	case KEY_3: return string("KEY_3");
	case KEY_4: return string("KEY_4");
	case KEY_5: return string("KEY_5");
	case KEY_6: return string("KEY_6");
	case KEY_7: return string("KEY_7");
	case KEY_8: return string("KEY_8");
	case KEY_9: return string("KEY_9");
	case KEY_A: return string("KEY_A");
	case KEY_B: return string("KEY_B");
	case KEY_C: return string("KEY_C");
	case KEY_D: return string("KEY_D");
	case KEY_E: return string("KEY_E");
	case KEY_F: return string("KEY_F");
	case KEY_G: return string("KEY_G");
	case KEY_H: return string("KEY_H");
	case KEY_I: return string("KEY_I");
	case KEY_J: return string("KEY_J");
	case KEY_K: return string("KEY_K");
	case KEY_L: return string("KEY_L");
	case KEY_M: return string("KEY_M");
	case KEY_N: return string("KEY_N");
	case KEY_O: return string("KEY_O");
	case KEY_P: return string("KEY_P");
	case KEY_Q: return string("KEY_Q");
	case KEY_R: return string("KEY_R");
	case KEY_S: return string("KEY_S");
	case KEY_T: return string("KEY_T");
	case KEY_U: return string("KEY_U");
	case KEY_V: return string("KEY_V");
	case KEY_W: return string("KEY_W");
	case KEY_X: return string("KEY_X");
	case KEY_Y: return string("KEY_Y");
	case KEY_Z: return string("KEY_Z");
	case KEY_NUMPAD_0: return string("KEY_NUMPAD_0");
	case KEY_NUMPAD_1: return string("KEY_NUMPAD_1");
	case KEY_NUMPAD_2: return string("KEY_NUMPAD_2");
	case KEY_NUMPAD_3: return string("KEY_NUMPAD_3");
	case KEY_NUMPAD_4: return string("KEY_NUMPAD_4");
	case KEY_NUMPAD_5: return string("KEY_NUMPAD_5");
	case KEY_NUMPAD_6: return string("KEY_NUMPAD_6");
	case KEY_NUMPAD_7: return string("KEY_NUMPAD_7");
	case KEY_NUMPAD_8: return string("KEY_NUMPAD_8");
	case KEY_NUMPAD_9: return string("KEY_NUMPAD_9");
	case KEY_F1: return string("KEY_F1");
	case KEY_F2: return string("KEY_F2");
	case KEY_F3: return string("KEY_F3");
	case KEY_F4: return string("KEY_F4");
	case KEY_F5: return string("KEY_F5");
	case KEY_F6: return string("KEY_F6");
	case KEY_F7: return string("KEY_F7");
	case KEY_F8: return string("KEY_F8");
	case KEY_F9: return string("KEY_F9");
	case KEY_F10: return string("KEY_F10");
	case KEY_F11: return string("KEY_F11");
	case KEY_F12: return string("KEY_F12");
	case KEY_F13: return string("KEY_F13");
	case KEY_F14: return string("KEY_F14");
	case KEY_F15: return string("KEY_F15");
	case KEY_F16: return string("KEY_F16");
	case KEY_F17: return string("KEY_F17");
	case KEY_F18: return string("KEY_F18");
	case KEY_F19: return string("KEY_F19");
	case KEY_F20: return string("KEY_F20");
	case KEY_F21: return string("KEY_F21");
	case KEY_F22: return string("KEY_F22");
	case KEY_F23: return string("KEY_F23");
	case KEY_F24: return string("KEY_F24");
	case KEY_CTRL: return string("KEY_CTRL");
	case KEY_SHIFT: return string("KEY_SHIFT");
	case KEY_LEFT_SHIFT: return string("KEY_LEFT_SHIFT");
	case KEY_LEFT_CTRL: return string("KEY_LEFT_CTRL");
	case KEY_RIGHT_SHIFT: return string("KEY_RIGHT_SHIFT");
	case KEY_RIGHT_CTRL: return string("KEY_RIGHT_CTRL");
	case KEY_ALT: return string("KEY_ALT");
	case KEY_LEFT_ALT: return string("KEY_LEFT_ALT");
	case KEY_ALTGR: return string("KEY_ALTGR");
	case KEY_ESCAPE: return string("KEY_ESCAPE");
	case KEY_PAGE_UP: return string("KEY_PAGE_UP");
	case KEY_PAGE_DOWN: return string("KEY_PAGE_DOWN");
	case KEY_END: return string("KEY_END");
	case KEY_HOME: return string("KEY_HOME");
	case KEY_LEFT_CMD: return string("KEY_LEFT_CMD");
	case KEY_RIGHT_CMD: return string("KEY_RIGHT_CMD");
	case KEY_APPS: return string("KEY_APPS");
	case KEY_HELP: return string("KEY_HELP");
	case KEY_PAUSE: return string("KEY_PAUSE");
	case KEY_INSERT: return string("KEY_INSERT");
	case KEY_DELETE: return string("KEY_DELETE");
	case KEY_BACKSPACE: return string("KEY_BACKSPACE");
	case KEY_TAB: return string("KEY_TAB");
	case KEY_CLEAR: return string("KEY_CLEAR");
	case KEY_ENTER: return string("KEY_ENTER");
	case KEY_SPACE: return string("KEY_SPACE");
	case KEY_COMMA: return string("KEY_COMMA");
	case KEY_PERIOD: return string("KEY_PERIOD");
	case KEY_TILDE: return string("KEY_TILDE");
	case KEY_QUOTE: return string("KEY_QUOTE");
	case KEY_SLASH: return string("KEY_SLASH");
	case KEY_BACKSLASH: return string("KEY_BACKSLASH");
	case KEY_SEMI_COLON: return string("KEY_SEMI_COLON");
	case KEY_DECIMAL: return string("KEY_DECIMAL");
	case KEY_MINUS: return string("KEY_MINUS");
	case KEY_PLUS: return string("KEY_PLUS");
	case KEY_DIVIDE: return string("KEY_DIVIDE");
	case KEY_MULTIPLY: return string("KEY_MULTIPLY");
	case KEY_SUBTRACT: return string("KEY_SUBTRACT");
	case KEY_ADD: return string("KEY_ADD");
	case KEY_LEFT_BRACKET: return string("KEY_LEFT_BRACKET");
	case KEY_RIGHT_BRACKET: return string("KEY_RIGHT_BRACKET");
	case KEY_ANGLE_BRACKET: return string("KEY_ANGLE_BRACKET");
	case KEY_NUMLOCK: return string("KEY_NUMLOCK");
	case KEY_CAPSLOCK: return string("KEY_CAPSLOCK");
	case KEY_SCROLLOCK: return string("KEY_SCROLLOCK");
	default: return string("KEY_UNKNOWN");
	}
}

uint32 read_file(const std::string& filename, const char** const buffer)
{
	FILE* file = nullptr;
	file = fopen(filename.c_str(), "rb");
	if (file == nullptr)
		return 0;

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
		return 0;

	fseek(file, 0, SEEK_END);
	int64 filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*buffer = (const char*)calloc(filesize, sizeof(char));
	uint32 bytesRead = (uint32)fread((void*)*buffer, sizeof(uint8), filesize, file);

	fclose(file);
	return bytesRead;
}

uint32 get_attribute_stride(ATTRIBUTETYPE attribute)
{
	if (attribute == ATTRIBUTETYPE_FLOAT)
		return sizeof(float);
	else if (attribute == ATTRIBUTETYPE_VECTOR2F)
		return (sizeof(float) * 2);
	else if (attribute == ATTRIBUTETYPE_VECTOR3F)
		return (sizeof(float) * 3);
	else if (attribute == ATTRIBUTETYPE_VECTOR4F)
		return (sizeof(float) * 4);

	return 0;
}

uint32 get_attribute_components(ATTRIBUTETYPE attribute)
{
	if (attribute == ATTRIBUTETYPE_FLOAT)
		return 1;
	else if (attribute == ATTRIBUTETYPE_VECTOR2F)
		return 2;
	else if (attribute == ATTRIBUTETYPE_VECTOR3F)
		return 3;
	else if (attribute == ATTRIBUTETYPE_VECTOR4F)
		return 4;

	return 0;
}