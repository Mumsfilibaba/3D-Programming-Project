#include "GLRenderTarget2D.h"

namespace Graphics
{
	GLRenderTarget2D::GLRenderTarget2D(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format)
		: m_Texture(context)
	{
		Create(width, height, format);
	}

	GLRenderTarget2D::~GLRenderTarget2D()
	{
		Release();
	}

	bool GLRenderTarget2D::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		return m_Texture.Create(nullptr, width, height, format, false);
	}

	void GLRenderTarget2D::Release()
	{
		m_Texture.Release();
	}
}