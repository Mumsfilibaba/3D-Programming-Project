#include <iostream>
#include "..\Core\Window.h"
#include "..\Core\Graphics\ColorF.h"
#include "GLDeviceContext.h"
#include "GLPipelineState.h"

namespace Graphics
{
	GLDeviceContext::GLDeviceContext(const Window& window, const RenderSettings& manager)
		: m_Window(nullptr),
		m_Manager(&manager),
		m_SystemContext(nullptr),
		m_GLContext(nullptr),
		m_TargetSize(window.GetSize().x, window.GetSize().y),
		m_Viewport()
	{
		Create(window);
	}

	GLDeviceContext::~GLDeviceContext()
	{
		Release();
	}

	void GLDeviceContext::MakeWindowTarget() const
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLDeviceContext::ClearWindow(const ColorF& color) const
	{
		GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GL_Call(glClearColor(color.r, color.g, color.b, color.a));
		GL_Call(glClear(GL_COLOR_BUFFER_BIT));
	}

	void GLDeviceContext::Draw(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex) const
	{
		assert(state != nullptr);

		state->Bind();
		GL_Call(glDrawArrays(reinterpret_cast<const GLPipelineState*>(state)->GetGLTopology(), startVertex, vertexCount));
		state->Unbind();
	}

	void GLDeviceContext::DrawIndexed(const IPipelineState* const state, uint32 indexCount, uint32 startVertex) const
	{
		assert(state != nullptr);

		GL_Call(glDrawElementsBaseVertex(reinterpret_cast<const GLPipelineState*>(state)->GetGLTopology(),
			indexCount, GL_UNSIGNED_INT, nullptr, startVertex));
	}

	void GLDeviceContext::DrawInstanced(const IPipelineState * const state, uint32 vertexCount, uint32 startVertex, uint32 instances) const
	{
	}

	void GLDeviceContext::DrawIndexInstanced(const IPipelineState * const state, uint32 indexCount, uint32 startVertex, uint32 instances) const
	{
	}
}