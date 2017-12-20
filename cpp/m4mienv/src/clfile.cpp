//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clfile.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifdef _UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <io.h>
#include <direct.h>
#endif

#ifdef _WINDOWS
#include <windows.h>
#include "regedit.hpp"
#include "m4filedownldr.hpp"
#endif

#include "m4string.hpp"
#include "mit.h"
#include "trace.h"
#include "mitdefs.cpp"
#include "clfile.hmg"
#include "clfile.h"
#include "clstr.h"
#include "sysinfo.h"
#include "m4regdef.hpp"
#include "m4unicode.hpp"
#include "m4buildversion.hpp"



ClMIT_File::ClMIT_File() 
{
	m_pstF=NULL;
	m_StPath.Str=NULL ;
	m_poInsp=GET_INSP();

}

//NOTA: Open NO debe contabilizar ninguna alocacion en Trace que se desaloque en Close, 
//	porque los test de salida en Trace deben ejecutarse ANTES del Close(), y se provocaria
//	un error.

m4bool_t ClMIT_File::Open( 
		const m4char_t *ai_pcPath, ClMIT_File::MODE_EXIST ai_Exist, 
		ClMIT_File::MODE_WRITE ai_Write, ClMIT_File::ACTION_ON_OPEN ai_Mode, m4bool_t ai_bAddBOM)
{
	char *Mode ;
	int iWrite = 0 ;
	int iLength = -1 ;
	char* pcFileName = NULL ;
	m4bool_t Return=M4_TRUE ;
	
	// Verificar si el fichero se puede bajar desde una URL (necesario para el RichWeb).
	// Si se ha podido descargar, en pFilePath estará el path local donde se ha dejado.
	// Si no se ha podido descargar, la función devuelve false.
	m4char_t pszFilePath[M4_MAX_PATH + 1];
	*pszFilePath = '\0';
	if (TryToDownloadFromURL(ai_pcPath, M4_MAX_PATH, pszFilePath) != M4_TRUE)
	{
		strcpy(pszFilePath, ai_pcPath);
	}

	if (IsOpen()) {
		if (ai_Mode!=SILENT_TRY_OPEN) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_10 ] ) << pszFilePath << SEND_MSG ;
		} ;
		Return=M4_FALSE ;
		goto exit;
	} ;

	if ( (ai_Exist == EXIST &&  ai_Write==WRITE) || (ai_Exist == NEW &&  ai_Write==READ) 
		|| (ai_Exist == OVERWRITE &&  ai_Write==READ) ) 
	{
		if (ai_Mode!=SILENT_TRY_OPEN) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_20 ] ) << pszFilePath << SEND_MSG ;
		} ;
		Return=M4_FALSE ;
		goto exit;
	} ;
	if ( ai_Exist == EXIST ) {
		if ( !Exist(pszFilePath) )  {
			if (ai_Mode!=SILENT_TRY_OPEN) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_30 ] ) << pszFilePath << SEND_MSG ;
			} ;
			Return=M4_FALSE ;
			goto exit;
		} ;
		if (ai_Write == READ ) {
			Mode="rb";
		} else if (ai_Write == READ_WRITE ) {
			Mode="rb+";
		} ;
	} else {
		if ( ai_Exist == NEW ) {
			if ( Exist(pszFilePath) )  {
				if (ai_Mode!=SILENT_TRY_OPEN) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_40 ] ) << pszFilePath << SEND_MSG ;
				} ;
				Return=M4_FALSE ;
				goto exit;
			} ;
		} ;
		if (ai_Write == WRITE ) {
			Mode="wb";
		} else if (ai_Write == READ_WRITE ) {
			Mode="wb+";
		} ;
	} ;

	// UNICODE FILE
	iLength = -1 ;
	pcFileName = M4CppToANSI( pszFilePath, iLength ) ;
	m_pstF=fopen(pcFileName, Mode);
	delete [] pcFileName ;
	
	if (! IsOpen() ) {
		if (ai_Mode!=SILENT_TRY_OPEN) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_50 ] ) << pszFilePath << SEND_MSG ;
		} ;
		Return=M4_FALSE ;
		goto exit;
	} ;

	// UNICODE FILE
	/* Bug 0125700
	Si el fichero debe tener BOM en unicode se añade manualmente (para trazas)
	*/
	if( ai_bAddBOM == M4_TRUE && ai_Write == WRITE )
	{
		iWrite = M4WriteBOM( m_pstF ) ;
	}


