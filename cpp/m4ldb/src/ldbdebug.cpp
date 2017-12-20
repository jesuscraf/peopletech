//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                ldbdebug.cpp   
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


#include "defmain.hpp"

#ifdef _M4LDB_DEBUG
#include "ldbdebug.hpp"


#include "cldates.hpp"
#include "m4log.hpp"
#include "access.hpp"
#include "clldb.hpp"
#include "cllconn.hpp"
#include "cursor.hpp"
#include "ldbcontext.hpp"
#include "m4buildversion.hpp"
#include "m4mdrt.hpp"
#include "cllgadap.hpp"
#include "m4sysalloc.hpp"
#include "blocksynchronization.hpp"
#include "nodedef.hpp"
#include "m4unicode.hpp"
#include "m4clock.hpp"



// Warning : This macro only can be used inside ClLDBInspector
#define M4_LDB_PRINT_IN_LDB_INSP(x)		do { if( _CreateLDBFile() == M4_TRUE ) { m_iActualPos += x ; if( m_iActualPos > m_iMaxPos ) { _ChangeLDBInspector() ; } } } while( 0 )



//Detalle de trazas fichero.
m4cint8_t *g_sMaskTypeToTrace[] = { "UNDEFINED","ROLE","USER","M4OBJECT","NODE" };

ClLDBInspector *g_pInspector = NULL;


ClLDBInspector::ClLDBInspector (m4pcchar_t ai_pccFile1, m4pcchar_t ai_pccFile2, m4int32_t ai_iSize)
{
	m_bInit = M4_FALSE;
	m_pContext = NULL;
	m_lReferences = 0;

	m_pFile1 = NULL;
	m_pFile2 = NULL;
	m_pFile = NULL;

	m_pcPathFile1 = new m4char_t[strlen(ai_pccFile1) + 1];
	strcpy(m_pcPathFile1 ,ai_pccFile1);

	m_pcPathFile2 = new m4char_t[strlen(ai_pccFile2) + 1];
	strcpy(m_pcPathFile2 ,ai_pccFile2);

	m_poLastLConn = NULL;
	m_pcLastUser = NULL;

	m_iActualPos = 0;
	m_iMaxPos = ai_iSize * 524288;//     *1048576/2 (in MB)
	m_iTotSize = 0;

	m_psetIncDbgDetail = new SetIncDbgDetail;
	m_psetTypeToTrace = new SetTypeToTrace;
	m_iMaskTypeToTrace = M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED;
}


ClLDBInspector::~ClLDBInspector()
{
	if (m_pcPathFile1){
		delete m_pcPathFile1;
		m_pcPathFile1 = NULL;
	}

	if (m_pcPathFile2){
		delete m_pcPathFile2;
		m_pcPathFile2 = NULL;
	}

	if (m_pFile2){
		fclose (m_pFile2);
		m_pFile2=0;
	}

	if (m_pFile1){
		fclose (m_pFile1);
		m_pFile1 = NULL;
	}

	if (m_pcLastUser){
		delete m_pcLastUser;
		m_pcLastUser = NULL;
	}

	//Detalle de trazas en ldbinsp.
	if (m_psetIncDbgDetail) {
		m_psetIncDbgDetail->erase (m_psetIncDbgDetail->begin (), m_psetIncDbgDetail->end());
		delete m_psetIncDbgDetail;
	}
	m_psetIncDbgDetail = NULL;

	if (m_psetTypeToTrace) {
		m_psetTypeToTrace->erase (m_psetTypeToTrace->begin (), m_psetTypeToTrace->end());
		delete m_psetTypeToTrace;
	}
	m_psetTypeToTrace = NULL;

	m_iMaskTypeToTrace = M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED;
}


//Detalle de trazas en ldbinsp.
	//Escenario:	
	//Para cada volcado podemos establecer qué nivel de detalle tiene asociado el volcado.
	//Para un nivel de detalle dado, tendremos que comprobar el nivel de detalle que viene en el registro.
	//El registro nos indica qué permite que salgan.
	//SystemDebugEnable		SystemDebugDetailLevel
	//		0					0							No mostramos informacion
	//		1					0							Traza de datos
	//		2					0							Traza de datos y metadatos
	//		0					<> 0						No mostramos informacion
	//		1					<> 0						Traza de datos si nivel de detalle
	//		2					<> 0						Traza de datos y metadatos si nivel de detalle.

	//IMPORTANTE!!: Si en el registro ponemos 0, me llegará FFFF en el GetSystemDebugDetailLevel().
	//Si en el registro = 0x0004, me llegará 4, estamos indicando que queremos volcar mensajes del grupo 4,
	//al hacer argumento and registro si es 0 no sacaremos en ldbinsp.
	//Se trata de poner una comprobacion previa, de forma que compruebe el nivel de detalle.

//Para no modificar todo el codigo.
//Si queremos detalle, invocaremos a una nueva macro que llamara a las funciones Detail, invocara a la privada.
//La macro antigua no se modifica, se invocara a una nueva funcion privada.
//La privada pasa a tener el código de la antigua.
//Para cada funcion original se crean dos, una privada (con el código de la antigua) y una de detalle.
//Tambien modificamos la antigua que dependiendo de si estamos en detalle se invocara a la privada.


