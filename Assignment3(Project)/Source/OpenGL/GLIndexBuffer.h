#pragma once
#include "..\Core\Graphics\API\IIndexBuffer.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLIndexBuffer : public IIndexBuffer
	{
	public:
		GLIndexBuffer(const GLDeviceContext* const context, const uint32* const indices, uint32 len, uint32 count);
		~GLIndexBuffer();

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
		const GLDeviceContext* m_Context;
		uint32 m_Buffer;
		uint32 m_Count;
	};
}