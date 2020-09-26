#include "PBRMaterial.h"
#include "Camera.h"
#include "..\Physics\Transform3D.h"
#include "API\ITexture2D.h"
#include "API\ITextureCube.h"
#include "API\IUniformBuffer.h"

#define MATERIALBUFFER 0

#define ALBEDOMAP 0
#define METALLICMAP 1
#define AOMAP 2
#define ROUGHNESSMAP 3
#define NORMALMAP 4
#define DISPLACEMENTMAP 5
#define HEIGHTMAP 6

#define ALBEDOMAP_SLOT_PS 0
#define METALLICMAP_SLOT_PS 4
#define AOMAP_SLOT_PS 3
#define ROUGHNESSMAP_SLOT_PS 2
#define NORMALMAP_SLOT_PS 1
#define HEIGHTMAP_SLOT_PS 5
#define DISPLACEMENTMAP_SLOT_DS 0

#define MATERIALBUFFER_SLOT_PS 0

namespace Graphics
{
	//Material
	PBRMaterial::PBRMaterial(const IDeviceContext* const context)
		: Material(context, 0, 1)
	{
		//Create MaterialBuffer
		CreateBuffer(MATERIALBUFFER, context, nullptr, 0, 1, sizeof(MaterialDataPS));
	}
	
	PBRMaterial::~PBRMaterial()
	{
	}

	MaterialInstance* PBRMaterial::CreateInstance() const
	{
		return new PBRMaterialInstance(this);
	}

	void PBRMaterial::Bind() const
	{
		//Bind pipelinestate
		GetPipelineState()->Bind();

		//Get buffers
		const IUniformBuffer* materialBuffer = GetBuffer(MATERIALBUFFER);

		//Bind buffers
		if (materialBuffer != nullptr)
			materialBuffer->BindPS(MATERIALBUFFER_SLOT_PS);
	}

	void PBRMaterial::Unbind() const
	{
		//Unbind PipelineState
		GetPipelineState()->Unbind();

		//Get buffers
		const IUniformBuffer* materialBuffer = GetBuffer(MATERIALBUFFER);

		//Unbind buffers
		if (materialBuffer != nullptr)
			materialBuffer->UnbindPS();
	}

	void PBRMaterial::SetMaterialProperties(const MaterialDataPS& material) const
	{
		UpdateBuffer(MATERIALBUFFER, &material, 1, 0);
	}

	//Instance
	PBRMaterialInstance::PBRMaterialInstance(const PBRMaterial* const material)
		: MaterialInstance(material, 7)
	{
	}

	PBRMaterialInstance::~PBRMaterialInstance()
	{
	}

	void PBRMaterialInstance::SetAlbedo(const ITexture2D* const albedo)
	{
		SetTexture(albedo, ALBEDOMAP);

		m_PSBuffer.useAlbedoMap = (albedo == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetAlbedo(const ColorF& albedo)
	{
		m_PSBuffer.color = albedo;
	}

	void PBRMaterialInstance::SetMetallic(const ITexture2D* const metallic)
	{
		SetTexture(metallic, METALLICMAP);

		m_PSBuffer.useMetallicMap = (metallic == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetMetallic(float metallic)
	{
		m_PSBuffer.metallic = metallic;
	}

	void PBRMaterialInstance::SetAmbientOcclusion(const ITexture2D* const ao)
	{
		SetTexture(ao, AOMAP);

		m_PSBuffer.useAoMap = (ao == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetAmbientOcclusion(float ao)
	{
		m_PSBuffer.ambientOcclusion = ao;
	}

	void PBRMaterialInstance::SetRoughness(const ITexture2D* const roughness)
	{
		SetTexture(roughness, ROUGHNESSMAP);

		m_PSBuffer.useRoughnessMap = (roughness == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetRoughness(float roughness)
	{
		m_PSBuffer.roughness = roughness;
	}

	void PBRMaterialInstance::SetNormalMap(const ITexture2D* const normal)
	{
		SetTexture(normal, NORMALMAP);

		m_PSBuffer.useNormalMap = (normal == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetDisplacementMap(const ITexture2D* const displacement)
	{
		SetTexture(displacement, DISPLACEMENTMAP);
	}

	void PBRMaterialInstance::SetHeightMap(const ITexture2D* const height)
	{
		SetTexture(height, 6);

		m_PSBuffer.useHeightMap = (height == nullptr) ? 0.0f : 1.0f;
	}

	void PBRMaterialInstance::SetParallaxScale(float scale)
	{
		m_PSBuffer.parallaxScale = scale;
	}

	void PBRMaterialInstance::SetParallaxLayersMin(float layers)
	{
		m_PSBuffer.parallaxLayersMin = layers;
	}

	void PBRMaterialInstance::SetParallaxLayersMax(float layers)
	{
		m_PSBuffer.parallaxLayersMax = layers;
	}

	void PBRMaterialInstance::Bind() const
	{
		//Bind material
		reinterpret_cast<const PBRMaterial*>(GetMaterial())->Bind();
		//Update Materialproperties
		reinterpret_cast<const PBRMaterial*>(GetMaterial())->SetMaterialProperties(m_PSBuffer);

		//Get textures
		const ITexture* albedo = GetTexture(ALBEDOMAP);
		const ITexture* roughness = GetTexture(ROUGHNESSMAP);
		const ITexture* ao = GetTexture(AOMAP);
		const ITexture* metallic = GetTexture(METALLICMAP);
		const ITexture* normal = GetTexture(NORMALMAP);
		const ITexture* height = GetTexture(HEIGHTMAP);
		const ITexture* displacementMap = GetTexture(DISPLACEMENTMAP);

		//Bind textures PS
		if (albedo != nullptr)
			albedo->BindPS(ALBEDOMAP_SLOT_PS);
		if (roughness != nullptr)
			roughness->BindPS(ROUGHNESSMAP_SLOT_PS);
		if (ao != nullptr)
			ao->BindPS(AOMAP_SLOT_PS);
		if (metallic != nullptr)
			metallic->BindPS(METALLICMAP_SLOT_PS);
		if (normal != nullptr)
			normal->BindPS(NORMALMAP_SLOT_PS);
		if (height != nullptr)
			height->BindPS(HEIGHTMAP_SLOT_PS);
		//Bind Textures -
		if (displacementMap != nullptr)
			displacementMap->BindDS(DISPLACEMENTMAP_SLOT_DS);
	}

	void PBRMaterialInstance::Unbind() const
	{
		//Unbind material
		reinterpret_cast<const PBRMaterial*>(GetMaterial())->Unbind();

		//Get textures
		const ITexture* albedo = GetTexture(ALBEDOMAP);
		const ITexture* roughness = GetTexture(ROUGHNESSMAP);
		const ITexture* ao = GetTexture(AOMAP);
		const ITexture* metallic = GetTexture(METALLICMAP);
		const ITexture* normal = GetTexture(NORMALMAP);
		const ITexture* height = GetTexture(HEIGHTMAP);
		const ITexture* displacementMap = GetTexture(DISPLACEMENTMAP);

		//Bind textures PS
		if (albedo != nullptr)
			albedo->UnbindPS();
		if (roughness != nullptr)
			roughness->UnbindPS();
		if (ao != nullptr)
			ao->UnbindPS();
		if (metallic != nullptr)
			metallic->UnbindPS();
		if (normal != nullptr)
			normal->UnbindPS();
		if (height != nullptr)
			height->UnbindPS();
		//Bind Textures - Domain shader
		if (displacementMap != nullptr)
			displacementMap->UnbindDS();
	}
}