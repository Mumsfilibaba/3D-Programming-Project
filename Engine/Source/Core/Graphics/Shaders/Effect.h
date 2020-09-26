#ifndef EFFECT_H
#define EFFECT_H

#include "Shader.h"

namespace Graphics
{
	class UniformBuffer
	{
	};


	class Effect
	{
	public:
		virtual ~Effect() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual bool SetVertexShader(const Shader* const shader) = 0;
		virtual bool SetPixelShader(const Shader* const shader) = 0;
		virtual bool SetGeometryShader(const Shader* const shader) = 0;

		virtual void VSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;
		virtual void PSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;
		virtual void GSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;
		virtual const UniformBuffer& VSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;
		virtual const UniformBuffer& PSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;
		virtual const UniformBuffer& GSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const = 0;

	public:
		static Effect* Create(const Context& context, const Shader* const vertex, const Shader* const pixel, const Shader* const geometry);
	};
}

#endif