//Detalle de trazas fichero.
m4bool_t ClLDBInspector::_IsReadyToDumpDebugInfo (ClLConn *ai_poLConn)
{
	//Detalle de trazas en ldbinsp.
	//Si el tipo de detalle NO ES DE FICHERO, miramos si hay otro tipo de detalle.
	//El motivo es que si es de fichero tendremos que comprobar el fichero
	if (_IsDebuggingFromFileDebugDetail(ai_poLConn) == M4_FALSE) {

		//Si estamos aqui es pq no es detalle de fichero, pero puede ser de otro tipo
		//En ese caso si estamos en detalle no volcamos nada, lo debe volcar detalle.
		if ( (ai_poLConn->GetSystemDebugDetailLevel() & ~M4LDB_DETAIL_LEVEL_SHOW_FILE_CONFIGURABLE)!= M4LDB_DETAIL_LEVEL_SHOW_NONE) {
				//Hay detalle de otro tipo, ya lo imprimira la funcion detail!!.
					return M4_FALSE;
		}
		//Si estamos aqui es pq no hay detalle de fichero ni de nada, es el caso típico!!
	}
	//Si el detalle es de fichero...
	else {

		//-----------------------------------------------
		//... comprobamos que SOLO es de fichero!!.
		if ( (ai_poLConn->GetSystemDebugDetailLevel() & ~M4LDB_DETAIL_LEVEL_SHOW_FILE_CONFIGURABLE)!= M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			//Hay detalle de otro tipo, ya lo imprimira la funcion detail!!.
				return M4_FALSE;
		}

		//Es de SOLO fichero, por lo tanto tengo que desencadenar la búsqueda. Si no se encuentra nos vamos.
		//Si se encuentra tendremos que mostrar la informacion.
		else {
			if (_FindObjectDebugDetail (ai_poLConn) == M4_FALSE)	{
				return M4_FALSE;
			}
			else {
				return M4_TRUE;
			}
		}
		//-----------------------------------------------
	}

	return M4_TRUE;
}


//Si no hay detalle de fichero retornamos true.
m4bool_t ClLDBInspector::_FindObjectDebugDetail (ClLConn *ai_poLConn)
{
	//Hay un orden de prioridad: Role, Usuario, M4O, Nodo!!.

	//Indica si hay que buscar en alguna lista.	
	m4bool_t bFind = M4_FALSE;

	//Si en el registro indicamos que hay que buscar en el fichero
	if ( (ai_poLConn) 
		&& ((M4LDB_DETAIL_LEVEL_SHOW_FILE_CONFIGURABLE & ai_poLConn->GetSystemDebugDetailLevel()) != M4LDB_DETAIL_LEVEL_SHOW_NONE) ) 
	{

		bFind = M4_TRUE;

		//Si hay el detalle buscamos en su lista.	
		if (_FindObjectInListDebugDetail (ai_poLConn) == M4_TRUE) {

			//Si se encuentra nos vamos retornando que se ha encontrado. 
			return M4_TRUE;
		}
		//Si estamos aqui es pq el flag está para buscar pero el objeto no se ha encontrado
		//en la lista, pero puede que tengamos que buscar en otras listas
	}
	//---------

	//Si había que buscar y llegamos a este punto es pq no se encontro en ninguna de las listas,
	//por lo tanto retornamos false.
	if (bFind = M4_TRUE) {
		return M4_FALSE;
	}

	return M4_TRUE;
}


