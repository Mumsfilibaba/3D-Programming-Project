#pragma once

#include "..\Core\Graphics\API\IRenderTargetDepth.h"
#include "GLTexture2D.h"

namespace Graphics
{
	class GLRenderTargetDepth : public IRenderTargetDepth
	{
	public:
		GLRenderTargetDepth(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
		~GLRenderTargetDepth();

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

	private:
		GLTexture2D m_Texture;
	};
}