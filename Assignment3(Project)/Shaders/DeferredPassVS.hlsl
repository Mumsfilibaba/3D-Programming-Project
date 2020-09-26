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

struct VS_IN
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
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

cbuffer VSCameraBuffer : register(b0)
{
	float4x4 g_Camera;
	float4x4 g_CameraView;
	float4 g_CameraPosition;
};

cbuffer VSTransformBuffer : register(b1)
{
	float4x4 g_TransformMatrix;
	float4x4 g_NormalMatrix;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	//Transform to world
	output.worldPosition = mul(float4(input.position, 1.0f), g_TransformMatrix).xyz;
	//Transform to camera and project
	output.position = mul(float4(output.worldPosition, 1.0f), g_Camera);
	//Transform normal
	output.normal = mul(float4(input.normal, 0.0f), g_NormalMatrix).xyz;
	//Pass on UV-coords
	output.uv = input.uv;

//Calculate tangents if needed
#ifdef CALCTANGENTS
	//Transform tangent
	output.tangent = mul(float4(input.tangent, 0.0f), g_NormalMatrix).xyz;
	//Calculate bitangent from normal and tangent
	output.bitangent = float4(cross(output.normal.xyz, output.tangent.xyz), 0.0f).xyz;
#else
	output.tangent = input.tangent;
	output.bitangent.xyz = 0.0f;
#endif

//if we want parallaxmapping
#ifdef PARALLAXMAPPING
	//Transform cameraviewdirection to tangentspace
	float3x3 tbn = transpose(float3x3(output.tangent, output.bitangent, output.normal));
	output.tangentPos = mul(output.worldPosition, tbn);
	output.tangentCamera = mul(g_CameraPosition.xyz, tbn);
#else
	output.tangentPos.xyz = 0.0f;
	output.tangentCamera.xyz = 0.0f;
#endif

	return output;
}