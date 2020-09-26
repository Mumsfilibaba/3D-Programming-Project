#pragma once
#include <vector>
#include "..\Core\Graphics\API\IShader.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLShader : public IShader
	{
	public:
		GLShader(const GLDeviceContext* const context, const std::string& filename, 
			const std::string& filepath, SHADER type);
		~GLShader();

		bool Compile(const std::string& filename, const std::string& filepath, SHADER type) override;
		void OnFileModified() override;

		inline const std::string& GetFilename() const override
		{
			return m_Filename;
		}

		inline const std::string& GetFilepath() const override
		{
			return m_Filepath;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		inline SHADER GetShaderType() const override
		{
			return m_Type;
		}

		inline uint32 GetShaderHandle() const
		{
			return m_Program;
		}

		void Release() override;

	private:
		uint32 GetGLShaderType();

	private:
		const GLDeviceContext* m_Context;
		uint32 m_Program;
		SHADER m_Type;
		std::string m_Filename;
		std::string m_Filepath;
	};
}
