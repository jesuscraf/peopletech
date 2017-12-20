//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlutapi.cpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 21/10/2015
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines Unit Testing API and utilities.
//
//==============================================================================

#include <m4stl.hpp>
#include <m4xlutapi.hpp>
#include <m4xlutint.hpp>
#include <m4xlhelper.hpp>
#include <access.hpp>
#include <wchar.h>


#define ASSERT_EQUALS( X, Y, Z)	{ if( !equals( X, Y ) ){ SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", Z ); return false; } }
#define ASSERT_EQUALS_N1( X, Y, N1, Z)	{ if( !equals( X, Y ) ){ SETCODEF( M4_XL_UT_GEN_ERR_N1, ERRORLOG, "#*s1*#%s#%d#", Z, N1 ); return false; } }
#define ASSERT_EQUALS_N2( X, Y, N1, N2, Z)	{ if( !equals( X, Y ) ){ SETCODEF( M4_XL_UT_GEN_ERR_N2, ERRORLOG, "#*s1*#%s#%d#%d#", Z, N1, N2 ); return false; } }

typedef map< int, int, less< int > >	CounterPerColumn_t;
typedef CounterPerColumn_t::iterator	CounterPerColumnIt_t;


// -----------------------------------------------------------------------------
// Unit testing helper functions.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Integer equality.
// -----------------------------------------------------------------------------
bool equals( int ai_i1, int ai_i2 )
{
	return( ai_i1 == ai_i2 );
}


// -----------------------------------------------------------------------------
// Double equality.
// -----------------------------------------------------------------------------
bool equals( double ai_d1, double ai_d2 )
{
	// When used for date-time/time values there is a difference in the last
	// decimal places so truncates it to 10.
	char acVal[350];

	sprintf( acVal, "%.10f", ai_d1 );
	double dVal1 = atof( acVal );
	
	sprintf( acVal, "%.10f", ai_d2 );
	double dVal2 = atof( acVal );

	return( dVal1 == dVal2 );
}


// -----------------------------------------------------------------------------
// String equality.
// -----------------------------------------------------------------------------
bool equals( const wchar_t* ai_pwc1, const wchar_t* ai_pwc2 )
{
	bool	bEq;


	if( ai_pwc1 == NULL && ai_pwc2 == NULL )
	{
		return true;
	}
	
	if( ai_pwc1 != NULL && ai_pwc2 != NULL )
	{
		bEq = !wcscmp( ai_pwc1, ai_pwc2);
		
		return( bEq );
	}


	return false;
}