//Si no lo encuentra en la lista devolvemos falso.
m4bool_t ClLDBInspector::_FindObjectInListDebugDetail (ClLConn *ai_poLConn)
{
	string sIdObjectToTrace ("");
	string sTypeToTrace ("");
	m4bool_t	bReadyToFind = M4_FALSE;

	m4uint8_t	iTypeToTrace = M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED;
	
	//---------------------------------------------------------------------
	//Leemos el fichero. Rellenamos el set con la informacion del fichero.
	_ReadFromFileDebugDetail();

	//Tendremos relleno los tipos de traza que se han leido del fichero y sus objetos.
	//En la mascara tenemos qué tipo de trazas hay que tener en cuenta.
	//---------------------------------------------------------------------

	//-----------------------------------
	//Si hay mascara de ROLE y todavia no sabemos si hay que buscar en otros sitio
	iTypeToTrace = 1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_ROLE;
	if ( ((m_iMaskTypeToTrace & iTypeToTrace) != M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED) 
		&& (bReadyToFind == M4_FALSE))
		{			
			if (ai_poLConn->GetIdRoleTrace()) {				
				sTypeToTrace.append(g_sMaskTypeToTrace[M4LDB_OBJECT_TYPE_IN_TRACE_FILE_ROLE]);
				sIdObjectToTrace.append(ai_poLConn->GetIdRoleTrace());

				//Si se encuentra en el set ponemos a true la variable retorno.
				//Si no se encuentra no indicamos nada.
				if (_FindInSetInclusionDebugDetail (sTypeToTrace, sIdObjectToTrace) == M4_TRUE) {
					bReadyToFind = M4_TRUE;
				}
				//Si no se encuentra, tenemos que limpiar cadenas.
				else {
					sTypeToTrace.erase (sTypeToTrace.begin(), sTypeToTrace.end());
					sIdObjectToTrace.erase (sIdObjectToTrace.begin(), sIdObjectToTrace.end());
				}
			}
		}
	//-----------------------------------

	//-----------------------------------
	//Si hay mascara de USUARIO y todavia no sabemos si hay que buscar en otros sitio
	iTypeToTrace = 1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_USER;
	if ( ((m_iMaskTypeToTrace & iTypeToTrace) != M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED) 
		&& (bReadyToFind == M4_FALSE))
		{			
			if (ai_poLConn->GetIdUserTrace()) {
				sTypeToTrace.append(g_sMaskTypeToTrace[M4LDB_OBJECT_TYPE_IN_TRACE_FILE_USER]);
				sIdObjectToTrace.append(ai_poLConn->GetIdUserTrace());				

				//Si se encuentra en el set ponemos a true la variable retorno.
				//Si no se encuentra no indicamos nada.
				if (_FindInSetInclusionDebugDetail (sTypeToTrace, sIdObjectToTrace) == M4_TRUE) {
					bReadyToFind = M4_TRUE;
				}
				//Si no se encuentra, tenemos que limpiar cadenas.
				else {
					sTypeToTrace.erase (sTypeToTrace.begin(), sTypeToTrace.end());
					sIdObjectToTrace.erase (sIdObjectToTrace.begin(), sIdObjectToTrace.end());
				}
			}
		}
	//-----------------------------------


	//-----------------------------------
	//Si hay mascara de M40 y todavia no sabemos si hay que buscar en otros sitio
	iTypeToTrace = 1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_M4O;
	if ( ((m_iMaskTypeToTrace & iTypeToTrace) != M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED) 
		&& (bReadyToFind == M4_FALSE))
		{			
			if (ai_poLConn->GetIdT3Trace()) {
				sTypeToTrace.append(g_sMaskTypeToTrace[M4LDB_OBJECT_TYPE_IN_TRACE_FILE_M4O]);
				sIdObjectToTrace.append(ai_poLConn->GetIdT3Trace());

				//Si se encuentra en el set ponemos a true la variable retorno.
				//Si no se encuentra no indicamos nada.
				if (_FindInSetInclusionDebugDetail (sTypeToTrace, sIdObjectToTrace) == M4_TRUE) {
					bReadyToFind = M4_TRUE;
				}
				//Si no se encuentra, tenemos que limpiar cadenas.
				else {
					sTypeToTrace.erase (sTypeToTrace.begin(), sTypeToTrace.end());
					sIdObjectToTrace.erase (sIdObjectToTrace.begin(), sIdObjectToTrace.end());
				}
			}
		}
	//-----------------------------------

	//-----------------------------------
	//Si hay mascara de NODO y todavia no sabemos si hay que buscar en otros sitio
	iTypeToTrace = 1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_NODE;
	if ( ((m_iMaskTypeToTrace & iTypeToTrace) != M4LDB_OBJECT_TYPE_IN_TRACE_FILE_UNDEFINED) 
		&& (bReadyToFind == M4_FALSE))
		{			
			if (ai_poLConn->GetIdNodeTrace()) {
				sTypeToTrace.append(g_sMaskTypeToTrace[M4LDB_OBJECT_TYPE_IN_TRACE_FILE_NODE]);
				sIdObjectToTrace.append(ai_poLConn->GetIdNodeTrace());

				//Si se encuentra en el set ponemos a true la variable retorno.
				//Si no se encuentra no indicamos nada.
				if (_FindInSetInclusionDebugDetail (sTypeToTrace, sIdObjectToTrace) == M4_TRUE) {
					bReadyToFind = M4_TRUE;
				}
				//Si no se encuentra, tenemos que limpiar cadenas.
				else {
					sTypeToTrace.erase (sTypeToTrace.begin(), sTypeToTrace.end());
					sIdObjectToTrace.erase (sIdObjectToTrace.begin(), sIdObjectToTrace.end());
				}
			}
		}
	//-----------------------------------

	return bReadyToFind;
}


m4bool_t ClLDBInspector::_FindInSetInclusionDebugDetail (string ai_sTypeToTrace, string ai_sIdObjectToTrace)
{
	string sToFind ("");
	itSetIncDbgDetail	itSet;

	//En el mapa se mete la concatenacion de ambos, separados por punto y coma.
	sToFind.append(ai_sTypeToTrace);
	sToFind.append(";");
	sToFind.append(ai_sIdObjectToTrace);

	if (m_psetIncDbgDetail) {
		itSet = m_psetIncDbgDetail->find (sToFind);
		//Si lo encuentra devolvemos true
		if (itSet != m_psetIncDbgDetail->end()) {
			return M4_TRUE;	
		}
	}

	return M4_FALSE;
}


