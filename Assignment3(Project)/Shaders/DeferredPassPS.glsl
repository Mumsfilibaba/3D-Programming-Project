#version 440

#define PARALLAXMAPPING
#define NORMALMAPPING

layout(location = 0) out vec4 g_AlbedoBuffer;
layout(location = 1) out vec4 g_NormalBuffer;
layout(location = 2) out vec4 g_PositionBuffer;
layout(location = 3) out float g_MaterialBuffer;

//Pixelshaders has textureslots 0-11
layout(binding = 0) uniform sampler2D g_AlbedoMap;
#ifdef NORMALMAPPING
layout(binding = 1) uniform sampler2D g_NormalMap; 
#endif
layout(binding = 2) uniform sampler2D g_RoughnessMap;
layout(binding = 3) uniform sampler2D g_AoMap;
layout(binding = 4) uniform sampler2D g_MetallicMap;
#ifdef PARALLAXMAPPING
layout(binding = 5) uniform sampler2D g_HeightMap;
#endif

in PS_IN
{
	vec3 worldPosition;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 tangentPos;
	vec3 tangentCamera;
	vec2 uv;
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

//Gammaconstant
const float GAMMA = 2.2f;

//Gammacorrect textures (assumes that they are in sRGB space)
//Means that albedos has to be in sRGB
vec3 GammaCorrect(vec3 color)
{
	return pow(color, vec3(GAMMA));
}

#ifdef PARALLAXMAPPING
//Get miplevel
float GetMiplevel(vec2 uv)
{
	//Get texturesize
	vec2 texelSize = textureSize(g_HeightMap, 0);

	//Get derivetives of uv
	vec2 dx = vec2(dFdx(uv.x * texelSize.x));
	vec2 dy = vec2(dFdy(uv.y * texelSize.y));

	//Return level
	return 0.5 * log2(max(dot(dx, dx), dot(dy, dy)));
}
//ParallaxMapped UVs
vec2 GetParrallaxedUVs(vec2 uv, vec3 viewDir)
{
	if (g_UseHeightMap < 1.0f)
		return uv;
	
	//Get miplevelcount
	float miplevels = textureQueryLevels(g_HeightMap);
	//Get miplevel
	float miplevel = GetMiplevel(uv);


	//The number of layers based on viewing angle
	int layers = max(int(mix(g_ParralaxLayersMax, g_ParralaxLayersMin, 1.0f - (miplevel / miplevels))), int(g_ParralaxLayersMax));
	//Depth of one layer
	float layerDepth = 1.0f / layers;
	//Current depth
	float currentDepth = 0.0f;
	//Current UVs
	vec2 currentUV = uv;
	//UV change per layer
	vec2 deltaUV = ((viewDir.xy / viewDir.z) * g_ParralaxScale) / layers;
	//Sample height
	float mappedDepth = (1.0f - textureLod(g_HeightMap, currentUV, miplevel)).r;

	//Find correct UVs
	for (int i = 0; i < layers; i++)
	{
		//Check depth
		if (currentDepth > mappedDepth)
			break;
		//Add to UVs
		currentUV -= deltaUV;
		//Sample mapped depth from heightmap (1.0f - because of depth instead of height)
		mappedDepth = (1.0f - textureLod(g_HeightMap, currentUV, miplevel)).r;		
		//Add to depth
		currentDepth += layerDepth;
	}

	//Interpolate the value before and after collision
	vec2 prev = currentUV + deltaUV;

	float before = (1.0f - textureLod(g_HeightMap, currentUV, miplevel)).r - (currentDepth + layerDepth);	
	float after = mappedDepth - currentDepth;

	float weight = after / (after - before);
	vec2 final = (currentUV * weight) + (prev * (1.0f - weight));

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
vec3 GetNormal(vec2 uv, vec3 inputNormal, vec3 inputTangent, vec3 inputBitangent)
{
	if (g_UseNormalMap > 0.0f)
	{
		//Sample normal map
		vec3 textureNormal = texture(g_NormalMap, uv).rgb;
		//Put normal in [-1, 1] range
		textureNormal = (textureNormal * 2.0f) - 1.0f;
		//Calculate normal in world-space
		mat3 tbn = mat3(inputTangent, inputBitangent, inputNormal);
		textureNormal = tbn * textureNormal;

		return textureNormal;
	}
	else
	{
		return inputNormal;
	}
}
#endif

//Get albedo
vec3 GetAlbedo(vec2 uv)
{
	return (GammaCorrect(texture(g_AlbedoMap, uv).rgb) * g_UseAlbedoMap) + (g_MaterialColor.rgb * (1.0f - g_UseAlbedoMap));
}

//Get roughness
float GetRoughness(vec2 uv)
{
	return (texture(g_RoughnessMap, uv).r * g_UseRoughnessMap) + (g_Roughness * (1.0f - g_UseRoughnessMap));
}

//Get ambient occlusion
float GetAmbientOcclution(vec2 uv)
{
	return (texture(g_AoMap, uv).r * g_UseAoMap) + (g_AmbientOcclusion * (1.0f - g_UseAoMap));
}

//Get metalness
float GetMetalness(vec2 uv)
{
	return (texture(g_MetallicMap, uv).r * g_UseMetallicMap) + (g_Metallic * (1.0f - g_UseMetallicMap));
}

void main()
{
	//Get uvs
	vec2 uv;
#ifdef PARALLAXMAPPING
	uv = GetParrallaxedUVs(ps_in.uv, normalize(ps_in.tangentCamera - ps_in.tangentPos));
#else
	uv = ps_in.uv;
#endif

	//Output albedo and a mask in albedobuffer
	g_AlbedoBuffer.rgb = GetAlbedo(uv);
	g_AlbedoBuffer.a = 0.0f;
	//Output normal
#ifdef NORMALMAPPING
	g_NormalBuffer.rgb = GetNormal(uv, ps_in.normal, ps_in.tangent, ps_in.bitangent);
#else
	g_NormalBuffer.rgb = ps_in.normal;
#endif
	g_NormalBuffer.rgb = (g_NormalBuffer.rgb * 0.5f) + 0.5f;
	//Output roughness into normalbuffer
	g_NormalBuffer.a = GetRoughness(uv);
	//Output position and metalness to position buffer
    g_PositionBuffer.rgb = ps_in.worldPosition.xyz;
	g_PositionBuffer.a = GetMetalness(uv);
	//Output ao to materialbuffer
	g_MaterialBuffer.r = GetAmbientOcclution(uv);
}