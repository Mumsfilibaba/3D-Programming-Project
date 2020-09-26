#define PARALLAXMAPPING
#define NORMALMAPPING

Texture2D<float3> g_AlbedoMap : register(t0);
SamplerState g_AlbedoSampler : register(s0);
#ifdef NORMALMAPPING
Texture2D g_NormalMap : register(t1);
SamplerState g_NormalSampler : register(s1);
#endif
Texture2D<float> g_RoughnessMap : register(t2);
SamplerState g_RoughnessSampler : register(s2);
Texture2D<float> g_AoMap : register(t3);
SamplerState g_AoSampler : register(s3);
Texture2D<float> g_MetallicMap : register(t4);
SamplerState g_MetallicSampler : register(s4);
#ifdef PARALLAXMAPPING
Texture2D<float> g_HeightMap : register(t5);
SamplerState g_HeightSampler : register(s5);
#endif

struct PS_IN
{
	float4 position : SV_POSITION0;
	float3 worldPosition : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 bitangent : BITANGENT0;
	float3 tangentPos : TANGENTPOS0;
	float3 tangentCamera : TANGENTCAMERA0;
	float2 uv : TEXCOORD0;
};

struct PS_OUT
{
	float4 albedo : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 position : SV_TARGET2;
	float material : SV_TARGET3;
};

cbuffer PSMaterialBuffer : register(b0)
{
	//MATERIALPROPERTIES
    float4 g_MaterialColor;
	//16 bytes
	float g_Roughness;
	float g_AmbientOcclusion;
	float g_Metallic;
	float g_ParralaxScale;
	//32 bytes
	float g_ParralaxLayersMin;
	float g_ParralaxLayersMax;
	//USE TEXTURES
	float g_UseAlbedoMap;
	float g_UseRoughnessMap;
	//48 bytes
	float g_UseAoMap;
	float g_UseNormalMap;
	float g_UseMetallicMap;
	float g_UseHeightMap;
};

//Gammaconstant
static const float GAMMA = 2.2f;

//Gammacorrect textures (assumes that they are in sRGB space)
//Means that albedos has to be in sRGB
float3 GammaCorrect(float3 color)
{
	return pow(color, GAMMA);
}

#ifdef PARALLAXMAPPING
//Get miplevel
float GetMiplevel(float2 uv, float2 texelSize)
{
	//Get derivetives of uv
	float2 dx = ddx(uv.x * texelSize.x);
	float2 dy = ddy(uv.y * texelSize.y);

	//Get level
	return 0.5f * log2(max(dot(dx, dx), dot(dy, dy))); 
}

//ParallaxMapped UVs
float2 GetParrallaxedUVs(float2 uv, float3 viewDir)
{
	if (g_UseHeightMap < 1.0f)
		return uv;

	//Get texturesize and miplevelcount
	float2 texelSize;
	float miplevels;
	g_HeightMap.GetDimensions(0, texelSize.x, texelSize.y, miplevels);
	//Get miplevel - Should be the same for all samples even tho the uv changes
	float miplevel = GetMiplevel(uv, texelSize);

	//The number of layers based on miplevel
	int layers = max(int(lerp(g_ParralaxLayersMax, g_ParralaxLayersMin, 1.0f - (miplevel / miplevels))), int(g_ParralaxLayersMax));
	//Depth of one layer
	float layerDepth = 1.0f / layers;
	//Current depth
	float currentDepth = 0.0f;
	//Current UVs
	float2 currentUV = uv;
	//UV change per layer
	float2 deltaUV = ((viewDir.xy / viewDir.z) * g_ParralaxScale) / layers;
	//Sample height
	float mappedDepth = (1.0f - g_HeightMap.SampleLevel(g_HeightSampler, currentUV, miplevel));

	//Find correct UVs
	for (int i = 0; i < layers; i++)
	{
		//Check depth
		if (currentDepth > mappedDepth)
			break;
		//Add to UVs
		currentUV -= deltaUV;
		//Sample mapped depth from heightmap (1.0f - because of depth instead of height)
		mappedDepth = (1.0f - g_HeightMap.SampleLevel(g_HeightSampler, currentUV, miplevel));
		//Add to depth
		currentDepth += layerDepth;
	}

	//Interpolate the value before and after collision
	float2 prev = currentUV + deltaUV;

	float before = (1.0f - g_HeightMap.SampleLevel(g_HeightSampler, currentUV, miplevel)) - (currentDepth + layerDepth);	
	float after = mappedDepth - currentDepth;

	float weight = after / (after - before);
	float2 final = (currentUV * weight) + (prev * (1.0f - weight));

	//Discard pixels outside of texture
	if (final.x < 0.0f || final.x > 1.0f)
		discard;

	if (final.y < 0.0f || final.y > 1.0f)
		discard;

	return final;
}
#endif

#ifdef NORMALMAPPING
//Get final normal
float3 GetNormal(float2 uv, float3 inputNormal, float3 inputTangent, float3 inputBitangent)
{
	if (g_UseNormalMap > 0.0f)
	{
		//Sample normal map
		float3 textureNormal = g_NormalMap.Sample(g_NormalSampler, uv).rgb;
		//Put normal in [-1, 1] range
		textureNormal = (textureNormal * 2.0f) - 1.0f;
		float3x3 tbn = float3x3(inputTangent, inputBitangent, inputNormal);
		//Calculate normal in world-space
		textureNormal = normalize(mul(textureNormal, tbn));

		return textureNormal;
	}
	else
	{
		return inputNormal;
	}
}
#endif

//Get albedo
float3 GetAlbedo(float2 uv)
{
	return (GammaCorrect(g_AlbedoMap.Sample(g_AlbedoSampler, uv)) * g_UseAlbedoMap) + (g_MaterialColor.rgb * (1.0f - g_UseAlbedoMap));
}

//Get roughness
float GetRoughness(float2 uv)
{
	return (g_RoughnessMap.Sample(g_RoughnessSampler, uv) * g_UseRoughnessMap) + (g_Roughness * (1.0f - g_UseRoughnessMap));
}

//Get ambient occlusion
float GetAmbientOcclution(float2 uv)
{
	return (g_AoMap.Sample(g_AoSampler, uv) * g_UseAoMap) + (g_AmbientOcclusion * (1.0f - g_UseAoMap));
}

//Get metalness
float GetMetalness(float2 uv)
{
	return (g_MetallicMap.Sample(g_MetallicSampler, uv) * g_UseMetallicMap) + (g_Metallic * (1.0f - g_UseMetallicMap));
}

PS_OUT main(PS_IN input)
{
	PS_OUT output;

	//Get uvs
	float2 uv;
	uv = GetParrallaxedUVs(input.uv, normalize(input.tangentPos - input.tangentCamera));
#ifdef PARALLAXMAPPING
#else
	uv = input.uv;
#endif
	//Output albedo and a mask in albedobuffer
	output.albedo.rgb = GetAlbedo(uv);
	output.albedo.a = 0.0f;
	//Output normal in normalbuffer
#ifdef NORMALMAPPING
	output.normal.rgb = GetNormal(uv, input.normal, input.tangent, input.bitangent);
#else
	output.normal.rgb = input.normal;
#endif
	output.normal.rgb = (output.normal.rgb * 0.5f) + 0.5f;
	//Output roughness into alpha of normalbuffer
	output.normal.a = GetRoughness(uv);	
	//Output position and metalness to position buffer
    output.position.rgb = input.worldPosition;
	output.position.a = GetMetalness(uv);
	//Output ao to materialbuffer
	output.material.r = GetAmbientOcclution(uv);

	return output;
}