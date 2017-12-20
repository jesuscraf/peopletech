
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fechas.h"
#include "m4cppobl.hpp"


#define M4CL_OBL_NUMERIC_TYPE         T_OBJ_PRP_INT
#define M4CL_OBL_STRING_TYPE          T_OBJ_PRP_STR
#define M4CL_OBL_DATE_TYPE            T_OBJ_PRP_FEC



M4ClObl :: M4ClObl( void )
{
    M4AOblInicializa( "", "", "", "", &m_uiHandle ) ;
    m_iPrecision = 0 ;
    m_iAllocated = 1 ;
}



M4ClObl :: M4ClObl( UINT a_uiHandle )
{
    m_uiHandle = a_uiHandle ;
    m_iPrecision = 0 ;
    m_iAllocated = 0 ;
}




M4ClObl :: ~M4ClObl( void )
{
    if( m_iAllocated )
    {
        M4AOblFinaliza( m_uiHandle ) ;        
    }
}



int M4ClObl :: Init( UINT a_uiHandle, int a_iAllocated )
{    
    if( m_iAllocated )
    {
        M4AOblFinaliza( m_uiHandle ) ;        
    }

    m_uiHandle = a_uiHandle ;
    m_iAllocated = a_iAllocated ;
    
    return( 0 ) ;    
}



int M4ClObl :: Init( M4ClObl &a_rclObl )
{    
    if( m_iAllocated )
    {
        M4AOblFinaliza( m_uiHandle ) ;        
    }

    m_uiHandle   = a_rclObl.m_uiHandle ;
    m_iPrecision = a_rclObl.m_iPrecision ;
    m_iAllocated = a_rclObl.m_iAllocated ;
    
    a_rclObl.m_iAllocated = 0 ;
    
    return( 0 ) ;    
}


