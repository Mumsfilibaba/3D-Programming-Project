#pragma once

#include "..\Core\Graphics\API\ITextureCube.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11TextureCube : public ITextureCube
	{
	public:
		D3D11TextureCube(const D3D11DeviceContext* const context);
		D3D11TextureCube(const D3D11DeviceContext* const context, const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format);
		D3D11TextureCube(const D3D11DeviceContext* const context, const std::string* const filenames, uint32 len, const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT forma);
		~D3D11TextureCube();

		bool Create(const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format) override;
		bool Create(const std::string* const filenames, uint32 len, const std::string& filepath, TEXTURECROSS cross,
			TEXTUREFORMAT format) override;

		void BindVS(uint32 slot = 0) const override;
		void BindPS(uint32 slot = 0) const override;
		void BindGS(uint32 slot = 0) const override;
		void BindHS(uint32 slot = 0) const override;
		void BindDS(uint32 slot = 0) const override;
		void UnbindVS() const override;
		void UnbindPS() const override;
		void UnbindGS() const override;
		void UnbindHS() const override;
		void UnbindDS() const override;


		void SetSamplerSettings(const SamplerSettings& setting) const override;

		inline uint16 GetWidth() const override
		{
			return m_Width;
		}

		inline uint16 GetHeight() const override
		{
			return m_Height;
		}

		inline TEXTUREFORMAT GetFormat() const override
		{
			return m_Format;
		}

		inline const SamplerSettings& GetSamplerSettings() const override
		{
			return m_SamplerSettings;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		void Release() override;

	private:
		bool Create(const void* const * const data, uint16 width, uint16 height, TEXTUREFORMAT format);

	private:
		//Pointers
		const D3D11DeviceContext* m_Context;
		ID3D11ShaderResourceView* m_View;
		ID3D11Texture2D* m_Texture;
		mutable ID3D11SamplerState* m_SamplerState;
		//Files
		std::string* m_Filenames;
		std::string m_Filepath;
		uint32 m_FileCount;
		//Settings
		mutable SamplerSettings m_SamplerSettings;
		uint16 m_Width;
		uint16 m_Height;
		uint32 m_MipLevels;
		TEXTUREFORMAT m_Format;
		TEXTURECROSS m_CrossType;
		mutable uint32 m_VSSlot;
		mutable uint32 m_PSSlot;
		mutable uint32 m_GSSlot;
		mutable uint32 m_HSSlot;
		mutable uint32 m_DSSlot;
	};
}