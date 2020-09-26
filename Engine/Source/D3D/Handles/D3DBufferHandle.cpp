#include "D3DBufferHandle.h"
#include <iostream>

namespace Graphics
{
	D3DBufferHandle::D3DBufferHandle()
		: m_Buffer(nullptr)
	{
	}

	D3DBufferHandle::~D3DBufferHandle()
	{
		Release();
	}

	bool D3DBufferHandle::Create(ID3D11Device* const device, void* const data, uint32 count, uint32 stride,
		D3D11_BIND_FLAG flag, D3D11_USAGE usage, uint32 cpuAccess)
	{
		D3D11_BUFFER_DESC desc;
		ZeroBlock(sizeof(D3D11_BUFFER_DESC), &desc);
		desc.BindFlags = flag;
		desc.StructureByteStride = stride;
		desc.ByteWidth = count * stride;
		desc.Usage = usage;
		desc.CPUAccessFlags = cpuAccess;
		
		//A dynamic resource needs to have D3D11_CPU_ACCESS_WRITE
		if (usage == D3D11_USAGE_DYNAMIC)
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;


		//Create a buffer with starting data or not
		if (data != nullptr)
		{
			D3D11_SUBRESOURCE_DATA resource;
			ZeroBlock(sizeof(D3D11_SUBRESOURCE_DATA), &resource);
			resource.pSysMem = data;

			if (FAILED(device->CreateBuffer(&desc, &resource, &m_Buffer)))
			{
				std::cout << "Failed to create Buffer\n";
				return false;
			}
		}
		else
		{
			if (FAILED(device->CreateBuffer(&desc, nullptr, &m_Buffer)))
			{
				std::cout << "Failed to create Buffer\n";
				return false;
			}
		}

		return true;
	}


	void D3DBufferHandle::Release()
	{
		D3DRelease_S(m_Buffer);
	}
}