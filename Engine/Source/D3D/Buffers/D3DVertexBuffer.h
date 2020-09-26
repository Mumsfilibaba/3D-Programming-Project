#ifndef D3DVERTEXBUFFER_H
#define D3DVERTEXBUFFER_H

#include "..\..\Core\Graphics\Buffers\VertexBuffer.h"
#include "..\Handles\D3DBufferHandle.h"

namespace Graphics
{
	class D3DContext;

	class D3DVertexBuffer : public VertexBuffer
	{
	public:
		D3DVertexBuffer(const D3DContext& context, void* const data, uint32 arrLen, uint32 count, uint32 stride, USAGE usage);
		~D3DVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void Resize(void* data, uint32 arrLen, uint32 count) override;

		void* Map() override;
		void Unmap() override;
		void Update(void* const data, uint32 count) override;


		inline const D3DBufferHandle& GetBuffer() const
		{
			return m_Buffer;
		}

	private:
		void Initialize(void* const data, uint32 len);

	private:
		const D3DContext& m_Context;
		D3DBufferHandle m_Buffer;
		uint32 m_Stride;
		uint32 m_Count;
		D3D11_USAGE m_Usage;
	};
}
#endif