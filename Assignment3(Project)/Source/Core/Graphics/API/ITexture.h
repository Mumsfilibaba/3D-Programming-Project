#pragma once

#include "IResource.h"

namespace Graphics
{
	class ITexture : public IResource
	{
	public:
		virtual ~ITexture() {}

		virtual void BindVS(uint32 slot = 0) const = 0;
		virtual void BindPS(uint32 slot = 0) const = 0;
		virtual void BindGS(uint32 slot = 0) const = 0;
		virtual void BindDS(uint32 slot = 0) const = 0;
		virtual void BindHS(uint32 slot = 0) const = 0;
		virtual void UnbindVS() const = 0;
		virtual void UnbindPS() const = 0;
		virtual void UnbindGS() const = 0;
		virtual void UnbindDS() const = 0;
		virtual void UnbindHS() const = 0;

		virtual uint16 GetWidth() const = 0;
		virtual uint16 GetHeight() const = 0;
		virtual TEXTUREFORMAT GetFormat() const = 0;
		virtual void SetSamplerSettings(const SamplerSettings& setting) const = 0;
		virtual const SamplerSettings& GetSamplerSettings() const = 0;
	};
}