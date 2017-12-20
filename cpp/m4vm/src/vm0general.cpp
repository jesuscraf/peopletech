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

#include "vm0general.hpp"

#include "vmres.hpp"
#include "vmcscontext.hpp"
#include "chlog.hpp"
#include "clstringpool.hpp"
#include "medvarin.hpp"
#include "regedit.hpp"
#include "fechas.h"
#include "cldates.hpp"
#include "m4mdrt.hpp"
#include "m4win2ux.hpp"
#include "m4uuid.hpp"
#include "m4objglb.hpp"
#include "m4objreg.hpp"
#include "m4regres.hpp"
#include "m4unicode.hpp"
#include "m4buildversion.hpp"
#include "blocksynchronization.hpp"
#include "m4thread.hpp"
#include "m4cryptc.hpp"


#define	M4CH_DUMMY_B1	"\xC\xD\x5\x9\xA\xA\xF\x1\x6\x7\xA\xD\x7\xE\xA\xB\x9\xB\x4\x4\x5\x7\xF\xD\xA\x6\xC\xA\x2\xB\xB\xD\xE\xA\x5\x1\x9\x9\x1\x5\xC\xB\x9\xA\xA\x8\x8\xB\xC\x4\x6\x4\x4\xC\xE\xE\x4\xD\x4\x5\xC\xD\x2\x0\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8\x0\xA\x6\x1\x8\x7\xD\xE\xC\x3\x6\xE\x7\xD\x9\x6\x8\x9\xF\xE\x8\x6\x1\x6\x3"
#define	M4CH_DUMMY_K	"\x8\xA\x3\x7\x2\x8\x9\x2\x2\xF\xC\x5\x2\x6\x4\xA\x2\x8\xD\xE\xA\x3\x1\xC\x4\x1\xC\x6\x5\x8\x4\x9\xB\xA\xA\xA\xE\xF\x1\x2\x5\x2\x2\x8\x9\x4\x6\x9\xC\x3\x5\x1\x1\x9\x3\x5\x6\x7\x2\xA\x3\x8\xC\x0\x3\x4\x6\x7\x8\xA\x3\x1\x6\x8\x7\x1\x2\x7\xF\xE\x2\x1\x4\x1\x6\x4\x8\x7"


m4millisec_t	ClVM0General::m_lBaseTickCount = ClPrecisionTicker::GetTimeTick() ;

//---------------------------------------------------------
// R O U N D
//---------------------------------------------------------
m4return_t  ClVM0General::Round           (void){

    m4VariantType   vTemp, vScale;
    m4return_t      ret;
	size_t			iChars = 0 ;
	size_t			iSize = 0 ;
	int				iPosition = 0 ;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Round");
#endif
    //sacamos la escala 
    ret=RuntimePop (vScale);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Round.")	;
	
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Round.")	;
	
    if (IsNull (vTemp) ){ //Null behaviour: Valor nulo->Nulo
		//liberamos las posibles cadenas
		DeleteVarString (vScale);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if ( !CheckNumberIfErrorPush (vScale, 0.0, M4_TRUE) )   {		//Error
		DeleteVarString (vTemp);
		return M4_SUCCESS;
	}
	
	int iScale = (int)vScale.Data.DoubleData;

    switch (vTemp.Type)
    {
    case M4CL_CPP_TYPE_STRING_POOL:
        {
			m4char_t *pszTemp, *pszStr;
			m4int32_t   iLon, iPrec;
            pszStr=m_oDynamicPool->GetString (vTemp.Data.CharData);
            if (pszStr==NULL) 
				break;
            iLon=m_oDynamicPool->GetStringLength (vTemp.Data.CharData);
            iPrec=iScale;
			
			// UNICODE LN4
			iChars = M4StrLen( pszStr, iLon ) ;

            if (abs((m4int_t)iPrec) > iChars) {
                iPrec=iChars;
            }
            
            if (iPrec <0 ){ //truncamos por detras                             

				// UNICODE LN4
				iPosition = M4CharPosition( pszStr, iLon, iChars + iPrec, iSize ) ;
				iPrec = iPosition - iLon;

                pszTemp=new char[-iPrec+1];
                strcpy (pszTemp, pszStr +iLon + iPrec  );
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
                m_oDynamicPool->AddString (pszTemp,vTemp.Data.CharData);
                delete [] pszTemp;
            }
            else{   //por delante
                
				// UNICODE LN4
				iPosition = M4CharPosition( pszStr, iLon, iPrec, iSize ) ;
				iPrec = iPosition ;

                pszTemp=new char[iPrec+1];
                strncpy (pszTemp, pszStr , iPrec);
                //truncamos
                pszTemp[iPrec]=0;
                
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
                m_oDynamicPool->AddString (pszTemp,vTemp.Data.CharData);
                delete [] pszTemp;
            }
            
            break;
        }    
        
    case M4CL_CPP_TYPE_NUMBER:
        {			
			vTemp.Data.DoubleData = M4VMRound(vTemp.Data.DoubleData, iScale);
            break;
        }
        
    case M4CL_CPP_TYPE_DATE:	
        {
			m4int32_t   aiDate[6], aiInit[6]={1800, 1, 1, 0, 0, 0};
			int         day, month, year;
		    m4uint16_t      i;

            if (iScale >6 || iScale <0) {                
                break;
            }
            //convertimos a números
            ClDateToNumbers (vTemp.Data.DoubleData, year, month, day, aiDate[3], aiDate[4], aiDate[5]);
            aiDate[0]=year;
            aiDate[1]=month;
            aiDate[2]=day;            
            
            //iniciamos los que debamos según la precisión
            for (i=iScale; i< 6; i++){
                aiDate[i]=aiInit[i];
            }
            
            //convertimos a double
            ClNumbersToDate (aiDate[0], aiDate[1], aiDate[2], aiDate[3], aiDate[4], aiDate[5], vTemp.Data.DoubleData);
            break;
        }
    }
    
    RuntimePush(vTemp);
    return M4_SUCCESS;       
}

//-----------------------------------------
//C O M M O N   T I M E
//-----------------------------------------
m4return_t  ClVM0General::CommonTime      (void){
	
	m4double_t val;
	m4return_t ret=_CalcCommonTime(val);
	if (ret==M4_SUCCESS){  //si va bien tenemos que meter el resultado en la pila
						   //Si va mal, ya tenemos el resultado en la pila (0.0), porque _calcCommotime ya lo mete
						   //así que salimos con éxito y ya está
		RuntimePushDouble(val);
	}
	return M4_SUCCESS;
}

//-----------------------------------------
//C O M M O N   T I M E S T A M P
//-----------------------------------------
m4return_t  ClVM0General::CommonTimestamp      (void){
	
	m4double_t val;
	m4return_t ret=_CalcCommonTime(val);
	if (ret==M4_SUCCESS){  //si va bien, tenemos que meter el resultado en la pila (restandole uno porque devuelve días sin tener en cuenta timestamp)
						   //Si va mal, ya tenemos el resultado en la pila (0.0), porque _calcCommotime ya lo mete
						   //así que salimos con éxito y ya está
		RuntimePushDouble(val>0?val-1.0:val);
	}
	return M4_SUCCESS;
}

//---------------------------------------------------------
//-C A L C C O M M O N   T I M E (private)
//Función que hace el cálculo de commontime, y que sirve para dias(commonTime)
//Saca los args de la pila 
//y tambien teniendo en cuenta horas...(commontimeStamp)
//Si error: Retorna error y deja 0.0 en la pila
//Si éxito:retorna éxito y el valor en el arg por ref.
//---------------------------------------------------------
m4return_t  ClVM0General::_CalcCommonTime      (m4double_t &ao_res){

    m4VariantType vTemp1, vTemp2, vTemp3, vTemp4;
    m4return_t  ret=M4_SUCCESS;   
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: CommonTime");
#endif
    //Fecha 4 (4º arg.)
    ret=RuntimePop(vTemp4);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CommonTime.")	;
    
    //Fecha 3 (3er arg.)
    ret=RuntimePop(vTemp3);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CommonTime.")	;
    
    //Fecha 2 (2do arg.)
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CommonTime.")	;
    
    //Fecha 1 (1er arg)
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CommonTime.")	;
	
	if ( !CheckDateIfErrorPush (vTemp4, 0.0) ) {	//error
		DeleteVarString (vTemp3);
		DeleteVarString (vTemp2);
		DeleteVarString (vTemp1);
        return M4_ERROR;
	}
	
	if ( !CheckDateIfErrorPush (vTemp3, 0.0) ) {	//error
		DeleteVarString (vTemp2);
		DeleteVarString (vTemp1);
        return M4_ERROR;
	}
	
	if ( !CheckDateIfErrorPush (vTemp2, 0.0) ) {	//error
		DeleteVarString (vTemp1);
        return M4_ERROR;
	}
	
	if ( CheckDateIfErrorPush (vTemp1, 0.0) ) {		//Ok
        ao_res= ndiascomun (vTemp1.Data.DoubleData , vTemp2.Data.DoubleData , vTemp3.Data.DoubleData ,vTemp4.Data.DoubleData);
	}      
	
    return M4_SUCCESS;
}



//---------------------------------------------------------
//-D A Y  O F   W E E K  (1-7)
//---------------------------------------------------------
m4return_t  ClVM0General::DayOfTheWeek    (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DayOfWeek"); 
#endif
	
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DayOfWeek.")	;
	
	if ( CheckDateIfErrorPush (vTemp, 0.0) ) {		//Ok
		RuntimePushDouble( ndiasem(vTemp.Data.DoubleData) );
	}    
    return M4_SUCCESS;
	
}


//---------------------------------------------------------
//-M O N T H S
//---------------------------------------------------------
m4return_t  ClVM0General::Months          (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Months"); 
#endif
	
    //Fecha 2 (2do arg.)
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Months")	;
    
    //Fecha 1 (1er arg)
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Months.")	;
	
	if ( !CheckDateIfErrorPush (vTemp1, 0.0) ) {	//error
		DeleteVarString (vTemp2);
        return M4_SUCCESS;
    }
    
	if ( CheckDateIfErrorPush (vTemp2, 0.0) ) {		//Ok
        RuntimePushDouble (nmeses(vTemp2.Data.DoubleData , vTemp1.Data.DoubleData ));
    }
    
    return M4_SUCCESS;    
}


//---------------------------------------------------------
//- Y E A R S
//---------------------------------------------------------
m4return_t  ClVM0General::Years           (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Years");
#endif
	
    //Fecha 2 (2do arg)
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Years.")	;
    
    //fecha 1 (1er arg)
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Years.")	;
    
	if ( !CheckDateIfErrorPush (vTemp1, 0.0) ) {	//error
		DeleteVarString (vTemp2);
        return M4_SUCCESS;
    }
    
	if ( CheckDateIfErrorPush (vTemp2, 0.0) ) {		//Ok       
		RuntimePushDouble (nannos (vTemp2.Data.DoubleData , vTemp1.Data.DoubleData ));
	}
	
    return M4_SUCCESS;    
}



//---------------------------------------------------------
//- M A X I M O 
//---------------------------------------------------------
m4return_t  ClVM0General::Max             (void){
    
    m4VariantType vTempNum, vTemp1, vTempMax;
    m4uint32_t   i;
    m4return_t  ret;
    m4bool_t    bNull=M4_FALSE;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Max");
#endif
	
    //numero de argumentos
    ret=RuntimePop(vTempNum);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Max.")	;
    
    //sacamos el primer elemento y comprobamos tipo y de momento ese es el máximo
    ret=RuntimePop(vTempMax);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Max.")	;
    
    if (vTempMax.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTempMax);
    }
    if (ret!=M4_SUCCESS || IsNull(vTempMax)){  //si no hemos podido convertir lo ponemos a cero (compromiso)
		//liberamos las posibles cadenas
		DeleteVarString (vTempMax);
		//ponemos el minimo
        vTempMax.Data.DoubleData = LONG_MIN;
        vTempMax.Type=M4CL_CPP_TYPE_NUMBER;
        bNull=M4_TRUE;
    } 
    
    //vamos hallando el máximo del resto de arg.
    for (i=0; i < (unsigned int)vTempNum.Data.DoubleData-1; i++){
        ret=RuntimePop(vTemp1);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Max.")	;
		
        if (IsNull(vTemp1)) 
			continue; //ignoramos los nulos
        
        if (vTemp1.Type==M4CL_CPP_TYPE_STRING_POOL ){
            ret=m_oVariantMediator->ToDouble (vTemp1);
        }
        if (ret!=M4_SUCCESS){  //si no hemos podido convertir ignoramos ese elemento y seguimnos
			//liberamos las posibles cadenas
			DeleteVarString (vTemp1);
            continue;            
        }
		
        bNull=M4_FALSE;        //hemos leido algun no nulo o no cadena
        vTempMax.Data.DoubleData =  (vTempMax.Data.DoubleData < vTemp1.Data.DoubleData ? vTemp1.Data.DoubleData : vTempMax.Data.DoubleData );
    }
	
    if (bNull){
        RuntimePushNull();
    }
    else{
        RuntimePush (vTempMax);
    }
	
    return M4_SUCCESS;
	
}

