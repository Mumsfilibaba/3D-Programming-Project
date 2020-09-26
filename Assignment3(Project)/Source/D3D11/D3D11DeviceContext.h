#pragma once

#include "..\Core\Graphics\API\IDeviceContext.h"
#include "D3D11Utilities.h"

#define D3D_FEATURE_LEVEL_COUNT 9

namespace Graphics
{
	class D3D11DeviceContext : public IDeviceContext
	{
	public:
		D3D11DeviceContext(const Window& window, const RenderSettings& manager);
		~D3D11DeviceContext();

		bool Create(const Window& window) override;
		void MakeWindowTarget() const override;
		void ClearWindow(const ColorF& color) const override;
		void Present() const override;

		void Draw(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex) const override;
		void DrawIndexed(const IPipelineState* const state, uint32 indexCount, uint32 startVertex) const override;
		void DrawInstanced(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex, uint32 instances) const override;
		void DrawIndexInstanced(const IPipelineState* const state, uint32 indexCount, uint32 startVertex, uint32 instances) const override;

		void Release() override;

		void WindowResizeCallback(uint16 width, uint16 height) override;
		
		inline const Viewport& GetWindowViewport() const override
		{
			return m_Viewport;
		}

		inline API GetApi() const override
		{
			return GRAPHICS_API_D3D11;
		}

		inline const Window* GetWindow() const override
		{
			return m_Window;
		}

		inline const Math::Point& GetTargetSize() const override
		{
			return m_TargetSize;
		}

		inline ID3D11Device* GetDevice() const
		{
			return m_Device;
		}

		inline ID3D11DeviceContext* GetContext() const
		{
			return m_DeviceContext;
		}

	private:
		bool CreateDevice();
		bool CreateSwapChain();
		bool CreateBuffer();

	private:
		const Window* m_Window;
		const RenderSettings* m_Manager;
		
		IDXGISwapChain* m_SwapChain;
		ID3D11Debug* m_Debug;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		ID3D11RenderTargetView* m_BackBuffer;

		D3D_FEATURE_LEVEL m_FeatureLevel;
		Viewport m_Viewport;
		Math::Point m_TargetSize;
	};
}