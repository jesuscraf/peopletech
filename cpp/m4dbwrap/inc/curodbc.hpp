//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                curodbc.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef  _CURODBC_HPP_
#define  _CURODBC_HPP_

//__________________________________________________________
//
//  Includes
//__________________________________________________________

#include "cursorba.hpp"


#define ODBCVER 0x0351

#include "sqlext.h"
#include "m4stl.hpp"
#include "cldates.hpp"

class ClDBConnection_odbc;


struct StTimeStamp
{
	m4date_t*			m_pdDate;
	TIMESTAMP_STRUCT	m_stTimestamp;

	StTimeStamp( m4pchar_t ai_pData )
	{
		m_pdDate = (m4date_t*) ai_pData;
		memset( &m_stTimestamp, 0, sizeof(TIMESTAMP_STRUCT) );
	}
};


// UNICODE DB
struct StUniStringParam
{
	m4pchar_t	m_pcData;
	m4pchar_t	*m_ppcData;
	m4int32_t	m_iPrec;
	m4bool_t	m_bIsUnicode;
	void		*m_pvData;

	StUniStringParam( m4pchar_t ai_pcData, m4pchar_t *ai_ppcData, m4int32_t ai_iPrec, m4bool_t ai_bIsUnicode )
	{
		m_pcData = ai_pcData;

		if( ai_ppcData != NULL )
		{
			m_ppcData = ai_ppcData;
		}
		else
		{
			m_ppcData = &m_pcData;
		}
		m_iPrec = ai_iPrec;
		m_bIsUnicode = ai_bIsUnicode;

		if( m_bIsUnicode == M4_TRUE )
		{
			m_pvData = new unsigned short[ m_iPrec + 1 ];
		}
		else
		{
			m_pvData = new unsigned char[ m_iPrec + 1 ];
		}
	}
};


struct StUniStringResult
{
	m4pchar_t*	m_ppcData;
	m4pint32_t	m_piMaxLength;
	m4bool_t	m_bIsUnicode;
	void*		m_pvData;

	StUniStringResult( m4pchar_t *ai_ppcData, m4pint32_t ai_piMaxLength, m4bool_t ai_bIsUnicode )
	{
		m_ppcData = ai_ppcData;
		m_piMaxLength = ai_piMaxLength;
		m_bIsUnicode = ai_bIsUnicode;

		if( m_bIsUnicode == M4_TRUE )
		{
			m_pvData = new unsigned short[ *m_piMaxLength + 1 ];
		}
		else
		{
			m_pvData = new unsigned char[ *m_piMaxLength + 1 ];
		}
	}
};


struct StLong
{
	m4int32_t	m_indParam;
	m4pchar_t*	m_ppcData;
	m4int32_t	m_iM4Type;
	m4bool_t	m_bIsUnicode;
	void*		m_pUniData;

	StLong( m4int32_t ai_indParam, m4pchar_t *ai_ppcData, m4int32_t ai_iM4Type, m4bool_t ai_bIsUnicode )
	{
		m_indParam = ai_indParam;
		m_ppcData = ai_ppcData;
		m_iM4Type = ai_iM4Type;
		m_bIsUnicode = ai_bIsUnicode;
		m_pUniData = NULL;
	}
};


struct StParamBinding
{
	m4pint32_t			m_piSourceLength;
	SQLLEN				m_iBindingLength;

	StTimeStamp			*m_poTimeStamp;
	StUniStringParam	*m_poUniString;
	StLong				*m_poLongString;

	StParamBinding( m4pint32_t ai_piSourceLength)
	{
		m_piSourceLength = ai_piSourceLength;
		m_iBindingLength = *ai_piSourceLength;

		m_poTimeStamp = NULL;
		m_poUniString = NULL;
		m_poLongString = NULL;
	}
};


struct StResultBinding
{
	m4pint32_t			m_piSourceLength;
	SQLLEN				m_iBindingLength;

	StTimeStamp			*m_poTimeStamp;
	StUniStringResult	*m_poUniString;

	StResultBinding( m4pint32_t ai_piSourceLength)
	{
		m_piSourceLength = ai_piSourceLength;
		m_iBindingLength = *ai_piSourceLength;

		m_poTimeStamp = NULL;
		m_poUniString = NULL;
	}
};


class ClCursor_odbc: public ClCursor_Base
{
protected:
  
	HSTMT m_hStmt;
	ClDBConnection_odbc *m_pConn;

	vector<StParamBinding*> m_vpcParamBinding;
	vector<StResultBinding*> m_vpcResultBinding;

	SQLSMALLINT m_indError;
	m4bool_t	m_bBindBinaryZeroPrec;

public:

	ClCursor_odbc (void);

    virtual ~ClCursor_odbc (void);

    virtual m4return_t Open (ClDBConnection_Base *);

    virtual m4return_t Close (void);

    virtual m4return_t Destroy (void);

	// binda una variable para que coloque ahi los resultados
	// posicion de la columna, variable donde se deja el resultado, longitud de esta)
	virtual m4return_t BindParam (m4int32_t ai_indPar, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ao_lLength);

	// binda una variable para que coloque ahi los resultados
	// posicion de la columna, variable donde se deja el resultado, longitud de esta)
	virtual m4return_t BindResult (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4int32_t ai_lMaxLenFld = -1);

	virtual m4return_t GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength, m4bool_t ai_bByPass = M4_FALSE);

	virtual m4return_t Prepare (m4pcchar_t ai_pStmt, m4int32_t ai_numPar = 0, m4int32_t ai_numCol = -1, m4bool_t ai_bMultiRow = M4_TRUE);
      
    virtual m4return_t Execute (m4uint32_t ai_iTimeOut, ICancelStatusProvider* ai_poCancelStatus, m4return_t& ao_riCanceled);
      
    virtual m4return_t Fetch (void);

	virtual m4return_t DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_cM4Type);

    virtual m4int32_t GetNumCol (void);
      
	virtual m4return_t GetErrorString( m4return_t ai_retcode, m4int32_t& ao_iErrorCode, m4pchar_t *ao_ppcErrorString );

	virtual m4return_t SetBlockingMode( m4uint32_t ai_iMode );
}; 


#ifdef _M4_USING_INLINE
	#include "curodbc.inl"
#endif


#endif
