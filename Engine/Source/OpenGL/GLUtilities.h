#ifndef GLUTILITIES_H
#define GLUTILITIES_H

#include "..\Defines.h"

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>

namespace Graphics
{
	uint32 convert_topology(TOPOLOGY topology);
	TOPOLOGY convert_topology(uint32 glTopology);
}

#endif