#include <iostream>
#include "..\Core\Utilities\Utilities.h"
#include "D3D11InputLayout.h"
#include <d3dcompiler.h>

namespace Graphics
{
	D3D11InputLayout::D3D11InputLayout(const D3D11DeviceContext* context, const Attribute* const attributes, uint32 len)
		: m_Context(context),
		m_Layout(nullptr),
		m_Attributes(nullptr),
		m_Count(0),
		m_Stride(0)
	{
		Create(attributes, len);
	}

	D3D11InputLayout::~D3D11InputLayout()
	{
		Release();
	}

	bool D3D11InputLayout::Create(const Attribute* const attributes, uint32 len)
	{
		assert(m_Context != nullptr);

		if (attributes == nullptr)
			return false;

		//Copy attributes
		m_Attributes = new Attribute[len];

		//Create dummyshader
		std::string shader =
			"struct VS_IN\n"
			"{\n";

		//Fill desc
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[len];
		for (uint32 i = 0; i < len; i++)
		{
			//Copy attribute
			m_Attributes[i] = attributes[i];

			//Set sematic and index
			desc[i].SemanticName = m_Attributes[i].semantic.c_str();
			desc[i].SemanticIndex = m_Attributes[i].index;

			//Set format
			switch (m_Attributes[i].type)
			{
			case ATTRIBUTETYPE_FLOAT:
				desc[i].Format = DXGI_FORMAT_R32_FLOAT;
				shader += "	float _" + std::to_string(i) + " : " + attributes[i].semantic + std::to_string(attributes[i].index) + ";\n";
				break;
			case ATTRIBUTETYPE_VECTOR2F:
				desc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
				shader += "	float2 _" + std::to_string(i) + " : " + attributes[i].semantic + std::to_string(attributes[i].index) + ";\n";
				break;
			case ATTRIBUTETYPE_VECTOR3F:
				desc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				shader += "	float3 _" + std::to_string(i) + " : " + attributes[i].semantic + std::to_string(attributes[i].index) + ";\n";
				break;
			case ATTRIBUTETYPE_VECTOR4F:
				desc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				shader += "	float4 _" + std::to_string(i) + " : " + attributes[i].semantic + std::to_string(attributes[i].index) + ";\n";
				break;
			default: desc[i].Format = DXGI_FORMAT_UNKNOWN;
			}

			//VertexBuffer slot
			desc[i].InputSlot = 0;
			//Offset
			desc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			//Instanced- or vertexdata
			desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			//Instance count before update shaderattribute
			desc[i].InstanceDataStepRate = 0;

			//Calculate stride
			m_Stride += get_attribute_stride(m_Attributes[i].type);
		}

		//Finish dummy shader code
		shader +=
			"};\n\n"
			"float4 main(VS_IN input) : SV_POSITION0\n"
			"{\n"
			"	return float4(0.0f, 0.0f, 0.0f, 0.0f);\n"
			"}\n";

		//Compile shader
		ID3DBlob* error = nullptr;
		ID3DBlob* blob = nullptr;
		if (FAILED(D3DCompile(shader.c_str(), shader.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_SKIP_OPTIMIZATION, 0, &blob, &error)))
		{
			//Output error message if it fails
			std::cout << "ERROR: Could not compile dummyshader" << (const char*)error->GetBufferPointer() << '\n';

			D3DRelease_S(error);
			ReleaseBuffer_S(desc);
			return false;
		}

		//Create inputlayout
		if (FAILED(m_Context->GetDevice()->CreateInputLayout(desc, len,	blob->GetBufferPointer(), blob->GetBufferSize(), &m_Layout)))
		{
			std::cout << "ERROR: Could not create InputLayout\n";

			D3DRelease_S(blob);
			ReleaseBuffer_S(desc);
			return false;
		}

		D3DRelease_S(blob);
		ReleaseBuffer_S(desc);
		return true;
	}

	void D3D11InputLayout::GetAttribute(Attribute & attribute, uint32 index) const
	{
		assert(index < m_Count);

		attribute = m_Attributes[index];
	}

	void D3D11InputLayout::Release()
	{
		D3DRelease_S(m_Layout);
		ReleaseBuffer_S(m_Attributes);
	}
}