exit:
	if (Return) {
		m_StPath.Size = strlen(pszFilePath)+1 ;
		//M4_NEW_ARRAY(g_oMIMemProf, MEM_GRP_OTHERS, 
		//	m_StPath.Str, m4char_t, m_StPath.Size
		//) ;
		m_StPath.Str = new m4char_t[m_StPath.Size] ;
		strcpy(m_StPath.Str, pszFilePath) ;		
	} ;

	if (ai_Mode!=SILENT_TRY_OPEN) {
		m_poInsp->m_poTrace->TestEffect( Return, ai_Mode==FORCE_OPEN ? ClMIT_Trace::IMMEDIATE_ERROR : ClMIT_Trace::PROPAGATE_ERROR ) ; 
	} ;
	return Return ;		
}

void ClMIT_File::Close() {
	if (IsOpen()) {
		fclose(m_pstF);
		m_pstF=NULL;
	} ;
	//M4_DELETE_ARRAY(g_oMIMemProf, MEM_GRP_OTHERS, 
	//	m_StPath.Str, m4char_t, m_StPath.Size
	//) ;
	if (m_StPath.Str)  {
		delete m_StPath.Str ; 
		m_StPath.Str=NULL ;
	} ;
}

void ClMIT_File::W( const void * ai_pvBuff, size_t ai_szNBytes ) 
{
	size_t szWritten;

	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::W()" << SEND_MSG ;
		return ;
	} ;
	if (ai_szNBytes) {
		
		szWritten = fwrite( ai_pvBuff, 1, ai_szNBytes, m_pstF );

		if (  szWritten!= ai_szNBytes ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_60 ] ) << SEND_MSG ;
			//Efecto local
		} ;
	} ;
}

void ClMIT_File::WStr( const m4char_t * ai_pcStr ) 
{
	W(ai_pcStr, strlen(ai_pcStr)) ;
}


void ClMIT_File::WChar(m4char_t ai_pcChar) 
{
	if(fputc(ai_pcChar, m_pstF)==EOF){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
	};
}

size_t ClMIT_File::R( void * ai_pvBuff, size_t ai_szNBytes ) 
{
	size_t szReturn ;

	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::R()" << SEND_MSG ;
		return 0 ;
	} ;

	//Ojo, szReturn queremos que de el numero de bytes, no es lo mismo un ai_szNBytes,1 
	// (ademas, creo que esto forzaria a leer bloques de al menos ai_szNBytes, y no menores)

	szReturn = fread(ai_pvBuff,1,ai_szNBytes,m_pstF);	
	
	return szReturn ;
}


m4uint32_t ClMIT_File::GetPos() 
{
	m4uint32_t uiReturn ;

	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::GetPos()" << SEND_MSG ;
		return 0;
	} ;

	uiReturn = ftell(m_pstF) ;

	return uiReturn ;
}

void ClMIT_File::SetPos( m4uint32_t ai_uiOff ) 
{
	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::SetPos()" << SEND_MSG ;
		return ;
	} ;

	fseek(m_pstF, ai_uiOff, SEEK_SET ) ;
}

m4bool_t ClMIT_File::IsEOF() 
{
	m4bool_t swReturn;

	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::IsEOF()" << SEND_MSG ;
		return M4_TRUE;
	} ;
	
	swReturn = M4_BOOL( (feof(m_pstF)!=0) );

	return swReturn ;
}


