#pragma once

#include <cstring>
#include "Core\Graphics\ColorF.h"

#ifdef _WIN32
#define PLATFORM_WINDOWS
#endif

#ifdef _DEBUG
#define DEBUG
#else
#define NDEBUG
#endif

//Macros
#define Release_C_Buffer(buffer) free(buffer); buffer = nullptr
#define Release_C_Buffer_S(buffer) if (buffer != nullptr) { Release_C_Buffer(buffer); }
#define ReleaseBuffer(buffer) delete[] buffer; buffer = nullptr
#define ReleaseBuffer_S(buffer) if (buffer != nullptr) { ReleaseBuffer(buffer); }
#define ReleaseObject(object) delete object; object = nullptr
#define ReleaseObject_S(object) if (object != nullptr) { ReleaseObject(object); }
#define ZeroBlock(ptr, bytes) memset(ptr, 0, bytes)

#define MAX_TEXTURES_VS 4
#define MAX_TEXTURES_HS 4
#define MAX_TEXTURES_DS 4
#define MAX_TEXTURES_GS 4
#define MAX_TEXTURES_PS 12

//Typedefs
typedef unsigned char byte;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

//Enums
enum GRAPHICS_API : byte
{
#ifdef PLATFORM_WINDOWS
	GRAPHICS_API_OPENGL = 0,
	GRAPHICS_API_D3D11 = 1
#endif
} typedef API;

enum TEXTURECROSS : byte
{
	TEXTURECROSS_UNKNOWN = 0,
	TEXTURECROSS_VERTICAL = 1,
	TEXTURECROSS_HORIZONTAL = 2,
};

enum SHADER : byte
{
	SHADER_UNKOWN = 0,
	SHADER_PIXEL = 1,
	SHADER_VERTEX = 2,
	SHADER_GEOMETRY = 3,
	SHADER_HULL = 4,
	SHADER_DOMAIN = 5,
};

enum ATTRIBUTETYPE : byte
{
	ATTRIBUTETYPE_UNKNOWN = 0,
	ATTRIBUTETYPE_FLOAT = 1,
	ATTRIBUTETYPE_VECTOR2F = 2,
	ATTRIBUTETYPE_VECTOR3F = 3,
	ATTRIBUTETYPE_VECTOR4F = 4,
};

enum TOPOLOGY : byte
{
	TOPOLOGY_UNKNOWN = 0,
	TOPOLOGY_TRIANGLELIST = 1,
	TOPOLOGY_TRIANGLESTRIP = 2,
	TOPOLOGY_PATCH_3_POINTS = 3,
	TOPOLOGY_PATCH_4_POINTS = 4,
};

enum FILTER : byte
{
	FILTER_UNKNOWN = 0,
	FILTER_LINEAR = 1,
	FILTER_POINT = 2,
};

enum CLAMP: byte
{
	CLAMP_UNKNOWN = 0,
	CLAMP_BORDER = 1,
	CLAMP_REPEAT = 2,
	CLAMP_EDGE = 3,
};

enum TEXTUREFORMAT : byte
{
	TEXTUREFORMAT_UNKNOWN = 0,
	TEXTUREFORMAT_DEPTH16 = 1,
	TEXTUREFORMAT_DEPTH32 = 2,
	TEXTUREFORMAT_R_FLOAT32 = 3,
	TEXTUREFORMAT_R_BYTE = 4,
	TEXTUREFORMAT_RGBA_BYTE = 5,
	TEXTUREFORMAT_RGBA_FLOAT16 = 6,
};

enum COMPARISON_FUNC : byte
{
	COMPARISON_FUNC_UNKNOWN = 0,
	COMPARISON_FUNC_NEVER = 1,
	COMPARISON_FUNC_LESS = 2,
	COMPARISON_FUNC_EQUAL = 3,
	COMPARISON_FUNC_LESS_EQUAL = 4,
	COMPARISON_FUNC_GREATER = 5,
	COMPARISON_FUNC_NOT_EQUAL = 6,
	COMPARISON_FUNC_GREATER_EQUAL = 7,
	COMPARISON_FUNC_ALWAYS = 8
};

enum CULLMODE : byte
{
	CULLMODE_NONE = 0,
	CULLMODE_FRONT = 1,
	CULLMODE_BACK = 2
};

enum BUTTON : byte
{
	BUTTON_MOUSE_LEFT = (1 << 0),
	BUTTON_MOUSE_MIDDLE = (1 << 1),
	BUTTON_MOUSE_RIGHT = (1 << 2),
	BUTTON_MOUSE_BACK = (1 << 3),
	BUTTON_MOUSE_FORWARD = (1 << 4),
};

