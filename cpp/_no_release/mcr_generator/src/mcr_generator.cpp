//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           MVC-MCR
// File:             mcr_generator.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:			 13/10/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module builds .mcr files from .m2 files
//
//
//==============================================================================


#ifdef _WINDOWS
#include <direct.h>
#endif

#include <errno.h>
#include "m4types.hpp"
#include "cldefine.hpp"
#include "m4mdrt.hpp"
#include "clfileiod.hpp"
#include "m4mdfac.hpp"
#include "m4facdef.hpp"
#include "clfactm4objservice.hpp"
#include "clm4objservice.hpp"



m4return_t	M4McrWinToUx( m4pcchar_t ai_pccFileName, m4uint8_t ai_iCsType, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccDestination )
{

    m4return_t          iResult ;
    m4uint32_t          iLength ;
    m4pcchar_t          pccBase ;
    m4char_t            acFile[ M4CH_MAX_FILE + 1 ] ;
	m4char_t			acBuffer[ sizeof( m4uint32_t ) ] ;
	ClCMCRWrapper		*poCMCRWrapper ;
	ClFileIODriver		oFileDriver ;


// Leemos el .mcr de fichero

	if( ai_pccSource != NULL && *ai_pccSource != '\0' )
	{
		sprintf( acFile, "%s%s%s&%d.mcr", ai_pccSource, M4_BACK_SLASH, ai_pccFileName, ai_iCsType ) ;
	}
	else
	{
		sprintf( acFile, "%s&%d.mcr", ai_pccFileName, ai_iCsType ) ;
	}

	iResult = oFileDriver.Init( GEN_IO_MODE_READ, acFile ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	poCMCRWrapper = new ClCMCRWrapper() ;

	if( poCMCRWrapper == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poCMCRWrapper->DeSerialize( oFileDriver ) ;

	oFileDriver.End( M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		delete( poCMCRWrapper ) ;
		return( M4_ERROR ) ;
	}


// Nos guardamos la parte que no se va a swapear (el tamaño)

	iLength = sizeof( m4uint32_t ) ;
	pccBase = poCMCRWrapper->GetBuffer() ;
	memcpy( acBuffer, pccBase, iLength ) ;


// Escribimos el .mcr en fichero en el directorio unix swapeado

	if( ai_pccDestination != NULL && *ai_pccDestination != '\0' )
    {
        sprintf( acFile, "%s%s%s&%d.mcr", ai_pccDestination, M4_BACK_SLASH, ai_pccFileName, ai_iCsType ) ;
    }
    else
    {
        sprintf( acFile, "%sunix%s%s&%d.mcr", M4_BACK_SLASH, M4_BACK_SLASH, ai_pccFileName, ai_iCsType ) ;
    }

	iResult = oFileDriver.Init( GEN_IO_MODE_WRITE, acFile ) ;

	if( iResult != M4_SUCCESS )
	{
		delete( poCMCRWrapper ) ;
		return( M4_ERROR ) ;
	}

	iResult = poCMCRWrapper->Serialize( oFileDriver, M4_TRUE ) ;

	oFileDriver.End( M4_TRUE ) ;

	delete( poCMCRWrapper ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}






int main( int argc, char** argv )
{

    m4return_t		    iResult ;
    m4char_t            acSource[ M4CH_MAX_FILE + 1 ] ;
    m4char_t            acDestination[ M4CH_MAX_FILE + 1 ] ;
	int					iDirectory ;
    ClCompiledMCR		*poCMCR ;
	ClCMCRFactory		*poFactory ;
	ClM4ObjService		*poObjService;

	cout << ( 0 + ( 600 << 7 ) )<<endl;
	cout << "mcr generator ..." << endl ;

	*acSource = '\0' ;
	*acDestination = '\0' ;

// El primer argumento es el workdir
// Si no viene se toma c:\workdir

	if( argc > 1 )
	{
		strcpy( acSource, argv[ 1 ] ) ;
	}

	if( *acSource == '\0' )
	{
		sprintf( acSource, "%s%s%s", "c:", M4_BACK_SLASH, "workdir" ) ;
	}

// El segundo argumento es el unixdir
// Si no viene se toma workdir\unix

	if( argc > 2 )
	{
		strcpy( acDestination, argv[ 2 ] ) ;
	}

	if( *acDestination == '\0' )
	{
		sprintf( acDestination, "%s%s%s", acSource, M4_BACK_SLASH, "unix" ) ;
	}

	cout << "   source: " << acSource << endl ;
	cout << "   target: " << acDestination << endl ;


	poObjService = ClFactM4ObjService :: CreateObjService( NULL, NULL, NULL, NULL, NULL, M4_FALSE ) ;

	if( poObjService == NULL )
	{
		return( -1 ) ;
	}

	poFactory = poObjService->GetpCMCRFactory();

	if( poFactory == NULL )
	{
		return( -1 ) ;
	}


// se crean los .mcr

	cout << "   m2 front: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4LOAD_M2, M4CL_CSTYPE_FRONT, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 both: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4LOAD_M2, M4CL_CSTYPE_BOTH, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 back: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4LOAD_M2, M4CL_CSTYPE_BACK, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   m2 front: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4DICT_M2, M4CL_CSTYPE_FRONT,   0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 both: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4DICT_M2, M4CL_CSTYPE_BOTH,   0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 back: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4DICT_M2, M4CL_CSTYPE_BACK,   0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   m2 front: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_FRONT, 0, acSource, poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 both: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_BOTH, 0, acSource, poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 back: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_BACK, 0, acSource, poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   m2 front: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SEC_M2, M4CL_CSTYPE_FRONT, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 both: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SEC_M2, M4CL_CSTYPE_BOTH, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 back: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4SEC_M2, M4CL_CSTYPE_BACK, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   m2 front: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4TRANS_M2, M4CL_CSTYPE_FRONT, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 both: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4TRANS_M2, M4CL_CSTYPE_BOTH, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   m2 back: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = poFactory->MDFac_BuildFromFile( M4MDFAC_FILE_M4TRANS_M2, M4CL_CSTYPE_BACK, 0, acSource,  poCMCR ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

// se pasan a unix
	iDirectory = mkdir( acDestination ) ;

	if( iDirectory != 0 )
	{
		if( errno != EEXIST )
		{
			return( -1 ) ;
		}
	}

	cout << "   unix/m2 front: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4LOAD_M2,   M4CL_CSTYPE_FRONT, acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 both: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4LOAD_M2,   M4CL_CSTYPE_BOTH,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 back: " << M4MDFAC_FILE_M4LOAD_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4LOAD_M2,   M4CL_CSTYPE_BACK,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   unix/m2 front: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4DICT_M2,   M4CL_CSTYPE_FRONT, acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 both: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4DICT_M2,   M4CL_CSTYPE_BOTH,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 back: " << M4MDFAC_FILE_M4DICT_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4DICT_M2,   M4CL_CSTYPE_BACK,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   unix/m2 front: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_FRONT, acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 both: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_BOTH,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 back: " << M4MDFAC_FILE_M4SYSTEM_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SYSTEM_M2, M4CL_CSTYPE_BACK,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   unix/m2 front: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SEC_M2,    M4CL_CSTYPE_FRONT, acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 both: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SEC_M2,    M4CL_CSTYPE_BOTH,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 back: " << M4MDFAC_FILE_M4SEC_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4SEC_M2,    M4CL_CSTYPE_BACK,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }

	cout << "   unix/m2 front: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4TRANS_M2,  M4CL_CSTYPE_FRONT, acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 both: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4TRANS_M2,  M4CL_CSTYPE_BOTH,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }
	cout << "   unix/m2 back: " << M4MDFAC_FILE_M4TRANS_M2 << endl ;
	iResult = M4McrWinToUx( M4MDFAC_FILE_M4TRANS_M2,  M4CL_CSTYPE_BACK,  acSource, acDestination ) ;
	if( iResult != M4_SUCCESS ) { return( -1 ) ; }


	cout << "end mcr generator ..." << endl ;

	return( 0 ) ;
}