m4bool_t ClMIT_File::Exist(const m4char_t *ai_pcStr) 
{
	// UNICODE FILE
	int		iAccess = -1 ;
	int		iLength = -1 ;
	char*	pcFileName = NULL ;

	iLength = -1;
	pcFileName = M4CppToANSI( ai_pcStr, iLength ) ;
	iAccess = access( pcFileName, 0 ) ;
	delete [] pcFileName ;

	return M4_BOOL( iAccess == 0 ) ;
}

m4bool_t ClMIT_File::Remove(const m4char_t *ai_pcStr, m4bool_t ai_SwMsg)
{
	m4bool_t R ;

	// UNICODE FILE
	int		iRemove = -1 ;
	int		iLength = -1 ;
	char*	pcFileName = NULL ;

	iLength = -1;
	pcFileName = M4CppToANSI( ai_pcStr, iLength ) ;
	iRemove = remove( pcFileName ) ;
	delete [] pcFileName ;

	R = (M4_BOOL( iRemove == 0 ));

	if ( ! R && ai_SwMsg ) {
		*(GET_INSP()->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_70 ] ) << ai_pcStr << SEND_MSG ;
		//Efecto local
	} ;
	return R ;
}


m4bool_t ClMIT_File::Move(const m4char_t *ai_pcSource, const m4char_t *ai_pcDest, m4bool_t ai_SwMsg) 
{
	m4bool_t R ;

	// UNICODE FILE
	int		iRename = -1 ;
	int		iLength = -1 ;
	char*	pcFileNameSource = NULL ;
	char*	pcFileNameDest = NULL ;

	iLength = -1;
	pcFileNameSource = M4CppToANSI( ai_pcSource, iLength ) ;
	iLength = -1;
	pcFileNameDest = M4CppToANSI( ai_pcDest, iLength ) ;

	iRename = rename( pcFileNameSource, pcFileNameDest ) ;
	delete [] pcFileNameSource ;
	delete [] pcFileNameDest ;

	R = (M4_BOOL( iRename == 0 ));

	if ( ! R && ai_SwMsg ) {

		*(GET_INSP()->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_90 ] ) 
			<< ai_pcSource << ai_pcDest << SEND_MSG ;
		//Efecto local
	} ;
	return R ;
}



void ClMIT_File::Flush() 
{
	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::Flush()" << SEND_MSG ;
		return ;
	} ;

	fflush(m_pstF) ;
}

m4uint32_t ClMIT_File::GetSize() 
{
	m4uint32_t Pos, Size ;

	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::GetSize()" << SEND_MSG ;
		return 0;
	} ;
	Pos=GetPos() ;
	fseek(m_pstF, 0, SEEK_END ) ;
	Size=ftell(m_pstF) ;
	SetPos(Pos) ;
	return Size ;
}

m4bool_t ClMIT_File::Compare( const m4char_t * ai_pcFileA, const m4char_t * ai_pcFileB ) 
{
	m4bool_t R=M4_TRUE ;
	ClMIT_File FA, FB ;	
	size_t SzA, SzB ;
	StMIT_ThreadEnv *Env=GET_THREAD_ENV() ;

	R = M4_BOOL( R && FA.Open( ai_pcFileA, EXIST, READ, SILENT_TRY_OPEN, M4_FALSE ) );
	if (!R) goto exit ;

	R = M4_BOOL( R && FB.Open( ai_pcFileB, EXIST, READ, SILENT_TRY_OPEN, M4_FALSE) );
	if (!R) goto exit ;

	R = M4_BOOL( R && FA.GetSize() == FB.GetSize() );
	if (!R) goto exit ;

	while (R && !FA.IsEOF() && !FB.IsEOF() ) {
		SzA = FA.R(Env->File_pcBuffA,StMIT_ThreadEnv::File_COMPARE_SIZE) ;
		SzB = FB.R(Env->File_pcBuffB,StMIT_ThreadEnv::File_COMPARE_SIZE) ;
		if (SzA!=SzB)  {
			R=M4_FALSE ;
			continue ;
		}  ;
		if (SzA>0) R = M4_BOOL( memcmp( Env->File_pcBuffA, Env->File_pcBuffB, SzA ) == 0 );		
	} ;


exit:

	FA.Close() ;
	FB.Close() ;

	return R ;
}

