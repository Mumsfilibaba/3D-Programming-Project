#pragma once

#include <vector>
#include "..\..\Defines.h"
#include "..\Math\Vector2.h"
#include "..\Math\Vector3.h"
#include "ColorF.h"

struct Vertex
{
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 uv;

	inline bool operator==(const Vertex& other) const
	{
		return (position == other.position) && (normal == other.normal) && (tangent == other.tangent) && (uv == other.uv);
	}

	inline bool operator!=(const Vertex& other) const
	{
		return !(*this == other);
	}
};

struct GeometryData
{
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
};

struct MaterialData
{
	std::string name;
	ColorF diffuse;
	ColorF ambient;
	ColorF specular;
};

struct MeshData
{
	GeometryData mesh;
	MaterialData material;
};