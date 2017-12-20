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
//    implementacion de la clase que implementa las funciones de cadenas
////
//==============================================================================

#include "vm0str.hpp"
#include "vmres.hpp"
#include "clstringpool.hpp"
#include "medvarin.hpp"
#include "m4win2ux.hpp"
#include "m4mdrt.hpp"
#include "clregexp.hpp"
#include "m4sysalloc.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"



//---------------------------------------------------------
//Destructor
//---------------------------------------------------------

ClVM0String::~ClVM0String(){
	delete m_poRegex;
}

//---------------------------------------------------------
//-M I D$ (troceauna cadena)
//---------------------------------------------------------
m4return_t  ClVM0String::Mid             (void){
    
    m4VariantType vHasta, vDesde, vCad;
    m4return_t  ret=M4_SUCCESS;
    m4int32_t  iLon;
    m4char_t *pszTemp, *pszOrig;
	size_t	iDesde = 0 ;
	size_t	iHasta = 0 ;
	size_t	iSize = 0 ;
	int		iPosition = 0 ;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Mid");
#endif
    //tercer arg- hasta donde
    ret=RuntimePop(vHasta);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Mid.")	;
    
    //(seg. arg)-. desde donde
    ret=RuntimePop(vDesde);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Mid.")	;
    
    //1er arg - Valor a cortar
    ret=RuntimePop(vCad);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Mid.")	;
	
	if ( !CheckStringIfErrorPush (vCad, 0.0) )   {		//Error
		DeleteVarString (vHasta);
		DeleteVarString (vDesde);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vDesde, vCad, M4_TRUE) )   {		//Error
		DeleteVarString (vHasta);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vHasta, vCad, M4_TRUE) )   {		//Error
		return M4_SUCCESS;
	}
	
	//Ok
	iLon=m_oDynamicPool->GetStringLength (vCad.Data.CharData) ;
    pszOrig= new m4char_t [iLon + 1];
	if (!pszOrig) {
		RuntimePush (vCad);
		return M4_SUCCESS;
	}
    pszTemp=pszOrig;
    //copiamos la cadena a un temporal, y la borramos del Pool
    strcpy (pszOrig , m_oDynamicPool->GetString (vCad.Data.CharData) );
    m_oDynamicPool->DeleteString (vCad.Data.CharData) ;


    //comprobaciones de rigor    
    if( vDesde.Data.DoubleData < 0 )
	{
        iDesde = 0 ;
    }
	else
	{
		// UNICODE LN4
		iPosition = M4CharPosition( pszOrig, iLon, vDesde.Data.DoubleData, iSize ) ;

		if( iPosition == -1 )
		{
			iDesde = iLon ;
		}
		else
		{
			iDesde = iPosition ;
		}
	}
		
	if( vHasta.Data.DoubleData < 0 )
	{
		iHasta = iDesde ;
	}
	else
	{
		// UNICODE LN4
		iPosition = M4CharPosition( pszOrig, iLon, vDesde.Data.DoubleData + vHasta.Data.DoubleData, iSize ) ;

		if( iPosition == -1 )
		{
			iHasta = iLon ;
		}
		else
		{
			iHasta = iPosition ;
		}
	}
			
	//cortamos la cadena 
	pszTemp[ iHasta ] = '\0' ;
	pszTemp += iDesde ;
	
	//metemos la cadena añadiendo
	RuntimePushString (pszTemp, vCad.IsUser);
	delete [] pszOrig;
	
	return M4_SUCCESS; 
}



