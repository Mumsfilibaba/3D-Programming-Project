#ifndef SHADER_H
#define SHADER_H

#include "..\..\..\Defines.h"
#include <string>

namespace Graphics
{
	class Context;

	enum SHADERTYPE : byte
	{
		SHADERTYPE_VERTEX = (1 << 0),
		SHADERTYPE_PIXEL = (1 << 1),
		SHADERTYPE_GEOMETRY = (1 << 2)
	};

	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual bool IsValid() const = 0;

		virtual SHADERTYPE GetType() const = 0;
		virtual const std::string& GetFilename() const = 0;
		virtual const std::string& GetFilepath() const = 0;

	public:
		static Shader* CreateFromFile(const Context& context, const std::string& filename, 
			const std::string& filepath, SHADERTYPE type);
	};
}
#endif
