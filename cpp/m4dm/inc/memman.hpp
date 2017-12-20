#ifndef __MEMMANAGER__HPP__
#define __MEMMANAGER__HPP__

#define M4_PAGE_SIZE        4096 // 4K
#define M4_PAGE_STORE_CACHE 10 // Cada cuanto creo o aumento el tamaño del arra de las posiciones de las paginas

#include "m4types.hpp"
#include "m4stl.hpp"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "chlog.hpp"
#include "dmres.hpp"
#include "m4unicode.hpp"

class ClMemoryProvider
{
public:

    ClMemoryProvider (m4uint32_t ai_iAllocSize)
    {
        m_iAllocSize      = ai_iAllocSize;
        m_pPageAddresses  = 0;
        m_pFirstFreeblock = 0;
        m_iPageNumber     = 0;
        m_iBlocksInPage   = M4_PAGE_SIZE / (m_iAllocSize + 8);
    }

   ~ClMemoryProvider ()
    {
        m4uint32_t i;
        for (i = 0; i < m_iPageNumber; i ++)
        {

#if _M4_DEBUG > 10
    ofstream ofs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
    M4Open( ofs, "C:\\TEMP\\pagelog.txt", M4UniAppend, eEncoding );

    ofs << "Delete Page: " << (m4uint32_t) m_pPageAddresses[i] << "\t" << m_iAllocSize << endl;
    ofs.close();
#endif

            free(m_pPageAddresses[i]);
        }
        delete m_pPageAddresses;
    }

#if _M4_DEBUG > 10
    m4uint8_t * M4malloc (void);
    m4uint8_t * M4calloc (void);
#else
    m4uint8_t * M4malloc (void)
    {
        if (m_pFirstFreeblock)
            return FastMalloc();

        else
        {
            AllocPage();
            if (m_pFirstFreeblock)
                return FastMalloc();
        }

        return 0;
    }

    m4uint8_t * M4calloc (void)
    {
        if (m_pFirstFreeblock)
            return FastCalloc();

        else
        {
            AllocPage();
            if (m_pFirstFreeblock)
                return FastCalloc();
        }

        return 0;
    }
#endif

    void        M4free      (void * ai_pBlock, m4uint32_t ai_iPage);
    m4bool_t    FindAddress (m4uint8_t * ai_pBlock, m4uint32_t &ao_iPage);

#if _M4_DEBUG > 10
    void DumpStatistic(ofstream & ofs);
#endif

private:

    void AllocPage (void);

    m4uint8_t * FastMalloc (void)
    {
    // Resto 8 bytes para tener la direccion original, contando los 8 bytes que se reservan
        m4uint8_t * auxpointer = m_pFirstFreeblock - 8;
    // Me guardo en los 8 bytes previos el puntero al puntero a la primera direccion libre
        * (m4uint8_t ***) auxpointer = &m_pFirstFreeblock;

        m_pFirstFreeblock = *(m4uint8_t **)m_pFirstFreeblock;
        return (auxpointer + 8);
    }

    m4uint8_t * FastCalloc (void)
    {
    // Resto 8 bytes para tener la direccion original, contando los 8 bytes que se reservan
        m4uint8_t * auxpointer = m_pFirstFreeblock - 8;
    // Me guardo en los 8 bytes previos el puntero al puntero a la primera direccion libre
        * (m4uint8_t ***) auxpointer = &m_pFirstFreeblock;

        m_pFirstFreeblock = *(m4uint8_t **)m_pFirstFreeblock;
        memset (auxpointer + 8, '\0', m_iAllocSize);
        return (auxpointer + 8);
    }


    m4uint8_t  * m_pFirstFreeblock, ** m_pPageAddresses;
    m4uint32_t m_iAllocSize, m_iPageNumber, m_iBlocksInPage;

#if _M4_DEBUG > 10
    m4uint32_t * m_piUsedBlocks;
#endif
};

class _ClMemoryManager
{
public:

    _ClMemoryManager()
    {
    }

    virtual ~_ClMemoryManager()
    {
    }

    virtual m4uint8_t * M4malloc (m4uint32_t ai_iSize) = 0;
    virtual m4uint8_t * M4calloc (m4uint32_t ai_iSize) = 0;
    virtual m4uint8_t * M4calloc (m4uint32_t ai_Num, m4uint32_t ai_iSize) = 0;
    virtual void        M4free (void * ai_pBlock) = 0;
};

class ClMemoryManager : public _ClMemoryManager
{
public:

    ClMemoryManager(m4uint32_t ai_iMin = 1, m4uint32_t ai_iMax = 256);

   ~ClMemoryManager(void);