// -----------------------------------------------------------------------------
// Unit testing public API.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Compares two Excel workbook structs.
// 
//	ai_pcWB1:		File 1 path.
//	ai_pcWB2:		File 2 path.
//	ai_pvContext:	VM context.
//	ao_bEqual:		Output. Compare result.
//
//	Return:			M4_SUCCESS/M4_ERROR
// -----------------------------------------------------------------------------
m4return_t CmpWbStruct( m4pchar_t ai_pcWB1, m4pchar_t ai_pcWB2, m4pvoid_t ai_pvContext, m4bool_t& ao_bEqual )
{
	ClUTConfig	oCfg( ( ClVMRunContext* )ai_pvContext );

	
	ao_bEqual = M4_FALSE;

	if( ai_pcWB1 == NULL || ai_pcWB2 == NULL )
	{
		return M4_ERROR;
	}

	ClWbook	oWb1( ai_pcWB1, &oCfg );

	if( oWb1.GetIsLoaded() == false )
	{
		return M4_ERROR;
	}

	ClWbook	oWb2( ai_pcWB2, &oCfg );

	if( oWb2.GetIsLoaded() == false )
	{
		return M4_ERROR;
	}

	ao_bEqual = ( oWb1 == oWb2 );


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Compares two Excel worksheets.
// 
//	ai_pcWB1:			File 1 path.
//	ai_pcWB2:			[Optional] File 2 path.
//	ai_iWS1:			Worksheet 1 index.
//	ai_iWS2:			Worksheet 2 index.
//	ai_iFR:				First row.
//	ai_iFC:				First column.
//	ai_iLR:				[Optional] Last row.
//	ai_iLC:				[Optional] Last column.
//	ai_pcIgnoreList:	[Optional] List of cells to ignore.
//	ai_pvContext:		VM context.
//	ao_bEqual:			Output. Compare result.
// -----------------------------------------------------------------------------
m4return_t CmpWsContent( m4pchar_t ai_pcWB1, m4pchar_t ai_pcWB2, int ai_iWS1, int ai_iWS2, int ai_iFR, int ai_iFC, int ai_iLR, int ai_iLC, m4pchar_t ai_pcIgnoreList, m4pvoid_t ai_pvContext, m4bool_t& ao_bEqual )
{
	m4bool_t		bEq;
	int				iLR, iLC;
	ClUTConfig		oCfg( ( ClVMRunContext* )ai_pvContext );
	ClDeleteThem	oDeleter;
	

	ao_bEqual = M4_FALSE;

	oDeleter.m_pWbook1 = new ClWbook( ai_pcWB1, &oCfg );

	if( oDeleter.m_pWbook1->GetIsLoaded() == false )
	{
		return M4_ERROR;
	}

	oDeleter.m_pWsheet1 = new ClWsheet( oDeleter.m_pWbook1, ai_iWS1 );

	if( oDeleter.m_pWsheet1->GetIsLoaded() == false )
	{
		return M4_ERROR;
	}
	
	if( ai_pcWB2 == NULL )
	{
		oDeleter.m_pWsheet2 = new ClWsheet( oDeleter.m_pWbook1, ai_iWS2 );
	}
	else
	{
		oDeleter.m_pWbook2 = new ClWbook( ai_pcWB2, &oCfg );

		if( oDeleter.m_pWbook2->GetIsLoaded() == false )
		{
			return M4_ERROR;
		}
		
		oDeleter.m_pWsheet2 = new ClWsheet( oDeleter.m_pWbook2, ai_iWS2 );
	}

	if( oDeleter.m_pWsheet2->GetIsLoaded() == false )
	{
		return M4_ERROR;
	}

	ao_bEqual = oDeleter.m_pWsheet1->IsEqual( oDeleter.m_pWsheet2, ai_iFR, ai_iFC, ai_iLR, ai_iLC, ai_pcIgnoreList );


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Unit testing helper classes.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Class ClUTConfig
// -----------------------------------------------------------------------------

const char*	ClUTConfig::c_sIemCfgScanXEmpty		= "CFG_SCAN_X_EMPTY";
const char*	ClUTConfig::c_sItemCfgTouchScanned	= "CFG_TOUCH_SCANNED";
const char*	ClUTConfig::c_sItemCfgTraceActive	= "CFG_TRACE_ACTIVE";
const char*	ClUTConfig::c_sItemCgfTraceFile		= "CFG_TRACE_FILE";
const int	ClUTConfig::c_iScanTillLastCell		= -1;
const int	ClUTConfig::c_iScanTillXEmpty		= -2;

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClUTConfig::ClUTConfig( ClVMRunContext*	ai_pVMCtx ): m_iScanXEmpty( 20 ), m_bTouchScanned( false )
{
	m4VariantType	vVal;


	m_pVMCtx = ai_pVMCtx;

	if( m_pVMCtx == NULL )
	{
		return;
	}
	
	( m_pVMCtx->m_pRecordSet )->Current.Item[ c_sIemCfgScanXEmpty ].Value.Get( vVal );
	m_iScanXEmpty = vVal.Data.DoubleData;
	
	if( m_iScanXEmpty < 0 )
	{
		m_iScanXEmpty = 20;
	}

	( m_pVMCtx->m_pRecordSet )->Current.Item[ c_sItemCfgTouchScanned ].Value.Get( vVal );
	m_bTouchScanned = vVal.Data.DoubleData;

	( m_pVMCtx->m_pRecordSet )->Current.Item[ c_sItemCfgTraceActive ].Value.Get( vVal );
	m_bTraceActive = vVal.Data.DoubleData;

	( m_pVMCtx->m_pRecordSet )->Current.Item[ c_sItemCgfTraceFile ].Value.Get( vVal );
	m_pcTraceFile = vVal.Data.PointerChar;
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClUTConfig::~ClUTConfig()
{}


// -----------------------------------------------------------------------------
// X empty cells getter.
// -----------------------------------------------------------------------------
int ClUTConfig::GetScanXEmpty()
{
	return m_iScanXEmpty;
}


// -----------------------------------------------------------------------------
// Scan touch getter.
// -----------------------------------------------------------------------------
bool ClUTConfig::GetTouchScanned()
{
	return m_bTouchScanned;
}


// -----------------------------------------------------------------------------
// Trace active getter.
// -----------------------------------------------------------------------------
bool ClUTConfig::GetTraceActive()
{
	return m_bTraceActive;
}


// -----------------------------------------------------------------------------
// Trace file getter.
// -----------------------------------------------------------------------------
char* ClUTConfig::GetTraceFile()
{
	return m_pcTraceFile;
}


// -----------------------------------------------------------------------------
// Class ClWbook
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClWbook::ClWbook( const char* ai_pcPath, ClUTConfig* ai_pCfg ) : m_pcPath( ai_pcPath ), m_bIsXML( true ), m_bIsLoaded( false ), m_pXlWb( NULL ), m_pCfg( ai_pCfg )
{
	loadXlWb();
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClWbook::~ClWbook()
{
	if( m_bIsLoaded == true )
	{
		releaseXlWb();
	}
}
		

// -----------------------------------------------------------------------------
// Equality operator for workbook structs.
// -----------------------------------------------------------------------------
bool ClWbook::operator==( const ClWbook& ai_Wbook )
{
	int		iWsCount;
	Sheet	*pWs1, *pWs2;


	if( m_bIsLoaded == false || ai_Wbook.m_bIsLoaded == false )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Workbook not open while comparing workbooks structure" );
		return false;
	}

	iWsCount = m_pXlWb->sheetCount();

	ASSERT_EQUALS( iWsCount, ai_Wbook.m_pXlWb->sheetCount(), "Different sheet count" )

	for( int iIdx = 0; iIdx < iWsCount; iIdx++ )
	{
		pWs1 = m_pXlWb->getSheet( iIdx );
		pWs2 = ai_Wbook.m_pXlWb->getSheet( iIdx );

		ASSERT_EQUALS_N1( pWs1->name(),    pWs2->name(),    iIdx, "Different sheet name" )
		ASSERT_EQUALS_N1( pWs1->protect(), pWs2->protect(), iIdx, "Different sheet protection" )
		ASSERT_EQUALS_N1( pWs1->hidden(),  pWs2->hidden(),  iIdx, "Different sheet hidden state" )
	}


	return true;
}


// -----------------------------------------------------------------------------
// Returns if the workbook has xml format.
// -----------------------------------------------------------------------------
bool ClWbook::IsXml()
{
	return m_bIsXML;
}


// -----------------------------------------------------------------------------
// Config object getter. 
// -----------------------------------------------------------------------------
ClUTConfig* ClWbook::GetCfg()
{
	return m_pCfg;
}


// -----------------------------------------------------------------------------
// Retrieves a libxl sheet object
// -----------------------------------------------------------------------------
Sheet* ClWbook::GetXlWs( int ai_iIdx )
{
	if( m_bIsLoaded == false )
	{
		return NULL;
	}

	return m_pXlWb->getSheet( ai_iIdx );
}


// -----------------------------------------------------------------------------
// Is loaded getter. 
// -----------------------------------------------------------------------------
bool ClWbook::GetIsLoaded()
{
	return m_bIsLoaded;
}


// -----------------------------------------------------------------------------
// Saves the workbook. 
// -----------------------------------------------------------------------------
bool ClWbook::SaveXlWb()
{
	wchar_t*	pwcFile = NULL;
	bool		bRet;


	if( m_bIsLoaded == false )
	{
		return false;
	}

	CppStrToLibxlStr( m_pcPath, pwcFile );
	bRet = m_pXlWb->save( pwcFile );

	delete [] pwcFile;


	return bRet;
}


// -----------------------------------------------------------------------------
// Private. Loads the workbook.
// -----------------------------------------------------------------------------
m4return_t ClWbook::loadXlWb()
{
	m4return_t	iRet;
	wchar_t*	pwcFile = NULL;
	bool		bRet;


	if( m_bIsLoaded == true )
	{
		return M4_SUCCESS;
	}

	iRet = IsXMLWorkbook( m_pcPath, m_bIsXML );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Bad file path while opening workbook" );
		return M4_ERROR;
	}

	if( m_bIsXML == M4_TRUE )
	{
		m_pXlWb = xlCreateXMLBook();
	}
	else
	{
		m_pXlWb = xlCreateBook();
	}

	if( m_pXlWb == NULL )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Null object while opening workbook" );
		return M4_ERROR;
	}

	iRet = CppStrToLibxlStr( m_pcPath, pwcFile );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Error encoding path while opening workbook" );
		return M4_ERROR;
	}

	bRet = m_pXlWb->load( pwcFile );

	delete [] pwcFile;

	if( bRet == false )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Load error while opening workbook" );
		return M4_ERROR;
	}

	m_bIsLoaded = true;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Private. Closes the workbook.
