#pragma once

#include "..\Core\Graphics\API\IDeviceContext.h"
#include "..\Core\Graphics\RenderManager.h"
#include "GLUtilities.h"

//offset as byte ptr - since byte is 1 byte big we get 'i' as offset 
#define GL_BUFFER_OFFSET(i) ((byte*)nullptr + (i))

namespace Graphics
{
	class GLDeviceContext : public IDeviceContext
	{
	public:
		GLDeviceContext(const Window& window, const RenderSettings& manager);
		~GLDeviceContext();

		bool Create(const Window& window) override;
		void MakeWindowTarget() const override;
		void ClearWindow(const ColorF& color) const override;
		void Present() const override;

		void Draw(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex) const override;
		void DrawIndexed(const IPipelineState* const state, uint32 indexCount, uint32 startVertex) const override;
		void DrawInstanced(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex, uint32 instances) const override;
		void DrawIndexInstanced(const IPipelineState* const state, uint32 indexCount, uint32 startVertex, uint32 instances) const override;

		void Release() override;

		void WindowResizeCallback(uint16 width, uint16 height) override;

		inline const Viewport& GetWindowViewport() const override
		{
			return m_Viewport;
		}

		inline API GetApi() const override
		{
			return GRAPHICS_API_OPENGL;
		}
		
		inline const Window* GetWindow() const override
		{
			return m_Window;
		}

		inline const Math::Point& GetTargetSize() const override
		{
			return m_TargetSize;
		}

	private:
		const Window* m_Window;
		const RenderSettings* m_Manager;
		void* m_SystemContext;
		void* m_GLContext;
		Math::Point m_TargetSize;
		Viewport m_Viewport;
	};
}