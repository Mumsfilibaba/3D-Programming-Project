#include <iostream>
#include <cassert>
#include "..\Core\Utilities\StringHelper.h"
#include "D3D11PipelineState.h"

namespace Graphics
{
	D3D11PipelineState::D3D11PipelineState(const D3D11DeviceContext* const context)
		: m_Context(context),
		m_VertexShader(nullptr),
		m_HullShader(nullptr),
		m_DomainShader(nullptr),
		m_GeometryShader(nullptr),
		m_PixelShader(nullptr),
		m_InputLayout(nullptr),
		m_Viewport()
	{
		Create();
	}

	D3D11PipelineState::~D3D11PipelineState()
	{
		Release();
	}

	void D3D11PipelineState::Release()
	{
		//States
		D3DRelease_S(m_DepthState.state);
		D3DRelease_S(m_RasterizerState.state);
	}

	bool D3D11PipelineState::Create()
	{
		if (!SetRasterizerState(RasterizerState()))
			return false;

		return SetDepthState(DepthState());
	}

	void D3D11PipelineState::Bind() const
	{
		assert(m_Context != nullptr);

		if (IsBound())
			return;

		//Input Assembler
		m_Context->GetContext()->IASetPrimitiveTopology(m_Topology);
		if (m_InputLayout != nullptr)
			m_Context->GetContext()->IASetInputLayout(m_InputLayout->GetLayoutPtr());
		else
			m_Context->GetContext()->IASetInputLayout(nullptr);
		//Shaders
		if (m_VertexShader != nullptr)
			m_Context->GetContext()->VSSetShader(m_VertexShader->GetShader<ID3D11VertexShader>(), nullptr, 0);
		else
			m_Context->GetContext()->VSSetShader(nullptr, nullptr, 0);
		if (m_HullShader != nullptr)
			m_Context->GetContext()->HSSetShader(m_HullShader->GetShader<ID3D11HullShader>(), nullptr, 0);
		else
			m_Context->GetContext()->HSSetShader(nullptr, nullptr, 0);
		if (m_DomainShader != nullptr)
			m_Context->GetContext()->DSSetShader(m_DomainShader->GetShader<ID3D11DomainShader>(), nullptr, 0);
		else
			m_Context->GetContext()->DSSetShader(nullptr, nullptr, 0);
		if (m_GeometryShader != nullptr)
			m_Context->GetContext()->GSSetShader(m_GeometryShader->GetShader<ID3D11GeometryShader>(), nullptr, 0);
		else
			m_Context->GetContext()->GSSetShader(nullptr, nullptr, 0);
		if (m_PixelShader != nullptr)
			m_Context->GetContext()->PSSetShader(m_PixelShader->GetShader<ID3D11PixelShader>(), nullptr, 0);
		else
			m_Context->GetContext()->PSSetShader(nullptr, nullptr, 0);

		//Viewport
		//TODO: Maybe not optimal
		D3D11_VIEWPORT viewport;
		viewport.Width = (float)m_Viewport.width;
		viewport.Height = (float)m_Viewport.height;
		viewport.TopLeftX = (float)m_Viewport.x;
		viewport.TopLeftY = (float)m_Viewport.y;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;

		m_Context->GetContext()->RSSetViewports(1, &viewport);

		//States
		m_Context->GetContext()->OMSetDepthStencilState(m_DepthState.state, 0);
		m_Context->GetContext()->RSSetState(m_RasterizerState.state);

		//Call pipelinestate to set currentstate
		IPipelineState::Bind();
	}

	void D3D11PipelineState::Unbind() const
	{
		assert(m_Context != nullptr);

		if (!IsBound())
			return;

		//Input Assembler
		m_Context->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
		m_Context->GetContext()->IASetInputLayout(nullptr);
		//Shaders
		m_Context->GetContext()->VSSetShader(nullptr, nullptr, 0);
		m_Context->GetContext()->HSSetShader(nullptr, nullptr, 0);
		m_Context->GetContext()->DSSetShader(nullptr, nullptr, 0);
		m_Context->GetContext()->GSSetShader(nullptr, nullptr, 0);
		m_Context->GetContext()->PSSetShader(nullptr, nullptr, 0);
		//Viewport
		m_Context->GetContext()->RSSetViewports(0, nullptr);
		//States
		m_Context->GetContext()->OMSetDepthStencilState(nullptr, 0);
		m_Context->GetContext()->RSSetState(nullptr);

		//Call pipelinestate to set currentstate
		IPipelineState::Unbind();
	}

