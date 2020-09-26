#version 440

#define SIZE 4

layout(binding = 0) uniform sampler2D g_Buffer;

in VS_OUT
{
    vec2 uv;
} ps_in;

out vec4 g_Color;

//KERNEL
const float weights[] = { 0.214607, 0.189879, 0.131514, 0.071303 };

//PixelShaders start at 16
layout(binding = 16) uniform PSGlowUniform
{
    float g_Mode;
    float g_Brightness;
    vec2 padding;
};

void main()
{
    float brightness = 0.0f;
    vec4 color =  vec4(0.0f, 0.0f, 0.0f, 1.0f);

    //First pass - Gather pixels
    if (g_Mode == 0.0f)
    {
        color = texture(g_Buffer, ps_in.uv); 
        brightness = dot(color.rgb, vec3(0.21f, 0.72f, 0.07f));
        color *= float(brightness > g_Brightness);
    }
    else 
    {
        //Get texelsize
        vec2 texelSize = 1.0f / textureSize(g_Buffer, 0);
        
        //Sample middletexel of the kernel
        color = texture(g_Buffer, ps_in.uv) * weights[0];
        
        //Second pass - Vertical blur
        if (g_Mode == 1.0f)
        {
            for (uint x = 1; x < SIZE; x++)
            {
                color += texture(g_Buffer, ps_in.uv + vec2(texelSize.x * x, 0.0f)) * weights[x];  
                color += texture(g_Buffer, ps_in.uv - vec2(texelSize.x * x, 0.0f)) * weights[x];
            }
        }
        //Third pass - horizontal blur
        else if (g_Mode == 2.0f)
        {
            for (uint y = 1; y < SIZE; y++)
            {
                color += texture(g_Buffer, ps_in.uv + vec2(0.0f, texelSize.y * y)) * weights[y];  
                color += texture(g_Buffer, ps_in.uv - vec2(0.0f, texelSize.y * y)) * weights[y];
            }
        }
    }

    g_Color = color;
}