//-------------------------------------------------------
//StrIn
//-------------------------------------------------------
m4return_t  ClVM0String::StrIn       (void){
	
    m4VariantType vTemp1, vTemp2;
    m4char_t *pszTemp;
    m4return_t  ret=M4_SUCCESS;
	m4pchar_t pcString1 = NULL ;
	m4pchar_t pcString2 = NULL ;
	int	iBytes = 0 ;
	int iChars = 0 ;

    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: StrIn");
#endif
    //cadena a buscar
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "StrIn")	;
    
    //cadena en la que buscar
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "StrIn")	;    
	
	if ( !CheckStringIfErrorPush (vTemp2, M4_ERROR) )   {	//error
        DeleteVarString (vTemp1);
		return M4_SUCCESS;
    }
	
	if ( !CheckStringIfErrorPush (vTemp1, M4_ERROR) )   {	//error
		DeleteVarString (vTemp2);
		return M4_SUCCESS;
    }

	pcString1 = m_oDynamicPool->GetString( vTemp1.Data.CharData ) ;
	pcString2 = m_oDynamicPool->GetString( vTemp2.Data.CharData ) ;
	
	//Ok   
    pszTemp = strstr( pcString2, pcString1 );

	if (pszTemp == NULL ){	// no encontrada
        RuntimePushDouble (M4_ERROR);
    }
    else{					// encontrada

		// UNICODE LN4
		iBytes = pszTemp - pcString2 ;
		iChars = M4StrLen( pcString2, iBytes ) ;

        RuntimePushDouble( iChars );
    }
	
    //borramos las cadenas
    m_oDynamicPool->DeleteString (vTemp2.Data.CharData);
    m_oDynamicPool->DeleteString (vTemp1.Data.CharData);
	
    return M4_SUCCESS;
}

//----------------
//Length
//----------------

m4return_t  ClVM0String::Length(void)
{
	
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
	m4uint32_t	iBytes =  0 ;
	m4uint32_t	iChars = 0 ;
	m4pchar_t	pccString = NULL ;

    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Length");
#endif

    //cadena a buscar
    ret=RuntimePop( vTemp ) ;
    DEBUG_CHECK_RES( ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Length" ) ;
	
	if( CheckStringIfErrorPush( vTemp, 0.0 ) )
	{
		// UNICODE LN4
		pccString = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
		iBytes =  m_oDynamicPool->GetStringLength( vTemp.Data.CharData ) ;
		iChars = M4StrLen( pccString, iBytes ) ;

		RuntimePushDouble( iChars ) ;
        m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
    }
	
    return M4_SUCCESS;
}


//Filtra (espacio, return, tab) por LEFT,RIGHT o ambos
//Notas: Defines en ClDefine.hpp 
//Entrada: String(str), Filtro(d)
//Salida:  res = str | "" | Null (segun el caso)
//		   
m4return_t  ClVM0String::Trim            (void){
    
	m4VariantType vString, vFiltro;

	/* Bug 0135168
	Los caracteres que se pasan a isspace deben ser sin signo
	*/
    m4uchar_t *pszTemp, *pszTemp1, *pszTempLast;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Trim");
#endif
    //2o.arg: Sentido del Filtro: Left(-1) o Ambos(0) o Right(1)
    ret=RuntimePop(vFiltro);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Trim")	;
	
    //1o.arg: Cadena a filtrar
    ret=RuntimePop(vString);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Trim")	;
	
	if ( !CheckStringIfErrorPush (vString, "") )   {		//Error
		DeleteVarString (vFiltro);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vFiltro, vString, M4_TRUE) )   {	//Error
		return M4_SUCCESS;
	}
    
	//Accion: TRIM_LEFT
	if (vFiltro.Data.DoubleData == M4CL_TRIM_LEFT || 
		vFiltro.Data.DoubleData == M4CL_TRIM_ALL ) {
		// find first non-space character	
		pszTemp1 = (m4puchar_t) m_oDynamicPool->GetString (vString.Data.CharData);
		pszTemp = pszTemp1;

		while (isspace(*pszTemp))
			++pszTemp;
		//Truncamos la cadena
		if (pszTemp != pszTemp1)
		{
			memmove( pszTemp1, pszTemp, strlen((m4pcchar_t)pszTemp) + 1 ) ;
		}
	}
	
	//Accion: TRIM_RIGHT
	if (vFiltro.Data.DoubleData == M4CL_TRIM_RIGHT || 
		vFiltro.Data.DoubleData == M4CL_TRIM_ALL ) {
		// find last non-space character		
		pszTemp = (m4puchar_t) m_oDynamicPool->GetString (vString.Data.CharData);
		pszTempLast= NULL;
		while (*pszTemp != '\0') {
			if (isspace(*pszTemp)) {
				if (pszTempLast == NULL)
					pszTempLast = pszTemp;
			} 
			else
				pszTempLast = NULL;
			++pszTemp;
		}
		if (pszTempLast != NULL) {
			//Truncamos la cadena
			*pszTempLast = '\0';		
		}
	} //endif
	
	//Devolvemos la Cadena resultado en la Pila
	RuntimePush(vString);
    return M4_SUCCESS;
}


