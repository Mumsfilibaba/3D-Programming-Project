#pragma once

#include "ITexture.h"

namespace Graphics
{
	class ITextureCube : public ITexture
	{
	public:
		virtual ~ITextureCube() {}

		virtual bool Create(const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format) = 0;
		virtual bool Create(const std::string* const filenames, uint32 len, const std::string& filepath, TEXTURECROSS cross,
			TEXTUREFORMAT format) = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static ITextureCube* Create(const IDeviceContext* const context, const std::string filenames[6],
			const std::string& filepath, TEXTUREFORMAT format);
		static ITextureCube* Create(const IDeviceContext* const context, const std::string* const filenames, uint32 len,
			const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT format);
	};
}