//==============================================================================
//
// (c) Copyright 1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           Channel Wrapper
// File:             LogComObject.h
// Project:          Meta4Mind 3.x
// Author:           Meta Software M.S. , S.A
// Date:             29/07/98
// Language:         C++
// Operating System: WINDOWS
// Definition:
//    Declaration of the CLogComObject
//
//==============================================================================

#ifndef __LOGCOMOBJECT_H__
#define __LOGCOMOBJECT_H__

#include "m4define.hpp"
#include "m4log.hpp"
//#include "clres.hpp"

#ifdef _M4_COM_METRICS

//#if _ATL_VER != 0x0202
#if !(_ATL_VER >= 0x0200)
#error This hack is only guaranteed with ATL version 2.00 or greater, VC++ SP3 release
#endif

void ComLogTrace(const char* format, ...);

//
// CNamedLogComObject - A logging version of CComObject for
// use with CComCoClass derived objects. This version should 
// only be used indirectly through DECLARE_LOG_AGGREGATABLE. The 
// COM/ATL/CoCreateInstance framework will create the instance.
// 
// Rationale: Assumes Base has a member function called LogClassName.
// This is guaranteed if you put the DECLARE_LOG_AGGREGATABLE 
// inside Base.
//
template <class Base>
class CNamedLogComObject : public CComObject<Base>
{
public:
	typedef Base _BaseClass;
	CNamedLogComObject(void* = NULL)
	{
		ComLogTrace("Creating COM Object: "
					"name='%s', address='0x%X', size='%d', time='%d'\n", 
			LogClassName(), this, sizeof(*this), time(NULL));
	}
	// Set refcount to 1 to protect destruction
	~CNamedLogComObject()
	{
		ComLogTrace("Destroying COM Object: "
					"name='%s', address='0x%X', size='%d', time='%d'\n", 
			LogClassName(), this, sizeof(*this), time(NULL));
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
		return l;
	}
};

#define DECLARE_LOG_AGGREGATABLE(x) public:\
	typedef CComCreator2< CComCreator< CNamedLogComObject< x > >, CComCreator< CComAggObject< x > > > _CreatorClass;\
	const char* LogClassName() { return _T(#x); }

//
// CLogComObject - General logging replacement for CComObject. 
// You'll get the same behaviour, except for the additional 
// creation/destruction trace calls. Not useful for objects that
// are instanciated from "outside" (see CNamedLogComObject instead).
//  
template <class Base>
class CLogComObject : public CComObject<Base>
{
public:
	typedef Base _BaseClass;
	CLogComObject(LPCSTR name)
	{
		// If we were compiled using RTTI (#ifdef _CPPRTTI) we could do it
		// elegantly: m_Name = typeid(Base).name();
		// Instead we've got this kludge
		m_Name = name;
		ComLogTrace("Creating COM Object: "
					"name='%s', address='0x%X', size='%d', time='%d'\n", 
			m_Name, this, sizeof(*this), time(NULL));
	}
	~CLogComObject()
	{
		ComLogTrace("Destroying COM Object: "
					"name='%s', address='0x%X', size='%d', time='%d'\n", 
			m_Name, this, sizeof(*this), time(NULL));
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
		return l;
	}
	static HRESULT WINAPI CreateInstance(CLogComObject<Base>** pp, LPCSTR name);
private:
	LPCSTR m_Name;
};


template <class Base>
HRESULT WINAPI CLogComObject<Base>::CreateInstance
(
	CLogComObject<Base>** pp, 
	LPCSTR name	// It really should be LPCSTR but CLogComObject(void* name = NULL)
				// spoils it
)
{
	_ASSERTE(pp != NULL);
	HRESULT hRes = E_OUTOFMEMORY;
	CLogComObject<Base>* p = NULL;
	ATLTRY(p = new CLogComObject<Base>(name))
	if (p != NULL)
	{
		p->SetVoid(NULL);
		p->InternalFinalConstructAddRef();
		hRes = p->FinalConstruct();
		p->InternalFinalConstructRelease();
		if (hRes != S_OK)
		{
			delete p;
			p = NULL;
		}
	}
	*pp = p;
	return hRes;
}

#define M4_CREATE_INSTANCE(classname, pointer)\
	CLogComObject<classname>::CreateInstance(pointer, _T(#classname))

#define M4_COM_OBJECT CLogComObject 

#else

#define M4_COM_OBJECT CComObject 

#define DECLARE_LOG_AGGREGATABLE(x)

#define M4_COM_OBJECT CComObject

#define M4_CREATE_INSTANCE(classname, pointer)\
	CComObject<classname>::CreateInstance(pointer)

#endif // _M4_COM_METRICS

/*
 * M4_CreateInstance - Creates an instance of an ATL object on
 * the heap *and* increments the reference count.
 */

template<class Impl, class Iface>
HRESULT M4_CreateInstance(Iface** ao_Iface, const char* pName)
{
#ifndef _M4_COM_METRICS
//	HRESULT hRes = M4_COM_OBJECT<Impl>::CreateInstance(&pComImpl);
	M4_COM_OBJECT<Impl>* pComImpl = new M4_COM_OBJECT<Impl>();
#else
//	HRESULT hRes = M4_COM_OBJECT<Impl>::CreateInstance(&pComImpl, pName);
	M4_COM_OBJECT<Impl>* pComImpl = new M4_COM_OBJECT<Impl>(pName);
#endif
	HRESULT hRes = pComImpl == NULL? E_OUTOFMEMORY : S_OK;
	if (SUCCEEDED(hRes)) {
		static_cast<IUnknown*>(static_cast<Iface*>(pComImpl))->AddRef();
		*ao_Iface = pComImpl;
	}
	return hRes;
}

#define M4_CREATE_INTERFACE(Impl, ppIFace) M4_CreateInstance<Impl>(ppIFace,  _T(#Impl))

#endif // __LOGCOMOBJECT_H__