// Convierte una cadena a Mayusculas o minusculas.
// Defines en ClDefine.hpp :	M4CL_UPPERCASE(1) o M4CL_LOWERCASE(-1)
// Macros en Trad.cpp (en el Constructor ClTrad::ClTrad)
// Entrada: String(str), Filtro(d)
// Salida:  res = str | "" | Null
m4return_t  ClVM0String::ConvertCase     (void){
    
	m4VariantType vString, vFiltro;
    m4char_t *pszTemp;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ConvertCase");
#endif
    //2o.arg: Sentido del Filtro: M4CL_UPPERCASE(1) o M4CL_LOWERCASE(-1)
    ret=RuntimePop(vFiltro);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConvertCase")	;
	
    //1o.arg: Cadena a filtrar
    ret=RuntimePop(vString);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConvertCase")	;
	
	if ( !CheckStringIfErrorPush (vString, "") )   {		//Error
		DeleteVarString (vFiltro);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vFiltro, vString, M4_TRUE) )   {		//Error
		return M4_SUCCESS;
	}
    
	//Ok
	pszTemp = m_oDynamicPool->GetString (vString.Data.CharData);
	
	//Accion: CASE_UPPER
	if (vFiltro.Data.DoubleData == M4CL_UPPERCASE)
	{
		// UNICODE LOCALE
		M4StrUpr( pszTemp );
	}
	//Accion: CASE_LOWER
	else if (vFiltro.Data.DoubleData == M4CL_LOWERCASE)
	{
		// UNICODE LOCALE
		M4StrLwr( pszTemp );
	}
		
	//Devolvemos la Cadena resultado en la Pila
	RuntimePush (vString);
	
	return M4_SUCCESS;
}