m4bool_t ClMIT_File::WFile( const m4char_t * ai_pcFile, m4uint32_t ai_From, size_t ai_Count,
		size_t *ao_szWritten ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StMIT_ThreadEnv *Env=GET_THREAD_ENV() ;
	size_t  Written=0, Read ;	//Info debug
	ClMIT_File FileR ;
	
	if ( ! FileR.Open(ai_pcFile,EXIST,READ,TRY_OPEN, M4_FALSE)  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	Written=0 ;
	FileR.SetPos(ai_From) ;
	if (ai_Count==-1) {
		while ( !FileR.IsEOF() ) {
			Read=FileR.R( Env->File_pcBuffA, StMIT_ThreadEnv::File_COMPARE_SIZE ) ;
			W( Env->File_pcBuffA, Read ) ;
			Written += Read ;
		} ;
	} else {
		while ( !FileR.IsEOF() && ai_Count ) {
			Read=FileR.R( Env->File_pcBuffA,  
				M4_MIN( StMIT_ThreadEnv::File_COMPARE_SIZE, ai_Count) ) ;
			W( Env->File_pcBuffA, Read ) ;
			Written += Read ;
			ai_Count -= Read ;
		} ;
	} ;
exit:
	if (ao_szWritten) *ao_szWritten=Written ;
	return SwOk ;
}


size_t ClMIT_File::DumpToString( ClMIT_Str *ao_Str ) 
{
	size_t sz ;
	
	ao_Str->StrCpy("") ;
	
	if (!IsOpen()) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFILE_MSG_80 ] ) << "ClMIT_File::DumpToString()" << SEND_MSG ;
		return 0 ;
	} ;

	sz = GetSize()+1 ; //+1 para el null final
	SetPos(0) ;
	ao_Str->Accommodate(sz) ;
	fread( ao_Str->InChar(0),1,sz-1, m_pstF ) ;
	*ao_Str->InChar(sz-1)=M4_END_STR ;

	return sz-1 ;
}

//------------ Tratamiento de Paths y Directorios -----------------------

#define ALL_PATH_SEP "\\/" 

#ifdef _UNIX
	const m4char_t ClMIT_File::m_cPathSep='/' ;
	const m4char_t ClMIT_File::m_cExtSep='.' ;
	const m4char_t ClMIT_File::m_cDriveSep='/' ;
#else
	const m4char_t ClMIT_File::m_cPathSep='\\' ;
	const m4char_t ClMIT_File::m_cExtSep='.' ;
	const m4char_t ClMIT_File::m_cDriveSep=':' ;
#endif

m4bool_t ClMIT_File::SplitPath( const m4char_t * ai_pcFullPath, ClMIT_Str *ao_poPath, 
                        		ClMIT_Str *ao_poFile, ClMIT_Str *ao_poExt)
{
	m4bool_t SwOk;
	m4pcchar_t pcPos ;
	m4uint16_t iPos, iCounter=0;
	
	pcPos = strrchr(ai_pcFullPath, m_cPathSep);
	
	if(pcPos){
		ao_poFile->StrCpy(pcPos+1);
		ao_poPath->ToASCIZ(ai_pcFullPath, (int)(pcPos - ai_pcFullPath +1));
	}else{
		ao_poFile->StrCpy(ai_pcFullPath) ;
		ao_poPath->StrCpy("") ;
	};

	if(ao_poExt){
		pcPos = strchr(ao_poFile->InChar(0), '.');
		if(pcPos){
			ao_poExt->StrCpy(pcPos);
			ao_poFile->StrDuplicateChar((int)(pcPos - ao_poFile->InChar(0)), 1, M4_END_STR);
		};
	};

	pcPos = strchr( ao_poPath->InChar(0), m_cPathSep );
	while (pcPos!=NULL) {
		iCounter++;
		iPos = pcPos-ao_poPath->InChar(0)+1;
		pcPos = strchr( ao_poPath->InChar(iPos), m_cPathSep );   
	};

	if (iCounter>1){
		SwOk=M4_TRUE;
	} else {
		SwOk=M4_FALSE;
	};

	return SwOk;
};



