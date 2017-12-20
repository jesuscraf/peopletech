#ifndef    __CLOBL_HPP__
#define    __CLOBL_HPP__

#include <stdlib.h>

#ifndef MAIN_OBL
#	include "m4win.hpp"
#endif

#include "m4obl_dll.hpp"
#include "m4lgobj.hpp"


#define M4CL_MAX_OBL_PRO_SZ         255

#define M4CL_OBL_DAY_POS_ISO          8   // YYYY-MM-DD HH:MM:SS   <- ISO FORMAT
#define M4CL_OBL_MONTH_POS_ISO        5
#define M4CL_OBL_YEAR_POS_ISO         0

  




class M4_DECL_M4OBL M4ClObl {

protected:

    char    m_acBuffer[ M4CL_MAX_OBL_PRO_SZ + 1 ] ;
    
    UINT    m_uiHandle ;
    int     m_iPrecision ;
    int     m_iAllocated ;    

public:

            M4ClObl( void ) ;

            M4ClObl( UINT a_uiHandle ) ;
            
            ~M4ClObl( void ) ; 
            
            
    int     Init( UINT a_uiHandle, int a_iAllocated = 0 ) ;
    
    int     Init( M4ClObl &a_rclObl ) ;
    
    int     Init( char** a_ppszText, int a_iLength ) ;
    
    int     Init( const char* ca_pszFile, unsigned int a_iReadSize, unsigned int a_iWriteSize ) ;


    int     GetHandle( void ) const
    {
        return( m_uiHandle ) ;        
    }    
    
    int     RetireHandle( void ) 
    {
        m_iAllocated = 0 ;
        return( m_uiHandle ) ;        
    }    
    
    int     GetPrecision( void ) const
    {
        return( m_iPrecision ) ;
    }
     
    void    SetPrecision( int a_iPrecision )
    {
        m_iPrecision = a_iPrecision ;
    }
    
    int     GetClass   ( char* ao_pszString, size_t ai_iStringSize, size_t &ao_riOutput ) const ;
    
    int     GetInstance( char* ao_pszString, size_t ai_iStringSize, size_t &ao_riOutput ) const ;
    
    int     GetNumericValue( char* ai_pszPath, char* ai_pszName, double &ao_rdNumber  ) ;
    int     GetStringValue ( char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const ;
    int     GetDateValue   ( char* ai_pszPath, char* ai_pszName, double &ao_rdDate    ) ;

    int     LendStringValue( char* ai_pszPath, char* ai_pszName, char* &ao_rpszString ) ; 

    int     SetNumericValue( char* a_pszPath, char* a_pszName, double a_dNumber   ) ;
    int     SetStringValue ( char* a_pszPath, char* a_pszName, char*  a_pszString ) ;
    int     SetDateValue   ( char* a_pszPath, char* a_pszName, char*  a_pszDate   ) ;


    int     GetHandleToBlock( char* a_pszPath, UINT &ao_rhHandle ) ;

    int     GetBlockInstance ( UINT ai_hBlockHandle, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const ;
    int     LendBlockInstance( UINT ai_hBlockHandle, char* &ao_rpszString ) ;


    int     GetNumericValue( UINT ai_hBlockHandle, char* ai_pszName, double &ao_rdNumber  ) ;
    int     GetStringValue ( UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t &ao_riStringLength ) const ;
    int     GetDateValue   ( UINT ai_hBlockHandle, char* ai_pszName, double &ao_rdDate    ) ;

    int     LendStringValue( UINT ai_hBlockHandle, char* ai_pszName, char* &ao_rpszString ) ; 

    int     SetNumericValue( UINT ai_hBlockHandle, char* a_pszName, double a_dNumber   ) ;
    int     SetStringValue ( UINT ai_hBlockHandle, char* a_pszName, char*  a_pszString ) ;
    int     SetDateValue   ( UINT ai_hBlockHandle, char* a_pszName, char*  a_pszDate   ) ;


    int     AddNumericProperty( char* a_pszPath, char* a_pszName, double a_dNumber   ) ;
    int     AddStringProperty ( char* a_pszPath, char* a_pszName, char*  a_pszString ) ;
    int     AddDateProperty   ( char* a_pszPath, char* a_pszName, char*  a_pszDate   ) ;
    

    int     AddBlock( char* a_pszPath, char *pszClass, char* pszInstance, UINT &ao_rhHandle ) ;

    int     AddNumericProperty( UINT a_hBlockHandle, char* a_pszName, double a_dNumber   ) ;
    int     AddStringProperty ( UINT a_hBlockHandle, char* a_pszName, char*  a_pszString ) ;
    int     AddDateProperty   ( UINT a_hBlockHandle, char* a_pszName, char*  a_pszDate   ) ;


    int     GetAllMatchingBlocks( char* ai_pszPath, UINT *ao_auiBlockHandles, size_t ai_iHandleSize, size_t &ao_riHandleLength ) ;    

	int		WriteOut(char* ao_szTextOut, UINT ai_iLenBuf, UINT & ao_iLenOut);

	int		GetPathBlq ( unsigned int  ai_hBlq ,  char * ao_szPathBlq  );
    
    void    operator = ( M4ClObl &a_rclObl ) ;

   //int WriteOut(char* ao_szTextOut, UINT ai_iLenBuf, UINT & ao_iLenOut);
        
} ;



#endif


