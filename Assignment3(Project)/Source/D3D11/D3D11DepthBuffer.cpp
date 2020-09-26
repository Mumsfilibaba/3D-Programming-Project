#include <cassert>
#include <iostream>
#include "D3D11DepthBuffer.h"

namespace Graphics
{
	D3D11DepthBuffer::D3D11DepthBuffer(const D3D11DeviceContext * const context, uint16 width, uint16 height, TEXTUREFORMAT format)
		: m_Context(context),
		m_View(nullptr),
		m_Width(0),
		m_Height(0)
	{
		Create(width, height, format);
	}

	D3D11DepthBuffer::~D3D11DepthBuffer()
	{
		Release();
	}

	bool D3D11DepthBuffer::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		assert(m_Context != nullptr);

		m_Height = height;
		m_Width = width;

		D3D11_TEXTURE2D_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Format = d3d_convert_textureformat(format, D3D11_BIND_DEPTH_STENCIL);
		desc.Height = m_Height;
		desc.Width = m_Width;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* texture = nullptr;
		if (FAILED(m_Context->GetDevice()->CreateTexture2D(&desc, nullptr, &texture)))
		{
			std::cout << "ERROR: Could not create IDepthBuffer\n";
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
		ZeroBlock(&desc2, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc2.Texture2D.MipSlice = 0;
		desc2.Format = desc.Format;
		desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		if (FAILED(m_Context->GetDevice()->CreateDepthStencilView(texture, &desc2, &m_View)))
		{
			std::cout << "ERROR: Could not create DepthBufferView\n";
			D3DRelease_S(texture);

			return false;
		}

		D3DRelease_S(texture);
		return true;
	}

	void D3D11DepthBuffer::Release()
	{
		D3DRelease_S(m_View);
	}
}