#include <iostream>
#include <cassert>
#include "D3D11UniformBuffer.h"

namespace Graphics
{
	D3D11UniformBuffer::D3D11UniformBuffer(const D3D11DeviceContext* const context, const void* const data, 
		uint32 len, uint32 count, uint32 stride)
		: m_Context(context),
		m_Buffer(nullptr),
		m_Count(0),
		m_Stride(0),
		m_VSSlot(0),
		m_PSSlot(0),
		m_HSSlot(0),
		m_DSSlot(0),
		m_GSSlot(0)
	{
		Create(data, len, stride, count);
	}

	D3D11UniformBuffer::~D3D11UniformBuffer()
	{
		Release();
	}

	bool D3D11UniformBuffer::Create(const void* const data, uint32 len, uint32 stride, uint32 count)
	{
		assert(m_Context != nullptr);

		//Copy data
		m_Count = count;
		m_Stride = stride;

		//Create buffer
		D3D11_BUFFER_DESC desc;
		ZeroBlock(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = m_Stride * m_Count;
		desc.StructureByteStride = m_Stride;
		desc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = 0;
		if (data == nullptr)
		{
			hr = m_Context->GetDevice()->CreateBuffer(&desc, nullptr, &m_Buffer);
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;

			hr = m_Context->GetDevice()->CreateBuffer(&desc, &initData, &m_Buffer);
		}

		if (FAILED(hr))
		{
			std::cout << "ERROR: Failed to create UniformBuffer\n";
			return false;
		}

		return true;
	}

	bool D3D11UniformBuffer::Update(const void* const data, uint32 len, uint32 startElement)
	{
		assert(m_Context != nullptr);

		if (data == nullptr)
			return false;

		if (startElement >= len)
			return false;

		//TODO: DO NOT UPDATE THE WHOLE BUFFER IF THIS IS NOT WANTED

		m_Context->GetContext()->UpdateSubresource(m_Buffer, 0, nullptr, data, 0, 0);
		return true;
	}

	void D3D11UniformBuffer::BindVS(uint8 slot) const
	{
		assert(m_Context != nullptr);
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_VSSlot = slot;
		m_Context->GetContext()->VSSetConstantBuffers(m_VSSlot, 1, &m_Buffer);
	}

	void D3D11UniformBuffer::BindPS(uint8 slot) const
	{
		assert(m_Context != nullptr);
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_PSSlot = slot;
		m_Context->GetContext()->PSSetConstantBuffers(m_PSSlot, 1, &m_Buffer);
	}

	void D3D11UniformBuffer::BindHS(uint8 slot) const
	{
		assert(m_Context != nullptr);
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_HSSlot = slot;
		m_Context->GetContext()->HSSetConstantBuffers(m_HSSlot, 1, &m_Buffer);
	}

	void D3D11UniformBuffer::BindDS(uint8 slot) const
	{
		assert(m_Context != nullptr);
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_DSSlot = slot;
		m_Context->GetContext()->DSSetConstantBuffers(m_DSSlot, 1, &m_Buffer);
	}

	void D3D11UniformBuffer::BindGS(uint8 slot) const
	{
		assert(m_Context != nullptr);
		assert(slot < MAX_UNIFORM_BUFFERS);

		m_GSSlot = slot;
		m_Context->GetContext()->GSSetConstantBuffers(m_GSSlot, 1, &m_Buffer);
	}

	void D3D11UniformBuffer::UnbindVS() const
	{
		assert(m_Context != nullptr);

		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->VSSetConstantBuffers(m_VSSlot, 1, &buffer);
		m_VSSlot = 0;
	}

	void D3D11UniformBuffer::UnbindPS() const
	{
		assert(m_Context != nullptr);

		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->PSSetConstantBuffers(m_PSSlot, 1, &buffer);
		m_PSSlot = 0;
	}

	void D3D11UniformBuffer::UnbindHS() const
	{
		assert(m_Context != nullptr);

		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->HSSetConstantBuffers(m_HSSlot, 1, &buffer);
		m_HSSlot = 0;
	}

	void D3D11UniformBuffer::UnbindDS() const
	{
		assert(m_Context != nullptr);

		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->DSSetConstantBuffers(m_DSSlot, 1, &buffer);
		m_DSSlot = 0;
	}

	void D3D11UniformBuffer::UnbindGS() const
	{
		assert(m_Context != nullptr);

		ID3D11Buffer* buffer = nullptr;

		m_Context->GetContext()->GSSetConstantBuffers(m_GSSlot, 1, &buffer);
		m_GSSlot = 0;
	}

	void D3D11UniformBuffer::Release()
	{
		D3DRelease_S(m_Buffer);
	}
}