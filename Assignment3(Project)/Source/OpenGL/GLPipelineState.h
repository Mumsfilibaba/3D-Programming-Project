#pragma once

#include "..\Core\Graphics\API\IPipelineState.h"
#include "GLShader.h"
#include "GLInputLayout.h"

namespace Graphics
{
	struct GLDepthState
	{
		DepthState desc;

		inline void Bind() const
		{
			//TODO: Fix comparisonfunc

			//DepthTesting
			if (desc.depthTest)
			{
				GL_Call(glEnable(GL_DEPTH_TEST));
			}
			else
			{
				GL_Call(glDisable(GL_DEPTH_TEST));
			}

			//StencilTesting
			if (desc.stencilTest)
			{
				GL_Call(glEnable(GL_STENCIL_TEST));
			}
			else
			{
				GL_Call(glDisable(GL_STENCIL_TEST));
			}
		}

		inline void Unbind() const
		{
			//DepthTesting
			GL_Call(glDisable(GL_DEPTH_TEST));
			//StencilTesting
			GL_Call(glDisable(GL_STENCIL_TEST));
		}
	};

	struct GLRasterizerState
	{
		RasterizerState desc;

		inline void Bind() const
		{
			//Triangle order
			GL_Call(glFrontFace(GL_CW));

			//Culling
			if (desc.cull == CULLMODE_NONE)
			{
				GL_Call(glDisable(GL_CULL_FACE));
			}
			else
			{ 
				GL_Call(glEnable(GL_CULL_FACE));
				
				if (desc.cull == CULLMODE_FRONT)
				{
					GL_Call(glCullFace(GL_FRONT));
				}
				else if (desc.cull == CULLMODE_BACK)
				{
					GL_Call(glCullFace(GL_BACK));
				}
			}

			//Wireframe
			if (desc.wireframe)
			{
				GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
				GL_Call(glEnable(GL_LINE_SMOOTH));
			}
			else
			{
				GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
				GL_Call(glDisable(GL_LINE_SMOOTH));
			}

			//TODO: Fix to set the scissor rect somehow
			//Scissor
			if (desc.scissor)
			{
				GL_Call(glEnable(GL_SCISSOR_TEST));
			}
			else
			{
				GL_Call(glDisable(GL_SCISSOR_TEST));
			}

			//TODO: Make sure this is what we want
			//DepthClip
			if (desc.depthClip)
			{
				GL_Call(glEnable(GL_DEPTH_CLAMP));
			}
			else
			{
				GL_Call(glDisable(GL_DEPTH_CLAMP));
			}

			//TODO: Fix depthbias
			//GL_Call(glEnable(GL_POLYGON_OFFSET_FILL));
			//GL_Call(glPolygonOffset());
			//desc.depthBias;
		}

		inline void Unbind() const
		{
			GL_Call(glDisable(GL_CULL_FACE));
			GL_Call(glDisable(GL_LINE_SMOOTH));
			GL_Call(glDisable(GL_SCISSOR_TEST));
			GL_Call(glDisable(GL_DEPTH_CLAMP));
		}
	};

	class GLPipelineState : public IPipelineState
	{
	public:
		GLPipelineState(const GLDeviceContext* const context);
		~GLPipelineState();

		bool Create() override;

		void Bind() const override;
		void Unbind() const override;

		bool SetVertexShader(const IShader* const shader) override;
		bool SetPixelShader(const IShader* const shader) override;
		bool SetGeometryShader(const IShader* const shader) override;
		bool SetHullShader(const IShader* const shader) override;
		bool SetDomainShader(const IShader* const shader) override;

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

		inline uint32 GetProgramPipeline() const
		{
			return m_PipelineObject;
		}

		void Release() override;

		inline uint32 GetGLTopology() const
		{
			return m_Topology;
		}

	private:
		const GLDeviceContext* m_Context;
		const GLShader* m_VertexShader;
		const GLShader* m_PixelShader;
		const GLShader* m_GeometryShader;
		const GLShader* m_HullShader;
		const GLShader* m_DomainShader;
		const GLInputLayout* m_InputLayout;
		uint32 m_Topology;
		uint32 m_PipelineObject;
		Viewport m_Viewport;
		GLDepthState m_DepthState;
		GLRasterizerState m_RasterizerState;
	};
}