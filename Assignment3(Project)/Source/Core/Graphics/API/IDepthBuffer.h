#pragma once

#include "IResource.h"

namespace Graphics
{
	class IDepthBuffer : public IResource
	{
	public: 
		virtual ~IDepthBuffer() {}

		virtual bool Create(uint16 width, uint16 height, TEXTUREFORMAT format) = 0;

		//TODO: Stencil
		//TODO: Resize

		virtual uint16 GetWidth() const = 0;
		virtual uint16 GetHeight() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IDepthBuffer* Create(const IDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
	};
}