void ClLDBInspector::_ReadFromFileDebugDetail (void)
{
	static  m4bool_t		bLoaded = M4_FALSE;
	static	ClMutex			s_oMutex( M4_TRUE ) ;

	s_oMutex.Lock() ;

	if( bLoaded == M4_FALSE) 
	{
		bLoaded = M4_TRUE;

		string	s = ClVMDebugEnv::getInstance()->getDebugDir();		
		s = s + M4_BACK_SLASH + "ldbdetail.txt" ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		FILE	*fInput = M4Fopen( s.c_str(), M4UniRead, eEncoding ) ;

		if( fInput != NULL )
		{	
			char	acLine [M4LDB_SIZE_DEFAULT_STRING + 1];
			//Los separadores en el fichero, espacio, tabulador.
			char seps[]   = " \t\n";
			char *token;

			char	pTypeToTrace [M4LDB_SIZE_DEFAULT_STRING + 1];
			char	pIdObjectToTrace [M4LDB_SIZE_DEFAULT_STRING + 1];			

			for (;;) {
				if (fgets( acLine, M4LDB_SIZE_DEFAULT_STRING + 1, fInput ) == NULL) {
					break;		
				}

				//Formato (1 espacio o tabulador entre ellos):
				//TIPO ID				
				token = strtok( acLine, seps );

				//Obtengo el tipo
				if (token != NULL ) {
					//Para permitir comentarios en el fichero.
					if (*token ==  ';') {
						continue;
					}
					strcpy(pTypeToTrace, token);
					token = strtok( NULL, seps );
					//Obtengo el id
					if (token != NULL ) {
						strcpy(pIdObjectToTrace, token);
					}
					//Falta el ID
					else {
						continue;
					}
				}
				else {
					continue;
				}

				//La estructura es primero leemos el tipo (Nodo, M4O, User, Role)
				string sTypeToTrace ("");
				//Despues leemos el Id del objeto.
				string sIdObjectToTrace ("");

				// UNICODE LOCALE
				sTypeToTrace.append(M4StrUpr(pTypeToTrace));
				sIdObjectToTrace.append(M4StrUpr(pIdObjectToTrace));

				//Actualizamos la mascara de tipos de traza de ficheros que hay que tener en cuenta.
				//Si no corresponde con niguna de los posibles tipos de traza seguimos leyendo lineas.
				if (_TranslateMaskTypeToTrace (sTypeToTrace) == M4_ERROR) {
					continue;
				}

				//Lo metemos en el mapa.
				if (m_psetIncDbgDetail) {
					string sToFind("");
					sToFind.append(sTypeToTrace);
					sToFind.append(";");
					sToFind.append(sIdObjectToTrace);
					m_psetIncDbgDetail->insert (sToFind);
				}
			}

			fclose( fInput ) ;
		}
	}
	
	s_oMutex.Unlock() ;
}


m4return_t ClLDBInspector::_TranslateMaskTypeToTrace (string ai_sMaskTypeToTrace)
{
	if (ai_sMaskTypeToTrace == g_sMaskTypeToTrace [M4LDB_OBJECT_TYPE_IN_TRACE_FILE_ROLE]) {
		m_iMaskTypeToTrace |= (1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_ROLE);
		return M4_SUCCESS;
	}

	if (ai_sMaskTypeToTrace == g_sMaskTypeToTrace [M4LDB_OBJECT_TYPE_IN_TRACE_FILE_USER]) {
		m_iMaskTypeToTrace |= (1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_USER);
		return M4_SUCCESS;
	}

	if (ai_sMaskTypeToTrace == g_sMaskTypeToTrace [M4LDB_OBJECT_TYPE_IN_TRACE_FILE_M4O]) {
		m_iMaskTypeToTrace |= (1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_M4O);
		return M4_SUCCESS;
	}

	if (ai_sMaskTypeToTrace == g_sMaskTypeToTrace [M4LDB_OBJECT_TYPE_IN_TRACE_FILE_NODE]) {
		m_iMaskTypeToTrace |= (1 << M4LDB_OBJECT_TYPE_IN_TRACE_FILE_NODE);
		return M4_SUCCESS;
	}

	//El tipo de traza que se ha puesto en el fichero no corresponde con ninguna de las validas (M4O,...)
	return M4_ERROR;
}


