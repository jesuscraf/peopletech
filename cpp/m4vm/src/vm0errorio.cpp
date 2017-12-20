
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    implementacion de la clase que implementa las funciones de error e IO
////
//==============================================================================

#include "vm0errorio.hpp"
#include "m4stl.hpp"

#include <errno.h>

#include "vmcscontext.hpp"

#include "clstringpool.hpp"
#include "medvarin.hpp"
#include "clfilemap.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "fechas.h"
#include "cldates.hpp"
#include "vmres.hpp"
#include "m4logsys.hpp"
#include "m4filemanager.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"




//---------------------------------------------------------
//-I N I T   F I L E 
//---------------------------------------------------------
m4return_t  ClVM0ErrorIO::InitFile                   (void){
    
    m4VariantType vTexto, vFichero,vNumArg, vEOL, vEncoding, vBinary;
    m4return_t  ret=M4_SUCCESS;
    m4char_t pszTemp [M4CL_MAX_STRING];
	m4int32_t i;
	m4bool_t bAddEOL;
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	eUniFileOpenMode_t eMode = M4UniWrite ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: InitFile");
#endif
	//sacamos el número de argumentos
	ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ParametersNumber" << "InitFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack(&m_oState) );
	
	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);

	if (iNumArg  < 2) { //error->Faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "InitFile"<<"2-3" << DumpCallStack(&m_oState) );
		return M4_ERROR;
	}

	if (iNumArg >5) {
		for (i=0; i< iNumArg -5 ;++i){ //sacamos los que sobran
			RuntimePopAndDelete();
		}
		iNumArg = 5;
	}

	// BinaryMode
	if (iNumArg==5)
	{
		ret=RuntimePopNumber(vBinary);

		if (ret!=M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Binary"<<"InitFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vBinary.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		if( vBinary.Data.DoubleData == 1.0 )
		{
			eMode = M4UniWriteBinary ;
		}
		else
		{
			eMode = M4UniWrite ;
		}
	}
	else
	{
		//tomamos por defecto como anteriormente
		eMode = M4UniWrite ;
	}

	// UNICODE FILE
	if (iNumArg>=4)
	{
		ret=RuntimePopNumber(vEncoding);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Encoding"<<"InitFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vEncoding.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		if( vEncoding.Data.DoubleData == M4UniNative )
		{
			eEncoding = M4UniNative ;
		}
		else if( vEncoding.Data.DoubleData == M4UniUtf8 )
		{
			eEncoding = M4UniUtf8 ;
		}
		else if( vEncoding.Data.DoubleData == M4UniUtf16 )
		{
			eEncoding = M4UniUtf16 ;
		}
		else
		{
			eEncoding = M4UniANSI ;
		}
	}
	else{ //tomamos por defecto ansi
		eEncoding = M4UniANSI ;
	}

	if (iNumArg>=3){
		ret=RuntimePopNumber(vEOL);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "EOL"<<"InitFile" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vEOL.Type)<< DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		bAddEOL= (vEOL.Data.DoubleData==0.0 ? M4_FALSE : M4_TRUE);
	}
	else{ //tomamos por defecto fin_linea
		bAddEOL=M4_TRUE;
	}

    ret=RuntimePop(vTexto);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "InitFile.")	;
	
    ret=RuntimePop(vFichero);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "InitFile.")	;
	
    //comprobamos tipo (solo del nombre)
	if ( !CheckStringIfErrorPush (vFichero , 0.0 , M4_TRUE) ){
		//liberamos las posibles cadenas
		DeleteVarString (vTexto);
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FileName"<<"InitFile" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFichero.Type)<< DumpCallStack(&m_oState) );
        return M4_ERROR;
    }
	
	/* Bug 0096619
	Se cambia la forma de operar sobre el fichero para volvar los errores
	*/
	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccEOL = "" ;
	m4pcchar_t	pccFile = m_oDynamicPool->GetString( vFichero.Data.CharData );
	FILE		*pfFile = NULL ;


	if( bAddEOL )
	{
		pccEOL = "\n" ;
	}

	// UNICODE FILE
	pfFile = M4Fopen( pccFile, eMode, eEncoding ) ;

	if( pfFile != NULL )
	{
        ret = M4_SUCCESS ;

        switch (vTexto.Type)
		{
			case M4CL_CPP_TYPE_STRING_POOL:
            {
				// UNICODE FILE
				int			iLength = -1 ;
				m4pcchar_t	pccText = NULL ;
				m4pchar_t	pcTranslatedText = NULL ;

				pccText = m_oDynamicPool->GetString( vTexto.Data.CharData ) ;

				switch( eEncoding )
				{
					case M4UniUtf8:
						pcTranslatedText = M4CppToUtf8( pccText, iLength ) ;
						iWrite = fwrite( pcTranslatedText, 1, iLength, pfFile ) ;

						if( iWrite != iLength )
						{
							iWrite = -1 ;
						}
						delete [] pcTranslatedText ;
						break ;

					case M4UniANSI:
					default:
						pcTranslatedText = M4CppToANSI( pccText, iLength ) ;
						iWrite = fwrite( pcTranslatedText, 1, iLength, pfFile ) ;

						if( iWrite != iLength )
						{
							iWrite = -1 ;
						}
						delete [] pcTranslatedText ;
						break ;
				}

				fprintf( pfFile, "%s", pccEOL ) ;

                m_oDynamicPool->DeleteString(vTexto.Data.CharData); //borramos la cadena

                break;
            }

			case M4CL_CPP_TYPE_NUMBER:
            {
				iWrite = fprintf( pfFile, "%.6f%s", vTexto.Data.DoubleData, pccEOL ) ;
                break;
            }

			case M4CL_CPP_TYPE_DATE:	
            {
                ClDateToString(vTexto.Data.DoubleData, pszTemp ,M4CL_MAX_STRING);
				iWrite = fprintf( pfFile, "%s%s", pszTemp, pccEOL ) ;
                break;
            }

			case M4CL_CPP_TYPE_NULL:
            {
				iWrite = fprintf( pfFile, "%s%s", " <NULL> ", pccEOL ) ;
                break;
            }            
        }

		if( iWrite < 0 )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_WRITE_IN_FILE, "InitFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
			ret = M4_ERROR ;
		}

		iFlush = fflush( pfFile ) ;

		if( iFlush == EOF )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_WRITE_IN_FILE, "InitFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
			ret = M4_ERROR ;
		}

		fclose( pfFile ) ;
	}
	else
	{
		//Fichero no se puede crear
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FILE, "InitFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
		//liberamos las posibles cadenas
		DeleteVarString (vTexto);
		ret = M4_ERROR ;
	}
    
	//resultado
	DeleteVarString (vFichero);
	RuntimePushDouble (ret);
	return M4_SUCCESS;
    
}