	bool D3D11PipelineState::SetVertexShader(const IShader* const shader)
	{
		m_VertexShader = reinterpret_cast<const D3D11Shader*>(shader);
		return true;
	}
	
	bool D3D11PipelineState::SetHullShader(const IShader* const shader)
	{
		m_HullShader = reinterpret_cast<const D3D11Shader*>(shader);
		return true;
	}

	bool D3D11PipelineState::SetDomainShader(const IShader* const shader)
	{
		m_DomainShader = reinterpret_cast<const D3D11Shader*>(shader);
		return true;
	}

	bool D3D11PipelineState::SetGeometryShader(const IShader* const shader)
	{
		m_GeometryShader = reinterpret_cast<const D3D11Shader*>(shader);
		return true;
	}

	void D3D11PipelineState::SetInputLayout(const IInputLayout* const inputlayout)
	{
		m_InputLayout = reinterpret_cast<const D3D11InputLayout*>(inputlayout);
	}

	bool D3D11PipelineState::SetPixelShader(const IShader* const shader)
	{
		m_PixelShader = reinterpret_cast<const D3D11Shader*>(shader);
		return true;
	}

	void D3D11PipelineState::SetTopology(TOPOLOGY topology)
	{
		switch (topology)
		{
		case TOPOLOGY_TRIANGLELIST: m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
		case TOPOLOGY_TRIANGLESTRIP: m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
		case TOPOLOGY_PATCH_3_POINTS: m_Topology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST; break;
		case TOPOLOGY_PATCH_4_POINTS: m_Topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST; break;
		default: m_Topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED; break;
		}
	}

	TOPOLOGY D3D11PipelineState::GetTopology() const
	{
		switch (m_Topology)
		{
		case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST: return TOPOLOGY_TRIANGLELIST;
		case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: return TOPOLOGY_TRIANGLESTRIP;
		case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST: return TOPOLOGY_PATCH_3_POINTS;
		case D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST: return TOPOLOGY_PATCH_4_POINTS;
		default: return TOPOLOGY_UNKNOWN;
		}
	}

	bool D3D11PipelineState::SetDepthState(const DepthState& state)
	{
		if (m_DepthState.state != nullptr)
		{
			D3DRelease(m_DepthState.state);
		}

		m_DepthState.desc = state;

		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = state.depthTest;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.StencilEnable = false;
		desc.DepthFunc = d3d_convert_comparison_dunc(m_DepthState.desc.comparisonFunc);
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace = desc.BackFace;

		if (FAILED(m_Context->GetDevice()->CreateDepthStencilState(&desc, &m_DepthState.state)))
		{
			std::cout << "ERROR: COULD NOT CREATE DEPTHSTATE\n";
			return false;
		}

		return true;
	}

	bool D3D11PipelineState::SetRasterizerState(const RasterizerState& state)
	{
		assert(m_Context->GetDevice());

		if (m_RasterizerState.state != nullptr)
		{
			D3DRelease(m_RasterizerState.state);
		}

		m_RasterizerState.desc = state;

		D3D11_RASTERIZER_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_RASTERIZER_DESC));
		if (state.cull == CULLMODE_NONE)
			desc.CullMode = D3D11_CULL_NONE;
		else if (state.cull == CULLMODE_FRONT)
			desc.CullMode = D3D11_CULL_FRONT;
		else if (state.cull == CULLMODE_BACK)
			desc.CullMode = D3D11_CULL_BACK;

		desc.FillMode = (state.wireframe) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;
		desc.ScissorEnable = state.scissor;
		desc.SlopeScaledDepthBias = state.slopeDepthBias;
		desc.DepthClipEnable = state.depthClip;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = state.depthBias;
		desc.DepthBiasClamp = state.depthClamp;

		if (FAILED(m_Context->GetDevice()->CreateRasterizerState(&desc, &m_RasterizerState.state)))
		{
			std::cout << "ERROR: Could not set RasterizerState\n";
			return false;
		}

		return true;
	}
}