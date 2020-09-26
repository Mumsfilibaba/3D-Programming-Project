#include "D3DVertexBuffer.h"
#include "..\D3DContext.h"


namespace Graphics
{
	D3DVertexBuffer::D3DVertexBuffer(const D3DContext& context, void* const data, uint32 arrLen, uint32 count, uint32 stride, USAGE usage)
		: m_Context(context),
		m_Stride(stride),
		m_Count(count),
		m_Usage(convert_usage(usage))
	{
		Initialize(data, arrLen);
	}

	D3DVertexBuffer::~D3DVertexBuffer()
	{
	}

	void D3DVertexBuffer::Bind() const
	{
		ID3D11Buffer* buffer = m_Buffer.GetBuffer();
		uint32 offset = 0;

		m_Context.GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &m_Stride, &offset);
	}

	void D3DVertexBuffer::Unbind() const
	{
		ID3D11Buffer* buffer = nullptr;
		uint32 offset = 0;

		m_Context.GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &m_Stride, &offset);
	}

	void D3DVertexBuffer::Resize(void * data, uint32 arrLen, uint32 count)
	{
	}

	void * D3DVertexBuffer::Map()
	{
		return nullptr;
	}

	void D3DVertexBuffer::Unmap()
	{
	}

	void D3DVertexBuffer::Update(void* const data, uint32 count)
	{
	}

	void D3DVertexBuffer::Initialize(void* const data, uint32 len)
	{
		m_Buffer.Create(m_Context.GetDevice(), data, m_Count, m_Stride, D3D11_BIND_VERTEX_BUFFER, m_Usage, 0);
	}
}