//Reemplaza 'OldSub' de 'String' con 'NewSub' !!!
// Entrada: String(str), OldSub(str), NewSub(str)
// Salida:  res = str | "" | Null | M4_ERROR(si no hay memoria)
m4return_t  ClVM0String::Replace         (void){
	
	m4VariantType vString, vOldSub, vNewSub;
    m4char_t *pszTemp, *pszTempS, *pszTempO, *pszTempN, *pszTempNew, *pszTempNewX;
	m4int32_t	lTempN;
	m4char_t	vTempChar;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Replace");
#endif
    //3o.arg: Nueva cadena
    ret=RuntimePop(vNewSub);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Replace")	;
	
    //2o.arg: Antigua cadena
    ret=RuntimePop(vOldSub);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Replace")	;
	
    //1o.arg: Cadena a filtrar
    ret=RuntimePop(vString);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Replace")	;
	
	if ( !CheckStringIfErrorPush (vString, "") )   {			//Error
		//liberamos las posibles cadenas
		DeleteVarString (vOldSub);
		DeleteVarString (vNewSub);
		return M4_SUCCESS;
	}
	
	if ( !CheckStringIfErrorPush (vOldSub, vString, M4_TRUE) )   {		//Error
		//liberamos las posibles cadenas
		DeleteVarString (vNewSub);
		return M4_SUCCESS;
	}
	
	if ( !CheckStringIfErrorPush (vNewSub, vString, M4_TRUE) )   {		//Error
		m_oDynamicPool->DeleteString (vOldSub.Data.CharData);
		return M4_SUCCESS;
	}
	
	//Ok    
	//Accion: Replace
	pszTempS= m_oDynamicPool->GetString (vString.Data.CharData);
	pszTempO= m_oDynamicPool->GetString (vOldSub.Data.CharData);
	pszTempN= m_oDynamicPool->GetString (vNewSub.Data.CharData);	
	lTempN = m_oDynamicPool->GetStringLength (vNewSub.Data.CharData) ;
	//Si *pszTempO == '\0' devolver la misma
	if (*pszTempS == '\0' || *pszTempO == '\0') {
		//liberamos las posibles cadenas
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		m_oDynamicPool->DeleteString (vOldSub.Data.CharData);
        RuntimePush (vString);	//metemos la cadena sin tocar
        return M4_SUCCESS;      //retornamos OK
    }
	
	//Si *pszTempN == '\0' => borrar OldSub
	pszTempNew = new m4char_t [1];
	if (pszTempNew == NULL) {
		//liberamos las posibles cadenas
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		m_oDynamicPool->DeleteString (vOldSub.Data.CharData);
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	pszTempNew [0] = '\0';
	
	//Buscamos en String la subcadena Old
	while ( (pszTemp = strstr(pszTempS, pszTempO)) != NULL ) { 
		//Copiamos hasta la primera ocurrencia
		vTempChar = *pszTemp;
		*pszTemp = '\0';		
		pszTempNewX = new m4char_t [ strlen(pszTempNew) + strlen(pszTempS) + lTempN + 1 ];
		if (pszTempNewX == NULL) {
			//liberamos las posibles cadenas
			delete [] pszTempNew;
			m_oDynamicPool->DeleteString (vString.Data.CharData);
			m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
			m_oDynamicPool->DeleteString (vOldSub.Data.CharData);
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
		strcpy(pszTempNewX, pszTempNew);
		strcat(pszTempNewX, pszTempS);
		strcat(pszTempNewX, pszTempN);
		delete [] pszTempNew;
		*pszTemp = vTempChar;
		pszTempS   = pszTemp + strlen(pszTempO);
		pszTempNew = pszTempNewX;
	}
	pszTempNewX = new m4char_t [ strlen(pszTempNew) + strlen(pszTempS) +1 ];
	if (pszTempNewX == NULL) {
		//liberamos las posibles cadenas
		delete [] pszTempNew;
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		m_oDynamicPool->DeleteString (vOldSub.Data.CharData);
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	strcpy(pszTempNewX, pszTempNew);
	strcat(pszTempNewX, pszTempS);
	delete [] pszTempNew;
	pszTempNew = pszTempNewX;
	
	//liberamos las posibles cadenas
	m_oDynamicPool->DeleteString (vString.Data.CharData);
	m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
	m_oDynamicPool->DeleteString (vOldSub.Data.CharData);

	//Devolvemos la Cadena resultado en la Pila
	RuntimePushString( pszTempNew, vString.IsUser || vNewSub.IsUser );
	delete [] pszTempNew;
	
    return M4_SUCCESS;
}

//Indice de la primera ocurrencia de NewSub en String a
//partir de la posicion Index (empezando por 0)!!!
//Entrada:  Cadena(str), Subcadena(str), IndexInicial(num)
//Salida:	Index de Subcadena en Cadena.
//			Si Cadena o Subcadena == ""	=> Index = 0
//			Cualquier Situacion de error => No encontrado Index => Index = -1
// Entrada: String(str), NewSub(str), Index(d)
// Salida:  res = str | Null | M4_ERROR (si no hay memoria)

m4return_t  ClVM0String::IndexOf         (void){
	
	m4VariantType vString, vNewSub, vIndex;
    m4char_t	*pszTempS, *pszTempN;
	m4int32_t	iIndex;
    m4return_t  ret=M4_SUCCESS;
	size_t	iSize = 0 ;
	int		iLength = 0 ;
	int		iBytes = 0 ;
	int		iChars = 0 ;

    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: IndexOf");
#endif
    //3o.arg: Index
    ret=RuntimePop(vIndex);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "IndexOf")	;
	
    //2o.arg: SubCadena
    ret=RuntimePop(vNewSub);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "IndexOf") ;
	
    //1o.arg: Cadena en la que buscar
    ret=RuntimePop(vString);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "IndexOf") ;
	
	if ( !CheckStringIfErrorPush (vString, M4_ERROR) )
	{		//Error
		//liberamos las posibles cadenas
		DeleteVarString (vNewSub);
		DeleteVarString (vIndex);
		return M4_SUCCESS;
	}
	
	if ( !CheckStringIfErrorPush (vNewSub, M4_ERROR, M4_TRUE) )
	{		//Error
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		DeleteVarString (vIndex);
		return M4_SUCCESS;
	}
	
	if (IsNull(vIndex))
	{
		vIndex.Type = M4CL_CPP_TYPE_NUMBER;
		vIndex.Data.DoubleData = 0;
	}
	else if ( !CheckNumberIfErrorPush (vIndex, M4_ERROR, M4_TRUE) )
	{		//Error
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		return M4_SUCCESS;
	}
	
	//Ok
	//Accion: IndexOf
	pszTempS = m_oDynamicPool->GetString (vString.Data.CharData);
	pszTempN = m_oDynamicPool->GetString (vNewSub.Data.CharData);

	// UNICODE LN4
	iLength = m_oDynamicPool->GetStringLength( vString.Data.CharData ) ;
	iIndex = M4CharPosition( pszTempS, iLength, vIndex.Data.DoubleData, iSize ) ;

	//Comprobamos el rango del Indice en [0 - (Longitud-1)]

	/* Bug 0125235
	Hay que dejar la comprobación de que sea mayor que el número de caracteres
	*/
	iChars = M4StrLen( pszTempS, iLength ) ;

	// Bug 0207778: it was comparing iIndex and iChars instead of iLength. We must compare bytes to bytes
	if( iIndex < 0 || iIndex >= iLength )
	{
		//liberamos las cadenas
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
        RuntimePushDouble(M4_ERROR); //metemos un -1 como resultado
		return M4_SUCCESS;
	}    		

	//Posicionamos en el Indice Inicial
	pszTempS= pszTempS + iIndex;
	//Si *pszTempN == '\0' devolver la misma
	if(*pszTempS == '\0' || *pszTempN == '\0')
	{
		//liberamos las cadenas
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
        RuntimePushDouble(0.0);
        return M4_SUCCESS;      //retornamos OK
    }

	//Buscamos la subcadena
	pszTempN = strstr( pszTempS, pszTempN );
	if(pszTempN != NULL)
	{
		// UNICODE LN4
		iBytes = pszTempN - pszTempS ;
		iChars = M4StrLen( pszTempS, iBytes ) ;

		iIndex = vIndex.Data.DoubleData + iChars ;		
	}
	else
	{
		iIndex  = -1; //metemos un -1 como resultado
	}

	RuntimePushDouble(iIndex);
	
	//liberamos las cadenas
	m_oDynamicPool->DeleteString (vString.Data.CharData);
	m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
    return M4_SUCCESS;
}

