#version 440

//PCF-Kernel
#define KERNELSIZE 1
//Shadow-Bias
#define MAXBIAS 0.001f
#define MINBIAS 0.0005f
//PI
#define PI 3.14159265359f
//Gamma factor
#define GAMMA 2.2f

//Pixelshaders has textureslots 0-11 
//G-Buffers
layout(binding = 0) uniform sampler2D g_AlbedoBuffer;
layout(binding = 1) uniform sampler2D g_NormalBuffer;
layout(binding = 2) uniform sampler2D g_PositionBuffer;
layout(binding = 3) uniform sampler2D g_MaterialBuffer;
//ShadowMap
layout(binding = 4) uniform sampler2D g_ShadowMap;
//IntegrationMap - Look up texture for PBR
layout(binding = 5) uniform sampler2D g_IntegrationMap;
//EnvironmentMap
layout(binding = 6) uniform samplerCube g_RadianceMap;
//IrradianceMap
layout(binding = 7) uniform samplerCube g_IrradianceMap;

//Output and input
in VS_OUT
{
	vec2 uv;
} ps_in;

out vec4 g_Color;

//PixelShaders begin at 16 - LightBuffer
layout(binding = 16, std140) uniform LightBuffer
{
	mat4 g_LightMatrix;
	vec3 g_LightDirection;
	float g_LightIntensity;
	vec3 g_LightColor;
	float g_IblIntensity;
};

//CameraBuffer
layout(binding = 17, std140) uniform CameraBuffer
{
	mat4 g_Camera;
	mat4 g_CameraRotation;
	vec4 g_CameraPosition;
};

