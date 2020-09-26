#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include "..\Core\System\Context.h"
#include "GLUtilities.h"

class Window;

namespace Graphics
{
	class GLContext : public Context
	{
	public:
		GLContext(const GLContext& other) = delete;
		GLContext& operator=(const GLContext& other) = delete;

		GLContext(const Window& window);
		~GLContext();

		void Release();
		
		bool Initialize(const Window& window) override;

		void Draw(uint32 vertexCount) const override;
		
		void Present() const override;

		void BindBackBuffers() const override;

		void SetTopology(TOPOLOGY topology) const override;
		void SetViewport(float width, float height, float x, float y) const override;
		void SetClearColor(const ColorF& color) const override;
		void SetClearDepth(float depth) const override;


		inline GRAPHICS_API GetApi() const override
		{
			return GRAPHICS_API_OPENGL;
		}

	public:
		static const GLContext& Instance()
		{
			return *s_Instance;
		}

	private:
		void* m_GLContext;
		void* m_SysContext;
		const void* m_WindowHandle;

		mutable int32 m_Topology;

		mutable float m_ClearDepth;
		mutable float m_ClearColor[4];

	public:
		static bool IsGLValid();

	private:
		static GLContext* s_Instance;
	};
}
#endif