    virtual m4uint8_t * M4malloc (m4uint32_t ai_iSize)
    {

    #if _M4_DEBUG > 10
        ofstream ofs;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		M4Open( ofs, "C:\\TEMP\\allocations.txt", M4UniAppend, eEncoding );

        ofs << "Malloc: " << ai_iSize << endl;
        DumpStatistic();
        ofs.close();
    #endif

        if ((m_iMin <= ai_iSize)&&(ai_iSize <= m_iMax))
            return m_oMemoryMap[ai_iSize - m_iMin]->M4malloc();
        else
        {
            m4uint8_t * auxpointer = (m4uint8_t *) malloc(ai_iSize + 8);
			CHECK_CHLOG_ERRORF( auxpointer == NULL, NULL, M4_CH_DM_NO_MEMORY, ai_iSize + 8 << __LINE__ << __FILE__ ) ;

            // La cabecera a 0 me indica un malloc externo
            memset (auxpointer, '\0', 8);

            return auxpointer + 8;
        }
    }

    virtual m4uint8_t * M4calloc (m4uint32_t ai_iSize)
    {
    #if _M4_DEBUG > 10
        ofstream ofs;
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		M4Open( ofs, "C:\\TEMP\\allocations.txt", M4UniAppend, eEncoding );
        ofs << "Calloc: " << ai_iSize << endl;
        ofs.close();
        DumpStatistic();
    #endif

        if ((m_iMin <= ai_iSize)&&(ai_iSize <= m_iMax))
            return m_oMemoryMap[ai_iSize - m_iMin]->M4calloc();
        else
        {
            // La cabecera a 0 me indica un calloc externo, que ya esta a 0 por ser calloc
            m4uint8_t * auxpointer = (m4uint8_t *) calloc(1, ai_iSize + 8);
			CHECK_CHLOG_ERRORF( auxpointer == NULL, NULL, M4_CH_DM_NO_MEMORY, ai_iSize + 8 << __LINE__ << __FILE__ ) ;

            return auxpointer + 8;
        }
    }

    virtual void M4free (void * ai_pBlock)
    {
        m4uint8_t * auxpointer = ((m4uint8_t *)ai_pBlock) - 8;
        m4uint8_t ** pFirstFree = * (m4uint8_t ***) auxpointer;

        if (pFirstFree != 0)
        {
            * (m4uint8_t **) ai_pBlock = * pFirstFree;
            * pFirstFree = (m4uint8_t *) ai_pBlock;
        }

        else
            free((m4uint8_t *) ai_pBlock - 8);
    }


    virtual m4uint8_t * M4calloc (m4uint32_t ai_Num, m4uint32_t ai_iSize)
    {
        return M4calloc (ai_Num * ai_iSize);
    }


#if _M4_DEBUG > 10
    void DumpStatistic(void);
#endif

protected:

    ClMemoryProvider ** m_oMemoryMap; // Mapa para todos los tamaños, es una indireccion 
    ClMemoryProvider ** m_oRealProvider; //Lista de los verdaderos porveedores de memoria son menos.
    m4uint32_t m_iMin, m_iMax, m_iLogMin, m_iLogMax;
};

class ClMemoryManagerNotPool : public _ClMemoryManager
{
public:

    ClMemoryManagerNotPool(void)
    {
    }

    virtual m4uint8_t * M4malloc (m4uint32_t ai_iSize)
    {
        m4uint8_t * auxpointer = (m4uint8_t *) malloc (ai_iSize);
		CHECK_CHLOG_ERRORF( auxpointer == NULL, NULL, M4_CH_DM_NO_MEMORY, ai_iSize << __LINE__ << __FILE__ ) ;

        return auxpointer;
    }

    virtual m4uint8_t * M4calloc (m4uint32_t ai_iSize)
    {
        m4uint8_t * auxpointer = (m4uint8_t *) calloc (1, ai_iSize);
		CHECK_CHLOG_ERRORF( auxpointer == NULL, NULL, M4_CH_DM_NO_MEMORY, ai_iSize << __LINE__ << __FILE__ ) ;

        return auxpointer;
    }

    virtual m4uint8_t * M4calloc (m4uint32_t ai_Num, m4uint32_t ai_iSize)
    {
        m4uint8_t * auxpointer = (m4uint8_t *) calloc (ai_Num, ai_iSize);
		CHECK_CHLOG_ERRORF( auxpointer == NULL, NULL, M4_CH_DM_NO_MEMORY, ai_Num * ai_iSize << __LINE__ << __FILE__ ) ;

        return auxpointer;
    }

    virtual void M4free (void * ai_pBlock)
    {
        free (ai_pBlock);
    }
};

#endif //__MEMMANAGER__HPP__