//Indice de la ultima ocurrencia de NewSub en String a
//partir de la posicion Index (empezando por 0)!!!
// 2 argumentos obligatorios, 3 argumentos posibles
// Entrada: String(str), NewSub(str), [IndexInitial(d)]
// Salida:  res = Index(d) | M4_ERROR (Error) | Null

m4return_t  ClVM0String::LastIndexOf     (void){
	
	m4VariantType vString, vNewSub, vIndex;
    m4char_t	*pszTempS, *pszTempN, *pszTemp;
	m4int32_t	iIndex, iTempN;
	m4int32_t	iIndexLast;
    m4return_t  ret=M4_SUCCESS;
	size_t	iSize = 0 ;
	int		iLength = 0 ;
	int		iBytes = 0 ;
	int		iChars = 0 ;
    

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: LastIndexOf");
#endif
    //leemos el número de argumentos
    RuntimePop(vIndex);
    iIndex = int(vIndex.Data.DoubleData);
    if (iIndex<2)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "LastIndexOf"<<">=2" << DumpCallStack(&m_oState) );
		//limpiamos el Pool, porsi
		if (iIndex == 1)
		{
			RuntimePopAndDelete();
		}
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
	// 3 o más argumentos
	if (iIndex>=3)
	{
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "LastIndexOf" <<3<< iIndex << DumpCallStack(&m_oState) );
		//Sacamos todos los argumentos variables inutiles
		for ( iTempN=0; iTempN < iIndex - 3; iTempN++ )
		{
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LastIndexOf")	;
		}
		//3o.arg: Index
		ret=RuntimePop(vIndex);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LastIndexOf")	;
	}
	else
	{
		vIndex.Type = M4CL_CPP_TYPE_NUMBER;
		vIndex.Data.DoubleData = 0;
	}
	
    //2o.arg: SubCadena
    ret=RuntimePop(vNewSub);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LastIndexOf") ;
	
    //1o.arg: Cadena en la que buscar
    ret=RuntimePop(vString);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LastIndexOf") ;
	
	
	if ( !CheckStringIfErrorPush (vString, M4_ERROR) )
	{		//Error
		DeleteVarString (vNewSub);
		DeleteVarString (vIndex);
		return M4_SUCCESS;
	}
	
	if ( !CheckStringIfErrorPush (vNewSub, M4_ERROR, M4_TRUE) )
	{		//Error
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		DeleteVarString (vIndex);
		return M4_SUCCESS;
	}
	
	
    if (IsNull (vIndex) )
	{		//Null behaviour: Valor nulo->Nulo
		vIndex.Type	= M4CL_CPP_TYPE_NUMBER;
		vIndex.Data.DoubleData = 0.0;
    }
	else if ( !CheckNumberIfErrorPush (vIndex, M4_ERROR, M4_TRUE) )
	{		//Error
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		return M4_SUCCESS;
	}
	
	//Ok
	//Accion: IndexOf
	pszTempS = m_oDynamicPool->GetString (vString.Data.CharData);
	pszTempN = m_oDynamicPool->GetString (vNewSub.Data.CharData);

	// UNICODE LN4
	iLength = m_oDynamicPool->GetStringLength( vString.Data.CharData ) ;
	iIndex = M4CharPosition( pszTempS, iLength, vIndex.Data.DoubleData, iSize ) ;

	//Comprobamos el rango del Indice en [0 - (Longitud-1)]

	/* Bug 0125235
	Hay que dejar la comprobación de que sea mayor que el número de caracteres
	*/
	iChars = M4StrLen( pszTempS, iLength ) ;

	if ( iIndex < 0 || iIndex >= iChars )
	{
        RuntimePushDouble(M4_ERROR); //metemos un -1 como resultado
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
		return M4_SUCCESS;
	} 
	
	//Posicionamos en el Indice Inicial
	pszTempS= pszTempS + iIndex;
	//Si *pszTempN == '\0' devolver la misma
	if(*pszTempS == '\0' || *pszTempN == '\0')
	{
        RuntimePushDouble(0.0); //metemos un -1 como resultado
		m_oDynamicPool->DeleteString (vString.Data.CharData);
		m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
        return M4_SUCCESS;      //retornamos OK
    }

	//Buscamos la ultima subcadena
	iIndexLast = -1;
	iTempN = strlen(pszTempN);
	pszTemp = pszTempS;
	while( (pszTemp = strstr( pszTemp, pszTempN )) != NULL ) 
	{
		// UNICODE LN4
		iBytes = pszTemp - pszTempS ;
		iIndexLast = M4StrLen( pszTempS, iBytes ) ;

		pszTemp += iTempN;		
	}

	if (iIndexLast != -1 )
	{
		iIndex = vIndex.Data.DoubleData + iIndexLast ;
	}
	else
	{
		iIndex = -1; //metemos un -1 como resultado
	}

	RuntimePushDouble( iIndex );
	
	//Eliminamos las Cadenas temporales del Pool en orden inverso
	m_oDynamicPool->DeleteString (vString.Data.CharData);
	m_oDynamicPool->DeleteString (vNewSub.Data.CharData);
    return M4_SUCCESS;
}



