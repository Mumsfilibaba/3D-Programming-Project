#pragma once

#include "..\Core\Graphics\API\IRenderTargetDepth.h"
#include "D3D11Texture2D.h"

namespace Graphics
{
	class D3D11RenderTargetDepth : public IRenderTargetDepth
	{
	public:
		D3D11RenderTargetDepth(const D3D11DeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
		~D3D11RenderTargetDepth();

		bool Create(uint16 width, uint16 height, TEXTUREFORMAT format) override;

		void Release() override;

		inline uint16 GetWidth() const override
		{
			return m_Texture.GetWidth();
		}

		inline uint16 GetHeight() const override
		{
			return m_Texture.GetHeight();
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Texture.GetContext();
		}

		inline const ITexture2D* GetTexture() const override
		{
			return &m_Texture;
		}

		inline ID3D11DepthStencilView* GetDXView() const
		{
			return m_View;
		}

	private:
		ID3D11DepthStencilView* m_View;
		D3D11Texture2D m_Texture;
	};
}