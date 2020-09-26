#pragma once

#include "Mesh.h"
#include "Material.h"
#include "..\Physics\Transform3D.h"

namespace Graphics
{
	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer();

		void Submit(const MaterialInstance* material, const Mesh& mesh, const Transform3D& transform);
		void Flush();

	private:
		void ShadowPass() const;
		void GeometryPass() const;
		void RadiancePass() const;
		void LightPass() const;
		void PostProcessPass() const;

	private:

	};
}