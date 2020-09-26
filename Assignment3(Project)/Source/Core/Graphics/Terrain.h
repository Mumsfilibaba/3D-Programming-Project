#pragma once

#include "Mesh.h"
#include "..\Math\Point.h"

namespace Graphics
{
	class IPipelineState;
	class ITexture2D;
	class IUniformBuffer;
	class MaterialInstance;

	struct TerrainVertex
	{
	public:
		Math::Vector3 position;
		Math::Vector2 uv;

		inline bool operator==(const TerrainVertex& other) const
		{
			return (uv == other.uv) && (position == other.position);
		}

		inline bool operator!=(const TerrainVertex& other) const
		{
			return !(*this == other);
		}
	};

	class Terrain
	{
	private:
		//Data for shaders
		//TODO: Split up, some of this data should be in the material
		struct DataBuffer
		{
			float displacementScale;
			float minDistance;
			float maxDistance;
			float terrainLength;
			float heightScale;
			float terrainUvScale;
			Math::Vector2 padding;
		};

	public:
		Terrain(const Terrain& other) = delete;
		Terrain(Terrain&& other) = delete;
		Terrain& operator=(const Terrain& other) = delete;
		Terrain& operator=(Terrain&& other) = delete;

		Terrain();
		Terrain(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
			float units, uint32 resolution, float heightScale, float uvScale, float displacementScale, float minDistance,
			float maxDistance);
		~Terrain();

	public:
		void GenerateGeometry(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
			float units, uint32 resolution, float heightScale, float uvScale, float displacementScale, float minDistance,
			float maxDistance);
		void Render(const IDeviceContext* const context, const MaterialInstance* const material);

		void Release();

	private:
		Mesh m_Mesh;
		ITexture2D* m_HeightMap;
		IUniformBuffer* m_DataBuffer;
		DataBuffer m_Data;
	};
}