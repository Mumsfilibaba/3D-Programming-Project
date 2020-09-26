#include <iostream>
#include <cassert>
#include "GLUniformBuffer.h"

namespace Graphics
{
	GLUniformBuffer::GLUniformBuffer(const GLDeviceContext* const context, const void* const data, uint32 len,
		uint32 stride, uint32 count)
		: m_Context(context),
		m_Count(0),
		m_Stride(0),
		m_Buffer(0),
		m_CurrentSlot(0)
	{
		Create(data, len, stride, count);
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
		Release();
	}

	bool GLUniformBuffer::Create(const void* const data, uint32 len, uint32 stride, uint32 count)
	{
		//Copy variables
		m_Stride = stride;
		m_Count = count;

		//Create buffer
		GL_Call(glGenBuffers(1, &m_Buffer));

		//Fill buffer
		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer));
		if (glIsBuffer(m_Buffer) == GL_FALSE)
			return false;

		GL_Call(glBufferData(GL_UNIFORM_BUFFER, m_Stride * m_Count, data, GL_DYNAMIC_DRAW));

		//Unbind
		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		return true;
	}

	bool GLUniformBuffer::Update(const void* const data, uint32 len, uint32 startElement)
	{
		if (data == nullptr)
			return false;

		if (startElement >= len)
			return false;


		//Copy GPU-memory
		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer));
		GL_Call(glBufferSubData(GL_UNIFORM_BUFFER, m_Stride * startElement, m_Stride * len, data));
		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		return true;
	}

	//Bind
	void GLUniformBuffer::Bind(uint32 slot, uint32 shaderBaseSlot) const
	{
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_CurrentSlot = slot;

		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer));
		GL_Call(glBindBufferBase(GL_UNIFORM_BUFFER, shaderBaseSlot + m_CurrentSlot, m_Buffer));
	}
	
	void GLUniformBuffer::BindVS(uint8 slot) const
	{
		Bind(slot, VS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::BindPS(uint8 slot) const
	{
		Bind(slot, PS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::BindHS(uint8 slot) const
	{
		Bind(slot, HS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::BindDS(uint8 slot) const
	{
		Bind(slot, DS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::BindGS(uint8 slot) const
	{
		Bind(slot, GS_UNIFORM_BUFFER_SLOT_0);
	}

	//Unbind
	void GLUniformBuffer::Unbind(uint32 shaderBaseSlot) const
	{
		GL_Call(glBindBufferBase(GL_UNIFORM_BUFFER, shaderBaseSlot + m_CurrentSlot, 0));
		GL_Call(glBindBuffer(GL_UNIFORM_BUFFER, 0));

		m_CurrentSlot = 0;
	}

	void GLUniformBuffer::UnbindVS() const
	{
		Unbind(VS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::UnbindPS() const
	{
		Unbind(PS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::UnbindHS() const
	{
		Unbind(HS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::UnbindDS() const
	{
		Unbind(DS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::UnbindGS() const
	{
		Unbind(GS_UNIFORM_BUFFER_SLOT_0);
	}

	void GLUniformBuffer::Release()
	{
		if (glIsBuffer(m_Buffer))
		{
			GL_Call(glDeleteBuffers(1, &m_Buffer));
		}
	}
}