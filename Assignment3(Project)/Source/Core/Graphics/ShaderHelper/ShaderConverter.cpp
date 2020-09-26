#include "ShaderConverter.h"
#include <vector>

namespace Graphics
{
	//Helper enum
	enum VARIABLETYPE
	{
		VARIABLETYPE_UNKNOWN = 0,
		VARIABLETYPE_BOOL = 1,
		VARIABLETYPE_FLOAT = 2,
		VARIABLETYPE_FLOAT2 = 3,
		VARIABLETYPE_FLOAT3 = 4,
		VARIABLETYPE_FLOAT4 = 5,
		VARIABLETYPE_FLOAT2X2 = 6,
		VARIABLETYPE_FLOAT2X3 = 7,
		VARIABLETYPE_FLOAT2X4 = 8,
		VARIABLETYPE_FLOAT3X2 = 9,
		VARIABLETYPE_FLOAT3X3 = 10,
		VARIABLETYPE_FLOAT3X4 = 12,
		VARIABLETYPE_FLOAT4X2 = 13,
		VARIABLETYPE_FLOAT4X3 = 14,
		VARIABLETYPE_FLOAT4X4 = 15,
	};

	//Helper structs
	struct ShaderChecklist
	{
	public:
		bool main = false;

	public:
		inline bool IsValid() const
		{
			return main;
		}
	};

	struct ShaderVariable
	{
		std::string name;
		VARIABLETYPE type = VARIABLETYPE_UNKNOWN;
	};

	struct ShaderStruct
	{
		std::string name;
		std::vector<ShaderVariable> variables;
	};

	//Forward Declaration
	void build_glsl_struct_str(std::string& str, const ShaderStruct& desc);
	std::string variabletype_to_str(VARIABLETYPE type);

	//Function
	bool hlsl_to_glsl(const std::string& entryPoint, const std::string& hlslSrc, std::string& glslSrc)
	{
		//Empty the glsl src
		glslSrc.clear();

		//Loop the HLSL src
		for (int32 i = 0; i < hlslSrc.size(); i++)
		{
			switch (hlslSrc[i])
			case '/': 
				i++;
				
				if (hlslSrc[i] == '/')
					i = hlslSrc.find('\n', i);
				else if (hlslSrc[i] == '*')
					i = hlslSrc.find("*/", i);

				break;
		}

		return false;
	}

	//construct a struct
	void build_glsl_struct_str(std::string& str, const ShaderStruct& desc)
	{
		str = "struct " + desc.name + "\n{\n";

		for (int32 i = 0; i < desc.variables.size(); i++)
			str += '\t' + variabletype_to_str(desc.variables[i].type) + ' ' + desc.variables[i].name + ";\n";

		str += "};\n";
	}

	std::string variabletype_to_str(VARIABLETYPE type)
	{
		switch (type)
		{
		case VARIABLETYPE_BOOL: return std::string("bool");
		case VARIABLETYPE_FLOAT: return std::string("float");
		case VARIABLETYPE_FLOAT2: return std::string("float2");
		case VARIABLETYPE_FLOAT3: return std::string("float3");
		case VARIABLETYPE_FLOAT4: return std::string("float4");
		case VARIABLETYPE_FLOAT2X2: return std::string("float2x2");
		case VARIABLETYPE_FLOAT2X3: return std::string("float2x3");
		case VARIABLETYPE_FLOAT2X4: return std::string("float2x4");
		case VARIABLETYPE_FLOAT3X2: return std::string("float3x2");
		case VARIABLETYPE_FLOAT3X3: return std::string("float3x3");
		case VARIABLETYPE_FLOAT3X4: return std::string("float3x4");
		case VARIABLETYPE_FLOAT4X2: return std::string("float4x2");
		case VARIABLETYPE_FLOAT4X3: return std::string("float4x3");
		case VARIABLETYPE_FLOAT4X4: return std::string("float4x4");
		}
	}
}