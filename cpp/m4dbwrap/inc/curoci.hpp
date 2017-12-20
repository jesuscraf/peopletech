//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                curoci.hpp   
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

#ifndef  _CUROCI_HPP_
#define  _CUROCI_HPP_

//__________________________________________________________
//
//  Includes
//__________________________________________________________

#include "cursorba.hpp"


#ifdef DRIVER_OCI
#include "oci.h"
#endif


class ClDBConnection_oci;


struct stBindResult
{
	m4int8_t	m_iM4Type;
	m4pchar_t*	m_ppcResult;
	m4pint32_t	m_piLength;
	m4pint32_t	m_piMaxLength;
	m4bool_t	m_bLongRead;

#ifdef DRIVER_OCI
	sb4			m_lSize;				/* Size of the storage area */
	OCIDefine*	m_lBind;
	ub1*		m_pBuf;					/* The area to store the column */
	sb2*		m_pInd;					/* Indicator variable for this column */
	ub2*		m_pLen;					/* Fetched length of the column */
	ub2*		m_pRcode;				/* Array of return codes */
	sb4			m_lSizeItem;			/* Size of the storage area in the Item*/
#endif

	stBindResult (void)
	{
		m_iM4Type = 0;
		m_ppcResult = NULL;
		m_piLength = NULL;
		m_piMaxLength = NULL;
		m_bLongRead = M4_TRUE;

#ifdef DRIVER_OCI
		m_lSize = 0;
		m_lBind = NULL;
		m_pBuf = NULL;
		m_pInd = NULL;
		m_pLen = NULL;
		m_pRcode = NULL;
		m_lSizeItem = -1;
#endif
	}
};



struct stBindParam 
{
	m4int8_t		m_iM4Type;
	m4int32_t		m_lPrec;
	m4int32_t		m_lScale;
	m4pint32_t		m_piLength;
	m4int32_t		m_indPar;
	m4pchar_t		m_pBuf;
	m4pchar_t*		m_ppBuf;
	m4int32_t		m_lwcSize;
	unsigned short*	m_pwcData;

#ifdef DRIVER_OCI
	OCIBind*		m_lBind;
	sb2				m_pInd;
	OCIDate			m_dDate;
#endif

	stBindParam (void)
	{
		m_iM4Type = 0;
		m_lPrec = 0;
		m_lScale = 0;
		m_piLength = NULL;
		m_indPar = 0;
		m_pBuf = NULL;
		m_ppBuf = NULL;
		m_lwcSize = 0;
		m_pwcData = NULL;

#ifdef DRIVER_OCI
		m_lBind = NULL;
		m_pInd = 0;
#endif
	}
};

typedef struct stBindResult* PstBindResult;
typedef struct stBindParam * PstBindParam;


class ClCursor_oci: public ClCursor_Base
{
protected:
    ClDBConnection_oci *m_pConn;

public:

	ClCursor_oci (void);
	virtual ~ClCursor_oci (void);

	virtual m4return_t Open (ClDBConnection_Base *);

	virtual m4return_t Close (void);

	virtual m4return_t Destroy (void);

	// binda una variable para que coloque ahi los resultados
	// posicion de la columna, variable donde se deja el resultado, longitud de esta)
	virtual m4return_t BindParam (m4int32_t ai_lCol, m4pchar_t ai_pValue, m4pchar_t *ai_ppValue, m4int32_t ai_lPrec, m4int32_t ai_lM4Type, m4int32_t ai_lScale, m4pint32_t ao_lLength);

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

private:
	m4bool_t m_bInvExecFetch;
	m4bool_t m_bIsSelect;
	PstBindResult m_pstBindResult;
	PstBindParam m_pstBindParam;
	m4int32_t m_lnumBindResult;  
	m4int32_t m_lnumBindParam;  
	m4int32_t m_lRemainder;
	m4int32_t m_lNumRow;
	m4bool_t m_bLongExist;
	m4bool_t m_bRebindAllParameters;

#ifdef DRIVER_OCI
	OCIStmt *m_hStmt;
	m4int8_t OracleToM4Type (OCITypeCode datatype);
	sword ReadLong (m4int32_t ai_indCol);
#endif
	
};


#ifdef _M4_USING_INLINE
	#include "curoci.inl"
#endif


#endif
