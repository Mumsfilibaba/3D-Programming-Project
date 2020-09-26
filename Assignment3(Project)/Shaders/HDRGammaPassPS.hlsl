#define GAMMA 2.2f
#define EXPOSURE 0.75f

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//Buffer
Texture2D g_BackBuffer : register(t0);
SamplerState g_SamplerState : register(s0);
Texture2D g_GlowBuffer : register(t1);
SamplerState g_GlowSampler : register(s1);

//Will do gammacorrection and HDR -> LDR mapping in here for now
float4 main(PS_IN input) : SV_TARGET0
{
    //Sample backbuffer
    half4 color = g_BackBuffer.Sample(g_SamplerState, input.uv);
    half4 glowColor = g_GlowBuffer.Sample(g_GlowSampler, input.uv);
    color += glowColor;
    //Tone mapping 
    half4 ldr = 1.0f - exp(-color * EXPOSURE);
    //Gamma correct
    return pow(ldr, 1.0f / GAMMA);
}