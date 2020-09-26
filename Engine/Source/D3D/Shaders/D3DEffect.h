#ifndef D3DEFFECT_H
#define D3DEFFECT_H

#include "..\..\Core\Graphics\Shaders\Effect.h"
#include "..\D3DUtilities.h"

namespace Graphics
{
	class D3DContext;
	class D3DShader;

	class D3DEffect : public Effect
	{
	public:
		D3DEffect(const D3DContext& context, const D3DShader* const vertex, const D3DShader* const pixel, const D3DShader* const geometry);
		~D3DEffect();

		void Bind() const override;
		void Unbind() const override;

		bool SetVertexShader(const Shader* const shader) override;
		bool SetPixelShader(const Shader* const shader) override;
		bool SetGeometryShader(const Shader* const shader) override;

		void VSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;
		void PSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;
		void GSSetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;
		const UniformBuffer& VSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;
		const UniformBuffer& PSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;
		const UniformBuffer& GSGetUniformBuffer(const UniformBuffer& buffer, byte slot) const override;

		void Release();

	private:
		bool CreateInputLayoutFromShader();
		bool CreateVertexShader();
		bool CreatePixelShader();
		bool CreateGeometryShader();

	private:
		ID3D11InputLayout* m_Layout;
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11GeometryShader* m_GeometryShader;

		const D3DContext& m_Context;
		D3DShader* m_Vertex;
		D3DShader* m_Pixel;
		D3DShader* m_Geometry;
	};
}
#endif