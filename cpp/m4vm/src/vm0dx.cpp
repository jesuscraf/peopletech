#include "vmres.hpp"

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
//    implementacion de la clase que implementa las funciones de compatibilidad con 2x
////
//==============================================================================

#include "vm0dx.hpp"

#include "fechas.h"
#include "clstringpool.hpp"
#include "cldates.hpp"
#include "medvarin.hpp"
#include "m4objglb.hpp"


//----------------------------------
//De 2.x a 3.x
//----------------------------------

// Devuelve el año dada una fecha Date !!!
m4return_t  ClVM0dx::DxYear           (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxYear");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxYear")	;
	
	if ( CheckDateIfErrorPush (vTemp, 0.0) ) {
		RuntimePushDouble ( nano(vTemp.Data.DoubleData) );
	}
    return M4_SUCCESS;   
}

m4return_t  ClVM0dx::DxCompare        (void){
	m4VariantType vString1, vString2;
	m4int8_t	iComp;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF  (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxCompare");
#endif
    //2o.arg: Cadena 2
    ret=RuntimePop(vString2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxCompare") ;
	
    //1o.arg: Cadena 1
    ret=RuntimePop(vString1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxCompare") ;
	
	if ( !CheckStringIfErrorPush (vString1, 0.0) )   {		//Error
		DeleteVarString (vString2);
		return M4_SUCCESS;
	}
	
	if ( !CheckStringIfErrorPush (vString2, 0.0) )   {		//Error
		m_oDynamicPool->DeleteString (vString1.Data.CharData);
		return M4_SUCCESS;
	}
	
	//Ok
	//Accion: Comparar vS1 y vS2
	iComp = strcmp(m_oDynamicPool->GetString (vString1.Data.CharData), m_oDynamicPool->GetString (vString2.Data.CharData));
	if (iComp < 0)
		iComp = 1;
	else if (iComp > 0)
		iComp = -1;
	
	//Eliminamos las Cadenas temporales del Pool en orden inverso
	m_oDynamicPool->DeleteString (vString1.Data.CharData);
	m_oDynamicPool->DeleteString (vString2.Data.CharData);
	
	//Devolvemos resultado
	RuntimePushDouble(iComp);
    return M4_SUCCESS;
}

// Devuelve el Dia dada una fecha Date
m4return_t  ClVM0dx::DxDay            (void){
	
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    int			years, months, days;
    m4int32_t	hours, minutes, seconds;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDay");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDay")	;
	
	if ( CheckDateIfErrorPush (vTemp, 0.0) ) {
		//convertimos a dia, mes , año
		ClDateToNumbers (vTemp.Data.DoubleData, years, months, days, hours, minutes, seconds);
		//metemos dias
		RuntimePushDouble (days);
	}
    return M4_SUCCESS;   
}

//Devuelve el Numero de Dias del Mes del Año indicado !!!
m4return_t  ClVM0dx::DxDaysOfMonth    (void){
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
	int iMes, iAno;
	
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDaysOfMonth")	;
	
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDaysOfMonth")	;
	
	if ( !CheckNumberIfErrorPush (vTemp2, 0.0) )   {		//Error
		DeleteVarString (vTemp1);
		return M4_SUCCESS;
	}
	
	if ( CheckNumberIfErrorPush (vTemp1, 0.0) )   {			//Ok
		iMes = int(vTemp1.Data.DoubleData);
		iAno = int(vTemp2.Data.DoubleData);	
		RuntimePushDouble (ndiasdelmes (iMes, iAno));	
	}
    return M4_SUCCESS;       
}


//Devuelve la fecha Date dada una fecha days + months + years
m4return_t  ClVM0dx::DxDate           (void){
    m4VariantType vTemp1, vTemp2, vTemp3;
    m4return_t  ret=M4_SUCCESS;
    m4date_t    dFecha;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDate");
#endif
    ret=RuntimePop(vTemp3);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate")	;
	
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate")	;
	
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate")	;
	
	if ( !CheckNumberIfErrorPush (vTemp3, 0.0) )   {		//Error
		DeleteVarString (vTemp1);
		DeleteVarString (vTemp2);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vTemp2, 0.0) )   {		//Error
		DeleteVarString (vTemp1);
		return M4_SUCCESS;
	}
	
	if ( CheckNumberIfErrorPush (vTemp1, 0.0) )   {		//Ok    
		//convertimos a fecha
		ClNumbersToDate( int(vTemp3.Data.DoubleData), int(vTemp2.Data.DoubleData), int(vTemp1.Data.DoubleData), 
			0, 0, 0, dFecha );
		RuntimePushDate (dFecha);
	}    
    return M4_SUCCESS;
}

//Devuelve la fecha Juliana dada una fecha day + month + year !!!
//usando meses de 30 dias
m4return_t  ClVM0dx::DxDate30         (void){
	
    m4VariantType vTemp1, vTemp2, vTemp3;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDate30");
#endif
    ret=RuntimePop(vTemp3);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30")	;
	
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30")	;
	
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30")	;
    
	if ( !CheckNumberIfErrorPush (vTemp3, 0.0) )   {		//Error
		DeleteVarString (vTemp1);
		DeleteVarString (vTemp2);
		return M4_SUCCESS;
	}
	
	if ( !CheckNumberIfErrorPush (vTemp2, 0.0) )   {		//Error
		DeleteVarString (vTemp1);
		return M4_SUCCESS;
	}
	
	if ( CheckNumberIfErrorPush (vTemp1, 0.0) )   {		//Ok    
		RuntimePushDouble ( nfecha30( int(vTemp1.Data.DoubleData), int(vTemp2.Data.DoubleData), int(vTemp3.Data.DoubleData), 0) );
	}    
	
    return M4_SUCCESS;
}

//Devuelve la fecha Juliana dada una fecha day + month + year !!!
//usando meses de 30 dias
m4return_t  ClVM0dx::DxDate30Ex( void )
{

    m4return_t		iResult = M4_SUCCESS ;
	m4int32_t		i = 0 ;
	m4int32_t		iNumArgs = 0 ;
	m4int32_t		iFebruaryAs30 = 1 ;
	m4VariantType	vNumArgs ;
	m4VariantType	vFebruaryAs30 ;
	m4VariantType	vYear ;
	m4VariantType	vMonth ;
	m4VariantType	vDay ;
	m4VariantType	vResult ;

	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDate30Ex");
#endif

	//número de argumentos
	iResult= RuntimePopNumber( vNumArgs ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30Ex" ) ;
	iNumArgs = m4int32_t( vNumArgs.Data.DoubleData ) ;

    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if( iNumArgs > 4 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "DxDate30Ex" << 4 << iNumArgs << DumpCallStack() ) ;
        //sacamos los que sobran
        for( i = 0 ; i < iNumArgs - 4 ; i++ )
		{
            iResult = RuntimePopAndDelete() ;
            DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30Ex" ) ;
        }
        iNumArgs = 4 ; //ya hemos sacado los sobrantes
    }

	if( iNumArgs == 4 )
	{
		iResult = RuntimePopNumber( vFebruaryAs30 ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FebruaryAs30" << "DxDate30Ex" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vFebruaryAs30.Type ) << DumpCallStack() ) ;
	}
	else
	{
		vFebruaryAs30.Data.DoubleData = M4_TRUE ;
	}

	if( m4int32_t( vFebruaryAs30.Data.DoubleData ) == M4_TRUE )
	{
		iFebruaryAs30 = 1 ;
	}
	else
	{
		iFebruaryAs30 = 0 ;
	}

	iResult = RuntimePop( vYear ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30Ex" ) ;
	
	iResult = RuntimePop( vMonth ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30Ex" ) ;
	
	iResult = RuntimePop( vDay ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDate30Ex" ) ;
	
	if( !CheckNumberIfErrorPush( vYear, 0.0 ) )
	{		//Error
		DeleteVarString( vDay ) ;
		DeleteVarString( vMonth ) ;
		return( M4_SUCCESS ) ;
	}
	
	if( !CheckNumberIfErrorPush( vMonth, 0.0 ) )
	{		//Error
		DeleteVarString( vDay ) ;
		return( M4_SUCCESS ) ;
	}
	
	if( CheckNumberIfErrorPush( vDay, 0.0 ) )
	{
		RuntimePushDouble( nfecha30( int( vDay.Data.DoubleData ), int( vMonth.Data.DoubleData ), int( vYear.Data.DoubleData ), iFebruaryAs30 ) ) ;
	}

    return( M4_SUCCESS ) ;
}

//Devuelve la fecha ISO: "yyyy-mm-dd" dada una fecha Date !!!
m4return_t  ClVM0dx::DxDateIso        (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;	
	m4char_t	azTemp[ M4_SECOND_END + 1 ] ;
    int			iYear, iMonth, iDay;
    m4int32_t	iHour, iMinute, iSecond;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDateIso");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDateIso")	;
	
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {        
		//convertimos a dia, mes , año, Formateamos, y Metemos
		ClDateToNumbers (vTemp.Data.DoubleData, iYear, iMonth, iDay, iHour, iMinute, iSecond );
		sprintf( azTemp, "%04d-%02d-%02d", iYear, iMonth, iDay ) ;    
		RuntimePushString (azTemp);
	}
    return M4_SUCCESS;   
}

//Devuelve la fecha ISOD: "{d yyyy-mm-dd}" dada una fecha Date !!!
m4return_t  ClVM0dx::DxDateIsoD       (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;	
	m4char_t	azTemp[ M4CL_MAX_STRING + 1 ] ;
    int			iYear, iMonth, iDay;
    m4int32_t	iHour, iMinute, iSecond;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDateIsoD");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDateIsoD")	;
    
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {
		//convertimos a dia, mes , año, Formateamos, y metemos
		ClDateToNumbers (vTemp.Data.DoubleData, iYear, iMonth, iDay, iHour, iMinute, iSecond );
		sprintf( azTemp, "{d %04d-%02d-%02d}", iYear, iMonth, iDay ) ;
		RuntimePushString (azTemp);
	}
    return M4_SUCCESS;   
}

//Devuelve la fecha ISOTS: "{ts yyyy-mm-dd hh:mm:ss}" dada una fecha Date !!!
m4return_t  ClVM0dx::DxDateIsoTS      (void){
	
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;	
	m4char_t	azTemp[ M4CL_MAX_STRING + 1 ] ;
    int			iYear, iMonth, iDay;
    m4int32_t	iHour, iMinute, iSecond;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxDateIsoTS");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxDateIsoTS")	;
    
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {
		//convertimos a dia, mes , año, Formateamos, y metemos fecha
		ClDateToNumbers (vTemp.Data.DoubleData, iYear, iMonth, iDay, iHour, iMinute, iSecond );
		sprintf( azTemp, "{ts %04d-%02d-%02d %02d:%02d:%02d}", iYear, iMonth, iDay, iHour, iMinute, iSecond ) ;
		RuntimePushString (azTemp);
	}
    return M4_SUCCESS;   
}

//Incrementa el valor de una variable !!!
m4return_t  ClVM0dx::DxInc            (void){
    
    m4VariantType vTemp;
    m4return_t ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxInc");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxInc")	;
	
    if (IsNull (vTemp) ){       //Null behaviour: Valor nulo->Nulo
        RuntimePushNull ();
        return M4_SUCCESS;
    }   
    if (vTemp.Type!=M4CL_CPP_TYPE_STRING_POOL){ //fecha o número
        ++vTemp.Data.DoubleData;
    }
    
    RuntimePush (vTemp);    
    return M4_SUCCESS;
}

//Devuelve el Mes dada una fecha Date !!!
m4return_t  ClVM0dx::DxMonth          (void){
	
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    int			years, months, days;
    m4int32_t	hours, minutes, seconds;
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxMonth");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxMonth")	;
    
	if ( CheckDateIfErrorPush (vTemp, 0.0) ) {
		//convertimos a dia, mes , año
		ClDateToNumbers (vTemp.Data.DoubleData, years, months, days, hours, minutes, seconds);
		//metemos años
		RuntimePushDouble (months);
	}
    return M4_SUCCESS;   
}

