#include <cassert>
#include "GLVertexBuffer.h"

namespace Graphics
{
	GLVertexBuffer::GLVertexBuffer(const GLDeviceContext* const context, const void* const vertices, 
		uint32 len, uint32 stride, uint32 count)
		: m_Context(context),
		m_Count(0),
		m_Buffer(0)
	{
		Create(vertices, len, stride, count);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		Release();
	}

	bool GLVertexBuffer::Create(const void* const vertices, uint32 len, uint32 stride, uint32 count)
	{
		//Copy variables
		m_Stride = stride;
		m_Count = count;

		//Create buffer
		GL_Call(glGenBuffers(1, &m_Buffer));

		//Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
		if (glIsBuffer(m_Buffer) == GL_FALSE)
			return false;

		GL_Call(glBufferData(GL_ARRAY_BUFFER, m_Stride * m_Count, vertices, GL_STATIC_DRAW));

		//Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}

	void GLVertexBuffer::Bind() const
	{
		GL_Call(glBindVertexBuffer(0, m_Buffer, 0, m_Stride));
	}

	void GLVertexBuffer::Unbind() const
	{
		GL_Call(glBindVertexBuffer(0, 0, 0, 0));
	}

	void GLVertexBuffer::Release()
	{
		//GL-Objects
		if (glIsBuffer(m_Buffer))
		{
			GL_Call(glDeleteBuffers(1, &m_Buffer));
		}
	}
}