enum KEY : byte
{
#ifdef PLATFORM_WINDOWS
	//Arrowkeys
	KEY_LEFT = 0x25,
	KEY_UP = 0x26,
	KEY_RIGHT = 0x27,
	KEY_DOWN = 0x28,
	//0-9
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,
	//A-Z
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,
	//Numpad 0-9
	KEY_NUMPAD_0 = 0x60,
	KEY_NUMPAD_1 = 0x61,
	KEY_NUMPAD_2 = 0x62,
	KEY_NUMPAD_3 = 0x63,
	KEY_NUMPAD_4 = 0x64,
	KEY_NUMPAD_5 = 0x65,
	KEY_NUMPAD_6 = 0x66,
	KEY_NUMPAD_7 = 0x67,
	KEY_NUMPAD_8 = 0x68,
	KEY_NUMPAD_9 = 0x69,
	//F1-F24 
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
	KEY_F13 = 0x7C,
	KEY_F14 = 0x7D,
	KEY_F15 = 0x7E,
	KEY_F16 = 0x7F,
	KEY_F17 = 0x80,
	KEY_F18 = 0x81,
	KEY_F19 = 0x82,
	KEY_F20 = 0x83,
	KEY_F21 = 0x84,
	KEY_F22 = 0x85,
	KEY_F23 = 0x86,
	KEY_F24 = 0x87,
	//Special keys
	KEY_CTRL = 0x11,
	KEY_SHIFT = 0x10,
	KEY_LEFT_SHIFT = 0xA0,
	KEY_LEFT_CTRL = 0xA2,
	KEY_RIGHT_SHIFT = 0xA1,
	KEY_RIGHT_CTRL = 0xA3,
	KEY_ALT = 0x12,
	KEY_LEFT_ALT = 0xA4,
	KEY_ALTGR = 0xA5,
	KEY_ESCAPE = 0x1B,
	//OS-Keys
	KEY_PAGE_UP = 0x21,
	KEY_PAGE_DOWN = 0x22,
	KEY_END = 0x23,
	KEY_HOME = 0x24,
	KEY_LEFT_CMD = 0x5B,
	KEY_RIGHT_CMD = 0x5C,
	KEY_APPS = 0x5D,
	KEY_HELP = 0x2F,
	KEY_PAUSE = 0x13,
	//Textmodifiers
	KEY_INSERT = 0x2D,
	KEY_DELETE = 0x2E,
	KEY_BACKSPACE = 0x08,
	KEY_TAB = 0x09,
	KEY_CLEAR = 0x0C,
	KEY_ENTER = 0x0D,
	KEY_SPACE = 0x20,
	KEY_COMMA = 0xBC,
	KEY_PERIOD = 0xBE,
	KEY_TILDE = 0xC0,
	KEY_QUOTE = 0xDE,
	KEY_SLASH = 0xBF,
	KEY_BACKSLASH = 0xDC,
	KEY_SEMI_COLON = 0xBA,
	KEY_DECIMAL = 0x6E,
	//Math
	KEY_MINUS = 0xBD,
	KEY_PLUS = 0xBB,
	KEY_DIVIDE = 0x6F,
	KEY_MULTIPLY = 0x6A,
	KEY_SUBTRACT = 0x6D,
	KEY_ADD = 0x6B,
	//Brackets
	KEY_LEFT_BRACKET = 0xDD,
	KEY_RIGHT_BRACKET = 0xDB,
	KEY_ANGLE_BRACKET = 0xE2,
	//Toggle
	KEY_NUMLOCK = 0x90,
	KEY_CAPSLOCK = 0x14,
	KEY_SCROLLOCK = 0x91
#endif
};

//Structs
namespace Graphics
{
	struct Viewport
	{
		int32 x = 0;
		int32 y = 0;
		int32 width = 0;
		int32 height = 0;
	};

	struct DepthState
	{
		COMPARISON_FUNC comparisonFunc = COMPARISON_FUNC_LESS;
		bool depthTest = true;
		bool stencilTest = false;
	};

	struct RasterizerState
	{
		CULLMODE cull = CULLMODE_BACK;
		bool wireframe = false;
		bool depthClip = true;
		bool scissor = false;
		int depthBias = 0;
		float depthClamp = 0.0f;
		float slopeDepthBias = 0.0f;
	};

	struct SamplerSettings
	{
		FILTER filter = FILTER_LINEAR;
		CLAMP clamp = CLAMP_EDGE;
		ColorF borderColor = ColorF::WHITE;

		inline bool operator==(const SamplerSettings& other) const
		{
			return (filter == other.filter) && (clamp == other.clamp) && (borderColor == other.borderColor);
		}
	};
}