//-----------------------------------------
//Funcion 1
//-----------------------------------------
//Funcion detalle
void ClLDBInspector::PrintFileDebugDetailDate (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...)
{
	//Detalle de trazas en ldbinsp.
	if (ai_poLConn) {

		//Si no hay detalle de ningun tipo nos vamos.
		if (ai_poLConn->GetSystemDebugDetailLevel() == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
		
		//Si el detalle es de fichero no volcamos nada!!. En todo caso lo volcara la de toda la vida.
		if (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_TRUE) {
			return;
		}

		//Si el detalle es diferente de fichero comprobamos si hay que imprimir.		
		if ((ai_iSystemDebugDetailLevel & ai_poLConn->GetSystemDebugDetailLevel()) == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
	}

	va_list vaListVariableArguments;
	va_start (vaListVariableArguments, ai_pFormat);

	_PrintFileDebug (true, ai_poLConn, ai_pFormat, vaListVariableArguments);
	
	va_end (vaListVariableArguments);
}


void ClLDBInspector::PrintFileDebugDetail (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...)
{
	//Detalle de trazas en ldbinsp.
	if (ai_poLConn) {

		//Si no hay detalle de ningun tipo nos vamos.
		if (ai_poLConn->GetSystemDebugDetailLevel() == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
		
		//Si el detalle es de fichero no volcamos nada!!. En todo caso lo volcara la de toda la vida.
		if (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_TRUE) {
			return;
		}

		//Si el detalle es diferente de fichero comprobamos si hay que imprimir.		
		if ((ai_iSystemDebugDetailLevel & ai_poLConn->GetSystemDebugDetailLevel()) == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
	}

	va_list vaListVariableArguments;
	va_start (vaListVariableArguments, ai_pFormat);

	_PrintFileDebug (false, ai_poLConn, ai_pFormat, vaListVariableArguments);
	
	va_end (vaListVariableArguments);
}


void ClLDBInspector::PrintFileDebugDate (ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...)
{
	//Detalle de trazas en ldbinsp.
	//Si el detalle es de fichero puede ser que tengamos que volcar!!.

	if (ai_poLConn) {		

		//Escenarios:
		//Si hay detalle (!= none) tendremos que comprobar más cosas para ver si realmente tenemos que volcar algo.
		//Si no hay detalle en el registro, debemos comportarnos como siempre, es el caso típico.
		if (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) {

			//Ojo: Si hay detalle pero no es de fichero salimos.
			//Si es de fichero tendremos que hacer más comprobaciones (ver si realmente debo volcar).
			if ( (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) 
				&& (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_FALSE) ){
				return;
			}

			//Se comprueba si la funcion print de toda la vida efectivamente tiene que volcar!!.
			if (_IsReadyToDumpDebugInfo (ai_poLConn) == M4_FALSE) {
				return;
			}
		}
	}

	va_list list;
	va_start (list, ai_pFormat);

	_PrintFileDebug (true, ai_poLConn, ai_pFormat, list);
	va_end (list);
}


void ClLDBInspector::PrintFileDebug (ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, ...)
{
	//Detalle de trazas en ldbinsp.
	//Si el detalle es de fichero puede ser que tengamos que volcar!!.

	if (ai_poLConn)
	{		
		//Escenarios:
		//Si hay detalle (!= none) tendremos que comprobar más cosas para ver si realmente tenemos que volcar algo.
		//Si no hay detalle en el registro, debemos comportarnos como siempre, es el caso típico.
		if (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) {

			//Ojo: Si hay detalle pero no es de fichero salimos.
			//Si es de fichero tendremos que hacer más comprobaciones (ver si realmente debo volcar).
			if ( (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) 
				&& (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_FALSE) ){
				return;
			}

			//Se comprueba si la funcion print de toda la vida efectivamente tiene que volcar!!.
			if (_IsReadyToDumpDebugInfo (ai_poLConn) == M4_FALSE) {
				return;
			}
		}
	}

	va_list list;
	va_start (list, ai_pFormat);

	_PrintFileDebug (false, ai_poLConn, ai_pFormat, list);
	va_end (list);
}


//Privada
void ClLDBInspector::_PrintFileDebug (m4bool_t ai_bWithDate, ClLConn *ai_poLConn, m4pcchar_t ai_pFormat, va_list ai_vaListVariableArguments)
{
	if( _BeginPrinting( ai_poLConn ) == M4_FALSE )
	{
		return ;
	}

	M4_LDB_PRINT_IN_LDB_INSP( vfprintf( m_pFile, ai_pFormat, ai_vaListVariableArguments ) ) ;

	if( ai_bWithDate == M4_TRUE )
	{
		// Si se pide se añade la fecha
		m4char_t acBuffer[ M4_SECOND_END + 1 ] ;
		ClActualDate( acBuffer, M4_SECOND_END + 1 ) ;
		M4_LDB_PRINT_IN_LDB_INSP( fprintf( m_pFile, " Date = %s. Tick = %lld.", acBuffer, ClPrecisionTicker::GetTimeTick() ) ) ;
	}

	_EndPrinting() ;
}


void ClLDBInspector::PrintFileDebugDetail (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData)
{
	//Detalle de trazas en ldbinsp.
	if (ai_poLConn)
	{
		//Si no hay detalle de ningun tipo nos vamos.
		if (ai_poLConn->GetSystemDebugDetailLevel() == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
		
		//Si el detalle es de fichero no volcamos nada!!. En todo caso lo volcara la de toda la vida.
		if (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_TRUE) {
			return;
		}

		//Si el detalle es diferente de fichero comprobamos si hay que imprimir.		
		if ((ai_iSystemDebugDetailLevel & ai_poLConn->GetSystemDebugDetailLevel()) == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
	}

	_PrintFileDebug (ai_poLConn, ai_pRecordSet,ao_iNumData, ai_iFirstData);
}


void ClLDBInspector::PrintFileDebug (ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData)
{
	//Detalle de trazas en ldbinsp.
	//Si el detalle es de fichero puede ser que tengamos que volcar!!.

	if (ai_poLConn)
	{		
		//Escenarios:
		//Si hay detalle (!= none) tendremos que comprobar más cosas para ver si realmente tenemos que volcar algo.
		//Si no hay detalle en el registro, debemos comportarnos como siempre, es el caso típico.
		if (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) {

			//Ojo: Si hay detalle pero no es de fichero salimos.
			//Si es de fichero tendremos que hacer más comprobaciones (ver si realmente debo volcar).
			if ( (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) 
				&& (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_FALSE) ){
				return;
			}

			//Se comprueba si la funcion print de toda la vida efectivamente tiene que volcar!!.
			if (_IsReadyToDumpDebugInfo (ai_poLConn) == M4_FALSE) {
				return;
			}
		}
	}
	
	_PrintFileDebug (ai_poLConn, ai_pRecordSet,ao_iNumData, ai_iFirstData);
}


//Privada
void ClLDBInspector::_PrintFileDebug (ClLConn *ai_poLConn, ClRecordSet *ai_pRecordSet, m4int32_t *ao_iNumData, m4int32_t ai_iFirstData)
{
	if( _BeginPrinting( ai_poLConn ) == M4_FALSE )
	{
		return ;
	}

	m4date_t dDate;
	m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];
	ClRegisterIndex indReg(0);
	ClRegister *pReg;
	ClItemIndex indItem(0);
	ClItem *pItem;

	m4uint32_t numItem = ai_pRecordSet->GetpNodeDef ()->NumberOfItems ();

	m4uint32_t First = 0, Last;

    if (ao_iNumData) {
        First = ai_iFirstData;
        Last = First + (*ao_iNumData);
		M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "\n  Num Items = %ld. Num Registers = %ld.", numItem, (*ao_iNumData)));
    }
    else {
        Last = ai_pRecordSet->Count();
		M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "\n  Num Items = %ld. Num Registers = %ld.", numItem, Last));
    }

    pReg = &ai_pRecordSet->Register[(ClRegisterIndex) First];

	M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"\n  "));

	for (indItem=0; indItem<numItem; ++indItem) {
		if (pReg->Item[indItem].ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {						
			M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"#--------------------#"));
		}
	}

	M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"\n  "));

	for (indItem=0; indItem<numItem; ++indItem) {
		if (pReg->Item[indItem].ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {						
			M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "#%-20.20s#", pReg->Item[indItem].ItemDef.Id ()));
		}
	}

	M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"\n  "));

	for (indItem=0; indItem<numItem; ++indItem) {
		if (pReg->Item[indItem].ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {						
			M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"#--------------------#"));
		}
	}

	for (indReg=First; indReg<Last; ++indReg)
	{
		M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"\n  "));

		for (indItem=0; indItem<numItem; ++indItem) {

			pItem = &ai_pRecordSet->Register[indReg].Item[indItem];

			if (pItem->ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {

				m4int32_t cppType = pItem->ItemDef.CppType ();

				void *pValue = pItem->Value.GetWithOutSec();

				if (pValue) {
					switch (cppType) {

						case M4CL_CPP_TYPE_DATE:
							dDate = M4LDB_DOUBLE (pValue);
							ClDateToString (dDate, aDate, M4LDB_SIZE_TIMESTAMP + 1);
							M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "|%-20.20s|", aDate));
							break;

						case M4CL_CPP_TYPE_NUMBER:
							M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "|%-20.20lG|", M4LDB_DOUBLE (pValue)));
							break;

						case M4CL_CPP_TYPE_STRING:
							M4_LDB_PRINT_IN_LDB_INSP(fprintf (m_pFile, "|%-20.20s|", pValue));
							break;
					}
				}
				else {
					M4_LDB_PRINT_IN_LDB_INSP(fprintf(m_pFile,"|       <null>       |"));
				}
			}
		}
	}

	_EndPrinting() ;
}


