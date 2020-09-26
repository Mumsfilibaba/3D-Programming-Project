#version 440

#define MAXTESSFACTOR 32.0f

in VS_OUT
{
    vec3 position;
    vec2 uv;
} hs_in[];

out HS_OUT
{
    vec3 position;
    vec2 uv;
    float distanceSqrd;
} hs_out[];

//HullShaders begin at 24
layout(binding = 24, std140) uniform HSCameraBuffer
{
    mat4 g_HSCamera;
    mat4 g_HSCameraRotation;
    vec4 g_HSCameraPosition;
};

layout(binding = 25, std140) uniform HSMaterialBuffer
{
	float g_HSDisplacementScale;
	float g_HSMinDistance;
	float g_HSMaxDistance;
	float g_HSTerrainUnits;

	float g_HSHeightScale;
	float g_HSTerrainUvScale;
	vec2 HSpadding;
};

//Calculate distance sqrd
float DistanceSqrd(vec3 position)
{
	vec3 vec = position - g_HSCameraPosition.xyz;
	float minDistSqrd = g_HSMinDistance * g_HSMinDistance;
	float distanceSqrd = dot(vec, vec);
	
	return ((distanceSqrd - minDistSqrd) / ((g_HSMaxDistance * g_HSMaxDistance) - minDistSqrd));
}

//Calculate tesselation factor
float CalculateFactor(float distance)
{
	//Zero if far away else 1.0f
	float distanceFactor = 1.0f - distance;
	//Make sure factor always is 1
	return max(MAXTESSFACTOR * distanceFactor, 1.0f);
}

layout(vertices = 3) out;
void main()
{
    //Set output to next stage
    hs_out[gl_InvocationID].position = hs_in[gl_InvocationID].position;
    hs_out[gl_InvocationID].uv = hs_in[gl_InvocationID].uv;
    
    /*TODO: Fix backfaceculling, some triangles gets false positives
    //To screenspace and perspective divide
	vec4 positions[3];
	positions[0] = vec4(hs_in[0].position, 1.0f) * g_HSCamera;
	positions[1] = vec4(hs_in[1].position, 1.0f) * g_HSCamera;
	positions[2] = vec4(hs_in[2].position, 1.0f) * g_HSCamera;

	positions[0].xyz = positions[0].xyz / positions[0].w;
	positions[1].xyz = positions[1].xyz / positions[1].w;
	positions[2].xyz = positions[2].xyz / positions[2].w;

    //Calculate visibility
    vec3 e1 = positions[1].xyz - positions[0].xyz;
    vec3 e2 = positions[2].xyz - positions[0].xyz;
    vec3 normal = normalize(cross(e1, e2));
   
    if (normal.z > 0.0f)
	{
        //Set tesselation factors
        gl_TessLevelOuter[0] = 0.0f;
        gl_TessLevelOuter[1] = 0.0f;
        gl_TessLevelOuter[2] = 0.0f;
        gl_TessLevelInner[0] = 0.0f;

		return;
	}*/

    //Calculate distance from position to camerapos
    float dist[3];
    for (int i = 0; i < 3; i++)
    {
        dist[i] = DistanceSqrd(hs_in[i].position);
    }

    hs_out[gl_InvocationID].distanceSqrd = dist[gl_InvocationID];

    //TODO: Implement frustum culling per triangle/patch

    //Calculate factors
    float p1 = (hs_out[1].distanceSqrd + hs_out[2].distanceSqrd) * 0.5f;
    float p2 = (hs_out[2].distanceSqrd + hs_out[0].distanceSqrd) * 0.5f;
    float p3 = (hs_out[0].distanceSqrd + hs_out[1].distanceSqrd) * 0.5f;
    float centre = (hs_out[0].distanceSqrd + hs_out[1].distanceSqrd + hs_out[2].distanceSqrd) * (1.0f / 3.0f);

    //Set tesselation factors
    gl_TessLevelOuter[0] = CalculateFactor(p1);
    gl_TessLevelOuter[1] = CalculateFactor(p2);
    gl_TessLevelOuter[2] = CalculateFactor(p3);
    gl_TessLevelInner[0] = CalculateFactor(centre);
}