#pragma once

#include "IResource.h"

namespace Graphics
{
	struct Attribute
	{
		ATTRIBUTETYPE type;
		std::string semantic;
		byte index;
	};

	class IInputLayout : public IResource
	{
	public:
		virtual ~IInputLayout() {}

		virtual bool Create(const Attribute* const attributes, uint32 len) = 0;
		
		virtual void GetAttribute(Attribute& attribute, uint32 index) const = 0;
		virtual uint32 GetStride() const = 0;
		virtual uint32 GetCount() const = 0;

		virtual void Release() = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IInputLayout* Create(const IDeviceContext* const context, const Attribute* const attributes, uint32 len);
	};
}