int    M4ClObl :: Init( char** a_ppszText, int a_iLength )
{

    int        iResult ;
    int        i ;
    int        iError ;
    
    
    if( m_iAllocated )
    {
        M4AOblFinaliza( m_uiHandle ) ;        
    }

    if( a_ppszText != NULL )
    {
        iError = M4AOblInicializa( "", "", "", "", &m_uiHandle ) ;

        if( iError == OBL_SUCCESS )
        {
            for( i = 0 ; i < ( a_iLength - 1 ) && iError == OBL_SUCCESS ; i++ )
            {
                if( a_ppszText[ i ] != NULL )
                {
                    iError = M4AOblAddTexto( m_uiHandle, a_ppszText[ i ] ) ;
                }
                else
                {
                    iError = !OBL_SUCCESS ;
                }
            }
            
            if( iError == OBL_SUCCESS )
            {
                if( a_ppszText[ a_iLength - 1 ] != NULL )
                {
                    iError = M4AOblGeneraObl( m_uiHandle, a_ppszText[ a_iLength - 1 ] ) ;
                    
                    if( iError == OBL_SUCCESS )
                    {
                        m_iAllocated = 1 ;
                        iResult = 0 ;
                    }
                    else
                    {
                        iResult = -1 ;
                    }
                }
                else
                {
                    iResult = -1 ;
                }                
            }            
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}


int M4ClObl :: Init( const char* ca_pszFile, unsigned int a_iReadSize, unsigned int a_iWriteSize )
{

    int					iResult ;
    int					iError ;
    char*				pszLine ;
	unsigned int		lSize ;
	unsigned int		i ;
	unsigned int		iFree ;
	unsigned int		iRest ;
	unsigned int		iLineRest ;
	unsigned int		iLineLength ;
	unsigned int		iChunkLength ;
	unsigned int		iChunk ;
	unsigned int		iChunks ;
    char				**ppszChunks ;
    FILE				*pfFile ;
    
    
    pfFile = fopen( ca_pszFile, "r" ) ;
    
    if( pfFile != NULL )
    {
        pszLine = new char[a_iReadSize] ;
        
        if( pszLine != NULL )
        {
            lSize = 0 ;       
            while( fgets( pszLine, a_iReadSize, pfFile ) != NULL )
            {    
                lSize += (unsigned int) strlen( pszLine ) + 1 ; // el 1 para el '\n'
            }

            if( feof( pfFile ) != 0 )
            {
                fclose( pfFile ) ;

                iChunks = lSize / a_iWriteSize;
                iRest = lSize - iChunks * a_iWriteSize;
                
                ppszChunks = new char*[ iChunks + 1 ] ;
                
                if( ppszChunks != NULL )
                {
                    iError = 0 ;
                    for( i = 0 ; i < iChunks && !iError ; i++ )
                    {
                        ppszChunks[ i ] = new char[ a_iWriteSize + 1 ] ;
                        
                        if( ppszChunks[ i ] != NULL )
                        {
                            *ppszChunks[ i ] = '\0' ;
                        }
                        else
                        {
                            iError = 1 ;
                        }
                    }
                    
                    ppszChunks[ iChunks ] = new char[ iRest + 1 ];
                    
                    if( ppszChunks[ iChunks ] != NULL )
                    {
                       *ppszChunks[ iChunks ] = '\0' ;
                    }
                    else
                    {
                        iError = 1 ;
                    }
                    
                    if( iError != 1 )
                    {
                        pfFile = fopen( ca_pszFile, "r" ) ;
                        
                        if( pfFile != NULL )
                        {
                            iChunk = 0 ;
                            iChunkLength = 0 ;
                            iFree = a_iWriteSize ;
                            
                            while( fgets( pszLine, a_iReadSize, pfFile ) != NULL )
                            {    
                                iLineLength  = (unsigned int) strlen( pszLine ) ;
                                
                                if( iFree > iLineLength )
                                {
                                    memcpy( ppszChunks[ iChunk ] + iChunkLength, pszLine, iLineLength ) ;
                                    iChunkLength += iLineLength ;
                                    ppszChunks[ iChunk ][ iChunkLength++ ] = '\n' ;
                                    iFree -= iLineLength + 1 ;
                                }
                                else
                                {
                                    memcpy( ppszChunks[ iChunk ] + iChunkLength, pszLine, iFree ) ;
                                    ppszChunks[ iChunk++ ][ a_iWriteSize ] = '\0' ;

                                    iLineRest = iLineLength - iFree ;

                                    memcpy( ppszChunks[ iChunk ], pszLine + iFree, iLineRest ) ;
                                    iChunkLength = iLineRest ;
                                    ppszChunks[ iChunk ][ iChunkLength++ ] = '\n' ;
                                    iFree = a_iWriteSize - iLineRest - 1 ;
                                }
                            }
                            ppszChunks[ iChunk ][ iRest ] = '\0' ;

                            iResult = Init( ppszChunks, (iChunks + 1) ) ;

                            fclose( pfFile ) ;
                            
                            for( i = 0 ; i < iChunks + 1 ; i++ )
                            {
                                delete[]( ppszChunks[ i ] ) ;
                                ppszChunks[ i ] = NULL ;
                            }
                            delete[]( ppszChunks ) ;
                            ppszChunks = NULL ;
                        }
                        else
                        {
                            iResult = -1 ;
                        }
                    }
                    else
                    {
                        iResult = -1 ;
                    }                     
                }
                else
                {
                    iResult = -1 ;
                }
            }
            else
            {
                iResult = -1 ;
            }       
        
            delete [] pszLine ;
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }

    return( iResult ) ;
}

     
int M4ClObl :: GetClass( char* ao_pszString, size_t ai_iStringSize, size_t &ao_riOutput ) const
{

    int				iResult ;
    int				iError ;
	unsigned int	iLenOut = 0 ;

	iError = M4AOblGetClass(m_uiHandle, ao_pszString, (unsigned int)ai_iStringSize, &iLenOut);
	ao_riOutput = iLenOut;

    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}



int M4ClObl :: GetInstance( char* ao_pszString, size_t ai_iStringSize, size_t &ao_riOutput ) const
{

    int				iResult ;
    int				iError ;
	unsigned int	iLenOut = 0 ;

	iError = M4AOblGetHinst(m_uiHandle, ao_pszString, (unsigned int) ai_iStringSize, &iLenOut);
	ao_riOutput = iLenOut;

    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}





int M4ClObl :: GetNumericValue( char* ai_pszPath, char* ai_pszName, double &ao_rdNumber ) 
{

    int        iResult ;
    int        iError ;
    int        iType ;
    UINT       uiOutput ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblGetHandPropPathNomb( m_uiHandle, ai_pszPath, ai_pszName, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iError = M4AOblGetTipoPropHandle( m_uiHandle, uiPropertyHandle, &iType ) ;

        if( iError == OBL_SUCCESS )
        {
            if( iType == M4CL_OBL_NUMERIC_TYPE )
            {
                iError = M4AOblGetValPropHandle( m_uiHandle, uiPropertyHandle, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
                
                if( iError == OBL_SUCCESS )
                {
                    ao_rdNumber = atof( m_acBuffer ) ;
                    iResult = 0 ;
                }
                else
                {
                    iResult = -1 ;
                }                
            }
            else
            {
                iResult = -1 ;
            }
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}




int M4ClObl :: GetStringValue( char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const
{

    int				iResult ;
    int				iError ;
    int				iType ;
    UINT			uiPropertyHandle ;
	unsigned int	iLenOut ;
    
    iError = M4AOblGetHandPropPathNomb( m_uiHandle, ai_pszPath, ai_pszName, &uiPropertyHandle ) ;
    if( iError == OBL_SUCCESS )
    {
        iError = M4AOblGetTipoPropHandle( m_uiHandle, uiPropertyHandle, &iType ) ;

        if( iError == OBL_SUCCESS )
        {
            if( iType == M4CL_OBL_STRING_TYPE )
            {
                iError = M4AOblGetValPropHandle( m_uiHandle, uiPropertyHandle, ao_pszString, (unsigned int) ai_iStringSize, &iLenOut) ;
				ao_riStringLength = iLenOut;
         
                if( iError == OBL_SUCCESS )
                {
                    iResult = 0 ;
                }
                else
                {
                    iResult = -1 ;
                }                
            }
            else
            {
                iResult = -1 ;
            }
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}




int M4ClObl :: GetDateValue( char* ai_pszPath, char* ai_pszName, double &ao_rdDate ) 
{

    int        iResult ;
    int        iError ;
    int        iType ;
    UINT       uiOutput ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblGetHandPropPathNomb( m_uiHandle, ai_pszPath, ai_pszName, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iError = M4AOblGetTipoPropHandle( m_uiHandle, uiPropertyHandle, &iType ) ;

        if( iError == OBL_SUCCESS )
        {
            if( iType == M4CL_OBL_DATE_TYPE )
            {
                iError = M4AOblGetValPropHandle( m_uiHandle, uiPropertyHandle, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
                
                if( iError == OBL_SUCCESS )
                {
                    ao_rdDate = numdias
                                    (
                                        atoi( m_acBuffer + M4CL_OBL_DAY_POS_ISO   ),
                                        atoi( m_acBuffer + M4CL_OBL_MONTH_POS_ISO ),
                                        atoi( m_acBuffer + M4CL_OBL_YEAR_POS_ISO  )
                                    ) ;
                    iResult = 0 ;
                }
                else
                {
                    iResult = -1 ;
                }                
            }
            else
            {
                iResult = -1 ;
            }
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}


int M4ClObl :: LendStringValue( char* ai_pszPath, char* ai_pszName, char* &ao_rpszString ) 
{

    int         iResult ;
    int         iError ;
    int         iType ;
    UINT        uiOutput ;
    UINT        uiPropertyHandle ;
    
    
    iError = M4AOblGetHandPropPathNomb( m_uiHandle, ai_pszPath, ai_pszName, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iError = M4AOblGetTipoPropHandle( m_uiHandle, uiPropertyHandle, &iType ) ;

        if( iError == OBL_SUCCESS )
        {
            if( iType == M4CL_OBL_STRING_TYPE )
            {
                iError = M4AOblGetValPropHandle( m_uiHandle, uiPropertyHandle, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
                
                if( iError == OBL_SUCCESS )
                {
                    ao_rpszString = m_acBuffer ;
                    iResult = 0 ;
                }
                else
                {
                    iResult = -1 ;
                }                
            }
            else
            {
                iResult = -1 ;
            }
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}





int M4ClObl :: SetNumericValue( char* a_pszPath, char* a_pszName, double a_dNumber )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetValPropPathNomb( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    sprintf( m_acBuffer, "%.*f", m_iPrecision, a_dNumber ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropPath( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;

}


int    M4ClObl :: SetStringValue( char* a_pszPath, char* a_pszName, char* a_pszString )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetValPropPathNomb( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}



int M4ClObl :: SetDateValue( char* a_pszPath, char* a_pszName, char* a_pszDate )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetValPropPathNomb( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}
 


int M4ClObl :: GetHandleToBlock( char* ai_pszPath, UINT &ao_rhHandle )
{

    int        iResult ;
    int        iError ;
    
    
    iError = M4AOblGetHandBlq( m_uiHandle, ai_pszPath, &ao_rhHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}


int M4ClObl :: GetBlockInstance ( UINT ai_hBlockHandle, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const
{

    int				iResult ;
    int				iError ;
	unsigned int	iLenOut ;
 
    iError = M4AOblGetHinstBlqHandle( m_uiHandle, ai_hBlockHandle, ao_pszString, (unsigned int) ai_iStringSize, &iLenOut) ;
	ao_riStringLength = iLenOut;

    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}


int M4ClObl :: LendBlockInstance( UINT ai_hBlockHandle, char* &ao_rpszString )
{

    int        iResult ;
    int        iError ;
    UINT       uiOutput ;
    
    
    iError = M4AOblGetHinstBlqHandle( m_uiHandle, ai_hBlockHandle, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
    
    if( iError == OBL_SUCCESS )
    {
        ao_rpszString = m_acBuffer ;
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}


int M4ClObl :: GetNumericValue( UINT ai_hBlockHandle, char* ai_pszName, double &ao_rdNumber ) 
{

    int        iResult ;
    int        iError ;
    int        iType ;
    UINT       uiOutput ;
    
    
    iError = M4AOblGetTipoPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, &iType ) ;

    if( iError == OBL_SUCCESS )
    {
        if( iType == M4CL_OBL_NUMERIC_TYPE )
        {
            iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
                
            if( iError == OBL_SUCCESS )
            {
                ao_rdNumber = atof( m_acBuffer ) ;
                iResult = 0 ;
            }
            else
            {
                iResult = -1 ;
            }                
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}




int M4ClObl :: GetStringValue( UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const
{

    int				iResult ;
    int				iError ;
    int				iType ;
	unsigned int	iLenOut ;
    
    iError = M4AOblGetTipoPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, &iType ) ;
    if( iError == OBL_SUCCESS )
    {
        if( iType == M4CL_OBL_STRING_TYPE )
        {
            iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, ao_pszString, (unsigned int) ai_iStringSize, &iLenOut ) ;
			ao_riStringLength = iLenOut ;

            if( iError == OBL_SUCCESS )
            {
                iResult = 0 ;
            }
            else
            {
                iResult = -1 ;
            }                
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }

    return( iResult ) ;
    
}




int M4ClObl :: GetDateValue( UINT ai_hBlockHandle, char* ai_pszName, double &ao_rdDate ) 
{

    int        iResult ;
    int        iError ;
    int        iType ;
    UINT       uiOutput ;
    
    
    iError = M4AOblGetTipoPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, &iType ) ;
    
    if( iError == OBL_SUCCESS )
    {
        if( iType == M4CL_OBL_DATE_TYPE )
        {
            iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiOutput ) ;
                
            if( iError == OBL_SUCCESS )
            {
                ao_rdDate = numdias
                                (
                                    atoi( m_acBuffer + M4CL_OBL_DAY_POS_ISO   ),
                                    atoi( m_acBuffer + M4CL_OBL_MONTH_POS_ISO ),
                                    atoi( m_acBuffer + M4CL_OBL_YEAR_POS_ISO  )
                                ) ;
                iResult = 0 ;
            }
            else
            {
                iResult = -1 ;
            }                
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
    
}


int M4ClObl :: LendStringValue( UINT ai_hBlockHandle, char *ai_pszName, char* &ao_rpszString ) 
{

    int         iResult ;
    int         iError ;
    int         iType ;
    UINT        iOutput ;
    
    
    iError = M4AOblGetTipoPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, &iType ) ;
    
    if( iError == OBL_SUCCESS )
    {
        if( iType == M4CL_OBL_STRING_TYPE )
        {
            iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, ai_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &iOutput ) ;
                
            if( iError == OBL_SUCCESS )
            {
                ao_rpszString = m_acBuffer ;
                iResult = 0 ;
            }
            else
            {
                iResult = -1 ;
            }                
        }
        else
        {
            iResult = -1 ;
        }
    }
    else
    {
        iResult = -1 ;
    }

    return( iResult ) ;
    
}





int M4ClObl :: SetNumericValue( UINT ai_hBlockHandle, char* a_pszName, double a_dNumber )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    sprintf( m_acBuffer, "%.*f", m_iPrecision, a_dNumber ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropHand( m_uiHandle, ai_hBlockHandle, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropNomb( m_uiHandle, ai_hBlockHandle, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;

}


int    M4ClObl :: SetStringValue( UINT ai_hBlockHandle, char* a_pszName, char* a_pszString )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropHand( m_uiHandle, ai_hBlockHandle, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropNomb( m_uiHandle, ai_hBlockHandle, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}



int M4ClObl :: SetDateValue( UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate )
{

    int         iResult ;
    int         iError ;
    UINT        uiPropertyHandle ;
    

    iError = M4AOblGetPropHandNomb( m_uiHandle, ai_hBlockHandle, a_pszName, m_acBuffer, M4CL_MAX_OBL_PRO_SZ, &uiPropertyHandle ) ;
    
    if( iError == IDS_OBJ_NOT_FOUND )
    {
        iError = M4AOblAddPropHand( m_uiHandle, ai_hBlockHandle, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE, &uiPropertyHandle ) ;
    }
    else
    {
        iError = M4AOblSetPropNomb( m_uiHandle, ai_hBlockHandle, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE ) ;
    }
        
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}
 

int M4ClObl :: AddBlock( char* a_pszPath, char *pszClass, char* pszInstance, UINT &ao_rhHandle )
{

    int        iResult ;
    int        iError ;
    
    
    iError = M4AOblAddBlqPath( m_uiHandle, a_pszPath, pszClass, pszInstance, &ao_rhHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;

}



int M4ClObl :: AddNumericProperty( UINT a_hBlockHandle, char* a_pszName, double a_dNumber )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    sprintf( m_acBuffer, "%.*f", m_iPrecision, a_dNumber ) ;
    
    iError = M4AOblAddPropHand( m_uiHandle, a_hBlockHandle, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;

}


int    M4ClObl :: AddStringProperty( UINT a_hBlockHandle, char* a_pszName, char* a_pszString )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblAddPropHand( m_uiHandle, a_hBlockHandle, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}



int M4ClObl :: AddDateProperty( UINT a_hBlockHandle, char* a_pszName, char* a_pszDate )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblAddPropHand( m_uiHandle, a_hBlockHandle, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}
 



int M4ClObl :: AddNumericProperty( char* a_pszPath, char* a_pszName, double a_dNumber )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    sprintf( m_acBuffer, "%.*f", m_iPrecision, a_dNumber ) ;
    
    iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, m_acBuffer, M4CL_OBL_NUMERIC_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;

}


int    M4ClObl :: AddStringProperty( char* a_pszPath, char* a_pszName, char* a_pszString )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszString, M4CL_OBL_STRING_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}



int M4ClObl :: AddDateProperty( char* a_pszPath, char* a_pszName, char* a_pszDate )
{

    int        iResult ;
    int        iError ;
    UINT       uiPropertyHandle ;
    
    
    iError = M4AOblAddPropPath( m_uiHandle, a_pszPath, a_pszName, a_pszDate, M4CL_OBL_DATE_TYPE, &uiPropertyHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }
    
    return( iResult ) ;
}
 


int M4ClObl :: GetAllMatchingBlocks( char* ai_pszPath, UINT *ao_auiBlockHandles, size_t ai_iHandleSize, size_t &ao_riHandleLength )
{

    int        iResult ;
    int        iError ;
    UINT       uiBlockHandle ;
    UINT       uiMatchHandle ;
    

    ao_riHandleLength = 0 ;
    
    iError = M4AOblFirstHndBlq( m_uiHandle, ai_pszPath, &uiBlockHandle, &uiMatchHandle ) ;
    
    if( iError == OBL_SUCCESS )
    {
        if( ao_riHandleLength < ai_iHandleSize )
        {
            ao_auiBlockHandles[ ao_riHandleLength ] = uiBlockHandle ;
            ao_riHandleLength++ ;
            
            iResult = 0 ;
            do
            {
                iError = M4AOblNextHndBlq( m_uiHandle, uiMatchHandle, &uiBlockHandle ) ;

                if( iError == OBL_SUCCESS )
                {
                    if( ao_riHandleLength < ai_iHandleSize )
                    {
                        ao_auiBlockHandles[ ao_riHandleLength ] = uiBlockHandle ;
                        ao_riHandleLength++ ;
                    }
                    else
                    {
                        iResult = -1 ;
                    }
                }
                else if( iError == IDS_OBJ_NOT_FOUND )  
                {
                    iResult = 0 ;    
                }                 
            }
            while( iError == OBL_SUCCESS && iResult == 0 ) ;     
        }
        else
        {
            iResult = -1 ;
        }
    }
    else if( iError == IDS_OBJ_NOT_FOUND )
    {
        iResult = 0 ;
    }
    else
    {
        iResult = -1 ;
    }    
    
    return( iResult ) ;    
}

int M4ClObl :: WriteOut(char* ao_szTextOut, UINT ai_iLenBuf, UINT & ao_iLenOut)
{
		int iRet;
		int iResult = 0;

	iRet = M4AOblWriteFicheroFirst(m_uiHandle, ao_szTextOut, ai_iLenBuf, &ao_iLenOut);

	if ( iRet !=  OBL_SUCCESS ) {

		iResult = -1;
	}

	return 0;


};



void M4ClObl :: operator = ( M4ClObl &a_rclObl )
{
    m_uiHandle   = a_rclObl.m_uiHandle ;
    m_iPrecision = a_rclObl.m_iPrecision ;
    m_iAllocated = a_rclObl.m_iAllocated ;
    
    a_rclObl.m_iAllocated = 0 ;
} 

// Esta funcion esta vacia para que compile. Se trata de buscar en el futuro 
// una version de M4CPPOBL.CPP que implemente la definición de esta
// funcion.
int	M4ClObl ::	GetPathBlq ( unsigned int  ai_hBlq ,  char * ao_szPathBlq  )
{
	int				iRet;

	iRet = M4AOblGetPath( m_uiHandle , ai_hBlq , ao_szPathBlq );

	if ( iRet != OBL_SUCCESS ) {

		return -1;
	}

	return 0;
}