//---------------------------------------------------------
//- M I N I M O
//---------------------------------------------------------
m4return_t  ClVM0General::Min             (void){
    
    m4VariantType vTempNum, vTemp1, vTempMin;
    m4uint32_t   i;
    m4return_t  ret;
    m4bool_t    bNull=M4_FALSE;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Min");
#endif
    //numero de argumentos
    ret=RuntimePop(vTempNum);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Min.")	;
    
    //sacamos el primer elemento y comprobamos tipo y de momento ese es el minimo
    ret=RuntimePop(vTempMin);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Min.")	;
    
    if (vTempMin.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTempMin);
    }
    if (ret!=M4_SUCCESS || IsNull(vTempMin)){  //si no hemos podido convertir lo ponemos al maximo de un long(compromiso)
		//liberamos las posibles cadenas
		DeleteVarString (vTempMin);
		//ponemos el maximo
		vTempMin.Data.DoubleData=LONG_MAX;
        vTempMin.Type=M4CL_CPP_TYPE_NUMBER;
        bNull=M4_TRUE;
    }
    
    //vamos hallando el mínimo del resto de arg.
    for (i=0; i < (unsigned int)vTempNum.Data.DoubleData-1; i++){
        ret=RuntimePop(vTemp1);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Min.")	;
		
        if (IsNull(vTemp1)) continue; //ignoramos los nulos
		
        if (vTemp1.Type==M4CL_CPP_TYPE_STRING_POOL){
            ret=m_oVariantMediator->ToDouble (vTemp1);
        }
        if (ret!=M4_SUCCESS){  //si no hemos podido convertir ignoramos ese elemento y seguimnos
			//liberamos las posibles cadenas
			DeleteVarString (vTemp1);
            continue;
            
        }
        bNull=M4_FALSE;        //hemos leido algun no nulo o no cadena
        vTempMin.Data.DoubleData =  (vTempMin.Data.DoubleData > vTemp1.Data.DoubleData ? vTemp1.Data.DoubleData : vTempMin.Data.DoubleData );
    }
	
    if (bNull){
        RuntimePushNull();
    }
    else{
        RuntimePush (vTempMin);
    }
    
    return M4_SUCCESS;
}


//---------------------------------------------------------
//- A B S O L U T O
//---------------------------------------------------------
m4return_t  ClVM0General::Abs             (void){
    
    m4VariantType vTemp;
    m4return_t      ret=M4_SUCCESS;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Abs");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Abs.")	;
	
    if (IsNull(vTemp)){ //Null behaviour:  nulo->Nulo
        RuntimePushNull();
        return M4_SUCCESS;
    }
    
    if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTemp);
    }    
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp);
		RuntimePushDouble(0.0);
    }
    else{
        vTemp.Data.DoubleData = fabs(vTemp.Data.DoubleData);
        RuntimePush (vTemp);
    }
    
    return M4_SUCCESS;   
    
}

//---------------------------------------------------------
//-D A T E   3 0
//---------------------------------------------------------
m4return_t  ClVM0General::Date30          (void){
    
    m4VariantType vTemp;
    m4return_t      ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Date30");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Date30.")	;
    
	if ( CheckDateIfErrorPush (vTemp, 0.0) )   {	//Ok
        RuntimePushDate( nfecha30(vTemp.Data.DoubleData, 0) );
    }    
    return M4_SUCCESS;   
    
}

//---------------------------------------------------------
//-D A T E   3 0 E X
//---------------------------------------------------------
m4return_t  ClVM0General::Date30Ex(void)
{

    m4return_t		iResult = M4_SUCCESS ;
	m4int32_t		i = 0 ;
	m4int32_t		iNumArgs = 0 ;
	m4int32_t		iFebruaryAs30 = 1 ;
	m4VariantType	vNumArgs ;
	m4VariantType	vFebruaryAs30 ;
	m4VariantType	vDate ;

	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Date30Ex");
#endif

	//número de argumentos
	iResult= RuntimePopNumber( vNumArgs ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Date30Ex" ) ;
	iNumArgs = m4int32_t( vNumArgs.Data.DoubleData ) ;

    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if( iNumArgs > 2 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "Date30Ex" << 2 << iNumArgs << DumpCallStack() ) ;
        //sacamos los que sobran
        for( i = 0 ; i < iNumArgs - 2 ; i++ )
		{
            iResult = RuntimePopAndDelete() ;
            DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Date30Ex" ) ;
        }
        iNumArgs = 2 ; //ya hemos sacado los sobrantes
    }

	if( iNumArgs == 2 )
	{
		iResult = RuntimePopNumber( vFebruaryAs30 ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FebruaryAs30" << "Date30Ex" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vFebruaryAs30.Type ) << DumpCallStack() ) ;
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

    iResult = RuntimePop( vDate ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Date30Ex." ) ;
    
	if( CheckDateIfErrorPush( vDate, 0.0 ) )
	{	//Ok
        RuntimePushDate( nfecha30( vDate.Data.DoubleData, iFebruaryAs30 ) ) ;
    }

    return( M4_SUCCESS ) ;
}

//---------------------------------------------------------
//-A D D   M O N T H S
//---------------------------------------------------------
m4return_t  ClVM0General::AddMonths       (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: AddMonths");
#endif
    //No.meses	(2o. arg)
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "AddMonths.")	;
    
    //Fecha		(1er arg)
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "AddMonths.")	;

	if ( !CheckDateIfErrorPush (vTemp1, 0.0) ) {	//error
		DeleteVarString (vTemp2);
        return M4_SUCCESS;
    }
		
	if ( CheckNumberIfErrorPush (vTemp2, vTemp1, M4_TRUE) ) {	//Ok
		vTemp1.Data.DoubleData = nsumameses(vTemp1.Data.DoubleData , (int) vTemp2.Data.DoubleData ) ;
		RuntimePush (vTemp1);
	}
	
    return M4_SUCCESS;    
}

//---------------------------------------------------------
//-A D D   Y E A R S 
//---------------------------------------------------------
m4return_t  ClVM0General::AddYears        (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: AddYears");
#endif
    //Años	(Number) 2o. arg
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "AddYears.")	;
    
    //Fecha	(Date)	 1o. arg
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "AddYears.")	;
    
	if ( !CheckDateIfErrorPush (vTemp1, 0.0) ) {		//error
		DeleteVarString (vTemp2);
        return M4_SUCCESS;
    }
    
	if ( CheckNumberIfErrorPush (vTemp2, vTemp1, M4_TRUE) ) {	//Ok
		vTemp1.Data.DoubleData = nsumaannos(vTemp1.Data.DoubleData , (int) vTemp2.Data.DoubleData ) ;
		RuntimePush (vTemp1);
	}
    return M4_SUCCESS;
    
}

//---------------------------------------------------------
//-F R A C T I O N
//---------------------------------------------------------
m4return_t  ClVM0General::Fraction        (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    m4double_t  kk;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Fraction");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Fraction.")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) )   {
        vTemp.Data.DoubleData = modf(vTemp.Data.DoubleData, &kk);
        RuntimePushDouble (vTemp.Data.DoubleData );
    }    
    return M4_SUCCESS;   
    
}


