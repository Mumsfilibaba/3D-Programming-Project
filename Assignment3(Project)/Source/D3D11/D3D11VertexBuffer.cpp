#include <cassert>
#include <iostream>
#include "D3D11VertexBuffer.h"

namespace Graphics
{
	D3D11VertexBuffer::D3D11VertexBuffer(const D3D11DeviceContext* const context, const void* const vertices, 
		uint32 len, uint32 stride, uint32 count)
		: m_Context(context),
		m_Buffer(nullptr),
		m_Count(0)
	{
		Create(vertices, len, stride, count);
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		Release();
	}

	bool D3D11VertexBuffer::Create(const void* const vertices, uint32 len, uint32 stride, uint32 count)
	{
		assert(m_Context != nullptr);

		//Copy variables
		m_Stride = stride;
		m_Count = count;

		//Setup buffer
		D3D11_BUFFER_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_Stride * m_Count;
		desc.StructureByteStride = m_Stride;
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertices;

		if (FAILED(m_Context->GetDevice()->CreateBuffer(&desc, &data, &m_Buffer)))
		{
			std::cout << "FAILED TO CREATE VERTEXBUFFER\n";
			return false;
		}

		return true;
	}

	void D3D11VertexBuffer::Bind() const
	{
		assert(m_Context != nullptr);

		uint32 offset = 0;
		m_Context->GetContext()->IASetVertexBuffers(0, 1, &m_Buffer, &m_Stride, &offset);
	}

	void D3D11VertexBuffer::Unbind() const
	{
		assert(m_Context != nullptr);
		
		uint32 offset = 0;
		uint32 stride = 0;
		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	}

	void D3D11VertexBuffer::Release()
	{
		D3DRelease_S(m_Buffer);
	}
}