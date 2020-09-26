#ifndef D3DBUFFERHANDLE_H
#define D3DBUFFERHANDLE_H

#include "..\D3DUtilities.h"

namespace Graphics
{
	class D3DBufferHandle
	{
	public:
		D3DBufferHandle();
		~D3DBufferHandle();

		bool Create(ID3D11Device* const device, void* const data, uint32 count, uint32 stride, 
			D3D11_BIND_FLAG flag, D3D11_USAGE usage, uint32 cpuAccess);

		void Release();

		inline ID3D11Buffer* GetBuffer() const
		{
			return m_Buffer;
		}

	private:
		ID3D11Buffer* m_Buffer;
	};
}
#endif