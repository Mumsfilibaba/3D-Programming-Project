#ifndef D3DTARGETHANDLE_H
#define D3DTARGETHANDLE_H

#include "..\..\Core\Graphics\ColorF.h"
#include "..\D3DUtilities.h"

namespace Graphics
{
	class D3DRenderTargetHandle
	{
		friend class D3DContext;

	public:
		D3DRenderTargetHandle();
		D3DRenderTargetHandle(const D3DRenderTargetHandle& other);
		D3DRenderTargetHandle(D3DRenderTargetHandle&& other);
		~D3DRenderTargetHandle();

		bool Create(ID3D11Device* const device, ID3D11Resource* const resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc);

		void Release();

		void Clear(ID3D11DeviceContext* const context, const ColorF& clearColor) const;


		inline bool IsBound() const
		{
			return m_IsBound;
		}

		inline ID3D11Resource* GetResource() const
		{
			return m_Resource;
		}

		D3DRenderTargetHandle& operator=(const D3DRenderTargetHandle& other);
		D3DRenderTargetHandle& operator=(D3DRenderTargetHandle&& other);

	private:
		ID3D11RenderTargetView* m_View;
		ID3D11Resource* m_Resource;

		mutable bool m_IsBound;
	};
}
#endif