#version 440

//#define PARALLAXMAPPING
#define NORMALMAPPING
#define GAMMA 2.2f

//Outputs
layout(location = 0) out vec4 g_AlbedoBuffer;
layout(location = 1) out vec4 g_NormalBuffer;
layout(location = 2) out vec4 g_PositionBuffer;
layout(location = 3) out float g_MaterialBuffer; 

//Inputs
layout (binding = 0) uniform sampler2D g_AlbedoMap;
#ifdef NORMALMAPPING
layout (binding = 1) uniform sampler2D g_NormalMap;
#endif
layout (binding = 2) uniform sampler2D g_RoughnessMap;
layout (binding = 3) uniform sampler2D g_AoMap;
layout (binding = 4) uniform sampler2D g_MetallicMap;
#ifdef PARALLAXMAPPING
layout (binding = 5) uniform sampler2D g_HeightMap;
#endif

in PS_IN
{
	vec3 worldPosition;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 tangentPos;
	vec3 tangentCamera;
	vec2 uv1;
	vec2 uv2;
	vec2 uv3;
} ps_in;

//PixelShaders begin at 16
layout(binding = 16, std140) uniform PSMaterialBuffer
{
	//MATERIALPROPERTIES
	vec4 g_MaterialColor;
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
vec3 GammaCorrect(vec3 color)
{
	return pow(abs(color), vec3(GAMMA));
}

#ifdef PARALLAXMAPPING
//Get miplevel
float GetMiplevel(vec2 uv)
{
	//Get texturesize
	vec2 texSize = textureSize(g_HeightMap, 0);
	
    //Get derivetives of uv
	vec2 dx = dFdx(vec2(uv.x * texSize.x));
	vec2 dy = dFdx(vec2(uv.y * texSize.y));

	//Get level
	return 0.5f * log2(max(dot(dx, dx), dot(dy, dy))); 
}

//ParallaxMapped UVs
vec2 GetParrallaxedUVs(vec2 uv, vec3 viewDir)
{
		if (g_UseHeightMap < 1.0f)
		return uv;

	//Get and miplevelcount
	float miplevels = textureQueryLevels(g_HeightMap);
	//Get miplevel - Should be the same for all samples even tho the uv changes
	float miplevel = GetMiplevel(uv);


	float mappedDepth = 0.0f;
	//The number of layers based on miplevel
	float layers = lerp(g_ParralaxLayersMax, g_ParralaxLayersMin, miplevel / miplevels);
	//Depth of one layer
	float layerDepth = 1.0f / layers;
	//Current depth
	float currentDepth = 0.0f;
	//Current UVs
	vec2 currentUV = uv;
	//UV change per layer
	vec2 deltaUV = ((viewDir.xy / viewDir.z) * g_ParralaxScale) / layers;

	//Find correct UVs
	for (int i = 0; i < layers; i++)
	{
		//Sample mapped depth from heightmap (1.0f - because of depth instead of height)
		mappedDepth = (1.0f - textureLod(g_HeightMap, currentUV, miplevel)).r;
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
vec3 GetNormal(vec3 blend, vec2 uv1, vec2 uv2, vec2 uv3, vec3 inputNormal, vec3 inputTangent, vec3 inputBitangent)
{
	if (g_UseNormalMap > 0.0f)
	{
		//Sample normal map
		vec3 finalNormal    = (blend.x * texture(g_NormalMap, uv1).rgb)
						    + (blend.y * texture(g_NormalMap, uv2).rgb)
							+ (blend.z * texture(g_NormalMap, uv3).rgb);
		
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
vec3 GetAlbedo(vec3 blend, vec2 uv1, vec2 uv2, vec2 uv3)
{
	if (g_UseAlbedoMap > 0.0f)
	{
		return (blend.x * GammaCorrect(texture(g_AlbedoMap, uv1).rgb))
			+ (blend.y * GammaCorrect(texture(g_AlbedoMap, uv2).rgb))
			+ (blend.z * GammaCorrect(texture(g_AlbedoMap, uv3).rgb)); 
	}
	else
	{
		return g_MaterialColor.rgb;
	}
}

//Get roughness
float GetRoughness(vec3 blend, vec2 uv1, vec2 uv2, vec2 uv3)
{
	if (g_UseRoughnessMap > 0.0f)
	{
		return (blend.x * texture(g_RoughnessMap, uv1).r)
			+ (blend.y * texture(g_RoughnessMap, uv2).r)
			+ (blend.z * texture(g_RoughnessMap, uv3).r); 
	}
	else
	{
		return g_Roughness;
	}
}

//Get ambient occlusion
float GetAmbientOcclution(vec3 blend, vec2 uv1, vec2 uv2, vec2 uv3)
{
	if (g_UseAoMap > 0.0f)
	{
		return (blend.x * texture(g_AoMap, uv1).r)
			+ (blend.y * texture(g_AoMap, uv2).r)
			+ (blend.z * texture(g_AoMap, uv3).r); 
	}
	else
	{
		return g_AmbientOcclusion;
	}
}

//Get metalness
float GetMetalness(vec3 blend, vec2 uv1, vec2 uv2, vec2 uv3)
{
	if (g_UseMetallicMap > 0.0f)
	{
		return (blend.x * texture(g_MetallicMap, uv1).r)
			+ (blend.y * texture(g_MetallicMap, uv2).r)
			+ (blend.z * texture(g_MetallicMap, uv3).r); 
	}
	else
	{
		return g_Metallic;
	}
}

void main()
{
//Get uvs
	vec2 uv1 = ps_in.uv1;
	vec2 uv2 = ps_in.uv2;
	vec2 uv3 = ps_in.uv3;

	//Blendfactor
	vec3 blend = abs(normalize(ps_in.normal));
	blend /= blend.x + blend.y + blend.z + 0.0001f;  

//TODO: Triplanar mapping for Parralaxmapping?
#ifdef PARALLAXMAPPING
	uv2 = GetParrallaxedUVs(uv2, normalize(ps_in.tangentCamera - ps_in.tangentPos));
#else
	uv2 = uv2;
#endif
	//Output albedo and a mask in albedobuffer
	g_AlbedoBuffer.rgb = GetAlbedo(blend, uv1, uv2, uv3);
	g_AlbedoBuffer.a = 0.0f;
	//Output normal in normalbuffer
#ifdef NORMALMAPPING
	g_NormalBuffer.rgb = GetNormal(blend, uv1, uv2, uv3, ps_in.normal, ps_in.tangent, ps_in.bitangent); 
#else
	g_NormalBuffer.rgb = ps_in.normal;
#endif
	g_NormalBuffer.rgb = (g_NormalBuffer.rgb * 0.5f) + 0.5f;
	//Output roughness into alpha of normalbuffer
	g_NormalBuffer.a = GetRoughness(blend, uv1, uv2, uv3);	
	//Output position and metalness to position buffer
    g_PositionBuffer.rgb = ps_in.worldPosition;
	g_PositionBuffer.a = GetMetalness(blend, uv1, uv2, uv3);
	//Output ao to materialbuffer
	g_MaterialBuffer.r = GetAmbientOcclution(blend, uv1, uv2, uv3);
}