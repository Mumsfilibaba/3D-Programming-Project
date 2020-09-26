#include <cassert>
#include "Mesh.h"
#include "API\IVertexBuffer.h"
#include "API\IIndexBuffer.h"

namespace Graphics 
{
	Mesh::Mesh()
		: m_VertexBuffer(nullptr),
		m_IndexBuffer(nullptr)
	{
	}

	Mesh::Mesh(const IDeviceContext* const context, const GeometryData& data)
		: m_VertexBuffer(nullptr),
		m_IndexBuffer(nullptr)
	{
		Create(context, data);
	}

	Mesh::Mesh(const IDeviceContext* const context, const void* const vertices, uint32 vertCount, uint32 stride, 
		const uint32* const indices, uint32 indexCount)
		: m_VertexBuffer(nullptr),
		m_IndexBuffer(nullptr)
	{
		Create(context, vertices, vertCount, stride, indices, indexCount);
	}

	Mesh::~Mesh()
	{
		Release();
	}

	void Mesh::Release()
	{
		ReleaseObject_S(m_VertexBuffer);
		ReleaseObject_S(m_IndexBuffer);
	}

	bool Mesh::Create(const IDeviceContext* const context, const GeometryData& data)
	{
		m_VertexBuffer = IVertexBuffer::Create(context, data.vertices.data(), (uint32)data.vertices.size(), sizeof(Vertex), (uint32)data.vertices.size());

		if (m_VertexBuffer == nullptr)
			return false;

		m_IndexBuffer = IIndexBuffer::Create(context, data.indices.data(), (uint32)data.indices.size(), (uint32)data.indices.size());

		if (m_IndexBuffer == nullptr)
			return false;

		return true;
	}

	bool Mesh::Create(const IDeviceContext* const context, const void* const vertices, uint32 vertCount, uint32 stride,
		const uint32* const indices, uint32 indexCount)
	{
		m_VertexBuffer = IVertexBuffer::Create(context, vertices, vertCount, stride, vertCount);

		if (m_VertexBuffer == nullptr)
			return false;

		m_IndexBuffer = IIndexBuffer::Create(context, indices, indexCount, indexCount);

		if (m_IndexBuffer == nullptr)
			return false;

		return true;
	}

	void Mesh::Bind() const
	{
		assert(m_VertexBuffer != nullptr);
		assert(m_IndexBuffer != nullptr);

		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
	}

	void Mesh::Unbind() const
	{
		assert(m_VertexBuffer != nullptr);
		assert(m_IndexBuffer != nullptr);

		m_IndexBuffer->Unbind();
		m_VertexBuffer->Unbind();
	}

	uint32 Mesh::GetVertexCount() const
	{
		assert(m_VertexBuffer != nullptr);

		return m_VertexBuffer->GetCount();
	}

	uint32 Mesh::GetIndexCount() const
	{
		assert(m_IndexBuffer != nullptr);

		return m_IndexBuffer->GetCount();
	}
}