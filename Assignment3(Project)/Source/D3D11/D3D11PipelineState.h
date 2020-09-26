#pragma once

#include "..\Core\Graphics\API\IPipelineState.h"
#include "D3D11Shader.h"
#include "D3D11InputLayout.h"

namespace Graphics
{
	//Helper structs to make class more readable
	struct DX11DepthState
	{
		ID3D11DepthStencilState* state = nullptr;
		DepthState desc;
	};

	struct DX11RasterizerState
	{
		ID3D11RasterizerState* state = nullptr;
		RasterizerState desc;
	};


	//Class
	class D3D11PipelineState : public IPipelineState
	{
	public:
		D3D11PipelineState(const D3D11DeviceContext* const context);
		~D3D11PipelineState();

		bool Create() override;

		void Bind() const override;
		void Unbind() const override;

		bool SetVertexShader(const IShader* const shader) override;
		bool SetHullShader(const IShader* const shader) override;
		bool SetDomainShader(const IShader* const shader) override;
		bool SetPixelShader(const IShader* const shader) override;
		bool SetGeometryShader(const IShader* const shader) override;

		void SetInputLayout(const IInputLayout* const inputlayout) override;
		inline const IInputLayout* GetInputLayout() const override
		{
			return m_InputLayout;
		}

		void SetTopology(TOPOLOGY topology) override;
		TOPOLOGY GetTopology() const override;

		bool SetDepthState(const DepthState& state) override;
		
		bool SetRasterizerState(const RasterizerState& state) override;
		
		inline const RasterizerState& GetRasterizerState() const override
		{
			return m_RasterizerState.desc;
		}

		inline const DepthState& GetDepthState() const override
		{
			return m_DepthState.desc;
		}

		inline void SetViewport(const Viewport& viewport) override
		{
			m_Viewport = viewport;
		}
		
		inline const Viewport& GetViewport() const override
		{
			return m_Viewport;
		}

		inline const IShader* GetVertexShader() const override
		{
			return m_VertexShader;
		}

		inline const IShader* GetPixelShader() const override
		{
			return m_PixelShader;
		}

		inline const IShader* GetGeometryShader() const override
		{
			return m_GeometryShader;
		}

		inline const IShader* GetHullShader() const override
		{
			return m_HullShader;
		}

		inline const IShader* GetDomainShader() const override
		{
			return m_DomainShader;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		const D3D11Shader* m_VertexShader;
		const D3D11Shader* m_HullShader;
		const D3D11Shader* m_DomainShader;
		const D3D11Shader* m_GeometryShader;
		const D3D11Shader* m_PixelShader;
		const D3D11InputLayout* m_InputLayout;
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
		Viewport m_Viewport;
		DX11DepthState m_DepthState;
		DX11RasterizerState m_RasterizerState;
	};
}