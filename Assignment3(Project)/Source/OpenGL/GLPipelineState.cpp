#include "GLPipelineState.h"
#include <iostream>

namespace Graphics
{
	GLPipelineState::GLPipelineState(const GLDeviceContext* const context)
		: m_Context(context),
		m_VertexShader(nullptr),
		m_HullShader(nullptr),
		m_DomainShader(nullptr),
		m_GeometryShader(nullptr),
		m_PixelShader(nullptr),
		m_InputLayout(nullptr),
		m_PipelineObject(0)
	{
		Create();
	}

	GLPipelineState::~GLPipelineState()
	{
		Release();
	}

	bool GLPipelineState::Create()
	{
		GL_Call(glGenProgramPipelines(1, &m_PipelineObject));

		if (!glIsProgramPipeline(m_PipelineObject))
			return false;

		return true;
	}

	void GLPipelineState::Bind() const
	{
		if (IsBound())
			return;

		//Bind shader
		GL_Call(glBindProgramPipeline(m_PipelineObject));
		if (m_VertexShader != nullptr)
		{
			GL_Call(glUseProgramStages(m_PipelineObject, GL_VERTEX_SHADER_BIT, m_VertexShader->GetShaderHandle()));
		}
		if (m_HullShader != nullptr)
		{
			GL_Call(glUseProgramStages(m_PipelineObject, GL_TESS_CONTROL_SHADER_BIT, m_HullShader->GetShaderHandle()));
		}
		if (m_DomainShader != nullptr)
		{
			GL_Call(glUseProgramStages(m_PipelineObject, GL_TESS_EVALUATION_SHADER_BIT, m_DomainShader->GetShaderHandle()));
		}
		if (m_GeometryShader != nullptr)
		{
			GL_Call(glUseProgramStages(m_PipelineObject, GL_GEOMETRY_SHADER_BIT, m_GeometryShader->GetShaderHandle()));
		}
		if (m_PixelShader != nullptr)
		{
			GL_Call(glUseProgramStages(m_PipelineObject, GL_FRAGMENT_SHADER_BIT, m_PixelShader->GetShaderHandle()));
		}

		//Bind inputlayout
		if (m_InputLayout != nullptr)
		{
			GL_Call(glBindVertexArray(m_InputLayout->GetLayout()));
		}

		//Bind viewport
		GL_Call(glViewport(m_Viewport.x, m_Viewport.y, m_Viewport.width, m_Viewport.height));

		//Bindstates
		m_DepthState.Bind();
		m_RasterizerState.Bind();

		IPipelineState::Bind();
	}

	void GLPipelineState::Unbind() const
	{
		if (!IsBound())
			return;

		//Unbind pipeline
		GL_Call(glBindProgramPipeline(0));

		//Unbind inputlayout
		GL_Call(glBindVertexArray(0));

		//Unbind viewport
		GL_Call(glViewport(0, 0, 0, 0));

		//Unbind states
		m_DepthState.Unbind();
		m_RasterizerState.Unbind();

		IPipelineState::Unbind();
	}

	bool GLPipelineState::SetVertexShader(const IShader* const shader)
	{
		m_VertexShader = reinterpret_cast<const GLShader*>(shader);
		return true;
	}

	bool GLPipelineState::SetPixelShader(const IShader* const shader)
	{
		m_PixelShader = reinterpret_cast<const GLShader*>(shader);
		return true;
	}

	bool GLPipelineState::SetGeometryShader(const IShader* const shader)
	{
		m_GeometryShader = reinterpret_cast<const GLShader*>(shader);
		return true;
	}

	bool GLPipelineState::SetHullShader(const IShader* const shader)
	{
		m_HullShader = reinterpret_cast<const GLShader*>(shader);
		return true;
	}

	bool GLPipelineState::SetDomainShader(const IShader* const shader)
	{
		m_DomainShader = reinterpret_cast<const GLShader*>(shader);
		return true;
	}

	void GLPipelineState::SetInputLayout(const IInputLayout* const inputlayout)
	{
		m_InputLayout = reinterpret_cast<const GLInputLayout*>(inputlayout);
	}

	void GLPipelineState::SetTopology(TOPOLOGY topology)
	{
		switch (topology)
		{
		case TOPOLOGY_TRIANGLELIST: m_Topology = GL_TRIANGLES; break;
		case TOPOLOGY_TRIANGLESTRIP: m_Topology = GL_TRIANGLE_STRIP; break;
		case TOPOLOGY_PATCH_3_POINTS: 
			m_Topology = GL_PATCHES; 
			glPatchParameteri(GL_PATCH_VERTICES, 3);
			break;
		default: m_Topology = 0; break;
		}
	}

	TOPOLOGY GLPipelineState::GetTopology() const
	{
		switch (m_Topology)
		{
		case GL_TRIANGLES: return TOPOLOGY_TRIANGLELIST;
		case GL_TRIANGLE_STRIP: return TOPOLOGY_TRIANGLESTRIP;
		case GL_PATCHES: return TOPOLOGY_PATCH_3_POINTS;
		default: return TOPOLOGY_UNKNOWN;
		}
	}

	bool GLPipelineState::SetDepthState(const DepthState& state)
	{
		m_DepthState.desc = state;
		return true;
	}

	bool GLPipelineState::SetRasterizerState(const RasterizerState & state)
	{
		m_RasterizerState.desc = state;
		return true;
	}

	void GLPipelineState::Release()
	{
		if (glIsProgramPipeline(m_PipelineObject))
		{
			GL_Call(glDeleteProgramPipelines(1, &m_PipelineObject));
		}
	}
}