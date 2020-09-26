#pragma once

#include "..\Core\Graphics\API\IVertexBuffer.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLVertexBuffer : public IVertexBuffer
	{
	public:
		GLVertexBuffer(const GLDeviceContext* const context, const void* const vertices, uint32 len, 
			uint32 stride, uint32 count);
		~GLVertexBuffer();

		bool Create(const void* const vertices, uint32 len, uint32 stride, uint32 count) override;
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
		const GLDeviceContext* m_Context;
		uint32 m_Stride;
		uint32 m_Count;
		uint32 m_Buffer;
	};
}