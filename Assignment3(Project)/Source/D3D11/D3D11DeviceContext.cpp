#include <iostream>
#include "..\Core\Window.h"
#include "..\Core\Graphics\RenderManager.h"
#include "..\Core\Utilities\Utilities.h"
#include "..\Core\Graphics\ColorF.h"
#include "D3D11PipelineState.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	D3D11DeviceContext::D3D11DeviceContext(const Window& window, const RenderSettings& manager)
		: m_Manager(&manager),
		m_Window(nullptr),
		m_SwapChain(nullptr),
		m_Debug(nullptr),
		m_Device(nullptr),
		m_DeviceContext(nullptr),
		m_BackBuffer(nullptr),
		m_FeatureLevel(),
		m_TargetSize(window.GetSize().x, window.GetSize().y),
		m_Viewport()
	{
		Create(window);
	}

	D3D11DeviceContext::~D3D11DeviceContext()
	{
		Release();
	}

	bool D3D11DeviceContext::Create(const Window& window)
	{
		m_Window = &window;

		if (!CreateDevice())
			return false;

		if (!CreateSwapChain())
			return false;

		return true;
	}

	void D3D11DeviceContext::MakeWindowTarget() const
	{
		m_DeviceContext->OMSetRenderTargets(1, &m_BackBuffer, nullptr);
	}

	void D3D11DeviceContext::ClearWindow(const ColorF& color) const
	{
		m_DeviceContext->ClearRenderTargetView(m_BackBuffer, color);
	}

	void D3D11DeviceContext::Present() const
	{
		m_SwapChain->Present(0, 0);
	}

	void D3D11DeviceContext::Draw(const IPipelineState* const state, uint32 vertexCount, uint32 startVertex) const
	{
		assert(state != nullptr);

		state->Bind();
		m_DeviceContext->Draw(vertexCount, startVertex);
		state->Unbind();
	}

	void D3D11DeviceContext::DrawIndexed(const IPipelineState* const state, uint32 indexCount, uint32 startVertex) const
	{
		assert(state != nullptr);
		
		state->Bind();
		m_DeviceContext->DrawIndexed(indexCount, 0, startVertex);
		state->Unbind();
	}

	void D3D11DeviceContext::DrawInstanced(const IPipelineState * const state, uint32 vertexCount, uint32 startVertex, uint32 instances) const
	{
		assert(state != nullptr);

		state->Bind();
		m_DeviceContext->DrawInstanced(vertexCount, instances, startVertex, 0);
		state->Unbind();
	}

	void D3D11DeviceContext::DrawIndexInstanced(const IPipelineState * const state, uint32 indexCount, uint32 startVertex, uint32 instances) const
	{
		assert(state != nullptr);

		state->Bind();
		m_DeviceContext->DrawIndexedInstanced(indexCount, instances, 0, startVertex, 0);
		state->Unbind();
	}

	void D3D11DeviceContext::Release()
	{
		m_Window = nullptr;

		D3DRelease_S(m_BackBuffer);
		D3DRelease_S(m_SwapChain);

		if (m_DeviceContext != nullptr)
		{
			m_DeviceContext->Flush();
			m_DeviceContext->ClearState();

			D3DRelease(m_DeviceContext);
		}
		
		if (m_Debug != nullptr)
		{
			m_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			
			D3DRelease(m_Debug);
		}

		D3DRelease_S(m_Device);
	}

	void D3D11DeviceContext::WindowResizeCallback(uint16 width, uint16 height)
	{
		//Calculate size of the viewports
		m_Manager->CalculateViewportSize(Math::Point(width, height), m_Viewport);

		//Resize buffers
		if (m_TargetSize.x == width && m_TargetSize.y == height)
			return;

		if (width < 1 || height < 1)
			return;


		m_DeviceContext->ClearState();
		D3DRelease_S(m_BackBuffer);
		
		if (SUCCEEDED(m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		{
			if (CreateBuffer())
			{
				m_TargetSize.x = width;
				m_TargetSize.y = height;

				std::cout << "Reszied target: (Width = " << width << ", height = " << height << ")\n";
			}
		}
	}

	bool D3D11DeviceContext::CreateDevice()
	{
		if (m_Device != nullptr)
			return true;

		//Supported featurelevels
		D3D_FEATURE_LEVEL featureLevels[D3D_FEATURE_LEVEL_COUNT] =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		//Flags for device (Singlethreaded for underlying optimizations and Debug for debugbuilds)
		uint32 deviceFlags = 0;
#ifdef DEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		//Create device
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, featureLevels,
			D3D_FEATURE_LEVEL_COUNT, D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, &m_DeviceContext);
		if (FAILED(hr))
		{
			std::cout << "ERROR: Could not create Device\n";
			return false;
		}

#ifdef DEBUG
		if (FAILED(m_Device->QueryInterface<ID3D11Debug>(&m_Debug)))
		{
			std::cout << "ERROR: Failed to query Debug-Interface\n";
			return false;
		}
		
		//Disable some warings and set break on errors
		ID3D11InfoQueue* infoQueue = nullptr;
		if (SUCCEEDED(m_Debug->QueryInterface<ID3D11InfoQueue>(&infoQueue)))
		{
			//Messages that debuglayer should hide
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
			};

			//Setup filter for hiding debugmessages
			D3D11_INFO_QUEUE_FILTER filter;
			ZeroBlock(&filter, sizeof(D3D11_INFO_QUEUE_FILTER));
			filter.DenyList.NumIDs = 1;
			filter.DenyList.pIDList = hide;

			//Set break on errors
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

			//Add filter
			if (FAILED(infoQueue->AddRetrievalFilterEntries(&filter)))
			{
				D3DRelease(infoQueue);
				std::cout << "ERROR: Could not set Debuglayer-filter\n";
				return false;
			}

			D3DRelease(infoQueue);
		}
		else
		{
			std::cout << "ERROR: Failed to query InfoQueue\n";
			return false;
		}
#endif


		IDXGIDevice* device = nullptr;
		IDXGIAdapter* adapter = nullptr;
		//Get adapter info (Graphics Card)
		DXGI_ADAPTER_DESC aDesc;
		char str[sizeof(aDesc.Description)];

		if (SUCCEEDED(m_Device->QueryInterface<IDXGIDevice>(&device)))
		{
			if (SUCCEEDED(device->GetAdapter(&adapter)))
			{
				adapter->GetDesc(&aDesc);
				wcstombs(str, aDesc.Description, sizeof(str));
			}
			else
			{
				std::cout << "ERROR: Failed at GetAdapter()";
				return false;
			}
		}
		else
		{
			std::cout << "ERROR: Failed to query Device-Interface";
			return false;
		}


		//Block for using namespace std;
		{
			using namespace std;

			//Print info on creation
			cout << "Created Context (API = " << graphicsapi_to_string(GRAPHICS_API_D3D11) << " | " <<
				d3d_feature_level_as_string(m_FeatureLevel) << ", GPU = " << str << ")\n";
		}


		//Release pointers
		D3DRelease_S(adapter);
		D3DRelease_S(device);
		return true;
	}

	bool D3D11DeviceContext::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC sDesc;
		ZeroBlock(&sDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		sDesc.BufferCount = 2; //Two buffers front and back
		sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Standard format
		sDesc.BufferDesc.RefreshRate.Numerator = 0; //Let DXGI decide refreshrate
		sDesc.BufferDesc.RefreshRate.Denominator = 1;
		sDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allow DXGI to change modes
		sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //Buffers are going to be used as rendertargets
		sDesc.OutputWindow = (HWND)m_Window->GetHandle();
		sDesc.Windowed = true; //Always start windowed
		sDesc.SampleDesc.Count = 1; //Backbuffer is not multisampled
		sDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		bool result = false;
		IDXGIDevice* device = nullptr;
		IDXGIAdapter* adapter = nullptr;
		IDXGIFactory* factory = nullptr;

		//Get the factory so we can create SwapChain and disable alt-enter
		if (SUCCEEDED(m_Device->QueryInterface<IDXGIDevice>(&device)))
		{
			if (SUCCEEDED(device->GetAdapter(&adapter)))
			{
				if (SUCCEEDED(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory)))
				{
					if (SUCCEEDED(factory->CreateSwapChain(device, &sDesc, &m_SwapChain)))
					{
						factory->MakeWindowAssociation(sDesc.OutputWindow, DXGI_MWA_NO_ALT_ENTER);
						result = true;
					}
				}
			}
		}


		//Release pointers
		D3DRelease_S(device);
		D3DRelease_S(adapter);
		D3DRelease_S(factory);


		if (!result)
		{
			std::cout << "Could not create swapchain\n";
			return false;
		}

		return CreateBuffer();
	}

	bool D3D11DeviceContext::CreateBuffer()
	{
		if (m_SwapChain == nullptr)
			return false;


		ID3D11Texture2D* backBuffer = nullptr;
		if (SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
		{
			D3D11_RENDER_TARGET_VIEW_DESC vDesc;
			ZeroBlock(&vDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			vDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			vDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			vDesc.Texture2D.MipSlice = 0;

			if (FAILED(m_Device->CreateRenderTargetView(backBuffer, &vDesc, &m_BackBuffer)))
			{
				std::cout << "Coudl not Initialize BackBuffer\n";
				
				D3DRelease_S(backBuffer);
				return false;
			}
		}

		D3DRelease_S(backBuffer);
		return true;
	}
}