//---------------------------------------------------------
//-B O U N D
//---------------------------------------------------------
m4return_t  ClVM0General::Bound           (void){
    
    m4VariantType vTemp1, vTemp2, vTemp3;
    m4return_t  ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Bound");
#endif
    //tercer arg-maximo
    ret=RuntimePop(vTemp3);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Bound.")	;
    
    //(seg. arg)-mimimo
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Bound.")	;
    
    //1er arg - Valor a acotar
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Bound.")	;
	
    if (IsNull(vTemp1) || IsNull(vTemp2) || IsNull(vTemp3) ){ //Null behaviour:  nulo->Nulo
		//liberamos las posibles cadenas
		DeleteVarString (vTemp1);
		DeleteVarString (vTemp2);
		DeleteVarString (vTemp3);
        RuntimePushNull();
        return M4_SUCCESS;
    }
	
    //tipo del 1er arg
    if (vTemp1.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTemp1);
    }
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp3);
		DeleteVarString (vTemp2);
		DeleteVarString (vTemp1);
        RuntimePushDouble(0.0); //metemos un 0 como resultado
        return M4_SUCCESS;      //retornamos OK aunque no hallamos calculado
    }
    
    //tipo del 2do arg
    if (vTemp2.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTemp2);
    }
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp2);
		DeleteVarString (vTemp3);
        RuntimePushDouble(0.0); //metemos un 0 como resultado
        return M4_SUCCESS;      //retornamos OK aunque no hallamos calculado
    }
    
    //tipo del 3er arg
    if (vTemp3.Type==M4CL_CPP_TYPE_STRING_POOL){
        ret=m_oVariantMediator->ToDouble (vTemp3);
    }
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp3);
        RuntimePushDouble(0.0); //metemos un 0 como resultado
        return M4_SUCCESS;      //retornamos OK aunque no hallamos calculado
    }
	
    vTemp1.Data.DoubleData = __min (__max (vTemp1.Data.DoubleData ,  vTemp2.Data.DoubleData ) , vTemp3.Data.DoubleData) ;
    
    RuntimePush (vTemp1);
    return M4_SUCCESS;       
    
}

//---------------------------------------------------------
//-D A Y S   O F   Y E A R 
//---------------------------------------------------------
m4return_t  ClVM0General::DaysOfYear      (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DaysOfYear");
#endif
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DaysOfYear.")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) )   {
        RuntimePushDouble (ndiasdelano ((m4int32_t)vTemp.Data.DoubleData));
    }    
    return M4_SUCCESS;   
}

//---------------------------------------------------------
//-D A Y S   O F   M O N T H
//---------------------------------------------------------
m4return_t  ClVM0General::DaysOfMonth     (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
	
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DaysOfMonth.")	;
    
	if ( CheckNumberIfErrorPush (vTemp, 0.0) )   {
        RuntimePushDouble (ndiasdelmes (vTemp.Data.DoubleData));
    }    
    return M4_SUCCESS;       
}

//---------------------------------------------------------
//-P O W E R
//---------------------------------------------------------
m4return_t  ClVM0General::Power           (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Power");
#endif
    //potencia a la que elevamos
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Power.")	;
    
    //número a elevar
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Power.")	;
	
	if ( !CheckNumberIfErrorPush (vTemp1, 0.0) ) {	//error
		DeleteVarString (vTemp2);
        return M4_SUCCESS;
    }
    
	if ( CheckNumberIfErrorPush (vTemp2, 0.0) ) {	//Ok               
		vTemp1.Data.DoubleData = pow(vTemp2.Data.DoubleData , vTemp1.Data.DoubleData ) ;
		RuntimePush (vTemp1);
	}
    
	return M4_SUCCESS;    
}

//---------------------------------------------------------
//-T O D A Y  
//---------------------------------------------------------
m4return_t  ClVM0General::Today           (void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Today");
#endif
    RuntimePushDate(nhoy(M4CL_TIME_LOCAL_TIME));	//Ojo: No Devuelve la hora como se esperaba!!!
    return M4_SUCCESS;   
    
}

//---------------------------------------------------------
//-T O D A Y  NOW
//---------------------------------------------------------
m4return_t  ClVM0General::TodayNow        (void){
	
#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: TodayNow");
#endif

    RuntimePushDate(ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME));
    return M4_SUCCESS;   
    
}
//---------------------------------------------------------
//-T O D A Y  GMT
//---------------------------------------------------------
m4return_t  ClVM0General::TodayGMT        (void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: TodayGMT");
#endif
    RuntimePushDate(nhoy());	//Ojo: No Devuelve la hora como se esperaba!!!
    return M4_SUCCESS;   
    
}

//---------------------------------------------------------
//-T O D A Y  NOW GMT
//---------------------------------------------------------
m4return_t  ClVM0General::TodayNowGMT     (void){
	
#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: TodayNowGMT");
#endif
    RuntimePushDate(ClActualDate());
    return M4_SUCCESS;   
    
}
//---------------------------------------------------------
//-P O R C E N T A G E
//---------------------------------------------------------
m4return_t  ClVM0General::Percentage       (void){
    
    m4VariantType vTemp1, vTemp2;
    m4return_t  ret=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Percentage");
#endif
    //total (2o. param)
    ret=RuntimePop(vTemp1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Percentage.")	;
    
    //tanto pot ciento a hallar (1o. param)
    ret=RuntimePop(vTemp2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Percentage.")	;
	
    if ( !CheckNumberIfErrorPush (vTemp1, 0.0) ) {	//error
		DeleteVarString (vTemp2);
		return M4_SUCCESS;
	}
	
    if ( CheckNumberIfErrorPush (vTemp2, 0.0) ) {	//Ok
		//si el total es 0
		if (vTemp1.Data.DoubleData==0.0){
			RuntimePushDouble(0.0);
		}
		else{
			RuntimePushDouble( (vTemp1.Data.DoubleData * vTemp2.Data.DoubleData ) / 100 );
		}        
	}
    return M4_SUCCESS;
	
}


//---------------------------------------------------------
//-T O  I N T
//---------------------------------------------------------
m4return_t  ClVM0General::ToInt              (void){
    
    m4VariantType   vTemp;
    m4return_t      ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ToInt");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToInt.")	;
	
	if ( CheckNumberIfErrorPush (vTemp, 0.0) )   {    
		vTemp.Data.DoubleData = (int)vTemp.Data.DoubleData;
        RuntimePush(vTemp);
    }
    return M4_SUCCESS;       
}

//---------------------------------------------------------
//-T O    D O U B L E
//---------------------------------------------------------
m4return_t  ClVM0General::ToDouble           (void){
    
    m4VariantType   vTemp;
    m4return_t      ret;
	
#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ToDouble");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToDouble.")	;
    
	if ( CheckNumberIfErrorPush (vTemp, 0.0) )   {        
        RuntimePush(vTemp);
    }
    return M4_SUCCESS;       
}

//---------------------------------------------------------
//-T O   S T R I N G
//---------------------------------------------------------
m4return_t  ClVM0General::ToString           (void){
    
    m4VariantType   vTemp, vType, vNumArg;
    m4return_t      ret;
    m4uint32_t      i;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ToString");
#endif
    //sacamos el número de argumentos
    ret=RuntimePop (vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToStringS.")	;
    
    if (vNumArg.Data.DoubleData >1 ){
		//eliminamos los posibles argumentos extras
        for (i=0; i < vNumArg.Data.DoubleData-2; ++i){            
            ret=RuntimePopAndDelete ();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToString.")	;
		}
		//sacamos el tipo de conversión que deseamos 
		ret=RuntimePop (vType);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToString.")	;
		//comprobación de tipo
		if (m_oVariantMediator->ToDouble (vType)!=M4_SUCCESS){
			
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Precission" << "ToString" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR; //salimso parando la ejecución
		}
    }
    else{
		//Tipo por defecto:	6 decimales
        vType.Data.DoubleData=6.0;
    }
    
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToString.")	;
    
    if (IsNull (vTemp) ){       //Null behaviour: nulo->Nulo
        RuntimePushString ("");
        return M4_SUCCESS;
    }
	
    m4char_t  pszTemp[M4CL_MAX_STRING];
    
    m4uint32_t iYear  ;
    m4uint32_t iMonth ;
    m4uint32_t iDay   ;
	
    // Es una FECHA con SHORT FORMAT
    if (vTemp.Type == M4CL_CPP_TYPE_DATE &&
        vType.Data.DoubleData==M4_SHORT_DATE_FORMAT){ //short format	DATE
        
        iYear  = nano( vTemp.Data.DoubleData) ;
		if ( iYear > M4CL_PLUS_INFINITE_YEAR ){	//para evitar años de 5 cifras que provocan memory overrun. 

			DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, M4CL_PLUS_INFINITE_DATE<<"ToString"<< DumpCallStack(&m_oState) );
			
			iYear = M4CL_PLUS_INFINITE_YEAR ;
			iMonth = 1 ;
			iDay = 1;
		}
		else{
			iMonth = nmes( vTemp.Data.DoubleData) ;
			iDay   = ndia( vTemp.Data.DoubleData) ;
		}

        sprintf( pszTemp, "%04d-%02d-%02d", iYear, iMonth, iDay ) ;
        vTemp.Type=M4CL_CPP_TYPE_STRING_POOL; 
        ret=m_oDynamicPool->AddString(pszTemp, vTemp.Data.CharData);                
    }
	// Es un NUMERO
    else if (vTemp.Type == M4CL_CPP_TYPE_NUMBER){	// Numero
		// Si la precision es "exagerada" pasamos... para evitar GP
		if ( ((int)vType.Data.DoubleData) > 8 ) {
			vType.Data.DoubleData = 8.0;	//Precision exagerada => ponemos 8
		}
		// Ajuste a punto gordo, para evitar las perdidas de precision del modf
		vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);

        sprintf (pszTemp,"%.*f",(m4uint32_t)vType.Data.DoubleData, vTemp.Data.DoubleData);
        vTemp.Type=M4CL_CPP_TYPE_STRING_POOL; 
        ret=m_oDynamicPool->AddString(pszTemp, vTemp.Data.CharData);
    }
	// Es una CADENA o fecha larga
    else {
        ret=m_oVariantMediator->ToPoolString(vTemp);
		if (m_oVariantMediator->IsDateOverflow() && ret==M4_SUCCESS){ //si hay overflow de fecha i hizo la conversión (era un fecha, sino no pude haber dateoverflow)
			DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, m_oDynamicPool->GetString(vTemp.Data.CharData)<<"ToString"<< DumpCallStack(&m_oState) );
		}
    }
    
    if (ret==M4_ERROR){
        RuntimePushDouble(0.0);
        return M4_SUCCESS;
    }
    
    RuntimePush(vTemp);
	
    return M4_SUCCESS;       
}

