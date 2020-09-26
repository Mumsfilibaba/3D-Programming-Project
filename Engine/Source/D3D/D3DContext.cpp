#include "D3DContext.h"
#include "D3DUtilities.h"
#include "Handles\D3DDepthStencilHandle.h"
#include "Handles\D3DRenderTargetHandle.h"
#include "Handles\D3DShaderResourceHandle.h"
#include "..\Core\System\Window.h"

#include <iostream>
#include <dxgi.h>

#pragma comment(lib, "dxgi.lib")

namespace Graphics
{
	//Defintions
	D3DContext::D3DContext()
		: m_SwapChain(nullptr),
		//Device Pointers
		m_Debug(nullptr),
		m_Device(nullptr),
		m_DeviceContext(nullptr),
		//Buffers
		m_BackBuffer(),
		m_DepthBuffer(),
		//Device information
		m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
		//Current bound resources
		m_CurrentDepthStencil(nullptr),
		m_CurrentRenderTargets(),
		m_CurrentTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED),
		m_CurrentViewPort(),
		//Currently bound resourceinformation
		m_RebindTargets(true),
		m_RenderTargetSlots(0),
		//Clear
		m_ClearDepth(0.0f),
		m_ClearColor(ColorF::CORNFLOWERBLUE)
	{
	}

	D3DContext::~D3DContext()
	{
		Release();
	}

	void D3DContext::Release()
	{
		D3DRelease_S(m_SwapChain);

		if (m_DeviceContext != nullptr)
		{
			m_DeviceContext->ClearState();
			m_DeviceContext->Flush();
		}

		D3DRelease_S(m_DeviceContext);
		D3DRelease_S(m_Device);
		
		if (m_Debug != nullptr)
			m_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		
		D3DRelease_S(m_Debug);
	}


	bool D3DContext::Initialize(const Window& window)
	{
		if (m_Device != nullptr)
		{
			std::cout << "Context is already Initialized";
			return true;
		}

		//Initialize Device, SwapChain and buffers
		if (!CreateDevice())
			return false;

		if (!CreateSwapChain((HWND)window.GetHandle()))
			return false;

		if (!CreateBuffers())
			return false;


		//Set standard topology
		SetTopology(TOPOLOGY_TRIANGLELIST);
		
		//Set Standard viewport
		SetViewport((float)window.GetClientSize().x, (float)window.GetClientSize().x, 0.0f, 0.0f);

		return true;
	}



	bool D3DContext::CreateSwapChain(HWND window)
	{
		DXGI_SWAP_CHAIN_DESC sDesc;
		ZeroBlock(sizeof(DXGI_SWAP_CHAIN_DESC), &sDesc);
		sDesc.BufferCount = 2; //Two buffers front and back
		sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Standard format
		sDesc.BufferDesc.RefreshRate.Numerator = 0; //Let DXGI decide refreshrate
		sDesc.BufferDesc.RefreshRate.Denominator = 1;
		sDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allow DXGI to change modes
		sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //Buffers are going to be used as rendertargets
		sDesc.OutputWindow = window; 
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
						factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);
						result = true;
					}
				}
			}
		}


		if (!result)
			std::cout << "Could not create swapchain";


		//Release pointers
		D3DRelease_S(device);
		D3DRelease_S(adapter);
		D3DRelease_S(factory);

		return result;
	}



	bool D3DContext::CreateBuffers()
	{
		//Failsafe
		if (m_SwapChain == nullptr)
			return false;

		//Get created buffer
		ID3D11Texture2D* buffer = nullptr;
		if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer)))
		{
			std::cout << "Failed at GetBuffer()";
			return false;
		}


		//Setup view
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroBlock(sizeof(D3D11_RENDER_TARGET_VIEW_DESC), &desc);
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		//Get buffer-desc for later use before creating view
		D3D11_TEXTURE2D_DESC desc1;
		ZeroBlock(sizeof(D3D11_TEXTURE2D_DESC), &desc1);
		buffer->GetDesc(&desc1);


		//Create view for backbuffer
		if (!m_BackBuffer.Create(m_Device, buffer, desc))
		{
			std::cout << "Could not create BackBufferView";
			
			//Release Backbuffer
			D3DRelease_S(buffer);

			return false;
		}


		//Release Backbuffer
		D3DRelease_S(buffer);


		//Use same buffer desc as for Backbuffer but set depthbuffer specific details
		desc1.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc1.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		

		//Create Texture
		if (SUCCEEDED(m_Device->CreateTexture2D(&desc1, nullptr, &buffer)))
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
			ZeroBlock(sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC), &desc2);
			desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc2.Texture2D.MipSlice = 0;


			//Create view for depthbuffer
			if (!m_DepthBuffer.Create(m_Device, buffer, desc2))
			{
				std::cout << "Could not create DepthBufferView";
				
				return false;
			}
		}
		else
		{
			std::cout << "Could not create Texture for DepthBuffer";

			return false;
		}

		//Release DepthBuffer
		D3DRelease_S(buffer);

		return true;
	}

	void D3DContext::BindRenderTargets() const
	{
		//Bind renderTargets only if necesarry (Don't interact with the drivers if not needed to)
		if (m_RebindTargets)
		{
			m_DeviceContext->OMSetRenderTargets(m_RenderTargetSlots, m_CurrentRenderTargets, m_CurrentDepthStencil);

			m_RebindTargets = false;
		}
	}



	bool D3DContext::CreateDevice()
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
		};


		//Flags for device (Singlethreaded for underlying optimizations and Debug for debugbuilds)
		uint32 deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef DEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		//Create device
		if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, featureLevels,
			D3D_FEATURE_LEVEL_COUNT, D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, &m_DeviceContext)))
		{
			std::cout << "Could not create Device";
			return false;
		}