//Gammacorrection function
vec3 GammaCorrect(vec3 color)
{
	return pow(color, vec3(GAMMA));
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
vec3 FresnelSchlick(vec3 Fo, float cosTheta)
{
    return Fo + (1.0f - Fo) * pow(1.0f - cosTheta, 5.0f);
}

//Fresnel with roughtness - function
vec3 FresnelSchlickRoughness(vec3 Fo, float cosTheta, float glossiness)
{
    return Fo + (max(vec3(glossiness, glossiness, glossiness), Fo) - Fo) * pow(1.0f - cosTheta, 5.0f);
}

//Lightning calculation
vec3 CalculateLight(vec3 normal, vec3 viewDir, vec3 lightDirection, vec3 radiance,
 vec3 F0, vec3 albedo, float roughness, float NdotV, float metallic)
{
	//Halftvector
	vec3 halfVector = normalize(viewDir + lightDirection);
	float NdotL = max(dot(normal, lightDirection), 0.0f);

	//Calculate BRDF
	float NDF = DistrubutionGGX(max(dot(normal, halfVector), 0.0f), roughness);
	float G = GeometrySmith(NdotL, NdotV, roughness);
	//Calculate specular
	vec3 Ks = FresnelSchlick(F0, max(dot(halfVector, viewDir), 0.0f));
    vec3 specular = (NDF * G * Ks) / (4 * NdotV * NdotL + 0.001f);

	//Calculate diffuse;
    vec3 Kd = 1.0f - Ks;
    Kd = Kd * (1.0f - metallic);

	//Return light
	return (((Kd * albedo) / PI) + specular) * radiance * NdotL;
}

//Sample environmentmap //Becuase these arn't in HDR we simulate higher intensities by a constant
vec3 SampleEnvironmentMap(vec3 uvw, float mip)
{
	return textureLod(g_RadianceMap, uvw, mip).rgb * g_IblIntensity;
}

//Image based lightning
vec3 CalculateIBL(vec3 albedo, vec3 F0, vec3 reflection, vec3 normal, float NdotV, float roughness, float metallic, float ao)
{
	//ambient light
    vec3 Ks = FresnelSchlickRoughness(F0, NdotV, 1.0f - roughness);
    vec3 Kd = (1.0f - Ks);
	Kd = Kd * (1.0f - metallic);

    //IBL
    vec3 irradiance = texture(g_IrradianceMap, normal).rgb * g_IblIntensity;
    vec3 diffuse = irradiance * albedo;

	//Get miplevels
	float miplevels = textureQueryLevels(g_RadianceMap);

    vec3 prefilterColor = SampleEnvironmentMap(reflection, roughness * miplevels);
    vec2 brdf = texture(g_IntegrationMap, vec2(max(NdotV, 0.0f), 1.0f - roughness)).rg;

    vec3 specular = prefilterColor * ((Ks * brdf.x) + brdf.y);   
	return ((Kd * diffuse) + specular) * ao;
}

//Sample shadowmap with PCF
float PCFShadowSampling(vec3 normal, vec3 lightDir, vec2 uv, float currentDepth)
{
	//For UV calculation
	vec2 texelsize = vec2(1.0f) / textureSize(g_ShadowMap, 0);

	//Depth
	float totalDepth = 0.0f;
	float depthMap = 0.0f;
	//Bias
	float bias = max(MAXBIAS * (1.0f - abs(dot(normal, lightDir))), MINBIAS);

	//Sample ShadowMap
	for (int x = -KERNELSIZE; x <= KERNELSIZE; x++)
	{
		for (int y = -KERNELSIZE; y <= KERNELSIZE; y++)
		{
			//Sample depthmap
			depthMap = texture(g_ShadowMap, uv + (vec2(x, y) * texelsize)).r;
			totalDepth += ((currentDepth - bias) < depthMap) ? 1.0f : 0.0f;
		}
	}

	//return depth
	float temp = (KERNELSIZE * 2.0f) + 1.0f;
	return totalDepth / (temp * temp);
}

//Shadow calculation
float ShadowCalculation(vec3 position, vec3 normal, vec3 lightDir)
{
	//Move position to light
	vec4 lightProjection = vec4(position, 1.0f) * g_LightMatrix;
	//Perspective divide
	lightProjection.xyz = (lightProjection.xyz / lightProjection.w);

	//to [0, 1] range (DirectX do not need to do the Z coord since it seems to already be in [0, 1] range)
	lightProjection.xyz = (lightProjection.xyz * 0.5f) + 0.5f;
	
	if (lightProjection.z > 1.0f)
		return 1.0f;

	float result = PCFShadowSampling(normal, lightDir, lightProjection.xy, lightProjection.z); 
    return (result > 1.0f) ? 1.0f : result;
}

//Main
void main()
{
	//Sample albedo and check mask
	vec4 albedo = texture(g_AlbedoBuffer, ps_in.uv);
	if (albedo.a != 0.0f)
	{
		vec4 position = vec4(ps_in.uv, 1.0f, 1.0f);
		position.xy = (position.xy * 2.0f) - 1.0f;
		
		vec3 uvw = normalize(position * g_CameraRotation).xyz;
		g_Color = vec4(SampleEnvironmentMap(uvw, 0), 1.0f);
		return;
	}

	//Sample worldposition ant metalness
	vec4 posMetal = texture(g_PositionBuffer, ps_in.uv);
	vec3 position = posMetal.rgb;
	float metallic = posMetal.a;
	//Sample normal and roughness
	vec4 normRough = texture(g_NormalBuffer, ps_in.uv);
	vec3 normal = normalize((normRough.rgb * 2.0f) - 1.0f);
	float roughness = normRough.a;
	//Sample ao
	float ao = texture(g_MaterialBuffer, ps_in.uv).r;


	//Fresnel when looking directly at surface
	vec3 F0 = mix(vec3(0.04f, 0.04f, 0.04f), albedo.rgb, vec3(metallic, metallic, metallic));
	//Get view direction
	vec3 viewDir = normalize(g_CameraPosition.xyz - position);
	//Dot product normal and view
	float NdotV = max(dot(normal, viewDir), 0.0f);
	//Get reflection
	vec3 reflection = reflect(-viewDir, normal);
	//Set ambientColor
	vec3 ambient = CalculateIBL(albedo.rgb, F0, reflection, normal, NdotV, roughness, metallic, ao);
	//Set lightdir	
	vec3 lightDir = -g_LightDirection.xyz;
	//Check if in shadow
	float shadow = ShadowCalculation(position, normal, lightDir);
	if (!(shadow > 0.0f))
	{
		g_Color = vec4(ambient, 1.0f);
		return;
	}

	//Start light
	vec3 L0 = vec3(0.0f, 0.0f, 0.0f);
	vec3 radiance = g_LightColor * g_LightIntensity;
	
	//Calculate light
	L0 += CalculateLight(normal, viewDir, lightDir, radiance, F0, albedo.rgb, roughness, NdotV, metallic) * shadow;

	//Lightning
	vec3 color = ambient + L0;
	g_Color = vec4(color, 1.0f);
}