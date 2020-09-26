#include <cassert>
#include <iostream>
#include "..\Core\Parsing\LoadTexture.h"
#include "D3D11Texture2D.h"

namespace Graphics
{
	D3D11Texture2D::D3D11Texture2D(const D3D11DeviceContext* const context)
		: m_Context(context),
		m_SamplerState(nullptr),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerSettings(),
		m_Width(0),
		m_Height(0),
		m_DXFormat(DXGI_FORMAT_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filename(),
		m_Filepath()
	{
	}

	D3D11Texture2D::D3D11Texture2D(const D3D11DeviceContext* const context, const void* const data, 
		uint16 width, uint16 height, TEXTUREFORMAT format, bool mips)
		: m_Context(context),
		m_SamplerState(nullptr),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerSettings(),
		m_Width(0),
		m_Height(0),
		m_DXFormat(DXGI_FORMAT_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filename(),
		m_Filepath()
	{
		Create(data, width, height, format, mips);
	}

	D3D11Texture2D::D3D11Texture2D(const D3D11DeviceContext* const context, const std::string& filename,
		const std::string& filepath, TEXTUREFORMAT format, bool mips)
		: m_Context(context),
		m_SamplerState(nullptr),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerSettings(),
		m_Width(0),
		m_Height(0),
		m_DXFormat(DXGI_FORMAT_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN)
	{
		Create(filename, filepath, format,mips);
	}

	D3D11Texture2D::~D3D11Texture2D()
	{
		Release();
	}

	void D3D11Texture2D::Release()
	{
		D3DRelease_S(m_SamplerState);
		D3DRelease_S(m_View);
		D3DRelease_S(m_Texture);
	}

	bool D3D11Texture2D::Create(const void* const data, uint16 width, uint16 height, 
		TEXTUREFORMAT format, bool mips)
	{
		assert(m_Context != nullptr);

		return Create(data, width, height, 0, format, mips);
	}

	bool D3D11Texture2D::Create(const void* const data, uint16 width, uint16 height, uint32 flags,
		TEXTUREFORMAT format, bool mips)
	{
		assert(m_Context != nullptr);

		//Set size
		m_Width = width;
		m_Height = height;

		//Set format
		m_Format = format;
		m_DXFormat = d3d_convert_textureformat(m_Format, 0);

		//Setup miplevel data
		D3D11_SUBRESOURCE_DATA* subresourceData = nullptr;
		uint32 miplevels = (mips == true) ? available_miplevels(m_Width, m_Height) : 1;
		if (data != nullptr)
		{
			//Get formatattributes
			uint32 stride = texformat_stride(m_Format);
			uint32 components = texformat_components(m_Format);

			//Generate miplevels
			subresourceData = new D3D11_SUBRESOURCE_DATA[miplevels];
			subresourceData[0].pSysMem = data;
			subresourceData[0].SysMemPitch = m_Width * stride * components;
			uint16 w = m_Width;
			uint16 h = m_Height;
		
			for (uint32 i = 1; i < miplevels; i++)
			{
				subresourceData[i].pSysMem = nullptr;
				subresourceData[i].pSysMem = generate_miplevel((byte*)subresourceData[i - 1].pSysMem, w, h, w, h, m_Format);
				subresourceData[i].SysMemPitch = w * stride * components;
			}
		}

		//Setup texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | flags;
		desc.Format = m_DXFormat;
		desc.Height = m_Height;
		desc.Width = m_Width;
		desc.MipLevels = miplevels;
		desc.SampleDesc.Count = 1;

		//Create texture
		HRESULT hr = m_Context->GetDevice()->CreateTexture2D(&desc, subresourceData, &m_Texture);

		//Release mipdata
		for (uint32 i = 1; i < miplevels; i++)
		{
			ReleaseBuffer_S(subresourceData[i].pSysMem);
		}
		ReleaseBuffer_S(subresourceData);
		
		//Check if creation succeded
		if (FAILED(hr))
		{
			std::cout << "ERROR: Failed to create Texture2D\n";
			return false;
		}

		//Create view
		D3D11_SHADER_RESOURCE_VIEW_DESC desc2;
		ZeroBlock(&desc2, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc2.Format = d3d_convert_textureformat(m_Format, D3D11_BIND_SHADER_RESOURCE);
		desc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc2.Texture2D.MipLevels = miplevels;
		desc2.Texture2D.MostDetailedMip = 0;

		//Create shaderresourceview
		if (FAILED(m_Context->GetDevice()->CreateShaderResourceView(m_Texture, &desc2, &m_View)))
		{
			std::cout << "ERROR: Failed to create Texture-View\n";
			return false;
		}

		//Create samplerstate
		SetSamplerSettings(SamplerSettings());
		return true;
	}

	bool D3D11Texture2D::Create(const std::string& filename, const std::string& filepath, 
		TEXTUREFORMAT format, bool mips)
	{
		//Set filename and path
		m_Filename = filename;
		m_Filepath = filepath;

		//Load image
		uint16 width = 0;
		uint16 height = 0;
		byte* bytes = load_image(m_Filename, m_Filepath, width, height, format);
		if (bytes == nullptr)
		{
			std::cout << "ERROR: Could not load file '" << m_Filepath << '/' << m_Filename << '\n';
			return false;
		}

		//Create
		bool result = Create(bytes, width, height, format, mips);
		
		ReleaseBuffer_S(bytes);
		return result;
	}

	void D3D11Texture2D::BindVS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_VSSlot = slot;
		m_Context->GetContext()->VSSetSamplers(m_VSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->VSSetShaderResources(m_VSSlot, 1, &m_View);
	}

	void D3D11Texture2D::BindPS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_PSSlot = slot;
		m_Context->GetContext()->PSSetSamplers(m_PSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->PSSetShaderResources(m_PSSlot, 1, &m_View);
	}

	void D3D11Texture2D::BindGS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_GSSlot = slot;
		m_Context->GetContext()->GSSetSamplers(m_GSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->GSSetShaderResources(m_GSSlot, 1, &m_View);
	}

	void D3D11Texture2D::BindHS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_HSSlot = slot;
		m_Context->GetContext()->HSSetSamplers(m_HSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->HSSetShaderResources(m_HSSlot, 1, &m_View);
	}

	void D3D11Texture2D::BindDS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_DSSlot = slot;
		m_Context->GetContext()->DSSetSamplers(m_DSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->DSSetShaderResources(m_DSSlot, 1, &m_View);
	}

	void D3D11Texture2D::UnbindVS() const
	{
		assert(m_Context != nullptr);

		//Temporary variables for unbinding
		ID3D11SamplerState* nullSampler = nullptr;
		ID3D11ShaderResourceView* nullView = nullptr;

		//Unbind texture and samplerstate
		m_Context->GetContext()->VSSetSamplers(m_VSSlot, 1, &nullSampler);
		m_Context->GetContext()->VSSetShaderResources(m_VSSlot, 1, &nullView);
		m_VSSlot = 0;
	}

	void D3D11Texture2D::UnbindPS() const
	{
		assert(m_Context != nullptr);

		//Temporary variables for unbinding
		ID3D11SamplerState* nullSampler = nullptr;
		ID3D11ShaderResourceView* nullView = nullptr;

		//Unbind texture and samplerstate
		m_Context->GetContext()->PSSetSamplers(m_PSSlot, 1, &nullSampler);
		m_Context->GetContext()->PSSetShaderResources(m_PSSlot, 1, &nullView);
		m_PSSlot = 0;
	}

	void D3D11Texture2D::UnbindGS() const
	{
		assert(m_Context != nullptr);

		//Temporary variables for unbinding
		ID3D11SamplerState* nullSampler = nullptr;
		ID3D11ShaderResourceView* nullView = nullptr;

		//Unbind texture and samplerstate
		m_Context->GetContext()->GSSetSamplers(m_GSSlot, 1, &nullSampler);
		m_Context->GetContext()->GSSetShaderResources(m_GSSlot, 1, &nullView);
		m_GSSlot = 0;
	}

	void D3D11Texture2D::UnbindHS() const
	{
		assert(m_Context != nullptr);

		//Temporary variables for unbinding
		ID3D11SamplerState* nullSampler = nullptr;
		ID3D11ShaderResourceView* nullView = nullptr;

		//Unbind texture and samplerstate
		m_Context->GetContext()->HSSetSamplers(m_HSSlot, 1, &nullSampler);
		m_Context->GetContext()->HSSetShaderResources(m_HSSlot, 1, &nullView);
		m_HSSlot = 0;
	}

	void D3D11Texture2D::UnbindDS() const
	{
		assert(m_Context != nullptr);

		//Temporary variables for unbinding
		ID3D11SamplerState* nullSampler = nullptr;
		ID3D11ShaderResourceView* nullView = nullptr;

		//Unbind texture and samplerstate
		m_Context->GetContext()->DSSetSamplers(m_DSSlot, 1, &nullSampler);
		m_Context->GetContext()->DSSetShaderResources(m_DSSlot, 1, &nullView);
		m_DSSlot = 0;
	}

	void D3D11Texture2D::SetSamplerSettings(const SamplerSettings& setting) const
	{
		assert(m_Context != nullptr);

		if (m_SamplerSettings == setting && m_SamplerState != nullptr)
		{
			return;
		}

		//Release old samplerstate
		if (m_SamplerState != nullptr)
		{
			D3DRelease(m_SamplerState);
		}

		//set new state
		m_SamplerSettings = setting;

		//Fill desc
		D3D11_SAMPLER_DESC sDesc;
		ZeroBlock(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		d3d_convert_samplersettings(sDesc, m_SamplerSettings);

		//Create samplerstate
		if (FAILED(m_Context->GetDevice()->CreateSamplerState(&sDesc, &m_SamplerState)))
			std::cout << "ERROR: Could create set SamplerState\n";
	}
}