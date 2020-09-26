#pragma once

#include "ModelData.h"

namespace Graphics
{
	class IDeviceContext;
	class IVertexBuffer;
	class IIndexBuffer;
	class BufferLayout;

	class Mesh
	{
	public:
		Mesh();
		Mesh(const IDeviceContext* const context, const GeometryData& data);
		Mesh(const IDeviceContext* const context, const void* const vertices, uint32 vertCount, uint32 stride,
			const uint32* const indices, uint32 indexCount);
		~Mesh();

		bool Create(const IDeviceContext* const context, const GeometryData& data);
		bool Create(const IDeviceContext* const context, const void* const vertices, uint32 vertCount, uint32 stride, 
			const uint32* const indices, uint32 indexCount);

		void Bind() const;
		void Unbind() const;

		uint32 GetVertexCount() const;
		uint32 GetIndexCount() const;

		void Release();

		inline const IVertexBuffer* GetVertexBuffer() const
		{
			m_VertexBuffer;
		}

		inline const IIndexBuffer* GetIndexBuffer() const
		{
			return m_IndexBuffer;
		}

	private:
		IVertexBuffer* m_VertexBuffer;
		IIndexBuffer* m_IndexBuffer;
	};
}