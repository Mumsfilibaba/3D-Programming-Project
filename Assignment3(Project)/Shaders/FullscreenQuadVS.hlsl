struct VS_OUT
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

//Plane data
static const float2 positions[] =
{
	float2(-1.0f, -1.0f),
	float2(-1.0f,  1.0f),
	float2( 1.0f, -1.0f),
	float2( 1.0f,  1.0f)
};

static const float2 uvs[] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

VS_OUT main(uint id: SV_VERTEXID)
{
	VS_OUT output;

	output.uv = uvs[id];
	output.position = float4(positions[id], 0.0f, 1.0f);

	return output;
}