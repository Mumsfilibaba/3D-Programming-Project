
#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS

#ifndef WIN32KEYCODES_H
#define WIN32KEYCODES_H

enum WIN32KEYCODE : uint8
{
	//Arrowkeys
	WIN32KEYCODE_LEFT = 0x25,
	WIN32KEYCODE_UP = 0x26,
	WIN32KEYCODE_RIGHT = 0x27,
	WIN32KEYCODE_DOWN = 0x28,
	//0-9
	WIN32KEYCODE_0 = 0x30,
	WIN32KEYCODE_1 = 0x31,
	WIN32KEYCODE_2 = 0x32,
	WIN32KEYCODE_3 = 0x33,
	WIN32KEYCODE_4 = 0x34,
	WIN32KEYCODE_5 = 0x35,
	WIN32KEYCODE_6 = 0x36,
	WIN32KEYCODE_7 = 0x37,
	WIN32KEYCODE_8 = 0x38,
	WIN32KEYCODE_9 = 0x39,
	//A-Z
	WIN32KEYCODE_A = 0x41,
	WIN32KEYCODE_B = 0x42,
	WIN32KEYCODE_C = 0x43,
	WIN32KEYCODE_D = 0x44,
	WIN32KEYCODE_E = 0x45,
	WIN32KEYCODE_F = 0x46,
	WIN32KEYCODE_G = 0x47,
	WIN32KEYCODE_H = 0x48,
	WIN32KEYCODE_I = 0x49,
	WIN32KEYCODE_J = 0x4A,
	WIN32KEYCODE_K = 0x4B,
	WIN32KEYCODE_L = 0x4C,
	WIN32KEYCODE_M = 0x4D,
	WIN32KEYCODE_N = 0x4E,
	WIN32KEYCODE_O = 0x4F,
	WIN32KEYCODE_P = 0x50,
	WIN32KEYCODE_Q = 0x51,
	WIN32KEYCODE_R = 0x52,
	WIN32KEYCODE_S = 0x53,
	WIN32KEYCODE_T = 0x54,
	WIN32KEYCODE_U = 0x55,
	WIN32KEYCODE_V = 0x56,
	WIN32KEYCODE_W = 0x57,
	WIN32KEYCODE_X = 0x58,
	WIN32KEYCODE_Y = 0x59,
	WIN32KEYCODE_Z = 0x5A,
	//Numpad 0-9
	WIN32KEYCODE_NUMPAD_0 = 0x60,
	WIN32KEYCODE_NUMPAD_1 = 0x61,
	WIN32KEYCODE_NUMPAD_2 = 0x62,
	WIN32KEYCODE_NUMPAD_3 = 0x63,
	WIN32KEYCODE_NUMPAD_4 = 0x64,
	WIN32KEYCODE_NUMPAD_5 = 0x65,
	WIN32KEYCODE_NUMPAD_6 = 0x66,
	WIN32KEYCODE_NUMPAD_7 = 0x67,
	WIN32KEYCODE_NUMPAD_8 = 0x68,
	WIN32KEYCODE_NUMPAD_9 = 0x69,
	//F1-F24 
	WIN32KEYCODE_F1 = 0x70,
	WIN32KEYCODE_F2 = 0x71,
	WIN32KEYCODE_F3 = 0x72,
	WIN32KEYCODE_F4 = 0x73,
	WIN32KEYCODE_F5 = 0x74,
	WIN32KEYCODE_F6 = 0x75,
	WIN32KEYCODE_F7 = 0x76,
	WIN32KEYCODE_F8 = 0x77,
	WIN32KEYCODE_F9 = 0x78,
	WIN32KEYCODE_F10 = 0x79,
	WIN32KEYCODE_F11 = 0x7A,
	WIN32KEYCODE_F12 = 0x7B,
	WIN32KEYCODE_F13 = 0x7C,
	WIN32KEYCODE_F14 = 0x7D,
	WIN32KEYCODE_F15 = 0x7E,
	WIN32KEYCODE_F16 = 0x7F,
	WIN32KEYCODE_F17 = 0x80,
	WIN32KEYCODE_F18 = 0x81,
	WIN32KEYCODE_F19 = 0x82,
	WIN32KEYCODE_F20 = 0x83,
	WIN32KEYCODE_F21 = 0x84,
	WIN32KEYCODE_F22 = 0x85,
	WIN32KEYCODE_F23 = 0x86,
	WIN32KEYCODE_F24 = 0x87,
	//Special keys
	WIN32KEYCODE_CTRL = 0x11,
	WIN32KEYCODE_SHIFT = 0x10,
	WIN32KEYCODE_LEFT_SHIFT = 0xA0,
	WIN32KEYCODE_LEFT_CTRL = 0xA2,
	WIN32KEYCODE_RIGHT_SHIFT = 0xA1,
	WIN32KEYCODE_RIGHT_CTRL = 0xA3,
	WIN32KEYCODE_ALT = 0x12,
	WIN32KEYCODE_LEFT_ALT = 0xA4,
	WIN32KEYCODE_ALTGR = 0xA5,
	WIN32KEYCODE_ESCAPE = 0x1B,
	//OS-Keys
	WIN32KEYCODE_PAGE_UP = 0x21,
	WIN32KEYCODE_PAGE_DOWN = 0x22,
	WIN32KEYCODE_END = 0x23,
	WIN32KEYCODE_HOME = 0x24,
	WIN32KEYCODE_LEFT_WIN_CMD = 0x5B,
	WIN32KEYCODE_RIGHT_WIN_CMD = 0x5C,
	WIN32KEYCODE_APPS = 0x5D,
	WIN32KEYCODE_HELP = 0x2F,
	WIN32KEYCODE_PAUSE = 0x13,
	//Textmodifiers
	WIN32KEYCODE_INSERT = 0x2D,
	WIN32KEYCODE_DELETE = 0x2E,
	WIN32KEYCODE_BACKSPACE = 0x08,
	WIN32KEYCODE_TAB = 0x09,
	WIN32KEYCODE_CLEAR = 0x0C,
	WIN32KEYCODE_ENTER = 0x0D,
	WIN32KEYCODE_SPACE = 0x20,
	WIN32KEYCODE_COMMA = 0xBC,
	WIN32KEYCODE_PERIOD = 0xBE,
	WIN32KEYCODE_TILDE = 0xC0,
	WIN32KEYCODE_QUOTE = 0xDE,
	WIN32KEYCODE_SLASH = 0xBF,
	WIN32KEYCODE_BACKSLASH = 0xDC,
	WIN32KEYCODE_SEMI_COLON = 0xBA,
	WIN32KEYCODE_DECIMAL = 0x6E,
	//Math
	WIN32KEYCODE_MINUS = 0xBD,
	WIN32KEYCODE_PLUS = 0xBB,
	WIN32KEYCODE_DIVIDE = 0x6F,
	WIN32KEYCODE_MULTIPLY = 0x6A,
	WIN32KEYCODE_SUBTRACT = 0x6D,
	WIN32KEYCODE_ADD = 0x6B,
	//Brackets
	WIN32KEYCODE_LEFT_BRACKET = 0xDD,
	WIN32KEYCODE_RIGHT_BRACKET = 0xDB,
	WIN32KEYCODE_ANGLE_BRACKET = 0xE2,
	//Toggle
	WIN32KEYCODE_NUMLOCK = 0x90,
	WIN32KEYCODE_CAPSLOCK = 0x14,
	WIN32KEYCODE_SCROLLOCK = 0x91,
};

#endif

#endif