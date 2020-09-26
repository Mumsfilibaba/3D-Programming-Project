#include <cassert>
#include <iostream>
#include "D3D11RenderTarget2D.h"

namespace Graphics
{
	D3D11RenderTarget2D::D3D11RenderTarget2D(const D3D11DeviceContext* const context, uint16 width, uint16 height, 
		TEXTUREFORMAT format)
		: m_View(nullptr),
		m_Texture(context)
	{
		Create(width, height, format);
	}

	D3D11RenderTarget2D::~D3D11RenderTarget2D()
	{
		Release();
	}

	bool D3D11RenderTarget2D::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		assert(m_Texture.GetContext() != nullptr);

		//Create texture
		if (!m_Texture.Create(nullptr, width, height, D3D11_BIND_RENDER_TARGET, format, false))
			return false;

		//Create view
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_BIND_RENDER_TARGET));
		desc.Format = m_Texture.GetDXFormat();
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;


		ID3D11Device* device = (reinterpret_cast<const D3D11DeviceContext*>(m_Texture.GetContext()))->GetDevice();
		if (FAILED(device->CreateRenderTargetView(m_Texture.GetDXTexturePtr(), &desc, &m_View)))
		{
			std::cout << "ERROR: Could not create RenderTarget2D\n";
			return false;
		}

		return true;
	}

	void D3D11RenderTarget2D::Release()
	{
		D3DRelease_S(m_View);

		m_Texture.Release();
	}
}