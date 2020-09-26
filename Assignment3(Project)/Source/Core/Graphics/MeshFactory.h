#pragma once

#include "ModelData.h"

namespace Graphics
{
	class Meshfactory
	{
	public:
		static GeometryData CreateCube();
		static GeometryData CreatePlane(uint8 width = 1, uint8 height = 1);
		static GeometryData CreateSphere(uint8 subdivisions = 0);
		static GeometryData CreatePyramid();
		static GeometryData CreateCylinder(uint8 sides = 5);

		static void Subdivide(GeometryData& data, uint8 subdivisions = 1);
		static void Optimize(GeometryData& data, uint32 startVertex = 0);
		static void CalculateNormalsHardened(GeometryData& data);

	private:
		Meshfactory() = delete;
		~Meshfactory() = delete;
	};
}