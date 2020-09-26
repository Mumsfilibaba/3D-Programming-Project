#include "..\Core\Utilities\Utilities.h"
#include "GLInputLayout.h"

namespace Graphics
{
	GLInputLayout::GLInputLayout(const GLDeviceContext* context, const Attribute* const attributes, uint32 len)
		: m_Context(context), 
		m_Stride(0),
		m_Count(0),
		m_Layout(0)
	{
		Create(attributes, len);
	}

	GLInputLayout::~GLInputLayout()
	{
		Release();
	}

	bool GLInputLayout::Create(const Attribute* const attributes, uint32 len)
	{
		//Check ptr
		if (attributes == nullptr)
			return false;

		//Create attributes
		m_Attributes = new Attribute[len];

		//Create VertexArray
		GL_Call(glCreateVertexArrays(1, &m_Layout));

		//Bind and activate attributes
		GL_Call(glBindVertexArray(m_Layout));
		if (glIsVertexArray(m_Layout) == GL_FALSE)
			return false;

		//Enable inputlayout.attributes
		uint32 components = 0;
		uint32 offset = 0;
		uint32 stride = 0;
		for (uint32 i = 0; i < len; i++)
		{
			//Copy attribute
			m_Attributes[i] = attributes[i];
			
			//Set size
			components = get_attribute_components(m_Attributes[i].type);
			stride = get_attribute_stride(m_Attributes[i].type);
			m_Stride += stride;

			//Set sttributes
			GL_Call(glEnableVertexAttribArray(i));
			GL_Call(glVertexAttribFormat(i, components, GL_FLOAT, GL_FALSE, offset));
			GL_Call(glVertexAttribBinding(i, 0));

			//Calculate offset
			offset += stride;
		}

		//Unbind
		GL_Call(glBindVertexArray(0));

		return true;
	}

	void GLInputLayout::GetAttribute(Attribute & attribute, uint32 index) const
	{
		assert(index < m_Count);

		attribute = m_Attributes[index];
	}

	void GLInputLayout::Release()
	{
		ReleaseBuffer_S(m_Attributes);

		if (glIsVertexArray(m_Layout))
		{
			GL_Call(glDeleteVertexArrays(1, &m_Layout));
			m_Layout = 0;
		}
	}
}