//---------------------------------------------------------
//-T O   D A T E
//---------------------------------------------------------
m4return_t  ClVM0General::ToDate             (void){
    
    m4VariantType   vTemp;
    m4return_t      ret;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ToDate");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ToDate.")	;
    
	if ( CheckDateIfErrorPush (vTemp, 0.0) )   {
		if (m_oVariantMediator->IsDateOverflow()){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, DateLog(vTemp.Data.DoubleData)<<"ToDate"<< DumpCallStack(&m_oState) );
		}
        RuntimePush(vTemp);
    }
    return M4_SUCCESS;       
    
}


//---------------------------------------------------------
//-T R U N C A T E
//---------------------------------------------------------
m4return_t  ClVM0General::Truncate             (void){
    
    m4VariantType   vTemp, vScale;
    m4return_t      ret;

    m4int32_t   aiDate[6], aiInit[6]={1800, 1, 1, 0, 0, 0};
    int         day, month, year;
    m4int32_t   iLon, iPrec;
	size_t		iChars = 0 ;
	size_t		iSize = 0 ;
	int			iPosition = 0 ;
    
    
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Truncate");
#endif
    //sacamos la escala 
    ret=RuntimePop (vScale);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Truncate.")	;
	
    //valor a convertir
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Truncate.")	;
	
    if (vScale.Type!=M4CL_CPP_TYPE_NUMBER){
		//liberamos las posibles cadenas
		DeleteVarString (vScale);
		DeleteVarString (vTemp);
        RuntimePushDouble (0.0); //fallo, aunque seguimos
        return M4_SUCCESS;
    }
    
    if (IsNull (vTemp) ){       //Null behaviour: Valor nulo->Nulo
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	    

	iPrec = (m4int32_t)vScale.Data.DoubleData;

    switch (vTemp.Type)
    {
    case M4CL_CPP_TYPE_STRING_POOL:
        {
			m4char_t *pszTemp, *pszStr;
            
            pszStr=m_oDynamicPool->GetString (vTemp.Data.CharData);
            if (pszStr==NULL) break;            
			iLon=m_oDynamicPool->GetStringLength (vTemp.Data.CharData);
			
			// UNICODE LN4
			iChars = M4StrLen( pszStr, iLon ) ;

            if (abs((m4int_t)iPrec) > iChars) {
                iPrec=iChars;
            }
            
            if (iPrec <0 ){ //truncamos por detras
				
				// UNICODE LN4
				iPosition = M4CharPosition( pszStr, iLon, iChars + iPrec, iSize ) ;
				iPrec = iPosition - iLon;

                pszTemp=new char[-iPrec+1];
                strcpy (pszTemp, pszStr +iLon + iPrec  );
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
                m_oDynamicPool->AddString (pszTemp,vTemp.Data.CharData);
                delete [] pszTemp;
            }
            else{   //por delante
                
				// UNICODE LN4
				iPosition = M4CharPosition( pszStr, iLon, iPrec, iSize ) ;
				iPrec = iPosition ;

                pszTemp=new char[iPrec+1];
                strncpy (pszTemp, pszStr , iPrec);
                //truncamos
                pszTemp[iPrec]=0;
                
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
                m_oDynamicPool->AddString (pszTemp,vTemp.Data.CharData);
                delete [] pszTemp;
            }
            
            break;
        }          
    case M4CL_CPP_TYPE_NUMBER:
        {
            if (iPrec >8){
                break;
            }

			// Solucion usando "modf" sobre la parte fraccional ajustada y convertida a entera aplicando la escala
			m4double_t	dFractional, dInteger, dBase;

			// Ajuste a punto gordo, para evitar las perdidas de precision del modf
			vTemp.Data.DoubleData = M4VMDoubleAdjust(vTemp.Data.DoubleData);

			if (iPrec >0)
			{
				dBase = pow( 10.0, (m4int_t)iPrec );
				dFractional = modf( vTemp.Data.DoubleData, &dInteger ) ;
				modf( dFractional * dBase, &vTemp.Data.DoubleData ) ;
				vTemp.Data.DoubleData = vTemp.Data.DoubleData / dBase + dInteger;
			}
			else
			{
				dBase = pow( 10.0, (m4int_t)-iPrec );
				modf( vTemp.Data.DoubleData / dBase, &dInteger ) ;
				vTemp.Data.DoubleData = dInteger * dBase ;
			}			

            break;            
        }
        
    case M4CL_CPP_TYPE_DATE:	
        {
            if (iPrec >6 || iPrec <=0) {
                break;
            }
            //convertimos a números
            ClDateToNumbers (vTemp.Data.DoubleData, year, month, day, aiDate[3], aiDate[4], aiDate[5]);
            aiDate[0]=year;
            aiDate[1]=month;
            aiDate[2]=day;
			
            //iniciamos los que debamos según la precisión
            for (m4int32_t i=iPrec; i< 6; i++){
                aiDate[i]=aiInit[i];
            }
            
            //convertimos a double
            ClNumbersToDate (aiDate[0], aiDate[1], aiDate[2], aiDate[3], aiDate[4], aiDate[5], vTemp.Data.DoubleData);
            break;
        }
    }
    
    RuntimePush(vTemp);
    return M4_SUCCESS;       
	
}

//---------------------------------------------------------
//-D A T E   T O   N U M B E R S
//---------------------------------------------------------
m4return_t  ClVM0General::DateToNumbers             (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    int  years, months, days;
    m4int32_t  hours, minutes, seconds;	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DateToNumbers");
#endif
    //sacamos los 3 parametros por referencia y los ignoramos, el 4 debe ser Date
	for (m4int32_t i=0; i<3; ++i) {
		ret=RuntimePopAndDelete();
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateToNumbers.")	;
	}
	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateToNumbers.")	;

    if (IsNull (vTemp) ){       //Null behaviour: Valor nulo->Nulo
        RuntimePushNull (); //res
        RuntimePushNull (); //year
        RuntimePushNull (); //month
        RuntimePushNull (); //day
        return M4_SUCCESS;
    }
	
    if (vTemp.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vTemp);
    }
    
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp);
        RuntimePushDouble(0.0); //res
        RuntimePushDouble(0.0); //year
        RuntimePushDouble(0.0); //month
        RuntimePushDouble(0.0); //day
        return M4_SUCCESS;
    }
    
    //convertimos a dia, mes , año
    ClDateToNumbers (vTemp.Data.DoubleData, years, months, days, hours, minutes, seconds);
	
    //metemos como resultado un M4_SUCCESS
    RuntimePushDouble (M4_SUCCESS);
	
    //metemos años, meses y dias 
    RuntimePushDouble (days);
    RuntimePushDouble (months);
    RuntimePushDouble (years);
	
    return M4_SUCCESS;   
	
}

//---------------------------------------------------------
//-N U M B E R S   T O  D A T E
//---------------------------------------------------------
m4return_t  ClVM0General::NumbersToDate             (void){
    
    m4VariantType vTempNum, vTemp1;
    m4return_t  ret=M4_SUCCESS;
    m4int32_t   aiDate[6]={1800, 1, 1, 0, 0, 0};
    m4uint32_t  i;
    m4date_t    dFecha;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: NumbersToDate");
#endif
    //numero de argumentos
    ret=RuntimePop(vTempNum);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "NumbersToDate.")	;
	
    if (vTempNum.Data.DoubleData >6.0) {
		vTempNum.Data.DoubleData -= 6.0;
		//sacamos los argumentos que sobran y los ignoramos
		for (i=0; i< (unsigned int) vTempNum.Data.DoubleData; ++i) {
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateToNumbers.")	;
		}
		vTempNum.Data.DoubleData = 6.0; //para porsi
	}
	//sacamos los argumentos
    for (i=0; i< (unsigned int) vTempNum.Data.DoubleData ; i++){
		
        //sacamos elemento y comprobamos tipo 
        ret=RuntimePop(vTemp1);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "NumbersToDate.")	;
		
        if (vTemp1.Type!=M4CL_CPP_TYPE_NUMBER){
            ret=m_oVariantMediator->ToDouble (vTemp1);
        }
        if (ret==M4_SUCCESS){  //si no hemos podido convertir lo ignoramos
            aiDate[(unsigned int) vTempNum.Data.DoubleData -i -1] = int (vTemp1.Data.DoubleData);
        } else {
			//liberamos las posibles cadenas
			DeleteVarString (vTemp1);
		}
        
    }
	
    //convertimos a fecha
    ClNumbersToDate (aiDate[0], aiDate[1] , aiDate[2] , aiDate[3] , aiDate[4] , aiDate[5] , dFecha);
    
    //metemos resultado
    RuntimePushDate (dFecha);
    
    return M4_SUCCESS;
    
}

