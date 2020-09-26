//PCF-Kernel
#define KERNELSIZE 1
//Shadow-Bias
#define MAXBIAS 0.001f
#define MINBIAS 0.0005f
//PI
#define PI 3.14159265359f
//Gamma factor
#define GAMMA 2.2f

//G-Buffers
Texture2D g_AlbedoBuffer : register(t0);
SamplerState g_AlbedoSampler : register(s0);
Texture2D g_NormalBuffer : register(t1);
SamplerState g_NormalSampler : register(s1);
Texture2D g_PositionBuffer : register(t2);
SamplerState g_PositionSampler : register(s2);
Texture2D<float> g_MaterialBuffer : register(t3);
SamplerState g_MaterialSampler : register(s3);
//ShadowMap
Texture2D<float> g_ShadowMap : register(t4);
SamplerState g_ShadowSampler : register(s4);
//IntegrationMap - Look up texture for PBR
Texture2D<float2> g_IntegrationMap : register(t5);
SamplerState g_IntegrationSampler : register(s5);
//EnvironmentMap 
TextureCube g_RadianceMap : register(t6);
SamplerState g_RadianceSampler : register(s6);
//IrradianceMap 
TextureCube g_IrradianceMap : register(t7);
SamplerState g_IrradianceSampler : register(s7);

//Vertex input
struct PS_IN
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//LightBuffer
cbuffer PSLightBuffer : register(b0)
{
	float4x4 g_LightMatrix;
	float3 g_LightDirection;
	float g_LightIntensity;
	float3 g_LightColor;
	float g_IblIntensity;
};

//CameraBuffer
cbuffer PSCameraBuffer : register(b1)
{
	float4x4 g_Camera;
	float4x4 g_CameraRotation;
	float4 g_CameraPosition;
};

//Gamma correction function
float3 GammaCorrect(float3 color)
{
	return pow(color, GAMMA);
}

//Distrubution function
float DistrubutionGGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float denominator = (NdotH * NdotH * (a2 - 1.0f) + 1.0f);

	return a2 / (PI * denominator * denominator);
}

//Geometry function (SchlickGGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    return NdotV / (NdotV * (1.0f - k) + k);
}

