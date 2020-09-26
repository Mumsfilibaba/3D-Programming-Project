#pragma once

#include "..\Core\Graphics\API\IFrameBuffer.h"
#include "GLRenderTarget2D.h"
#include "GLRenderTargetDepth.h"
#include "GLDepthBuffer.h"

namespace Graphics
{
	class GLFrameBuffer : public IFrameBuffer
	{
	public:
		GLFrameBuffer(const GLDeviceContext* const context, const IRenderTarget2D* const * const rendertargets, uint32 len);
		~GLFrameBuffer();

		bool Create(const IRenderTarget2D* const * const rendertargets, uint32 len);
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
		uint32 GetColorAttachmentSlot(uint32 slot) const;
		int32 IsSlotUsed(uint32 attachmentSlot) const;
		void RemoveFromAttachments(uint32 index);

	private:
		const GLDeviceContext* m_Context;
		uint32 m_FrameBuffer;
		uint32 m_ClearBits;
		uint32 m_Count;
		uint32 m_Attachments[MAX_RENDERTARGETS + 1]; //8 RenderTargets + DepthBuffer
	};
}