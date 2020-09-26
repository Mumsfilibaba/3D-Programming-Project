#include "..\..\Core\Utilities\Utilities.h"
#include <iostream>
#include "GLShader.h"

namespace Graphics
{
	GLShader::GLShader(const std::string& filename, byte flags)
		: m_Filename(filename),
		m_Program(0),
		m_Vertex(0),
		m_Pixel(0),
		m_Geometry(0)
	{
		Initialize(flags);
	}

	GLShader::~GLShader()
	{
	}

	void GLShader::Bind() const
	{
	}

	void GLShader::Unbind() const
	{
	}

	void GLShader::SetUniformBuffer(const UniformBuffer& uniformBuffer, byte slotS) const
	{
	}

	const UniformBuffer& GLShader::GetUniformBuffer(byte slot) const
	{
		return UniformBuffer();
	}

	void GLShader::Initialize(byte flags)
	{
		const char* code = nullptr;

		//Load .hlsl version of shader
		if (read_textfile(m_Filename + ".glsl", &code) == 0)
			return;

		std::cout << code << '\n';

		bool result = true;

		//Create StandardShaders
		result = CreateVertexShader(&code);
		result = CreatePixelShader(&code);

		//Create special shaders, as geometry
		if (flags & SHADERFLAGS_GEOMETRY)
			result = CreateGeometryShader(&code);

		if (result)
			

		ReleaseBuffer_S(code);
	}

	bool GLShader::CreateVertexShader(const char* const * const code)
	{
		m_Vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_Vertex, 1, code, nullptr);

		return Compile(m_Vertex);
	}

	bool GLShader::CreatePixelShader(const char* const * const code)
	{
		m_Pixel = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_Pixel, 1, code, nullptr);

		return Compile(m_Pixel);
	}

	bool GLShader::CreateGeometryShader(const char* const * const code)
	{
		return false;
	}

	bool GLShader::Compile(uint32 shader)
	{
		glCompileShader(shader);

		int32 compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
			int32 log_length = 0;
			char message[1024];
			glGetShaderInfoLog(shader, 1024, &log_length, message);
			
			std::cout << message;
			return false;
		}

		return true;
	}
}