//---------------------------------------------------------
//-C O N C A T   F I L E
//---------------------------------------------------------
m4return_t  ClVM0ErrorIO::ConcatFile             (void){
    
    m4VariantType vTexto, vFichero,vNumArg, vEOL, vEncoding, vBinary;
    m4return_t  ret=M4_SUCCESS;
    m4char_t pszTemp [M4CL_MAX_STRING];
	m4int32_t i;
	m4bool_t bAddEOL;
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	eUniFileOpenMode_t eMode = M4UniAppend ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ConcatFile");
#endif
	//sacamos el número de argumentos
	ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ParametersNumber" << "ConcatFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack(&m_oState) );
	
	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);

	if (iNumArg  < 2) { //error->Faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "ConcatFile"<<"2-3" << DumpCallStack(&m_oState) );
		return M4_ERROR;
	}

	if (iNumArg >5) {
		for (i=0; i< iNumArg -5 ;++i){ //sacamos los que sobran
			RuntimePopAndDelete();
		}
		iNumArg = 5;
	}

	// BinaryMode
	if (iNumArg==5)
	{
		ret=RuntimePopNumber(vBinary);

		if (ret!=M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Binary"<<"ConcatFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vBinary.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		if( vBinary.Data.DoubleData == 1.0 )
		{
			eMode = M4UniAppendBinary ;
		}
		else
		{
			eMode = M4UniAppend ;
		}
	}
	else
	{
		//tomamos por defecto como anteriormente
		eMode = M4UniAppend ;
	}

	// UNICODE FILE
	if (iNumArg>=4)
	{
		ret=RuntimePopNumber(vEncoding);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Encoding"<<"ConcatFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vEncoding.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		if( vEncoding.Data.DoubleData == M4UniNative )
		{
			eEncoding = M4UniNative ;
		}
		else if( vEncoding.Data.DoubleData == M4UniUtf8 )
		{
			eEncoding = M4UniUtf8 ;
		}
		else if( vEncoding.Data.DoubleData == M4UniUtf16 )
		{
			eEncoding = M4UniUtf16 ;
		}
		else
		{
			eEncoding = M4UniANSI ;
		}
	}
	else{ //tomamos por defecto ansi
		eEncoding = M4UniANSI ;
	}

	if (iNumArg>=3){
		ret=RuntimePopNumber(vEOL);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "EOL"<<"ConcatFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vEOL.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		bAddEOL= (vEOL.Data.DoubleData==0.0 ? M4_FALSE : M4_TRUE);
	}
	else{ //tomamos por defecto fin_linea
		bAddEOL=M4_TRUE;
	}


    ret=RuntimePop(vTexto);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConcatFile.")	;
	
    ret=RuntimePop(vFichero);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConcatFile.")	;
	
    //comprobamos tipo (solo del nombre)
    
    if ( !CheckStringIfErrorPush (vFichero,0.0,M4_TRUE) ){
		DeleteVarString (vTexto);
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FileName"<<"ConcatFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFichero.Type)<<DumpCallStack(&m_oState) );
        return M4_ERROR;
    }


	/* Bug 0096619
	Se cambia la forma de operar sobre el fichero para volvar los errores
	*/
	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccEOL = "" ;
	m4pcchar_t	pccFile = m_oDynamicPool->GetString( vFichero.Data.CharData );
	FILE		*pfFile = NULL ;


	if( bAddEOL )
	{
		pccEOL = "\n" ;
	}

	// UNICODE FILE
	pfFile = M4Fopen( pccFile, eMode, eEncoding ) ;

	if( pfFile != NULL )
	{
        ret = M4_SUCCESS ;

        switch (vTexto.Type)
		{
			case M4CL_CPP_TYPE_STRING_POOL:
            {
				// UNICODE FILE
				int				iLength = -1 ;
				m4pcchar_t		pccText = NULL ;
				m4pchar_t		pcTranslatedText = NULL ;

				pccText = m_oDynamicPool->GetString( vTexto.Data.CharData ) ;

				switch( eEncoding )
				{
					case M4UniUtf8:
						pcTranslatedText = M4CppToUtf8( pccText, iLength ) ;
						iWrite = fwrite( pcTranslatedText, 1, iLength, pfFile ) ;

						if( iWrite != iLength )
						{
							iWrite = -1 ;
						}
						delete [] pcTranslatedText ;
						break ;

					case M4UniANSI:
					default:
						pcTranslatedText = M4CppToANSI( pccText, iLength ) ;
						iWrite = fwrite( pcTranslatedText, 1, iLength, pfFile ) ;

						if( iWrite != iLength )
						{
							iWrite = -1 ;
						}
						delete [] pcTranslatedText ;
						break ;
				}

				fprintf( pfFile, "%s", pccEOL ) ;

                m_oDynamicPool->DeleteString(vTexto.Data.CharData); //borramos la cadena
                break;
            }

			case M4CL_CPP_TYPE_NUMBER:
            {
				iWrite = fprintf( pfFile, "%.6f%s", vTexto.Data.DoubleData, pccEOL ) ;
                break;
            }

			case M4CL_CPP_TYPE_DATE:	
            {
                ClDateToString(vTexto.Data.DoubleData, pszTemp ,M4CL_MAX_STRING);
				iWrite = fprintf( pfFile, "%s%s", pszTemp, pccEOL ) ;
                break;
            }

			case M4CL_CPP_TYPE_NULL:
            {
				iWrite = fprintf( pfFile, "%s%s", " <NULL> ", pccEOL ) ;
                break;
            }            
        }

		if( iWrite < 0 )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_WRITE_IN_FILE, "ConcatFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
			ret = M4_ERROR ;
		}

		iFlush = fflush( pfFile ) ;

		if( iFlush == EOF )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_WRITE_IN_FILE, "ConcatFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
			ret = M4_ERROR ;
		}

		fclose( pfFile ) ;
	}
	else
	{
		//Fichero no se puede crear
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FILE, "ConcatFile" << pccFile << strerror( errno ) << DumpCallStack(&m_oState) );
		//liberamos las posibles cadenas
		DeleteVarString (vTexto);
		ret = M4_ERROR ;
	}
    
	//resultado
	DeleteVarString (vFichero);
	RuntimePushDouble (ret);
    return M4_SUCCESS;
	
}


