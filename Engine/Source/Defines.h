#ifndef DEFINES_H
#define DEFINES_H

#ifdef _WIN32
#define PLATFORM_WINDOWS
#endif

#ifdef _DEBUG
#define DEBUG
#endif

//Unsinged typedefs
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
//Types typedefs
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

//Misc typedefs
typedef unsigned char byte;

//Engine Macros
#define ReleaseObject(x) delete x; x = nullptr
#define ReleaseObject_S(x) if (x != nullptr) { ReleaseObject(x); }
#define ReleaseBuffer(x) delete[] x; x = nullptr
#define ReleaseBuffer_S(x) if (x != nullptr) { ReleaseBuffer(x); }
#define ZeroBlock(byteWidth, addressData) memset(addressData, 0, byteWidth)

//Engine Constants
#define MINWINDOWWIDTH 160
#define MINWINDOWHEIGHT 90
#define VIEWPORT_WIDTH 800.0f
#define VIEWPORT_HEIGHT 600.0f
#define MAX_DEPTH 1.0f
#define MIN_DEPTH 0.0f


//Engine enums
enum ICON : byte
{
	ICON_ERROR = 1,
	ICON_QUESTION = 2,
	ICON_WARNING = 3,
	ICON_INFORMATION = 4,
	ICON_STANDARD = 5
};

enum GRAPHICS_API : byte
{
	GRAPHICS_API_UNKNOWN = 0,
	GRAPHICS_API_FIRST_AVAILABLE = 1,
	GRAPHICS_API_D3D11 = 2,
	GRAPHICS_API_OPENGL = 3,
};

enum TOPOLOGY : byte
{
	TOPOLOGY_UNKNOWN = 0,
	TOPOLOGY_TRIANGLELIST = 1,
	TOPOLOGY_TRIANGLESTRIP = 2
};

enum USAGE : byte
{
	USAGE_UNKNOWN = 0,
	USAGE_DYNAMIC = 1,
	USAGE_DEFAULT = 2,
	USAGE_STATIC = 3,
};

#endif