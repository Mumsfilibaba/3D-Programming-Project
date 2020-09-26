#pragma once

#include "..\..\..\Defines.h"

namespace Graphics
{
	bool hlsl_to_glsl(const std::string& entryPoint, const std::string& hlslSrc, std::string& glslSrc);
}