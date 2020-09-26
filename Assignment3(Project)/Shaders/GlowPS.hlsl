#define SIZE 4

Texture2D g_Buffer : register(t0);
SamplerState g_BufferSampler : register(s0);

struct VS_OUT
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer PSGlowUniform
{
    float g_Mode;
    float g_Brightness;
    float2 padding;
};

//KERNEL
static const float weights[] = { 0.214607, 0.189879, 0.131514, 0.071303 };

float4 main(VS_OUT input) : SV_TARGET
{
    float brightness = 0.0f;
    float4 color =  float4(0.0f, 0.0f, 0.0f, 1.0f);

    //First pass - Gather pixels
    if (g_Mode == 0.0f)
    {
        color = g_Buffer.Sample(g_BufferSampler, input.uv);
        brightness = dot(color.rgb, float3(0.21f, 0.72f, 0.07f));
        color *= (brightness > g_Brightness);
    }
    else 
    {
        //Get textelsize
        float2 texelSize;
		g_Buffer.GetDimensions(texelSize.x, texelSize.y);
        texelSize = 1.0f / texelSize;

        //Sample middle pixel of kernel
        color = g_Buffer.Sample(g_BufferSampler, input.uv) * weights[0];
        
        //Second pass - Vertical blur
        if (g_Mode == 1.0f)
        {
            for (uint x = 1; x < SIZE; x++)
            {
                color += g_Buffer.Sample(g_BufferSampler, input.uv + float2(texelSize.x * x, 0.0f)) * weights[x];
                color += g_Buffer.Sample(g_BufferSampler, input.uv - float2(texelSize.x * x, 0.0f)) * weights[x];
            }
        }
        //Third pass - horizontal blur
        else if (g_Mode == 2.0f)
        {
            for (uint y = 1; y < SIZE; y++)
            {
                color += g_Buffer.Sample(g_BufferSampler, input.uv + float2(0.0f, texelSize.y * y)) * weights[y];
                color += g_Buffer.Sample(g_BufferSampler, input.uv - float2(0.0f, texelSize.y * y)) * weights[y];
            }
        }
    }

    return color;
}