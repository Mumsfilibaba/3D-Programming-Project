#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS
#include "..\Core\Window.h"
#include "..\Core\FileManegment\FileListener.h"
//D3D11
#define D3D11CONTEXT_CAST(x) reinterpret_cast<const D3D11DeviceContext*>(x)
#include "..\D3D11\D3D11DeviceContext.h"
#include "..\D3D11\D3D11Shader.h"
#include "..\D3D11\D3D11PipelineState.h"
#include "..\D3D11\D3D11VertexBuffer.h"
#include "..\D3D11\D3D11IndexBuffer.h"
#include "..\D3D11\D3D11UniformBuffer.h"
#include "..\D3D11\D3D11RenderTarget2D.h"
#include "..\D3D11\D3D11Texture2D.h"
#include "..\D3D11\D3D11FrameBuffer.h"
#include "..\D3D11\D3D11DepthBuffer.h"
#include "..\D3D11\D3D11RenderTargetDepth.h"
#include "..\D3D11\D3D11TextureCube.h"
#include "..\D3D11\D3D11InputLayout.h"
//OpenGL
#define GLCONTEXT_CAST(x) reinterpret_cast<const GLDeviceContext*>(x)
#include "..\OpenGL\GLDeviceContext.h"
#include "..\OpenGL\GLShader.h"
#include "..\OpenGL\GLPipelineState.h"
#include "..\OpenGL\GLVertexBuffer.h"
#include "..\OpenGL\GLIndexBuffer.h"
#include "..\OpenGL\GLUniformBuffer.h"
#include "..\OpenGL\GLRenderTarget2D.h"
#include "..\OpenGL\GLTexture2D.h"
#include "..\OpenGL\GLFrameBuffer.h"
#include "..\OpenGL\GLDepthBuffer.h"
#include "..\OpenGL\GLRenderTargetDepth.h"
#include "..\OpenGL\GLTextureCube.h"
#include "..\OpenGL\GLInputLayout.h"

namespace Graphics
{
	const IDeviceContext* IDeviceContext::Create(Window& window, API api, const RenderSettings& manager)
	{
		if (!window.CreateContext(api, manager))
			return nullptr;

		return window.GetDeviceContext();
	}

	const IDeviceContext* IDeviceContext::ChangeAPI(Window& window, API api)
	{
		return nullptr;
	}

	IShader* IShader::Create(const IDeviceContext* const context, const std::string& filename, const std::string& filepath,
		FileListener* const fileListener, SHADER type)
	{
		//For safty
		if (context == nullptr)
			return nullptr;

		//Create Shader
		IShader* ptr = nullptr;
		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11:
			ptr = new D3D11Shader(D3D11CONTEXT_CAST(context), filename, filepath, type); 
			
			//TODO: What if the same file gets compiled into multiple shaderobjects?
			//Add to fileListener
			if (fileListener != nullptr && ptr != nullptr)
			{
				if (fileListener->GetPath() == filepath)
					fileListener->AddFile(filename + ".hlsl", ptr);
			}
			
			break;
		case GRAPHICS_API_OPENGL: 
			ptr = new GLShader(GLCONTEXT_CAST(context), filename, filepath, type);
			
			//TODO: What if the same file gets compiled into multiple shaderobjects?
			//Add to fileListener
			if (fileListener != nullptr && ptr != nullptr)
			{
				if (fileListener->GetPath() == filepath)
					fileListener->AddFile(filename + ".glsl", ptr);
			}
			
			break;
		}

