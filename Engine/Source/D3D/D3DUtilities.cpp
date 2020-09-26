#include "D3DUtilities.h"

D3D11_PRIMITIVE_TOPOLOGY convert_topology(TOPOLOGY topology)
{
	switch (topology)
	{
	case TOPOLOGY_TRIANGLELIST: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case TOPOLOGY_TRIANGLESTRIP: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

TOPOLOGY convert_topology(D3D11_PRIMITIVE_TOPOLOGY topology) 
{
	switch (topology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST: return TOPOLOGY_TRIANGLELIST;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: return TOPOLOGY_TRIANGLESTRIP;
	default: return TOPOLOGY_UNKNOWN;
	}
}


D3D11_USAGE convert_usage(USAGE usage)
{
	switch (usage)
	{
	case USAGE_STATIC: return D3D11_USAGE_IMMUTABLE;
	case USAGE_DYNAMIC: return D3D11_USAGE_DYNAMIC;
	case USAGE_DEFAULT:
	case USAGE_UNKNOWN:
	default: return D3D11_USAGE_DEFAULT;
	}
}

USAGE convert_usage(D3D11_USAGE usage)
{
	switch (usage)
	{
	case D3D11_USAGE_IMMUTABLE: return USAGE_STATIC;
	case D3D11_USAGE_DYNAMIC: return USAGE_DYNAMIC;
	case D3D11_USAGE_DEFAULT: return USAGE_DEFAULT;
	default: return USAGE_UNKNOWN;
	}
}


std::string d3d_errorcode_to_string(HRESULT hr)
{
	std::string result = "Direct3D failed with code: ";

	switch (hr)
	{
	case S_OK: result = "NO ERROR, EVERYTHING OK"; break;
	case E_INVALIDARG: result += "E_INVALIDARG"; break;
	case DXGI_ERROR_DEVICE_HUNG: result += "DXGI_ERROR_DEVICE_HUNG"; break;
	case DXGI_ERROR_DEVICE_REMOVED: result += "DXGI_ERROR_DEVICE_REMOVED"; break;
	case DXGI_ERROR_DEVICE_RESET: result += "DXGI_ERROR_DEVICE_RESET"; break;
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR: result += "DXGI_ERROR_DRIVER_INTERNAL_ERROR"; break;
	case DXGI_ERROR_FRAME_STATISTICS_DISJOINT: result += "DXGI_ERROR_FRAME_STATISTICS_DISJOINT"; break;
	case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE: result += "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE"; break;
	case DXGI_ERROR_INVALID_CALL: result += "DXGI_ERROR_INVALID_CALL"; break;
	case DXGI_ERROR_MORE_DATA: result += "DXGI_ERROR_MORE_DATA"; break;
	case DXGI_ERROR_NONEXCLUSIVE: result += "DXGI_ERROR_NONEXCLUSIVE"; break;
	case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE: result += "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE"; break;
	case DXGI_ERROR_NOT_FOUND: result += "DXGI_ERROR_NOT_FOUND"; break;
	case DXGI_ERROR_WAS_STILL_DRAWING: result += "DXGI_ERROR_WAS_STILL_DRAWING"; break;
	case DXGI_ERROR_UNSUPPORTED: result += "DXGI_ERROR_UNSUPPORTED"; break;
	case DXGI_ERROR_ACCESS_LOST: result += "DXGI_ERROR_ACCESS_LOST"; break;
	case DXGI_ERROR_WAIT_TIMEOUT: result += "DXGI_ERROR_WAIT_TIMEOUT"; break;
	case DXGI_ERROR_SESSION_DISCONNECTED: result += "DXGI_ERROR_SESSION_DISCONNECTED"; break;
	case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE: result += "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE"; break;
	case DXGI_ERROR_CANNOT_PROTECT_CONTENT: result += "DXGI_ERROR_CANNOT_PROTECT_CONTENT"; break;
	case DXGI_ERROR_ACCESS_DENIED: result += "DXGI_ERROR_ACCESS_DENIED"; break;
	case DXGI_ERROR_NAME_ALREADY_EXISTS: result += "DXGI_ERROR_NAME_ALREADY_EXISTS"; break;
	case DXGI_ERROR_SDK_COMPONENT_MISSING: result += "DXGI_ERROR_SDK_COMPONENT_MISSING"; break;
	default: result = "Unknown error code"; break;
	}

	return result;
}

std::string d3d_featurelevel_to_string(D3D_FEATURE_LEVEL level)
{
	using namespace std;

	switch (level)
	{
	case D3D_FEATURE_LEVEL_9_1: return string("D3D_FEATURE_LEVEL_9_1");
	case D3D_FEATURE_LEVEL_9_2:  return string("D3D_FEATURE_LEVEL_9_2");
	case D3D_FEATURE_LEVEL_9_3: return string("D3D_FEATURE_LEVEL_9_3");
	case D3D_FEATURE_LEVEL_10_0: return string("D3D_FEATURE_LEVEL_10_0");
	case D3D_FEATURE_LEVEL_10_1: return string("D3D_FEATURE_LEVEL_10_1");
	case D3D_FEATURE_LEVEL_11_0: return string("D3D_FEATURE_LEVEL_11_0");
	case D3D_FEATURE_LEVEL_11_1: return string("D3D_FEATURE_LEVEL_11_1");
	default: return string("UNKNOWN");
	}
}