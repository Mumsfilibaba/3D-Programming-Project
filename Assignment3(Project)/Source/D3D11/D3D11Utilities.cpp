#include "D3D11Utilities.h"

std::string d3d_feature_level_as_string(D3D_FEATURE_LEVEL fLevel)
{
	using namespace std;

	switch (fLevel)
	{
	case D3D_FEATURE_LEVEL_12_1: return string("D3D_FEATURE_LEVEL_12_1");
	case D3D_FEATURE_LEVEL_12_0: return string("D3D_FEATURE_LEVEL_12_0");
	case D3D_FEATURE_LEVEL_11_1: return string("D3D_FEATURE_LEVEL_11_1");
	case D3D_FEATURE_LEVEL_11_0: return string("D3D_FEATURE_LEVEL_11_0");
	case D3D_FEATURE_LEVEL_10_1: return string("D3D_FEATURE_LEVEL_10_1");
	case D3D_FEATURE_LEVEL_10_0: return string("D3D_FEATURE_LEVEL_10_0");
	case D3D_FEATURE_LEVEL_9_3: return string("D3D_FEATURE_LEVEL_9_3");
	case D3D_FEATURE_LEVEL_9_2: return string("D3D_FEATURE_LEVEL_9_2");
	case D3D_FEATURE_LEVEL_9_1: return string("D3D_FEATURE_LEVEL_9_1");
	default: return string("D3D_FEATURE_LEVEL_UNKNOWN");
	}
}

D3D11_COMPARISON_FUNC d3d_convert_comparison_dunc(COMPARISON_FUNC comparison)
{
	switch (comparison)
	{
	case COMPARISON_FUNC_NEVER: return D3D11_COMPARISON_NEVER;
	case COMPARISON_FUNC_LESS: return D3D11_COMPARISON_LESS;
	case COMPARISON_FUNC_EQUAL: return D3D11_COMPARISON_EQUAL;
	case COMPARISON_FUNC_LESS_EQUAL: return D3D11_COMPARISON_LESS_EQUAL;
	case COMPARISON_FUNC_GREATER: return D3D11_COMPARISON_GREATER;
	case COMPARISON_FUNC_NOT_EQUAL: return D3D11_COMPARISON_NOT_EQUAL;
	case COMPARISON_FUNC_GREATER_EQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
	case COMPARISON_FUNC_ALWAYS: return D3D11_COMPARISON_ALWAYS;
	case COMPARISON_FUNC_UNKNOWN: 
	default: return (D3D11_COMPARISON_FUNC)0;
	}
}

DXGI_FORMAT d3d_convert_textureformat(TEXTUREFORMAT format, uint32 bindflag)
{
	switch (format)
	{
	case TEXTUREFORMAT_DEPTH16: 
		if (bindflag == D3D11_BIND_DEPTH_STENCIL)
			return DXGI_FORMAT_D16_UNORM;
		else if (bindflag == D3D11_BIND_SHADER_RESOURCE)
			return DXGI_FORMAT_R16_UNORM;

		return DXGI_FORMAT_R16_TYPELESS;
	case TEXTUREFORMAT_DEPTH32: 
		if (bindflag == D3D11_BIND_DEPTH_STENCIL)
			return DXGI_FORMAT_D32_FLOAT;
		else if (bindflag == D3D11_BIND_SHADER_RESOURCE)
			return DXGI_FORMAT_R32_FLOAT;

		return DXGI_FORMAT_R32_TYPELESS;
	case TEXTUREFORMAT_R_FLOAT32: return DXGI_FORMAT_R32_FLOAT;
	case TEXTUREFORMAT_RGBA_FLOAT16: return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case TEXTUREFORMAT_R_BYTE: return DXGI_FORMAT_R8_UNORM;
	case TEXTUREFORMAT_RGBA_BYTE: return DXGI_FORMAT_R8G8B8A8_UNORM;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

void d3d_convert_samplersettings(D3D11_SAMPLER_DESC& desc, const Graphics::SamplerSettings& setting)
{
	desc.BorderColor[0] = setting.borderColor.r * 255;
	desc.BorderColor[1] = setting.borderColor.g * 255;
	desc.BorderColor[2] = setting.borderColor.b * 255;
	desc.BorderColor[3] = 255;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = -FLT_MAX;

	desc.MaxAnisotropy = 1;
	switch (setting.filter)
	{
	case FILTER_LINEAR: desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; break;
	case FILTER_POINT:
	default: desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; break;
	}

	switch (setting.clamp)
	{
	case CLAMP_BORDER: 
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	case CLAMP_REPEAT:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case CLAMP_EDGE:
	default:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}
}