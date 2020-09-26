#ifndef D3DSHADER_H
#define D3DSHADER_H

#include "..\..\Core\Graphics\Shaders\Shader.h"
#include "..\D3DUtilities.h"

#define VS_ENTRYPOINT "mainVS"
#define PS_ENTRYPOINT "mainPS"
#define GS_ENTRYPOINT "mainGS"
#define VS_TARGET "vs_5_0"
#define PS_TARGET "ps_5_0"
#define GS_TARGET "gs_5_0"
#define COMPONENT_COUNT_1 (1 << 0)
#define COMPONENT_COUNT_2 (1 << 1)
#define COMPONENT_COUNT_3 (1 << 2)
#define COMPONENT_COUNT_4 (1 << 3)

namespace Graphics
{
	class D3DContext;

	class D3DShader : public Shader
	{
	public:
		D3DShader(const std::string& filename, const std::string& filepath, SHADERTYPE type);
		~D3DShader();

		inline bool IsValid() const override
		{
			return (m_Blob != nullptr);
		}

		inline SHADERTYPE GetType() const override
		{
			return m_Type;
		}

		inline const std::string& GetFilename() const override
		{
			return m_Filename;
		}

		inline const std::string& GetFilepath() const override
		{
			return m_Filepath;
		}



		inline ID3DBlob* GetBlob() const
		{
			return m_Blob;
		}

		void Release();

	private:
		bool Compile(const char* const code);
		std::string&& GetTarget();
		std::string&& GetEntryPoint();
		
	private:
		ID3DBlob* m_Blob;
		std::string m_Filename;
		std::string m_Filepath;
		SHADERTYPE m_Type;
	};
}
#endif