#include <cassert>
#include <iostream>
#include "..\Core\Parsing\LoadTexture.h"
#include "D3D11TextureCube.h"

namespace Graphics
{
	D3D11TextureCube::D3D11TextureCube(const D3D11DeviceContext* const context)
		: m_Context(context),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerState(nullptr),
		m_Filenames(nullptr),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_GSSlot(0),
		m_FileCount(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_SamplerSettings(),
		m_Filepath()
	{
	}

	D3D11TextureCube::D3D11TextureCube(const D3D11DeviceContext * const context, const std::string filenames[6], 
		const std::string& filepath, TEXTUREFORMAT format)
		: m_Context(context),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerState(nullptr),
		m_Filenames(nullptr),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_GSSlot(0),
		m_FileCount(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_SamplerSettings(),
		m_Filepath()
	{
		Create(filenames, filepath, format);
	}

	D3D11TextureCube::D3D11TextureCube(const D3D11DeviceContext* const context, const std::string* const filenames, uint32 len, const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT format)
		: m_Context(context),
		m_View(nullptr),
		m_Texture(nullptr),
		m_SamplerState(nullptr),
		m_Filenames(nullptr),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_GSSlot(0),
		m_FileCount(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_SamplerSettings(),
		m_Filepath()
	{
		Create(filenames, len, filepath, cross, format);
	}

	D3D11TextureCube::~D3D11TextureCube()
	{
		Release();
	}

	bool D3D11TextureCube::Create(const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format)
	{
		//Width and height
		uint16 width = 0;
		uint16 height = 0;

		//Set miplevels
		m_MipLevels = 1;
		//Miplevels * 6 is the amount of faces
		void** data = new void*[6];

		//Set filenames and path
		m_Filenames = new std::string[6];
		m_FileCount = 6;
		m_Filepath = filepath;

		//Load files
		for (int i = 0; i < 6; i++)
		{
			m_Filenames[i] = filenames[i];

			data[i] = nullptr;
			data[i] = load_image(m_Filenames[i], m_Filepath, width, height, format);
			if (data[i] == nullptr)
			{
				std::cout << "ERROR: Could not load file '" << m_Filepath << '/' << m_Filenames[i] << '\n';
				return false;
			}
		}

		//Create
		bool result = Create(data, width, height, format);

		//Release Data
		for (int32 i = 0; i < 6; i++)
		{
			ReleaseBuffer_S(data[i]);
		}
		ReleaseBuffer_S(data);

		//Create
		return result;
	}

	bool D3D11TextureCube::Create(const std::string* const filenames, uint32 len, const std::string& filepath,
		TEXTURECROSS cross, TEXTUREFORMAT format)
	{
		//Set miplevels
		m_MipLevels = len;
		//Set filenames and path
		m_FileCount = len;
		m_Filenames = new std::string[m_FileCount];
		m_Filepath = filepath;
		//Width and height
		uint16 width = 0;
		uint16 height = 0;
		uint16 srcWidth = 0;
		uint16 srcHeight = 0;
		//Set crosstype
		m_CrossType = cross;

		//Allocate array for the faces, (Miplevels * 6) is the amount of faces
		void** data = new void*[m_MipLevels * 6];
		//Set all faces to zero
		for (uint32 i = 0; i < (m_MipLevels * 6); i++)
			data[i] = nullptr;
		//Load files
		byte* crossTexture = nullptr;
		byte** iter = (byte**)data;
		for (uint32 i = 0; i < m_FileCount; i++)
		{
			//Set filename
			m_Filenames[i] = filenames[i];
			//Load
			crossTexture = load_image(m_Filenames[i], m_Filepath, srcWidth, srcHeight, format);
			if (crossTexture == nullptr)
			{
				std::cout << "ERROR: Could not load file '" << m_Filepath << '/' << m_Filenames[i] << '\n';
				return false;
			}

			//Extract faces
			cubemap_to_faces(crossTexture, &iter, srcWidth, srcHeight, width, height, m_CrossType, format);

			//Release crosstexture
			ReleaseBuffer_S(crossTexture);

			//Move to next set of faces
			iter += 6;
		}

		//Get the first mips size
		for (int i = 0; i < (int)(m_MipLevels - 1); i++)
		{
			width = width * 2;
			height = height * 2;
		}

		//Create
		bool result = Create(data, width, height, format);

		//Release Data
		for (uint32 i = 0; i < m_MipLevels * 6; i++)
		{
			ReleaseBuffer_S(data[i]);
		}
		ReleaseBuffer_S(data);

		//Create
		return result;
	}

	bool D3D11TextureCube::Create(const void* const * const data, uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		//Set width and height
		m_Width = width;
		m_Height = height;
		//Format
		m_Format = format;

		//Get stride and components
		uint32 components = texformat_components(m_Format);
		uint32 stride = texformat_stride(m_Format);

		//TextureDesc
		D3D11_TEXTURE2D_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.ArraySize = 6;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Format = d3d_convert_textureformat(m_Format, 0);
		desc.Height = m_Height;
		desc.Width = m_Width;
		desc.MipLevels = m_MipLevels;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.SampleDesc.Count = 1;

		//Set data;
		uint32 wi = m_Width;
		uint32 pitch = 0;
		D3D11_SUBRESOURCE_DATA* subresource = new D3D11_SUBRESOURCE_DATA[m_MipLevels * 6];
		for (uint32 i = 0; i < m_MipLevels; i++)
		{
			pitch = wi * stride * components;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_X)].pSysMem = data[(i * 6) + 0];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_X)].SysMemPitch = pitch;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_Y)].pSysMem = data[(i * 6) + 1];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_Y)].SysMemPitch = pitch;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_Z)].pSysMem = data[(i * 6) + 2];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_POSITIVE_Z)].SysMemPitch = pitch;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_X)].pSysMem = data[(i * 6) + 3];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_X)].SysMemPitch = pitch;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_Y)].pSysMem = data[(i * 6) + 4];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_Y)].SysMemPitch = pitch;

			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_Z)].pSysMem = data[(i * 6) + 5];
			subresource[i + (m_MipLevels * D3D11_TEXTURECUBE_FACE_NEGATIVE_Z)].SysMemPitch = pitch;

			wi = wi / 2;
		}

		//Create texturecube
		if (FAILED(m_Context->GetDevice()->CreateTexture2D(&desc, subresource, &m_Texture)))
		{
			ReleaseBuffer_S(subresource);
			std::cout << "ERROR: Could not create TextureCube\n";
			return false;
		}

		//Release subresource data
		ReleaseBuffer_S(subresource);

		//Shader view 
		D3D11_SHADER_RESOURCE_VIEW_DESC desc2;
		ZeroBlock(&desc2, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc2.TextureCube.MipLevels = m_MipLevels;
		desc2.TextureCube.MostDetailedMip = 0;
		desc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc2.Format = desc.Format;

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
	
	void D3D11TextureCube::BindVS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_VSSlot = slot;
		m_Context->GetContext()->VSSetSamplers(m_VSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->VSSetShaderResources(m_VSSlot, 1, &m_View);
	}

	void D3D11TextureCube::BindPS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_PSSlot = slot;
		m_Context->GetContext()->PSSetSamplers(m_PSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->PSSetShaderResources(m_PSSlot, 1, &m_View);
	}

	void D3D11TextureCube::BindGS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_GSSlot = slot;
		m_Context->GetContext()->GSSetSamplers(m_GSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->GSSetShaderResources(m_GSSlot, 1, &m_View);
	}

	void D3D11TextureCube::BindHS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_HSSlot = slot;
		m_Context->GetContext()->HSSetSamplers(m_HSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->HSSetShaderResources(m_HSSlot, 1, &m_View);
	}

	void D3D11TextureCube::BindDS(uint32 slot) const
	{
		assert(m_Context != nullptr);

		//Bind texture and samplerstate
		m_DSSlot = slot;
		m_Context->GetContext()->DSSetSamplers(m_DSSlot, 1, &m_SamplerState);
		m_Context->GetContext()->DSSetShaderResources(m_DSSlot, 1, &m_View);
	}

	void D3D11TextureCube::UnbindVS() const
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

	void D3D11TextureCube::UnbindPS() const
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

	void D3D11TextureCube::UnbindGS() const
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

	void D3D11TextureCube::UnbindHS() const
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

	void D3D11TextureCube::UnbindDS() const
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

	void D3D11TextureCube::SetSamplerSettings(const SamplerSettings& setting) const
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
			std::cout << "ERROR: Could not create SamplerState\n";
	}

	void D3D11TextureCube::Release()
	{
		//Release D3D11 interfaces
		D3DRelease_S(m_View);
		D3DRelease_S(m_Texture);
		D3DRelease_S(m_SamplerState);
		//Release Filenames
		ReleaseBuffer_S(m_Filenames);
	}
}