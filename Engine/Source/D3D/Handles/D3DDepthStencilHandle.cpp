#include "D3DDepthStencilHandle.h"
#include "..\D3DContext.h"
#include <iostream>

namespace Graphics
{
	D3DDepthStencilHandle::D3DDepthStencilHandle()
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
	}

	D3DDepthStencilHandle::D3DDepthStencilHandle(const D3DDepthStencilHandle& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
		*this = other;
	}

	D3DDepthStencilHandle::D3DDepthStencilHandle(D3DDepthStencilHandle&& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
		*this = other;
	}

	D3DDepthStencilHandle::~D3DDepthStencilHandle()
	{
		Release();
	}



	void D3DDepthStencilHandle::Release()
	{
		m_IsBound = false;

		D3DRelease_S(m_View);
		D3DRelease_S(m_Resource);
	}



	bool D3DDepthStencilHandle::Create(ID3D11Device* const device, ID3D11Resource* const resource, D3D11_DEPTH_STENCIL_VIEW_DESC& desc)
	{
		Release();


		if (FAILED(device->CreateDepthStencilView(resource, &desc, &m_View)))
		{
			std::cout << "Could not create DepthStencilView\n";

			return false;
		}

		return d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, resource);
	}



	void D3DDepthStencilHandle::Clear(ID3D11DeviceContext* const context, uint32 clearFlags, float depth, uint8 stencil)
	{
		context->ClearDepthStencilView(m_View, clearFlags, depth, stencil);
	}



	D3DDepthStencilHandle& D3DDepthStencilHandle::operator=(const D3DDepthStencilHandle& other)
	{
		if (this != &other)
		{
			Release();


			m_IsBound = other.m_IsBound;

			d3d_query_resource_ptr<ID3D11DepthStencilView>(&m_View, other.m_View);
			d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, other.m_Resource);
		}

		return *this;
	}

	D3DDepthStencilHandle& D3DDepthStencilHandle::operator=(D3DDepthStencilHandle&& other)
	{
		if (this != &other)
		{
			Release();


			m_IsBound = other.m_IsBound;

			m_Resource = other.m_Resource;
			other.m_Resource = nullptr;

			m_View = other.m_View;
			other.m_View = nullptr;
		}

		return *this;
	}
}