// -----------------------------------------------------------------------------
m4return_t ClWbook::releaseXlWb()
{
	if( m_bIsLoaded == false )
	{
		return M4_SUCCESS;
	}

	m_pXlWb->release();

	m_bIsLoaded = false;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Class ClWsheet
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClWsheet::ClWsheet( ClWbook* ai_pWbook, int ai_iIdx ) : m_pWbook( ai_pWbook ), m_iIdx( ai_iIdx ), m_bIsLoaded( false ), m_pXlWs( NULL )
{
	loadXlWs();
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClWsheet::~ClWsheet()
{
	if( m_bIsLoaded == true )
	{
		dbgSaveWb();
	}
}


// -----------------------------------------------------------------------------
// Worksheets comparator.
// -----------------------------------------------------------------------------
bool ClWsheet::IsEqual( ClWsheet* ai_pWsheet, int ai_iFR, int ai_iFC, int ai_iLR, int ai_iLC, char* ai_pcIgnoreList )
{
	ClCell					oCell1( this ), oCell2( ai_pWsheet );
	int						iEmptiesInRow;
	CounterPerColumn_t		vEmptiesInCol;
	CounterPerColumnIt_t	it;
	int						iColTop = 0;
	ClUTConfig*				pCfg = m_pWbook->GetCfg();
	ClCellPos				oCellPos;
	CellPosSet_t			setIgnoreList;
	bool					bIgnoreCell;


	if( m_bIsLoaded == false || ai_pWsheet->m_bIsLoaded == false )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Worksheet not loaded while comparing worksheets content" );
		return false;
	}

	dbgInitTrace();


	if( ai_iLR == ClUTConfig::c_iScanTillLastCell )
	{
		ai_iLR = m_pXlWs->lastRow() - 1;
	}

	if( ai_iLC == ClUTConfig::c_iScanTillLastCell )
	{
		ai_iLC = m_pXlWs->lastCol() - 1;
	}

	if( ai_iLR != ClUTConfig::c_iScanTillXEmpty && ai_iFR > ai_iLR )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Bad range to compare" );
		return false;
	}

	if( ai_iLC != ClUTConfig::c_iScanTillXEmpty && ai_iFC > ai_iLC )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Bad range to compare" );
		return false;
	}

	if( ( ai_iLC == ClUTConfig::c_iScanTillXEmpty ) ^ ( ai_iLR == ClUTConfig::c_iScanTillXEmpty ) )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Unsupported mixed mode including scan till X empty" );
		return false;
	}

	if( ai_pcIgnoreList != NULL )
	{
		parseCellList( ai_pcIgnoreList, setIgnoreList );
	}

	if( ai_pWsheet->GetWbook() != NULL &&  GetWbook() != ai_pWsheet->GetWbook() )
	{
		ASSERT_EQUALS( GetXlWs()->name(),    ai_pWsheet->GetXlWs()->name(),    "Different sheet name" )
	}
	ASSERT_EQUALS( GetXlWs()->protect(), ai_pWsheet->GetXlWs()->protect(), "Different sheet protection" )
	ASSERT_EQUALS( GetXlWs()->hidden(),  ai_pWsheet->GetXlWs()->hidden(),  "Different sheet hidden state" )


	for( int iRowIdx = ai_iFR; !iteStatEndCond( iRowIdx, ai_iLR, MSLIM_MAX_ROW ); iRowIdx++ )
	{
		ASSERT_EQUALS_N1( m_pXlWs->rowHidden( iRowIdx ), ai_pWsheet->GetXlWs()->rowHidden( iRowIdx ), iRowIdx, "Different row hidden state" )
		
		iEmptiesInRow = 0;
	
		for( int iColIdx = ai_iFC; !iteStatEndCond( iColIdx, ai_iLC, MSLIM_MAX_COL ); iColIdx++ )
		{
			ASSERT_EQUALS_N1( m_pXlWs->colHidden( iColIdx ), ai_pWsheet->GetXlWs()->colHidden( iColIdx ), iColIdx, "Different column hidden state" )

			bIgnoreCell = false;

			if( ai_pcIgnoreList != NULL )
			{
				oCellPos.Init( iRowIdx, iColIdx );

				if( setIgnoreList.find( oCellPos ) != setIgnoreList.end() )
				{
					bIgnoreCell = true;
				}
			}

			if( bIgnoreCell == false )
			{
				oCell1.Set( iRowIdx, iColIdx );
				oCell2.Set( iRowIdx, iColIdx );

				if( oCell1 != oCell2 )
				{
					dbgWriteTrace( "%s\t(%d, %d)", "*Diff", iRowIdx, iColIdx );
					return false;
				}
				else
				{
					dbgWriteTrace( "%s\t(%d, %d)", "Equal", iRowIdx, iColIdx );
				}
			}
			else
			{
				dbgWriteTrace( "%s\t(%d, %d)", "Ignore", iRowIdx, iColIdx );
			}

			if( ai_iLC == ClUTConfig::c_iScanTillXEmpty )
			{
				it = vEmptiesInCol.find( iColIdx );
				
				if( oCell1.IsEmpty() == true)
				{
					dbgTouchCell( iRowIdx, iColIdx, "·" );

					iEmptiesInRow++;

					if( it == vEmptiesInCol.end() )
					{
						vEmptiesInCol.insert( CounterPerColumn_t::value_type( iColIdx, 1 ) );
					}
					else
					{
						++( (*it).second );
					}

					if( iEmptiesInRow >= pCfg->GetScanXEmpty() && iColIdx > iColTop )
					{
						break;
					}

					if( iColIdx > iColTop )
					{
						iColTop = iColIdx;
					}
				}
				else
				{
					iEmptiesInRow = 0;

					if( it == vEmptiesInCol.end() )
					{
						vEmptiesInCol.insert( CounterPerColumn_t::value_type( iColIdx, 0 ) );
					}
					else
					{
						(*it).second = 0;
					}
				}
			}
		}
		
		if( ai_iLC == ClUTConfig::c_iScanTillXEmpty )
		{
			for( it = vEmptiesInCol.begin(); it != vEmptiesInCol.end(); it++ )
			{
				if( (*it).second < pCfg->GetScanXEmpty() )
				{
					break;
				}
			}

			if( it == vEmptiesInCol.end() )
			{
				return true;
			}
		}
	}

	
	return true;
}


