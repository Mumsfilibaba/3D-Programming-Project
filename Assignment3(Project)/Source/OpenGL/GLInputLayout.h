#pragma once

#include "..\Core\Graphics\API\IInputLayout.h"
#include "GLDeviceContext.h"

namespace Graphics
{
	class GLInputLayout : public IInputLayout
	{
	public:
		GLInputLayout(const GLDeviceContext* context, const Attribute* const attributes, uint32 len);
		~GLInputLayout();

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

		void Release() override;

		inline uint32 GetLayout() const
		{
			return m_Layout;
		}

	private:
		const GLDeviceContext* m_Context;
		Attribute* m_Attributes;
		uint32 m_Count;
		uint32 m_Stride;
		uint32 m_Layout;
	};
}