#include "D3DRenderTargetHandle.h"
#include "..\D3DContext.h"
#include <iostream>

namespace Graphics
{
	D3DRenderTargetHandle::D3DRenderTargetHandle()
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
	}

	D3DRenderTargetHandle::D3DRenderTargetHandle(const D3DRenderTargetHandle& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
		*this = other;
	}

	D3DRenderTargetHandle::D3DRenderTargetHandle(D3DRenderTargetHandle&& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
		*this = other;
	}

	D3DRenderTargetHandle::~D3DRenderTargetHandle()
	{
		Release();
	}



	void D3DRenderTargetHandle::Release()
	{
		m_IsBound = false;

		D3DRelease_S(m_View);
		D3DRelease_S(m_Resource);
	}



	void D3DRenderTargetHandle::Clear(ID3D11DeviceContext* const context,  const ColorF& clearColor) const
	{
		context->ClearRenderTargetView(m_View, clearColor);
	}



	bool D3DRenderTargetHandle::Create(ID3D11Device* const device, ID3D11Resource* const resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc)
	{
		Release();


		if (FAILED(device->CreateRenderTargetView(resource, &desc, &m_View)))
		{
			std::cout << "Could not create RenderTargetView";
			return false;
		}

		return d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, resource);
	}



	D3DRenderTargetHandle& D3DRenderTargetHandle::operator=(const D3DRenderTargetHandle& other)
	{
		if (this != &other)
		{
			Release();


			m_IsBound = other.m_IsBound;

			d3d_query_resource_ptr<ID3D11RenderTargetView>(&m_View, other.m_View);
			d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, other.m_Resource);
		}

		return *this;
	}

	D3DRenderTargetHandle& D3DRenderTargetHandle::operator=(D3DRenderTargetHandle&& other)
	{
		if (this != &other)
		{
			Release();


			m_IsBound = other.m_IsBound;


			m_View = other.m_View;
			other.m_View = nullptr;


			m_Resource = other.m_Resource;
			other.m_Resource = nullptr;
		}

		return *this;
	}
}