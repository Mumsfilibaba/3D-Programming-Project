#pragma once

#include <vector>
#include "..\Core\Graphics\API\IShader.h"
#include "D3D11DeviceContext.h"
#include <d3dcompiler.h>

namespace Graphics
{
	class D3D11Shader : public IShader
	{
	public:
		D3D11Shader(const D3D11DeviceContext* const context, const std::string& filename,
			const std::string& filepath, SHADER type);
		~D3D11Shader();

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

		void Release() override;

		inline ID3DBlob* GetShaderBlob() const
		{
			return m_ShaderBlob;
		}

		template<typename T>
		T* GetShader() const;

		template<>
		inline ID3D11VertexShader* GetShader<ID3D11VertexShader>() const
		{
			return reinterpret_cast<ID3D11VertexShader*>(m_Shader);
		}

		template<>
		inline ID3D11HullShader* GetShader<ID3D11HullShader>() const
		{
			return reinterpret_cast<ID3D11HullShader*>(m_Shader);
		}

		template<>
		inline ID3D11DomainShader* GetShader<ID3D11DomainShader>() const
		{
			return reinterpret_cast<ID3D11DomainShader*>(m_Shader);
		}

		template<>
		inline ID3D11GeometryShader* GetShader<ID3D11GeometryShader>() const
		{
			return reinterpret_cast<ID3D11GeometryShader*>(m_Shader);
		}

		template<>
		inline ID3D11PixelShader* GetShader<ID3D11PixelShader>() const
		{
			return reinterpret_cast<ID3D11PixelShader*>(m_Shader);
		}

	private:
		std::string GetTarget();
		void ReleaseShader(void** shader);
		bool Create(const std::string& filename, const std::string& filepath, SHADER type);

		template <typename T>
		void* CreateShader();

		template<>
		inline void* CreateShader<ID3D11VertexShader>()
		{
			assert(m_Context != nullptr);

			void* ptr = nullptr;
			if (FAILED(m_Context->GetDevice()->CreateVertexShader(m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11VertexShader**>(&ptr))))
			{
				return nullptr;
			}

			return ptr;
		}

		template<>
		inline void* CreateShader<ID3D11HullShader>()
		{
			assert(m_Context != nullptr);

			void* ptr = nullptr;
			if (FAILED(m_Context->GetDevice()->CreateHullShader(m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11HullShader**>(&ptr))))
			{
				return nullptr;
			}

			return ptr;
		}

		template<>
		inline void* CreateShader<ID3D11DomainShader>()
		{
			assert(m_Context != nullptr);

			void* ptr = nullptr;
			if (FAILED(m_Context->GetDevice()->CreateDomainShader(m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11DomainShader**>(&ptr))))
			{
				return nullptr;
			}

			return ptr;
		}

		template<>
		inline void* CreateShader<ID3D11GeometryShader>()
		{
			assert(m_Context != nullptr);

			void* ptr = nullptr;
			if (FAILED(m_Context->GetDevice()->CreateGeometryShader(m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11GeometryShader**>(&ptr))))
			{
				return nullptr;
			}

			return ptr;
		}

		template<>
		inline void* CreateShader<ID3D11PixelShader>()
		{
			assert(m_Context != nullptr);

			void* ptr = nullptr;
			if (FAILED(m_Context->GetDevice()->CreatePixelShader(m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11PixelShader**>(&ptr))))
			{
				return nullptr;
			}

			return ptr;
		}

	private:
		const D3D11DeviceContext* m_Context;
		ID3DBlob* m_ShaderBlob;
		void* m_Shader;
		SHADER m_Type;
		std::string m_Filename;
		std::string m_Filepath;
	};
}