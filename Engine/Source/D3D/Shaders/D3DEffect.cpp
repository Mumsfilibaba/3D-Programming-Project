#include "..\D3DContext.h"
#include "D3DEffect.h"
#include "D3DShader.h"
#include <d3dcompiler.h>

namespace Graphics
{
	D3DEffect::D3DEffect(const D3DContext& context, const D3DShader* const vertex, const D3DShader* const pixel,
		const D3DShader* const geometry)
		: m_Context(context),
		m_Layout(nullptr),
		m_VertexShader(nullptr),
		m_PixelShader(nullptr),
		m_GeometryShader(nullptr),
		m_Vertex(nullptr),
		m_Pixel(nullptr),
		m_Geometry(nullptr)
	{
		SetVertexShader(vertex);
		SetPixelShader(vertex);
		SetGeometryShader(vertex);
	}

	D3DEffect::~D3DEffect()
	{
		Release();
	}


	void D3DEffect::Bind() const
	{
		ID3D11DeviceContext* context = m_Context.GetDeviceContext();

		context->IASetInputLayout(m_Layout);
		context->VSSetShader(m_VertexShader, nullptr, 0);
		context->PSSetShader(m_PixelShader, nullptr, 0);
		context->GSSetShader(m_GeometryShader, nullptr, 0);
	}

	void D3DEffect::Unbind() const
	{
		ID3D11DeviceContext* context = m_Context.GetDeviceContext();

		context->IASetInputLayout(nullptr);
		context->VSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
		context->GSSetShader(nullptr, nullptr, 0);
	}


	bool D3DEffect::SetVertexShader(const Shader* const shader)
	{
		D3DRelease_S(m_VertexShader);
		ReleaseObject_S(m_Vertex);

		if (shader != nullptr)
		{
			m_Vertex = const_cast<D3DShader*>(reinterpret_cast<const D3DShader*>(shader));

			if (CreateVertexShader())
				return false;
		}

		return true;
	}

	bool D3DEffect::SetPixelShader(const Shader * const shader)
	{
		D3DRelease_S(m_PixelShader);
		ReleaseObject_S(m_Pixel);

		if (shader != nullptr)
		{
			m_Pixel = const_cast<D3DShader*>(reinterpret_cast<const D3DShader*>(shader));

			if (CreatePixelShader())
				return false;
		}

		return true;
	}

	bool D3DEffect::SetGeometryShader(const Shader * const shader)
	{
		D3DRelease_S(m_GeometryShader);
		ReleaseObject_S(m_Geometry);

		if (shader != nullptr)
		{
			m_Geometry = const_cast<D3DShader*>(reinterpret_cast<const D3DShader*>(shader));

			if (CreateGeometryShader())
				return false;
		}

		return true;
	}


	void D3DEffect::VSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
	}

	void D3DEffect::PSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
	}

	void D3DEffect::GSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
	}


	const UniformBuffer& D3DEffect::VSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
		return UniformBuffer();
	}

	const UniformBuffer& D3DEffect::PSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
		return UniformBuffer();
	}

	const UniformBuffer& D3DEffect::GSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const
	{
		return UniformBuffer();
	}


	void D3DEffect::Release()
	{
		D3DRelease_S(m_Layout);
		D3DRelease_S(m_VertexShader);
		D3DRelease_S(m_PixelShader);
		D3DRelease_S(m_GeometryShader);

		ReleaseObject_S(m_Vertex);
		ReleaseObject_S(m_Pixel);
		ReleaseObject_S(m_Geometry);
	}


	bool D3DEffect::CreateVertexShader()
	{
		return false;
	}
	bool D3DEffect::CreatePixelShader()
	{
		return false;
	}
	bool D3DEffect::CreateGeometryShader()
	{
		return false;
	}


	bool D3DEffect::CreateInputLayoutFromShader()
	{
		ID3D11ShaderReflection* shader = nullptr;

		//Reflect shader (Get shader information)
		if (SUCCEEDED(D3DReflect(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)&shader)))
		{
			D3D11_SHADER_DESC sDesc;
			ZeroBlock(sizeof(D3D11_SHADER_DESC), &sDesc);

			if (SUCCEEDED(shader->GetDesc(&sDesc)))
			{
				D3D11_SIGNATURE_PARAMETER_DESC pDesc;
				ZeroBlock(sizeof(D3D11_SIGNATURE_PARAMETER_DESC), &pDesc);

				D3D11_INPUT_ELEMENT_DESC eDesc;
				ZeroBlock(sizeof(D3D11_INPUT_ELEMENT_DESC), &eDesc);


				std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

				for (uint32 i = 0; i < sDesc.InputParameters; i++)
				{
					shader->GetInputParameterDesc(i, &pDesc);

					//Set element-desc
					eDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
					eDesc.SemanticIndex = pDesc.SemanticIndex;
					eDesc.SemanticName = pDesc.SemanticName;
					eDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;


					//TODO: Handle instancing


					if (pDesc.Mask & COMPONENT_COUNT_4)
					{
						if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					}
					else if (pDesc.Mask & COMPONENT_COUNT_3)
					{
						if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
							eDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
					}
					else if (pDesc.Mask & COMPONENT_COUNT_2)
					{
						if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
							eDesc.Format = DXGI_FORMAT_R32G32_UINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
							eDesc.Format = DXGI_FORMAT_R32G32_SINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
							eDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
					}
					else if (pDesc.Mask & COMPONENT_COUNT_1)
					{
						if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
							eDesc.Format = DXGI_FORMAT_R32_UINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
							eDesc.Format = DXGI_FORMAT_R32_SINT;
						else if (pDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
							eDesc.Format = DXGI_FORMAT_R32_FLOAT;
					}


					elements.push_back(eDesc);
				}


				if (FAILED(m_Context.GetDevice()->CreateInputLayout(elements.data(), (UINT)elements.size(),
					vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_Layout)))
				{
					std::cout << "Could not create InputLayout";

					D3DRelease_S(shader);
					return false;
				}
			}
		}
	}
}