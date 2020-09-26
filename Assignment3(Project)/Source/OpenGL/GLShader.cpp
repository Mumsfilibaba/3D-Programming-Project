#include "..\Core\Utilities\Utilities.h"
#include "GLShader.h"
#include "GLPipelineState.h"

#include <iostream>

namespace Graphics
{
	GLShader::GLShader(const GLDeviceContext* context, const std::string& filename, const std::string& filepath, SHADER type)
		: m_Context(context),
		m_Program(0),
		m_Filepath(),
		m_Filename(),
		m_Type(SHADER_UNKOWN)
	{
		Compile(filename, filepath, type);
	}

	GLShader::~GLShader()
	{
		Release();
	}

	bool GLShader::Compile(const std::string& filename, const std::string& filepath, SHADER type)
	{
		//TODO: What happens when user calls this? FileListener?

		m_Filename = filename;
		m_Filepath = filepath;
		m_Type = type;

		const char* buffer = nullptr;

		//Read in the whole textfile
		uint32 bytes = read_textfile((m_Filepath + m_Filename + ".glsl"), &buffer);
		if (bytes == 0)
		{
			std::cout << "ERROR: Could not read file '" << m_Filepath << m_Filename << ".glsl\n";
			return false;
		}
		
		//Set last char to end-char
		const_cast<char*>(buffer)[bytes] = '\0';

		//Variables
		int32 result = GL_FALSE;
		char errBuffer[1024];
		uint32 shader = 0;

		//Create shader object and compile
		GL_Call(shader = glCreateShader(GetGLShaderType()));
		GL_Call(glShaderSource(shader, 1, &buffer, nullptr));
		GL_Call(glCompileShader(shader));

		GL_Call(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			ZeroBlock(errBuffer, sizeof(errBuffer));
			GL_Call(glGetShaderInfoLog(shader, sizeof(errBuffer), nullptr, errBuffer));
			
			std::cout << "ERROR: Could not compile shader (" << shadertype_to_string(type) << ", " 
				<< m_Filepath << m_Filename << "):\n" << errBuffer << '\n';

			GL_Call(glDeleteShader(shader));

			ReleaseBuffer_S(buffer);
			return false;
		}

		ReleaseBuffer_S(buffer);

		//Create seperable shaderprogram
		uint32 program = 0;
		GL_Call(program = glCreateProgram());
		GL_Call(glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE));

		//Attach shader
		GL_Call(glAttachShader(program, shader));

		//Link shaders
		GL_Call(glLinkProgram(program));
		GL_Call(glGetProgramiv(program, GL_LINK_STATUS, &result));
		if (result == GL_FALSE)
		{
			ZeroBlock(errBuffer, sizeof(errBuffer));

			GL_Call(glGetProgramInfoLog(program, sizeof(errBuffer), nullptr, errBuffer));

			std::cout << "ERROR: Could not create shader (" << shadertype_to_string(type) << ", "
				<< m_Filepath << m_Filename << "):\n" << errBuffer << '\n';
		}
		
		//Detach and remove shader
		GL_Call(glDetachShader(program, shader));
		GL_Call(glDeleteShader(shader));

		//Delete program if failed
		if (glIsProgram(program) != GL_TRUE && result != GL_TRUE)
		{
			GL_Call(glDeleteProgram(program));
			return false;
		}

		//Release old program and set new
		Release();
		m_Program = program;

		std::cout << "SHADER CREATED SUCCESSFULLY (" << shadertype_to_string(type) << ", "
			<< m_Filepath << m_Filename << "):\n";

		return true;
	}

	void GLShader::OnFileModified()
	{
		std::cout << "Shader '" << m_Filepath << m_Filename << ".glsl" << "' was modified\n";

		Compile(m_Filename, m_Filepath, m_Type);
	}

	void GLShader::Release()
	{
		//Delete shaderobject
		if (glIsProgram(m_Program) == GL_TRUE)
		{
			GL_Call(glDeleteProgram(m_Program));
			m_Program = 0;
		}
	}

	uint32 GLShader::GetGLShaderType()
	{
		switch (m_Type)
		{
		case SHADER_PIXEL: return GL_FRAGMENT_SHADER;
		case SHADER_VERTEX: return GL_VERTEX_SHADER;
		case SHADER_GEOMETRY: return GL_GEOMETRY_SHADER;
		case SHADER_HULL: return GL_TESS_CONTROL_SHADER;
		case SHADER_DOMAIN: return GL_TESS_EVALUATION_SHADER;
		case SHADER_UNKOWN:
		default:
			return 0;
		}
	}
}