#include "..\D3DContext.h"
#include "D3DShaderResourceHandle.h"
#include <iostream>

namespace Graphics
{
	D3DShaderResourceHandle::D3DShaderResourceHandle()
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
	}

	D3DShaderResourceHandle::D3DShaderResourceHandle(const D3DShaderResourceHandle& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
	}

	D3DShaderResourceHandle::D3DShaderResourceHandle(D3DShaderResourceHandle&& other)
		: m_View(nullptr),
		m_Resource(nullptr),
		m_IsBound(false)
	{
	}

	D3DShaderResourceHandle::~D3DShaderResourceHandle()
	{
		Release();
	}



	void D3DShaderResourceHandle::Release()
	{
		m_IsBound = false;

		D3DRelease_S(m_View);
		D3DRelease_S(m_Resource);
	}



	bool D3DShaderResourceHandle::Create(ID3D11Device* const device, ID3D11Resource* const resource, D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		Release();


		if (FAILED(device->CreateShaderResourceView(resource, &desc, &m_View)))
		{
			std::cout << "Could not create ShaderResourceView";
			return false;
		}

		return d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, resource);
	}



	D3DShaderResourceHandle& D3DShaderResourceHandle::operator=(const D3DShaderResourceHandle& other)
	{
		if (this != &other)
		{
			Release();


			m_IsBound = other.m_IsBound;

			d3d_query_resource_ptr<ID3D11ShaderResourceView>(&m_View, other.m_View);
			d3d_query_resource_ptr<ID3D11Resource>(&m_Resource, other.m_Resource);
		}

		return *this;
	}

	D3DShaderResourceHandle& D3DShaderResourceHandle::operator=(D3DShaderResourceHandle&& other)
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