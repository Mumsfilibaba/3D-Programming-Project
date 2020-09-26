#include "Material.h"
#include "API\IPipelineState.h"
#include "API\IUniformBuffer.h"

namespace Graphics
{
	//Material
	Material::Material(const IDeviceContext* const context, uint32 textureCount, uint32 bufferCount)
		: m_PipelineState(nullptr),
		m_Textures(nullptr),
		m_Buffers(nullptr),
		m_TextureCount(textureCount),
		m_BufferCount(bufferCount)
	{
		//Initialize textures
		m_Textures = new const ITexture*[m_TextureCount];
		for (uint32 i = 0; i < m_TextureCount; i++)
		{
			m_Textures[i] = nullptr;
		}

		//Initialize buffers
		m_Buffers = new IUniformBuffer*[m_BufferCount];
		for (uint32 i = 0; i < m_BufferCount; i++)
		{
			m_Buffers[i] = nullptr;
		}

		//Create pipelinestate
		m_PipelineState = IPipelineState::Create(context);
	}

	Material::~Material()
	{
		Release();
	}

	void Material::SetInputLayout(const IInputLayout* const inputlayout)
	{
		m_PipelineState->SetInputLayout(inputlayout);
	}

	void Material::Release()
	{
		//Release textures
		ReleaseBuffer_S(m_Textures);
		//Release Buffers
		for (uint32 i = 0; i < m_BufferCount; i++)
		{
			ReleaseObject_S(m_Buffers[i]);
		}
		ReleaseBuffer_S(m_Buffers);
		//Release pipelinestate
		ReleaseObject_S(m_PipelineState);
	}

	void Material::CreateBuffer(uint32 buffer, const IDeviceContext* const context, const void* const data, uint32 len, uint32 count, uint32 stride)
	{
		assert(buffer < m_BufferCount);

		m_Buffers[buffer] = IUniformBuffer::Create(context, data, len, stride, count);
	}

	void Material::UpdateBuffer(uint32 buffer, const void * const data, uint32 len, uint32 startIndex) const
	{
		assert(buffer < m_BufferCount);

		m_Buffers[buffer]->Update(data, len, startIndex);
	}

	void Material::SetTexture(const ITexture* const texture, uint32 slot)
	{
		assert(slot < m_TextureCount);

		m_Textures[slot] = texture;
	}

	const ITexture* Material::GetTexture(uint32 slot) const
	{
		assert(slot < m_TextureCount);

		return m_Textures[slot];
	}

	const IUniformBuffer* Material::GetBuffer(uint32 slot) const
	{
		assert(slot < m_BufferCount);

		return m_Buffers[slot];
	}

	void Material::SetVertexShader(const IShader* const shader)
	{
		m_PipelineState->SetVertexShader(shader);
	}

	void Material::SetHullShader(const IShader* const shader)
	{
		m_PipelineState->SetHullShader(shader);
	}

	void Material::SetDomainShader(const IShader* const shader)
	{
		m_PipelineState->SetDomainShader(shader);
	}

	void Material::SetGeomatryShader(const IShader* const shader)
	{
		m_PipelineState->SetGeometryShader(shader);
	}

	void Material::SetPixelShader(const IShader* const shader)
	{
		m_PipelineState->SetPixelShader(shader);
	}

	void Material::SetTopology(TOPOLOGY topology)
	{
		m_PipelineState->SetTopology(topology);
	}

	void Material::SetViewport(Viewport viewport)
	{
		m_PipelineState->SetViewport(viewport);
	}

	void Material::SetRasterizerState(RasterizerState state)
	{
		m_PipelineState->SetRasterizerState(state);
	}

	//Material instance
	MaterialInstance::MaterialInstance(const Material* const material, uint32 textureCount)
		: m_Material(material),
		m_Textures(nullptr),
		m_TextureCount(textureCount)
	{
		//Initialize textures
		m_Textures = new const ITexture*[m_TextureCount];
		for (uint32 i = 0; i < m_TextureCount; i++)
		{
			m_Textures[i] = nullptr;
		}
	}

	MaterialInstance::~MaterialInstance()
	{
		ReleaseBuffer_S(m_Textures);
	}

	void MaterialInstance::SetTexture(const ITexture* const texture, uint8 slot)
	{
		assert(slot < m_TextureCount);

		m_Textures[slot] = texture;
	}

	const ITexture* MaterialInstance::GetTexture(uint8 slot) const
	{
		assert(slot < m_TextureCount);

		return m_Textures[slot];
	}

	const Material* MaterialInstance::GetMaterial() const
	{
		assert(m_Material != nullptr);

		return m_Material;
	}

	const IPipelineState* MaterialInstance::GetPipelineState() const
	{
		assert(m_Material != nullptr);

		return m_Material->GetPipelineState();
	}
}