// -----------------------------------------------------------------------------
// Workbook getter
// -----------------------------------------------------------------------------
ClWbook* ClWsheet::GetWbook()
{
	return m_pWbook;
}


// -----------------------------------------------------------------------------
// Is loaded getter. 
// -----------------------------------------------------------------------------
bool ClWsheet::GetIsLoaded()
{
	return m_bIsLoaded;
}


// -----------------------------------------------------------------------------
// Retrieves a libxl sheet object
// -----------------------------------------------------------------------------
Sheet* ClWsheet::GetXlWs()
{
	if( m_bIsLoaded == false )
	{
		return NULL;
	}


	return m_pXlWs;
}


// -----------------------------------------------------------------------------
// Private. Checks if a Ms Excel limit has been reached.
// -----------------------------------------------------------------------------
bool ClWsheet::outMSLimit( eMSLimit_t ai_iLimit, int ai_iVal )
{
	int iTop = 0;


	if( m_pWbook->IsXml() == true )
	{
		switch( ai_iLimit )
		{
			case MSLIM_MAX_ROW:
				iTop = 1048576;
				break;
			case MSLIM_MAX_COL:
				iTop = 65536;
				break;
		}
	}
	else
	{
		switch( ai_iLimit )
		{
			case MSLIM_MAX_ROW:
				iTop = 16384;
				break;
			case MSLIM_MAX_COL:
				iTop = 256;
				break;
		}
	}


	return ( ai_iVal >= iTop );
}


