#pragma once

#include "IShader.h"
#include "IResource.h"

namespace Graphics
{
	struct Viewport;
	struct DepthState;
	struct RasterizerState;
	struct BlendState;

	class IDeviceContext;
	class IInputLayout;

	class IPipelineState : public IResource
	{
	public:
		virtual ~IPipelineState() {}

		virtual bool Create() = 0;

		inline bool IsBound() const
		{
			return this == *(CurrentState());
		}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual bool SetVertexShader(const IShader* const shader) = 0;
		virtual bool SetHullShader(const IShader* const shader) = 0;
		virtual bool SetDomainShader(const IShader* const shader) = 0;
		virtual bool SetGeometryShader(const IShader* const shader) = 0;
		virtual bool SetPixelShader(const IShader* const shader) = 0;

		virtual void SetInputLayout(const IInputLayout* const inputlayout) = 0;
		virtual const IInputLayout* GetInputLayout() const = 0;

		virtual void SetTopology(TOPOLOGY topology) = 0;
		virtual TOPOLOGY GetTopology() const = 0;

		virtual void SetViewport(const Viewport& viewport) = 0;
		virtual const Viewport& GetViewport() const = 0;

		virtual bool SetDepthState(const DepthState& state) = 0;
		virtual const DepthState& GetDepthState() const = 0;

		virtual bool SetRasterizerState(const RasterizerState& state) = 0;
		virtual const RasterizerState& GetRasterizerState() const = 0;

		virtual const IShader* GetVertexShader() const = 0;
		virtual const IShader* GetPixelShader() const = 0;
		virtual const IShader* GetGeometryShader() const = 0;
		virtual const IShader* GetHullShader() const = 0;
		virtual const IShader* GetDomainShader() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IPipelineState* Create(const IDeviceContext* const context);

	private:
		inline static const IPipelineState** CurrentState()
		{
			static const IPipelineState* currentState = nullptr;
			return &currentState;
		}
	};

	inline void IPipelineState::Bind() const
	{
		*(CurrentState()) = this;
	}

	inline void IPipelineState::Unbind() const
	{
		*(CurrentState()) = nullptr;
	}
}