#pragma once

#include "..\Core\Graphics\API\ITexture2D.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11Texture2D : public ITexture2D
	{
	public:
		D3D11Texture2D(const D3D11DeviceContext* const context);
		D3D11Texture2D(const D3D11DeviceContext* const context, const void* const data, uint16 width, uint16 height,
			TEXTUREFORMAT format, bool mips);
		D3D11Texture2D(const D3D11DeviceContext* const context, const std::string& filename, const std::string& filepath,
			TEXTUREFORMAT format, bool mips);
		~D3D11Texture2D();

		bool Create(const void* const data, uint16 width, uint16 height, TEXTUREFORMAT format, bool mips) override;
		bool Create(const std::string& filename, const std::string& filepath, TEXTUREFORMAT format, bool mips) override;
		bool Create(const void* const data, uint16 width, uint16 height, uint32 flags, TEXTUREFORMAT format, bool mips);

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

		inline ID3D11Texture2D* GetDXTexturePtr() const
		{
			return m_Texture;
		}

		inline DXGI_FORMAT GetDXFormat() const
		{
			return m_DXFormat;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		mutable ID3D11SamplerState* m_SamplerState;
		ID3D11ShaderResourceView* m_View;
		ID3D11Texture2D* m_Texture;

		DXGI_FORMAT m_DXFormat;
		mutable SamplerSettings m_SamplerSettings;
		uint16 m_Width;
		uint16 m_Height;
		TEXTUREFORMAT m_Format;
		mutable uint32 m_VSSlot;
		mutable uint32 m_PSSlot;
		mutable uint32 m_GSSlot;
		mutable uint32 m_HSSlot;
		mutable uint32 m_DSSlot;
		std::string m_Filename;
		std::string m_Filepath;
	};
}