// -----------------------------------------------------------------------------
// Private. Marks a cell.
// -----------------------------------------------------------------------------
void ClWsheet::dbgTouchCell( int ai_iRow, int ai_iCol, char* ai_pcMsg )
{
	ClUTConfig*	pCfg = m_pWbook->GetCfg();
	wchar_t*	pwcVal;


	if( m_bIsLoaded == true && pCfg->GetTouchScanned() == true )
	{
		CppStrToLibxlStr( ai_pcMsg, pwcVal );

		m_pXlWs->writeStr( ai_iRow, ai_iCol, pwcVal );

		delete [] pwcVal;
		
		m_bWritePending = true;
	}
}


// -----------------------------------------------------------------------------
// Private. Saves the workbook.
// -----------------------------------------------------------------------------
void ClWsheet::dbgSaveWb()
{
	ClUTConfig*	pCfg = m_pWbook->GetCfg();


	if( m_bIsLoaded == true && m_bWritePending == true )
	{
		m_pWbook->SaveXlWb();

		m_bWritePending = false;
	}
}


// -----------------------------------------------------------------------------
// Resets trace file.
// -----------------------------------------------------------------------------
void ClWsheet::dbgInitTrace()
{
	if( m_pWbook->GetCfg()->GetTraceActive() == true )
	{
		FILE* pFile = fopen( m_pWbook->GetCfg()->GetTraceFile(), "wt" );

		if( pFile != NULL )
		{
			fclose( pFile ) ;
		}
	}
}


// -----------------------------------------------------------------------------
// Writes a message in the trace file.
// -----------------------------------------------------------------------------
#define MAX_MSG_LEN	2048
void ClWsheet::dbgWriteTrace( m4pcchar_t ai_pccFormat, ... )
{
#ifdef _WINDOWS
	time_t		tTimeNow ;
	char*		pcTimeNow ;
	FILE*		pFile;
	va_list		list ;
	m4char_t	acMsg[ MAX_MSG_LEN + 1 ] = "" ;


	if( m_pWbook->GetCfg()->GetTraceActive() == true )
	{
		pFile = fopen(  m_pWbook->GetCfg()->GetTraceFile(), "at" );

		if( pFile == NULL )
		{
			return;
		}

		time( &tTimeNow ) ;

		pcTimeNow = ctime( &tTimeNow ) ;

		if( pcTimeNow[ strlen( pcTimeNow ) - 1 ] == '\n' )
		{
			pcTimeNow[ strlen( pcTimeNow ) - 1 ] = '\0' ;
		}

		fprintf( pFile, "%s", pcTimeNow );

		va_start( list, ai_pccFormat ) ;

		_vsnprintf( acMsg, MAX_MSG_LEN, ai_pccFormat, list ) ;

		va_end( list ) ;

		fprintf( pFile, "\t%s\n", acMsg );

		fclose( pFile );
	}
#endif
}


// -----------------------------------------------------------------------------
// Private. Rows/columns iteration static end condition.
// -----------------------------------------------------------------------------
bool ClWsheet::iteStatEndCond( int ai_iIdx, int ai_iLast, eMSLimit_t ai_iLimit )
{
	if( outMSLimit( ai_iLimit, ai_iIdx ) == true )
	{
		return true;
	}

	if( ai_iLast != ClUTConfig::c_iScanTillXEmpty && ai_iIdx > ai_iLast )
	{
		return true;
	}


	return false;
}


