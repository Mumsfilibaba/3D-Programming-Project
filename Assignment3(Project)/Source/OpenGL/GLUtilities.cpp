#include <iostream>
#include "GLUtilities.h"

void clear_gl_errors()
{
	while (glGetError() != GL_NO_ERROR);
}

void print_gl_errors(const char* str)
{
	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		const GLubyte* string = glewGetErrorString(err);
		std::cout << "ERROR WHEN CALLING '" << str << "': " << string << "\nCode: " << err << '\n';
	}
}

uint32 gl_convert_texturetype(TEXTUREFORMAT format)
{
	switch (format)
	{
	case TEXTUREFORMAT_R_BYTE:
	case TEXTUREFORMAT_RGBA_BYTE: return GL_UNSIGNED_BYTE;
	case TEXTUREFORMAT_DEPTH16:
	case TEXTUREFORMAT_DEPTH32:
	case TEXTUREFORMAT_R_FLOAT32:
	case TEXTUREFORMAT_RGBA_FLOAT16: return GL_FLOAT;
	default: return 0;
	}
}

uint32 gl_convert_textureformat(TEXTUREFORMAT format)
{
	switch (format)
	{
	case TEXTUREFORMAT_RGBA_BYTE:
	case TEXTUREFORMAT_RGBA_FLOAT16: return GL_RGBA;
	case TEXTUREFORMAT_R_BYTE:
	case TEXTUREFORMAT_R_FLOAT32: return GL_RED;
	case TEXTUREFORMAT_DEPTH16: return GL_DEPTH_COMPONENT;
	case TEXTUREFORMAT_DEPTH32: return GL_DEPTH_COMPONENT;
	default: return 0;
	}
}

uint32 gl_convert_textureinternalformat(TEXTUREFORMAT format)
{
	switch (format)
	{
	case TEXTUREFORMAT_R_BYTE: return GL_R8;
	case TEXTUREFORMAT_RGBA_BYTE: return GL_RGBA8;
	case TEXTUREFORMAT_DEPTH16: return GL_DEPTH_COMPONENT16;
	case TEXTUREFORMAT_DEPTH32: return GL_DEPTH_COMPONENT32;
	case TEXTUREFORMAT_R_FLOAT32: return GL_R32F;
	case TEXTUREFORMAT_RGBA_FLOAT16: return GL_RGBA16F;
	default: return 0;
	}
}

uint32 gl_convert_filter(FILTER filter, bool mips)
{
	switch (filter)
	{
	case FILTER_LINEAR: return mips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	case FILTER_POINT: return mips ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
	default: return 0;
	}
}

uint32 gl_convert_clamp(CLAMP clamp)
{
	switch (clamp)
	{
	case CLAMP_BORDER: return GL_CLAMP_TO_BORDER;
	case CLAMP_EDGE: return GL_CLAMP_TO_EDGE;
	case CLAMP_REPEAT: return GL_REPEAT;
	default: return 0;
	}
}

uint32 gl_active_textureslot(uint32 slot, SHADER shaderType)
{
	switch (shaderType)
	{
	case SHADER_PIXEL: 
		if (slot > (MAX_TEXTURES_PS - 1))
			return 0;

		return GL_TEXTURE0 + slot;
	case SHADER_DOMAIN: 
		if (slot > (MAX_TEXTURES_DS - 1))
			return 0;

		return GL_TEXTURE12 + slot;
	case SHADER_HULL: 
		if (slot > (MAX_TEXTURES_HS - 1))
			return 0;

		return GL_TEXTURE16 + slot;
	case SHADER_VERTEX: 
		if (slot > (MAX_TEXTURES_VS - 1))
			return 0;

		return GL_TEXTURE20 + slot;
	case SHADER_GEOMETRY: 
		if (slot > (MAX_TEXTURES_GS - 1))
			return 0;

		return GL_TEXTURE24 + slot;
	}

	return 0;
}

void gl_set_sampler(uint32 texture, uint32 textureType, const Graphics::SamplerSettings& setting, bool mips)
{
	GL_Call(glBindTexture(textureType, texture));

	int32 filter = gl_convert_filter(setting.filter, mips);
	int32 clamp = gl_convert_clamp(setting.clamp);

	//TODO: Better filter options
	GL_Call(glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filter));
	GL_Call(glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter));
	
	//TODO: Maybe remove this quickfix
	/*if (setting.filter == FILTER_LINEAR)
	{
		GL_Call(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	}*/

	//TODO: Better clamp options
	//U
	GL_Call(glTexParameteri(textureType, GL_TEXTURE_WRAP_S, clamp));
	//V
	GL_Call(glTexParameteri(textureType, GL_TEXTURE_WRAP_T, clamp));
	//W
	GL_Call(glTexParameteri(textureType, GL_TEXTURE_WRAP_R, clamp));
	//Border color
	float color[4] =
	{
		setting.borderColor.r,
		setting.borderColor.g,
		setting.borderColor.b,
		1.0f
	};
	GL_Call(glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, color));

	GL_Call(glBindTexture(textureType, 0));
}