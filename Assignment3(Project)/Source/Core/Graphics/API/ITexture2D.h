#pragma once

#include <string>
#include "ITexture.h"

namespace Graphics
{
	class ITexture2D : public ITexture
	{
	public:
		virtual ~ITexture2D() {}

		virtual bool Create(const void* const data, uint16 width, uint16 height, TEXTUREFORMAT format, bool mips) = 0;
		virtual bool Create(const std::string& filename, const std::string& filepath, TEXTUREFORMAT format, bool mips) = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static ITexture2D* Create(const IDeviceContext* const context, const void* const data, uint16 width, uint16 height,
			TEXTUREFORMAT format, bool mips);
		static ITexture2D* Create(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
			TEXTUREFORMAT format, bool mips);
	};
}