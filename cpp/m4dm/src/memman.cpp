#include "memman.hpp"


#if _M4_DEBUG > 10
m4uint8_t * ClMemoryProvider::M4malloc (void)
{
    m4uint8_t * pAddress = 0;
    m4uint32_t i;

    if (m_pFirstFreeblock)
        pAddress = FastMalloc();

    else
    {
        AllocPage();
        if (m_pFirstFreeblock)
            pAddress = FastMalloc();
    }

    FindAddress(pAddress, i);
    m_piUsedBlocks[i] ++;

    return pAddress;
}

m4uint8_t * ClMemoryProvider::M4calloc (void)
{
    m4uint8_t * pAddress = 0;
    m4uint32_t i;

    if (m_pFirstFreeblock)
        pAddress = FastCalloc();

    else
    {
        AllocPage();
        if (m_pFirstFreeblock)
            pAddress = FastCalloc();
    }

    FindAddress(pAddress, i);
    m_piUsedBlocks[i] ++;

    return pAddress;
}
#endif

void ClMemoryProvider::M4free (void * ai_pBlock, m4uint32_t ai_iPage)
{
    ai_pBlock = m_pFirstFreeblock;
    m_pFirstFreeblock = (m4uint8_t *) ai_pBlock;
#if _M4_DEBUG > 10
    m_piUsedBlocks[ai_iPage] --;
#endif
}

m4bool_t ClMemoryProvider::FindAddress (m4uint8_t * ai_pBlock, m4uint32_t &ao_iPage)
{
    m4uint32_t i;
    for (i = 0; i < m_iPageNumber; i ++)
    {
        if ((m_pPageAddresses[i] <= ai_pBlock)&&(ai_pBlock < (m_pPageAddresses[i] + M4_PAGE_SIZE)))
        {
            ao_iPage = i;
            return M4_TRUE;
        }
    }
    return M4_FALSE;
}

void ClMemoryProvider::AllocPage (void)
{
    m4uint8_t * pAuxPage = (m4uint8_t * ) malloc(M4_PAGE_SIZE);
    m4uint8_t * pAuxPointer, * pAuxPointerPrev;

    m4uint32_t i;

#if _M4_DEBUG > 10
    ofstream ofs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
    M4Open( ofs, "C:\\TEMP\\pagelog.txt", M4UniAppend, eEncoding );

    ofs << "Alloc Page: " << (m4uint32_t) pAuxPage << "\t" << m_iAllocSize << endl;
    ofs.close();
#endif

    pAuxPointerPrev = 0;
    for (i = 0; i < m_iBlocksInPage; i ++)
    {
        pAuxPointer = &pAuxPage[i * m_iAllocSize + (8 * (i + 1))];
        * (m4uint8_t **) pAuxPointer = pAuxPointerPrev;
        pAuxPointerPrev = pAuxPointer;
    }

    if ((m_iPageNumber % M4_PAGE_STORE_CACHE) == 0)
    {
        m4uint8_t ** pAuxPageAddresses = new m4uint8_t * [m_iPageNumber + M4_PAGE_STORE_CACHE];
        if (m_pPageAddresses)
        {
            memcpy (pAuxPageAddresses, m_pPageAddresses, sizeof(m4uint8_t *) * m_iPageNumber);
            delete m_pPageAddresses;
        }

        m_pPageAddresses = pAuxPageAddresses;

#if _M4_DEBUG > 10
        m4uint32_t * auxpiUsedBlocks = new m4uint32_t [m_iPageNumber + M4_PAGE_STORE_CACHE];
        if (m_piUsedBlocks)
        {
            memcpy (auxpiUsedBlocks, m_piUsedBlocks, sizeof(m4uint32_t) * m_iPageNumber);
            delete m_piUsedBlocks;
        }

        m_piUsedBlocks = auxpiUsedBlocks;
#endif
    }

    m_pPageAddresses[m_iPageNumber] = pAuxPage;
#if _M4_DEBUG > 10
    m_piUsedBlocks[m_iPageNumber] = 0;
#endif
    m_iPageNumber ++;
    m_pFirstFreeblock = pAuxPointerPrev;

}

#if _M4_DEBUG > 10

void ClMemoryProvider::DumpStatistic(ofstream & ofs)
{
    m4uint32_t i;
    for (i = 0; i < m_iPageNumber; i++)
	{
        ofs << "Pagina: " << i << ". Ocupacion: " << m_piUsedBlocks[i] << " de " << m_iBlocksInPage << endl;
	}
}

#endif

ClMemoryManager::ClMemoryManager (m4uint32_t ai_iMin, m4uint32_t ai_iMax)
{
    m_iMin = ai_iMin;
    m_iMax = ai_iMax;
    
    m4uint32_t i = 3, pSize = 8, j, pSizePrev, pSizeMin;
    
    while (pSize < m_iMin)
    {
        i ++;
        pSize = pSize * 2;
    }

    pSizeMin = pSize; //Variable auxiliar que me vale para luego, cuando haga los news
    m_iLogMin = i; //logaritmo en base 2 del limite inferior

    while (m_iMax > pSize)
    {
        i ++;
        pSize = pSize * 2;
    }

    m_iLogMax = i; //logaritmo en base 2 del limite superior

    m_oMemoryMap = new ClMemoryProvider * [m_iMax - m_iMin + 1];
    m_oRealProvider = new ClMemoryProvider * [m_iLogMax - m_iLogMin + 1];

    pSizePrev = m_iMin;
    pSize = pSizeMin;

    for (i = m_iLogMin; i <= m_iLogMax; i ++)
    {
        m_oRealProvider[i - m_iLogMin] = new ClMemoryProvider(pSize);

        for (j = pSizePrev; j <= pSize; j ++)
            m_oMemoryMap[j - m_iMin] = m_oRealProvider[i - m_iLogMin];

        pSizePrev = pSize + 1;
        pSize = pSize * 2;
    }

    for (i = pSizePrev; i <= m_iMax; i ++)
        m_oMemoryMap[i - m_iMin] = m_oRealProvider[m_iLogMax - m_iLogMin];
}

ClMemoryManager::~ClMemoryManager(void)
{
    m4uint32_t i;

    for (i = m_iLogMin; i <= m_iLogMax; i ++)
        delete m_oRealProvider[i - m_iLogMin];

    delete [] m_oMemoryMap;
    delete [] m_oRealProvider;
}

#if _M4_DEBUG > 10
void ClMemoryManager::DumpStatistic(void)
{
    ofstream ofs;
    m4uint32_t i;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
    M4Open( ofs, "C:\\TEMP\\statistics.txt", M4UniAppend, eEncoding );

    for (i = m_iLogMin; i <= m_iLogMax; i ++)
    {
        ofs << "Manager Size: " << i << endl;
        m_oRealProvider[i - m_iLogMin]->DumpStatistic(ofs);
    }
    ofs.close();
}
#endif