//------------------------
//CHR
//------------------------
m4return_t  ClVM0String::Chr(void){
	
    m4return_t		ret=M4_SUCCESS;
	int				iLength = 0 ;
	unsigned int	iCodePoint = 0 ;
	m4pchar_t		pccString = NULL;
    m4VariantType	vTemp1;
	M4XMLCh			wcString[3];

    
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Chr");
#endif
    //cadena a buscar
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Chr")	;
	
    //tipo 
    if( vTemp1.Type!=M4CL_CPP_TYPE_NUMBER )
	{
		//Devolvemos lo mismo
        RuntimePush(vTemp1);
    }
    else
	{
		// Bug 0301392 Hay que soportar UTF-16 de dos palabras
		iCodePoint = (unsigned int)vTemp1.Data.DoubleData ;

		if( iCodePoint <= 0xFFFF )
		{
			//Cadena de longitud 1
			wcString[0] = iCodePoint & 0xFFFF ;
			wcString[1] = 0 ;
			iLength = 1 ;
		}
		else
		{
			//Cadena de longitud 2
			iCodePoint -= 0x10000 ;
			wcString[0] = 0xD800 | ( iCodePoint >> 10 ) ;
			wcString[1] = 0xDC00 | ( iCodePoint & 0x3FF ) ;
			wcString[2] = 0 ;
			iLength = 2 ;
		}

		// UNICODE LN4
		pccString = M4Utf16ToCpp( wcString, iLength ) ;
        RuntimePushString( pccString ) ;

		delete [] pccString ;
    }
	
    return M4_SUCCESS;
	
}

