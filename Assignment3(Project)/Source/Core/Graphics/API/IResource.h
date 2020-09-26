#pragma once

#include "..\..\..\Defines.h"

namespace Graphics
{
	class IDeviceContext;

	class IResource
	{
	public:
		virtual ~IResource() {}

		virtual const IDeviceContext* GetContext() const = 0;
		virtual void Release() = 0;

	private:
		virtual IResource* OnApiChange(API newApi) = 0;
	};
}