//---------------------------------------------------------
//-D A T E   T O   A L L   N U M B E R S
//---------------------------------------------------------
m4return_t  ClVM0General::DateToAllNumbers             (void){
    
    m4VariantType vTemp;
    m4return_t  ret=M4_SUCCESS;
    int  years, months, days;
    m4int32_t  hours, minutes, seconds;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DateToAllNumbers");
#endif
	//sacamos los 6 parametros por referencia y los ignoramos, el 7 es Date
	for (m4int32_t i=0; i<6; ++i) {
		ret=RuntimePopAndDelete();
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateToAllNumbers.")	;
	}
	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateToAllNumbers.")	;
	
    if (IsNull (vTemp) ){       //Null behaviour: Valor nulo->Nulo
        RuntimePushNull (); //res
        RuntimePushNull (); //year
        RuntimePushNull (); //month
        RuntimePushNull (); //day
        RuntimePushNull (); //hour
        RuntimePushNull (); //minute
        RuntimePushNull (); //second
		
        return M4_SUCCESS;
    }
	
    if (vTemp.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vTemp);
    }
    
    if (ret!=M4_SUCCESS){
		//liberamos las posibles cadenas
		DeleteVarString (vTemp);
        RuntimePushDouble(0.0); //res
        RuntimePushDouble(0.0); //year
        RuntimePushDouble(0.0); //month
        RuntimePushDouble(0.0); //day
        RuntimePushDouble(0.0); //hour
        RuntimePushDouble(0.0); //minute
        RuntimePushDouble(0.0); //sec
        return M4_SUCCESS;
    }
    
    //convertimos a dia, mes , año
    ClDateToNumbers (vTemp.Data.DoubleData, years, months, days, hours, minutes, seconds);
	
    //metemos como resultado un M4_SUCCESS
    RuntimePushDouble (M4_SUCCESS);
	
    //metemos años, meses y dias 
    RuntimePushDouble (seconds);
    RuntimePushDouble (minutes);
    RuntimePushDouble (hours);
    RuntimePushDouble (days);
    RuntimePushDouble (months);
    RuntimePushDouble (years);
    
    return M4_SUCCESS;   
}

m4return_t ClVM0General::Div(void){ //the remainder
	
	m4uint32_t		iDivider = 0 ;
	m4VariantType	vDivider;
	m4VariantType	vDividend;
	
	RuntimePop(vDivider);	//divisor
	RuntimePop(vDividend);	//dividendo
	
    if (IsNull (vDivider) || IsNull (vDividend) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vDivider);
		DeleteVarString (vDividend);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vDivider.Type!=M4CL_CPP_TYPE_NUMBER || vDividend.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "div" << M4ClCppType(vDividend.Type)<<M4ClCppType(vDivider.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vDivider);
		DeleteVarString (vDividend);
		return M4_ERROR;
	}
	
	/* Bug 0098813
	La comprobación de cero hay que hacerla después de hacer el casting a entero
	por si el número es decimal
	*/
	iDivider = (m4uint32_t)vDivider.Data.DoubleData ;

	//double 
	if (iDivider==0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_DIV_BY_ZERO, DumpLine(&m_oState) << DumpCallStack(&m_oState) );
		RuntimePushDouble (0.0);
	}
	else
	{
		/* Bug 0141947
		El casting hay que hacerlo a entero de 8 bytes para que no trunque
		*/
        RuntimePushDouble( m4uint32_t( m4uint64_t( vDividend.Data.DoubleData ) % iDivider ) ) ;
	}
	
	return M4_SUCCESS;
}

//
//Sign
//
m4return_t  ClVM0General::Sign(void){
	
    m4VariantType   vTemp;
    m4return_t      ret;
	
    m4double_t iRes;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Sign");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Sign.")	;
    
    if (vTemp.Type==M4CL_CPP_TYPE_NUMBER ||vTemp.Type==M4CL_CPP_TYPE_DATE){
        //vemos si es positivo o negativo
        iRes= (vTemp.Data.DoubleData==0.0 ? 0.0: (vTemp.Data.DoubleData<0.0 ? -1.0:1.0) ); 
    }
    else{
		//liberamos las posibles cadenas
		DeleteVarString (vTemp);  
        iRes= 0.0;
    }
    RuntimePushDouble (iRes);
    
    return M4_SUCCESS;
}



//
//Convert
//
m4return_t  ClVM0General::Convert(void){
	
    m4VariantType   vTemp ;
    m4return_t      ret = M4_ERROR ;
	m4int_t			iLength = -1 ;
	m4pchar_t		pcString = NULL ;
	m4pchar_t		pcStringOut = NULL ;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Convert");
#endif

	ret = RuntimeTop( vTemp ) ;
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Convert.")	;

    //si no es cadena devolvemos tal cual. no hacemos nada
	if( vTemp.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		pcString = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;

		ret = EncryptTwoWaySecondKey( pcString, strlen( pcString ) * sizeof( m4char_t ), M4CH_DUMMY_B1 + 23, strlen( M4CH_DUMMY_B1 + 23 ) * sizeof(m4char_t), pcStringOut, iLength ) ;

		// Si hay errores dejamos la cadena que había
		if( ret == M4_SUCCESS )
		{
			RuntimePopAndDelete() ;
			RuntimePushString( pcStringOut, vTemp.IsUser ) ;  
			delete pcStringOut ;
		}
	}

    return M4_SUCCESS;
}


//
//DataConvert
//
m4return_t  ClVM0General::DataConvert(void)
{
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: DataConvert");
#endif

	RuntimePopAndDelete() ;
	RuntimePushString( "Not implemented" ) ;  

    return M4_SUCCESS;
}


//
//KeyConvert
//
m4return_t  ClVM0General::KeyConvert(void){
	
    m4VariantType   vTemp ;
    m4return_t      ret = M4_ERROR ;
	m4int_t			iLength = -1;
	m4uint16_t		iLicenceLength = -1 ;
	m4pcchar_t		pccLicense = NULL ;
	m4pchar_t		pcString = NULL ;
	m4pchar_t		pcStringOut = NULL ;
	m4pchar_t		pcResult = NULL ;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM1: KeyConvert");
#endif

	ret = RuntimeTop( vTemp ) ;
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "KeyConvert.")	;

    //si no es cadena devolvemos tal cual. no hacemos nada
	if( vTemp.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		pcString = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;

		ret = EncryptTwoWay( pcString, strlen( pcString ) * sizeof( m4char_t ), M4CH_DUMMY_K + 23, strlen( M4CH_DUMMY_K + 23 ) * sizeof(m4char_t), pcStringOut, iLength ) ;

		// Si hay errores dejamos la cadena que había
		if( ret == M4_SUCCESS )
		{
			pccLicense = m_oState.m_poAccess->GetEnvironment()->GetUserLicense() ;

			iLicenceLength = strlen( pccLicense );
			pcResult = new  m4char_t[ iLicenceLength + iLength + 1 ] ;
			memcpy( pcResult, pccLicense, iLicenceLength ) ;
			memcpy( pcResult + iLicenceLength, pcStringOut, iLength + 1 ) ;
			delete pcStringOut;

			RuntimePopAndDelete() ;
			RuntimePushString( pcResult, vTemp.IsUser ) ;  
			delete pcResult;
		}
	}

    return M4_SUCCESS;
}



//
//ConvertOneWay
//
m4return_t  ClVM0General::ConvertOneWay(void){
	
    m4VariantType   vTemp;
    m4return_t      ret;
	m4pchar_t		pcString ;
	m4char_t		acString[ 2 * M4_MAX_LEN_MD5 + 1 ] ;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ConvertOneWay");
#endif

	ret = RuntimeTop( vTemp ) ;
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConvertOneWay.")	;

    //si no es cadena devolvemos tal cual. no hacemos nada
	if( vTemp.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		pcString = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;

		ret = M4ConvertOneWayMD5( pcString, acString, 2 * M4_MAX_LEN_MD5 ) ;

		// Si hay errores dejamos la cadena que había
		if( ret == M4_SUCCESS )
		{
			RuntimePopAndDelete() ;
			RuntimePushString( acString, vTemp.IsUser ) ;  
		}
	}

    return M4_SUCCESS;
}


//
//ConvertPassword
//
m4return_t  ClVM0General::ConvertPassword(void)
{

    m4return_t      ret;
	m4bool_t		bIsUser = M4_FALSE;
	m4pchar_t		pcUser;
	m4pchar_t		pcPassword;
	m4VariantType   vTemp;
	m4char_t		acString[ 2 * M4_MAX_LEN_ONE_WAY + 1 ];
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ConvertPassword");
#endif

	// Password
	ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Password" << "ConvertPassword" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;
    pcPassword = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
	bIsUser = vTemp.IsUser;

	// User
	ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "User" << "ConvertPassword" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;
    pcUser = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
	bIsUser = bIsUser || vTemp.IsUser;

	*acString = '\0' ;
	ret = M4ConvertPassword( pcUser, pcPassword, acString, 2 * M4_MAX_LEN_ONE_WAY ) ;

	if( ret != M4_SUCCESS )
	{
		RuntimePushNull() ;
		return M4_ERROR ;
	}

	RuntimePushString( acString, bIsUser ) ;
    return M4_SUCCESS ;
}


