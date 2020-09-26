#pragma once

#include "..\..\..\Defines.h"
#include "..\..\Math\Point.h"

class ColorF;
class Window;

namespace Graphics
{
	struct Viewport;

	class RenderSettings;
	class IPipelineState;

	class IDeviceContext
	{
	public:
		virtual ~IDeviceContext() {}

		virtual bool Create(const Window& window) = 0;
		virtual void MakeWindowTarget() const = 0;
		virtual void ClearWindow(const ColorF& color) const = 0;
		virtual void Present() const = 0;

		virtual void Draw(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex) const = 0;
		virtual void DrawIndexed(const IPipelineState* const state, uint32 indexCount, uint32 startVertex) const = 0;
		virtual void DrawInstanced(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex, uint32 instances) const = 0;
		virtual void DrawIndexInstanced(const IPipelineState* const state, uint32 indexCount, uint32 startVertex, uint32 instances) const = 0;

		virtual void Release() = 0;

		virtual API GetApi() const = 0;
		virtual const Window* GetWindow() const = 0;
		virtual const Math::Point& GetTargetSize() const = 0;

		virtual const Viewport& GetWindowViewport() const = 0;

		virtual void WindowResizeCallback(uint16 width, uint16 height) = 0;

	public:
		static const IDeviceContext* Create(Window& window, API api, const RenderSettings& manager);
		static const IDeviceContext* ChangeAPI(Window& window, API api);
	};
}