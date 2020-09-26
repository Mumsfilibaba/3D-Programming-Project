#include <iostream>
#include "..\Core\Utilities\Utilities.h"
#include "D3D11PipelineState.h"
#include "D3D11Shader.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace Graphics
{
	D3D11Shader::D3D11Shader(const D3D11DeviceContext* const context, const std::string& filename, 
		const std::string& filepath, SHADER type)
		: m_ShaderBlob(nullptr),
		m_Context(context),
		m_Filename(),
		m_Filepath(),
		m_Type(SHADER_UNKOWN)
	{
		Create(filename, filepath, type);
	}

	D3D11Shader::~D3D11Shader()
	{
		Release();
	}

	bool D3D11Shader::Compile(const std::string& filename, const std::string& filepath, SHADER type)
	{
		m_Filename = filename;
		m_Filepath = filepath;
		m_Type = type;

		const char* buffer = nullptr;
		uint32 bytes = read_textfile(m_Filepath + m_Filename + ".hlsl", &buffer);
		if (bytes == 0)
		{
			std::cout << "ERROR: Could not read file '" << m_Filepath << m_Filename << ".hlsl\n";
			return false;
		}

		//Set last char to end-char
		const_cast<char*>(buffer)[bytes] = '\0';

		//Set compileflags
		uint32 flags = 0;
#ifdef _DEBUG
		flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		flags = D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_SKIP_VALIDATION;
#endif

		std::string target(GetTarget());
		ID3DBlob* error = nullptr;
		ID3DBlob* shader = nullptr;

		bool result = true;
		//Compile shader
		if (FAILED(D3DCompile(buffer, strlen(buffer), nullptr, nullptr, nullptr, "main",
			target.c_str(), flags, 0, &shader, &error)))
		{
			//Output error message if it fails
			std::cout << "ERROR: Could not compile shader (" << shadertype_to_string(type) << ", "
				<< m_Filepath << m_Filename << "):\n" << (const char*)error->GetBufferPointer() << '\n';

			D3DRelease_S(error);

			result = false;
		}
		else
		{
			D3DRelease_S(m_ShaderBlob);
			m_ShaderBlob = shader;
		}

		ReleaseBuffer_S(buffer);
		return result;
	}

	void D3D11Shader::OnFileModified()
	{
		std::cout << "Shader '" << m_Filepath << m_Filename << ".hlsl" << "' was modified\n";

		Create(m_Filename, m_Filepath, m_Type);
	}

	void D3D11Shader::Release()
	{
		D3DRelease_S(m_ShaderBlob);
		ReleaseShader(&m_Shader);
	}

	std::string D3D11Shader::GetTarget()
	{
		switch (m_Type)
		{
		case SHADER_VERTEX: return std::string("vs_5_0");
		case SHADER_PIXEL: return std::string("ps_5_0");
		case SHADER_GEOMETRY: return std::string("gs_5_0");
		case SHADER_HULL: return std::string("hs_5_0");
		case SHADER_DOMAIN: return std::string("ds_5_0");
		default: return std::string("_4_0");
		}
	}

	void D3D11Shader::ReleaseShader(void** shader)
	{
		assert(shader != nullptr);

		void* ptr = *shader;
		switch (m_Type)
		{
		case SHADER_VERTEX:
			if (ptr != nullptr)
				reinterpret_cast<ID3D11VertexShader*>(ptr)->Release();
			break;
		case SHADER_HULL:
			if (ptr != nullptr)
				reinterpret_cast<ID3D11HullShader*>(ptr)->Release();
			break;
		case SHADER_DOMAIN:
			if (ptr != nullptr)
				reinterpret_cast<ID3D11DomainShader*>(ptr)->Release();
			break;
		case SHADER_GEOMETRY:
			if (ptr != nullptr)
				reinterpret_cast<ID3D11GeometryShader*>(ptr)->Release();
			break;
		case SHADER_PIXEL:
			if (ptr != nullptr)
				reinterpret_cast<ID3D11PixelShader*>(ptr)->Release();
			break;
		default: 
			return;
		}

		*shader = nullptr;
	}

	bool D3D11Shader::Create(const std::string& filename, const std::string& filepath, SHADER type)
	{
		//Compile shader
		if (Compile(filename, filepath, type))
		{
			//Create shader
			void* shader = nullptr;
			switch (m_Type)
			{
			case SHADER_VERTEX:
				shader = CreateShader<ID3D11VertexShader>();
				break;
			case SHADER_HULL:
				shader = CreateShader<ID3D11HullShader>();
				break;
			case SHADER_DOMAIN:
				shader = CreateShader<ID3D11DomainShader>();
				break;
			case SHADER_GEOMETRY:
				shader = CreateShader<ID3D11GeometryShader>();
				break;
			case SHADER_PIXEL:
				shader = CreateShader<ID3D11PixelShader>();
				break;
			}

			//If created successfully
			if (shader != nullptr)
			{
				ReleaseShader(&m_Shader);
				m_Shader = shader;
			}
			else
			{
				//D3DRelease_S(m_ShaderBlob);
				ReleaseShader(&shader);
				std::cout << "ERROR: Could not create shader\n";
				return false;
			}
		}
		else
		{
			return false;
		}

		std::cout << "SHADER CREATED SUCCESSFULLY (" << shadertype_to_string(type) << ", " << m_Filepath << m_Filename << "):\n";
		return true;
	}
}