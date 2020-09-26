#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include "..\..\Core\Graphics\Buffers\VertexBuffer.h"
#include "..\GLUtilities.h"

namespace Graphics
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer();
		~GLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void Resize(void* data, uint32 arrLen, uint32 count) override;

		void* Map() override;
		void Unmap() override;
		void Update(void* const data, uint32 count) override;
	};
}
#endif