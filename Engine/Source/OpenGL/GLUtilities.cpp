#include "GLUtilities.h"

namespace Graphics
{
	uint32 convert_topology(TOPOLOGY topology)
	{
		switch (topology)
		{
		case TOPOLOGY_TRIANGLELIST: return GL_TRIANGLES;
		case TOPOLOGY_TRIANGLESTRIP: return GL_TRIANGLE_STRIP;
		case TOPOLOGY_UNKNOWN:
		default: return -1;
		}
	}

	TOPOLOGY convert_topology(uint32 glTopology)
	{
		switch (glTopology)
		{
		case GL_TRIANGLES: return TOPOLOGY_TRIANGLELIST;
		case GL_TRIANGLE_STRIP: return TOPOLOGY_TRIANGLESTRIP;
		default: return TOPOLOGY_UNKNOWN;
		}
	}
}