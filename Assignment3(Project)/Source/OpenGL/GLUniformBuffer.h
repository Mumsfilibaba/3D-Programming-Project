#pragma once

#include "..\Core\Graphics\API\IUniformBuffer.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLUniformBuffer: public IUniformBuffer
	{
	public:
		GLUniformBuffer(const GLDeviceContext* const context, const void* const data, uint32 len,
			uint32 stride, uint32 count);
		~GLUniformBuffer();

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
		void Bind(uint32 slot, uint32 shaderBaseSlot) const;
		void Unbind(uint32 shaderBaseSlot) const;

	private:
		const GLDeviceContext* m_Context;
		uint32 m_Buffer;
		uint32 m_Stride;
		uint32 m_Count;
		mutable uint32 m_CurrentSlot;
	};
}