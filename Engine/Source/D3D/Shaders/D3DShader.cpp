#include "D3DShader.h"
#include "..\D3DContext.h"
#include <iostream>
#include <vector>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


namespace Graphics
{
	D3DShader::D3DShader(const std::string& filename, const std::string& filepath, SHADERTYPE type)
		: m_Blob(nullptr),
		m_Filename(filename),
		m_Filepath(filepath),
		m_Type(type)
	{
	}

	D3DShader::~D3DShader()
	{
		Release();
	}

	void D3DShader::Release()
	{
		D3DRelease_S(m_Blob);
	}


	bool D3DShader::Compile(const char* const code)
	{
		//Set compileflags
		uint32 flags = 0;
#ifdef _DEBUG
		flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif


		std::string entry(GetEntryPoint());
		std::string target(GetTarget());
		ID3DBlob* error = nullptr;

		
		//Compile shader
		if (FAILED(D3DCompile(code, strlen(code), nullptr, nullptr, nullptr, entry.c_str(), 
			target.c_str(), flags, 0, &m_Blob, &error)))
		{
			//Output error message if it fails
			std::cout << (const char*)error->GetBufferPointer();
			D3DRelease_S(error);
			
			return false;
		}

		return true;
	}


	std::string&& D3DShader::GetTarget()
	{
		switch (m_Type)
		{
		case SHADERTYPE_VERTEX: return std::string("vs_5_0");
		case SHADERTYPE_PIXEL: return std::string("ps_5_0");
		case SHADERTYPE_GEOMETRY: return std::string("gs_5_0");
		default: return std::string("_5_0");
		}
	}

	std::string&& D3DShader::GetEntryPoint()
	{
		switch (m_Type)
		{
		case SHADERTYPE_VERTEX: return std::string("mainVS");
		case SHADERTYPE_PIXEL: return std::string("mainPS");
		case SHADERTYPE_GEOMETRY: return std::string("mainGS");
		default: return std::string("main");
		}
	}
}