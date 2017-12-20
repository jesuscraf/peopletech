//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//          zdelta code
//	=====================================================================

#ifndef commpduzdelta_h
#define commpduzdelta_h 1

#include "m4communication_dll.hpp"
#include "commprotocols.hpp"
#include "m4stl.hpp"
#include "syncro.hpp"
#include "blocksynchronization.hpp"
#include "rollingfile.hpp"

// header size for a PDU compressed with the zdelta engine.
#define ZDELTA_HEADER_OFFSET 20

// the zdelta ID.
#define M4_PDU_ZDELTA_ID 0x7777

// a ZDelta (really a full PDU) container.
struct M4ZDeltaElement 
{
    m4pchar_t buffer;
    m4uint32_t size;
    m4uint32_t identifier;
	m4bool_t bJustDisconnectedSession;
	m4bool_t bJustCanceledRequest;
};


// map definitions for ZDelta.
typedef map<m4uint64_t, M4ZDeltaElement> M4ZDeltaElementMap_t;
typedef M4ZDeltaElementMap_t::iterator M4ZDeltaElementMapIt_t;


//
// This class, a singleton, is the manager/container of zdeltas.
//
class M4_DECL_M4COMMUNICATION M4ZDeltaManager
{
protected:
    // maps for input/output PDUs.
    M4ZDeltaElementMap_t m_oReferenceInMap;
    M4ZDeltaElementMap_t m_oReferenceOutMap;

    // map size.
    m4uint32_t m_InMapSize;
    m4uint32_t m_OutMapSize;
    
    // mutex.
    static ClMutex m_InMutex;
    static ClMutex m_OutMutex;
    
public:
    // singleton interface.
    static M4ZDeltaManager* GetInstance();

    // deletes an entry in all maps.
    void erase(m4uint64_t ai_session);

    // operations over the output map.
    M4ZDeltaElementMapIt_t find_out(m4uint64_t ai_session);    
    M4ZDeltaElementMapIt_t end_out();    
    size_t erase_out(m4uint64_t ai_session);    
    void insert_out(m4uint64_t ai_session, M4ZDeltaElement& ai_oElm);
        
    // operations over the input map.
    M4ZDeltaElementMapIt_t find_in(m4uint64_t ai_session);    
    M4ZDeltaElementMapIt_t end_in();    
    size_t erase_in(m4uint64_t ai_session);    
    void insert_in(m4uint64_t ai_session, M4ZDeltaElement& ai_oElm);
    
    // traces.
    void setTrace(m4bool_t ai_enable, m4string_t ai_traceDirName);
    void trace(m4uint32_t ai_uiInitTickCount, 
        m4uint64_t ai_iSessionId, m4uint32_t ai_iIdentifier, m4pcchar_t ai_pcOperationId, 
        m4uint32_t ai_uiSourceSize, m4uint32_t ai_uiReferenceSize, m4uint32_t ai_uiTargetSize, 
        m4uint32_t ai_uiEndTickCount);
    
protected:
    M4ZDeltaElementMapIt_t _find(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session);
    M4ZDeltaElementMapIt_t _end(M4ZDeltaElementMap_t& ai_map);
    size_t _erase(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session);
    void _insert(M4ZDeltaElementMap_t& ai_map, m4uint64_t ai_session, M4ZDeltaElement& ai_oElm);

    // traces.
    m4bool_t m_isTraceEnable;
    ClRollingFile m_traceFile;

private:
    // private constructor (singleton)
    M4ZDeltaManager();
    static M4ZDeltaManager* m_poInstance;
};



// wrapper for ClCommPDUProtocol/ClCommProtocol 
class M4_DECL_M4COMMUNICATION ClPDUCommZDelta 
{   
public:    
    // ctor, dtor.
    ClPDUCommZDelta();
    ~ClPDUCommZDelta();

    // for compression
    m4return_t DoCompress(ClPDU* ai_oPDU, m4pchar_t& ao_pcBuffer, m4uint32_t& ao_uiLen);
    
    // for decompression
    m4return_t AddChunk(m4pchar_t ai_pcChunk, m4uint32_t ai_iChunkSize, m4bool_t ai_bUpdtDoDelta, m4bool_t& ao_bFinish);
    m4return_t DoDeCompress(m4pchar_t& aio_pcBuffer, m4uint32_t& ao_uiLen);
        
    m4bool_t DoDelta() 
    { 
        return m_bDoDelta; 
    }
    
    m4uint32_t GetNextBlockSize() 
    { 
        return m_iSize; 
    }

    // reset the state of this object.
    void Reset(ClPDU* ai_oPDU);
    
    // return TRUE if the input PDU has FileDataUnits.
    static m4bool_t CheckForFiles(ClPDU* ai_oPDU);

    m4uint32_t GetCompressionType()
    {
        return m_iCompressionType;
    }
    m4uint32_t GetSessionId()
    {
        return m_iSessionId;
    }

	static m4uint64_t MakeKey( m4uint32_t ai_iSesssion, m4uint32_t ai_iPreKey, m4bool_t ai_bClientSide ) ;
	static m4return_t ExtractKey( m4uint64_t ai_iKey, m4uint32_t& ao_iSesssion, m4uint32_t& ao_iPreKey ) ;

protected:
    m4uint32_t m_iSize;
    m4uint32_t m_iIdentifier;
    m4bool_t m_bDoDelta;
    m4uint32_t m_iSessionId;
    m4uint32_t m_iCompressionType;
	m4int8_t   m_iGapBetweenIdentifiers;
    
    m4return_t ComposeOutput(m4uint32_t ai_iSesssion, m4pchar_t aio_pcInput, m4uint32_t ai_iInputSize, m4uint32_t ai_iIdentifier, m4pchar_t& ao_pcCompressed, m4uint32_t& ao_iSize);
    m4void_t GetHeader(m4pchar_t ai_pcBuffer, m4uint16_t& ao_iType, m4uint32_t& ao_iSize, m4uint32_t& ao_iIdentifier, m4uint32_t& ao_iSession);

	static	ClMutex		m_oMutex ;
	static	m4int32_t	s_iInstCounter ;
	m4int32_t			m_iInstNum ;
};


/*
 * class ClPDUWithDataForZDelta
 * we need have access to m_poDataStorage without change the PDU internal state.
 */
class M4_DECL_M4COMMUNICATION ClPDUWithDataForZDelta : public ClPDUWithData 
{
public:
	M4DataStorage *GetDataStorageWrapper();
};


#endif