//
//OpenFile
//
m4return_t ClVM0ErrorIO::OpenFile (void)
{

	m4return_t ret =M4_ERROR;
	const 	m4int8_t iNumArgs = 2;
	m4VariantType  vArgs[iNumArgs];
	m4int32_t iNewHandle = 0;

	for ( m4int32_t i = 0;i < iNumArgs;i++)
	{
		ret = RuntimePop ( vArgs[i] );
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenFile.")	;
	}
	m4VariantType &vFileName = vArgs[1], &vHandle = vArgs[0] ;


	if (vFileName.Type != M4CL_CPP_TYPE_STRING_POOL)
	{
		if ( m_oVariantMediator->ToPoolString (vFileName) != M4_SUCCESS )
		{

			FreePoolStrings ( vArgs ,iNumArgs );
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Filename"<<"OpenFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<<DumpCallStack(&m_oState) );
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}

	}

	M4_ASSERT (m_oState.m_poAccess);
	m_oState.m_poAccess -> GetpChannel() ->InitFileTable();
	if ( m_oState.m_poAccess -> GetpChannel() -> m_poFileTable ->InsertFile ( m_oDynamicPool -> GetString ( vFileName.Data.CharData ), iNewHandle ) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_FILE, m_oDynamicPool->GetString (vFileName.Data.CharData) << DumpCallStack(&m_oState) );
		FreePoolStrings (vArgs, iNumArgs );
		RuntimePushDouble (M4_ERROR);
		RuntimePushDouble (-1.0); // Handle -1.
		return M4_SUCCESS;

	}

	FreePoolStrings (vArgs, iNumArgs );

	RuntimePushDouble (M4_SUCCESS);
	RuntimePushDouble (iNewHandle);
	return M4_SUCCESS ;

}

//
//close file
//
m4return_t ClVM0ErrorIO::CloseFile (void)
{

	m4return_t ret = M4_ERROR;
	m4VariantType	vHandle;

	ret = RuntimePop (vHandle );
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CloseFile.")	;

	if (vHandle.Type != M4CL_CPP_TYPE_NUMBER )
	{
		if (m_oVariantMediator->ToDouble (vHandle) != M4_SUCCESS )
		{
			
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Handle"<<"CloseFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vHandle.Type)<<DumpCallStack(&m_oState) );

			RuntimePushDouble (M4_ERROR);
			return ( M4_SUCCESS );
		}
	}

	M4_ASSERT (m_oState.m_poAccess);

	/* Bug 0073253
	Si no existe la tabla se da un error de que no existe el handle
	*/
	ClFileTable	*poFileTable = m_oState.m_poAccess->GetpChannel()->m_poFileTable ;

	if( poFileTable == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_SUCH_HANDLE_FILE, vHandle.Data.DoubleData << "CloseFile" << DumpCallStack( &m_oState ) ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	if ( ( ret = poFileTable->RemoveFile ( (m4int32_t ) vHandle.Data.DoubleData ) ) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_HANDLE_FILE, vHandle.Data.DoubleData<<"CloseFile" << DumpCallStack(&m_oState) );
	}

	RuntimePushDouble (ret);
	return ( M4_SUCCESS );
//  Devuelvo éxito aunque no haya podido cerrarlo. 
//	Al finalizar ya cerrraré los que se han quedado abiertos.

}

