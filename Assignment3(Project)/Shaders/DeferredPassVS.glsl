#version 440

#define PARALLAXMAPPING
#define NORMALMAPPING

#ifdef PARALLAXMAPPING
#define CALCTANGENTS
#endif

#ifdef NORMALMAPPING
#ifndef CALCTANGENTS
#define CALCTANGENTS
#endif
#endif

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;
layout(location = 2) in vec3 vs_Tangent;
layout(location = 3) in vec2 vs_Uv;

out gl_PerVertex
{
	vec4 gl_Position;
};

out VS_OUT
{
	vec3 worldPosition;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 tangentPos;
	vec3 tangentCamera;
	vec2 uv;
} vs_out;

//Vertexshaders starts at slot 0
layout(binding = 0, std140) uniform VSCameraBuffer
{
	mat4 g_Camera;
	mat4 g_CameraView;
	vec4 g_CameraPosition;
};

layout(binding = 1, std140) uniform VSTransformBuffer
{
	mat4 g_TransformMatrix;
	mat4 g_NormalMatrix;
};

void main()
{
	//Transform to world
	vs_out.worldPosition = (vec4(vs_Position, 1.0f) * g_TransformMatrix).xyz;
	//Set gl_Position
	gl_Position = vec4(vs_out.worldPosition, 1.0f) * g_Camera;
	//Transform normal
	vs_out.normal = (vec4(vs_Normal, 0.0f) * g_NormalMatrix).xyz;
	//Pass on uv coords
	vs_out.uv = vs_Uv;

//Calculate tangents if needed
#ifdef CALCTANGENTS
	//Calculate bitangent
	vs_out.tangent = (vec4(vs_Tangent, 0.0f) * g_NormalMatrix).xyz;
	vs_out.bitangent = cross(vs_out.normal, vs_out.tangent);
#else
	vs_out.tangent = vs_Tangent;
	vs_out.bitangent = vec3(0.0f);
#endif

//If we want parallaxmapping
#ifdef PARALLAXMAPPING
	//Transform cameraviewdirection to tangentspace
	mat3 tbn = transpose(mat3(vs_out.tangent, vs_out.bitangent, vs_out.normal));
	vs_out.tangentPos = tbn * vs_out.worldPosition;
	vs_out.tangentCamera =  tbn * g_CameraPosition.xyz;
#else
	vs_out.tangentPos = vec3(0.0f);
	vs_out.tangentCamera = vec3(0.0f);
#endif
}