//
//ComparePassword
//
m4return_t  ClVM0General::_ComparePassword(m4bool_t ai_bIs2FA, m4pchar_t ai_pccFunctionName)
{

    m4return_t      iResult;
	m4bool_t		bIsUser = M4_FALSE;
	m4bool_t		bEqual;
	m4double_t		dReserved;
	m4pchar_t		pcUser;
	m4pchar_t		pcPassword;
	m4pchar_t		pcStoredPassword;
	m4pchar_t		pcNewPassword;
	m4pchar_t		pcVerificationCode;
    m4VariantType   vTemp;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: ComparePassword");
#endif

	bEqual = M4_FALSE ;
	pcNewPassword = NULL ;
	pcVerificationCode = NULL ;

	if( ai_bIs2FA == M4_TRUE )
	{
		// Verification code (output)
		iResult = RuntimePopAndDelete() ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "VerificationCode" << ai_pccFunctionName << DumpCallStack(&m_oState) ) ;
	}

	// New Password (output)
	iResult = RuntimePopAndDelete() ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "NewPassword" << ai_pccFunctionName << DumpCallStack(&m_oState) ) ;

	// Reserved
	iResult = RuntimePop( vTemp ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Reserved" << ai_pccFunctionName << M4ClCppType(M4CL_CPP_TYPE_NUMBER) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;

	if( vTemp.Type == M4CL_CPP_TYPE_NUMBER )
	{
		dReserved = vTemp.Data.DoubleData ;
	}
	else
	{
		dReserved = 0 ;
	}

	// Stored Password
	iResult = RuntimePopString( vTemp ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "StoredPassword" << ai_pccFunctionName << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;
    pcStoredPassword = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
	
	// Password
	iResult = RuntimePopString( vTemp ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Password" << ai_pccFunctionName << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;
    pcPassword = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
	bIsUser = vTemp.IsUser;
	
	// User
	iResult = RuntimePopString( vTemp ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "User" << ai_pccFunctionName << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) << M4ClCppType(vTemp.Type) << DumpCallStack(&m_oState) ) ;
    pcUser = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;
	bIsUser = bIsUser || vTemp.IsUser;
	
	iResult = M4ComparePassword( pcUser, pcPassword, pcStoredPassword, dReserved, ai_bIs2FA, bEqual, pcNewPassword, pcVerificationCode ) ;

	if (iResult != M4_SUCCESS)
	{
		RuntimePushDouble( bEqual ) ;
		RuntimePushNull() ;
		return M4_ERROR ;
	}

	RuntimePushDouble( bEqual ) ;

	if( ai_bIs2FA == M4_TRUE )
	{
		if (pcVerificationCode != NULL)
		{
			RuntimePushString( pcVerificationCode, bIsUser ) ;
			delete ( pcVerificationCode ) ;
		}
		else
		{
			RuntimePushNull() ;
		}
	}

	if (pcNewPassword != NULL)
	{
		RuntimePushString( pcNewPassword, bIsUser ) ;
		delete ( pcNewPassword ) ;
	}
	else
	{
		RuntimePushNull() ;
	}

    return M4_SUCCESS;
}



//
//ComparePassword
//
m4return_t  ClVM0General::ComparePassword(void)
{
    return _ComparePassword( M4_FALSE, "ComparePassword" );
}


//
//Compare2FAPassword
//
m4return_t  ClVM0General::Compare2FAPassword(void)
{
    return _ComparePassword( M4_TRUE, "Compare2FAPassword" );
}


//
// GetServerValue
// Si estamos en server devuelve el valor de una variable de server
//
m4return_t  ClVM0General::GetServerValue(void){

    m4VariantType   vTemp;
    m4return_t      ret;
	m4pchar_t		pcServerValue;
	m4pchar_t		pcServerPath;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetServerValue");
#endif

	//arg2 ServerValue
    ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Server Value" << "GetServerValue" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<<DumpCallStack(&m_oState) );
	
    pcServerValue = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    M4CHECK_RES( pcServerValue!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Server Value" << "GetServerValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack(&m_oState) );

    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;

	//arg2 ServerPath
    ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Server Path" << "GetServerValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<< DumpCallStack(&m_oState) );

    pcServerPath = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    M4CHECK_RES( pcServerPath!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Server Path" << "GetServerValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack(&m_oState) );

    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;

	m4char_t	acBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	
	ret = GetVMCSContext()->GetServerValue( pcServerPath, pcServerValue, acBuffer, M4CH_MAX_TEMP_BUFFER ) ;
	
	
	if( ret != M4_SUCCESS ){
		RuntimePushString( "" ) ;
	}
	else{
		RuntimePushString( acBuffer ) ;
	}
	
	
    return( M4_SUCCESS ) ;
}



//
// GetRegistryValue
// Devuelve el valor de una variable del registry
//


m4return_t  ClVM0General::GetRegistryValue(void){

    m4VariantType   vTemp;
    m4return_t      ret;
	m4pchar_t		pcRegistryValue;
	m4pchar_t		pcRegistryPath;
    m4char_t		acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetRegistryValue");
#endif

	//arg2 RegistryValue
    ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Registry Value" << "GetRegistryValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<< DumpCallStack(&m_oState) );
	
    pcRegistryValue = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    M4CHECK_RES( pcRegistryValue!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Registry Value" << "GetRegistryValue" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<  DumpCallStack(&m_oState) );

    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;

	//arg2 RegistryPath
    ret = RuntimePopString( vTemp ) ;
    M4CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Registry Path" << "GetRegistryValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<< DumpCallStack(&m_oState) );

    pcRegistryPath = m_oDynamicPool->GetString( vTemp.Data.CharData ) ;
    M4CHECK_RES( pcRegistryPath!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Registry Path" << "GetRegistryValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack(&m_oState) );

    m_oDynamicPool->DeleteString( vTemp.Data.CharData ) ;


	// jcr. Si es la password le devolvemos cadena vacía y nos callamos
	if( strcmpi( pcRegistryValue, M4_INIT_DB_PWD ) == 0 )
	{
		RuntimePushString( "" ) ;
	    return( M4_SUCCESS ) ;
	}


	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	
	//Escenario:
	//En función de  si la ejecución está en Cliente o en Server o si tenemos SM o Server
		//Es Cliente y SM (SM parte cliente): Leeremos del registro de la entrada CLIENT.
		//Es Server y SM (SM parte server): Leeremos del registro de la entrada CLIENT.
		//Es Cliente y No es SM (Es cliente distribuido: DIC): Leeremos del registro de la entrada CLIENT.
		//Es Server y No es SM (Es server puro): Leeremos del registro de la entrada APPServer.
	m4char_t		acRegistryRelativePath[ M4CH_MAX_TEMP_BUFFER + 1 ] ;
	m4pchar_t		pRegistryRelativePath = NULL;
	m4uint32_t		oLen = 0;

	//Si la entrada empieza por ".\" significa que estamos buscando una entrada relativa. Por ejemplo .\CVM
	if( pcRegistryPath != NULL && strlen(pcRegistryPath) >= 2 && pcRegistryPath[0] == '.' && pcRegistryPath[1] == '\\' )
	{
		//Apuntamos al siguiente caracter despues de '\'.
		pRegistryRelativePath = pcRegistryPath + 2;

		ClVMCSContext	* pVMCSContext = GetVMCSContext ();
		
		if (pVMCSContext) {
			if ( (pVMCSContext->IsClient() == M4_FALSE) && (pVMCSContext->IsSM() == M4_FALSE) ) {
				M4ComposeRegistryPath (M4_NEW_REGEDIT_KEY_SERVER, pRegistryRelativePath, acRegistryRelativePath, M4CH_MAX_TEMP_BUFFER + 1, oLen);
			}
			else {
				M4ComposeRegistryPath (M4_NEW_REGEDIT_KEY_CLIENT, pRegistryRelativePath, acRegistryRelativePath, M4CH_MAX_TEMP_BUFFER + 1, oLen);
			}
			//Apuntamos a la entrada que se ha construido.
			if (acRegistryRelativePath) {
				pcRegistryPath = acRegistryRelativePath;
			}
		}
	}
	else
	{
		// Si viene la estructura de registro vieja se cambia por la nueva
		m4char_t	c = 0 ;
		int			iCompare = 0 ;
		m4pcchar_t	pattern = "SOFTWARE\\Meta4\\Mind\\3.X\\" ;
		size_t		iPatternLength = strlen( pattern ) ;

		if( strlen( pcRegistryPath ) > iPatternLength )
		{
			c = pcRegistryPath[ iPatternLength ] ;
			pcRegistryPath[ iPatternLength ] = '\0' ;
			iCompare = strcmpi( pcRegistryPath, pattern ) ;
			pcRegistryPath[ iPatternLength ] = c ;

			if( iCompare == 0 )
			{
				strcpy( acRegistryRelativePath, "SOFTWARE\\Meta4\\PeopleNet\\" ) ;
				strcat( acRegistryRelativePath, pcRegistryPath + iPatternLength ) ;
				pcRegistryPath = acRegistryRelativePath ;
			}
		}
	}
	
	if( GetRegKeyData( pcRegistryPath, pcRegistryValue, acTempBuffer ) == FALSE )
	{
		DUMP_CHLOG_ERRORF( M4_CH_OBJREG_PROPERTY_NOT_FOUND, pcRegistryValue << pcRegistryPath ) ;
		RuntimePushString( "" ) ;
	}
	else
	{
		RuntimePushString( acTempBuffer ) ;
	}

    return( M4_SUCCESS ) ;
}



//bitwise operations
m4return_t  ClVM0General::BitwiseOr			(void){
	m4VariantType vLeft;
	m4VariantType vRight;
	
	RuntimePop(vLeft); 
	RuntimePop(vRight);
	
    if (IsNull (vLeft) || IsNull (vRight) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vLeft.Type!=M4CL_CPP_TYPE_NUMBER || vRight.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "BitwiseOr" <<M4ClCppType(vLeft.Type)<<M4ClCppType(vRight.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
		return M4_ERROR;
	}

	m4uint32_t iLeft =(m4uint32_t)  vLeft.Data.DoubleData;
	m4uint32_t iRight=(m4uint32_t) vRight.Data.DoubleData;

	RuntimePushDouble (iRight|iLeft);
	
	return M4_SUCCESS;

}

m4return_t  ClVM0General::BitwiseAnd		(void){
	m4VariantType vLeft;
	m4VariantType vRight;
	
	RuntimePop(vLeft); 
	RuntimePop(vRight);
	
    if (IsNull (vLeft) || IsNull (vRight) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vLeft.Type!=M4CL_CPP_TYPE_NUMBER || vRight.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "BitwiseAnd" << M4ClCppType(vLeft.Type)<<M4ClCppType(vRight.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
		return M4_ERROR;
	}

	m4uint32_t iLeft =(m4uint32_t)  vLeft.Data.DoubleData;
	m4uint32_t iRight=(m4uint32_t) vRight.Data.DoubleData;

	RuntimePushDouble (iRight&iLeft);
	
	return M4_SUCCESS;
}

m4return_t  ClVM0General::BitwiseNot		(void){
	m4VariantType vRight;
	
	RuntimePop(vRight);
	
    if (IsNull (vRight) ){ //Null behaviour: Algun nulo->Nulo
		DeleteVarString (vRight);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vRight.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "operand"<<"BitwiseNot" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vRight.Type)<< DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vRight);
		return M4_ERROR;
	}

	m4uint32_t iRight=(m4uint32_t) vRight.Data.DoubleData;

	RuntimePushDouble (~iRight);
	
	return M4_SUCCESS;
}

