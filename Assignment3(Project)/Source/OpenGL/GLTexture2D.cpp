#include <iostream>
#include "..\Core\Parsing\LoadTexture.h"
#include "GLTexture2D.h"

namespace Graphics
{
	GLTexture2D::GLTexture2D(const GLDeviceContext* const context)
		: m_Context(context),
		m_Sampler(),
		m_Texture(0),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_MipLevels(0),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filename(),
		m_Filepath()
	{
	}

	GLTexture2D::GLTexture2D(const GLDeviceContext* const context, const void* const data, uint16 width,
		uint16 height, TEXTUREFORMAT format, bool mips)
		: m_Context(context),
		m_Sampler(),
		m_Texture(0),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_MipLevels(0),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filename(),
		m_Filepath()
	{
		Create(data, width, height, format, mips);
	}

	GLTexture2D::GLTexture2D(const GLDeviceContext* const context, const std::string& filename, 
		const std::string& filepath, TEXTUREFORMAT format, bool mips)
		: m_Context(context),
		m_Sampler(),
		m_Texture(0),
		m_Width(0),
		m_Height(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_MipLevels(0),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filename(),
		m_Filepath()
	{
		Create(filename, filepath, format, mips);
	}

	GLTexture2D::~GLTexture2D()
	{
		Release();
	}

	bool GLTexture2D::Create(const void* const data, uint16 width, uint16 height, 
		TEXTUREFORMAT format, bool mips)
	{
		//Set size
		m_Width = width;
		m_Height = height;
		
		//Set miplevel count
		m_MipLevels = mips ? available_miplevels(width, height) : 1;

		//Create and bind texture
		GL_Call(glGenTextures(1, &m_Texture));
		GL_Call(glBindTexture(GL_TEXTURE_2D, m_Texture));

		//Set format
		m_Format = format;
		uint32 gltype = gl_convert_texturetype(m_Format);
		uint32 glformat = gl_convert_textureformat(m_Format);
		uint32 glinternalFormat = gl_convert_textureinternalformat(m_Format);

		//Generate miplevels
		void** mipdata = new void*[m_MipLevels];
		mipdata[0] = const_cast<void*>(data);
		uint16 w = m_Width;
		uint16 h = m_Height;

		for (uint32 i = 1; i < m_MipLevels; i++)
		{
			mipdata[i] = nullptr;
			mipdata[i] = generate_miplevel((const byte*)mipdata[i - 1], w, h, w, h, format);
		}
		
		//Fill data
		w = m_Width;
		h = m_Height;
		for (uint32 i = 0; i < m_MipLevels; i++)
		{
			//Set data
			GL_Call(glTexImage2D(GL_TEXTURE_2D, i, glinternalFormat, w, h, 0, glformat, gltype, mipdata[i]));

			w = w / 2;
			h = h / 2;
		}

		//Release created mips
		for (uint32 i = 1; i < m_MipLevels; i++)
		{
			ReleaseBuffer_S(mipdata[i]);
		}
		ReleaseBuffer_S(mipdata);

		//Set samplersettings
		SetSamplerSettings(SamplerSettings());

		GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
		return true;
	}

	bool GLTexture2D::Create(const std::string& filename, const std::string& filepath, 
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

	void GLTexture2D::BindVS(uint32 slot) const
	{	
		m_VSSlot = slot;
		Bind(m_VSSlot, SHADER_VERTEX);
	}

	void GLTexture2D::BindPS(uint32 slot) const
	{
		m_PSSlot = slot;
		Bind(m_PSSlot, SHADER_PIXEL);
	}

	void GLTexture2D::BindHS(uint32 slot) const
	{
		m_HSSlot = slot;
		Bind(m_HSSlot, SHADER_HULL);
	}

	void GLTexture2D::BindDS(uint32 slot) const
	{
		m_DSSlot = slot;
		Bind(m_DSSlot, SHADER_DOMAIN);
	}

	void GLTexture2D::BindGS(uint32 slot) const
	{
		m_GSSlot = slot;
		Bind(m_GSSlot, SHADER_GEOMETRY);
	}

	void GLTexture2D::UnbindVS() const
	{
		Unbind(m_VSSlot, SHADER_VERTEX);
		m_VSSlot = 0;
	}

	void GLTexture2D::UnbindPS() const
	{
		Unbind(m_PSSlot, SHADER_PIXEL);
		m_PSSlot = 0;
	}

	void GLTexture2D::UnbindGS() const
	{
		Unbind(m_GSSlot, SHADER_GEOMETRY);
		m_GSSlot = 0;
	}

	void GLTexture2D::UnbindDS() const
	{
		Unbind(m_DSSlot, SHADER_DOMAIN);
		m_DSSlot = 0;
	}

	void GLTexture2D::UnbindHS() const
	{
		Unbind(m_HSSlot, SHADER_HULL);
		m_HSSlot = 0;
	}

	void GLTexture2D::SetSamplerSettings(const SamplerSettings& setting) const
	{
		gl_set_sampler(m_Texture, GL_TEXTURE_2D, setting, (m_MipLevels > 1));
	}

	void GLTexture2D::Release()
	{
		if (glIsTexture(m_Texture))
		{
			GL_Call(glDeleteTextures(1, &m_Texture));
		}
	}

	void GLTexture2D::Bind(uint8 slot, SHADER shader) const
	{
		GL_Call(glActiveTexture(gl_active_textureslot(slot, shader)));
		GL_Call(glBindTexture(GL_TEXTURE_2D, m_Texture));
	}

	void GLTexture2D::Unbind(uint8 slot, SHADER shader) const
	{
		GL_Call(glActiveTexture(gl_active_textureslot(slot, shader)));
		GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
	}
}