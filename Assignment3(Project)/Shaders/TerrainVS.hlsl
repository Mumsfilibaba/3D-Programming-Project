struct VS_IN
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

Texture2D<float> g_HeightMap : register(t0);
SamplerState g_HeightSampler : register(s0);

cbuffer MaterialBuffer : register(b0)
{
	float g_DisplacementScale;
	float g_MinDistance;
	float g_MaxDistance;
	float g_TerrainUnits;

	float g_HeightScale;
	float g_TerrainUvScale;
	float2 padding;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	//Set Position
	output.position = input.position;
	//Set uv
	output.uv = input.uv;

	//Set height
	output.position.y = ((g_HeightMap.SampleLevel(g_HeightSampler, output.uv, 0) * 2.0f) - 1.0f) * g_HeightScale;

	return output;
}