#version 440

#define GAMMA 2.2f
#define EXPOSURE 0.75f

in VS_OUT
{
	vec2 uv;
} ps_in;

out vec4 g_Color;

//Pixelshaders has textureslots 0-11
layout(binding = 0) uniform sampler2D g_BackBuffer;
layout(binding = 1) uniform sampler2D g_GlowBuffer;

//Will do gammacorrection and HDR -> LDR mapping in here for now
void main()
{
    //Sample backbuffer
    vec4 color = texture(g_BackBuffer, ps_in.uv);
    vec4 glowColor = texture(g_GlowBuffer, ps_in.uv);
    color += glowColor;
    //Tone mapping 
    vec4 ldr = 1.0f - exp(-color * EXPOSURE);
    //Gamma correct
    g_Color = pow(ldr, vec4(1.0f / GAMMA));
}