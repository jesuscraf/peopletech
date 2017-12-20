//## begin module%3912A6BD033D.cm preserve=no
//## end module%3912A6BD033D.cm

//## begin module%3912A6BD033D.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3912A6BD033D.cp

//## Module: m4glbmisc%3912A6BD033D; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\m4glbmisc.cpp

//## begin module%3912A6BD033D.additionalIncludes preserve=no
//## end module%3912A6BD033D.additionalIncludes

//## begin module%3912A6BD033D.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                m4glbprt.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las funciones globales miscelaneas
//
//
//
//==============================================================================

#include "m4objglb.hpp"

#include <math.h>
#include <sys/stat.h>

#ifdef _WINDOWS
#include "locale.h"
#endif

#include "m4stl.hpp"
#include "m4win2ux.hpp"
#include "m4cryptc.hpp"
#include "m4unicode.hpp"
#include "chlog.hpp"
#include "m4glbres.hpp"

extern int	M4IsUnicode( void ) ;

//## end module%3912A6BD033D.includes

//## begin module%3912A6BD033D.declarations preserve=no
//## end module%3912A6BD033D.declarations

//## begin module%3912A6BD033D.additionalDeclarations preserve=yes
//## end module%3912A6BD033D.additionalDeclarations


//## begin module%3912A6BD033D.epilog preserve=yes

// correccion de tiempo a aplicar
m4date_t  M4_DECL_M4OBJGLB  g_dTimeCorrection = 0.0 ;


#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"



m4double_t M4VMRound(m4double_t ai_dValue, int ai_iScale /*=8*/ )
{
	m4double_t dNum, base;
	
    if (ai_iScale <=8 ){
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		//ai_dValue = ai_dValue + ( ( ai_dValue >= 0.0 ) ? M4_LN4_MATH_PRECISION_MODF : -M4_LN4_MATH_PRECISION_MODF ) ;
		ai_dValue = M4VMDoubleAdjust(ai_dValue);
		
		//******* Opcion 1: 1ºDividir y 2ºMultiplicar *******
		/*
        base=pow(10.0, - ai_iScale);
		dNum=ai_dValue;
		dNum/= base;
		*/
		
		//******* Opcion 2: 1ºMultiplicar y 2ºDividir *******
		base=pow(10.0, ai_iScale);
		dNum=ai_dValue;
		dNum*=base;
		
		//Ajustamos y truncamos con modf
		dNum += ( (dNum>0)? 0.5 : -0.5 ) ;
		modf (dNum, &ai_dValue);
		
		//******* Opcion 1: *******
		//multiplicamos por la base
		/*
		vValue.Data.DoubleData*=base;
		*/
		
		//******* Opcion 2: *******
		//dividimos por la base
		ai_dValue /= base;	
    }

    return ai_dValue;
}


//Funcion:	Convierte un Tipo M4 (varios tipos abstractos) al tipo CPP (4 tipos basicos) correspondiente.
//Entrada:	ai_iItemM4Type	= Tipo M4
//Salida:	return			= Tipo CPP
m4uint8_t ConvertM4TypeToCppType( m4uint8_t ai_iItemM4Type )
{
	m4uint8_t uiItemCppType = M4CL_CPP_TYPE_NULL;

    switch( ai_iItemM4Type )
    {
        case M4CL_M4_TYPE_FIXED_STRING :
        case M4CL_M4_TYPE_UNI_FIXED_STRING :
        case M4CL_M4_TYPE_VAR_STRING :
        case M4CL_M4_TYPE_UNI_VAR_STRING :
        case M4CL_M4_TYPE_LONG :
        case M4CL_M4_TYPE_UNI_LONG :
        case M4CL_M4_TYPE_VARIANT :
        case M4CL_M4_TYPE_UNI_VARIANT :
        case M4CL_M4_TYPE_BLOB :
        case M4CL_M4_TYPE_BINARY_STRING :

            uiItemCppType = M4CL_CPP_TYPE_STRING ;
            break ;

        case M4CL_M4_TYPE_DATE :
        case M4CL_M4_TYPE_TIMESTAMP :
        case M4CL_M4_TYPE_HOUR :

            uiItemCppType = M4CL_CPP_TYPE_DATE ;
            break ;

        case M4CL_M4_TYPE_NUMBER :
        case M4CL_M4_TYPE_CURRENCY :
        case M4CL_M4_TYPE_NUM_VARIANT :
        case M4CL_M4_TYPE_TIME_SPAN :

            uiItemCppType = M4CL_CPP_TYPE_NUMBER ;
            break ;

        default :
            break ;
    }
	
	return uiItemCppType;
}



