#define PARALLAXMAPPING
#define NORMALMAPPING
#define GAMMA 2.2f

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
	float2 uv1 : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float2 uv3 : TEXCOORD2;
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

//Gammacorrect textures (assumes that they are in sRGB space)
//Means that albedos has to be in sRGB
float3 GammaCorrect(float3 color)
{
	return pow(abs(color), GAMMA);
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


	float mappedDepth = 0.0f;
	//The number of layers based on miplevel
	float layers = lerp(g_ParralaxLayersMax, g_ParralaxLayersMin, miplevel / miplevels);
	//Depth of one layer
	float layerDepth = 1.0f / layers;
	//Current depth
	float currentDepth = 0.0f;
	//Current UVs
	float2 currentUV = uv;
	//UV change per layer
	float2 deltaUV = ((viewDir.xy / viewDir.z) * g_ParralaxScale) / layers;

	//Find correct UVs
	for (int i = 0; i < layers; i++)
	{
		//Sample mapped depth from heightmap (1.0f - because of depth instead of height)
		mappedDepth = (1.0f - g_HeightMap.SampleLevel(g_HeightSampler, currentUV, miplevel));
		//Check depth
		if (currentDepth > mappedDepth)
			break;
		//Add to UVs
		currentUV -= deltaUV;
		//Add to depth
		currentDepth += layerDepth;
	}

	if (currentUV.x < 0.0f || currentUV.x > 1.0f)
		discard;

	if (currentUV.y < 0.0f || currentUV.y > 1.0f)
		discard;

	return currentUV;
}
#endif

#ifdef NORMALMAPPING
//Get final normal
float3 GetNormal(float3 blend, float2 uv1, float2 uv2, float2 uv3, float3 inputNormal, float3 inputTangent, float3 inputBitangent)
{
	if (g_UseNormalMap > 0.0f)
	{
		//Sample normal map
		float3 finalNormal  = (blend.x * g_NormalMap.Sample(g_NormalSampler, uv1).rgb)
						   	+ (blend.y * g_NormalMap.Sample(g_NormalSampler, uv2).rgb)
							+ (blend.z * g_NormalMap.Sample(g_NormalSampler, uv3).rgb);
		
		finalNormal = (finalNormal * 2.0f) - 1.0f;
		//Calculate normal in world-space
		finalNormal = (finalNormal.x * inputTangent) + (finalNormal.y * inputBitangent) + (finalNormal.z * inputNormal);

		return finalNormal;
	}
	else
	{
		return inputNormal;
	}
}
#endif

//Get albedo
float3 GetAlbedo(float3 blend, float2 uv1, float2 uv2, float2 uv3)
{
	if (g_UseAlbedoMap)
	{
		return (blend.x * GammaCorrect(g_AlbedoMap.Sample(g_AlbedoSampler, uv1)))
			+ (blend.y * GammaCorrect(g_AlbedoMap.Sample(g_AlbedoSampler, uv2)))
			+ (blend.z * GammaCorrect(g_AlbedoMap.Sample(g_AlbedoSampler, uv3))); 
	}
	else
	{
		return g_MaterialColor.rgb;
	}
}

//Get roughness
float GetRoughness(float3 blend, float2 uv1, float2 uv2, float2 uv3)
{
	if (g_UseRoughnessMap)
	{
		return (blend.x * g_RoughnessMap.Sample(g_RoughnessSampler, uv1))
			+ (blend.y * g_RoughnessMap.Sample(g_RoughnessSampler, uv2))
			+ (blend.z * g_RoughnessMap.Sample(g_RoughnessSampler, uv3)); 
	}
	else
	{
		return g_Roughness;
	}
}

//Get ambient occlusion
float GetAmbientOcclution(float3 blend, float2 uv1, float2 uv2, float2 uv3)
{
	if (g_UseAoMap)
	{
		return (blend.x * g_AoMap.Sample(g_AoSampler, uv1))
			+ (blend.y * g_AoMap.Sample(g_AoSampler, uv2))
			+ (blend.z * g_AoMap.Sample(g_AoSampler, uv3)); 
	}
	else
	{
		return g_AmbientOcclusion;
	}
}

//Get metalness
float GetMetalness(float3 blend, float2 uv1, float2 uv2, float2 uv3)
{
	if (g_UseMetallicMap)
	{
		return (blend.x * g_MetallicMap.Sample(g_MetallicSampler, uv1))
			+ (blend.y * g_MetallicMap.Sample(g_MetallicSampler, uv2))
			+ (blend.z * g_MetallicMap.Sample(g_MetallicSampler, uv3)); 
	}
	else
	{
		return g_Metallic;
	}
}

PS_OUT main(PS_IN input)
{
	PS_OUT output;

	//Get uvs
	float2 uv1 = input.uv1;
	float2 uv2 = input.uv2;
	float2 uv3 = input.uv3;

	//Blendfactor
	float3 blend = abs(normalize(input.normal));
	blend /= blend.x + blend.y + blend.z + 0.0001f;  

//TODO: Triplanar mapping for Parralaxmapping?
#ifdef PARALLAXMAPPING
	uv2 = GetParrallaxedUVs(uv2, normalize(input.tangentCamera - input.tangentPos));
#else
	uv2 = uv2;
#endif
	//Output albedo and a mask in albedobuffer
	output.albedo.rgb = GetAlbedo(blend, uv1, uv2, uv3);
	output.albedo.a = 0.0f;
	//Output normal in normalbuffer
#ifdef NORMALMAPPING
	output.normal.rgb = GetNormal(blend, uv1, uv2, uv3, input.normal, input.tangent, input.bitangent); 
#else
	output.normal.rgb = input.normal;
#endif
	output.normal.rgb = (output.normal.rgb * 0.5f) + 0.5f;
	//Output roughness into alpha of normalbuffer
	output.normal.a = GetRoughness(blend, uv1, uv2, uv3);	
	//Output position and metalness to position buffer
    output.position.rgb = input.worldPosition;
	output.position.a = GetMetalness(blend, uv1, uv2, uv3);
	//Output ao to materialbuffer
	output.material.r = GetAmbientOcclution(blend, uv1, uv2, uv3);

	return output;
}