m4return_t  ClVM0General::BitwiseXor		(void){
	m4VariantType vLeft;
	m4VariantType vRight;
	
	RuntimePop(vLeft); 
	RuntimePop(vRight);
	
    if (IsNull (vLeft) || IsNull (vRight) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vLeft.Type!=M4CL_CPP_TYPE_NUMBER || vRight.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "BitwiseXor" << M4ClCppType(vLeft.Type)<<M4ClCppType(vRight.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vLeft);
		DeleteVarString (vRight);
		return M4_ERROR;
	}

	m4uint32_t iLeft =(m4uint32_t)  vLeft.Data.DoubleData;
	m4uint32_t iRight=(m4uint32_t) vRight.Data.DoubleData;

	RuntimePushDouble (iRight^iLeft);
	
	return M4_SUCCESS;

}

	//-------------
	//Variant Type
	//------------
m4return_t  ClVM0General::GetVariantType	(void){


	m4VariantType vTemp;

	RuntimePop (vTemp);
	DeleteVarString (vTemp);	// Bug 0274670
	RuntimePushDouble (vTemp.Type & 0x0F); 
		
		//el & es para anular los 4 bits + significativos 
		//en esos 4 bits es donde se guarda si las cadenas son de pool o var.
		//al anularlo las dejamos como solo string
					

	return M4_SUCCESS;
}

	//-------------
	//GetUniqueID
	//------------
m4return_t  ClVM0General::GetUniqueID	(void){

	//pedimos una KEY
	m4char_t   acKey[M4CL_VM_UUID_KEY_SIZE];
	m4return_t ret=GetUUID(acKey,M4CL_VM_UUID_KEY_SIZE);

	/* Bug 0115589
	Se trunca la cadena a 20
	No hay problema porque M4CL_VM_UUID_KEY_SIZE = 25 y eso no puede cambiar
	porque fallaría por debajo
	*/
	acKey[ 20 ] = '\0' ;

	//a la pila
	if (ret==M4_SUCCESS){
		RuntimePushString(acKey);
	}
	else{
		RuntimePushNull();
	}
	return M4_SUCCESS;
}


	//---------------
	//Rnd
	//---------------
	//Función que genera un número pseudoaleatorio entre 0 y el argumento

