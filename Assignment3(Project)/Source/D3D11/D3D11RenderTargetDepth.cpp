#include <cassert>
#include <iostream>
#include "D3D11RenderTargetDepth.h"

namespace Graphics
{
	D3D11RenderTargetDepth::D3D11RenderTargetDepth(const D3D11DeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format)
		: m_View(nullptr),
		m_Texture(context)
	{
		Create(width, height, format);
	}

	D3D11RenderTargetDepth::~D3D11RenderTargetDepth()
	{
		Release();
	}

	bool D3D11RenderTargetDepth::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		assert(m_Texture.GetContext() != nullptr);

		if (!m_Texture.Create(nullptr, width, height, D3D11_BIND_DEPTH_STENCIL, format, false))
			return false;

		D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
		ZeroBlock(&desc2, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc2.Texture2D.MipSlice = 0;
		desc2.Format = d3d_convert_textureformat(format, D3D11_BIND_DEPTH_STENCIL);
		desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		ID3D11Device* device = (reinterpret_cast<const D3D11DeviceContext*>(m_Texture.GetContext()))->GetDevice();
		if (FAILED(device->CreateDepthStencilView(m_Texture.GetDXTexturePtr(), &desc2, &m_View)))
		{
			std::cout << "ERROR: Could not create RenderTargetDepth View\n";
			return false;
		}

		return true;
	}

	void D3D11RenderTargetDepth::Release()
	{
		D3DRelease_S(m_View);
		m_Texture.Release();
	}
}