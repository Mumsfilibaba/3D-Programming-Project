#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS

//D3D
#include "..\D3D\D3DContext.h"
#include "..\D3D\Shaders\D3DShader.h"
#include "..\D3D\Shaders\D3DEffect.h"
#include "..\D3D\Buffers\D3DVertexBuffer.h"
#define D3D_CONTEXT_CAST(x) reinterpret_cast<const D3DContext&>(x)

//GL
#include "..\OpenGL\GLContext.h"
#include "..\OpenGL\Shaders\GLShader.h"
#include "..\OpenGL\Buffers\GLVertexBuffer.h"
//System
#include "..\Core\System\Window.h"

namespace Graphics
{
	Context* Context::Create(GRAPHICS_API api, const Window& window)
	{
		Context* context = nullptr;

		switch (api)
		{
		case GRAPHICS_API_FIRST_AVAILABLE:
		case GRAPHICS_API_D3D11: context = new D3DContext(); break;
		case GRAPHICS_API_OPENGL: context = new GLContext(window); break;
		case GRAPHICS_API_UNKNOWN:
		default: return nullptr;
		}


		if (!context->Initialize(window))
			return nullptr;


		return context;
	}



	Shader* Shader::CreateFromFile(const Context& context, const std::string& filename,
		const std::string& filepath, SHADERTYPE type)
	{
		switch (context.GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3DShader(filename, filepath, type);
		case GRAPHICS_API_OPENGL: return new GLShader(filename, filepath, type);
		default: return nullptr;
		}
	}



	Effect* Effect::Create(const Context& context, const Shader* const vertex, const Shader* const pixel,
		const Shader* const geometry)
	{
		switch (context.GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3DEffect(D3D_CONTEXT_CAST(context), vertex, pixel, geometry);
		default: return nullptr;
		}
	}


	VertexBuffer* VertexBuffer::Create(const Context& context, void* const data, uint32 arrLen, uint32 count,
		uint32 stride, USAGE usage)
	{
		switch (context.GetApi())
		{
		case GRAPHICS_API_D3D11: return new D3DVertexBuffer(D3D_CONTEXT_CAST(context), data, arrLen, count, stride, usage);
		case GRAPHICS_API_OPENGL: return new GLVertexBuffer();
		default: return nullptr;
		}
	}
}

#endif