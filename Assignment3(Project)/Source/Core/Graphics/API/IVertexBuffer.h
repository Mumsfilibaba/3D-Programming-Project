#pragma once
#include "IResource.h"

namespace Graphics
{
	class BufferLayout;
	class IDeviceContext;

	class IVertexBuffer : public IResource
	{
	public:
		virtual ~IVertexBuffer() {}

		virtual bool Create(const void* const vertices, uint32 len, uint32 stride, uint32 count) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//TODO: Update()
		//TODO: Resize()
		//TODO: IsBound()
		//TODO: Different usages, static, dynamic, default

		virtual uint32 GetCount() const = 0;
		virtual uint32 GetStride() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IVertexBuffer* Create(const IDeviceContext* const context, const void* const vertices, uint32 len, uint32 stride, uint32 count);
	};
}