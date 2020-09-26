#include <iostream>
#include "Terrain.h"
#include "API\IDeviceContext.h"
#include "API\IPipelineState.h"
#include "API\ITexture2D.h"
#include "API\IUniformBuffer.h"
#include "Material.h"
#include "..\Parsing\LoadTexture.h"


namespace Graphics
{
	Terrain::Terrain()
		: m_Mesh()
	{
	}

	Terrain::Terrain(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
		float units, uint32 resolution, float heightScale, float uvScale, float displacementScale, float minDistance,
		float maxDistance)
		: m_HeightMap(nullptr),
		m_DataBuffer(nullptr),
		m_Mesh(),
		m_Data()
	{
		GenerateGeometry(context, filename, filepath, units, resolution, heightScale, uvScale, displacementScale, minDistance, maxDistance);
	}

	Terrain::~Terrain()
	{
		Release();
	}

	void Terrain::GenerateGeometry(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
		float units, uint32 resolution, float heightScale, float uvScale, float displacementScale, float minDistance,
		float maxDistance)
	{
		using namespace Math;
		using namespace std;

		//Create texture
		m_HeightMap = ITexture2D::Create(context, filename, filepath, TEXTUREFORMAT_R_BYTE, false);
		
		SamplerSettings settings;
		m_HeightMap->SetSamplerSettings(settings);

		//SetData
		m_Data.terrainLength = units;
		m_Data.heightScale = heightScale;
		m_Data.terrainUvScale = uvScale;
		m_Data.minDistance = minDistance;
		m_Data.maxDistance = maxDistance;
		m_Data.displacementScale = displacementScale;

		//Create buffer
		m_DataBuffer = IUniformBuffer::Create(context, &m_Data, 1, sizeof(DataBuffer), 1);

		//Set number of vertices and indices
		uint32 res = (resolution + 1);
		uint32 vertCount = res * res;
		uint32 indexCount = resolution * resolution * 6;

		//Allocate data
		TerrainVertex* vertices = new TerrainVertex[vertCount];
		uint32* indices = new uint32[indexCount];

		//Calculate starting point for both X and Y
		float start = (units / 2.0f);
		float quadSize = units / resolution;
		float quadUvSize = 1.0f / resolution;
		
		//Initialize vertices
		uint32 v = 0;
		float h = 0;
		for (uint32 y = 0; y <= resolution; y++)
		{
			for (uint32 x = 0; x <= resolution; x++)
			{
				v = (y * (resolution + 1)) + x;
				
				//Calculate uv
				vertices[v].uv.x = 0.0f + (quadUvSize * x);
				vertices[v].uv.y = 0.0f + (quadUvSize * y);

				//Calculate position based on uv 
				vertices[v].position.x = -start + (quadSize * x);
				vertices[v].position.y = 0.0f;
				vertices[v].position.z = start - (quadSize * y);
			}
		}

		//Initialize indices
		uint32 index = 0;
		for (uint32 y = 0; y < resolution; y++)
		{
			for (uint32 x = 0; x < resolution; x++)
			{
				index = (y * resolution * 6) + (x * 6);

				//First triangle
				indices[index + 0] = (y * ((resolution) + 1)) + x;
				indices[index + 1] = ((y + 1) * ((resolution) + 1)) + x + 1;
				indices[index + 2] = ((y + 1) * ((resolution) + 1)) + x;

				//Second triangle
				indices[index + 3] = indices[index + 0];
				indices[index + 4] = indices[index + 0] + 1;
				indices[index + 5] = indices[index + 1];
			}
		}

		//Create mesh
		assert(context != nullptr);
		m_Mesh.Create(context, vertices, vertCount, sizeof(TerrainVertex), indices, indexCount);

		//Release data
		ReleaseBuffer_S(vertices);
		ReleaseBuffer_S(indices);
	}

	void Terrain::Render(const IDeviceContext* const context, const MaterialInstance* const material)
	{
		//Bind material
		material->Bind();

		//Bind mesh
		m_Mesh.Bind();

		//Bind heightmap
		m_HeightMap->BindVS(0);
		m_HeightMap->BindDS(1);

		//Bind buffer
		m_DataBuffer->BindVS(0);
		m_DataBuffer->BindHS(1);
		m_DataBuffer->BindDS(1);

		//Draw
		context->DrawIndexed(material->GetPipelineState(), m_Mesh.GetIndexCount(), 0);

		//Unbind heightmap
		m_HeightMap->UnbindVS();
		m_HeightMap->UnbindDS();

		//Bind buffer
		m_DataBuffer->UnbindVS();
		m_DataBuffer->UnbindHS();
		m_DataBuffer->UnbindDS();

		//Unbind mesh
		m_Mesh.Unbind();

		//Unbind pipelinestate
		material->Unbind();
	}

	void Terrain::Release()
	{
		m_Mesh.Release();
		ReleaseObject_S(m_HeightMap);
		ReleaseBuffer_S(m_DataBuffer);
	}
}