//
//ReadFile
//
m4return_t ClVM0ErrorIO::ReadFile (void)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t  pRet = NULL;
	int iCharsRead = 0 ;
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	
	
	const 	m4int8_t iNumArgs = 3;
	m4VariantType  vArgs[iNumArgs];
	
	
	m4char_t	cType = 0;
	m4int32_t	iNumChars = 0,iNumCharsRead = 0, iNewHandle = 0;
	m4pchar_t	pCharSep;
	
	for ( m4int32_t i = 0;i < iNumArgs;i++)
	{
		RuntimePop ( vArgs[i] );
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ReadFile.")	;
	}
	
	m4VariantType &vHandle = vArgs[2], &vType = vArgs[1],&vRet = vArgs[0] ; // el tercero es el que se devuelve


	ifstream *ifFile;
	M4_ASSERT (m_oState.m_poAccess);

	if (!m_oState.m_poAccess -> GetpChannel() -> m_poFileTable ){ // si no hay tabla de ficheros->No lo han abierto
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_HANDLE_FILE, vHandle.Data.DoubleData<<"ReadFile" << DumpCallStack(&m_oState) );
		
		FreePoolStrings ( vArgs ,iNumArgs );
		RuntimePushDouble (-1.0); // Numero de caracteres leídos
		RuntimePushString ( "" ); // Cadena leida.
		return M4_SUCCESS;
		
	}

	if ( M4_BOOL ( CheckNumberIfErrorPush ( vHandle, M4_ERROR, M4_TRUE ) ) )
	{
		if ( m_oState.m_poAccess -> GetpChannel() -> m_poFileTable ->GetFile ( (m4int32_t )vHandle.Data.DoubleData, ifFile, eEncoding ) != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_HANDLE_FILE, vHandle.Data.DoubleData<<"ReadFile" << DumpCallStack(&m_oState) );

			FreePoolStrings ( vArgs ,iNumArgs );
			RuntimePushDouble (-1.0); // Numero de caracteres leídos
			RuntimePushString ( "" ); // Cadena leida.
			return M4_SUCCESS;
		}
	}
	else 
	{

		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Handle"<<"ReadFile" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vHandle.Type)<<DumpCallStack(&m_oState) );

		FreePoolStrings ( vArgs ,iNumArgs );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	// Ya tengo el ifstream posicionado para empezar a leer.
	switch (vType.Type)
	{
	case M4CL_CPP_TYPE_NUMBER:
		
		iNumChars = (m4int32_t )vType.Data.DoubleData;

		// jcr. Se comprueba que no se pida más de 1 MB de memoria
		if (iNumChars<0||iNumChars>=1048576){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "NumberofChars"<<"ReadFile"<<iNumChars <<DumpCallStack(&m_oState) );
			FreePoolStrings ( vArgs ,iNumArgs );
			RuntimePushDouble (-1.0); // Numero de caracteres leídos
			RuntimePushString ( "" ); // Cadena leida.
			return M4_SUCCESS;
		}

		// UNICODE LN4
		pRet = M4ReadFile( *ifFile, iNumChars, eEncoding, iCharsRead ) ;

		break;
			
	case M4CL_CPP_TYPE_STRING_VAR:

		if ( m_oVariantMediator->ToPoolString (vType) != M4_SUCCESS )
		{
			FreePoolStrings ( vArgs ,iNumArgs );
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Filename"<<"ReadFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );

			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
		// no hay break a proposito.
			
	case M4CL_CPP_TYPE_STRING_POOL:
		{
			pCharSep = m_oDynamicPool-> GetString (vType.Data.CharData );
			
			// UNICODE LN4
			pRet = M4ReadFile( *ifFile, pCharSep, eEncoding, iCharsRead ) ;
			break;
		}
			
	default:
	
		FreePoolStrings ( vArgs ,iNumArgs );
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "SearchType"<<"ReadFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );

		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
		break;
		
	}
	////////////Temporal, cuando tenga el AddEMptyString  lo cambio.

	
	FreePoolStrings ( vArgs ,iNumArgs ); // Hay que liberar antes, por si acaso viniera con algo la 
	// variable por referencia
	
	vRet.Data.PointerChar = pRet;
	vRet.Type = M4CL_CPP_TYPE_STRING_VAR;
	ret = m_oVariantMediator->ToPoolString (vRet);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ADD_TO_POOL, ERRORLOG, DumpCallStack(&m_oState))	;
	vRet.IsUser = M4_TRUE;
		
	RuntimePushDouble ( iCharsRead ); // Numero de caracteres leídos
	RuntimePush ( vRet );
		
	delete []pRet;

	return M4_SUCCESS;
	
}



//-----------------------------------
//Función de DestroyFile
//-----------------------------------
// Funcion LN4: DestroyFile(file_name)	- ""
//	- Entrada: nombre del fichero
//	- Salida:  Código de error
m4return_t ClVM0ErrorIO::DestroyFile (void)
{

	int				iLength = -1 ;
	int				error = -1 ;
	m4return_t		ret = M4_ERROR ;
	m4pcchar_t		pccFile = NULL ;
	m4pchar_t		pcANSIFile = NULL ;
	m4VariantType	vFileName ;

	//nombre del fichero
	ret = RuntimePopString (vFileName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FileName" << "DestroyFile" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<< DumpCallStack() );

	pccFile=m_oDynamicPool->GetString(vFileName.Data.CharData);

	/* Bug 0190403
	UNICODE FILE
	*/
	pcANSIFile = M4CppToANSI( pccFile, iLength ) ;
	error = unlink( pcANSIFile ) ;
	delete( pcANSIFile ) ;

	if ( error !=0 ) {
		// If we can't remove it
		switch (errno){
		
		case EACCES:
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_NO_DESTROY_FILE, pccFile << DumpCallStack() );
				ret=M4_ERROR;
				break;
			}
		case ENOENT:
			{
				DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_SUCH_FILE, pccFile << DumpCallStack() );
				ret=M4_ERROR;
				break;
			}
		default:
			{
				ret=M4_SUCCESS;
				break;
			}
		}
	}
	else {
		ret=M4_SUCCESS;
	}

		//liberamos la variable
	m_oDynamicPool->DeleteString(vFileName.Data.CharData);

	//resultado a la pila
	RuntimePushDouble(ret);
	// Devolvemos el resultado
	return M4_SUCCESS;
}


//-------------------------------------
//Errores de usuario
//-------------------------------------

//Retorna el texto del error 
m4return_t  ClVM0ErrorIO::GetErrorCode  (void){
	
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "GetErrorCode"<<"Getlog" << DumpCallStack(&m_oState) );
	return M4_ERROR;

}


m4return_t  ClVM0ErrorIO::PurgeErrorCode(void){
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "PurgeErrorCode"<<"Purgelog" << DumpCallStack(&m_oState) );
	return M4_ERROR;
}


m4return_t ClVM0ErrorIO::DumpUserInfo (void){
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "DumpUserInfo"<<"SetLog" << DumpCallStack(&m_oState) );
	return M4_ERROR;
}

m4return_t ClVM0ErrorIO::DumpUserError (void){

	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "DumpUserError"<<"SetLog" << DumpCallStack(&m_oState) );
	return M4_ERROR;
}



//---------------------------------
//Funciones de ERRORES
//---------------------------------

//
//GetLog
//

