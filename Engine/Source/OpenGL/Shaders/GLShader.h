#ifndef GLSHADER_H
#define GLSHADER_H

#include "..\..\Core\Graphics\Shaders\Shader.h"
#include "..\GLUtilities.h"

namespace Graphics
{
	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& filename, byte flags);
		~GLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniformBuffer(const UniformBuffer& uniformBuffer, byte slotS) const override;
		const UniformBuffer& GetUniformBuffer(byte slot) const override;

	private:
		void Initialize(byte flags);
		bool CreateVertexShader(const char* const * const code);
		bool CreatePixelShader(const char* const * const code);
		bool CreateGeometryShader(const char* const * const code);
		bool Compile(uint32 shader);

	private:
		std::string m_Filename;
		uint32 m_Program;
		uint32 m_Vertex;
		uint32 m_Pixel;
		uint32 m_Geometry;
	};
}
#endif