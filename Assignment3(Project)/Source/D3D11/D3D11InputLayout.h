#pragma once

#include "..\Core\Graphics\API\IInputLayout.h"
#include "D3D11DeviceContext.h"

namespace Graphics
{
	class D3D11InputLayout : public IInputLayout
	{
	public:
		D3D11InputLayout(const D3D11DeviceContext* context, const Attribute* const attributes, uint32 len);
		~D3D11InputLayout();

		bool Create(const Attribute* const attributes, uint32 len) override;

		void GetAttribute(Attribute& attribute, uint32 index) const override;
		
		inline uint32 GetStride() const override
		{
			return m_Stride;
		}

		inline uint32 GetCount() const override
		{
			return m_Count;
		}

		inline const IDeviceContext* GetContext() const override
		{
			return m_Context;
		}

		inline ID3D11InputLayout* GetLayoutPtr() const
		{
			return m_Layout;
		}

		void Release() override;

	private:
		const D3D11DeviceContext* m_Context;
		ID3D11InputLayout* m_Layout;
		Attribute* m_Attributes;
		uint32 m_Count;
		uint32 m_Stride;
	};
}