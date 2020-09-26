#define MAXTESSFACTOR 32.0f

//How much each edge and triangle middlepoint should tesselate
struct PatchFactor
{
	float edgeFactor[3] : SV_TESSFACTOR;
	float insideFactor[1] : SV_INSIDETESSFACTOR;
	float vDistanceSqrd[3] : DISTANCE;
};

struct HS_VERTEX
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

cbuffer HSCameraBuffer : register(b0)
{
	float4x4 g_Camera;
	float4x4 g_CameraView;
	float4 g_CameraPosition;	
};

cbuffer HSMaterialBuffer : register(b1)
{
	float g_DisplacementScale;
	float g_MinDistance;
	float g_MaxDistance;
	float g_TerrainUnits;

	float g_HeightScale;
	float g_TerrainUvScale;
	float2 padding;
}

//Calculate distance sqrd
float DistanceSqrd(float3 position)
{
	float3 vec = position - g_CameraPosition.xyz;
	float minDistSqrd = g_MinDistance * g_MinDistance;
	float distanceSqrd = dot(vec, vec);
	
	return ((distanceSqrd - minDistSqrd) / ((g_MaxDistance * g_MaxDistance) - minDistSqrd));
}

//Calculate tesselation factor
float CalculateFactor(float distance)
{
	//Zero if far away else 1.0f
	float distanceFactor = 1.0f - distance;
	//Make sure factor always is 1
	return max(MAXTESSFACTOR * distanceFactor, 1.0f);
}

//Constant Patch function, return how much each patch should be tesselated
//Can take the controlpoints as input and an uint that is the SV_PRIMITIVEID
PatchFactor ConstantPatchFunc(InputPatch<HS_VERTEX, 3> input)
{
    PatchFactor output;

	/*TODO: Fix backfaceculling, some triangles gets false positives
	//To screenspace and perspective divide
	float4 positions[3];
	positions[0] = mul(float4(input[0].position, 1.0f), g_Camera);
	positions[1] = mul(float4(input[1].position, 1.0f), g_Camera);
	positions[2] = mul(float4(input[2].position, 1.0f), g_Camera);

	positions[0].xyz = positions[0].xyz / positions[0].w;
	positions[1].xyz = positions[1].xyz / positions[1].w;
	positions[2].xyz = positions[2].xyz / positions[2].w;

	//Backfaceculling in screenspace
	float3 e1 = positions[1].xyz - positions[0].xyz;
	float3 e2 = positions[2].xyz - positions[0].xyz;
	float3 normal = normalize(cross(e2, e1));	
	if (normal.z < 0.0f)
	{
		//Distance = 0
		output.vDistanceSqrd[0] = 0.0f;
		output.vDistanceSqrd[1] = 0.0f;
		output.vDistanceSqrd[2] = 0.0f; 

		//Output factors
		output.edgeFactor[0] = 0.0f;
		output.edgeFactor[1] = 0.0f;
		output.edgeFactor[2] = 0.0f;
    	output.insideFactor[0] = 0.0f;

		return output;
	}*/
	
	//Calculate distance from position to camerapos
	for (int i = 0; i < 3; i++)
	{
		output.vDistanceSqrd[i] = DistanceSqrd(input[i].position);
	}

	//TODO: Implement frustum culling per triangle/patch

	//Calculate factors
	float p1 = (output.vDistanceSqrd[1] + output.vDistanceSqrd[2]) * 0.5f;
    float p2 = (output.vDistanceSqrd[2] + output.vDistanceSqrd[0]) * 0.5f;
    float p3 = (output.vDistanceSqrd[0] + output.vDistanceSqrd[1]) * 0.5f;
    float centre = (output.vDistanceSqrd[0] + output.vDistanceSqrd[1] + output.vDistanceSqrd[2]) / 3.0f;


    //Output factors
	output.edgeFactor[0] = CalculateFactor(p1);
	output.edgeFactor[1] = CalculateFactor(p2);
	output.edgeFactor[2] = CalculateFactor(p3);
    output.insideFactor[0] = CalculateFactor(centre);

	return output;
};

//Shader attributes
[domain("tri")]
[partitioning("fractional_odd")]
[maxtessfactor(MAXTESSFACTOR)]
[outputtopology("triangle_cw")]
[patchconstantfunc("ConstantPatchFunc")]
[outputcontrolpoints(3)]
//Main
//Once per output controlpoint - Inputs the control points, the index of the controlpoint
//and the index of the patch
HS_VERTEX main(InputPatch<HS_VERTEX, 3> input, uint i : SV_OUTPUTCONTROLPOINTID)
{
	return input[i];
}