// Funciones de conversión

m4return_t	M4ConvertOneWayMD5( m4pcchar_t ai_pccInString, m4pchar_t ao_pcOutString, m4uint16_t ai_iOutLength )
{

	m4return_t	iResult = M4_ERROR ;
	m4int_t		iLength = -1 ;

	iResult = EncryptOneWayMD5( ai_pccInString, strlen( ai_pccInString ) * sizeof( m4char_t ), ao_pcOutString, ai_iOutLength, iLength ) ;

	return( iResult ) ;
}


m4return_t M4ConvertPassword( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccPassword, m4pchar_t ai_pcPasswEncripted, m4int_t ai_iMaxLenOut )
{

	m4return_t	iResult = M4_ERROR;
	m4int_t		iUser = -1;
	m4int_t		iPassword = -1;
	m4int_t		iLength = -1;
	m4int_t		iUtf8Length = -1;
	m4pchar_t	pcUserPassword = NULL;
	m4pchar_t	pcUtf8UserPassword = NULL;


	iUser = strlen( ai_pccUser ) ;
	iPassword = strlen( ai_pccPassword ) ;

	pcUserPassword = new m4char_t[ iUser + iPassword + 1 ] ;
	
	memcpy( pcUserPassword, ai_pccUser, iUser ) ;
	memcpy( pcUserPassword + iUser, ai_pccPassword, iPassword + 1 ) ;

	pcUtf8UserPassword = M4CppToUtf8( pcUserPassword, iUtf8Length ) ;
	delete( pcUserPassword ) ;

	iResult = EncryptOneWaySHA( pcUtf8UserPassword, strlen( pcUtf8UserPassword ) * sizeof(m4char_t), ai_pcPasswEncripted, ai_iMaxLenOut, iLength ) ;
	delete( pcUtf8UserPassword ) ;

	return iResult ;
}