m4return_t	ClVM0ErrorIO::GetLog		(void){

	//parametros: 
	//	1.- error que quieren ->	>0 desde el principio ese
	//								<0 Desde el final ese
	//								=0 none
	//	2,3,4,5 por referencia modulo, sub, codigo, texto
	//
    m4return_t ret;

	m4VariantType vError;
	m4char_t acText[M4CL_MAX_STRING_ERROR];
	m4char_t acType[M4CL_MAX_STRING];
	//m4char_t acCode[M4CL_MAX_STRING];
	
	m4int32_t iSpaceLeft=M4CL_MAX_STRING_ERROR;
	m4int32_t iSpaceCopied=0;
	m4int32_t iTotalSpaceCopied=0;
	m4int32_t iError;	
	m4int32_t iCodigo;

	//sacamos los argumentos por referencia
	ret=RuntimePopAndDelete (); //texto
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetLog.")	;
	ret=RuntimePopAndDelete (); //codigo
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetLog.")	;
	ret=RuntimePopAndDelete (); //submodulo
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetLog.")	;
	ret=RuntimePopAndDelete (); //modulo
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetLog.")	;
	ret=RuntimePopAndDelete (); //type
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetLog.")	;


    //sacamos la posición
	ret=RuntimePopNumber (vError);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ErrorNumber"<<"GetErrorCode" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vError.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR;
	}
	iError= m4int32_t(vError.Data.DoubleData);
	
	//si la posición que pasan es 0 ->todo vacio
	if (iError != 0){
		
		if (iError >0){
			
			//tipo de error
			ret=GETH_N_ERRORTYPE ( iError-1, acType,iSpaceLeft, iSpaceCopied);
			if (ret!= M4_SUCCESS){

				/* Bug 0116432
				Si no se encuentra el mensaje es mejor devolver error para que lo puedan chequear
				*/
				RuntimePushDouble (M4_ERROR); // resultado
		
				RuntimePushString ("") ; // texto
				RuntimePushDouble (0.0); // Codigo
				RuntimePushDouble (0.0); // Submódulo
				RuntimePushDouble (0.0); // Modulo
				RuntimePushDouble (0.0); // Tipo
				DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_SUCH_ERROR_IN_STACK, DumpCallStack(&m_oState) );
				return M4_SUCCESS;
			}
			
			
			//codigo
			/*ret=GETH_N_ERRORCODE (iError-1, acCode ,iSpaceLeft, iSpaceCopied);
			iCodigo=atoi(acCode);*/
			ret=GETH_DECIMAL_N_ERRORCODE (iError-1,iCodigo);

			//text
			ret=GETH_N_ERRORTEXT (iError-1, acText, iSpaceLeft, iSpaceCopied);
			
		}
		else { 
			//tipo de error
			ret=GETH_N_LASTERRORTYPE ( -1-iError, acType,iSpaceLeft, iSpaceCopied);
			if (ret!= M4_SUCCESS){
	
				/* Bug 0116432
				Si no se encuentra el mensaje es mejor devolver error para que lo puedan chequear
				*/
				RuntimePushDouble (M4_ERROR); // resultado
			
				RuntimePushString ("") ; // texto
				RuntimePushDouble (0.0); // Codigo
				RuntimePushDouble (0.0); // Submódulo
				RuntimePushDouble (0.0); // Modulo
				RuntimePushDouble (0.0); // Tipo
				DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_SUCH_ERROR_IN_STACK, DumpCallStack(&m_oState) );
				return M4_SUCCESS;
			}
			
			
			//codigo
			/*ret=GETH_N_LASTERRORCODE (-1-iError, acCode ,iSpaceLeft, iSpaceCopied);
			iCodigo=atoi(acCode);*/
			ret=GETH_DECIMAL_N_LASTERRORCODE (-1-iError, iCodigo);

			//text
			ret=GETH_N_LASTERRORTEXT (-1-iError, acText, iSpaceLeft, iSpaceCopied);
			//argumentos por referencia + resultado
			
		}
		
		//argumentos por referencia + resultado
		
		RuntimePushDouble (M4_SUCCESS); // resultado
		
		RuntimePushString (acText) ; // texto
		RuntimePushDouble (  iCodigo&0x0000FFFF); // Codigo
		RuntimePushDouble ( (iCodigo&0x00FF0000) >> 16); // Submódulo
		RuntimePushDouble ( (iCodigo&0xFF000000) >> 24); // Modulo
		if (strcmp (acType, "_error_")==0 ){
			RuntimePushDouble (ERRORLOG);
		}
		else if (strcmp (acType, "_warning_")==0 ){
			RuntimePushDouble (WARNINGLOG);
		}
		else{
			RuntimePushDouble (DEBUGINFOLOG);
		}
	}
	else{ //cuando la posición es 0
		//argumetnos por referencia + resultado
		
		RuntimePushDouble (M4_SUCCESS); // resultado
		
		RuntimePushString ("") ; // texto
		RuntimePushDouble (0.0); // Codigo
		RuntimePushDouble (0.0); // Submódulo
		RuntimePushDouble (0.0); // Modulo
		RuntimePushDouble (0.0); // Tipo
	}
	
	return M4_SUCCESS;


}

//
//SetLog:	SetLog(type, module, submodule, code [, vars_for_additional_text, ...] )
//
m4return_t	ClVM0ErrorIO::SetLog		(void){
 
	m4VariantType vTemp, vType, vModule, vSubModule, vCode;
    m4int8_t iNumArg;
	m4return_t ret;

	m4int32_t		i;
	m4uint32_t		uiBufferNeeded=0;					//Tamano del buffer necesario para el error formateado
	m4char_t*		pszBuffer = NULL ;					//Zona de memoria con el error formateado.
	m4VariantType*	pVectVariants = NULL;				//Array de variants
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: SetLog");
#endif
    //leemos el número de argumentos
    RuntimePop(vTemp);
    iNumArg=int(vTemp.Data.DoubleData);

	if (iNumArg<4){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "SetLog"<<"4-" << DumpCallStack(&m_oState) );
        return M4_ERROR;
    }
	
	if (iNumArg>4){
		pVectVariants = new m4VariantType[iNumArg-4]; //4 arg. nos indican el tipo + codigo

#ifdef _DEBUG
		if (!pVectVariants) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for working in SetLog."<<__FILE__<<__LINE__ );
			return M4_ERROR;
		}