		return ptr;
	}

	IPipelineState* IPipelineState::Create(const IDeviceContext* const context)
	{
		if (context == nullptr)
			return nullptr;

		switch(context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11PipelineState(D3D11CONTEXT_CAST(context));
		case GRAPHICS_API_OPENGL: return new GLPipelineState(GLCONTEXT_CAST(context));
		}

		return nullptr;
	}

	IVertexBuffer* IVertexBuffer::Create(const IDeviceContext* const context, const void* const vertices, 
		uint32 len, uint32 stride, uint32 count)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11:
			return new D3D11VertexBuffer(D3D11CONTEXT_CAST(context), vertices, len, stride, count);
		case GRAPHICS_API_OPENGL: 
			return new GLVertexBuffer(GLCONTEXT_CAST(context), vertices, len, stride, count);
		}

		return nullptr;
	}

	IIndexBuffer* IIndexBuffer::Create(const IDeviceContext* const context, const uint32* const indices, 
		uint32 len, uint32 count)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11:
			return new D3D11IndexBuffer(D3D11CONTEXT_CAST(context), indices, len, count);
		case GRAPHICS_API_OPENGL:
			return new GLIndexBuffer(GLCONTEXT_CAST(context), indices, len, count);
		}

		return nullptr;
	}

	IUniformBuffer* IUniformBuffer::Create(const IDeviceContext* const context, const void* const data, uint32 len,
		uint32 stride, uint32 count)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11UniformBuffer(D3D11CONTEXT_CAST(context), data, len, count, stride);
		case GRAPHICS_API_OPENGL: return new GLUniformBuffer(GLCONTEXT_CAST(context), data, len, stride, count);
		}

		return nullptr;
	}

	IRenderTarget2D* IRenderTarget2D::Create(const IDeviceContext* const context, uint16 width, uint16 height,
		TEXTUREFORMAT format)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11RenderTarget2D(D3D11CONTEXT_CAST(context), width, height, format);
		case GRAPHICS_API_OPENGL: return new GLRenderTarget2D(GLCONTEXT_CAST(context), width, height, format);
		}

		return nullptr;
	}

	ITexture2D* ITexture2D::Create(const IDeviceContext* const context, const void* const data, uint16 width, uint16 height,
		TEXTUREFORMAT format, bool mips)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11Texture2D(D3D11CONTEXT_CAST(context), data, width, height, format, mips);
		case GRAPHICS_API_OPENGL: return new GLTexture2D(GLCONTEXT_CAST(context), data, width, height, format, mips);
		}

		return nullptr;
	}

	ITexture2D* ITexture2D::Create(const IDeviceContext* const context, const std::string& filename, const std::string& filepath, 
		TEXTUREFORMAT format, bool mips)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11Texture2D(D3D11CONTEXT_CAST(context), filename, filepath, format, mips);
		case GRAPHICS_API_OPENGL: return new GLTexture2D(GLCONTEXT_CAST(context), filename, filepath, format, mips);
		}

		return nullptr;
	}

	IFrameBuffer* IFrameBuffer::Create(const IDeviceContext* const context, const IRenderTarget2D* const * const rendertargets, uint32 len)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11FrameBuffer(D3D11CONTEXT_CAST(context), rendertargets, len);
		case GRAPHICS_API_OPENGL: return new GLFrameBuffer(GLCONTEXT_CAST(context), rendertargets, len);
		}

		return nullptr;
	}

	IDepthBuffer* IDepthBuffer::Create(const IDeviceContext* const context, uint16 width, uint16 height,
		TEXTUREFORMAT format)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11DepthBuffer(D3D11CONTEXT_CAST(context), width, height, format);
		case GRAPHICS_API_OPENGL: return new GLDepthBuffer(GLCONTEXT_CAST(context), width, height, format);
		}

		return nullptr;
	}

	IRenderTargetDepth* IRenderTargetDepth::Create(const IDeviceContext* const context, uint16 width, uint16 height, TEXTUREFORMAT format)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11RenderTargetDepth(D3D11CONTEXT_CAST(context), width, height, format);
		case GRAPHICS_API_OPENGL: return new GLRenderTargetDepth(GLCONTEXT_CAST(context), width, height, format);
		}

		return nullptr;
	}

	ITextureCube* ITextureCube::Create(const IDeviceContext* const context, const std::string filenames[6], 
		const std::string& filepath, TEXTUREFORMAT format)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11TextureCube(D3D11CONTEXT_CAST(context), filenames, filepath, format);
		case GRAPHICS_API_OPENGL: return new GLTextureCube(GLCONTEXT_CAST(context), filenames, filepath, format);
		}

		return nullptr;
	}

	ITextureCube* ITextureCube::Create(const IDeviceContext* const context, const std::string* const filenames, uint32 len,
		const std::string& filepath, TEXTURECROSS cross, TEXTUREFORMAT format)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11TextureCube(D3D11CONTEXT_CAST(context), filenames, len, filepath, cross, format);
		case GRAPHICS_API_OPENGL: return new GLTextureCube(GLCONTEXT_CAST(context), filenames, len, filepath, cross, format);
		}

		return nullptr;
	}

	IInputLayout* Graphics::IInputLayout::Create(const IDeviceContext* const context, const Attribute* const attributes, uint32 len)
	{
		if (context == nullptr)
			return nullptr;

		switch (context->GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3D11InputLayout(D3D11CONTEXT_CAST(context), attributes, len);
		case GRAPHICS_API_OPENGL: return new GLInputLayout(GLCONTEXT_CAST(context), attributes, len);
		}

		return nullptr;
	}
}
#endif