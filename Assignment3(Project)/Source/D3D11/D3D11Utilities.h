#pragma once

#include <string>
#include "..\Defines.h"
#include <dxgi.h>
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#define D3DRelease(COMObject) COMObject->Release(); COMObject = nullptr
#define D3DRelease_S(COMObject) if (COMObject != nullptr) { D3DRelease(COMObject); }

std::string d3d_feature_level_as_string(D3D_FEATURE_LEVEL fLevel);

D3D11_COMPARISON_FUNC d3d_convert_comparison_dunc(COMPARISON_FUNC comparison);

DXGI_FORMAT d3d_convert_textureformat(TEXTUREFORMAT format, uint32 bindflags);

void d3d_convert_samplersettings(D3D11_SAMPLER_DESC& desc, const Graphics::SamplerSettings& setting);