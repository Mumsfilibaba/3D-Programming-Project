#ifndef D3DTEXTUREHANDLE_H
#define D3DTEXTUREHANDLE_H

#include "..\D3DUtilities.h"

namespace Graphics
{
	struct D3DShaderResourceHandle
	{
		friend class D3DContext;

	public:
		D3DShaderResourceHandle();
		D3DShaderResourceHandle(const D3DShaderResourceHandle& other);
		D3DShaderResourceHandle(D3DShaderResourceHandle&& other);
		~D3DShaderResourceHandle();

		bool Create(ID3D11Device* const device, ID3D11Resource* const resource, D3D11_SHADER_RESOURCE_VIEW_DESC& desc);

		void Release();


		inline bool IsBound() const
		{
			return m_IsBound;
		}

		inline ID3D11Resource* GetResource() const
		{
			return m_Resource;
		}

		D3DShaderResourceHandle& operator=(const D3DShaderResourceHandle& other);
		D3DShaderResourceHandle& operator=(D3DShaderResourceHandle&& other);

	public:
		ID3D11ShaderResourceView* m_View;
		ID3D11Resource* m_Resource;

		mutable bool m_IsBound;
	};
}
#endif