#pragma once

#include "IResource.h"

namespace Graphics
{
	class IDeviceContext;
	class ITexture2D;

	class IRenderTarget2D : public IResource
	{
	public:
		virtual ~IRenderTarget2D() {}

		//TODO: Different formats
		//TODO: Resize
		virtual bool Create(uint16 width, uint16 height, TEXTUREFORMAT format) = 0;

		virtual uint16 GetWidth() const = 0;
		virtual uint16 GetHeight() const = 0;
		virtual const ITexture2D* GetTexture() const = 0;
		virtual TEXTUREFORMAT GetFormat() const = 0;
	
	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IRenderTarget2D* Create(const IDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format);
	};
}