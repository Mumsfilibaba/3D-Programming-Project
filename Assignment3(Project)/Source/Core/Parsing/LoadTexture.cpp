#include <iostream>
#include "LoadTexture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\..\Dependencies\stb_image\stb_image.h"

byte* load_image(const std::string& filename, const std::string& filepath, uint16& width, uint16& height, TEXTUREFORMAT format)
{
	//Only load bytes
	if (texformat_stride(format) != sizeof(byte))
	{
		width = 0;
		height = 0;
		return nullptr;
	}

	//Do not flip
	stbi_set_flip_vertically_on_load(false);

	int components = texformat_components(format);
	int comp = 0;
	int wi = 0;
	int he = 0;

	//Load file
	std::string fullpath(filepath + filename);
	byte* bytes = stbi_load(fullpath.c_str(), &wi, &he, &comp, components);
	if (bytes == nullptr)
	{
		width = 0;
		height = 0;
		return nullptr;
	}

	//Set size and return bytes
	width = wi;
	height = he;
	return bytes;
}

void cubemap_to_faces(const byte* const srcImage, byte** const * const faces, uint16 srcWidth, uint16 srcHeight, uint16& width, uint16& height, TEXTURECROSS cross, TEXTUREFORMAT format)
{
	//Return if no crosstype
	if (cross == TEXTURECROSS_UNKNOWN)
		return;

	//Calculate size
	if (cross == TEXTURECROSS_VERTICAL)
	{
		width = srcWidth / 3;
		height = srcHeight / 4;
	}
	else if (cross == TEXTURECROSS_HORIZONTAL)
	{
		return;
	}

	//Get nr of components
	uint32 components = texformat_components(format);
	uint32 rowSize = width * components * sizeof(byte);
	uint32 srcRowSize = srcWidth* components * sizeof(byte);
	//Allocate memory for each face
	for (int i = 0; i < 6; i++)
	{
		(*faces)[i] = (byte*)malloc(rowSize * height);
		memset((*faces)[i], 255, rowSize * height);
	}

	//Copy faces from cross
	int32 passedRows = 0;
	const byte* srcOffset = 0;

	if (cross == TEXTURECROSS_VERTICAL)
	{
		uint32 i = 0;
		byte* tempRow = new byte[rowSize];
		byte* negZ = nullptr;
		for (int y = 0; y < height; y++)
		{
			passedRows = (y * rowSize);
			srcOffset = (srcImage + (y * srcRowSize));

			//PosX
			memcpy((*faces)[0] + passedRows, srcOffset + ((rowSize * 2) + (srcRowSize * height)), rowSize);
			//PosY
			memcpy((*faces)[1] + passedRows, srcOffset + (rowSize), rowSize);
			//PosZ
			memcpy((*faces)[2] + passedRows, srcOffset + (rowSize + (srcRowSize * height)), rowSize);
			//NegX
			memcpy((*faces)[3] + passedRows, srcOffset + (srcRowSize * height), rowSize);
			//NegY
			memcpy((*faces)[4] + passedRows, srcOffset + (rowSize + ((srcRowSize * height) * 2)), rowSize);
			//NegZ
			memcpy(tempRow, srcOffset + (rowSize + ((srcRowSize * height) * 3)), rowSize);

			//Flip X
			negZ = ((*faces)[5] + (rowSize * (height - 1))) - passedRows;
			for (int x = rowSize; x > 0; (x -= components))
			{
				for (i = 0; i < components; i++)
					negZ[(rowSize - x) + i] = tempRow[(x - components) + i];
			}
		}

		ReleaseBuffer_S(tempRow);
	}
}

byte* generate_miplevel(const byte* const src, uint16 srcWidth, uint16 srcHeight, 
	uint16& width, uint16& height, TEXTUREFORMAT format)
{
	if (srcWidth < 2 || srcHeight < 2)
		return nullptr;

	if (src == nullptr)
		return nullptr;

	uint32 components = texformat_components(format);
	if (components < 1)
		return nullptr;
	
	width = srcWidth / 2;
	height = srcHeight / 2;

	byte* data = new byte[width * height * components];
	uint32 srcX = 0;
	uint32 srcY = 0;

	for (uint32 y = 0; y < height; y++)
	{
		for (uint32 x = 0; x < width; x++)
		{
			for (uint32 i = 0; i < components; i++)
			{
				data[(y * width * components) + (x * components) + i] =
					((	src[((srcY + 0) * width * components) + ((srcX + 0) * components) + i] +
						src[((srcY + 0) * width * components) + ((srcX + 1) * components) + i] +
						src[((srcY + 1) * width * components) + ((srcX + 0) * components) + i] +
						src[((srcY + 1) * width * components) + ((srcX + 1) * components) + i]) / 4);
			}

			srcX += 2;
		}

		srcY += 2;
	}

	return data;
}

uint32 available_miplevels(uint16 width, uint16 height)
{
	uint32 result = 1;
	uint16 w = width;
	uint16 h = height;

	while (w > 1 && h > 1)
	{
		w = w / 2;
		h = h / 2;

		result++;
	}

	return result;
}

uint32 texformat_stride(TEXTUREFORMAT format)
{
	switch (format)
	{
	case TEXTUREFORMAT_R_BYTE:
	case TEXTUREFORMAT_RGBA_BYTE: return sizeof(byte);
	case TEXTUREFORMAT_DEPTH32:
	case TEXTUREFORMAT_R_FLOAT32: return sizeof(float);
	case TEXTUREFORMAT_RGBA_FLOAT16: return sizeof(float) / 2;
	default: return 0;
	}
}

uint32 texformat_components(TEXTUREFORMAT format)
{
	switch (format)
	{
	case TEXTUREFORMAT_DEPTH32:
	case TEXTUREFORMAT_R_BYTE:
	case TEXTUREFORMAT_R_FLOAT32: return 1;
	case TEXTUREFORMAT_RGBA_BYTE:
	case TEXTUREFORMAT_RGBA_FLOAT16: return 4;
	default: return 0;
	}
}