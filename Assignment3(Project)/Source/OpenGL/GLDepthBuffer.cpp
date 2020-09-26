#include "GLDepthBuffer.h"

namespace Graphics
{
	GLDepthBuffer::GLDepthBuffer(const GLDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format)
		: m_Context(context),
		m_Width(0),
		m_Height(0),
		m_Buffer(0)
	{
		Create(width, height, format);
	}

	GLDepthBuffer::~GLDepthBuffer()
	{
		Release();
	}

	bool GLDepthBuffer::Create(uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		m_Height = height;
		m_Width = width;

		GL_Call(glGenRenderbuffers(1, &m_Buffer));
		GL_Call(glBindRenderbuffer(GL_RENDERBUFFER, m_Buffer));

		uint32 internalFormat = gl_convert_textureinternalformat(format);
		GL_Call(glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_Width, m_Height));
		
		GL_Call(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		if (!glIsRenderbuffer(m_Buffer))
			return false;

		return true;
	}

	void GLDepthBuffer::Release()
	{
		if (glIsRenderbuffer(m_Buffer))
		{
			GL_Call(glDeleteRenderbuffers(1, &m_Buffer));
		}
	}
}