//Geometry function (Smith)
float GeometrySmith(float NdotL, float NdotV, float roughness)
{
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

//Fresnel function
float3 FresnelSchlick(float3 Fo, float cosTheta)
{
    return Fo + (1.0f - Fo) * pow(1.0f - cosTheta, 5.0f);
}

//Fresnel with roughtness - function
float3 FresnelSchlickRoughness(float3 Fo, float cosTheta, float glossiness)
{
    return Fo + (max(float3(glossiness, glossiness, glossiness), Fo) - Fo) * pow(1.0f - cosTheta, 5.0f);
}

//Lightning calculation
float3 CalculateLight(float3 normal, float3 viewDir, float3 lightDirection, float3 radiance,
 float3 F0, float3 albedo, float roughness, float NdotV, float metallic)
{
	//Halftvector
	float3 halfVector = normalize(viewDir + lightDirection);
	float NdotL = max(dot(normal, lightDirection), 0.0f);

	//Calculate BRDF
	float NDF = DistrubutionGGX(max(dot(normal, halfVector), 0.0f), roughness);
	float G = GeometrySmith(NdotL, NdotV, roughness);
	//Calculate specular
	float3 Ks = FresnelSchlick(F0, max(dot(halfVector, viewDir), 0.0f));
    float3 specular = (NDF * G * Ks) / (4 * NdotV * NdotL + 0.001f);

	//Calculate diffuse;
    float3 Kd = 1.0f - Ks;
    Kd = Kd * (1.0f - metallic);

	//Return light
	return (((Kd * albedo) / PI) + specular) * radiance * NdotL;
}

//Sample environmentmap //Becuase these arn't in HDR we simulate higher intensities by a constant
float3 SampleEnvironmentMap(float3 uvw, float mip)
{
	return g_RadianceMap.SampleLevel(g_RadianceSampler, uvw, mip).rgb * g_IblIntensity;
}

//Image based lightning
float3 CalculateIBL(float3 albedo, float3 F0, float3 reflection, float3 normal, float NdotV, float roughness, float metallic, float ao)
{
	//ambient light
    float3 Ks = FresnelSchlickRoughness(F0, NdotV, (1.0f - roughness));
    float3 Kd = 1.0f - Ks;
    Kd = Kd * (1.0f - metallic);

    //IBL
    float3 irradiance = g_IrradianceMap.Sample(g_IrradianceSampler, normal).rgb * g_IblIntensity;
    float3 diffuse = irradiance * albedo;

	//Get miplevels
	float2 texelSize;
	float miplevels;
	g_RadianceMap.GetDimensions(0, texelSize.x, texelSize.y, miplevels);

    float3 prefilterColor = SampleEnvironmentMap(reflection, roughness * miplevels);
    float2 brdf = g_IntegrationMap.Sample(g_IntegrationSampler, float2(max(NdotV, 0.0f), 1.0f - roughness)).rg;

    float3 specular = prefilterColor * ((Ks * brdf.x) + brdf.y);   
	return ((Kd * diffuse) + specular) * ao;
}

//Sample shadowmap with PCF
float PCFShadowSampling(float3 normal, float3 lightDir, float2 uv, float currentDepth)
{
	//For UV calculation
	float2 texelsize;
	g_ShadowMap.GetDimensions(texelsize.x, texelsize.y);
	texelsize = 1.0f / texelsize;

	//Depth
	float totalDepth = 0.0f;
	float depthMap = 0.0f;
	//Bias
	float bias = max(MAXBIAS * (1.0f - abs(dot(normal, lightDir))), MINBIAS); 

	//Sample ShadowMap
	for (int y = -KERNELSIZE; y <= KERNELSIZE; y++)
	{
		for (int x = -KERNELSIZE; x <= KERNELSIZE; x++)
		{
			//Sample depthmap
			depthMap = g_ShadowMap.Sample(g_ShadowSampler, uv + (float2(x, y) * texelsize)).r;
			totalDepth += ((currentDepth - bias) < depthMap) ? 1.0f : 0.0f;
		}
	}

	//return depth
	float temp = (KERNELSIZE * 2.0f) + 1.0f;
	return totalDepth / (temp * temp);
}

//Shadow calculation
float ShadowCalculation(float3 position, float3 normal, float3 lightDir)
{
	//Move position to light
	float4 lightProjection = mul(float4(position, 1.0f), g_LightMatrix);
	//Perspective divide
	lightProjection.xyz = lightProjection.xyz / lightProjection.w;

	//to [0, 1] range
	lightProjection.xy = (lightProjection.xy * 0.5f) + 0.5f;

	if (lightProjection.z > 1.0f)
		return 1.0f;

	//Flip the y
	lightProjection.y = (1.0 - lightProjection.y);

	float result = PCFShadowSampling(normal, lightDir, lightProjection.xy, lightProjection.z); 
    return (result > 1.0f) ? 1.0f : result;
}

//Main
float4 main(PS_IN input) : SV_TARGET0
{
	//Sample albedo and check mask
	float4 albedo = g_AlbedoBuffer.Sample(g_AlbedoSampler, input.uv);
	if (albedo.a != 0.0f)
	{
		float4 position = float4(input.uv, 1.0f, 1.0f);
		position.y = 1.0f - position.y;
		position.xy = (position.xy * 2.0f) - 1.0f;
		
		float3 uvw = normalize(mul(position, g_CameraRotation).xyz);
        return float4(SampleEnvironmentMap(uvw, 0), 1.0f);
    }

	//Sample worldposition and metalness
	float4 posMetal = g_PositionBuffer.Sample(g_PositionSampler, input.uv);
	float3 position = posMetal.xyz;
	float metallic = posMetal.a;
	//Sample normal and roughness
	float4 normRough = g_NormalBuffer.Sample(g_NormalSampler, input.uv);
	float3 normal = normalize((normRough.xyz * 2.0f) - 1.0f);
	float roughness = normRough.a;
	//Sample ao
	float ao = g_MaterialBuffer.Sample(g_MaterialSampler, input.uv).r;

	//Fresnel when looking directly at surface
	float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo.rgb, float3(metallic, metallic, metallic));
	//Get view direction
	float3 viewDir = normalize(g_CameraPosition.xyz - position);
	//Dot product normal and view
	float NdotV = max(dot(normal, viewDir), 0.0f);
	//Get reflection
	float3 reflection = reflect(-viewDir, normal);
	//Set ambientColor
	float3 ambient = CalculateIBL(albedo.rgb, F0, reflection, normal, NdotV, roughness, metallic, ao);
	//Set lightdir	
	float3 lightDir = -g_LightDirection.xyz;
	//Check if in shadow
	float shadow = ShadowCalculation(position, normal, lightDir);

	if ((shadow < 0.01f))
		return float4(ambient, 1.0f);

	//Start light
	float3 L0 = float3(0.0f, 0.0f, 0.0f);
	float3 radiance = g_LightColor * g_LightIntensity;

	//Calculate light
	L0 += CalculateLight(normal, viewDir, lightDir, radiance, F0, albedo.rgb, roughness, NdotV, metallic) * shadow;

	//Lightning
	float3 color = ambient + L0;
    return float4(color, 1.0f);
}