void ClMIT_File::ConvertToPath(ClMIT_Str *ao_poPath)
{
	size_t l = ao_poPath->StrLen() ;
	if ( l>0 ) {
		if ( *ao_poPath->InChar(l-1) != m_cPathSep && 
			*ao_poPath->InChar(l-1)  != m_cDriveSep )
		{
			ao_poPath->StrInsertChar(ClMIT_Str::STR_CAT, 1, m_cPathSep);
		};
	} ;
};

void ClMIT_File::ExpandPathName( ClMIT_Str *ao_poPath, const m4char_t *ai_pcPathExt ) 
{
	size_t l ;

	l=ao_poPath->StrLen() ;
	if ( *ao_poPath->InChar(l-1) == m_cPathSep ) {
		ao_poPath->StrDeleteSegment( l-1, 1 ) ;
	} ;
	ao_poPath->StrCat(ai_pcPathExt) ;
} 


void ClMIT_File::ConvertToThisPlatform(ClMIT_Str *ao_poPath)
{
	m4char_t *Pos, *Beg ;
	
	for (Pos=Beg=ao_poPath->InChar(0); *Pos!=M4_END_STR; Pos++) {
		if ( strchr(ALL_PATH_SEP, *Pos) ) {
			*Pos = m_cPathSep ;
		} ;
	};
};

void ClMIT_File::ConvertToWebPath(ClMIT_Str *ao_poPath) 
{
	m4char_t *Pos, *Beg;

	for (Pos=Beg=ao_poPath->InChar(0); *Pos!=M4_END_STR; Pos++) {
		if(*Pos=='\\') *Pos= '/';
	};
}



void ClMIT_File::ConcatFileExt(ClMIT_Str *ao_poFile,const m4char_t *ai_pcExt)
{
	if(ai_pcExt){
		if ( *ai_pcExt != m_cExtSep ) {
			ao_poFile->StrInsertChar(ClMIT_Str::STR_CAT, 1, m_cExtSep);
		} ;
		ao_poFile->StrCat(ai_pcExt);
	};
};

void ClMIT_File::ConcatToPath(ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr)
{
	ConvertToPath(ao_poPath);
	ao_poPath->StrCat(ai_pcStr);
};

void ClMIT_File::InsertFileName(ClMIT_Str *ao_poPath, const m4char_t *ai_pcStr)
{
	m4char_t* pcPos;

	pcPos = strrchr(ao_poPath->InChar(0), m_cExtSep);
	if(pcPos) {
		ao_poPath->StrInsertStr(pcPos-ao_poPath->InChar(0),ai_pcStr);
	} else {
		ao_poPath->StrCat(ai_pcStr);
	};
};

ClMIT_File::CREATE_PATH_RETURN ClMIT_File::CreatePath(const m4char_t *ai_pcPath, ACTION_ON_CREATE_PATH ai_Action)
{
	CREATE_PATH_RETURN Return;
	ClMIT_Str DirString, AuxString ;
	m4int16_t iPos=0, iOldPos=0;
	m4char_t *pcPos;


	AuxString.StrCpy(ai_pcPath) ;
	ConvertToPath(&AuxString);
	
	do {
		pcPos = strchr(AuxString.InChar(iOldPos), m_cPathSep);
		
		if(pcPos){
			iPos=pcPos - AuxString.InChar(iOldPos)+1;
			DirString.ToASCIZ( AuxString.InChar(0), iPos+iOldPos);
			M4MakeDirectory(DirString.InChar(0));
			iOldPos+=iPos;
		};
	} while (pcPos);

	return ClMIT_File::PATH_CREATED;
};


