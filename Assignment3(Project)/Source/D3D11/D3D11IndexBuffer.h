#pragma once

#include "..\Core\Graphics\API\IIndexBuffer.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11IndexBuffer : public IIndexBuffer
	{
	public:
		D3D11IndexBuffer(const D3D11DeviceContext* const context, const uint32* const indices, 
			uint32 len, uint32 count);
		~D3D11IndexBuffer();

		bool Create(const uint32* const indices, uint32 len, uint32 count) override;
		void Bind() const override;
		void Unbind() const override;

		//TODO: Update()
		//TODO: Resize()
		//TODO: IsBound()
		//TODO: Different usages, static, dynamic, default

		inline uint32 GetCount() const override
		{
			return m_Count;
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
	};
}