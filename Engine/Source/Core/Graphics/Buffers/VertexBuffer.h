#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "..\..\..\Defines.h"

namespace Graphics
{
	class Context;

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void Resize(void* data, uint32 arrLen, uint32 count) = 0;
		
		virtual void* Map() = 0;
		virtual void Unmap() = 0;
		virtual void Update(void* const data, uint32 count) = 0;

	public:
		static VertexBuffer* Create(const Context& context, void* const data, uint32 arrLen, uint32 count, 
			uint32 stride, USAGE usage = USAGE_DEFAULT);
	};
}
#endif