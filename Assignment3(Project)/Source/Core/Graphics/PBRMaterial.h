#pragma once

#include "DirectionalLight.h"
#include "Material.h"
#include "ColorF.h"
#include "..\Math\Matrix4.h"

class Transform3D;

namespace Graphics
{
	class Camera;
	class IShader;
	class ITextureCube;
	class ITexture2D;

	//Material
	class PBRMaterial : public Material
	{
		friend class PBRMaterialInstance;

	private:
		//Materialbuffer
		struct MaterialDataPS
		{
			ColorF color = ColorF::RED;
			float roughness = 1.0f;
			float ambientOcclusion = 1.0f;
			float metallic = 0.0f;
			float parallaxScale = 0.25f;
			float parallaxLayersMin = 8.0f;
			float parallaxLayersMax = 32.0f;
			float useAlbedoMap = 0.0f;
			float useRoughnessMap = 0.0f;
			float useAoMap = 0.0f;
			float useNormalMap = 0.0f;
			float useMetallicMap = 0.0f;
			float useHeightMap = 0.0f;
		};
		
	public:
		PBRMaterial(const IDeviceContext* const context);
		~PBRMaterial();

		MaterialInstance* CreateInstance() const override;

	protected:
		void Bind() const;
		void Unbind() const;

		void SetMaterialProperties(const MaterialDataPS& material) const;
	};


	//MaterialInstance
	class PBRMaterialInstance : public MaterialInstance
	{
		friend class PBRMaterial;
	
	public:
		~PBRMaterialInstance();

		void Bind() const override;
		void Unbind() const override;

		//PixelShader
		void SetAlbedo(const ITexture2D* const albedo);
		void SetAlbedo(const ColorF& albedo);
		void SetMetallic(const ITexture2D* const metallic);
		void SetMetallic(float metallic);
		void SetAmbientOcclusion(const ITexture2D* const ao);
		void SetAmbientOcclusion(float ao);
		void SetRoughness(const ITexture2D* const roughness);
		void SetRoughness(float roughness);
		void SetNormalMap(const ITexture2D* const normal);
		void SetHeightMap(const ITexture2D* const height);		
		void SetParallaxScale(float scale);
		void SetParallaxLayersMin(float layers);
		void SetParallaxLayersMax(float layers);
		//Tesselation
		void SetDisplacementMap(const ITexture2D* const dicplacement);

	protected:
		PBRMaterialInstance(const PBRMaterial* const material);

	private:
		PBRMaterial::MaterialDataPS m_PSBuffer;
	};
}