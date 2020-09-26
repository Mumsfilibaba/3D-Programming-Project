#pragma once

#include "..\Core\Graphics\API\ITexture2D.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLTexture2D : public ITexture2D
	{
	public:
		GLTexture2D(const GLDeviceContext* const context);
		GLTexture2D(const GLDeviceContext* const context, const void* const data, uint16 width, uint16 height, TEXTUREFORMAT format, bool mips);
		GLTexture2D(const GLDeviceContext* const context, const std::string& filename, const std::string& filepath, TEXTUREFORMAT format, bool mips);
		~GLTexture2D();

		bool Create(const void* const data, uint16 width, uint16 height, TEXTUREFORMAT format, bool mips) override;
		bool Create(const std::string& filename, const std::string& filepath, TEXTUREFORMAT format, bool mips) override;

		void BindVS(uint32 slot = 0) const override;
		void BindPS(uint32 slot = 0) const override;
		void BindHS(uint32 slot = 0) const override;
		void BindDS(uint32 slot = 0) const override;
		void BindGS(uint32 slot = 0) const override;
		void UnbindVS() const override;
		void UnbindPS() const override;
		void UnbindHS() const override;
		void UnbindDS() const override;
		void UnbindGS() const override;

		void SetSamplerSettings(const SamplerSettings& setting) const override;

		inline const SamplerSettings& GetSamplerSettings() const override
		{
			return m_Sampler;
		}

		inline uint16 GetWidth() const override
		{
			return m_Width;
		}

		inline uint16 GetHeight() const override
		{
			return m_Height;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		inline TEXTUREFORMAT GetFormat() const override
		{
			return m_Format;
		}

		inline uint32 GetGLTexture() const
		{
			return m_Texture;
		}

		void Release() override;

	private:
		void Bind(uint8 slot, SHADER shader) const;
		void Unbind(uint8 slot, SHADER shader) const;

	private:
		const GLDeviceContext* m_Context;
		mutable SamplerSettings m_Sampler;
		mutable uint8 m_VSSlot;
		mutable uint8 m_PSSlot;
		mutable uint8 m_HSSlot;
		mutable uint8 m_DSSlot;
		mutable uint8 m_GSSlot;
		uint32 m_Texture;
		uint32 m_MipLevels;
		uint16 m_Width;
		uint16 m_Height;
		TEXTUREFORMAT m_Format;
		std::string m_Filename;
		std::string m_Filepath;
	};
}