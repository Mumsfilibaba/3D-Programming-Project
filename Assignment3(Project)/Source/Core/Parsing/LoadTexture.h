#pragma once

#include "..\..\Defines.h"
#include <string>

byte* load_image(const std::string& filename, const std::string& filepath, uint16& width, uint16& height, TEXTUREFORMAT format);

void cubemap_to_faces(const byte* const srcImage, byte** const * const faces, uint16 srcWidth, uint16 srcHeight, uint16& width, 
	uint16& height, TEXTURECROSS cross, TEXTUREFORMAT format);

byte* generate_miplevel(const byte* const src, uint16 srcWidth, uint16 srcHeight, uint16& width, 
	uint16& height, TEXTUREFORMAT format);
uint32 available_miplevels(uint16 width, uint16 height);

uint32 texformat_stride(TEXTUREFORMAT format);
uint32 texformat_components(TEXTUREFORMAT format);