#endif
		//1ª Pasada: leemos los argumentos variables en el Array, 
		//			 y calculamos la longitud a reservar para el String de salida
		for (i=0; i < iNumArg-4;++i) {
			RuntimePop( pVectVariants[iNumArg-5-i] );	//leemos el argumento
			//Si es cadena del Pool la convertimos a M4CL_CPP_TYPE_STRING_VAR (porque la M4LogSys no conoce el Pool)
			if ( pVectVariants[iNumArg-5-i].Type == M4CL_CPP_TYPE_STRING_POOL )
				m_oVariantMediator->ToCharString( pVectVariants[iNumArg-5-i] );
		}
	} //end-if

	//sacamos codigo
    ret=RuntimePopNumber (vCode);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Code" << "SetLog" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vCode.Type)<<DumpCallStack(&m_oState) );
	m4uint32_t iCode= (m4uint32_t) vCode;
	if (iCode>0xFFFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Code"<<"SetLog"<<iCode << DumpCallStack(&m_oState) );
		iCode=0xFFFF;
	}

	//sacamos submodulo
    ret=RuntimePopNumber (vSubModule);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "SubModule" << "SetLog" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSubModule.Type)<<DumpCallStack(&m_oState) );
	m4uint32_t iSubModule= (m4uint32_t) vSubModule;
	if (iSubModule>0xFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Submodule"<<"SetLog"<<iSubModule << DumpCallStack(&m_oState) );
		iSubModule=0xFF;
	}
	
	//sacamos modulo
    ret=RuntimePopNumber (vModule);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Module" << "SetLog" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vModule.Type)<<DumpCallStack(&m_oState) );
	m4uint32_t iModule= (m4uint32_t) vModule;
	if (iModule>0xFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Module"<<"SetLog"<<iModule << DumpCallStack(&m_oState) );
		iModule=0xFF;
	}

	//sacamos tipo
    ret=RuntimePopNumber (vType);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Type" << "SetLog" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );
	m4int16_t iType= (m4int16_t)vType.Data.DoubleData;

	if (iType != DEBUGINFOLOG && iType != ERRORLOG && iType != WARNINGLOG){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "type"<<"SetLog"<<iType << DumpCallStack(&m_oState) );
		iType=ERRORLOG;
	}
// pues ya no
//	iType=ERRORLOG; //Raul Duque y Sonia Leiva-<Siempre error a CAPON

	//creamos codigo
    m4uint32_t iCodErr = ( (iModule&0xFF) << 24) | ( (iSubModule&0xFF) <<16) | (iCode&0xFFFF);

	if (iNumArg>4) {
		//Llamamos a la M4LogSys, para calcular el buffer necesario #*S1*#
		if (SerializeParams(pVectVariants, iNumArg-4, 1, NULL, uiBufferNeeded, &uiBufferNeeded) != M4_SUCCESS) {
			//DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for text in SetLog."<<__FILE__<<__LINE__ );
			delete [] pVectVariants;
			return M4_ERROR;
		}

		//Reservamos la zona de trabajo
		pszBuffer = new m4char_t [ uiBufferNeeded + 1 ] ;

#ifdef _DEBUG
		if (!pszBuffer) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for text in SetLog."<<__FILE__<<__LINE__ );
			delete [] pVectVariants;
			return M4_ERROR;
		}
#endif
		//Llamamos a la M4LogSys, para montar la cadena #*S1*#
		if (SerializeParams(pVectVariants, iNumArg-4, 1, pszBuffer, uiBufferNeeded, &uiBufferNeeded) != M4_SUCCESS ) {
			delete [] pVectVariants;
			delete [] pszBuffer;
			return M4_ERROR;
		}
		//Borramos el vector de variants
		delete [] pVectVariants;
	} //end-if

	//mostramos el error
	if( pszBuffer != NULL ){
		SETCODE (iCodErr , iType, pszBuffer);
		//Liberar memoria
		delete [] pszBuffer ;
	}
	else{
		SETCODE (iCodErr , iType,"#*S1*#");
	}

    //meter en la pila como resultado un M4_SUCCESS
    RuntimePushDouble(M4_SUCCESS);

    return M4_SUCCESS;
}


//
//GetLogSize
//
m4return_t	ClVM0ErrorIO::GetLogSize	(void){
	RuntimePushDouble (GET_ERROR_QUEUE_SIZE());
	return M4_SUCCESS;
}

//
//PurgeLog
//
m4return_t	ClVM0ErrorIO::PurgeLog	(void){
	//parametros: 
	//	errors que quieren purgar->	>0 desde el principio 
	//								<0 Desde el final 
	//								=0 todos
	//	
	m4return_t ret;
	m4int32_t i;
	m4VariantType vTemp;

	 //numero de purges
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "PurgeLog.")	;
	
	if ( CheckNumberIfErrorPush (vTemp, "") ) {
        if (vTemp.Data.DoubleData >0){
            for (i=0; i< (m4int32_t)vTemp.Data.DoubleData; ++i){
                POP(); //saca error de la lista de errores
            }
        }
        else if (vTemp.Data.DoubleData <0){
			for (i=0; i< -(m4int32_t)vTemp.Data.DoubleData; ++i){
                POP_BACK(); //saca error de la lista de errores
            }
        }
		else { //==0
			CLEAN();
		}
    }
	RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}


//---------------------------------------------------------
//-MessageBox
//---------------------------------------------------------
m4return_t  ClVM0ErrorIO::MessageBox       (void){   
    
    m4VariantType vTitulo;
    m4int32_t i;
	m4char_t pszTemp	[M4CL_MAX_STRING];	//Zona estática temporal
    m4char_t *pszListArg ;			// Zona de memoria con la lista de arg.
    m4char_t *pszTitle ;				// Zona de memoria con el Titulo.    
	m4VariantType *pVectVariants ;	// Array de variants
	m4int8_t iNumArg;
	m4int32_t iLenArgString = 0;
    m4int32_t iLenTitulo    = 0;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: MessageBox");
#endif
    //leemos el número de argumentos
    RuntimePop(vTitulo);
    iNumArg=int(vTitulo.Data.DoubleData);
    if (iNumArg<2){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "MessageBox"<<"2-" << DumpCallStack(&m_oState) );
		for (i=0; i<iNumArg; ++i) {
			RuntimePopAndDelete();
		}
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
	pVectVariants = new m4VariantType[iNumArg-1];

#ifdef _DEBUG
	if (!pVectVariants) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for working in MessageBox."<<__FILE__<<__LINE__ );

		for (i=0; i<iNumArg; ++i) {
			RuntimePopAndDelete();
		}
        RuntimePushDouble (M4_ERROR);		
        return M4_ERROR;
	}
