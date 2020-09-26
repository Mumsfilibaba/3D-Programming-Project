#pragma once

#include "..\Core\Graphics\API\IUniformBuffer.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11UniformBuffer : public IUniformBuffer
	{
	public:
		D3D11UniformBuffer(const D3D11DeviceContext* const context, const void* const data, uint32 len, 
			uint32 count, uint32 stride);
		~D3D11UniformBuffer();

		bool Create(const void* const data, uint32 len, uint32 stride, uint32 count) override;
		bool Update(const void* const data, uint32 len, uint32 startElement) override;

		//TODO: Map? (dynamic resources)
		//TODO: Unmap? (dynamic resources)

		void BindVS(uint8 slot = 0) const override;
		void BindPS(uint8 slot = 0) const override;
		void BindHS(uint8 slot = 0) const override;
		void BindDS(uint8 slot = 0) const override;
		void BindGS(uint8 slot = 0) const override;
		void UnbindVS() const override;
		void UnbindPS() const override;
		void UnbindHS() const override;
		void UnbindDS() const override;
		void UnbindGS() const override;

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
		//Shaderslots
		mutable byte m_VSSlot;
		mutable byte m_PSSlot;
		mutable byte m_HSSlot;
		mutable byte m_DSSlot;
		mutable byte m_GSSlot;
	};
}