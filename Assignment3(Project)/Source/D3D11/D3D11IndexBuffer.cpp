#include <iostream>
#include <cassert>
#include "D3D11IndexBuffer.h"

namespace Graphics
{
	D3D11IndexBuffer::D3D11IndexBuffer(const D3D11DeviceContext* const context, const uint32* const indices,
		uint32 len, uint32 count)
		: m_Context(context),
		m_Buffer(nullptr),
		m_Count(0)
	{
		Create(indices, len, count);
	}

	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		Release();
	}

	bool D3D11IndexBuffer::Create(const uint32* const indices, uint32 len, uint32 count)
	{
		assert(m_Context != nullptr);

		//Copy data
		m_Count = count;

		D3D11_BUFFER_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint32) * m_Count;
		desc.StructureByteStride = sizeof(uint32);
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices;

		if (FAILED(m_Context->GetDevice()->CreateBuffer(&desc, &data, &m_Buffer)))
		{
			std::cout << "ERROR: Failed to create Indexbuffer\n";
			return false;
		}

		return true;
	}

	void D3D11IndexBuffer::Bind() const
	{
		assert(m_Context != nullptr);

		m_Context->GetContext()->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11IndexBuffer::Unbind() const
	{
		assert(m_Context != nullptr);

		m_Context->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
	
	void D3D11IndexBuffer::Release()
	{
		D3DRelease_S(m_Buffer);
	}
}