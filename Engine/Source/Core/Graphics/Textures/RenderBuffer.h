#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "..\..\..\Defines.h"

namespace Graphics
{
	class Context;

	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer() {}

	public:
		static RenderBuffer* Create(const Context& context)
		{
			return nullptr;
		}
	};
}
#endif