//Redondea un Variant a Centenas !!!
m4return_t  ClVM0dx::DxRoundCent      (void){
	
    m4VariantType   vTemp;
	m4double_t      dEntera, dFrac;
    m4return_t      ret;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxRoundCent");
#endif
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxRoundCent")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) ) {
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);
		dFrac = modf (100 * vTemp.Data.DoubleData + (vTemp.Data.DoubleData>=0.0 ? 0.5 : -0.5), &dEntera );
		RuntimePushDouble( dEntera/100 );
	}
    return M4_SUCCESS;       
}

//Redondea un Variant !!!
m4return_t  ClVM0dx::DxRound          (void){
	
    m4VariantType   vTemp;
	m4double_t      dEntera, dFrac;
    m4return_t      ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxRound");
#endif
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxRound")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) ) {
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);
		dFrac = modf (vTemp.Data.DoubleData + (vTemp.Data.DoubleData>=0.0 ? 0.5 : -0.5), &dEntera );
		RuntimePushDouble(dEntera);
	}
    return M4_SUCCESS;       
}

//Redondea un Variant !!!
m4return_t  ClVM0dx::DxRound50        (void){
	
    m4VariantType   vTemp;
	m4double_t      dEntera, dFrac;
    m4return_t      ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxRound50");
#endif
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxRound50")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) ) {
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);
		dFrac = modf (vTemp.Data.DoubleData, &dEntera );
		vTemp.Data.DoubleData += ( (dFrac<0.25) ? (-dFrac) : ( (dFrac>0.75) ? (1-dFrac) : (0.5 - dFrac) ) );
		RuntimePushDouble( vTemp.Data.DoubleData );
	}
    return M4_SUCCESS;       
}


//Trunca las Centenas. Ej: 1557 -> 1500  !!!
m4return_t  ClVM0dx::DxTruncaCent     (void){
	
    m4VariantType   vTemp;
	m4double_t      dInteger;
    m4return_t      ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DxTruncaCent");
#endif
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DxTruncaCent")	;
	
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) ) {
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);
		modf (100.0L * vTemp.Data.DoubleData, &dInteger );
		RuntimePushDouble( dInteger / 100.0L );
	}
	
	return M4_SUCCESS;       
}
