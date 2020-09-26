#version 440

//Plane data
const vec2 positions[] = 
{
	vec2(-1.0,  1.0),
	vec2(-1.0, -1.0),
	vec2( 1.0,  1.0),
	vec2( 1.0, -1.0)
};

const vec2 uvs[] = 
{
	vec2(0.0, 1.0),
	vec2(0.0, 0.0),
	vec2(1.0, 1.0),
	vec2(1.0, 0.0)
};

out gl_PerVertex
{
	vec4 gl_Position;
};

out VS_OUT
{
	vec2 uv;
} vs_out;

void main()
{
	gl_Position = vec4(positions[gl_VertexID], 0.0f, 1.0f);
	vs_out.uv = vec2(uvs[gl_VertexID]);
}