#ifdef DEBUG
		//Disable some warings and set break on errors
		ID3D11InfoQueue* infoQueue = nullptr;
		if (SUCCEEDED(m_Device->QueryInterface<ID3D11Debug>(&m_Debug)))
		{
			if (SUCCEEDED(m_Debug->QueryInterface<ID3D11InfoQueue>(&infoQueue)))
			{
				//Messages that debuglayer should hide
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
				};


				//Setup filter for hiding debugmessages
				D3D11_INFO_QUEUE_FILTER filter;
				ZeroBlock(sizeof(D3D11_INFO_QUEUE_FILTER), &filter);
				filter.DenyList.NumIDs = sizeof(hide) / sizeof(D3D11_MESSAGE_ID);
				filter.DenyList.pIDList = hide;


				//Set breakpoint
				infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
				infoQueue->AddRetrievalFilterEntries(&filter);

				D3DRelease(infoQueue);
			}
			else
			{
				std::cout << "Failed to query InfoQueue";
				return false;
			}
		}
		else
		{
			std::cout << "Failed to query Debug-Interface";
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
				std::cout << "Failed at GetAdapter()";
				return false;
			}
		}
		else
		{
			std::cout << "Failed to query Device-Interface";
			return false;
		}


		//Block for using namespace std;
		{
			using namespace std;

			//Print info on creation
			cout << "Created Context (API = " << graphicsapi_to_string(GRAPHICS_API_D3D11) << " | " << 
				d3d_featurelevel_to_string(m_FeatureLevel) << ", GPU = " << str << ")\n";
		}


		//Release pointers
		D3DRelease_S(adapter);
		D3DRelease_S(device);
		return true;
	}



	void D3DContext::Draw(uint32 vertexCount) const
	{
		//Draw
		assert(m_DeviceContext != nullptr);

		BindRenderTargets();

		m_DeviceContext->Draw(vertexCount, 0);
	}

	void D3DContext::BindBackBuffers() const
	{
		BindRenderTarget(m_BackBuffer, 0);
		//BindDepthStencil(m_DepthBuffer);
	}


	void D3DContext::SetTopology(TOPOLOGY topology) const
	{
		//Set topology
		assert(m_DeviceContext != nullptr);

		D3D11_PRIMITIVE_TOPOLOGY top = convert_topology(topology);

		if (top != m_CurrentTopology)
		{
			m_DeviceContext->IASetPrimitiveTopology(top);

			m_CurrentTopology = top;
		}
	}

	void D3DContext::SetViewport(float width, float height, float x, float y) const
	{
		assert(m_DeviceContext != nullptr);

		//Set viewport
		D3D11_VIEWPORT viewport;
		ZeroBlock(sizeof(D3D11_VIEWPORT), &viewport);
		viewport.Height = height;
		viewport.Width = width;
		viewport.MaxDepth = MAX_DEPTH;
		viewport.MinDepth = MIN_DEPTH;
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;


		if (viewport != m_CurrentViewPort)
		{
			m_CurrentViewPort = viewport;

			m_DeviceContext->RSSetViewports(1, &m_CurrentViewPort);
		}
	}

	void D3DContext::SetClearColor(const ColorF& color) const
	{
		m_ClearColor = color;
	}

	void D3DContext::SetClearDepth(float depth) const
	{
		m_ClearDepth = depth;
	}

	//SwapBuffers
	void D3DContext::Present() const
	{
		m_SwapChain->Present(0, 0);

		m_BackBuffer.Clear(m_DeviceContext, m_ClearColor);
	}

	void D3DContext::BindDepthStencil(const D3DDepthStencilHandle& depthStencil) const
	{
		//Bind DepthStencilHandle
		if (depthStencil.IsBound())
			return;


		m_CurrentDepthStencil = depthStencil.m_View;
		depthStencil.m_IsBound = true;

		m_RebindTargets = true;
	}

	void D3DContext::UnbindDepthStencil(const D3DDepthStencilHandle& depthStencil) const
	{
		//Unbind DepthStencilHandle
		if (m_CurrentDepthStencil != depthStencil.m_View)
			return;


		m_CurrentDepthStencil = nullptr;
		depthStencil.m_IsBound = false;

		m_RebindTargets = true;
	}

	void D3DContext::BindRenderTarget(const D3DRenderTargetHandle& renderTarget, uint8 slot) const
	{
		//Bind RenderTargetHandle
		assert(slot < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

		if (renderTarget.IsBound())
			return;


		if (slot + 1 > m_RenderTargetSlots)
			m_RenderTargetSlots = slot + 1;


		m_CurrentRenderTargets[slot] = renderTarget.m_View;
		renderTarget.m_IsBound = true;


		m_RebindTargets = true;
	}

	void D3DContext::UnbindRenderTarget(const D3DRenderTargetHandle& renderTarget, uint8 slot) const
	{
		//Unbind RenderTargetHandle
		assert(slot < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

		if (renderTarget.m_View != m_CurrentRenderTargets[slot])
			return;


		m_CurrentRenderTargets[slot] = nullptr;
		renderTarget.m_IsBound = false;


		m_RebindTargets = true;
	}
}