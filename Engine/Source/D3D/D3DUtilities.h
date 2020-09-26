#ifndef D3DUTILITIES_H
#define D3DUTILITIES_H

#include "..\Core\Utilities\Utilities.h"
#include <cassert>

#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

//Defines
#define D3DRelease(x) x->Release(); x = nullptr
#define D3DRelease_S(x) if (x != nullptr) { D3DRelease(x); }

//Declarations
D3D11_PRIMITIVE_TOPOLOGY convert_topology(TOPOLOGY topology);
TOPOLOGY convert_topology(D3D11_PRIMITIVE_TOPOLOGY topology);

D3D11_USAGE convert_usage(USAGE usage);
USAGE convert_usage(D3D11_USAGE usage);

std::string d3d_errorcode_to_string(HRESULT hr);
std::string d3d_featurelevel_to_string(D3D_FEATURE_LEVEL level);

//Function for handle errors when quering a resource from DX
template <class D3DInterface>
bool d3d_query_resource_ptr(D3DInterface** const dest, D3DInterface* const src)
{
	assert(dest != nullptr);
	assert(src != nullptr);

	if (FAILED(src->QueryInterface<D3DInterface>(dest)))
		return false;

	return true;
}
#endif