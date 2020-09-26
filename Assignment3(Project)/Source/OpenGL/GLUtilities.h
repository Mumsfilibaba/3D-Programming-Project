#pragma once

#include "..\Defines.h"
#define GLEW_STATIC
#include <GL\glew.h>

void clear_gl_errors();
void print_gl_errors(const char* str);

uint32 gl_convert_texturetype(TEXTUREFORMAT format);
uint32 gl_convert_textureformat(TEXTUREFORMAT format);
uint32 gl_convert_textureinternalformat(TEXTUREFORMAT format);

uint32 gl_convert_filter(FILTER filter, bool mips);
uint32 gl_convert_clamp(CLAMP clamp);

uint32 gl_active_textureslot(uint32 slot, SHADER shaderType);

void gl_set_sampler(uint32 texture, uint32 textureType, const Graphics::SamplerSettings& setting, bool mips);

#ifdef DEBUG
#define GL_Call(function) clear_gl_errors(); function; print_gl_errors(#function)
#else
#define GL_Call(function) function
#endif

//To make it more like D3D11
#define VS_TEXTURE_SLOT_0 0
#define GS_TEXTURE_SLOT_0 MAX_TEXTURES
#define PS_TEXTURE_SLOT_0 (MAX_TEXTURES * 2)
#define HS_TEXTURE_SLOT_0 (MAX_TEXTURES * 3)
#define DS_TEXTURE_SLOT_0 (MAX_TEXTURES * 4)

//To make it more like D3D11
#define VS_UNIFORM_BUFFER_SLOT_0 0
#define GS_UNIFORM_BUFFER_SLOT_0 MAX_UNIFORM_BUFFERS
#define PS_UNIFORM_BUFFER_SLOT_0 (MAX_UNIFORM_BUFFERS * 2)
#define HS_UNIFORM_BUFFER_SLOT_0 (MAX_UNIFORM_BUFFERS * 3)
#define DS_UNIFORM_BUFFER_SLOT_0 (MAX_UNIFORM_BUFFERS * 4)