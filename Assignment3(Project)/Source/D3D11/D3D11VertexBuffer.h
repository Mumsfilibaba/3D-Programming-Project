#pragma once

#include "..\Core\Graphics\API\IVertexBuffer.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11VertexBuffer : public IVertexBuffer
	{
	public:
		D3D11VertexBuffer(const D3D11DeviceContext* const context, const void* const vertices,
			uint32 len, uint32 stride, uint32 count);
		~D3D11VertexBuffer();

		//TODO: Update()
		//TODO: Resize()
		//TODO: IsBound()
		//TODO: Different usages, static, dynamic, default

		bool Create(const void* const vertices, uint32 len, uint32 stride, uint32 count) override;
		void Bind() const override;
		void Unbind() const override;

		inline uint32 GetCount() const override
		{
			return m_Count;
		}

		inline uint32 GetStride() const override
		{
			return m_Stride;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		ID3D11Buffer* m_Buffer;
		uint32 m_Count;
		uint32 m_Stride;
	};
}