void ClLDBInspector::PrintFileDebugDetail (m4uint32_t ai_iSystemDebugDetailLevel, ClLConn *ai_poLConn, ClCursor *ai_pCursor)
{
	//Detalle de trazas en ldbinsp.
	if (ai_poLConn)
	{
		//Si no hay detalle de ningun tipo nos vamos.
		if (ai_poLConn->GetSystemDebugDetailLevel() == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}

		//Si el detalle es de fichero no volcamos nada!!. En todo caso lo volcara la de toda la vida.
		if (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_TRUE) {
			return;
		}

		//Si el detalle es diferente de fichero comprobamos si hay que imprimir.		
		if ((ai_iSystemDebugDetailLevel & ai_poLConn->GetSystemDebugDetailLevel()) == M4LDB_DETAIL_LEVEL_SHOW_NONE) {
			return;
		}
	}

	_PrintFileDebug (ai_poLConn, ai_pCursor);
}


void ClLDBInspector::PrintFileDebug (ClLConn *ai_poLConn, ClCursor *ai_pCursor)
{
	//Detalle de trazas en ldbinsp.
	//Si el detalle es de fichero puede ser que tengamos que volcar!!.

	if (ai_poLConn)
	{		
		//Escenarios:
		//Si hay detalle (!= none) tendremos que comprobar más cosas para ver si realmente tenemos que volcar algo.
		//Si no hay detalle en el registro, debemos comportarnos como siempre, es el caso típico.
		if (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) {

			//Ojo: Si hay detalle pero no es de fichero salimos.
			//Si es de fichero tendremos que hacer más comprobaciones (ver si realmente debo volcar).
			if ( (ai_poLConn->GetSystemDebugDetailLevel() != M4LDB_DETAIL_LEVEL_SHOW_NONE) 
				&& (_IsDebuggingFromFileDebugDetail (ai_poLConn) == M4_FALSE) ){
				return;
			}

			//Se comprueba si la funcion print de toda la vida efectivamente tiene que volcar!!.
			if (_IsReadyToDumpDebugInfo (ai_poLConn) == M4_FALSE) {
				return;
			}
		}
	}

	_PrintFileDebug (ai_poLConn, ai_pCursor);
}


