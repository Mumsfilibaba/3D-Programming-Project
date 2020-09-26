#pragma once

#include "IResource.h"

namespace Graphics
{
#define MAX_UNIFORM_BUFFERS 8

	class IDeviceContext;

	class IUniformBuffer : public IResource
	{
	public:
		virtual ~IUniformBuffer() {}

		virtual bool Create(const void* const data, uint32 len, uint32 stride, uint32 count) = 0;
		virtual bool Update(const void* const data, uint32 len, uint32 startElement) = 0;

		//TODO: Map? (dynamic resources)
		//TODO: Unmap? (dynamic resources)

		virtual void BindVS(uint8 slot = 0) const = 0;
		virtual void BindPS(uint8 slot = 0) const = 0;
		virtual void BindHS(uint8 slot = 0) const = 0;
		virtual void BindDS(uint8 slot = 0) const = 0;
		virtual void BindGS(uint8 slot = 0) const = 0;
		virtual void UnbindVS() const = 0;
		virtual void UnbindPS() const = 0;
		virtual void UnbindHS() const = 0;
		virtual void UnbindDS() const = 0;
		virtual void UnbindGS() const = 0;
		
		virtual uint32 GetCount() const = 0;
		virtual uint32 GetStride() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IUniformBuffer* Create(const IDeviceContext* const context, const void* const data, uint32 len, 
			uint32 stride, uint32 count);
	};
}