m4return_t  M4ComparePassword( m4pcchar_t ai_pccUser, m4pcchar_t ai_pccPassword, m4pcchar_t ai_pccStoredPassword, m4double_t ai_dReserved, m4bool_t ai_bIs2FA, m4bool_t &ao_rbEqual, m4pchar_t &ao_rpcNewPassword, m4pchar_t &ao_rpcVerificationCode )
{

    m4return_t      iResult = M4_ERROR;
	m4bool_t		bNewPassword = M4_FALSE;
	m4int_t			iLength = -1;
	size_t			iStoredPassword = 0;
	m4pchar_t		pcOpenPassword = NULL;
	m4pchar_t		pcVerificationCode = NULL;
	m4pchar_t		pcOpenPasswordUnicode = NULL;
	m4char_t		acOneWayPassword   [ 2 * M4_MAX_LEN_ONE_WAY + 1 ];
	m4char_t		acOneWayPasswordMD5[ 2 * M4_MAX_LEN_MD5 + 1 ];
	
	
	ao_rbEqual = M4_FALSE ;
	ao_rpcVerificationCode = NULL ;
	ao_rpcNewPassword = NULL ;

	// Se desencrypta la password
	iResult = DecryptTwoWay( ai_pccPassword, strlen( ai_pccPassword ) * sizeof( m4char_t ), M4CH_DUMMY_B2 + 23, strlen( M4CH_DUMMY_B2 + 23 ) * sizeof( m4char_t ), pcOpenPassword, iLength ) ;
	
	if( iResult != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	if( ai_bIs2FA == M4_TRUE )
	{
		iLength = strlen( pcOpenPassword ) ;

		if( iLength <= 6 )
		{
			return M4_SUCCESS;
		}

		pcVerificationCode = new m4char_t[ 6 + 1 ] ;
		CHECK_CHLOG_ERRORF( pcVerificationCode == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * (6 + 1) ) << __FILE__ << __LINE__ ) ;
		memcpy( pcVerificationCode, pcOpenPassword + iLength - 6, 6 + 1 ) ;
		*( pcOpenPassword + iLength - 6 ) = '\0' ;
	}

	iStoredPassword = strlen( ai_pccStoredPassword ) ;
	
	if( iStoredPassword == M4_MAX_LEN_ONE_WAY )
	{
		if( *ai_pccStoredPassword >= '0' && *ai_pccStoredPassword <= 'f' )
		{
			// Modo nuevo
			*acOneWayPassword = '\0' ;
			iResult = M4ConvertPassword( ai_pccUser, pcOpenPassword, acOneWayPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

			if( iResult != M4_SUCCESS )
			{
				if( pcVerificationCode != NULL )
				{
					delete( pcVerificationCode ) ;
				}
				delete( pcOpenPassword ) ;
				return M4_ERROR;
			}

			if( strcmp( acOneWayPassword, ai_pccStoredPassword ) == 0 )
			{
				ao_rbEqual = M4_TRUE ;
			}
		}
		else
		{
			// Modo nuevo con md5
			if( *ai_pccStoredPassword < '0' )
			{
				*(m4pchar_t)ai_pccStoredPassword += 10 ;
			}
			else
			{
				*(m4pchar_t)ai_pccStoredPassword -= 6 ;
			}

			iResult = M4ConvertOneWayMD5( pcOpenPassword, acOneWayPasswordMD5, 2 * M4_MAX_LEN_MD5 ) ;

			if( iResult != M4_SUCCESS )
			{
				if( pcVerificationCode != NULL )
				{
					delete( pcVerificationCode ) ;
				}
				delete( pcOpenPassword ) ;
				return M4_ERROR;
			}

			*acOneWayPassword = '\0' ;
			iResult = M4ConvertPassword( ai_pccUser, acOneWayPasswordMD5, acOneWayPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

			if( iResult != M4_SUCCESS )
			{
				if( pcVerificationCode != NULL )
				{
					delete( pcVerificationCode ) ;
				}
				delete( pcOpenPassword ) ;
				return M4_ERROR;
			}

			if( strcmp( acOneWayPassword, ai_pccStoredPassword ) == 0 )
			{
				ao_rbEqual = M4_TRUE ;
				bNewPassword = M4_TRUE ;
			}
			else
			{
				// Se compara en el coding alternativo al modo nuevo con md5
				iLength = -1 ;

				if( M4IsUnicode() == 0 )
				{
					pcOpenPasswordUnicode = M4CppToUtf8( pcOpenPassword, iLength ) ;
				}
				else
				{
					pcOpenPasswordUnicode = M4CppToANSI( pcOpenPassword, iLength ) ;
				}

				iResult = M4ConvertOneWayMD5( pcOpenPasswordUnicode, acOneWayPasswordMD5, 2 * M4_MAX_LEN_MD5 ) ;
				delete( pcOpenPasswordUnicode ) ;

				if( iResult != M4_SUCCESS )
				{
					if( pcVerificationCode != NULL )
					{
						delete( pcVerificationCode ) ;
					}
					delete( pcOpenPassword ) ;
					return M4_ERROR;
				}

				*acOneWayPassword = '\0' ;
				iResult = M4ConvertPassword( ai_pccUser, acOneWayPasswordMD5, acOneWayPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

				if( iResult != M4_SUCCESS )
				{
					if( pcVerificationCode != NULL )
					{
						delete( pcVerificationCode ) ;
					}
					delete( pcOpenPassword ) ;
					return M4_ERROR;
				}

				if( strcmp( acOneWayPassword, ai_pccStoredPassword ) == 0 )
				{
					ao_rbEqual = M4_TRUE ;
					bNewPassword = M4_TRUE ;
				}
			}
		}
	}


	if( bNewPassword == M4_TRUE )
	{
		// Si hay que cambiar la password de formato se encripta one way de la manera nueva
		ao_rpcNewPassword = new m4char_t [2 * M4_MAX_LEN_ONE_WAY + 1] ;
		CHECK_CHLOG_ERRORF( ao_rpcNewPassword == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * (2 * M4_MAX_LEN_ONE_WAY + 1) ) << __FILE__ << __LINE__ ) ;
		
		iResult = M4ConvertPassword( ai_pccUser, pcOpenPassword, ao_rpcNewPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

		if( iResult != M4_SUCCESS )
		{
			delete( ao_rpcNewPassword ) ;
			ao_rpcNewPassword = NULL ;

			if( pcVerificationCode != NULL )
			{
				delete( pcVerificationCode ) ;
			}
			delete( pcOpenPassword ) ;
			return M4_ERROR;
		}
	}

	if( ai_bIs2FA == M4_TRUE )
	{
		ao_rpcVerificationCode = pcVerificationCode ;
	}

	delete( pcOpenPassword ) ;
	return M4_SUCCESS;
}


// Function that returns the extension of a file name
// In case it doesn't exists, returns a null string (not null ptr)
m4pcchar_t M4ClGetFileExtension(m4pcchar_t ai_psFileName)
{
	m4int32_t i,
             last_pos;

	for (i=0, last_pos=-1;
	     ai_psFileName[i]!=0;
		  i++)
      if (ai_psFileName[i]==M4CL_START_EXTENSION_OF_FILE_NAME)
			last_pos=i;

	if (last_pos==-1)
		return &ai_psFileName[i]; // You need to keep this implementation because
	   // the addresses are used by the serialization proccess to verify repeated
	   // file names/extensions
	else
		return &ai_psFileName[last_pos+1];
}

// Function that compare file names
int M4ClFileNameComp(m4pcchar_t ai_psFileName1,
							m4pcchar_t ai_psFileName2)
{
   #ifdef _UNIX
	return strcmp(ai_psFileName1,ai_psFileName2);
   #else
	return _stricmp(ai_psFileName1,ai_psFileName2);
   #endif
}


// el locale de windows

void	M4ResetLocale( void )
{
#ifdef _WINDOWS
	setlocale( LC_ALL, "C" ) ;
#endif
}





// Para el tipo de operación de un item

m4uint32_t	M4GetDBOperation( m4pcchar_t ai_pccItem )
{

	if( ai_pccItem == NULL || *ai_pccItem == '\0' )
	{
		return( 0 ) ;
	}

	if(    strcmpi( ai_pccItem, M4CL_ITEM_ROOT_LOAD ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_LOAD_PRG ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_LOAD_BLK ) == 0
	  )
	{
		return( 1 ) ;
	}

	if(    strcmpi( ai_pccItem, M4CL_ITEM_INSERT_PRG ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_INSERT_BLK ) == 0
	  )
	{
		return( 2 ) ;
	}

	if(    strcmpi( ai_pccItem, M4CL_ITEM_UPDATE_PRG ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_UPDATE_BLK ) == 0
	  )
	{
		return( 3 ) ;
	}

	if(    strcmpi( ai_pccItem, M4CL_ITEM_DELETE_PRG ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_UPDATE_BLK ) == 0
	  )
	{
		return( 4 ) ;
	}

	if(    strcmpi( ai_pccItem, M4CL_ITEM_PERSIST_TREE ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_BEGIN_TRANSACTION ) == 0
		|| strcmpi( ai_pccItem, M4CL_ITEM_END_TRANSACTION ) == 0
	  )
	{
		return( 5 ) ;
	}

	return( 0 ) ;
}

//## end module%3912A6BD033D.epilog
