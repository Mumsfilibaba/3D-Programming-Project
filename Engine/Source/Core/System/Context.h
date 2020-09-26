#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include "..\..\Defines.h"
#include "..\Graphics\ColorF.h"

class Window;

namespace Graphics
{
	class Context
	{
	public:
		virtual ~Context() {};

		virtual bool Initialize(const Window& window) = 0;

		virtual void Draw(uint32 vertexCount) const = 0;

		virtual void Present() const = 0;
		
		virtual void BindBackBuffers() const = 0;
		
		virtual void SetTopology(TOPOLOGY topology) const = 0;
		virtual void SetViewport(float width, float height, float x, float y) const = 0;
		virtual void SetClearColor(const ColorF& color) const = 0;
		virtual void SetClearDepth(float depth) const = 0;

		virtual GRAPHICS_API GetApi() const = 0;


	public:
		static Context* Create(GRAPHICS_API api, const Window& window);
	};
}
#endif