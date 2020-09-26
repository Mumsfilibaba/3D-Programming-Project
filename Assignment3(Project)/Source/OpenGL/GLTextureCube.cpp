#include <iostream>
#include "..\Core\Parsing\LoadTexture.h"
#include "GLTextureCube.h"

namespace Graphics
{
	GLTextureCube::GLTextureCube(const GLDeviceContext* const context)
		: m_Context(context),
		m_Filenames(nullptr),
		m_Sampler(),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_Texture(0),
		m_FileCount(0),
		m_Width(0),
		m_Height(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filepath()
	{
	}

	GLTextureCube::GLTextureCube(const GLDeviceContext* const context, const std::string filenames[6], 
		const std::string& filepath, TEXTUREFORMAT format)
		: m_Context(context),
		m_Filenames(nullptr),
		m_Sampler(),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_Texture(0),
		m_FileCount(0),
		m_Width(0),
		m_Height(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filepath()
	{
		Create(filenames, filepath, format);
	}

	GLTextureCube::GLTextureCube(const GLDeviceContext* const context, const std::string* const filenames, 
		uint32 len, const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT format)
		: m_Context(context),
		m_Filenames(nullptr),
		m_Sampler(),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0),
		m_Texture(0),
		m_FileCount(0),
		m_Width(0),
		m_Height(0),
		m_MipLevels(0),
		m_CrossType(TEXTURECROSS_UNKNOWN),
		m_Format(TEXTUREFORMAT_UNKNOWN),
		m_Filepath()
	{
		Create(filenames, len, filepath, cross, format);
	}

	GLTextureCube::~GLTextureCube()
	{
		Release();
	}

	bool GLTextureCube::Create(const std::string filenames[6], const std::string& filepath, TEXTUREFORMAT format)
	{
		//Width and height
		uint16 width = 0;
		uint16 height = 0;

		//Set miplevels
		m_MipLevels = 1;
		//Miplevels * 6 is the amount of faces
		void** faces = new void*[6];

		//Set filenames and path
		m_Filenames = new std::string[6];
		m_FileCount = 6;
		m_Filepath = filepath;

		//Load files
		for (int32 i = 0; i < 6; i++)
		{
			m_Filenames[i] = filenames[i];

			faces[i] = nullptr;
			faces[i] = load_image(m_Filenames[i], m_Filepath, width, height, format);
			if (faces[i] == nullptr)
			{
				std::cout << "ERROR: Could not load file '" << m_Filepath << '/' << m_Filenames[i] << '\n';
				return false;
			}
		}

		//Create
		bool result = Create(faces, width, height, format);

		//Release Data
		for (uint32 i = 0; i < 6; i++)
		{
			ReleaseBuffer_S(faces[i]);
		}
		ReleaseBuffer_S(faces);

		return result;
	}

	bool GLTextureCube::Create(const std::string* const filenames, uint32 len, const std::string& filepath, 
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
		void** faces = new void*[m_MipLevels * 6u];
		//Set all faces to zero
		for (uint32 i = 0; i < (m_MipLevels * 6u); i++)
			faces[i] = nullptr;
		//Load files
		byte* crossTexture = nullptr;
		byte** iter = (byte**)faces;
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
		for (int32 i = 0; i < (int32)(m_MipLevels - 1); i++)
		{
			width = width * 2;
			height = height * 2;
		}

		//Create
		bool result = Create(faces, width, height, format);

		//Release Data
		for (uint32 i = 0; i < (m_MipLevels * 6u); i++)
		{
			ReleaseBuffer_S(faces[i]);
		}
		ReleaseBuffer_S(faces);

		return result;
	}

	bool GLTextureCube::Create(const void* const * const faces, uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		//Set size
		m_Width = width;
		m_Height = height;

		//Create and bind texture
		GL_Call(glGenTextures(1, &m_Texture));
		GL_Call(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture));

		//Set format
		m_Format = format;
		uint32 gltype = gl_convert_texturetype(m_Format);
		uint32 glformat = gl_convert_textureformat(m_Format);
		uint32 glinternalFormat = gl_convert_textureinternalformat(m_Format);

		//To store the new size for the different mips
		uint16 wi = m_Width;
		uint16 he = m_Height;

		//Set mipelevel-count  in OpenGL
		GL_Call(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0));
		GL_Call(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, m_MipLevels - 1));

		//Set data
		for (int32 i = 0; i < (int32)m_MipLevels; i++)
		{
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 0]));
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 1]));
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 2]));
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 3]));
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 4]));
			GL_Call(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, glinternalFormat, wi, he, 0, glformat, gltype, faces[(i * 6) + 5]));

			wi = wi / 2;
			he = he / 2;
		}


		//Set samplersettings
		SetSamplerSettings(SamplerSettings());

		GL_Call(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		return true;
	}

	void GLTextureCube::Bind(uint8 slot, SHADER shader) const
	{
		GL_Call(glActiveTexture(gl_active_textureslot(slot, shader)));
		GL_Call(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture));
	}

	void GLTextureCube::Unbind(uint8 slot, SHADER shader) const
	{
		GL_Call(glActiveTexture(gl_active_textureslot(slot, shader)));
		GL_Call(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	void GLTextureCube::BindVS(uint32 slot) const
	{
		m_VSSlot = slot;
		Bind(m_VSSlot, SHADER_VERTEX);
	}

	void GLTextureCube::BindPS(uint32 slot) const
	{
		m_PSSlot = slot;
		Bind(m_PSSlot, SHADER_PIXEL);
	}

	void GLTextureCube::BindHS(uint32 slot) const
	{
		m_HSSlot = slot;
		Bind(m_HSSlot, SHADER_HULL);
	}

	void GLTextureCube::BindDS(uint32 slot) const
	{
		m_DSSlot = slot;
		Bind(m_DSSlot, SHADER_DOMAIN);
	}

	void GLTextureCube::BindGS(uint32 slot) const
	{
		m_GSSlot = slot;
		Bind(m_GSSlot, SHADER_GEOMETRY);
	}

	void GLTextureCube::UnbindVS() const
	{
		Unbind(m_VSSlot, SHADER_VERTEX);
		m_VSSlot = 0;
	}

	void GLTextureCube::UnbindPS() const
	{
		Unbind(m_PSSlot, SHADER_PIXEL);
		m_PSSlot = 0;
	}

	void GLTextureCube::UnbindGS() const
	{
		Unbind(m_GSSlot, SHADER_GEOMETRY);
		m_GSSlot = 0;
	}

	void GLTextureCube::UnbindDS() const
	{
		Unbind(m_DSSlot, SHADER_DOMAIN);
		m_DSSlot = 0;
	}

	void GLTextureCube::UnbindHS() const
	{
		Unbind(m_HSSlot, SHADER_HULL);
		m_HSSlot = 0;
	}

	void GLTextureCube::SetSamplerSettings(const SamplerSettings& setting) const
	{
		GL_Call(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
		gl_set_sampler(m_Texture, GL_TEXTURE_CUBE_MAP, setting, (m_MipLevels > 1));
	}

	void GLTextureCube::Release()
	{
		//Release GLTexture
		if (glIsTexture(m_Texture))
		{
			GL_Call(glDeleteTextures(1, &m_Texture));
		}
		//Release filenames
		ReleaseBuffer_S(m_Filenames);
	}
}