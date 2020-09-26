#pragma once

#include "IResource.h"

class ColorF;

namespace Graphics
{
#define MAX_RENDERTARGETS 8

	class IRenderTarget2D;
	class IDepthBuffer;
	class IRenderTargetDepth;

	class IFrameBuffer : public IResource
	{
	public:
		virtual ~IFrameBuffer() {}

		virtual bool Create(const IRenderTarget2D* const * const rendertargets, uint32 len) = 0;
		virtual bool AttachRenderTarget(const IRenderTarget2D* const renderTarget, uint32 slot) = 0;
		virtual bool AttachDepthBuffer(const IRenderTargetDepth* const depthBuffer) = 0;
		virtual bool AttachDepthBuffer(const IDepthBuffer* const depthBuffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void ClearColor(const ColorF& color) = 0;
		virtual void ClearDepth(float depth) = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IFrameBuffer* Create(const IDeviceContext* const context, const IRenderTarget2D* const * const renderTargets, uint32 len);
	};
}