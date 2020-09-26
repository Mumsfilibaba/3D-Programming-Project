#version 440

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec2 vs_Uv;

out VS_OUT
{
    vec3 position;
    vec2 uv;
} vs_out;

//VertexShaders starts at 20
layout(binding = 20) uniform sampler2D g_HeightMap;

//VertexShaders starts at 0
layout(binding = 0, std140) uniform VSMaterialBuffer
{
	float g_VSDisplacementScale;
	float g_VSMinDistance;
	float g_VSMaxDistance;
	float g_VSTerrainUnits;

	float g_VSHeightScale;
	float g_VSTerrainUvScale;
	vec2 VSpadding;
};

void main()
{
    //Set Position
    vs_out.position = vs_Position;
    //Set uv
    vs_out.uv = vs_Uv;

    //Sample heightmap
    vs_out.position.y = ((texture(g_HeightMap, vs_out.uv).r * 2.0f) - 1.0f) * g_VSHeightScale; 
}