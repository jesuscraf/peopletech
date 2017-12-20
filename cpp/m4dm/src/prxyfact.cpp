#include "prxyfact.hpp"
#include "syncro.hpp"



ClProxyProvider::ClProxyProvider () : NextArray (0)
{
    m4uint16_t i;
    m4pchar_t * pAux;
    for (i = 0; i < (M4_ARRAY_SIZE - 1); i ++)
    {
        pAux = (m4pchar_t * )(&ProxyArray[i * M4_PX_SIZE]);
        * pAux = &ProxyArray[(i + 1) * M4_PX_SIZE];
    }
    pAux = (m4pchar_t * )(&ProxyArray[(M4_ARRAY_SIZE - 1) * M4_PX_SIZE]);
    * pAux = (m4pchar_t)0;
}


m4return_t	ClProxyProvider::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(NextArray);

	if (NextArray){
		if (NextArray->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}






ClProxyFactory::ClProxyFactory(void)
{
    FirstFreeProxy = &ProxyMap.ProxyArray[0 * M4_PX_SIZE];
	m_poProxyMutex = NULL;
}


ClProxyFactory::~ClProxyFactory(void)
{
	/* Bug 191342
	Se borra de manera secuencial, no recursiva
	*/
	ClProxyProvider* poNextArray = ProxyMap.NextArray;

	while( poNextArray != NULL )
	{
		ClProxyProvider* poArray = poNextArray;
		poNextArray = poNextArray->NextArray;
		delete( poArray );
	}

	if( m_poProxyMutex != NULL )
	{
		delete m_poProxyMutex ;
		m_poProxyMutex = NULL ;
	}
}


void ClProxyFactory::ForceLock(void)
{
	if( m_poProxyMutex == NULL )
	{
		m_poProxyMutex = new ClMutex( M4_TRUE );
	}
}


ClBlock_Channel_Proxy * ClProxyFactory::Get(void)
{
	if( m_poProxyMutex != NULL )
	{
		m_poProxyMutex->Lock();
	}

    if (!FirstFreeProxy)
    {
        ClProxyProvider * pAuxPP = &ProxyMap;
        while (pAuxPP->NextArray)
            pAuxPP = pAuxPP->NextArray;
        pAuxPP->NextArray = new ClProxyProvider;
        FirstFreeProxy = &pAuxPP->NextArray->ProxyArray[0 * M4_PX_SIZE];
    }

    void * auxpointer = FirstFreeProxy;
    FirstFreeProxy =  *(void **)FirstFreeProxy;

	if( m_poProxyMutex != NULL )
	{
		m_poProxyMutex->Unlock();
	}

    return (ClBlock_Channel_Proxy *) auxpointer;
}


void ClProxyFactory::Reuse (ClBlock_Channel_Proxy * ai_pProxy)
{
	if( m_poProxyMutex != NULL )
	{
		m_poProxyMutex->Lock();
	}

    ClProxyProvider * pAuxPP = &ProxyMap;
    while (pAuxPP)
    {
        if ((ai_pProxy >= (ClBlock_Channel_Proxy *)&pAuxPP->ProxyArray[0 * M4_PX_SIZE])&&
            (ai_pProxy <= (ClBlock_Channel_Proxy *)&pAuxPP->ProxyArray[(M4_ARRAY_SIZE - 1) * M4_PX_SIZE]))
            break;
        pAuxPP = pAuxPP->NextArray;
    }

    m4uint32_t index = ai_pProxy - (ClBlock_Channel_Proxy *) &(pAuxPP->ProxyArray[0 * M4_PX_SIZE]);

    m4pchar_t * pAux;
    pAux = (m4pchar_t * )(&pAuxPP->ProxyArray[index * M4_PX_SIZE]);
    * pAux = (m4pchar_t)FirstFreeProxy;
    FirstFreeProxy = &pAuxPP->ProxyArray[index * M4_PX_SIZE];

	if( m_poProxyMutex != NULL )
	{
		m_poProxyMutex->Unlock();
	}
}


m4return_t	ClProxyFactory::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(FirstFreeProxy);

	// La forma de calcular va a ser :
	// Size = Tamaño estructura + sizeof (Proxys libres).
		// El sizeof de los que no estan libres se cuenta con cada Node_Data.
	
	// El Proxymap mete en su GetSize unicamente su estructura, de forma que lo que queda es sumar el espacio
	// ocupado por los libres.


	if (ProxyMap.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Cuento los libres que tengo.
	void *pFree=FirstFreeProxy;
	m4uint32_t icount = 0;
	
	while (pFree){
		pFree = *(void **)pFree;
		++icount;
	}

	ao_size+=M4_PX_SIZE*icount;

	return M4_SUCCESS;
}




