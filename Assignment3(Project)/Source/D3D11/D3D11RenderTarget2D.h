#pragma once

#include "..\Core\Graphics\API\IRenderTarget2D.h"
#include "D3D11Texture2D.h"

namespace Graphics
{
	class D3D11RenderTarget2D : public IRenderTarget2D
	{
	public:
		D3D11RenderTarget2D(const D3D11DeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
		~D3D11RenderTarget2D();

		//TODO: Different formats
		//TODO: Resize
		bool Create(uint16 width, uint16 height, TEXTUREFORMAT format) override;

		const ITexture2D* GetTexture() const override
		{
			return &m_Texture;
		}

		inline uint16 GetWidth() const override
		{
			return m_Texture.GetWidth();
		}

		inline uint16 GetHeight() const override
		{
			return m_Texture.GetHeight();
		}

		inline TEXTUREFORMAT GetFormat() const override
		{
			return m_Texture.GetFormat();
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Texture.GetContext();
		}

		inline ID3D11RenderTargetView* GetDXView() const
		{
			return m_View;
		}

		void Release() override;

	private:
		ID3D11RenderTargetView* m_View;
		D3D11Texture2D m_Texture;
	};
}