#pragma once

#include "..\..\Defines.h"

template<typename T>
class EngineObject
{
public:
	EngineObject(const EngineObject& other) = delete;
	EngineObject& operator=(const EngineObject& other) = delete;

	inline EngineObject()
		: m_Ptr(nullptr)
	{
	}

	inline EngineObject(T* const ptr)
		: m_Ptr(ptr)
	{
	}

	inline EngineObject(EngineObject&& other)
		: m_Ptr(other.m_Ptr)
	{
		other.m_Ptr = nullptr;
	}

	inline ~EngineObject()
	{
		Release();
	}

	inline void Release()
	{
		ReleaseBuffer_S(m_Ptr);
	}

	inline void SetPtr(T* const ptr)
	{
		Release();

		m_Ptr = ptr;
	}

	inline T* GetPtr()
	{
		return m_Ptr;
	}

	inline const T* GetPtr() const
	{
		return m_Ptr;
	}


	//Operators
	inline T* operator->()
	{
		return GetPtr();
	}

	inline const T* operator->() const
	{
		return GetPtr();
	}

	inline T* operator=(T* const ptr)
	{
		SetPtr(ptr);

		return m_Ptr;
	}

	inline EngineObject&  operator=(EngineObject&& other)
	{
		if (this != &other)
		{
			Release();

			m_Ptr = other.m_Ptr;
			other.m_Ptr = nullptr;
		}

		return *this;
	}

	inline operator T*()
	{
		return m_Ptr;
	}

	inline operator const T*() const
	{
		return m_Ptr;
	}

	inline T* const * operator&()
	{
		return &m_Ptr;
	}

	inline const T* const * operator&() const
	{
		return &m_Ptr;
	}

private:
	T* m_Ptr;
};