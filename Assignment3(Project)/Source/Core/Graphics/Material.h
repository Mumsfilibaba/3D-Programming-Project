#pragma once

#include "..\..\Defines.h"
#include "API\IPipelineState.h"

namespace Graphics
{
	class Material;
	class ITexture;
	class IUniformBuffer;

	//MaterialInstance
	class MaterialInstance
	{
	public:
		~MaterialInstance();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		const IPipelineState* GetPipelineState() const;
		const Material* GetMaterial() const;

		inline uint32 GetTextureCount() const
		{
			return m_TextureCount;
		}

	protected:
		MaterialInstance(const Material* const material, uint32 textureCount);
		
		void SetTexture(const ITexture* const texture, uint8 slot);
		const ITexture* GetTexture(uint8 slot) const;

	private:
		const Material* m_Material;
		const ITexture** m_Textures;
		uint32 m_TextureCount;
	};

	//Material
	class Material
	{
		friend class MaterialInstance;

	public:
		Material(const IDeviceContext* const context, uint32 textureCount, uint32 bufferCount);
		~Material();

		void SetInputLayout(const IInputLayout* const inputlayout);
		
		void SetTopology(TOPOLOGY topology);

		void SetVertexShader(const IShader* const shader);
		void SetHullShader(const IShader* const shader);
		void SetDomainShader(const IShader* const shader);
		void SetGeomatryShader(const IShader* const shader);
		void SetPixelShader(const IShader* const shader);

		void SetViewport(Viewport viewport);

		void SetRasterizerState(RasterizerState state);

		virtual MaterialInstance* CreateInstance() const = 0;
		
		inline const IPipelineState* GetPipelineState() const
		{
			return m_PipelineState;
		}

		void Release();

	protected:
		void CreateBuffer(uint32 buffer, const IDeviceContext* const context, const void* const data, uint32 len, uint32 count, uint32 stride);
		void UpdateBuffer(uint32 buffer, const void* const data, uint32 len, uint32 startIndex) const;

		void SetTexture(const ITexture* const texture, uint32 slot);

		const ITexture* GetTexture(uint32 slot) const;
		const IUniformBuffer* GetBuffer(uint32 slot) const;

	private:
		IPipelineState* m_PipelineState;
		mutable IUniformBuffer** m_Buffers;
		const ITexture** m_Textures;
		uint32 m_TextureCount;
		uint32 m_BufferCount;
	};
}