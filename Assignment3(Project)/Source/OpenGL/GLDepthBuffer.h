#pragma once

#include "..\Core\Graphics\API\IDepthBuffer.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLDepthBuffer : public IDepthBuffer
	{
	public:
		GLDepthBuffer(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
		~GLDepthBuffer();

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

		inline uint32 GetGLBuffer() const
		{
			return m_Buffer;
		}

		void Release() override;

	private:
		const GLDeviceContext* m_Context;
		uint16 m_Width;
		uint16 m_Height;
		uint32 m_Buffer;
	};
}