// -----------------------------------------------------------------------------
// Private. Loads the worksheet.
// -----------------------------------------------------------------------------
m4return_t ClWsheet::loadXlWs()
{
	if( m_bIsLoaded == true )
	{
		return M4_SUCCESS;
	}

	m_pXlWs = m_pWbook->GetXlWs( m_iIdx );

	if( m_pXlWs == NULL )
	{
		SETCODEF( M4_XL_UT_GEN_ERR, ERRORLOG, "#*s1*#%s#", "Bad index while opening worksheet" );
		return M4_ERROR;
	}

	m_bIsLoaded = true;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Private. Parses a list of cell positions populating a set container.
// -----------------------------------------------------------------------------
int ClWsheet::parseCellList( char* ai_pcCells, CellPosSet_t& ao_setCells)
{
	size_t		iSize;
	char*		pcList;
	char*		pcToken;
	char*		pcCol;
	ClCellPos	oCellPos;


	if( ai_pcCells == NULL )
	{
		return 0;
	}

	iSize = strlen( ai_pcCells ) + 1;

	pcList = new char[ iSize ];
	memcpy( pcList, ai_pcCells, iSize );

	// Tokenize the list of cells.
	pcToken = strtok( pcList, ";" );

	while( pcToken != NULL )
	{
		pcCol = strstr( pcToken, "," );

		if( pcCol != NULL )
		{
			*pcCol++ = NULL;

			while( *pcToken == 32 )
			{
				pcToken++;
			}

			while( *pcCol == 32 )
			{
				pcCol++;
			}

			oCellPos.Init( atoi( pcToken ), atoi( pcCol ) );
			ao_setCells.insert( oCellPos );
		}

		pcToken = strtok( NULL, ";" );
	}

	delete pcList;

	return ao_setCells.size();
}


// -----------------------------------------------------------------------------
// Class ClCell
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClCell::ClCell( ClWsheet* ai_pWsheet ) : m_pWsheet( ai_pWsheet )
{
	Set( 0, 0 );
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCell::~ClCell()
{}


// -----------------------------------------------------------------------------
// Cell position setter.
// -----------------------------------------------------------------------------
void ClCell::Set( int ai_iRow, int ai_iCol )
{
	m_iRow = ai_iRow;
	m_iCol = ai_iCol;

	if( m_pWsheet->GetWbook()->IsXml() == false )
	{
		m_pwcComment = m_pWsheet->GetXlWs()->readComment( m_iRow, m_iCol );
	}
	else
	{
		m_pwcComment = NULL;
	}

	m_iType		= m_pWsheet->GetXlWs()->cellType( m_iRow, m_iCol );
	m_pFormat	= m_pWsheet->GetXlWs()->cellFormat( m_iRow, m_iCol );
	
	m_aiBorder[ BDR_EDGE_TOP ]		= m_pFormat->borderTop();
	m_aiBorder[ BDR_EDGE_RIGHT ]	= m_pFormat->borderRight();
	m_aiBorder[ BDR_EDGE_BOTTOM ]	= m_pFormat->borderBottom();
	m_aiBorder[ BDR_EDGE_LEFT ]		= m_pFormat->borderLeft();

	m_iPattern		= m_pFormat->fillPattern();
	m_iForeground	= m_pFormat->patternForegroundColor();
	m_iBackground	= m_pFormat->patternBackgroundColor();
}


// -----------------------------------------------------------------------------
// Equality operator for cells.
// -----------------------------------------------------------------------------
bool ClCell::operator==( const ClCell& ai_Cell )
{
	bool		bRet1, bRet2;
	int			iRF1, iRL1, iCF1, iCL1, iRF2, iRL2, iCF2, iCL2;
	Font		*pFont1, *pFont2;
	

	if( m_pWsheet->GetWbook()->IsXml() == false )
	{
		ASSERT_EQUALS_N2( m_pwcComment, ai_Cell.m_pwcComment, m_iRow, m_iCol, "Different comments" )
	}

	ASSERT_EQUALS_N1( m_pWsheet->GetXlWs()->colWidth( m_iCol ),  ai_Cell.m_pWsheet->GetXlWs()->colWidth( m_iCol ),  m_iCol, "Different column width" )
	ASSERT_EQUALS_N1( m_pWsheet->GetXlWs()->rowHeight( m_iRow ), ai_Cell.m_pWsheet->GetXlWs()->rowHeight( m_iRow ), m_iRow, "Different row height" )

	bRet1 = m_pWsheet->GetXlWs()->getMerge( m_iRow, m_iCol, &iRF1, &iRL1, &iCF1, &iCL1 );
	bRet2 = ai_Cell.m_pWsheet->GetXlWs()->getMerge( m_iRow, m_iCol, &iRF2, &iRL2, &iCF2, &iCL2 );

	ASSERT_EQUALS_N2( bRet1, bRet1, m_iRow, m_iCol, "Difference in merged cells" )

	if( bRet1 == true )
	{
		ASSERT_EQUALS_N2( iRF1, iRF2, m_iRow, m_iCol, "Difference in merged cells" )
		ASSERT_EQUALS_N2( iRL1, iRL2, m_iRow, m_iCol, "Difference in merged cells" )
		ASSERT_EQUALS_N2( iCF1, iCF2, m_iRow, m_iCol, "Difference in merged cells" )
		ASSERT_EQUALS_N2( iCL1, iCL2, m_iRow, m_iCol, "Difference in merged cells" )
	}

	ASSERT_EQUALS_N2( m_iType, ai_Cell.m_iType, m_iRow, m_iCol, "Different cell types" )

	switch( m_iType )
	{
		case CELLTYPE_NUMBER:
			ASSERT_EQUALS_N2( m_pWsheet->GetXlWs()->readNum( m_iRow, m_iCol ), ai_Cell.m_pWsheet->GetXlWs()->readNum( m_iRow, m_iCol ), m_iRow, m_iCol, "Different cell number value" )
			break;
		case CELLTYPE_STRING:
			ASSERT_EQUALS_N2( m_pWsheet->GetXlWs()->readStr( m_iRow, m_iCol ), ai_Cell.m_pWsheet->GetXlWs()->readStr( m_iRow, m_iCol ), m_iRow, m_iCol, "Different cell string value" )
			break;
		case CELLTYPE_BOOLEAN:
			ASSERT_EQUALS_N2( m_pWsheet->GetXlWs()->readBool( m_iRow, m_iCol ), ai_Cell.m_pWsheet->GetXlWs()->readBool( m_iRow, m_iCol ), m_iRow, m_iCol, "Different cell boolean value" )
			break;
		case CELLTYPE_ERROR:
			ASSERT_EQUALS_N2( m_pWsheet->GetXlWs()->readError( m_iRow, m_iCol ), ai_Cell.m_pWsheet->GetXlWs()->readError( m_iRow, m_iCol ), m_iRow, m_iCol, "Different cell error value" )
			break;
		case CELLTYPE_EMPTY:
		case CELLTYPE_BLANK:
			break;
	}

	switch( m_iType )
	{
		case CELLTYPE_NUMBER:
		case CELLTYPE_STRING:
		case CELLTYPE_BOOLEAN:
		case CELLTYPE_ERROR:
			if(  m_pWsheet->GetXlWs()->isFormula( m_iRow, m_iCol ) == true )
			{
				ASSERT_EQUALS_N2( m_pWsheet->GetXlWs()->readFormula( m_iRow, m_iCol ), ai_Cell.m_pWsheet->GetXlWs()->readFormula( m_iRow, m_iCol ), m_iRow, m_iCol, "Different cell formula value" )
			}
			break;
	}

	if( m_pFormat == ai_Cell.m_pFormat )
	{
		return true;
	}

	if( m_pFormat == NULL || ai_Cell.m_pFormat == NULL )
	{
		SETCODEF( M4_XL_UT_GEN_ERR_N2, ERRORLOG, "#*s1*#%s#%d#%d#", "Null format in cell", m_iRow, m_iCol );
		return false;
	}

	ASSERT_EQUALS_N2( m_aiBorder[ BDR_EDGE_TOP ],    ai_Cell.m_aiBorder[ BDR_EDGE_TOP ],    m_iRow, m_iCol, "Different cell top border" )
	ASSERT_EQUALS_N2( m_aiBorder[ BDR_EDGE_RIGHT ],  ai_Cell.m_aiBorder[ BDR_EDGE_RIGHT ],  m_iRow, m_iCol, "Different cell right border" )
	ASSERT_EQUALS_N2( m_aiBorder[ BDR_EDGE_BOTTOM ], ai_Cell.m_aiBorder[ BDR_EDGE_BOTTOM ], m_iRow, m_iCol, "Different cell bottom border" )
	ASSERT_EQUALS_N2( m_aiBorder[ BDR_EDGE_LEFT ],   ai_Cell.m_aiBorder[ BDR_EDGE_LEFT ],   m_iRow, m_iCol, "Different cell left border" )
	
	ASSERT_EQUALS_N2( m_pFormat->borderTopColor(),    ai_Cell.m_pFormat->borderTopColor(),    m_iRow, m_iCol, "Different cell left border color" )
	ASSERT_EQUALS_N2( m_pFormat->borderRightColor(),  ai_Cell.m_pFormat->borderRightColor(),  m_iRow, m_iCol, "Different cell right border color" )
	ASSERT_EQUALS_N2( m_pFormat->borderBottomColor(), ai_Cell.m_pFormat->borderBottomColor(), m_iRow, m_iCol, "Different cell top border color" )
	ASSERT_EQUALS_N2( m_pFormat->borderLeftColor(),   ai_Cell.m_pFormat->borderLeftColor(),   m_iRow, m_iCol, "Different cell bottom border color" )

	ASSERT_EQUALS_N2( m_iPattern,    ai_Cell.m_iPattern,    m_iRow, m_iCol, "Different cell fill pattern" )
	ASSERT_EQUALS_N2( m_iForeground, ai_Cell.m_iForeground, m_iRow, m_iCol, "Different cell pattern foreground color" )
	ASSERT_EQUALS_N2( m_iBackground, ai_Cell.m_iBackground, m_iRow, m_iCol, "Different cell pattern background color" )

	if( m_pFormat->numFormat() != NUMFORMAT_TEXT || ai_Cell.m_pFormat->numFormat() != NUMFORMAT_GENERAL)
	{
		// Does not check number format when DATASET_RESULT is text and RUN_TIME_RESULT is general, because there is
		// no way to write a text number in a cell without loosing the general number format through the Excel app.
		ASSERT_EQUALS_N2( m_pFormat->numFormat(), ai_Cell.m_pFormat->numFormat(), m_iRow, m_iCol, "Different cell number format" )
	}

	ASSERT_EQUALS_N2( m_pFormat->locked(), ai_Cell.m_pFormat->locked(), m_iRow, m_iCol, "Different cell locked state" )
	ASSERT_EQUALS_N2( m_pFormat->hidden(), ai_Cell.m_pFormat->hidden(), m_iRow, m_iCol, "Different cell hidden state" )

	pFont1 = m_pFormat->font();
	pFont2 = ai_Cell.m_pFormat->font();

	if( pFont1 == pFont2 )
	{
		return true;
	}

	if( pFont1 == NULL || pFont1 == NULL )
	{
		SETCODEF( M4_XL_UT_GEN_ERR_N2, ERRORLOG, "#*s1*#%s#%d#%d#", "Null font in cell", m_iRow, m_iCol );
		return false;
	}

	ASSERT_EQUALS_N2( pFont1->name(),      pFont2->name(),      m_iRow, m_iCol, "Different font name" )
	ASSERT_EQUALS_N2( pFont1->size(),      pFont2->size(),      m_iRow, m_iCol, "Different font size" )
	ASSERT_EQUALS_N2( pFont1->italic(),    pFont2->italic(),    m_iRow, m_iCol, "Different font italic attribute" )
	ASSERT_EQUALS_N2( pFont1->strikeOut(), pFont2->strikeOut(), m_iRow, m_iCol, "Different font strike out attribute" )
	ASSERT_EQUALS_N2( pFont1->color(),     pFont2->color(),		m_iRow, m_iCol, "Different font color attribute" )
	ASSERT_EQUALS_N2( pFont1->bold(),      pFont2->bold(),      m_iRow, m_iCol, "Different font bold attribute" )
	ASSERT_EQUALS_N2( pFont1->script(),    pFont2->script(),    m_iRow, m_iCol, "Different font script attribute" )
	ASSERT_EQUALS_N2( pFont1->underline(), pFont2->underline(), m_iRow, m_iCol, "Different font underline attribute" )
	

	return true;
}


// -----------------------------------------------------------------------------
// Equality operator for cells.
// -----------------------------------------------------------------------------
bool ClCell::operator!=( const ClCell& ai_Cell )
{
	 return !( *this == ai_Cell );
}


// -----------------------------------------------------------------------------
// Gets if the cell is empty (data and format).
// Required have been compared this cell before.
// -----------------------------------------------------------------------------
bool ClCell::IsEmpty()
{
	const wchar_t*	pwcString;


	if( m_pwcComment != NULL )
	{
		return false;
	}

	if( m_iType == CELLTYPE_STRING )
	{
		pwcString = m_pWsheet->GetXlWs()->readStr( m_iRow, m_iCol );
		
		if( pwcString != NULL && !equals( pwcString, L"" ) )
		{
			return false;
		}
	}
	else if( m_iType != CELLTYPE_EMPTY && m_iType != CELLTYPE_BLANK )
	{
		return false;
	}

	if( m_pFormat == NULL )
	{
		return true;
	}

	if( m_aiBorder[ BDR_EDGE_TOP ] != BORDERSTYLE_NONE || m_aiBorder[ BDR_EDGE_RIGHT ] != BORDERSTYLE_NONE || 
		m_aiBorder[ BDR_EDGE_BOTTOM ] != BORDERSTYLE_NONE || m_aiBorder[ BDR_EDGE_LEFT ] != BORDERSTYLE_NONE )
	{
		return false;
	}

	if( m_iPattern != FILLPATTERN_NONE )
	{
		return false;
	}

	if( m_iForeground != COLOR_DEFAULT_FOREGROUND )
	{
		return false;
	}

	if( m_iBackground != COLOR_DEFAULT_BACKGROUND )
	{
		return false;
	}


	return true;
}


// -----------------------------------------------------------------------------
// Class ClDeleteThem
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClDeleteThem::ClDeleteThem()
{
	m_pWbook1 = NULL;
	m_pWbook2 = NULL;
	m_pWsheet1 = NULL;
	m_pWsheet2 = NULL;
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClDeleteThem::~ClDeleteThem()
{
	if( m_pWsheet1 != NULL )
	{
		delete m_pWsheet1;
	}

	if( m_pWsheet2 != NULL )
	{
		delete m_pWsheet2;
	}

	if( m_pWbook1 != m_pWbook2 && m_pWbook2 != NULL )
	{
		delete m_pWbook2;
	}

	if( m_pWbook1 != NULL )
	{
		delete m_pWbook1;
	}
}