m4bool_t ClMIT_File::GetRelativePath(
	const m4char_t *ai_pcFromPath, const m4char_t *ai_pcToPath, ClMIT_Str *ao_RelativePath,
	m4bool_t ai_bSwAllIsPath) 
{
	ClMIT_Str from, to ;
	size_t lFrom, lTo, lEqual, lAux ;
	const m4char_t *pFrom, *pTo ;

	//Copia a from-to de trabajo. Estos ya son path o files segun acaben o no en /

	from.StrCpy(ai_pcFromPath) ;
	to.StrCpy(ai_pcToPath) ;
	if ( ai_bSwAllIsPath ) {
		ConvertToPath(&from) ;
		ConvertToPath(&to) ;
	} ;
	lFrom = from.StrLen() ; 
	lTo = to.StrLen() ; 

	//Haya len parte comun a los dos paths, sin permitir partir paths.
	//Para ello, el ultimo char igual debera ser un / (por lo que en ambas cadenas los 
	//	primeros no iguales siempre son o un char no / o fin e cadena)

	for ( pFrom=from.InChar(0), pTo=to.InChar(0), lEqual=0, lAux=0 ; *pFrom && *pTo; ++pFrom, ++pTo ) {
		if ( toupper(*pFrom) != toupper(*pTo) ) {
			break ;
		} else {
			++lAux ;
			if ( *pFrom==m_cPathSep ) {
				lEqual=lAux ;
			} ;
		} ;
	} ;

	//Si no hay parte común, devolver path from y ya esta.

	if (lEqual==0) {
	    ao_RelativePath->StrCpy(to) ;
		return M4_FALSE ;
	} ;

	//Tengo que navegar en el from hasta la parte comun. Por cada path (no fichero), añadir ..
	//Para saber cuantos .. hay que añadir basta con contar los \ de la parte no comun.

	for ( pFrom=from.InChar(lEqual), ao_RelativePath->StrCpy("") ; *pFrom; ++pFrom ) {
		if (*pFrom==m_cPathSep) {
			ao_RelativePath->StrCat("..") ;
			ao_RelativePath->StrInsertChar(ClMIT_Str::STR_CAT,1,m_cPathSep) ;
		} ;
	} ;

	//Y ahora tengo que añadir toda la parte no comun del to 

	ao_RelativePath->StrCat( to.InChar(lEqual) ) ;

	return M4_TRUE ;
}



#ifdef _WINDOWS

//=============================================================================
// Convierte una cadena a minusculas y cambia los slash a formato unix
// Tiene en cuenta que sólo hay parte de la cadena que hay que pasar a minúsculas
// Para ello tiene en cuenta la raiz que se lee del registro
//=============================================================================

m4pchar_t	_NormalizePath( m4pcchar_t ai_pccPath )
{

	m4pchar_t	pcResult = NULL ;
	m4return_t	iResult = M4_SUCCESS ;
	char		cResult = TRUE ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;
	m4uint32_t	iOut = 0 ;
	m4char_t	acRegistryPath[ MAX_PATH + 1 ] = "" ;

	static	m4bool_t	s_bInitialized = M4_FALSE ;
	static	m4uint32_t	s_iSize = 0 ;
	static	m4char_t	s_acPattern[ MAX_PATH + 1 ] = "" ;


	if( s_bInitialized == M4_FALSE )
	{
		s_bInitialized = M4_TRUE ;

		// Se compone el path al patrón
		iResult = M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, "CLIENT\\Presentation Runtime Vars", acRegistryPath, MAX_PATH, iOut ) ;

		if( iResult == M4_SUCCESS )
		{
			// Se lee el patrón
			cResult = GetRegKeyData( acRegistryPath, "CommonFiles_Path", s_acPattern ) ;

			if( cResult == TRUE )
			{
				s_iSize = strlen( s_acPattern ) ;

				// Se normalizan las barras del patrón
				for( i = 0 ; i < s_iSize ; i++ )
				{
					if( s_acPattern[ i ] == '\\' )
					{
						s_acPattern[ i ] = '/' ;
					}
				}
			}
		}
	}


	if( ai_pccPath == NULL )
	{
		return( NULL ) ;
	}

	iLength = strlen( ai_pccPath ) ;

	// Se crea la cadena de salida con la cadena original
	pcResult = new m4char_t[ iLength + 1 ] ;
	
	if( pcResult == NULL )
	{
		return( NULL ) ;
	}

	memcpy( pcResult, ai_pccPath, iLength + 1 ) ;

	// Primero se normalizan las barras
	for( i = 0 ; i < iLength ; i++ )
	{
		if( pcResult[ i ] == '\\' )
		{
			pcResult[ i ] = '/' ;
		}
	}

	// Si el comienzo coincide con el patrón se pasa a minúsculas el final
	if( s_iSize > 0 && strnicmp( pcResult, s_acPattern, s_iSize ) == 0 )
	{
		for( i = s_iSize ; i < iLength ; i++ )
		{
			pcResult[ i ] = tolower( pcResult[ i ] ) ;
		}
	}

	return( pcResult ) ;
}