//Privada
void ClLDBInspector::_PrintFileDebug (ClLConn *ai_poLConn, ClCursor *ai_pCursor)
{
	if( _BeginPrinting( ai_poLConn ) == M4_FALSE )
	{
		return ;
	}

	string stSQL( M4LDB_EMPTY ) ;

	if( !ai_poLConn->m_pLdb->GetShowData() )
	{
		m4pcchar_t pSQL = NULL ;
		ai_pCursor->GetStmt( pSQL ) ;
		stSQL = pSQL ;
	}
	else
	{
		ai_pCursor->GetStmt( stSQL ) ;
	}

	M4_LDB_PRINT_IN_LDB_INSP( fprintf( m_pFile, " Stmt = %s", stSQL.c_str () ) ) ;

	_EndPrinting() ;
}


void ClLDBInspector::_PrintFileInfo (ClLConn *ai_poLConn, m4pcchar_t ai_pLabel, m4int32_t ai_lErrorCode, m4pcchar_t ai_pMessage)
{
	if( ai_poLConn != NULL && !ai_poLConn->m_pLdb->GetSysDebugLevel() )
	{
		return ;
	}

	if( _BeginPrinting( ai_poLConn ) == M4_FALSE )
	{
		return ;
	}

	M4_LDB_PRINT_IN_LDB_INSP( fprintf( m_pFile, "\n ***** %s ***** %d :\n%s\n", ai_pLabel, ai_lErrorCode, ai_pMessage ) ) ;

	_EndPrinting() ;
}


m4bool_t	ClLDBInspector::_BeginPrinting(ClLConn *ai_poLConn)
{

	m4bool_t		bPrint = M4_FALSE ;
	m4pcchar_t		pccUser = NULL ;
	ClLogonAdaptor	*poLogonAdaptor = NULL ;


	if( ai_poLConn == NULL )
	{
		return( M4_FALSE );
	}

	if( ai_poLConn->m_pLogAccess != NULL )
	{
		// Los canales de la licencia no vuelcan traza. El de auditoria de acciones de usuario tampoco.
		m4id_t pccChannel = ai_poLConn->m_pLogAccess->GetpCMCR()->GetChannelId() ;

		if( pccChannel != NULL && ( strcmpi( pccChannel, "SDC_MT_LICENSE" ) == 0 || strcmpi( pccChannel, "M4SYS_LICENSE" ) == 0 || strcmpi( pccChannel, "SSC_USER_ACTIONS_AUDITORY" ) == 0 ) )
		{
			return( M4_FALSE );
		}
	}

	// Bug 0310208 El bloqueo debe ser antes de las comprobaciones que cambian el estado del objeto
	if( m_lReferences > 1 && m_pContext->m_poMutexDebug != NULL )
	{
		m_pContext->m_poMutexDebug->Lock() ;
	}

	if( ai_poLConn != m_poLastLConn )
	{
		bPrint = M4_TRUE ;
		m_poLastLConn = ai_poLConn ;
	}

	poLogonAdaptor = ai_poLConn->m_pLogonAdaptor ;

	if( poLogonAdaptor == NULL )
	{
		if( ai_poLConn->m_pLogAccess != NULL )
		{
			ClChannel *poChannel = ai_poLConn->m_pLogAccess->GetpChannel() ;

			if( poChannel != NULL )
			{
				ClChannelManager *poChannelManager = poChannel->GetpChannelManager() ;

				if( poChannelManager != NULL )
				{
					poLogonAdaptor = poChannelManager->GetLogonAdaptor() ;
				}
			}
		}
	}

	if( poLogonAdaptor != NULL )
	{
		pccUser = poLogonAdaptor->GetUsername() ;
	}

	if( pccUser == NULL )
	{
		pccUser = "" ;
	}

	if( m_pcLastUser == NULL || strcmp( m_pcLastUser, pccUser ) != 0 )
	{
		bPrint = M4_TRUE ;

		if( m_pcLastUser != NULL )
		{
			delete m_pcLastUser ;
			m_pcLastUser = NULL ;
		}

		m_pcLastUser = new m4char_t[ strlen( pccUser ) + 1 ] ;

		if( m_pcLastUser != NULL )
		{
			strcpy( m_pcLastUser, pccUser ) ;
		}
	}

	if( bPrint == M4_TRUE )
	{
		M4_LDB_PRINT_IN_LDB_INSP( fprintf( m_pFile, "\n\n>>>C%ld<<< User : %s\n", ai_poLConn->GetIdLConn () + 1, pccUser ) ) ;
	}

	return( M4_TRUE );
}


