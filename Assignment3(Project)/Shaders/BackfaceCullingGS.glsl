#version 440

//#define FRUSTUMCULL

in VS_OUT
{
	vec3 worldPosition;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 tangentPos;
	vec3 tangentCamera;
	vec2 uv;
} gs_in[];

in gl_PerVertex
{
	vec4 gl_Position;
} gl_in[];

out PS_IN
{
	vec3 worldPosition;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 tangentPos;
	vec3 tangentCamera;
	vec2 uv;
} gs_out;

out gl_PerVertex
{
	vec4 gl_Position;
};

#ifdef FRUSTUMCULL
//Frustumcull
bool InsideFrustum(vec3 p[3])
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

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
void main()
{
    //Perspective divide
	vec3 positions[3];
	positions[0] = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
	positions[1] = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
	positions[2] = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;

    //Backfaceculling in screenspace
	vec3 e1 = positions[1] - positions[0];
	vec3 e2 = positions[2] - positions[0];
    vec3 normal = normalize(cross(e1, e2));

    if (normal.z > 0.0f)
        return;

#ifdef FRUSTUMCULL
	//Do frustum culling per triangle
	if (!InsideFrustum(positions))
        return;
#endif

    //Output triangle if frontface
    for (int i = 0; i < 3; i++)
    {
        gs_out.worldPosition = gs_in[i].worldPosition;
        gs_out.normal = gs_in[i].normal;
        gs_out.tangent = gs_in[i].tangent;
        gs_out.bitangent = gs_in[i].bitangent;
        gs_out.tangentPos = gs_in[i].tangentPos;
        gs_out.tangentCamera = gs_in[i].tangentCamera;
        gs_out.uv = gs_in[i].uv;

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}