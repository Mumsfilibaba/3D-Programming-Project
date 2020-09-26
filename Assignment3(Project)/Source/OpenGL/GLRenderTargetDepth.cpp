#include "GLRenderTargetDepth.h"

namespace Graphics
{
	GLRenderTargetDepth::GLRenderTargetDepth(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format)
		: m_Texture(context)
	{
		Create(width, height, format);
	}
	
	GLRenderTargetDepth::~GLRenderTargetDepth()
	{
		Release();
	}

	bool GLRenderTargetDepth::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		return m_Texture.Create(nullptr, width, height, format, false);
	}

	void GLRenderTargetDepth::Release()
	{
		m_Texture.Release();
	}
}