#pragma once

#include "..\Core\Graphics\API\IDepthBuffer.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11DepthBuffer : public IDepthBuffer
	{
	public:
		D3D11DepthBuffer(const D3D11DeviceContext* const context, uint16 width, uint16 height,
			TEXTUREFORMAT format);
		~D3D11DepthBuffer();

		bool Create(uint16 width, uint16 height, TEXTUREFORMAT format) override;

		inline uint16 GetWidth() const override
		{
			return m_Width;
		}

		inline uint16 GetHeight() const override
		{
			return m_Height;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		inline ID3D11DepthStencilView* GetDXView() const
		{
			return m_View;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		ID3D11DepthStencilView* m_View;
		uint16 m_Width;
		uint16 m_Height;
	};
}