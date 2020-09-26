#include <cassert>
#include "..\Core\Graphics\ColorF.h"
#include "D3D11FrameBuffer.h"

namespace Graphics
{
	D3D11FrameBuffer::D3D11FrameBuffer(const D3D11DeviceContext* const context, const IRenderTarget2D* const * const renderTargets,
		uint32 len)
		: m_Context(context),
		m_DepthBuffer(nullptr),
		m_RenderTargets(),
		m_Count(0)
	{
		Release();

		Create(renderTargets, len);
	}

	D3D11FrameBuffer::~D3D11FrameBuffer()
	{
		Release();
	}

	bool D3D11FrameBuffer::Create(const IRenderTarget2D* const * const rendertargets, uint32 len)
	{
		if (rendertargets == nullptr && len > 0)
			return false;

		//Attach renderTargets
		if (rendertargets != nullptr)
		{
			for (uint32 i = 0; i < len; i++)
				AttachRenderTarget(rendertargets[i], i);
		}

		return true;
	}

	bool D3D11FrameBuffer::AttachRenderTarget(const IRenderTarget2D* const renderTarget, uint32 slot)
	{
		assert(slot < MAX_RENDERTARGETS);

		//Set rendertarget
		if (renderTarget != nullptr)
		{
			const D3D11RenderTarget2D* target = nullptr;
			target = reinterpret_cast<const D3D11RenderTarget2D*>(renderTarget);
			m_RenderTargets[slot] = target->GetDXView();
		}
		else
		{
			m_RenderTargets[slot] = nullptr;
		}

		//Count up
		if (renderTarget != nullptr)
		{
			if (slot >= m_Count)
				m_Count = (slot + 1);
		}
		else
		{
			if ((slot + 1) == m_Count)
				m_Count--;
		}

		return true;
	}

	bool D3D11FrameBuffer::AttachDepthBuffer(const IDepthBuffer* const depthBuffer)
	{
		if (depthBuffer != nullptr)
		{
			const D3D11DepthBuffer* buffer = reinterpret_cast<const D3D11DepthBuffer*>(depthBuffer);
			m_DepthBuffer = buffer->GetDXView();
		}
		else
		{
			m_DepthBuffer = nullptr;
		}

		return true;
	}

	bool D3D11FrameBuffer::AttachDepthBuffer(const IRenderTargetDepth* const depthBuffer)
	{
		if (depthBuffer != nullptr)
		{
			const D3D11RenderTargetDepth* buffer = reinterpret_cast<const D3D11RenderTargetDepth*>(depthBuffer);
			m_DepthBuffer = buffer->GetDXView();
		}
		else
		{
			m_DepthBuffer = nullptr;
		}

		return true;
	}

	void D3D11FrameBuffer::Bind() const
	{
		assert(m_Context != nullptr);

		m_Context->GetContext()->OMSetRenderTargets(m_Count, m_RenderTargets, m_DepthBuffer);
	}

	void D3D11FrameBuffer::Unbind() const
	{
		assert(m_Context != nullptr);

		//TODO: Not very optimized
		ID3D11RenderTargetView* renderTargets[8] =
		{
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr
		};

		m_Context->GetContext()->OMSetRenderTargets(m_Count, renderTargets, nullptr);
	}

	void D3D11FrameBuffer::ClearColor(const ColorF & color)
	{
		assert(m_Context != nullptr);

		for (uint32 i = 0; i < m_Count; i++)
		{
			if (m_RenderTargets[i] != nullptr)
				m_Context->GetContext()->ClearRenderTargetView(m_RenderTargets[i], color);
		}
	}

	void D3D11FrameBuffer::ClearDepth(float depth)
	{
		assert(m_Context != nullptr);

		if (m_DepthBuffer != nullptr)
			m_Context->GetContext()->ClearDepthStencilView(m_DepthBuffer, D3D11_CLEAR_DEPTH, depth, 0);
	}

	void D3D11FrameBuffer::Release()
	{
		for (int32 i = 0; i < MAX_RENDERTARGETS; i++)
			m_RenderTargets[i] = nullptr;

		m_DepthBuffer = nullptr;
	}
}