m4return_t  ClVM0General::Rnd	(void){

	m4VariantType vMax;
	m4return_t ret;


	//sacamos el argumentos
	ret=RuntimePop(vMax);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Rnd.")	;

	if (IsNull(vMax)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
    
	//comprobación de tipo
	if (m_oVariantMediator->ToDouble (vMax)!=M4_SUCCESS){

		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "MaxValue" << "Rnd" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vMax.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }

	// fix bug 0178286 : obsoletes 0080865
	typedef set<m4iden_t> tSetThreadId;
	typedef tSetThreadId::iterator tSetThreadIdIterator;

	static ClMutex s_oLocalMutex(M4_TRUE);
	static tSetThreadId s_SetThreadId;
	static m4uint8_t s_iInitCounter = 0;
	m4bool_t bInit = M4_FALSE;
	m4iden_t iThreadId = M4Thread::GetCurrentIdThread();

	{
		// we now synchronize rand() to fix bug 0178286
		ClMutBlock oMutBlock(&s_oLocalMutex);

		// gets the Thread Id
		tSetThreadIdIterator oIt = s_SetThreadId.find(iThreadId);
		if ( oIt == s_SetThreadId.end() )
		{
			// no element found so we have to initialize it
			s_SetThreadId.insert(iThreadId);
			bInit = M4_TRUE;
		}
	}


	//pedimos un aleatorio
	if (bInit == M4_TRUE)
	{
		m4uint32_t uSeed = (unsigned int) ClPrecisionTicker::GetTimeTick();
		uSeed = (uSeed & 0xFFF0) + (++s_iInitCounter);
		srand( uSeed );
	}

	double iRnd = (double)rand();	

	//lo ponemos en el rango 0-vMax	y al saco, digo a la pila
	RuntimePushDouble( (iRnd/(double)RAND_MAX) * vMax.Data.DoubleData);
	return M4_SUCCESS;
}

//-----------------------------------------------
//DateAdd-aun no operativa
//-----------------------------------------------
m4return_t  ClVM0General::DateAdd			(void){

   	m4return_t ret;
	m4VariantType vNum,vUnit,vType,vDate;
	m4uint32_t iNumArgs,i;
	m4date_t dRes;

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateAdd.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 3 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "DateAdd"<<3<<iNumArgs << DumpCallStack(&m_oState) );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 3 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateAdd")	;
        }
        iNumArgs=3; //ya hemos sacado los sobrantes
    }

	//sacamos el tipo de las unidades
	ret=RuntimePop(vType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateAdd.")	;

	//sacamos las unidades a sumar
	ret=RuntimePop(vUnit);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateAdd.")	;

	//sacamos la fecha a la que sumar
	ret=RuntimePop(vDate);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateAdd.")	;

	//--comprobación de los tipos

	//vType
	if (IsNull(vType)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDouble (vType)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "UnitType"<<"DateAdd" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }

	//vUnit
	if (IsNull(vUnit)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDouble (vUnit)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Units"<<"DateAdd" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vUnit.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }

	//vDate
	if (IsNull(vDate)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDate (vDate)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Date"<<"DateAdd" <<M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }
	

	m4uint32_t iType=(m4uint32_t)vType.Data.DoubleData;
	if (iType> M4CL_VM_DATEADD_MAX_VALUE || iType< M4CL_VM_DATEADD_MIN_VALUE ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "UnitType"<<"DateAdd"<<iType<< DumpCallStack(&m_oState) );
		iType=M4CL_VM_DATEADD_DAY;
	}

	m4int32_t iHour,iMinute,iSecond;
	m4int_t iYear,iMonth,iDay;
	m4int_t iDiasMesDest;

	//según el tipo de unidades
	if (iType==M4CL_VM_DATEADD_DAY){
		dRes=vDate.Data.DoubleData+(m4int32_t)vUnit.Data.DoubleData;
	}
	else if (iType==M4CL_VM_DATEADD_WEEK ||  iType==M4CL_VM_DATEADD_COMPLETE_WEEK){
		dRes=vDate.Data.DoubleData+((m4int32_t)vUnit.Data.DoubleData*7);	//cable de días que tiene una semana, espero que no cambien, je, je, que graciosillo estoy hoy
	}
	else{
		//convertimos de fecha a números
        ClDateToNumbers (vDate.Data.DoubleData, iYear,iMonth,iDay, iHour,iMinute,iSecond);

		switch (iType)
		{	
		case M4CL_VM_DATEADD_MONTH:
			{
				//caso especial!!

				iMonth+=(m4int32_t)vUnit.Data.DoubleData;;
				
				iDiasMesDest=ndiasdelmes(iMonth, iYear);
				
				if (iDay>iDiasMesDest){
					iDay=iDiasMesDest;
				}
				break;
			}
		case M4CL_VM_DATEADD_YEAR:
			{
				iYear+=(m4int32_t)vUnit.Data.DoubleData;
			
				//por si suman algo al 29 de febrero 
				iDiasMesDest=ndiasdelmes(iMonth, iYear);
				
				if (iDay>iDiasMesDest){
					iDay=iDiasMesDest;
				}

				break;
			}
		case M4CL_VM_DATEADD_HOUR:
			{
				iHour+=(m4int32_t)vUnit.Data.DoubleData;
				break;
			}
		case M4CL_VM_DATEADD_MINUTE:
			{
				iMinute+=(m4int32_t)vUnit.Data.DoubleData;
				break;
			}
		case M4CL_VM_DATEADD_SECOND:
			{
				iSecond+=(m4int32_t)vUnit.Data.DoubleData;
				break;
			}
		//default->No hace nada
			
		}

		//volvemos a convertir a fecha
        ClNumbersToDate (iYear,iMonth,iDay, iHour,iMinute,iSecond, dRes);
	}

	RuntimePushDate(dRes);

	return M4_SUCCESS;
}


//-----------------------------------------------
//DateDiff
//-----------------------------------------------
m4return_t  ClVM0General::DateDiff		(void){
   	m4return_t ret;
	m4VariantType vNum,vDate2,vType,vDate1,vStartDay;
	m4uint32_t iNumArgs,i;
	m4int_t iRes,iStartWeekDay;
	
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateDiff.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 4 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "DateDiff"<<3<<iNumArgs << DumpCallStack(&m_oState) );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 4 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateDiff")	;
        }
        iNumArgs=4; //ya hemos sacado los sobrantes
    }

	if (iNumArgs==4){ //indican día de inicio de semana
		ret=RuntimePopNumber(vStartDay);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "StartDay"<<"DateDiff" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vStartDay.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR;
		}
		iStartWeekDay=(m4uint32_t)vStartDay.Data.DoubleData;
		if (iStartWeekDay<1 || iStartWeekDay>7) {
			iStartWeekDay=1;
		}
	}
	else{
		iStartWeekDay=1; //1=lunes, 7=domingo
	}
	//sacamos el tipo de las unidades
	ret=RuntimePop(vType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateDiff.")	;

	//sacamos las unidades a sumar
	ret=RuntimePop(vDate2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateDiff.")	;

	//sacamos la fecha a la que sumar
	ret=RuntimePop(vDate1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DateDiff.")	;

	//--comprobación de los tipos

	//vType
	if (IsNull(vType)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDouble (vType)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "UnitType"<<"DateDiff" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }

	//vUnit
	if (IsNull(vDate2)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDate (vDate2)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Date2"<<"DateDiff" <<M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate2.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }

	//vDate
	if (IsNull(vDate1)){ //devolvemos nulo y salimos dejando continuar la exe
		RuntimePushNull();
		return M4_SUCCESS;
	}
	if (m_oVariantMediator->ToDate (vDate1)!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Date1"<<"DateDiff" <<M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate1.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR; //salimso parando la ejecución
    }
	

	m4uint32_t iType=(m4uint32_t)vType.Data.DoubleData;
	
	if (iType> M4CL_VM_DATEADD_MAX_VALUE || iType< M4CL_VM_DATEADD_MIN_VALUE ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "UnitType"<<"DateDiff"<<iType<< DumpCallStack(&m_oState) );
		iType=M4CL_VM_DATEADD_DAY;
	}

	m4int32_t iHour1,iMinute1,iSecond1,iHour2,iMinute2,iSecond2;
	m4int_t iYear1,iMonth1,iDay1,iYear2,iMonth2,iDay2,iMonthDays1,iMonthDays2;
	m4int_t iSub=0;
	//según el tipo de unidades
	switch (iType)
	{
	case M4CL_VM_DATEADD_DAY:
		{
			iRes=m4int_t(vDate2.Data.DoubleData-vDate1.Data.DoubleData);
			break;
		}
	case M4CL_VM_DATEADD_MONTH:	
		{
	        ClDateToNumbers (vDate1.Data.DoubleData, iYear1,iMonth1,iDay1, iHour1,iMinute1,iSecond1);
			ClDateToNumbers (vDate2.Data.DoubleData, iYear2,iMonth2,iDay2, iHour2,iMinute2,iSecond2);
			
			iMonthDays1=ndiasdelmes(iMonth1, iYear1);
			iMonthDays2=ndiasdelmes(iMonth2, iYear2);
			
		//	if (iDay1>iMonthDays2)iDay1=iMonthDays2;
		//	if (iDay2>iMonthDays1)iDay2=iMonthDays1;
		/*	iTemp=iDay2;
			if (iDay1>iMonthDays2)iTemp=min(iMonthDays1;
			if (iDay2>iMonthDays1)iDay1=iMonthDays2;
			iDay2=iTemp;
*/
			if (iDay1==iMonthDays1) iDay1=max(iDay1,iMonthDays2);
			if (iDay2==iMonthDays2) iDay2=max(iDay2,iMonthDays1);

			if (iMonth2+iYear2*12 > iMonth1+iYear1*12){
				if (iDay1*86400 + iHour1*3600 + iMinute1*60 + iSecond1 > iDay2*86400 + iHour2*3600 + iMinute2*60 + iSecond2){
					iSub=1;
				}
			}
			else if (iMonth2+iYear2*12 < iMonth1+iYear1*12){
				if (iDay1*86400 + iHour1*3600 + iMinute1*60 + iSecond1 < iDay2*86400 + iHour2*3600 + iMinute2*60 + iSecond2){
					iSub=-1;
				}
			}
			iRes=(iYear2 - iYear1) * 12 + iMonth2 - iMonth1 - iSub;
			break;
		}
	case M4CL_VM_DATEADD_YEAR:	
		{
	        ClDateToNumbers (vDate1.Data.DoubleData, iYear1,iMonth1,iDay1, iHour1,iMinute1,iSecond1);
	        ClDateToNumbers (vDate2.Data.DoubleData, iYear2,iMonth2,iDay2, iHour2,iMinute2,iSecond2);
			
			//excepción-Los leap!!!!
			if (iMonth1==2 && iDay1==29) iDay1=28;
			if (iMonth2==2 && iDay2==29) iDay2=28;

			if (iYear2>iYear1){
				if ( ((iMonth1*32)+iDay1)* 86400 + iHour1*3600 + iMinute1*60 + iSecond1 > ((iMonth2*32)+iDay2)*86400 + iHour2*3600 + iMinute2*60 + iSecond2){
					iSub=1;
				}
			}
			else if (iYear2<iYear1){
				if ( ((iMonth1*32)+iDay1)* 86400 + iHour1*3600 + iMinute1*60 + iSecond1 < ((iMonth2*32)+iDay2)*86400 + iHour2*3600 + iMinute2*60 + iSecond2){
					iSub=-1;
				}

			}
			iRes=iYear2 - iYear1- iSub;
			
			break;
		}
	case M4CL_VM_DATEADD_HOUR:	
		{
			iRes=m4int_t(M4VMRound( (vDate2.Data.DoubleData-vDate1.Data.DoubleData)*24,0) );
			break;
		}
	case M4CL_VM_DATEADD_MINUTE:	
		{
			iRes=m4int_t(M4VMRound( (vDate2.Data.DoubleData-vDate1.Data.DoubleData)*1440,0) );
			break;
		}
	case M4CL_VM_DATEADD_SECOND:	
		{
			iRes=m4int_t(M4VMRound( (vDate2.Data.DoubleData-vDate1.Data.DoubleData)*86400,0));
			break;
		}
	case M4CL_VM_DATEADD_WEEK:	
		{
			// EFV Fix Bug 0065040
			iRes=m4int_t(vDate2.Data.DoubleData-vDate1.Data.DoubleData) / 7 ; //espero que no cambien el número de días por semana
			break;
		}
	/*case M4CL_VM_DATEADD_WEEKDAY:	
		{
			m4int32_t iWeekDay1, iWeekDay2;
			
			//para no repetir código, nos abstraemos de quien es mayor 
			m4date_t dMax,dMin;
			dMax=max(vDate2.Data.DoubleData,vDate1.Data.DoubleData);
			dMin=min(vDate2.Data.DoubleData,vDate1.Data.DoubleData);
			iRes=0;
			iWeekDay1=ndiasem(dMin);
			iWeekDay2=ndiasem(dMax);
			//1.- si el dia inicial no coincide con inicio de semana, y la diferencia entra fi,ff >7, contamos una semana
			if (dMax>dMin && (iWeekDay1!=iStartWeekDay || iWeekDay2!=iStartWeekDay) ){
				
				if (iWeekDay1!=iStartWeekDay){
					++iRes;
				}

				if (iWeekDay1>iStartWeekDay){
					dMin+= (7-iWeekDay1+iStartWeekDay);
				}
				else{
					dMin+= (iStartWeekDay-iWeekDay1);
				}

				if (dMax>dMin){
					
					iRes+=m4int_t(dMax-dMin)/7;
					if (iWeekDay2!=iStartWeekDay){
						++iRes;
					}
				}
				
				//vemos si debe ser negativo
				if (vDate1.Data.DoubleData>vDate2.Data.DoubleData){
					iRes=-iRes;
				}
			}
			else{
				iRes+=m4int_t(vDate2.Data.DoubleData-vDate1.Data.DoubleData)/7;
			}
			break;
		}
	*/
	case M4CL_VM_DATEADD_COMPLETE_WEEK:
		{
			m4int32_t iWeekDay1;
			
			//para no repetir código, nos abstraemos de quien es mayor 
			m4date_t dMax,dMin;
			dMax=max(vDate2.Data.DoubleData,vDate1.Data.DoubleData);
			dMin=min(vDate2.Data.DoubleData,vDate1.Data.DoubleData);

			iWeekDay1=ndiasem(dMin);
			
			
			if (iWeekDay1>iStartWeekDay){
				dMin+= (7-iWeekDay1+iStartWeekDay);
			}
			else{
				dMin+= (iStartWeekDay-iWeekDay1);
			}
			
			iRes=m4int_t(dMax-dMin+1)/7;
			
			//vemos si debe ser negativo
			if (vDate1.Data.DoubleData>vDate2.Data.DoubleData){
				iRes=-iRes;
			}
			break;
		}
	}

	RuntimePushDouble(iRes);

	return M4_SUCCESS;
}




//-----------------------------------------------
//TickCount
//-----------------------------------------------
m4return_t	ClVM0General::TickCount( void )
{

	m4double_t		dTickCount = 0 ;
	m4millisec_t	lTickCount = 0 ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: TickCount");
#endif


	lTickCount = ClPrecisionTicker::GetTimeTick() ;
	dTickCount = m4double_t( lTickCount - m_lBaseTickCount ) ;

	// Se devuelve el diferencial respecto al comienzo de la aplicación
	// Para que no se desborde el doble
	RuntimePushDouble( dTickCount ) ;

	return( M4_SUCCESS ) ;
}



//-----------------------------------------------
//Sleep
//-----------------------------------------------
m4return_t	ClVM0General::vmSleep( void )
{

    m4return_t      iResult = M4_ERROR ;
    m4VariantType   vSeconds ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Sleep");
#endif

	iResult = RuntimePopDouble( vSeconds ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Seconds" << "Sleep" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vSeconds.Type ) << DumpCallStack( &m_oState ) ) ;

	m4sleep( int( vSeconds.Data.DoubleData ) ) ;

	RuntimePushDouble( M4_SUCCESS ) ;

	return( M4_SUCCESS ) ;
}





//---------------------------------------------------------
//-L O G 
//---------------------------------------------------------
m4return_t  ClVM0General::Log           (void){
    
    m4VariantType   vTemp, vNumArg;
    m4return_t      ret;
    m4uint32_t      i;
	m4double_t		dBase;

	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: Log");
#endif
    //sacamos el número de argumentos
    ret=RuntimePop (vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Log")	;
    
    if (vNumArg.Data.DoubleData >1 ){
		//eliminamos los posibles argumentos extras
        for (i=0; i < vNumArg.Data.DoubleData-2; ++i){            
            ret=RuntimePopAndDelete ();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "log.")	;
		}
		//sacamos la base
		ret=RuntimePopDouble (vTemp);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH,  "Base"<<"Log" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack(&m_oState) );
			return M4_ERROR; //salimso parando la ejecución
		}
		dBase=vTemp.Data.DoubleData;
    }
    else{
		//por defecto base neperiana
		dBase=M4CL_MATH_E;
    }

	//sacamos el número
	ret=RuntimePopDouble(vTemp);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Number"<<"Log" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack(&m_oState) );
		return M4_ERROR;
	}

	//comprobaciones: Numero >0, base >0
	if (dBase<=0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Base"<<"Log"<<dBase <<DumpCallStack(&m_oState) );
		return M4_ERROR;
	}

	if (vTemp.Data.DoubleData<=0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Number"<<"Log"<<vTemp.Data.DoubleData <<DumpCallStack(&m_oState) );
		return M4_ERROR;
	}

	//metemos el resultado
	RuntimePushDouble( log(vTemp.Data.DoubleData) / log(dBase) );
	
	return M4_SUCCESS;
}
