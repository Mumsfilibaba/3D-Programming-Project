#version 440

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;
layout(location = 2) in vec3 vs_Tangent;
layout(location = 3) in vec2 vs_Uv;

//Vertexshaders starts at slot 0
//Same buffer as in shaingstage
layout(binding = 0, std140) uniform LightBuffer
{
	mat4 g_LightMatrix;
	vec3 g_LightDirection;
	float g_LightIntensity;
	vec3 g_LightColor;
	float g_IblIntensity;
};

//Same buffer as in shading-stage
layout(binding = 1, std140) uniform TransformBuffer
{
	mat4 g_TransformMatrix;
	mat4 g_NormalMatrix;
};

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
    gl_Position = ((vec4(vs_Position, 1.0f) * g_TransformMatrix) * g_LightMatrix);
}