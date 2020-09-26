#ifndef D3DDEPTHHANDLE_H
#define D3DDEPTHHANDLE_H

#include "..\D3DUtilities.h"

namespace Graphics
{
	class D3DDepthStencilHandle
	{
		friend class D3DContext;

	public:
		D3DDepthStencilHandle();
		D3DDepthStencilHandle(const D3DDepthStencilHandle& other);
		D3DDepthStencilHandle(D3DDepthStencilHandle&& other);
		~D3DDepthStencilHandle();

		bool Create(ID3D11Device* const device, ID3D11Resource* const resource, D3D11_DEPTH_STENCIL_VIEW_DESC& desc);

		void Release();

		void Clear(ID3D11DeviceContext* const context, uint32 clearFlags, float depth, uint8 stencil);


		inline bool IsBound() const
		{
			return m_IsBound;
		}

		inline ID3D11Resource* GetResource() const
		{
			return m_Resource;
		}

		D3DDepthStencilHandle& operator=(const D3DDepthStencilHandle& other);
		D3DDepthStencilHandle& operator=(D3DDepthStencilHandle&& other);

	private:
		ID3D11DepthStencilView* m_View;
		ID3D11Resource* m_Resource;

		mutable bool m_IsBound;
	};
}
#endif