#endif
    //1ª Pasada: leemos los argumentos variables en el Array, 
	//			 y calculamos la longitud a reservar para el String de salida
    for (i=0; i <= iNumArg-2;++i){
        //leemos el argumento
        RuntimePop( pVectVariants[i] );		
        //según el tipo 
        switch (pVectVariants[i].Type) {
        case M4CL_CPP_TYPE_STRING_POOL:{
			iLenArgString += m_oDynamicPool->GetStringLength(pVectVariants[i].Data.CharData) ;
            break;
                                       }
        case M4CL_CPP_TYPE_NUMBER:{
            //sprintf(pszTemp,"%.8lf",pVectVariants[i].Data.DoubleData);
			iLenArgString += M4CL_MAX_LONG_STRING_NUMBER;	//MAXimo numero representable, con decimales !!!
            break;
                                  }
        case M4CL_CPP_TYPE_DATE:	{
            //ClDateToString(pVectVariants[i].Data.DoubleData,pszTemp,M4CL_MAX_STRING);
			iLenArgString += M4CL_MAX_LONG_STRING_DATE;	//MAXima fecha representable !!!
            break;
                                    }        
        case M4CL_CPP_TYPE_NULL:{
            //strcpy (pszTemp, M4CL_STRING_NULL);
			iLenArgString += M4CL_MAX_LONG_STRING_NULL;		//Longitud de <NULL>
                                }
        }
    }
	
    //leemos el Título
    RuntimePop(vTitulo);		//titulo	    
    m_oVariantMediator->ToPoolString (vTitulo); //siempre se convierte a cadena.
	   
	//Reservamos la zona de trabajo
	pszListArg = new m4char_t [ iLenArgString + 2*iNumArg + 1 ] ;	// 2 = No. de espacios anterior y posterior en la cadena " < NULL> "

#ifdef _DEBUG
	if (!pszListArg) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for text in MessageBox."<<__FILE__<<__LINE__ );
        RuntimePushDouble (M4_ERROR);
		//liberamos las posibles cadenas
		DeleteVarString (vTitulo);
		delete [] pVectVariants;
        return M4_ERROR;
	}
#endif
	*pszListArg = '\0';
	   
    //2ª Pasada: Formateamos los argumentos variables en el Array, recorriendo al reves
    for (i=iNumArg-2; i>=0 ;--i){
        switch (pVectVariants[i].Type) {
        case M4CL_CPP_TYPE_STRING_POOL:{
            strcat(pszListArg, m_oDynamicPool->GetString(pVectVariants[i].Data.CharData));
            m_oDynamicPool->DeleteString(pVectVariants[i].Data.CharData); //borramos la cadena del Pool
            break;
                                       }
        case M4CL_CPP_TYPE_NUMBER:{
            sprintf(pszTemp,"%.8lf",pVectVariants[i].Data.DoubleData);
			strcat(pszListArg, pszTemp);
            break;
                                  }
        case M4CL_CPP_TYPE_DATE:	{
            ClDateToString(pVectVariants[i].Data.DoubleData, pszTemp, M4CL_MAX_STRING);
			strcat(pszListArg, pszTemp);
            break;
                                    }        
        case M4CL_CPP_TYPE_NULL:{
            strcat (pszListArg, M4CL_STRING_NULL);
                                }
			break;
        }
		if (i!=0)
			strcat(pszListArg, "  ");
    }
    
    //Longitud del Titulo
    if (IsNull (vTitulo) ){
		iLenTitulo = M4CL_MAX_LONG_STRING_NULL;	// Longitud de M4CL_STRING_NULL
	} else {
		iLenTitulo = m_oDynamicPool->GetStringLength(vTitulo.Data.CharData) ;
	}
	
	//Reservamos la zona de trabajo para Titulo
	pszTitle = new m4char_t [ iLenTitulo + 1 ] ;

#ifdef _DEBUG
	if (!pszTitle) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for title in MessageBox."<<__FILE__<<__LINE__ );

        RuntimePushDouble (M4_ERROR);
		DeleteVarString(vTitulo);
		delete [] pVectVariants;
		delete [] pszListArg;
        return M4_ERROR;
	}
#endif
    //Copiamos el Titulo
    if (IsNull (vTitulo) ){
        strcpy (pszTitle, M4CL_STRING_NULL);
    }
    else{
        strcpy(pszTitle, m_oDynamicPool->GetString(vTitulo.Data.CharData));        
        m_oDynamicPool->DeleteString(vTitulo.Data.CharData);
    }
    
	GetVMCSContext()->Messagebox(pszListArg, pszTitle, MB_OK|MB_TASKMODAL, M4_CH_VM_MESSAGE_BOX, M4_WARNING);

    //meter en la pila como resultado un M4_SUCCESS
    RuntimePushDouble(M4_SUCCESS);
	
	//Liberar memoria
    delete [] pVectVariants ;
	delete [] pszListArg ;
	delete [] pszTitle;
	
    return M4_SUCCESS;
}




//---------------------------------------------------------
//-DialogBox 
//---------------------------------------------------------
m4return_t  ClVM0ErrorIO::DialogWBox       (void){
	
    m4VariantType vTitulo, vType;
    m4int32_t i;
	m4char_t pszTemp	[M4CL_MAX_STRING];
    m4char_t *pszListArg ;			// Puntero a zona de memoria con la lista de arg.
    m4char_t *pszTitle ;				// Puntero a zona de memoria con el Titulo.    
	m4VariantType *pVectVariants ;	// Array de variants
	m4int8_t iNumArg;
	m4int32_t iLenArgString = 0;
	m4int32_t iLenTitulo    = 0;
    m4return_t ret;

	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DialogBox");
#endif

    //leemos el número de argumentos
    RuntimePop(vTitulo);
    iNumArg=int(vTitulo.Data.DoubleData);
    if (iNumArg<3){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "DialogBox"<<"3-" << DumpCallStack(&m_oState) );
		for (i=0; i<iNumArg; ++i) {
			RuntimePopAndDelete();
		}
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
	pVectVariants = new m4VariantType[iNumArg-2];

#ifdef _DEBUG
	if (!pVectVariants) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for working in DialogBox."<<__FILE__<<__LINE__ );

		for (i=0; i<iNumArg; ++i) {
			RuntimePopAndDelete();
		}
        RuntimePushDouble (M4_ERROR);		
        return M4_ERROR;
	}