//----------
//ASC
//----------
m4return_t  ClVM0String::Ascii(void){
	
    m4VariantType	vTemp ;
    m4return_t		ret = M4_SUCCESS ;
	int				iLength = 0 ;
	m4pcchar_t		pccString = NULL ;
	M4XMLCh*		pcwcString = NULL ;

    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Ascii");
#endif
    //cadena a buscar
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Ascii")	;   
	
	if ( CheckStringIfErrorPush (vTemp, 0.0) )
	{	//Ok

		// UNICODE LN4
		iLength = m_oDynamicPool->GetStringLength( vTemp.Data.CharData ) ;
		pccString = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
		pcwcString = M4CppToUtf16( pccString, iLength ) ;

        RuntimePushDouble( pcwcString[0] );
		delete [] pcwcString ;

        m_oDynamicPool->DeleteString (vTemp.Data.CharData);
    }
	
    return M4_SUCCESS;
}

//---------------------------------------------------------
//-F O R M A T
//---------------------------------------------------------
m4return_t  ClVM0String::Format          (void){
    
    char    *pszOrigen;        
    char    *pszSeg;                           
    int     iPunto, iSigno, iDetras;
    m4VariantType vNumero, vPrec;
    m4return_t  ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Format");
#endif
    //sacamos la precision (1er arg)
    ret=RuntimePop(vPrec);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Format")	;
    
    //sacamos el numero (2do arg)
    ret=RuntimePop(vNumero);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Format")	;
	
	if ( !CheckNumberIfErrorPush (vNumero, 0.0) ) {	//error
		DeleteVarString (vPrec);
        return M4_SUCCESS;
    }
	
	if ( !CheckNumberIfErrorPush (vPrec, 0.0) ) {	//error
        return M4_SUCCESS;
    }
	
	// Si la precision es "exagerada" pasamos... para evitar GP
	int iPrec = (int)vPrec.Data.DoubleData;	// Hacemos un solo cast de double o int
	if ( iPrec > 8 ) {
		iPrec = 8;	//Precision exagerada => ponemos 8
	}
	
	//Ok
	m4char_t buffer [ M4_WIN2UX_MAX_LONG_NUMBER ];
    m4char_t pszDestino [ M4CL_MAX_STRING ] ;
    
	// Ajuste a punto gordo, para evitar las perdidas de precision del modf
	vNumero.Data.DoubleData = M4VMDoubleAdjust(vNumero.Data.DoubleData);

    pszSeg = pszDestino;
    pszOrigen = M4_fcvt(vNumero.Data.DoubleData, iPrec , &iPunto, &iSigno, buffer, M4_WIN2UX_MAX_LONG_NUMBER);
    
    if (iSigno) *(pszSeg++) = '-';
	
	if (iPunto <= 0) {
		iPunto= (-iPunto > iPrec ? -iPrec : iPunto);
		strcpy (pszSeg,"0");
		pszSeg++;
		
		iDetras = strlen(pszOrigen) ;
	}
	else{
		strncpy( pszSeg, pszOrigen, iPunto  ) ;
		pszSeg += iPunto;  
		iDetras = strlen(pszOrigen) - iPunto;
// Parece que en Solaris la funcion de C :
//		char *fcvt( double value, int count, int *dec, int *sign );
// con el count<0 no trunca hacia la izquierda. 
// Ej: = fcvt(145673.458312321, -2, &iPunto, &iSigno);
//		Devuelve "145700" en vez de "1457"
// Format(145673.458312321,-2)="1457"
#if defined(_M4SOLX86_) || defined(_M4SOLSPARC_)
		if ( iPrec < 0 ) {
			iDetras = iPrec;
		}
#endif
	}
	
    if ( iPrec > 0) 
    {
        *(pszSeg++) = '.';                  
		if (iPunto <0){
			memset (pszSeg, '0', -iPunto);	
			pszSeg+= (-iPunto);
			iPunto=0;
		}
		
		strncpy( pszSeg, pszOrigen + iPunto, iDetras ) ;
		
    }
    * ( pszSeg + iDetras ) = '\0';   
    
    RuntimePushString (pszDestino); 
    
    return M4_SUCCESS;
}

