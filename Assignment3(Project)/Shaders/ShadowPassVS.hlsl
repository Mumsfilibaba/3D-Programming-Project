struct VS_IN
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float2 uv : TEXCOORD0;
};

//Uses same lightning buffer as in the shading
cbuffer VSLightMatrix : register(b0)
{
	float4x4 g_LightMatrix;
	float3 g_LightDirection;
	float g_LightIntensity;
	float3 g_LightColor;
	float g_IblIntensity;
}

//Uses the same buffer as the transform in the real rendering
cbuffer VSTransformBuffer : register(b1)
{
	float4x4 g_TransformMatrix;
	float4x4 g_NormalMatrix;
}

float4 main(VS_IN input) : SV_POSITION0
{
	float4 position = mul(float4(input.position, 1.0f), g_TransformMatrix);
	return mul(position, g_LightMatrix);
}