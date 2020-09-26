#define DISPLACEMENTMAPPING

Texture2D<float> g_DisplacementMap : register(t0);
SamplerState g_DisplacementSampler : register(s0);
Texture2D<float> g_HeightMap : register(t1);
SamplerState g_HeightSampler : register(s1);

struct DS_IN
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct DS_OUT
{
	float4 position : SV_POSITION0;
	float3 worldPosition : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 bitangent : BITANGENT0;
	float3 tangentPos : TANGENTPOS0;
	float3 tangentCamera : TANGENTCAMERA0;
	float2 uv1 : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float2 uv3 : TEXCOORD2;
};

struct PatchFactor
{
	float edgeFactor[3] : SV_TESSFACTOR;
	float insideFactor[1] : SV_INSIDETESSFACTOR;
	float vDistanceSqrd[3] : DISTANCE;
};

cbuffer DSCameraBuffer : register(b0)
{
	float4x4 g_Camera;
	float4x4 g_CameraView;
	float4 g_CameraPosition;	
};

cbuffer DSMaterialBuffer : register(b1)
{
	float g_DisplacementScale;
	float g_MinDistance;
	float g_MaxDistance;
	float g_TerrainUnits;

	float g_HeightScale;
	float g_TerrainUvScale;
	float2 padding;
}

//Sample heightmap
float SampleHeight(float2 uv)
{
	return ((g_HeightMap.SampleLevel(g_HeightSampler, uv, 0) * 2.0f) - 1.0f) * g_HeightScale;
}

//Get normal from heightmap
float3 GetNormal(float2 uv)
{
	float2 textelSize;
	g_HeightMap.GetDimensions(textelSize.x, textelSize.y);
	textelSize = 1.0f / textelSize; 

	float2 uv1 = uv + float2(-textelSize.x, 0.0f);
	float2 uv2 = uv + float2(textelSize.x, 0.0f);
	float2 uv3 = uv + float2(0.0f, textelSize.y);
	float2 uv4 = uv + float2(0.0f, -textelSize.y);

	float height1 = SampleHeight(uv1);
	float height2 = SampleHeight(uv2);
	float height3 = SampleHeight(uv3);
	float height4 = SampleHeight(uv4);

	return float3(height1 - height2, 1.0f, height3 - height4);
}

#ifdef DISPLACEMENTMAPPING
//Calculate miplevel
float GetMiplevel(float distance)
{
	//vars
	float mipCount;
	float2 size;
	//Get number of miplevels
	g_HeightMap.GetDimensions(0, size.x, size.y, mipCount);

	//Make sure miplevel always is atleast 0
	return max(mipCount * distance, 0.0f);
}

float SampleDisplacement(float3 position, float3 normal, float2 uv1, float2 uv2, float2 uv3, float distance)
{
	float3 blend = abs(normal);
	blend /= blend.x + blend.y + blend.z + 0.0001f; 

	//DISPLACEMENT MAPPING
	//Calculate miplevel
	float miplevel = GetMiplevel(distance);
	//Sample height from heightmap
	float mapped1 = g_DisplacementMap.SampleLevel(g_DisplacementSampler, uv1, miplevel);
	float mapped2 = g_DisplacementMap.SampleLevel(g_DisplacementSampler, uv2, miplevel);
	float mapped3 = g_DisplacementMap.SampleLevel(g_DisplacementSampler, uv3, miplevel);
	float mapped = (blend.x * mapped1) + (blend.y * mapped2) + (blend.z * mapped3);

	return ((mapped * 2.0f) - 1.0f) * g_DisplacementScale;
}
#endif

[domain("tri")]
DS_OUT main(PatchFactor input, float3 uvw : SV_DOMAINLOCATION, const OutputPatch<DS_IN, 3> patch)
{
	//Interpolate coordinates
	float3 position = (patch[0].position * uvw.x) + (patch[1].position * uvw.y) + (patch[2].position * uvw.z);
	float2 uv = (patch[0].uv * uvw.x) + (patch[1].uv * uvw.y) + (patch[2].uv * uvw.z);
	float distance = (input.vDistanceSqrd[0] * uvw.x) + (input.vDistanceSqrd[1] * uvw.y) + (input.vDistanceSqrd[2] * uvw.z); 

	//HEIGHTMAPPING
	position.y = SampleHeight(uv);

	//Get normal
	float3 normal = normalize(GetNormal(uv));
	float3 tangent = float3(1.0f, 0.0f, 0.0f);
	//Make tangent othogonal with normal
	tangent = normalize(tangent - (dot(tangent, normal) * normal));
	//Get bitangent
	float3 bitangent = cross(normal, tangent);

	//Set uv to sample material
	float len = g_TerrainUnits / 2.0f;
	float2 uv1 = (float2(position.y, position.z) + len) / g_TerrainUvScale;
	float2 uv2 = (float2(position.x, position.z) + len) / g_TerrainUvScale; 
	float2 uv3 = (float2(position.x, position.y) + len) / g_TerrainUvScale; 

	//DISPLACEMENTMAPPING
#ifdef DISPLACEMENTMAPPING
#ifndef PARALLAXMAPPING
	position += (normal * SampleDisplacement(position, normal, uv1, uv2, uv3, distance));
#endif
#endif

	//Set output
	DS_OUT output;
	output.worldPosition = position;
	output.position = mul(float4(output.worldPosition, 1.0f), g_Camera);
	output.normal = normal;
	output.tangent = tangent;
	output.bitangent = bitangent;
	output.uv1 = uv1;
	output.uv2 = uv2;
	output.uv3 = uv3;

#ifdef PARALLAXMAPPING
	//Transform cameraviewdirection to tangentspace
	float3x3 tbn = transpose(float3x3(output.tangent, output.bitangent, output.normal));
	output.tangentPos = mul(output.worldPosition, tbn);
	output.tangentCamera = mul(g_CameraPosition.xyz, tbn);
#else
	output.tangentPos = float3(0.0f, 0.0f, 0.0f);
	output.tangentCamera = float3(0.0f, 0.0f, 0.0f);
#endif

	return output;
}