#endif
    //1ª Pasada: leemos los argumentos variables en el Array, 
	//			 y calculamos la longitud a reservar para el String de salida
    for (i=0; i <= iNumArg-3;++i){
        //leemos el argumento
        RuntimePop( pVectVariants[i] );		
        //según el tipo 
        switch (pVectVariants[i].Type) {
        case M4CL_CPP_TYPE_STRING_POOL:{
			iLenArgString += m_oDynamicPool->GetStringLength(pVectVariants[i].Data.CharData) ;
            break;
                                       }
        case M4CL_CPP_TYPE_NUMBER:{
            //sprintf(pszTemp,"%.8lf",pVectVariants[i].Data.DoubleData);
			iLenArgString += M4CL_MAX_LONG_STRING_NUMBER;	//MAXimo numero representable, con decimales !!!
            break;
                                  }
        case M4CL_CPP_TYPE_DATE:	{
            //ClDateToString(pVectVariants[i].Data.DoubleData,pszTemp,M4CL_MAX_STRING);
			iLenArgString += M4CL_MAX_LONG_STRING_DATE;	//MAXima fecha representable !!!
            break;
                                    }        
        case M4CL_CPP_TYPE_NULL:{
            //strcpy (pszTemp, M4CL_STRING_NULL);
			iLenArgString += M4CL_MAX_LONG_STRING_NULL;		//Longitud de M4CL_STRING_NULL
                                }
        }
    }
	
	
    //leemos el el Título y el Tipo de caja de dialogo
    RuntimePop(vTitulo);		//titulo
    RuntimePop(vType);			//tipo
    
    m_oVariantMediator->ToPoolString (vTitulo); //siempre se convierte a cadena.
	ret=m_oVariantMediator->ToDouble (vType);
    if (ret!=M4_SUCCESS ){
        vType.Data.DoubleData=MB_OK;
    }
	
	//Reservamos la zona de trabajo
	pszListArg = new m4char_t [ iLenArgString + 2*iNumArg + 1 ] ;	// 2 = No. de espacios anterior y posterior en la cadena " < NULL> "

#ifdef _DEBUG
	if (!pszListArg) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for text in DialogBox."<<__FILE__<<__LINE__ );

        RuntimePushDouble (M4_ERROR);
		//liberamos las posibles cadenas
		DeleteVarString (vTitulo);
		delete [] pVectVariants;
        return M4_ERROR;
	}
#endif
	*pszListArg = '\0';
	   
    //2ª Pasada: Formateamos los argumentos variables en el Array, recorriendo al reves
    for (i=iNumArg-3; i>=0 ;--i){
        switch (pVectVariants[i].Type) {
        case M4CL_CPP_TYPE_STRING_POOL:{
            strcat(pszListArg, m_oDynamicPool->GetString(pVectVariants[i].Data.CharData));
            m_oDynamicPool->DeleteString(pVectVariants[i].Data.CharData); //borramos la cadena del Pool
            break;
                                       }
        case M4CL_CPP_TYPE_NUMBER:{
            sprintf(pszTemp,"%.8lf",pVectVariants[i].Data.DoubleData);
			strcat(pszListArg, pszTemp);
            break;
                                  }
        case M4CL_CPP_TYPE_DATE:	{
            ClDateToString(pVectVariants[i].Data.DoubleData, pszTemp, M4CL_MAX_STRING);
			strcat(pszListArg, pszTemp);
            break;
                                    }        
        case M4CL_CPP_TYPE_NULL:{
            strcat (pszListArg, M4CL_STRING_NULL);
                                }
			break;
        }
		if (i!=0)
			strcat(pszListArg, "  ");
    }
    
    //Longitud del Titulo
    if (IsNull (vTitulo) ){
		iLenTitulo = M4CL_MAX_LONG_STRING_NULL;	// Longitud de M4CL_STRING_NULL
	} else {
		iLenTitulo = m_oDynamicPool->GetStringLength(vTitulo.Data.CharData) ;
	}
	
	//Reservamos la zona de trabajo para Titulo
	pszTitle = new m4char_t [ iLenTitulo + 1 ] ;

#ifdef _DEBUG
	if (!pszTitle) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_OUT_OF_MEMORY, "Not enough memory for title in DialogBox."<<__FILE__<<__LINE__ );

        RuntimePushDouble (M4_ERROR);
		DeleteVarString(vTitulo);
		delete [] pVectVariants;
		delete [] pszListArg;
        return M4_ERROR;
	}
#endif
    //Copiamos el Titulo
    if (IsNull (vTitulo) ){
        strcpy (pszTitle, M4CL_STRING_NULL);
    }
    else{
        strcpy(pszTitle, m_oDynamicPool->GetString(vTitulo.Data.CharData));        
        m_oDynamicPool->DeleteString(vTitulo.Data.CharData);
    }
	
	m4int32_t retval ;

	retval=GetVMCSContext()->Messagebox(pszListArg, pszTitle, (m4uint32_t)vType.Data.DoubleData|MB_TASKMODAL, M4_CH_VM_DIALOG_BOX, M4_ERROR);

    //meter en la pila como resultado el boton pulsado
    RuntimePushDouble(retval);
	
	//Liberar memoria
    delete [] pVectVariants ;
	delete [] pszListArg ;
	delete [] pszTitle;
	
    return M4_SUCCESS;	
}


//-------------------------------------------------
//-------------------------------------------------
//File manager
m4return_t  ClVM0ErrorIO::ClearFiles (void){

	ClFileManager::Instance()->CollectGarbage();
	RuntimePushDouble(M4_SUCCESS);
	return M4_SUCCESS;
}
