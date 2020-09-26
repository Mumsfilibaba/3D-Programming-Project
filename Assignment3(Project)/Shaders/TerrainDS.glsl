#version 440

#define DISPLACEMENTMAPPING

//DomainShaders has textureslots 12 - 16 
layout(binding = 12) uniform sampler2D g_DisplacementMap;
layout(binding = 13) uniform sampler2D g_HeightMap;

in HS_OUT
{
    vec3 position;
    vec2 uv;
    float distanceSqrd;
} ds_in[];

in vec3 gl_TessCoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

out PS_IN
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
} ds_out;

//DomainShaders begin at 32
layout(binding = 32, std140) uniform DSCameraBuffer
{
    mat4 g_DSCamera;
    mat4 g_DSCameraRotation;
    vec4 g_DSCameraPosition;
};

layout(binding = 33, std140) uniform DSMaterialBuffer
{
	float g_DSDisplacementScale;
	float g_DSMinDistance;
	float g_DSMaxDistance;
	float g_DSTerrainUnits;

	float g_DSHeightScale;
	float g_DSTerrainUvScale;
	vec2 DSpadding;
};

//Sample heightmap
float SampleHeight(vec2 uv)
{
	return ((textureLod(g_HeightMap, uv, 0) * 2.0f) - 1.0f).r * g_DSHeightScale;
}

//Get normal from heightmap
vec3 GetNormal(vec2 uv)
{
	vec2 textelSize = textureSize(g_HeightMap, 0);
	textelSize = 1.0f / textelSize; 

	vec2 uv1 = uv + vec2(-textelSize.x, 0.0f);
	vec2 uv2 = uv + vec2(textelSize.x, 0.0f);
	vec2 uv3 = uv + vec2(0.0f, textelSize.y);
	vec2 uv4 = uv + vec2(0.0f, -textelSize.y);

	float height1 = SampleHeight(uv1);
	float height2 = SampleHeight(uv2);
	float height3 = SampleHeight(uv3);
	float height4 = SampleHeight(uv4);

	return vec3(height1 - height2, 1.0f, height3 - height4);
}

#ifdef DISPLACEMENTMAPPING
//Calculate miplevel
float GetMiplevel(float distance)
{
	//Mipcount
	float mipCount = textureQueryLevels(g_HeightMap);
	//Make sure miplevel always is atleast 0
	return max(mipCount * distance, 0.0f);
}

float SampleDisplacement(vec3 position, vec3 normal, vec2 uv1, vec2 uv2, vec2 uv3, float distance)
{
	vec3 blend = abs(normal);
	blend /= blend.x + blend.y + blend.z + 0.0001f; 

	//DISPLACEMENT MAPPING
	//Calculate miplevel
	float miplevel = GetMiplevel(distance);
	//Sample height from heightmap
	float mapped1 = texture(g_DisplacementMap, uv1, miplevel).r;
	float mapped2 = texture(g_DisplacementMap, uv2, miplevel).r;
	float mapped3 = texture(g_DisplacementMap, uv3, miplevel).r;
	float mapped = (blend.x * mapped1) + (blend.y * mapped2) + (blend.z * mapped3);

	return ((mapped * 2.0f) - 1.0f) * g_DSDisplacementScale;
}
#endif

vec2 ModuloVec2(vec2 left, int right)
{
	return vec2(int(left.x) % right, int(left.y) % right);
}

layout(triangles, fractional_odd_spacing, cw) in;
void main()
{
	//Interpolate coordinates
	vec3 position = (ds_in[0].position * gl_TessCoord.x) + (ds_in[1].position * gl_TessCoord.y) + (ds_in[2].position * gl_TessCoord.z);
	vec2 uv = (ds_in[0].uv * gl_TessCoord.x) + (ds_in[1].uv * gl_TessCoord.y) + (ds_in[2].uv * gl_TessCoord.z);
	float distance = (ds_in[0].distanceSqrd * gl_TessCoord.x) + (ds_in[1].distanceSqrd * gl_TessCoord.y) + (ds_in[2].distanceSqrd * gl_TessCoord.z);

	//HEIGHTMAPPING
	position.y = SampleHeight(uv);

	//Get normal
	vec3 normal = normalize(GetNormal(uv));
	vec3 tangent = vec3(1.0f, 0.0f, 0.0f);
	//Make tangent othogonal with normal
	tangent = normalize(tangent - (dot(tangent, normal) * normal));
	//Get bitangent
	vec3 bitangent = cross(normal, tangent);

	//Set uv to sample material
	float len = g_DSTerrainUnits / 2.0f;
	vec2 uv1 = (vec2(position.y, position.z) + len) / g_DSTerrainUvScale;
	vec2 uv2 = (vec2(position.x, position.z) + len) / g_DSTerrainUvScale; 
	vec2 uv3 = (vec2(position.x, position.y) + len) / g_DSTerrainUvScale; 

	//DISPLACEMENTMAPPING
#ifdef DISPLACEMENTMAPPING
#ifndef PARALLAXMAPPING
	position += (normal * SampleDisplacement(position, normal, uv1, uv2, uv3, distance));
#endif
#endif

	//Set output
	ds_out.worldPosition = position;	
	gl_Position = vec4(ds_out.worldPosition, 1.0f) * g_DSCamera;
	ds_out.normal = normal;
	ds_out.tangent = tangent;
	ds_out.bitangent = bitangent;
	ds_out.uv1 = uv1;
	ds_out.uv2 = uv2;
	ds_out.uv3 = uv3;

#ifdef PARALLAXMAPPING
	//Transform cameraviewdirection to tangentspace
	mat3 tbn = transpose(float3x3(ds_out.tangent, ds_out.bitangent, ds_out.normal));
	ds_out.tangentPos = ds_out.worldPosition * tbn;
	ds_out.tangentCamera = g_DSCameraPosition.xyz * tbn;
#else
	ds_out.tangentPos = vec3(0.0f, 0.0f, 0.0f);
	ds_out.tangentCamera = vec3(0.0f, 0.0f, 0.0f);
#endif
}