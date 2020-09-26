#pragma once
#include "IResource.h"

namespace Graphics
{
	class IIndexBuffer : public IResource
	{
	public:
		virtual ~IIndexBuffer() {}

		//TODO: Update()
		//TODO: Resize()
		//TODO: IsBound()
		//TODO: Different usages, static, dynamic, default

		virtual bool Create(const uint32* const indices, uint32 len, uint32 count) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}
	public:
		static IIndexBuffer* Create(const IDeviceContext* const context, const uint32* const indices, 
			uint32 len, uint32 count);
	};
}