#include "..\Core\Graphics\ColorF.h"
#include "GLFrameBuffer.h"

namespace Graphics
{
	GLFrameBuffer::GLFrameBuffer(const GLDeviceContext* const context, const IRenderTarget2D* const * const rendertargets, uint32 len)
		: m_Context(context),
		m_FrameBuffer(0),
		m_ClearBits(0),
		m_Count(0),
		m_Attachments()
	{
		Create(rendertargets, len);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		Release();
	}

	bool GLFrameBuffer::Create(const IRenderTarget2D* const * const rendertargets, uint32 len)
	{
		GL_Call(glGenFramebuffers(1, &m_FrameBuffer));
		
		//TODO: Optimze so that there are not that many unnecessary bind-calls

		//Attachrendertargets
		if (rendertargets != nullptr)
		{
			for (uint32 i = 0; i < len; i++)
			{
				if (!AttachRenderTarget(rendertargets[i], i))
					return false;
			}
		}

		return true;
	}

	bool GLFrameBuffer::AttachRenderTarget(const IRenderTarget2D* const renderTarget, uint32 slot)
	{
		assert(slot < MAX_RENDERTARGETS);

		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		//Find slot
		uint32 attachmentSlot = GetColorAttachmentSlot(slot);
		int32 index = IsSlotUsed(attachmentSlot);

		if (renderTarget != nullptr)
		{
			//Texture ID
			const GLTexture2D* texture = reinterpret_cast<const GLTexture2D*>(renderTarget->GetTexture());
			uint32 texID = texture->GetGLTexture();

			//Bind texture to frambuffer
			GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentSlot, GL_TEXTURE_2D, texID, 0));

			//Check if an add is necessary
			if (index < 0)
			{
				m_ClearBits |= GL_COLOR_BUFFER_BIT;
				m_Attachments[m_Count] = attachmentSlot;
				m_Count++;
			}
		}
		else
		{
			if (index >= 0)
			{
				//Unbind texture to frambuffer
				GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentSlot, GL_TEXTURE_2D, 0, 0));
				
				RemoveFromAttachments(index);
			}
		}

		//Check if complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return true;
	}

	bool GLFrameBuffer::AttachDepthBuffer(const IDepthBuffer* const depthBuffer)
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		if (depthBuffer != nullptr)
		{
			//Texture ID
			const GLDepthBuffer* buffer = reinterpret_cast<const GLDepthBuffer*>(depthBuffer);
			uint32 bufferID = buffer->GetGLBuffer();

			//Bind texture to frambuffer
			GL_Call(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bufferID));

			m_ClearBits |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			//Unbind texture to frambuffer
			GL_Call(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));

			m_ClearBits &= ~GL_DEPTH_BUFFER_BIT;
		}

		//Check if complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return true;
	}

	bool GLFrameBuffer::AttachDepthBuffer(const IRenderTargetDepth* const depthBuffer)
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		if (depthBuffer != nullptr)
		{
			//Texture ID
			const GLTexture2D* texture = reinterpret_cast<const GLTexture2D*>(depthBuffer->GetTexture());
			uint32 bufferID = texture->GetGLTexture();

			//Bind texture to frambuffer
			GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferID, 0));

			m_ClearBits |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			//Unbind texture to frambuffer
			GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0));

			m_ClearBits &= ~GL_DEPTH_BUFFER_BIT;
		}

		//Check if complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return true;
	}

	void GLFrameBuffer::Bind() const
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
		GL_Call(glDrawBuffers(m_Count, m_Attachments));
	}

	void GLFrameBuffer::Unbind() const
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFrameBuffer::ClearColor(const ColorF & color)
	{
		GL_Call(glClearColor(color.r, color.g, color.b, color.a));
		GL_Call(glClear(GL_COLOR_BUFFER_BIT));
	}

	void GLFrameBuffer::ClearDepth(float depth)
	{
		GL_Call(glClearDepthf(depth));
		GL_Call(glClear(GL_DEPTH_BUFFER_BIT));
	}

	void GLFrameBuffer::Release()
	{
		if (glIsFramebuffer(m_FrameBuffer))
		{
			GL_Call(glDeleteFramebuffers(1, &m_FrameBuffer));
		}
	}

	uint32 GLFrameBuffer::GetColorAttachmentSlot(uint32 slot) const
	{
		//DirectX has supprort for 8 slots
		switch (slot)
		{
		case 0: return GL_COLOR_ATTACHMENT0;
		case 1: return GL_COLOR_ATTACHMENT1;
		case 2: return GL_COLOR_ATTACHMENT2;
		case 3: return GL_COLOR_ATTACHMENT3;
		case 4: return GL_COLOR_ATTACHMENT4;
		case 5: return GL_COLOR_ATTACHMENT5;
		case 6: return GL_COLOR_ATTACHMENT6;
		case 7: return GL_COLOR_ATTACHMENT7;
		default: return 0;
		}
	}

	int32 GLFrameBuffer::IsSlotUsed(uint32 attachmentSlot) const
	{
		for (uint32 i = 0; i < m_Count; i++)
		{
			if (m_Attachments[i] == attachmentSlot)
				return i;
		}

		return -1;
	}

	void GLFrameBuffer::RemoveFromAttachments(uint32 index)
	{
		if (index < MAX_RENDERTARGETS)
			memcpy((m_Attachments + index), (m_Attachments + index + 1), sizeof(uint32) * ((MAX_RENDERTARGETS + 1) - (index + 1)));
		else
			m_Attachments[index] = 0;

		m_Count--;
	}
}