#pragma once
#include "..\Core\Graphics\API\IRenderTarget2D.h"
#include "GLTexture2D.h"

namespace Graphics
{
	class GLRenderTarget2D : public IRenderTarget2D
	{
	public:
		GLRenderTarget2D(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
		~GLRenderTarget2D();

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

		void Release() override;

	private:
		GLTexture2D m_Texture;
	};
}