//---------------------------------------------------
//MatchRegex
//---------------------------------------------------

m4return_t	ClVM0String::MatchRegex		(void){

	m4VariantType vNum,vRegex,vString,vSensitive;
	m4uint32_t iNumArgs,i;
	m4bool_t bSensitive=M4_TRUE;
	m4return_t ret;
	m4int_t	   res;
	//sacamos argumentos comprobando tipos

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "MatchRegex.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 3 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "MatchRegex"<<3<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 3 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "MatchRegex")	;
        }
        iNumArgs=3; //ya hemos sacado los sobrantes
    }
    
	if (iNumArgs==3){//son indican si case sensitive o no
		//sacamos el argumentos IsCaseSensitive
		ret = RuntimePopNumber( vSensitive ) ;
		M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "IsCaseSensitive" << "MatchRegex" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSensitive.Type)<< DumpCallStack(&m_oState) );

		bSensitive=(vSensitive.Data.DoubleData==0.0?M4_FALSE:M4_TRUE);
	}//sino por defecto=true

	//sacamos la regex
    ret = RuntimePopString( vRegex ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "RegularExpression" << "MatchRegex" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vRegex.Type)<< DumpCallStack(&m_oState) );
	

	ret = RuntimePop( vString ) ;
	if ( M4_SUCCESS == ret && IsNull( vString ) )
	{
		// Si el primer elemento es nulo no debe dar error. Simplemente que no cumple el patrón Regex.
		// (Esto habilita el uso de esta función, por ejemplo en filtros en RAM, en el caso en el que
		// el argumento sea un item de tipo cadena pero su valor sea nulo).
		res = -1;
	}
	else
	{
		//String
		M4CHECK_RES( ret == M4_SUCCESS && vString.Type == M4CL_CPP_TYPE_STRING_POOL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "String" << "MatchRegex" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vString.Type)<< DumpCallStack(&m_oState) );

		//preparamos el objeto

		//si el objeto no está creado, lo creamos
		if (!m_poRegex) m_poRegex=new ClRegExp;

		//case?
		m_poRegex->SetSensitive(bSensitive);

		//compilamos la expresión regular
		m4char_t *pszRes=m_poRegex->CompilePattern(m_oDynamicPool->GetString(vRegex.Data.CharData), m_oDynamicPool->GetStringLength(vRegex.Data.CharData) );
		if (pszRes){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_REGEX, m_oDynamicPool->GetString(vRegex.Data.CharData) << DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		//llamamos a buscar
		res=m_poRegex->Search(m_oDynamicPool->GetString(vString.Data.CharData),m_oDynamicPool->GetStringLength(vString.Data.CharData), 0, m_oDynamicPool->GetStringLength(vString.Data.CharData) );

		// UNICODE LN4
		/* Bug 0126182
		Si estamos en unicode hay que devolver caracteres y no bytes
		*/
		if( res > 0 )
		{
			res = M4StrLen( m_oDynamicPool->GetString(vString.Data.CharData), res ) ;
		}
		
		//Borramo las cadena de búsqueda
		m_oDynamicPool->DeleteString(vString.Data.CharData);
	}

	//Borramos las cadena de expresión
	m_oDynamicPool->DeleteString(vRegex.Data.CharData);

	//retornamos el índice que encuentre
	RuntimePushDouble(res);


	return M4_SUCCESS;

}

