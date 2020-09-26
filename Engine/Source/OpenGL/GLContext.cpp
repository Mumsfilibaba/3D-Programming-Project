#include "..\Core\System\Window.h"
#include "GLContext.h"

namespace Graphics
{
	GLContext* GLContext::s_Instance = nullptr;

	GLContext::GLContext(const Window& window)
		: m_GLContext(nullptr),
		m_SysContext(nullptr),
		m_WindowHandle(nullptr),
		//Current states
		m_Topology(convert_topology(TOPOLOGY_UNKNOWN)),
		//Clear
		m_ClearDepth(1.0f),
		m_ClearColor()
	{
		m_ClearColor[0] = ColorF::CORNFLOWERBLUE.r;
		m_ClearColor[1] = ColorF::CORNFLOWERBLUE.g;
		m_ClearColor[2] = ColorF::CORNFLOWERBLUE.b;
		m_ClearColor[3] = ColorF::CORNFLOWERBLUE.a;
	}

	GLContext::~GLContext()
	{
		Release();
	}


	bool GLContext::IsGLValid()
	{
		return (s_Instance->m_GLContext != nullptr) ? true : false;
	}


	void GLContext::Draw(uint32 vertexCount) const
	{
		glDrawArrays(m_Topology, 0, vertexCount);
	}

	void GLContext::SetTopology(TOPOLOGY topology) const
	{
		m_Topology = convert_topology(topology);
	}

	void GLContext::SetViewport(float width, float height, float x, float y) const
	{
		glViewport(0, 0, (int)width, (int)height);
	}

	void GLContext::SetClearColor(const ColorF& color) const
	{
		m_ClearColor[0] = color.r;
		m_ClearColor[1] = color.g;
		m_ClearColor[2] = color.b;
		m_ClearColor[3] = color.a;
	}

	void GLContext::SetClearDepth(float depth) const
	{
		m_ClearDepth = depth;
	}
}