#endif



m4bool_t ClMIT_File::TryToDownloadFromURL(const m4char_t * ai_pFromPath, m4int32_t ai_iTargetFilePathLen, m4char_t * aio_pTargetFilePath) 
{
	// Verificar los argumentos de entrada.
	if (ai_iTargetFilePathLen <= 0 || aio_pTargetFilePath == NULL)
	{
		return M4_FALSE;
	}

#ifdef _WINDOWS

	HRESULT		hr = 0;
	int			iLength = -1 ;
	char*		pcFileName = NULL ;
	m4pchar_t	pcNormalizedPath = NULL;
	M4FileDownloader oDownloader;
	m4return_t	iRet;
	ClMIT_Str	sTempDir;
	size_t		iLen;
	m4pchar_t	pcDotPos;

	pcNormalizedPath = _NormalizePath( ai_pFromPath ) ;

	/* Bug 0156464
	Se pasa al formato ANSI el url porque la función lo espera en ANSI
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( pcNormalizedPath, iLength ) ;
	delete [] pcNormalizedPath ;


	// bg 296460
	if( *aio_pTargetFilePath == '\0' )
	{
		m_poInsp->m_poSysInfo->GetValue( MIT_RK_WORKDIR, &sTempDir );

		iLen = sTempDir.StrLen();
		if( iLen > 0  )
		{
			pcDotPos = strrchr( pcFileName, '/' );
			if( pcDotPos == NULL || *( pcDotPos + 1 ) == NULL )
			{
				return M4_FALSE;
			}

			iLen += strlen( pcDotPos + 1 );
			iLen++; // slash char
			if( iLen > ai_iTargetFilePathLen )
			{
				return M4_FALSE;
			}

			sprintf( aio_pTargetFilePath, "%s/%s", ( m4char_t * )sTempDir, pcDotPos + 1 );
		}
	}
	
	// Descargar el fichero desde la URL a local.
	// bg 275109
	iRet = oDownloader.DownloadFile( pcFileName, aio_pTargetFilePath, ai_iTargetFilePathLen );
	delete [] pcFileName ;

	if (iRet != M4_SUCCESS)
	{
		return M4_FALSE;
	}

	/* Bug 0156464
	Se pasa al formato Cpp el path que se ha devuelto en ANSI
	*/
	iLength = -1 ;
	pcFileName = M4ANSIToCpp( aio_pTargetFilePath, iLength ) ;

	if( iLength > ai_iTargetFilePathLen )
	{
		*aio_pTargetFilePath = '\0' ;
		delete [] pcFileName ;
		return M4_FALSE;
	}

	memcpy( aio_pTargetFilePath, pcFileName, iLength + 1 ) ;
	delete [] pcFileName ;

	return M4_TRUE;

#else
	return M4_FALSE;
#endif
}

