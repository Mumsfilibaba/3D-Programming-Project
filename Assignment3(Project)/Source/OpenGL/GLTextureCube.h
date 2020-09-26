#pragma once

#include "..\Core\Graphics\API\ITextureCube.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLTextureCube : public ITextureCube
	{
	public:
		GLTextureCube(const GLDeviceContext* const context);
		GLTextureCube(const GLDeviceContext* const context, const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format);
		GLTextureCube(const GLDeviceContext* const context, const std::string* const filenames, uint32 len, 
			const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT format);
		~GLTextureCube();

		bool Create(const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format) override;
		bool Create(const std::string* const filenames, uint32 len, const std::string& filepath, TEXTURECROSS cross,
			TEXTUREFORMAT format) override;

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
		bool Create(const void* const * const faces, uint16 width, uint16 height, TEXTUREFORMAT format);
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
		uint8 m_FileCount;
		uint8 m_MipLevels;
		uint16 m_Width;
		uint16 m_Height;
		TEXTURECROSS m_CrossType;
		TEXTUREFORMAT m_Format;
		std::string* m_Filenames;
		std::string m_Filepath;
	};
}