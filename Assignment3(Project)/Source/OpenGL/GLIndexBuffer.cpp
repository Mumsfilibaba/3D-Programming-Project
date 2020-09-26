#include "GLIndexBuffer.h"

namespace Graphics
{
	GLIndexBuffer::GLIndexBuffer(const GLDeviceContext* const context, const uint32* const indices,
		uint32 len, uint32 count)
		: m_Context(context),
		m_Count(0),
		m_Buffer(0)
	{
		Create(indices, len, count);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		Release();
	}

	bool GLIndexBuffer::Create(const uint32* const indices, uint32 len, uint32 count)
	{
		//Copy data
		m_Count = count;

		//Create buffer
		GL_Call(glGenBuffers(1, &m_Buffer));

		//Fill buffer
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer));
		if (glIsBuffer(m_Buffer) == GL_FALSE)
			return false;

		GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * m_Count, indices, GL_STATIC_DRAW));

		//Unbind
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		return true;
	}

	void GLIndexBuffer::Bind() const
	{
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer));
	}

	void GLIndexBuffer::Unbind() const
	{
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void GLIndexBuffer::Release()
	{
		if (glIsBuffer(m_Buffer))
		{
			GL_Call(glDeleteBuffers(1, &m_Buffer));
		}
	}
}