void  ClLDBInspector::_EndPrinting(void)
{
	if( m_pFile != NULL )
	{
		fflush( m_pFile ) ;
	}

	if( m_lReferences > 1 && m_pContext->m_poMutexDebug != NULL )
	{
		m_pContext->m_poMutexDebug->Unlock() ;
	}
}


m4return_t  ClLDBInspector::_CreateLDBFile(void)
{
	if( m_bInit == M4_FALSE )
	{
		m_bInit = M4_TRUE ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		m_pFile = m_pFile1 = M4Fopen( m_pcPathFile1, M4UniWrite, eEncoding ) ;

		if( m_pFile != NULL )
		{
			m_iActualPos += fprintf( m_pFile, "\n\t#----------------------------------------------#" ) ;
			m_iActualPos += fprintf( m_pFile, "\n\t| Welcome to Logical Database V.%13s  |", GetM2VersionString() ) ;
			m_iActualPos += fprintf( m_pFile, "\n\t#----------------------------------------------#" ) ;
		}
	}

	return( m_pFile != NULL ) ;
}


void  ClLDBInspector::_ChangeLDBInspector(void)
{
	if (m_pcPathFile2)
	{
		m4pchar_t pTemp;
		
		// Si mantenemos dos ficheros.
		FILE * tmpfileused;
		FILE * tmpfiletouse;

		if (m_pFile2)
		{
			tmpfileused = m_pFile2;
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniNative ;
			tmpfiletouse = m_pFile1 = M4Fopen (m_pcPathFile1, M4UniWrite, eEncoding);

			if (!m_pFile1) {
				// No hemos podido abrirlo, asi que seguimos con el mismo fichero.
				rewind (m_pFile);
				m_iTotSize+=m_iActualPos;
				m_iActualPos = 0;
				return;
			}

			pTemp = m_pcPathFile1;
			m_pFile2 = 0;
		}
		else 
		{
			tmpfileused = m_pFile1;
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniNative ;
			tmpfiletouse = m_pFile2 = M4Fopen (m_pcPathFile2, M4UniWrite, eEncoding);

			if (!m_pFile2) {
				// No hemos podido abrirlo, asi que seguimos con el mismo fichero.
				rewind (m_pFile);
				m_iTotSize+=m_iActualPos;
				m_iActualPos = 0;
				return;
			}

			pTemp = m_pcPathFile2;
			m_pFile1 = 0;
		}

		m_iTotSize+=m_iActualPos/1024;
		m_iActualPos = 0;

		if (tmpfileused){
			fprintf(tmpfileused,"\nFile Swapped.\nAcumulatte Size : %ld Kbytes ( %ld MB)\n",m_iTotSize,m_iTotSize/1024);
			fprintf(tmpfileused,"Swapping to : %s",pTemp);
			fflush(tmpfileused);
			fclose(tmpfileused);
		}

		if (tmpfiletouse){
			fprintf (tmpfiletouse,"\nFile Swapped.\nAcumulatte Size : %ld Kbytes ( %ld MB )\nNext Swap at : %ld Kbytes(%ld MB)\n",m_iTotSize,m_iTotSize/1024,m_iTotSize + m_iMaxPos/1024,(m_iTotSize + m_iMaxPos/1024)/1024 );//in MB

			if (m_poLastLConn != NULL) {
				string sLicenseSummary("");
				if (m_poLastLConn->GetLicenseSummaryEncrypted(sLicenseSummary) == M4_TRUE) {
					if (sLicenseSummary.size() > 0) {
						fprintf(tmpfiletouse, "\nWatermark. %s\n", sLicenseSummary.c_str());					
					}
				}

				if( m_pcLastUser != NULL && *m_pcLastUser != '\0' )
				{
					fprintf( tmpfiletouse, "\n>>>C%ld<<< User : %s\n\n", m_poLastLConn->GetIdLConn () + 1, m_pcLastUser ) ;
				}
			}
			fflush(tmpfiletouse);
		}

		m_pFile = tmpfiletouse;

	}// m_pFile2, seguimos escribiendo en el mismo.
}


m4bool_t	ClLDBInspector::_IsDebuggingFromFileDebugDetail (ClLConn *ai_poLConn)
{
	if( ai_poLConn != NULL && (ai_poLConn->GetSystemDebugDetailLevel() & M4LDB_DETAIL_LEVEL_SHOW_FILE_CONFIGURABLE) != M4LDB_DETAIL_LEVEL_SHOW_NONE )
	{
		return M4_TRUE;
	}
	return M4_FALSE;
}	

#endif

