//#define FRUSTUMCULL

struct GS_VERTEX
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

#ifdef FRUSTUMCULL
//Frustumcull
bool InsideFrustum(float3 p[3])
{
	int inside1 = 0;
	int inside2 = 0;

	for (int i = 0; i < 3; i++)
	{
		//Something seems a bit odd when using [-1, 1]
		inside1 += (abs(p[i].y) < 1.15) ? 1 : 0; 
		inside2 += (abs(p[i].x) < 1.15) ? 1 : 0; 
	}

	return (inside1 > 0) || (inside2 > 0);
}
#endif

[maxvertexcount(3)]
void main(triangle GS_VERTEX input[3], inout TriangleStream<GS_VERTEX> output)
{
	//Perspective divide
	float3 positions[3];
	positions[0] = input[0].position.xyz / input[0].position.w;
	positions[1] = input[1].position.xyz / input[1].position.w;
	positions[2] = input[2].position.xyz / input[2].position.w;

	//Backfaceculling in screenspace
	float3 e1 = positions[1] - positions[0];
	float3 e2 = positions[2] - positions[0];
    float3 normal = normalize(cross(e1, e2));

    if (normal.z > 0.0f)
        return;

#ifdef FRUSTUMCULL
	//Do frustum culling per triangle
	if (!InsideFrustum(positions))
        return;
#endif

	//Output triangle if frontface and inside frustum
	for (int i = 0; i < 3; i++)
		output.Append(input[i]);
}