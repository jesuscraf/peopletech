#ifndef __PRXFACT__HPP__
#define __PRXFACT__HPP__

#include "m4types.hpp"
#include "px_block.hpp"

class ClMutex;



#define M4_PAGE_SIZE    4096 // = tamaño de una pagina de memoria = 4K
#define M4_PX_SIZE      sizeof(ClBlock_Channel_Proxy)
#define M4_ARRAY_SIZE   M4_PAGE_SIZE / M4_PX_SIZE


class ClProxyProvider
{
public:
    ClProxyProvider ();
	~ClProxyProvider ()  {}

	m4return_t GetSize(ClSize &ao_size);

    m4char_t ProxyArray[M4_PAGE_SIZE];
    ClProxyProvider * NextArray;
};


class ClProxyFactory
{
public:

    ClProxyFactory(void);
   ~ClProxyFactory(void);

    void ForceLock(void);

    ClBlock_Channel_Proxy * Get(void);
    void Reuse (ClBlock_Channel_Proxy * ai_pProxy);
	m4return_t GetSize(ClSize &ao_size);

protected:

    ClProxyProvider ProxyMap;
    void* FirstFreeProxy;

	ClMutex* m_poProxyMutex;
};

#endif

