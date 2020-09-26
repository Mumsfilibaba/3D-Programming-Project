#ifndef D3DCONTEXT_H
#define D3DCONTEXT_H

#include "..\Core\System\Context.h"
#include "D3DUtilities.h"
#include "Handles\D3DDepthStencilHandle.h"
#include "Handles\D3DRenderTargetHandle.h"

#define D3D_FEATURE_LEVEL_COUNT 4

class Window;

namespace Graphics
{
	class D3DDepthStencilHandle;
	class D3DRenderTargetHandle;

	class D3DContext : public Context
	{
	public:
		D3DContext(const D3DContext& other) = delete;
		D3DContext& operator=(const D3DContext& other) = delete;

		D3DContext();
		~D3DContext();

		bool Initialize(const Window& window) override;

		void Draw(uint32 vertexCount) const override;
		
		void BindBackBuffers() const override;

		void Present() const override;
		
		void SetTopology(TOPOLOGY topology) const override;
		void SetViewport(float width, float height, float x, float y) const override;
		void SetClearColor(const ColorF& color) const override;
		void SetClearDepth(float depth) const override;


		inline GRAPHICS_API GetApi() const override
		{
			return GRAPHICS_API_D3D11;
		}



		void BindDepthStencil(const D3DDepthStencilHandle& depthStencil) const;
		void UnbindDepthStencil(const D3DDepthStencilHandle& depthStencil) const;

		void BindRenderTarget(const D3DRenderTargetHandle& renderTarget, uint8 slot) const;
		void UnbindRenderTarget(const D3DRenderTargetHandle& renderTarget, uint8 slot) const;

		void Release();
		
		inline ID3D11Device* GetDevice() const
		{
			return m_Device;
		}

		inline ID3D11DeviceContext* GetDeviceContext() const
		{
			return m_DeviceContext;
		}

	private:
		bool CreateDevice();
		bool CreateBuffers();
		bool CreateSwapChain(HWND window);
		void BindRenderTargets() const;

	private:
		IDXGISwapChain* m_SwapChain;

		//Device pointers
		ID3D11Debug* m_Debug;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		
		//Buffers
		D3DDepthStencilHandle m_DepthBuffer;
		D3DRenderTargetHandle m_BackBuffer;

		//Device information
		D3D_FEATURE_LEVEL m_FeatureLevel;
		
		//Current bound resources
		mutable ID3D11DepthStencilView* m_CurrentDepthStencil;
		mutable ID3D11RenderTargetView* m_CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		mutable D3D11_PRIMITIVE_TOPOLOGY m_CurrentTopology;
		mutable D3D11_VIEWPORT m_CurrentViewPort;

		//Currently bound resourceinformation
		mutable bool m_RebindTargets;
		mutable uint8 m_RenderTargetSlots;

		//Clear
		mutable float m_ClearDepth;
		mutable ColorF m_ClearColor;
	};
}
#endif