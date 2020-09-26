#pragma once

#include "..\Core\Graphics\API\IFrameBuffer.h"
#include "D3D11RenderTarget2D.h"
#include "D3D11DepthBuffer.h"
#include "D3D11RenderTargetDepth.h"

namespace Graphics
{
	class D3D11FrameBuffer : public IFrameBuffer
	{
	public:
		D3D11FrameBuffer(const D3D11DeviceContext* const context, const IRenderTarget2D* const * const renderTargets, uint32 len);
		~D3D11FrameBuffer();

		bool Create(const IRenderTarget2D* const * const rendertargets, uint32 len) override;
		bool AttachRenderTarget(const IRenderTarget2D* const renderTarget, uint32 slot) override;
		bool AttachDepthBuffer(const IDepthBuffer* const depthBuffer) override;
		bool AttachDepthBuffer(const IRenderTargetDepth* const depthBuffer) override;

		void Bind() const override;
		void Unbind() const override;
		void ClearColor(const ColorF& color) override;
		void ClearDepth(float depth) override;

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		ID3D11RenderTargetView* m_RenderTargets[MAX_RENDERTARGETS];
		ID3D11DepthStencilView* m_DepthBuffer;
		uint32 m_Count;
	};
}