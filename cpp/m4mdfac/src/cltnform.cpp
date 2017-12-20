
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnform.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary formula of a nitem
//
//
//==============================================================================


#include "m4facres.hpp"

#include "cltnform.hpp"
#include "cltiform.hpp"
#include "chlog.hpp"
#include "instru.h"
#include "spoolstr.h"
#include "cltnitem.hpp"
#include "cltchann.hpp"
#include "cltnode.hpp"
#include "cltt3al.hpp"
#include "cltti.hpp"
#include "m4mdfaci.hpp"
#include "m4mdrt.hpp"
#include "tablafuncdesc.hpp"
#include "tablatt.hpp"
#include "cltarg.hpp"
#include "cltconn.hpp"
#include "cltnicon.hpp"
#include "vmmcrdef.hpp"
#include "recorset.hpp"
#include "nodedef.hpp"



// Unos defines ===============================================================

#define M4CL_MAX_CODE_GENERATED					10240

//para la propagación de formulas
#define M4CL_PARAM_ACTION_NONE					-20
#define M4CL_PARAM_ACTION_PURGE					-40
#define M4CL_PARAM_ACTION_ASSIGN				0

#define M4CL_VM_EXTERNAL_METHOD_CALL			"EXTERNALMETHODCALL"
#define M4CL_VM_REMOTE_METHOD_CALL				"REMOTECALL"

#define	M4CL_TEMP_BUFFER_SIZE					128




// ============================================================================
// Funciones para pasar el último comentario al final
// Bug 0068811
// ============================================================================

#define	M4CL_MAX_COMMENT_TEMP_BUFFER			50


// Busca la línea anterior
m4int32_t	M4PreviosInstruction( m4pcchar_t ai_pccPolish, m4int32_t ai_iPosition )
{

	m4int32_t	iResult = ai_iPosition - 1 ;


	while( iResult > 0 && ai_pccPolish[ iResult - 1 ] != '\n' )
	{
		iResult-- ;
	}

	return( iResult ) ;
}


/* Pasa el último comentario al final
La estructura del final de la fórmula debe ser
Cx Vx N0.0 [R-1 ... R-x] j0 L0 E
*/
void	M4MoveCommentToEnd( m4pchar_t aio_pcPolish, m4int32_t ai_iLength )
{

	m4int32_t	i = 0 ;
	m4int32_t	iPos = 0 ;
	m4int32_t	iCommentSize = 0 ;
	m4int32_t	iEndSize = 0 ;
	m4int32_t	iStart = 0 ;
	m4int32_t	iEnd = 0 ;
	m4char_t	acTempBuffer[ M4CL_MAX_COMMENT_TEMP_BUFFER ] ;


	iPos = ai_iLength ;


	// Búsqueda de L0
	iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;

	if( iPos < 0 || aio_pcPolish[ iPos ] != 'L' )
	{
		return ;	// Si no la encuentra continua
	}

	// Búsqueda de j0
	iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;

	if( iPos < 0 || aio_pcPolish[ iPos ] != 'j' )
	{
		return ;	// Si no la encuentra continua
	}

	// Se saltan los R-x
	iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;

	while( iPos >= 0 && aio_pcPolish[ iPos ] == 'R' )
	{
		iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;
	}

	// Búsqueda de N0.0
	if( iPos < 0 || aio_pcPolish[ iPos ] != 'N' )
	{
		return ;	// Si no la encuentra continua
	}

	// Búsqueda de Vx
	iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;

	if( iPos < 0 || aio_pcPolish[ iPos ] != 'V' )
	{
		return ;	// Si no la encuentra continua
	}


	// Se marca el final del código a mover
	iEnd = iPos ;

	// Se pasan las siguientes líneas de comentario
	iStart = iEnd ;
	iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;

	while( iPos >= 0 && aio_pcPolish[ iPos ] == 'C' )
	{
		// Se marca el comienzo del código a mover
		iStart = iPos ;
		iPos = M4PreviosInstruction( aio_pcPolish, iPos ) ;
	}

	iCommentSize = iEnd - iStart ;

	if( iCommentSize <= 0 || iCommentSize > M4CL_MAX_COMMENT_TEMP_BUFFER )
	{
		// Si no hay nada que mover o es muy grande continua
		return ;
	}

// Se guarda el código del comentario

	for( i = 0 ; i < iCommentSize ; i++ )
	{
		acTempBuffer[ i ] = aio_pcPolish[ iStart + i ] ;
	}

// Se mueve el resto hacia atrás

	iEndSize = ai_iLength - iEnd ;

	for( i = 0 ; i < iEndSize ; i++ )
	{
		aio_pcPolish[ iStart + i ] = aio_pcPolish[ iEnd + i ] ;
	}

// Se pone el comentario

	for( i = 0 ; i < iCommentSize ; i++ )
	{
		aio_pcPolish[ ai_iLength - iCommentSize + i ] = acTempBuffer[ i ] ;
	}
}




//                   ========================================
// formulas de nitem ========================================
//					 ========================================



//Construir

ClTempNitemFormula::ClTempNitemFormula( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;
	
	m_poTi = NULL ;
	m_acFormulaId[ 0 ] = '\0' ;
	m_acFormulaId[ 1 ] = '\0' ;
	
    m_iType = M4CL_LENGUAGE_LN4 ; //por defecto
	m_iLevel = 0 ;
	m_iIsMetarule = 0 ;
	m_iIsRetroactivity = 0 ;
	m_iBatchType = 0 ;
	m_iIsEvent = 0 ;
	m_iDBOrder = 0 ;
	m_bIsDatabase = M4_FALSE ;
	
    m_pccObject = NULL ;
    m_pccMethod = NULL ;
	m_pszTotalPolish=NULL;
    m_pcPolish = NULL ;
	m_bPolishOwner = M4_FALSE ;
	m_dStartDate = 0 ;
	m_dEndDate = 0 ;
	m_iIdHandle = 0 ;
	m_iOrder = m4uint16_t( -1 ) ;
    m_poFormulaNitem = NULL ;
	m_iIsToCompile = 0 ;
	//JIT
	m_poCMCRforJIT=NULL;
	m_poRecordSet=NULL;
	//
	
	m_pInstructions=NULL;
	m_bInstalled=M4_FALSE; 
	m_NumInstructions=0;
	
	//Inicializar Contador de instrucciones de cada tipo
	memset( m_NumInstructionsType, 0, sizeof( m4int32_t ) * M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES ) ;
	m_iNumVariantsAsInt=0;

	//m_iNumArgs=0;
	//m_iNumArgsByRef=0;
	m_iNumLocalVars=0;
	m_iOrigin=M4CL_NO_LENGUAJE; //por defecto
	m_bRemote=M4_FALSE;
	//establecemos el pool
	
    m_iMustReturn = 1 ;
    m_iPriority = M4CL_RULE_MAX_PRIORITY ;
	m_iCreationType = M4_MDFAC_RULE_CREATION_NORMAL ;
	
	m_hItemJIT = 0 ;
	
	m_iLabelsSize=M4CL_INIT_MAX_LABELS;
	m_aiLabels.reserve(m_iLabelsSize);
	m_aiLabels.resize(m_iLabelsSize);
}



void	ClTempNitemFormula :: Clear( void )
{
    
	m_iType = m_iOrigin ;
	m_bIsDatabase = M4_FALSE ;
	
	if( m_pszTotalPolish != NULL )
	{
		delete [] m_pszTotalPolish ;
		m_pszTotalPolish=NULL ;
	}
	
	m_poCMCRforJIT = NULL ;
	m_poRecordSet = NULL ;
	
	m_bInstalled = M4_FALSE ;
	m_NumInstructions = 0 ;
	
	//Inicializar Contador de instrucciones de cada tipo
	memset( m_NumInstructionsType, 0, sizeof( m4int32_t ) * M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES ) ;
	m_iNumVariantsAsInt=0;	

	m_iNumLocalVars = 0 ;
	//m_iOrigin = M4CL_NO_LENGUAJE ;
	m_bRemote=M4_FALSE;
	
    m_iMustReturn = 1 ;
    m_iPriority = M4CL_RULE_MAX_PRIORITY ;
	m_iCreationType = M4_MDFAC_RULE_CREATION_NORMAL ;
	
	m_hItemJIT = 0 ;

	if (m_pInstructions!=NULL) {
		delete [] m_pInstructions;
		m_pInstructions=NULL;
	}
}


//Destruir

ClTempNitemFormula::~ClTempNitemFormula( void )
{
	
	if (m_pInstructions!=NULL) {
		delete [] m_pInstructions;
		m_pInstructions=NULL;
	}
	
	if( m_bPolishOwner == M4_TRUE )
	{
		if( m_pcPolish != NULL )
		{
			delete [] m_pcPolish ;
			m_pcPolish = NULL ;
		}
	}
}


//---------------------------------------------------------
//PRE INSTALAR
//---------------------------------------------------------

//-----------------------------
//PreinstallLN4
//-----------------------------

m4return_t  ClTempNitemFormula::_PreinstalConnectors( m4pcchar_t ai_pccMidCode, m4bool_t ai_bConvertToLN4Always, m4bool_t ai_bRePushVars )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4int32_t	iLength = 0 ;
	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcPreviousCode = NULL ;
	m4pchar_t	pcPostCode = NULL ;
	m4pchar_t	pcStartComment = NULL ;
	m4pchar_t	pcEndComment = NULL ;
	m4pchar_t	pcReturnComment = NULL ;


	// Solo instalamos los conectores si es metaregla, porque sino se ejecutarían más de una vez
	if( m_iOrder == 0 )
	{	
		// Calculamos el número de variables "originales" para poder generar el código de la propagación
		PreGetNumVar( ai_pccMidCode ) ;
		
		pcPreviousCode = new m4char_t[ M4CL_MAX_CODE_GENERATED + 1 ] ;
		CHECK_CHLOG_ERRORF( pcPreviousCode == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( M4CL_MAX_CODE_GENERATED + 1 ) ) << __LINE__ << __FILE__ ) ;
		*pcPreviousCode = '\0' ;

		iResult = GenerateConnectingCodePre( pcPreviousCode, M4CL_MAX_CODE_GENERATED ) ;

		if( iResult != M4_SUCCESS )
		{
			delete [] pcPreviousCode;
			return( M4_ERROR ) ;
		}
		
		pcPostCode = new m4char_t[ M4CL_MAX_CODE_GENERATED + 1 ] ;
		CHECK_CHLOG_ERRORF( pcPostCode == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( M4CL_MAX_CODE_GENERATED + 1 ) ) << __LINE__ << __FILE__ ) ;
		*pcPostCode = '\0' ;

		iResult = GenerateConnectingCodePost( pcPostCode, M4CL_MAX_CODE_GENERATED ) ;

		if( iResult != M4_SUCCESS )
		{
			delete [] pcPreviousCode;
			delete [] pcPostCode;
			return( M4_ERROR ) ;
		}
		
	}
	else
	{
		pcPreviousCode = new m4char_t[ 1 ] ;
		CHECK_CHLOG_ERRORF( pcPreviousCode == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * M4CL_MAX_CODE_GENERATED ) << __LINE__ << __FILE__ ) ;
		*pcPreviousCode = '\0' ;

		pcPostCode = new m4char_t[ 1 ] ;
		CHECK_CHLOG_ERRORF( pcPostCode == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * 1 ) << __LINE__ << __FILE__ ) ;
		*pcPostCode = '\0' ;
	}

	// Miramos si tenemos que dejar el tipo que viniese o poner ln4
	// Convertinos a tipo ln4 cuando se ha añadido código de propagación o cuando dice que siempre hay que convertir
	if( m_iType != M4CL_LENGUAGE_LN4 )
	{
		if( strlen( pcPreviousCode ) > 0 || strlen( pcPostCode ) > 0 || ai_bConvertToLN4Always == M4_TRUE )
		{
			// Ahora la formula es de tipo LN4
			m_iType = M4CL_LENGUAGE_LN4 ;
			if( ai_bRePushVars == M4_TRUE )
			{
				//de momento solo el codigo original de cpp lo hace, ya que si hace pop de las vars. mientras
				//que el de remote o dll lee los argumentos sin sacar las variables de la pila
				RePushVars( pcPreviousCode ) ;
			}
		}
	}


	if( m_bRemote == M4_FALSE )
	{
		pcStartComment  = "C1\n" ;
		pcEndComment    = "C1\n" ;

		// Si es de C++ hay que ponerle el retorno del final
		if( m_iOrigin != M4CL_LENGUAGE_LN4 )
		{
			pcReturnComment = "C1\n" ;
		}
		else
		{
			pcReturnComment = "" ;
		}
	}
	else
	{
		pcStartComment  = "" ;
		pcEndComment    = "" ;
		pcReturnComment = "" ;
	}

	iSize = 2 ;	// El return
	iSize += strlen( pcPreviousCode ) ;
	iSize += strlen( pcStartComment ) ;
	iSize += strlen( ai_pccMidCode ) ;
	iSize += strlen( pcReturnComment ) ;
	iSize += strlen( pcPostCode ) ;
	iSize += strlen( pcEndComment ) ;

	
	m_pszTotalPolish = new m4char_t[ iSize + 1 ] ;
	CHECK_CHLOG_ERRORF( m_pszTotalPolish == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( iSize + 1 ) ) << __LINE__ << __FILE__ ) ;
	*m_pszTotalPolish = '\0' ;

	// Conector Pre
	strcat( m_pszTotalPolish, pcPreviousCode ) ;

	// Comentario C1 para el comienzo de la fórmula
	strcat( m_pszTotalPolish, pcStartComment ) ;

	// Código LN4 de la regla
	strcat( m_pszTotalPolish, ai_pccMidCode ) ;

	iLength = strlen( m_pszTotalPolish ) ;
	while( m_pszTotalPolish[ iLength ] != 'E' && iLength > 0 )
	{
		iLength-- ;
	}
	m_pszTotalPolish[ iLength ] = '\0' ;

	// Hay que pasar el último comentario al final
	M4MoveCommentToEnd( m_pszTotalPolish, iLength ) ;

	// Retorno del final si hubiera
	strcat( m_pszTotalPolish, pcReturnComment ) ;

	// Conector Post
	strcat( m_pszTotalPolish, pcPostCode ) ;

	// Comentario C1 para el final de fórmula
	strcat( m_pszTotalPolish, pcEndComment ) ;

	// Código de retorno
	strcat( m_pszTotalPolish, "E\n" ) ;


	// Para que quede preinstalada
	iResult = CountInstructionsLN4( m_pszTotalPolish ) ;
	
	delete [] pcPreviousCode ;
	delete [] pcPostCode ;

	return( iResult ) ;
}


m4return_t ClTempNitemFormula::PreInstallLN4( void )
{
	m4return_t	iResult = M4_SUCCESS ;


	iResult = _PreinstalConnectors( m_pcPolish, M4_FALSE, M4_FALSE ) ;

	return( iResult ) ;
}


m4return_t ClTempNitemFormula::PreInstallCPP(void){
	
	m4char_t MidCode[M4CL_MAX_LINEA_POLACA];
	MidCode[0]=0; //para poder concatenar
	
	
	//llamada a la función original
	GenerateInstructionFunctionCall (MidCode, m_pccMethod); 
	
	//le añadimos el return
	GenerateInstructionReturn( MidCode ) ;
	
	m4return_t ret=_PreinstalConnectors(MidCode, M4_FALSE, M4_TRUE);
	return ret; 
}


//----------------------------------
//PreinstallExternal
//----------------------------------

m4return_t ClTempNitemFormula::PreinstallExternal(void){
	
	m4char_t MidCode[M4CL_MAX_LINEA_POLACA*2];
	MidCode[0]=0; //para poder concatenar
	
	m4return_t ret;
	

	/* Bug 0102774
	Hay que quitar la extensión del objeto y pasarlo a minúsculas
	*/
	m4pchar_t pcPoint = NULL ;
	m4char_t acTemp[ M4CL_MAX_LINEA_POLACA ] ;

	strcpy( acTemp, m_pccObject ) ;
	strlwr( acTemp ) ;

	pcPoint = strchr( acTemp, '.' ) ;

	if( pcPoint != NULL )
	{
		*pcPoint = '\0' ;
	}

    GenerateInstructionPushString   (MidCode, acTemp);
    GenerateInstructionPushString   (MidCode, m_pccMethod);
    GenerateInstructionPushNum      (MidCode, m_iType);
    ret=GenerateInstructionFunctionCall (MidCode, M4CL_VM_EXTERNAL_METHOD_CALL);
	if( ret != M4_SUCCESS ){
		DumpDebugInfo (M4_MDFAC_WRONG_BAD_PREINSTALL, "external method call");
		return( M4_ERROR ) ;
	}
	
	
	//le añadimos el return
	GenerateInstructionReturn( MidCode ) ;

	//preinstalamos como si fuera LN4 (de hecho ahora lo es)
	ret=_PreinstalConnectors(MidCode,M4_TRUE,M4_FALSE);
	
	if( ret != M4_SUCCESS ){
		DumpDebugInfo (M4_MDFAC_WRONG_BAD_PREINSTALL, "external method call");
		return( M4_ERROR ) ;
	}
	
	
	return ret;
}

//----------------------------------
//PreinstallRemote
//----------------------------------

m4return_t ClTempNitemFormula::PreinstallRemote(void){
	
	
	m4char_t MidCode[M4CL_MAX_LINEA_POLACA*2];
	MidCode[0]=0; //para poder concatenar
	m4return_t ret;
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	m4int8_t iNumRealArgs, i8Type, fFlags;
	m4uint16_t iId;
	m4char_t acParamDesc[M4CL_MAX_PARAM];		
	
	
	
    ret=GenerateInstructionFunctionCall (MidCode, M4CL_VM_REMOTE_METHOD_CALL);
	if( ret != M4_SUCCESS ){
		DumpDebugInfo(M4_MDFAC_WRONG_BAD_PREINSTALL, "remote method call");
		return( M4_ERROR ) ;
	}

	
	if (m_iType == M4CL_LENGUAGE_CPP){
		
		strcpy(acName,m_pccMethod);
		strupr(acName);
		
		//si es de los que no asignan, quitamos el flag
		ret=m_poFunctionTable->GetId(acName, iId, iNumRealArgs, acParamDesc, i8Type);
		if (ret!=M4_SUCCESS){
			DumpError (M4_MDFAC_FUNCTION_NOT_FOUND, m_pccMethod);
			return ret;
		}
		
		fFlags=m_poFunctionTable->GetFlags(iId);
		//si la función tiene activado el flag de no asignar, quitamos el MustReturn
		if (M4CL_HAS_FLAG (fFlags, M4CL_FLAG_FN_NO_RETURN)){
			m_iMustReturn=0;
		}
		
	}
	
	//le añadimos el return
	GenerateInstructionReturn( MidCode ) ;

	//preinstalamos como si fuera LN4 (de hecho ahora lo es)
	ret=_PreinstalConnectors(MidCode,M4_TRUE,M4_FALSE);
	
	if( ret != M4_SUCCESS ){
		DumpDebugInfo(M4_MDFAC_WRONG_BAD_PREINSTALL, "remote method call");
		return( M4_ERROR ) ;
	}
	
	
	return ret;
	
}

//----------------------------------
//PreinstallUpperLevels
//----------------------------------

m4return_t ClTempNitemFormula::PreinstallUpperLevels(void){
	
	m4char_t MidCode[M4CL_MAX_LINEA_POLACA];
	MidCode[0]=0; //para poder concatenar
	
	//llamada a la función original
	GenerateInstructionItemLevelCall (MidCode); 
	
	//le añadimos el return
	GenerateInstructionReturn( MidCode ) ;

	return _PreinstalConnectors(MidCode,M4_TRUE,M4_FALSE);
}

//--------------------------------
//PreinstallInternalLevels
//--------------------------------
m4return_t	ClTempNitemFormula::PreinstalInternalLevels(void){
	
	
	m4return_t iResult,ret;
	
	
	ret=CheckForRemoteExecution (m_bRemote);
	if (ret != M4_SUCCESS){
		return( M4_ERROR ) ;
	}
	
	if (m_bRemote){  //en remoto
		iResult = PreinstallRemote();
		if( iResult != M4_SUCCESS ){
			return( M4_ERROR ) ;
		}
		return M4_SUCCESS;
	}
	
    switch (m_iType){
		
    case M4CL_LENGUAGE_CPP:
        {	
            if (m_pccMethod==0 || strcmp(m_pccMethod,"")==0) {
                m_pccMethod="NULL"; //la función que no hace nada util
            }
			
            iResult= CheckCppDefinition(); 
			if( iResult != M4_SUCCESS ){
                return( M4_ERROR ) ;
            }
			
            iResult = PreInstallCPP();
            if( iResult != M4_SUCCESS ){
				DumpDebugInfo(M4_MDFAC_WRONG_BAD_PREINSTALL, "CPP method");
                return( M4_ERROR ) ;
            }
            break;
        }
    case M4CL_LENGUAGE_LN4:
        {
            if (m_pcPolish==0 || strcmp(m_pcPolish,"")==0) {
                
				DumpError(M4_MDFAC_WRONG_NOTHING_TO_INSTALL, "LN4");
                return( M4_ERROR ) ;
            }
			
            iResult = PreInstallLN4();
            if( iResult != M4_SUCCESS )
            {
				DumpDebugInfo(M4_MDFAC_WRONG_BAD_PREINSTALL, "LN4 method");
                return( M4_ERROR ) ;
            }
            break;
        }
    case M4CL_INTERN_OBJECT_DLL:
		{
            if ( m_pccObject==0 || m_pccMethod==0 ||
				strcmp(m_pccObject,"")==0 || strcmp(m_pccMethod,"")==0 ) { //comprobamos que no sea null
				
				DumpError(M4_MDFAC_WRONG_NOTHING_TO_INSTALL, "External call");
                return( M4_ERROR ) ;
            }
			
            //llamamos preainstalar
			ret=PreinstallExternal();
            break;
        }
		
    default:
        {
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
            sprintf(acTemp, "<%d>", m_iType);
            DumpError(M4_MDFAC_WRONG_RULE_TYPE, acTemp);
			return( M4_ERROR ) ;
        }
    }
	
	return ret;
}


//------------------------------------------------------------
//Instalaciones
//------------------------------------------------------------

//***Instalar una formula LN4
m4return_t ClTempNitemFormula::InstallLN4(void){
	
	m4pcchar_t pszActualLine;
	m4int32_t iActualLine;
	m4uint32_t iAuxInt;
	m4uint32_t iAuxUInt;
	m4uint32_t iLength ;
	m4int32_t iNumVar;
	m4int8_t iAuxChar;
	m4int32_t iSourceCodeLine;
	m4pcchar_t psTemp2;
	m4return_t TempRet, ret;
	m4char_t acString[M4CL_MAX_LINEA_POLACA];
	m4uint16_t ui16NumFunc;
    m4pchar_t pszStoreString, pszInUseStoreString;
    m4uint32_t iStoreString;
	m4bool_t bArtificialFinalLine=M4_FALSE;
	
    pszStoreString=0;
    iStoreString=0;
	
	iSourceCodeLine=1;
	
	if (m_pszTotalPolish==NULL){ //de la formula original
		pszActualLine=m_pcPolish;
	}
	else{
		pszActualLine=m_pszTotalPolish;
	}
	iActualLine=0;
	
    
	//Ya tenemos el número de instrucciones reales y 
	//las etiquetas metidas (preinstalación)
	
	
	
	if (m_pInstructions!=NULL) { //si habia algo instalado nos lo cargamos
		delete [] m_pInstructions;
		m_pInstructions=NULL;
	}
	
	m_pInstructions=new ClInstruction[m_NumInstructions];
	CHECK_CHLOG_ERRORF( m_pInstructions == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClInstruction ) * m_NumInstructions ) << __LINE__ << __FILE__ ) ;
	
	m4return_t retCode=M4_SUCCESS;
	while (pszActualLine!=NULL && retCode==M4_SUCCESS){
		//los flags a cero
		m_pInstructions[iActualLine].m_Flags=0;
		m_pInstructions[iActualLine].m_bIsChar=M4_FALSE;
        //para que no de memoria sin ini
        m_pInstructions[iActualLine].m_poItem=0;
        m_pInstructions[iActualLine].m_uVariant.DoubleData=0;
        m_pInstructions[iActualLine].m_pcNameChannel=0;
        m_pInstructions[iActualLine].m_iNumAtrib=0;
        m_pInstructions[iActualLine].m_iNumRegla=0;
        m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumItem=0;
        m_pInstructions[iActualLine].m_iNumNodo=0;
        m_pInstructions[iActualLine].m_poNode=0;
        m_pInstructions[iActualLine].m_iNumFunc=0;
		m_pInstructions[iActualLine].m_iTipoIns=m4uint8_t( -1 );
		
		switch(*pszActualLine){
			
			
		case 'L': break; //no hacer nada
			
		case 'R':{	
			//iNumVar=atoi(pszActualLine+1);
			iNumVar = _ReadNegative( pszActualLine + 1, '\n' ) ;
			
			//si es de arg. varaibles, leemos un arg fijo y tipo ln4->vamos por un sitio que lea el argumento bien teniendo en cuenta 
			//los que se le han pasado
			
			if ( iNumVar<0 && m_iOrigin==M4CL_LENGUAGE_LN4 && HasLN4VariableArgs() ){
				ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_READ_VAR_ARGS];
				
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 1);
			}
			else{
				ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_READ];
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			}
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumVar=iNumVar;
			iActualLine++;
			break;
				 }
			
		case 'N':{
			m4bool_t bDouble;
			_CheckDouble(pszActualLine+1, bDouble);
			
			if (bDouble){
				m_pInstructions[iActualLine].m_uVariant.DoubleData=atof(pszActualLine+1);
			}
			else{
				m_pInstructions[iActualLine].m_uVariant.CharData=atoi(pszActualLine+1);
				++m_iNumVariantsAsInt;
			}
			ui16NumFunc=M4CL_VM_FT_PUSHNUM;
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_bVariantAsInt= (bDouble==0.0 ? M4_TRUE : M4_FALSE );
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
			
		case 'S':{
			m4uint32_t iTam;
			
			//añadir cadena al pool
			psTemp2=pszActualLine+1;

			/* Bug 0276029
			Los \r no deben cortar el parseo, pero si eliminarse si van con \n
			*/
			while (*psTemp2 != '\0' && *psTemp2 != '\n') psTemp2++;
			if (*psTemp2 == '\n' && *(psTemp2 - 1) == '\r')
			{
				psTemp2--;
			}
			
			iTam=psTemp2-pszActualLine -1 ;
			
			if (iTam < M4CL_MAX_LINEA_POLACA ){ 
				// Usamos memoria estatica                        
				pszInUseStoreString = acString;
			}
			else if (iTam > iStoreString) {
				// Usamos y Creamos memoria dinamica
				if (pszStoreString){
					// Borramos el trozo reservado anteriormente
					delete [] pszStoreString;
				}						
				if ( (pszStoreString= new m4char_t [iTam+1]) )
					iStoreString  = iTam;
				pszInUseStoreString = pszStoreString;
			} else {
				// Reutilizamos memoria dinamica
				pszInUseStoreString = pszStoreString;
			}
			
			memcpy( pszInUseStoreString, pszActualLine + 1, iTam ) ;
			
			pszInUseStoreString[iTam]='\0';
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT;
			
			if (m_poNoUpperPool->AddString(pszInUseStoreString,iAuxUInt) ==M4_SUCCESS){
				ui16NumFunc=M4CL_VM_FT_PUSHSTRING;
				
				m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
				
				m_pInstructions[iActualLine].m_uVariant.CharData=(m4int32_t)iAuxUInt;
				m_pInstructions[iActualLine].m_bIsChar=M4_TRUE;
				m_pInstructions[iActualLine].m_bVariantAsInt=M4_TRUE; //redundante
				++m_iNumVariantsAsInt;
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
				iActualLine++;
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				retCode=M4_ERROR;
			}
			
			break;
				 }
			
		case 'D':{
			m4bool_t bDouble;
			_CheckDouble(pszActualLine+1, bDouble);
			
			if (bDouble){
				m_pInstructions[iActualLine].m_uVariant.DoubleData=atof(pszActualLine+1);
			}
			else{
				m_pInstructions[iActualLine].m_uVariant.CharData=atoi(pszActualLine+1);
				++m_iNumVariantsAsInt;
			}
			
			ui16NumFunc=M4CL_VM_FT_PUSHDATE;
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_bVariantAsInt= (bDouble==0.0 ? M4_TRUE : M4_FALSE );
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
			
		case 'C':{
			//Para instalar el comentario (Stepper)
			iAuxInt = _ReadPositive( pszActualLine + 1, '\n' ) ;
			
			iSourceCodeLine+=iAuxInt;
			
			ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_READCOMMENT];
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumVar=iSourceCodeLine;
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			
			break;
				 }
			
		case 'P':{
			iAuxInt = _ReadPositive( pszActualLine + 1, '\n' ) ;
			
			ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_PURGE];
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumItem=iAuxInt;
			
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
			
		case 'A':{
			iNumVar = _ReadNegative( pszActualLine + 1, '\n' ) ;
			
			//si es de arg. varaibles, leemos un arg fijo y tipo ln4->vamos por un sitio que lea el argumento bien teniendo en cuenta 
			//los que se le han pasado
			if (  iNumVar<0 && m_iOrigin==M4CL_LENGUAGE_LN4 && HasLN4VariableArgs() ){
				ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_ASSIGN_VAR_ARGS];
				
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 1);
			}
			else{
				ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_ASSIGN];
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);			
			}
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumVar=iNumVar;
			
			iActualLine++;
			
			m_iNumLocalVars= (m_iNumLocalVars>iNumVar+1 ? m_iNumLocalVars : iNumVar+1);	 //tomamos el máximo apra saber el número de varaibles
			break;
				 }
			
		case 'J':{
			iAuxInt = _ReadPositive( pszActualLine + 1, '\n' ) ;
			
			ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_COND_JUMP];
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumVar=m_aiLabels[iAuxInt]-iActualLine; //salto es relativo
			
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
			
		case 'j':{
			iAuxInt = _ReadPositive( pszActualLine + 1, '\n' ) ;
			
			ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_INCOND_JUMP];
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			
			m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumVar=m_aiLabels[iAuxInt]-iActualLine; //salto relativo
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
			
		case 'F':{	//
			m4int32_t iAuxNumFunc;
			
			//obtenemos la función 
			iAuxNumFunc = _ReadPositive( pszActualLine + 1, '\n' ) ;
			
			ui16NumFunc = (m4uint16_t) iAuxNumFunc;
			TempRet=m_poFunctionTable->Exists(ui16NumFunc,iAuxChar );
			
			if (TempRet!=M4_SUCCESS){
				m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
				sprintf( acTemp, "%d",ui16NumFunc);
				DumpError (M4_MDFAC_WRONG_FUNCTION, acTemp);
				retCode=M4_ERROR;
				break;
			}
			
			//pomenos el nivel en la instrucción, 
			switch (m_poFunctionTable->GetType (ui16NumFunc))
			{
			case M4CL_SEMANTIC_FUNC2:
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags, 2); 
				break;
				
			case M4CL_PROPAGATION_FUNC:
			case M4CL_SEMANTIC_FUNC1:
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags, 1); 
				break;
				
			case M4CL_CHANNEL_FUNC:
				DumpError( M4_MDFAC_BAD_CHANNEL_FUNCTION_USE, m_poFunctionTable->GetName(ui16NumFunc) ) ;
				retCode=M4_ERROR;
				break;
				
			default:
				M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags, 0);
				break;
			}
			
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_SIMPLE;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			
			iActualLine++;				 
			break;
				 }
			
		case 'H':{	//Funciones del CANAL
			//obtenemos la función 
			ui16NumFunc = m4uint16_t( _ReadPositive( pszActualLine + 1, ' ' ) ) ;
			
			
			TempRet=m_poFunctionTable->Exists( ui16NumFunc,iAuxChar );
			if (TempRet!=M4_SUCCESS){
				m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
				sprintf( acTemp, "%d",ui16NumFunc);
				DumpError (M4_MDFAC_WRONG_CHANNEL_FUNCTION, acTemp);
				retCode=M4_ERROR;
				break;
			}
			if (m_poFunctionTable->GetType (ui16NumFunc) != M4CL_CHANNEL_FUNC){
				m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
				sprintf( acTemp, "%d",ui16NumFunc);
				DumpError (M4_MDFAC_WRONG_CHANNEL_FUNCTION, acTemp);
				retCode=M4_ERROR;
				break;
			}
			
			//pomenos el nivel en la instrucción->Son de nivel 2
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 2);
			
			//leemos el nombre del canal al que se refiere la función
			while(*pszActualLine!=' ' && *pszActualLine!='\n') pszActualLine++;
			
			m4pcchar_t pccT3AliasInstance ;
			
			if( *pszActualLine == ' ' )
			{
				iLength = _ReadString( pszActualLine + 2, ' ', acString ) ;
				pszActualLine += iLength + 2 ;  // salta el espacio

				if( GetT3AliasInstance( acString, pccT3AliasInstance ) == M4_ERROR )
				{
					pccT3AliasInstance = "" ;

					//si no encuentra como instancia, miramos si en nuestro mismo canal
					if( strcmp( GetThisChannelId(), acString ) != 0 )
					{
						//ni instancia ni nuestro canal->error
						DumpError (M4_MDFAC_WRONG_T3_NODE_INSTANCE, acString);
						retCode=M4_ERROR;
						break;
					}
				}
			}
			else
			{
				// Si viene sin alias de canal se toma el canal actual
				/* Bug 0105179
				Si no viene canal se deja a cadena vacía y no se pone el nombre actual del canal
				strcpy( acString, GetThisChannelId() ) ;
				*/
				pccT3AliasInstance = "" ;
			}
			
			//guardamos el nombre 
			if (m_poUpperPool->AddString(pccT3AliasInstance,iAuxUInt) ==M4_SUCCESS){
				m_pInstructions[iActualLine].m_pcNameChannel=iAuxUInt;						
				
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				retCode=M4_ERROR;
				break;
			}
			
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			//m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumItem=iAuxInt;//número de función de canal a ejecutar
			//regla
			if (m_poUpperPool->AddString("",iAuxUInt) ==M4_SUCCESS){
				m_pInstructions[iActualLine].m_iNumRegla=iAuxUInt;						
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				retCode=M4_ERROR;
				break;
			}
			//nodo
			if (m_poUpperPool->AddString(GetThisNodeId(),iAuxUInt) ==M4_SUCCESS){
				m_pInstructions[iActualLine].m_iNumNodo=iAuxUInt;						
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				retCode=M4_ERROR;
				break;
			}
			//Item
			if (m_poUpperPool->AddString(GetThisItemId(),iAuxUInt) ==M4_SUCCESS){
				m_pInstructions[iActualLine].m_iNumItem_Var.m_iNumItem=iAuxUInt;						
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				retCode=M4_ERROR;
				break;
			}
			
			iActualLine++;				 
			break;
				 }
			
		case 'O':{
			ret=InstallObject(pszActualLine+1,&m_pInstructions[iActualLine]);
			if (ret!=M4_SUCCESS){
				retCode=M4_ERROR;
				break;
			}   
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM;
			iActualLine++;
			break;
				 }
			
		case 'E':{
			ui16NumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_RETURN];
			
			m_pInstructions[iActualLine].m_iTipoIns=M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
			m_pInstructions[iActualLine].m_iNumFunc=ui16NumFunc;
			
			M4CL_SET_LEVEL_FLAG(m_pInstructions[iActualLine].m_Flags , 0);
			iActualLine++;
			break;
				 }
		}
		
		
		pszActualLine=GetNextLine(pszActualLine);
		
	}
	
	//finalmente y si todo va bien la damos por instalada
	if (retCode==M4_SUCCESS){
		m_bInstalled=M4_TRUE;
	}
	else{
		if (m_pInstructions!=NULL) { //borramos
			delete [] m_pInstructions;
			m_pInstructions=NULL;
		}
		m_NumInstructions=0;
	}
	
	if (m_pszTotalPolish!=NULL){
		delete [] m_pszTotalPolish;
		m_pszTotalPolish=NULL;
	}
    if (pszStoreString){
        delete [] pszStoreString;
		pszInUseStoreString = pszStoreString = 0;
		iStoreString  = 0;
    }
	return retCode;
	
}



//*************************
//Utiles
//*************************
m4return_t ClTempNitemFormula::InstallObject(m4pcchar_t ai_pszString, ClInstruction *ao_oInstruction){
	
	m4uint16_t ui16NumFunc;
	m4return_t	ret;
	
	switch (*ai_pszString){
		
	case 'P':{	//atributo
		ret=ScanLine(++ai_pszString,ao_oInstruction,M4_FALSE);
		
		if (ret != M4_SUCCESS){
			DumpDebugInfo (M4_MDFAC_WRONG_OBJECT_FORMAT, "attribute");
			return M4_ERROR;
		}
		
		if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) >1) { //hay que llamar a las funciones del nivel 2
			ui16NumFunc=M4CL_VM_FT_READATTRIBUTE2;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 2); //de momento solo hasta nivel 2!!!
		}
		else{
			ui16NumFunc=M4CL_VM_FT_READATTRIBUTE;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 1);
			
			//Añadimos a la lista de dependencias ->Los atributos de lectura generan TODOS dependencias
			AddConditionant (ao_oInstruction->m_poItem);
			
		}
		break;
			 }
	case 'T':{	//atributo-metodo
		ret=ScanLine(++ai_pszString,ao_oInstruction,M4_FALSE);
		if (ret != M4_SUCCESS){
			DumpDebugInfo (M4_MDFAC_WRONG_OBJECT_FORMAT, "attribute method");
			return M4_ERROR;
		}
		
		
		
		if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) >1) { //hay que llamar a las funciones del nivel 2
			ui16NumFunc=M4CL_VM_FT_EXECUTEATTRIBUTE2;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 2); //de momento solo hasta nivel 2!!!
		}
		else{
			ui16NumFunc=M4CL_VM_FT_EXECUTEATTRIBUTE;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 1);
			
			if (m_oAtributeMethodTable->GetGenDependencie(ao_oInstruction->m_iNumAtrib) ){
				//Añadimos a la lista de dependencias si procede
				AddConditionant (ao_oInstruction->m_poItem);
			}
			
		}
		break;
			 }
		
	case 'R':{
		ret=ScanLine(++ai_pszString,ao_oInstruction,M4_FALSE);
		
		if (ret != M4_SUCCESS){
			DumpDebugInfo (M4_MDFAC_WRONG_OBJECT_FORMAT, "item read");
			return M4_ERROR;
		}
		
		
		if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) >1) { //hay que llamar a las funciones del nivel 2
			ui16NumFunc=M4CL_VM_FT_PUSHITEM2;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 2);//de momento solo hasta nivel 2!!!
		}
		else{
			ui16NumFunc=M4CL_VM_FT_PUSHITEM;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 1);
			
			//Añadimos a la lista de dependencias
			AddConditionant (ao_oInstruction->m_poItem);
		}
		break;
			 }
		
	case 'A':{
		ret=ScanLine(++ai_pszString,ao_oInstruction,M4_FALSE);
		
		if (ret != M4_SUCCESS){
			DumpDebugInfo (M4_MDFAC_WRONG_OBJECT_FORMAT, "item write");
			return M4_ERROR;
		}
		
		
		if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) >1) { //hay que llamar a las funciones del nivel 2
			ui16NumFunc=M4CL_VM_FT_ASSIGNITEM2;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 2);//de momento solo hasta nivel 2!!!
		}
		else{
			ui16NumFunc=M4CL_VM_FT_ASSIGNITEM;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			M4CL_SET_LEVEL_FLAG(ao_oInstruction->m_Flags , 1);
			
			//Añadimos a la lista de asignados
			AddAssignedItem (ao_oInstruction->m_poItem);
			
		}
		break;
			 }
		
	case 'F':{
		ret=ScanLine(++ai_pszString,ao_oInstruction,M4_TRUE);
		
		
		if (ret != M4_SUCCESS){
			DumpDebugInfo (M4_MDFAC_WRONG_OBJECT_FORMAT, "item execution");
			return M4_ERROR;
		}
		
		//en scanline ya me han puesto de que nivel es el método
		
		if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) ==1) { 
			//para nivel 1
			ui16NumFunc=M4CL_VM_FT_EXECUTEITEM;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			
			//si es concepto y no lleva asociado fecha especifica
			//Añadimos a la lista de asignados
			if (m_poFormulaNitem!=NULL) //////////////////JIT. No se ejecuta con tramos.
				
				if (!M4CL_HAS_TRAME_FLAG (ao_oInstruction->m_Flags) &&
					ao_oInstruction->m_poItem->GetItem()->GetType()== M4CL_ITEM_TYPE_CONCEPT){
					AddExecutedConcept (ao_oInstruction->m_poItem);
				}
				
		}
		else if (M4CL_GET_LEVEL_FLAG(ao_oInstruction->m_Flags) ==2) { 
			//para nivel 2
			ui16NumFunc=M4CL_VM_FT_EXECUTEITEM2;
			ao_oInstruction->m_iNumFunc=ui16NumFunc;
			
			
		}
		else{ //de nivel superior a 2
			ao_oInstruction->m_iNumFunc=M4CL_L3_EXECUTE_ITEM;
		}
		
		
		break;
			 }
	}
	return M4_SUCCESS;
}







//*************************************************
//              U T I L E S  V A R I O S 
//*************************************************

m4return_t ClTempNitemFormula::ScanLine( m4pcchar_t ai_pszString, ClInstruction *ao_oInstruccion, m4bool_t ai_bNeedRule){
	
	m4char_t pszTemp[M4CL_MAX_LEN_NODE];
	m4char_t pszNodeName[M4CL_MAX_LEN_NODE], pszItemName [M4CL_MAX_LEN_ITEM];
	m4uint32_t iTemp,hItemJIT;
	m4uint32_t iLength ;
	m4uint32_t uiTemp;
	m4uint16_t ui16Temp;
	m4bool_t bIsFather;
	m4bool_t bGoOn,bHasFather;
	ClTempNitem *oTempItem;
	//ponemos todo a cero
	ao_oInstruccion->m_iNumItem_Var.m_iNumItem=0;
	ao_oInstruccion->m_iNumNodo=0;
    ao_oInstruccion->m_poNode=0;
	ao_oInstruccion->m_iNumItem_Var.m_iNumVar=0;
	ao_oInstruccion->m_iNumRegla=0;
	ao_oInstruccion->m_iNumAtrib=0;
	ao_oInstruccion->m_pcNameChannel=0;
	ao_oInstruccion->m_Flags=0;
	//Instruccion de Tipo ITEM
	ao_oInstruccion->m_iTipoIns = M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM;
	bIsFather=M4_FALSE;
	bGoOn=M4_TRUE;
	m4return_t ret;
	
    //nivel mínimo : 1
    M4CL_SET_LEVEL_FLAG(ao_oInstruccion->m_Flags,1);
	while (bGoOn){
		switch (*ai_pszString){
			
		case 'N':{	
			//el nodo
			iLength = _ReadString( ai_pszString + 1, ' ', pszTemp ) ;
			memcpy( pszNodeName, pszTemp, iLength + 1 ) ;
			ai_pszString += iLength + 2 ;  // salta el espacio
			
			
			//la fila
			iTemp = _ReadPositive( ai_pszString, ' ' ) ;
			
			if (iTemp>0) { //tiene row
				M4CL_SET_ROW_FLAG (ao_oInstruccion->m_Flags);
			}
			
			if (M4CL_GET_LEVEL_FLAG(ao_oInstruccion->m_Flags) > 1 ){
				//si es de nivel mayor que 1 en lugar del handle a nodo, item..
				//hay que guardar el nombre, ya que va a "salir" a un sitio donde 
				//no tienen sentido los handles
				
				if (m_poUpperPool->AddString(pszTemp,uiTemp) ==M4_SUCCESS){
					ao_oInstruccion->m_iNumNodo=uiTemp;						
				}
				else{
					DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
					return M4_ERROR;
				}
				
			}
			else{ //no es de nivel >1
				
				if (GetNodeAndIdHandle(pszNodeName, ao_oInstruccion->m_poNode, uiTemp) ==M4_SUCCESS){
					ao_oInstruccion->m_iNumNodo=uiTemp;						
				}
				else{
					return M4_ERROR;
				}
			}
			break;
				 }
			
		case 'I':
			{
				iLength = _ReadString( ai_pszString + 1, ' ', pszTemp ) ;
				memcpy( pszItemName, pszTemp, iLength + 1 ) ;
				ai_pszString += iLength + 2 ;  // salta el espacio
				
				
				if (M4CL_GET_LEVEL_FLAG(ao_oInstruccion->m_Flags) > 1 ){
					//si es de nivel mayor que 1 en lugar del handle a nodo, item..
					//hay que guardar el nombre, ya que va a "salir" a un sitio donde 
					//no tienen sentido los handles
					
					
					//miramos si tiene metido el nombre del canal
					if (ao_oInstruccion->m_pcNameChannel==0){ //metemos el nombre de este canal
						
						if (GetThisChannelIdHandle(uiTemp) ==M4_SUCCESS){
							ao_oInstruccion->m_pcNameChannel=uiTemp;						
						}
						else{
							DumpError (M4_MDFAC_WRONG_CHANNEL_INS, "");
							return M4_ERROR;
						}
						
						
					}
					
					//miramos si ya tiene el nombre del nodo
					if (ao_oInstruccion->m_iNumNodo==0){ //metemos el nombre de este nodo
						
						//metemos el nombre del nodo
						GetNodeAndIdHandle("",ao_oInstruccion->m_poNode, ao_oInstruccion->m_iNumNodo);
						strcpy(pszNodeName,GetThisNodeId(ao_oInstruccion->m_poNode) );
					}
					//metemos el nombre del item
					
					if (m_poUpperPool->AddString(pszTemp,uiTemp) ==M4_SUCCESS){
						ao_oInstruccion->m_iNumItem_Var.m_iNumItem=uiTemp;						
					}
					else{
						DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
						return M4_ERROR;
					}
					
				}
				else{ //nivel 1 
					
					if (ao_oInstruccion->m_iNumNodo==0){ //metemos el nombre de este nodo, aunque no hace falta (para debug)
						
						//metemos el nombre del nodo
						if (GetNodeAndIdHandle("",ao_oInstruccion->m_poNode, uiTemp) ==M4_SUCCESS){
							ao_oInstruccion->m_iNumNodo=uiTemp;						
						}
						else{
							DumpError (M4_MDFAC_WRONG_NODE_INS, "");
							return M4_ERROR;
						}
						strcpy(pszNodeName,"");
						
					}
					//metemos el item (puntero a la clase ClTempNItem)
					//JIT. Handle del item.
					
					if (m_poFormulaNitem!=NULL)
					{
						if (GetNitem(ao_oInstruccion->m_poNode,pszTemp,oTempItem) ==M4_SUCCESS){
							ao_oInstruccion->m_poItem=oTempItem;	
							M4CL_SET_ITEM_FLAG(ao_oInstruccion->m_Flags);
							
						}
						else{
							m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
							sprintf( acTemp, "%s&&%s", ao_oInstruccion->m_poNode->GetNodeId(), ao_oInstruccion->m_poNode->GetName() );
							DumpError(M4_MDFAC_WRONG_ITEM_HANDLE_INS, pszTemp, acTemp);
							return M4_ERROR;
						}
					}
					else 
					{
						if (GetNItemHandleforJIT(pszNodeName,pszTemp,hItemJIT)==M4_SUCCESS)
						{
							ao_oInstruccion->m_iNumItem_Var.m_iNumItem=hItemJIT;
							M4CL_SET_ITEM_FLAG(ao_oInstruccion->m_Flags);
						}
						else 
						{
							m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
							sprintf( acTemp, "%s(JIT)", pszTemp );
							DumpError(M4_MDFAC_WRONG_ITEM_HANDLE_INS, acTemp, pszNodeName);
							return M4_ERROR;
						}
					}
					
				}
				
				//miramos si tiene o no tramo
				if (*ai_pszString=='1') {//tiene tramo
					M4CL_SET_TRAME_FLAG(ao_oInstruccion->m_Flags);
				}
				
				ai_pszString++; //pasamos al final del token

				if( *ai_pszString != '\n' )
				{
					ai_pszString++ ;
				}
				
				break;
		}
		
		case 'A':{
			
			ao_oInstruccion->m_iNumAtrib= m4uint8_t( _ReadPositive( ai_pszString + 1, '\n' ) ) ;
			ai_pszString++; // saltamos la A
			break;
				 }
			
		case 'L': { //nivel del método
			iTemp = _ReadPositive( ai_pszString + 1, ' ' ) ;
			
			
			// jesus chapuza para salir al paso
            if( iTemp == 0 )
            {
                iTemp = 1 ;
            }
			
			M4CL_SET_LEVEL_FLAG(ao_oInstruccion->m_Flags ,(m4uint8_t)iTemp );
			ai_pszString++; // saltamos la L
			break;
			
				  }
			
		case 'R':{
			iLength = _ReadString( ai_pszString + 1, '\n', pszTemp ) ;
			ai_pszString += iLength + 1 ;  // salta el espacio
			
            if( *ai_pszString != '\n' )
            {
                ai_pszString++ ;
            }
			
			
            bHasFather=( (M4CL_HAS_FATHER_FLAG(ao_oInstruccion->m_Flags) !=0) ? M4_TRUE : M4_FALSE );
			
			if (M4CL_GET_LEVEL_FLAG(ao_oInstruccion->m_Flags) > 1 ){ //nivel mayor que 1
				//metemos el nombre de la regla en el pool
				if (m_poUpperPool->AddString(pszTemp,uiTemp) ==M4_SUCCESS){
					ao_oInstruccion->m_iNumRegla=uiTemp;						
				}
				else{
					DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
					return M4_ERROR;
				}
			}
			else{ //nivel 1
				
				if (m_poFormulaNitem!=NULL){
					ret=GetRuleOrder(ao_oInstruccion->m_poItem, pszTemp, bHasFather ,ui16Temp);
				}
				else{ //por nombre
					ret=GetRuleOrder(pszNodeName,pszItemName, pszTemp, bHasFather ,ui16Temp);
				}
				
				if (ret==M4_SUCCESS){
					ao_oInstruccion->m_iNumRegla=ui16Temp;	
				}
				else{
					return M4_ERROR;
				}
			}
			break;	
				 }
			
		case 'F':{
			ai_pszString++;
			bIsFather=M4_TRUE;
			M4CL_SET_FATHER_FLAG(ao_oInstruccion->m_Flags);
			break;
				 }
			
		case 'C':{
			
			iLength = _ReadString( ai_pszString + 1, ' ', pszTemp ) ;
			ai_pszString += iLength + 2 ;  // salta el espacio
			
			m4pcchar_t pccT3AliasInstance;
			if ( GetT3AliasInstance(pszTemp, pccT3AliasInstance) == M4_ERROR ){
				DumpError (M4_MDFAC_WRONG_T3_NODE_INSTANCE, pszTemp);
				return M4_ERROR;
			}
			
			if (m_poUpperPool->AddString(pccT3AliasInstance,uiTemp) ==M4_SUCCESS){
				ao_oInstruccion->m_pcNameChannel=uiTemp;						
				
			}
			else{
				DumpError (M4_MDFAC_ADD_TO_POOL_INS, "");
				return M4_ERROR;
			}
			//si nos viene indicando canal es de nivel 2 o superior
			M4CL_SET_LEVEL_FLAG(ao_oInstruccion->m_Flags ,max( 2, M4CL_GET_LEVEL_FLAG(ao_oInstruccion->m_Flags )  ));
			
			break;
				 }
			
		case '\n':{
			bGoOn=M4_FALSE;
			break;
				  }
			
		default: {
			ai_pszString++;
				 }
		}
	}
	
	if (ai_bNeedRule){
		//si no hay regla----------
		//Ponemos como regla la metaregla por número
		if (ao_oInstruccion->m_iNumRegla==0){
			if (M4CL_GET_LEVEL_FLAG(ao_oInstruccion->m_Flags) ==1 ){ //nivel 1
				
				bHasFather= ( (M4CL_HAS_FATHER_FLAG(ao_oInstruccion->m_Flags)!=0) ? M4_TRUE : M4_FALSE );
				
				if (m_poFormulaNitem!=NULL){
					ret=GetRuleOrder(ao_oInstruccion->m_poItem, "", bHasFather ,ui16Temp);
				}
				else{//jit->por nombre
					ret=GetRuleOrder(pszNodeName, pszItemName, "", bHasFather ,ui16Temp);
				}
				
				if (ret == M4_SUCCESS){
					ao_oInstruccion->m_iNumRegla=ui16Temp;	
				}
				else{
					return M4_ERROR;
				}
			}
		}
	}
	return M4_SUCCESS;
}


m4pcchar_t ClTempNitemFormula::GetNextLine( m4pcchar_t ai_pszBuffer){
	
	while (1){
		if (*ai_pszBuffer=='\n'){
			++ai_pszBuffer;
			if (*ai_pszBuffer=='\0')
				return NULL;
			else
				return ai_pszBuffer;
		}
		if (*ai_pszBuffer=='\0')
			return NULL;
		ai_pszBuffer++;
	}
	
}


//**************************************************
//Funciones de información sebre numeros de nodos...
//**************************************************


m4return_t	ClTempNitemFormula :: SetPolish( m4pchar_t ai_pcPolish )
{
	if( m_bPolishOwner == M4_TRUE )
	{
		if( m_pcPolish != NULL )
		{
			delete [] m_pcPolish ;
			m_pcPolish = NULL ;
		}
	}

	m_bPolishOwner = M4_TRUE ;
	m_pcPolish = ai_pcPolish ;

	return( M4_TRUE ) ;
}


m4return_t	ClTempNitemFormula :: Init( ClTempNitem *ai_poNitem, ClTempItemFormula *ai_poItemFormula, m4uint8_t ai_iLevel )
{
	
	m4return_t			iResult ;
    m4pcchar_t          pccString ;
    m4char_t            acFormulaId[ M4CL_MAX_NITEM_FORMULA_ID + 1 ] ;
	ClFactoryImp		*poFactory ;
	
	
	m_iLevel = ai_iLevel ;

	// La fórmula es de la Ti donde venga
	m_poTi = ai_poItemFormula->GetTi() ;
	
	acFormulaId[ 0 ] = 1 ;	// Si no hay posibilidad de llamar a la regla del padre esto lo ponemos siempre a 1
	acFormulaId[ 1 ] = ':' ;
	
	pccString = ai_poItemFormula->GetFormulaId() ;
	strcpy( acFormulaId + 2, pccString ) ;
	
	iResult = SetFormulaId( acFormulaId, M4_FALSE ) ;
	
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	
	
    m_poFormulaNitem = ai_poNitem ;
	
	if( m_poFormulaNitem != NULL )
	{
		poFactory = m_poFormulaNitem->GetChannel()->GetFactory() ;
		
		if( poFactory != NULL )
		{
			m_poFunctionTable = poFactory->_GetFunctionTable() ;
			m_oAtributeMethodTable = poFactory->_GetAtributeTable() ;
		}
	}
	
	
	m_iType = ai_poItemFormula->GetType() ;
    m_iOrigin = m_iType ;
	
    m_iIsMetarule = ai_poItemFormula->GetIsMetarule() ;
	m_iIsRetroactivity = ai_poItemFormula->GetIsRetroactivity()  ;
	m_iBatchType = ai_poItemFormula->GetBatchType() ;
	m_iIsEvent = ai_poItemFormula->GetIsEvent() ;
	m_iDBOrder = ai_poItemFormula->GetDBOrder() ;
	m_bIsDatabase = ai_poItemFormula->GetIsDatabase() ;
    m_iPriority = ai_poItemFormula->GetPriority() ;
	
    m_pcPolish = ai_poItemFormula->GetPolish() ;
    m_pccObject = ai_poItemFormula->GetFormObject() ;
    m_pccMethod = ai_poItemFormula->GetMethod() ;
	
    m_dStartDate = ai_poItemFormula->GetStartDate() ;
    m_dEndDate = ai_poItemFormula->GetEndDate() ;
	
	
	return( M4_SUCCESS ) ;
}


m4return_t	ClTempNitemFormula :: Init( ClTempNitem *ai_poNitem, ClTempNode *ai_poNode, m4int8_t ai_iType, m4pchar_t ai_pcPolish, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccMethod, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4uint8_t ai_iCreationType, m4uint8_t ai_iLevel )
{
	
	m4return_t			iResult ;
    m4pcchar_t          pccString ;
    m4char_t            acFormulaId[ M4CL_MAX_NITEM_FORMULA_ID + 1 ] ;
	ClFactoryImp		*poFactory ;
	
	
	m_iLevel = ai_iLevel ;

	// La fórmula es de la Ti del nodo donde estamos
	m_poTi = ai_poNode->GetTi() ;
	
	acFormulaId[ 0 ] = 1 ;	// Si no hay posibilidad de llamar a la regla del padre esto lo ponemos siempre a 1
	acFormulaId[ 1 ] = ':' ;
	
	m_iIsMetarule = 0 ;
	m_iBatchType = 0 ;

	switch( ai_iCreationType )
	{
		case M4_MDFAC_RULE_CREATION_TOTAL:

			pccString = "TOTALRULE" ;
			m_iBatchType = 2 ;
			m_bPolishOwner = M4_TRUE ;
			break ;

		case M4_MDFAC_RULE_CREATION_SYSTEM_METARULE:

			pccString = "METARULE" ;
			m_iIsMetarule = 1 ;
			m_bPolishOwner = M4_TRUE ;
			break ;

		case M4_MDFAC_RULE_CREATION_SYS_LOAD:

			pccString = "SYS_LOAD" ;
			break ;

		case M4_MDFAC_RULE_CREATION_SYS_PERSIST:

			pccString = "SYS_PERSIST" ;
			break ;

		case M4_MDFAC_RULE_CREATION_EXECUTEREPORT:

			ai_iCreationType = M4_MDFAC_RULE_CREATION_NORMAL ;
			pccString = "EXECUTEREPORT" ;
			break ;
	}

	
	strcpy( acFormulaId + 2, pccString ) ;
	iResult = SetFormulaId( acFormulaId, M4_FALSE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	
	
    m_poFormulaNitem = ai_poNitem ;
	
	if( m_poFormulaNitem != NULL )
	{
		poFactory = m_poFormulaNitem->GetChannel()->GetFactory() ;
		
		if( poFactory != NULL )
		{
			m_poFunctionTable = poFactory->_GetFunctionTable() ;
			m_oAtributeMethodTable = poFactory->_GetAtributeTable() ;
		}
	}
	
	m_iType = ai_iType ;
    m_iOrigin = m_iType ;
	
	m_iIsRetroactivity = 0 ;
	m_iIsEvent = 0 ;
	m_iDBOrder = 0 ;
	m_bIsDatabase = M4_FALSE ;
    m_iPriority = M4CL_RULE_MIN_PRIORITY ;
	
    m_pcPolish = ai_pcPolish ;
    m_pccObject = ai_pccObject ;
    m_pccMethod = ai_pccMethod ;
    m_dStartDate = ai_dStartDate ;
    m_dEndDate = ai_dEndDate ;
	
	m_iCreationType = ai_iCreationType ;
	
	return( M4_SUCCESS ) ;
}


//----------------------------------
//PreGetSize
//----------------------------------
m4return_t	ClTempNitemFormula::PreGetSize( void ) {
	
	
	m4return_t ret;
	

	//según el nivel vamos por un sitio u otro
    if (m_poFormulaNitem->GetItem()->GetMethodLevel() > 2){
		m_iType=M4CL_NO_LENGUAJE; //para que si no cambia no intentemos instalar lo que no es
		ret = PreinstallUpperLevels();
	}
	else{
		ret = PreinstalInternalLevels();
	}
	
	if (ret != M4_SUCCESS) {
		return( M4_ERROR ) ;
	}
	
	//ahora siempre es ln4 aunque el tipo puede quedar como ln4 (para ln4 y niveles superiores) o cpp 
	ret=InstallLN4(); //instalamos
	return ret;
}




//----------------------------------
//GetSize
//----------------------------------

m4return_t	ClTempNitemFormula::GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{
	
	
	// Fijos ======================================================================
	
	ao_iSize = M4_ROUND_TO_8( ao_iSize ) ;
	m_iHandle = ao_iSize ;
	
	ao_iSize += _GetFormulaSize();


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_FORMULAS, ao_iSize - m_iHandle ) ;
	}
	
	return M4_SUCCESS;
}



m4return_t	ClTempNitemFormula::Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{
	
	m4int32_t		i;
	m4uint8_t		iDateAsFlags ;
	m4uint32_t		iOffsetIns, iOffset;
	m4uint32_t		iHandle ;
	m4uint32_t		iFormulaSize ;
	m4uint32_t		iLevelAndInstructions ;
	m4double_t		dDays, dSeconds ;

	
    iOffsetIns  = m_iHandle ;	
	
	iFormulaSize = _GetFormulaSize() ;
	
	if( m_poTi != NULL ){
		iHandle = m_poTi->GetTiIdHandle() ;
	}
	else{
		// Es JIT
		iHandle = 0 ;
	}
	
	
	//Serializamos la Cabecera de la Formula:
    M4ClPackInt16( ai_pcBuffer, iOffsetIns, m_iNumLocalVars ) ;
    //parte de la cabecera para prioridad y debe-asignar
	if (m_iMustReturn ){
		M4CL_SET_FLAG(m_iPriority ,M4CL_MCR_CODE_ASSIGN_FLAG);
	}

	// Si es autogenerada, pero no remota
	if (m_iCreationType!=0 && m_bRemote==M4_FALSE){
		M4CL_SET_FLAG(m_iPriority ,M4CL_MCR_CODE_AUTOGENERATED_FLAG);
	}
    M4ClPackInt8 ( ai_pcBuffer, iOffsetIns, m_iPriority ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffsetIns, m_iType ) ;

	/* Bug 0069036
	Hay que serializar también el nivel de herencia de la regla
	Como no queremos gastar 4 bytes por regla lo vamos a meter
	"pegado" en la parte alta del número de instrucciones.
	Como el nivel ocupa 1 byte nos quedan 3 para el número de instrucciones,
	es decir 16777216 instrucciones. Si alguien pasa de eso que me avise.
	La más grande que he visto es del orden de 1000 instrucciones.
	*/
	iLevelAndInstructions = m_iLevel << 24 ;
	iLevelAndInstructions |= m_NumInstructions ;
	
	M4ClPackInt32( ai_pcBuffer, iOffsetIns, iLevelAndInstructions ) ;
	M4ClPackInt32( ai_pcBuffer, iOffsetIns, iFormulaSize ) ;
	
	
	//el ID de la TI donde se ha definido la regla
	M4ClPackInt32( ai_pcBuffer, iOffsetIns, iHandle + ai_iBufferSize) ;
	
	//Ejemplo:   ArrayOffsets	Instrucciones
	//				@@@@@@@ ####**####**####
	//Inicializamos los Offset: 
	// iOffsetIns = Offset en el Array de Offsets = @@@@@@
	// iOffset	  = Offset a la Instruccion = ####**####**####
    iOffsetIns	= M4_ROUND_TO_8( iOffsetIns ) ;	
	iOffset		= iOffsetIns + M4_ROUND_TO_8( m_NumInstructions * M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE ) ;
	
	//Serializamos cada instrucción
	for ( i=0; i<m_NumInstructions; i++ ) {
		
		if (m_pInstructions[i].m_bIsChar==M4_TRUE) //al datachar debe sumarsele ai_iBufferSize  para que contenga el verdadero offset
			m_pInstructions[i].m_uVariant.CharData += ( ai_iBufferSize + ai_iPoolSize ) ;
		
		//Segun el tipo de Instruccion
		//Con DEBUG empaquetamos el Offset + TipoInstruccion
		
		switch (m_pInstructions[i].m_iTipoIns) {
		case M4CL_MCR_CODE_INSTRUCTION_TYPE_SIMPLE	: {
			
			m4uint32_t iFlagsAsOffset;
			iFlagsAsOffset=0;
			
			M4CL_MCR_INS_EMB_SET_IS_SIMPLE(iFlagsAsOffset);
			
			M4CL_MCR_INS_EMB_SET_LEVEL(iFlagsAsOffset, M4CL_GET_LEVEL_FLAG(m_pInstructions[i].m_Flags) );
			
			iFlagsAsOffset+=m_pInstructions[i].m_iNumFunc;
			
			M4ClPackInt32( ai_pcBuffer, iOffsetIns, iFlagsAsOffset);
			
			
			break;
													  }
			
		case M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT	: {
			//Almacenamos Offset de la Instruccion (iOffset) y
			//actualizamos Offset del Array de Instrucciones (iOffsetIns)
			m4uint32_t iFlagsAsOffset=iOffset - iOffsetIns;
			if (m_pInstructions[i].m_bIsChar || m_pInstructions[i].m_bVariantAsInt){
				M4CL_MCR_INS_EMB_SET_VARIANT_INT(iFlagsAsOffset);
			}
			M4ClPackInt32( ai_pcBuffer, iOffsetIns,iFlagsAsOffset ) ;
			
			//Almacenamos la Instruccion propiamente dicha
			//Flags + Func + Variant
			M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_Flags ) ;
			M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumFunc ) ;	
			if (m_pInstructions[i].m_bIsChar || m_pInstructions[i].m_bVariantAsInt){
				M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_uVariant.CharData ) ;
			}
			else{
				//Relleno a 0 para ROUND_TO_8 bytes
				M4ClPackInt32( ai_pcBuffer, iOffset, 0 ) ;
				M4ClPackDouble( ai_pcBuffer, iOffset, m_pInstructions[i].m_uVariant.DoubleData) ;
			}
			break;
													  }
		case M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL	: {
			//Almacenamos Offset de la Instruccion (iOffset) y
			//actualizamos Offset del Array de Instrucciones (iOffsetIns)
			
			//en el lugar reservado al offset guardamos:
			//1er byte Combinación que indica la función, el nivel
			//2do byte->Vacio
			//3,4 byte: Argumento->Es de 16 bits (no puede ser un handle)
			m4uint32_t iFlagsAsOffset;
			iFlagsAsOffset=0;
			
			M4CL_MCR_INS_EMB_SET_IS_CONTROL(iFlagsAsOffset);
			
			iFlagsAsOffset|= ((m4uint8_t)m_pInstructions[i].m_iNumFunc)<<24;
			
			M4CL_MCR_INS_EMB_SET_LEVEL(iFlagsAsOffset, M4CL_GET_LEVEL_FLAG(m_pInstructions[i].m_Flags));
			
			if ( (m4int32_t)m_pInstructions[i].m_iNumItem_Var.m_iNumItem<0){
				M4CL_MCR_INS_EMB_SET_CONTROL_SIGN(iFlagsAsOffset);
				iFlagsAsOffset+=(m4uint16_t)(- ( (m4int32_t)m_pInstructions[i].m_iNumItem_Var.m_iNumItem) );
			}
			else{
				iFlagsAsOffset+=(m4uint16_t)m_pInstructions[i].m_iNumItem_Var.m_iNumItem;
			}
			
			M4ClPackInt32( ai_pcBuffer, iOffsetIns, iFlagsAsOffset);
			break;
													  }
		case M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM	: {
			//Almacenamos Offset de la Instruccion (iOffset) y
			//actualizamos Offset del Array de Instrucciones (iOffsetIns)
			M4ClPackInt32( ai_pcBuffer, iOffsetIns,(iOffset - iOffsetIns) ) ;
			if (M4CL_GET_LEVEL_FLAG(m_pInstructions[i].m_Flags) >1 ){ //nivel >1
				//Almacenamos la Instruccion propiamente dicha
				//Flags + Func + DatosItem:	
				M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_Flags ) ;
				M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumFunc ) ;
				// m_iNumItem_Var.m_iNumItem, m_iNumRegla, m_iNumNodo
				// m_pcNameChannel, m_iNumAtrib
				//m_iNumItem_Var es una union que contiene un signed y un unsigned. Empaquetamos el unsigned, pero da igual
				M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumItem_Var.m_iNumItem + ai_iBufferSize ) ;
				M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumRegla + ai_iBufferSize ) ;
				//contienen handles al pool de cadenas donde está su nombre. Habrá que sumarle el tam. del buffer
				//para que el handle quede correcto			
				M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumNodo + ai_iBufferSize ) ;
			}
			else{	//Nivel <=1  :  Todo es relativo al propio canal
				//m_iNumItem_Var es una union que contiene un signed y un unsigned. Empaquetamos el unsigned, pero da igual
				if (M4CL_HAS_ITEM_FLAG ( m_pInstructions[i].m_Flags) ){ //viene un puntero al TempItem y hay que obtener el handle y meterlo
					M4CL_RESET_ITEM_FLAG ( m_pInstructions[i].m_Flags);
					//Almacenamos la Instruccion propiamente dicha
					//Flags + Func + DatosItem:	
					M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_Flags ) ;
					M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumFunc ) ;
					// m_iNumItem_Var.m_iNumItem, m_iNumRegla, m_iNumNodo
					// m_pcNameChannel, m_iNumAtrib
					//JIT
					if (m_poFormulaNitem!=NULL)
						M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_poItem->GetHandle() ) ; 
					else 
						M4ClPackInt32(ai_pcBuffer,iOffset,m_pInstructions[i].m_iNumItem_Var.m_iNumItem);
					//
				}
				else{
					//Almacenamos la Instruccion propiamente dicha
					//Flags + Func + DatosItem:	
					M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_Flags ) ;
					M4ClPackInt16( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumFunc ) ;
					// m_iNumItem_Var.m_iNumItem, m_iNumRegla, m_iNumNodo
					// m_pcNameChannel, m_iNumAtrib
					M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumItem_Var.m_iNumItem ) ;
				}
				M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumRegla ) ;
				//Contienen el handle del item de verdad
				//M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumNodo ) ;
				//---Para porsi----
				if ( m_pInstructions[i].m_poNode ){
					M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_poNode->GetHandle()) ;
				}
				else{
					M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumNodo ) ;
				}
			}
			M4ClPackInt32( ai_pcBuffer, iOffset, m_pInstructions[i].m_pcNameChannel + ai_iBufferSize ) ;
			M4ClPackInt8 ( ai_pcBuffer, iOffset, m_pInstructions[i].m_iNumAtrib ) ;
			break;
													  }
		default: {
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%d", m_pInstructions[i].m_iTipoIns);
			DumpError (M4_MDFAC_NO_ILLEGAL_INSTRUCTION_SERIALIZE, acTemp);
			return( M4_ERROR ) ;
				 }
		} //end switch
		
		//Ajuste del Offset por cada Instruccion
		iOffset	= M4_ROUND_TO_8( iOffset ) ;
		
	} //end for
	
	
	
	//Metemos las fechas
	iDateAsFlags=CalcDatesByte();
	
	//empaquetamos según lo que tengamos-inicio
	if (M4CL_MCR_INS_HAS_START_DAYS (iDateAsFlags) ){
		iOffset	= M4_ROUND_TO_4( iOffset ) ;

		dSeconds=modf(m_dStartDate,&dDays);

		M4ClPackInt32 ( ai_pcBuffer, iOffset, (m4uint32_t)dDays ) ;

		if (M4CL_MCR_INS_HAS_START_DAYS_AND_SECONDS (iDateAsFlags) ){
			M4ClPackInt32 ( ai_pcBuffer, iOffset, (m4uint32_t)(dSeconds*M4_SECONDS_DAY) ) ;
		}
	}

	//empaquetamos según lo que tengamos-fecha fin
	if (M4CL_MCR_INS_HAS_END_DAYS (iDateAsFlags) ){
		iOffset	= M4_ROUND_TO_4( iOffset ) ;

		dSeconds=modf(m_dEndDate,&dDays);

		M4ClPackInt32 ( ai_pcBuffer, iOffset, (m4uint32_t)dDays ) ;

		if (M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS (iDateAsFlags) ){
			M4ClPackInt32 ( ai_pcBuffer, iOffset, (m4uint32_t)(dSeconds*M4_SECONDS_DAY) ) ;
		}
	}

	//metemos el byte de las fechas
	M4ClPackInt8 ( ai_pcBuffer, iOffset, iDateAsFlags ) ;



    return( M4_SUCCESS ) ;
}




// ---varias


//----------------------------
//GetItem
//----------------------------
m4return_t  ClTempNitemFormula ::GetNitem( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, ClTempNitem* &ai_rpoNitem ) const
{
	
    m4return_t      iResult ;
    ClTempNode      *poNode ;
	
	
	//JIT
	if( m_poFormulaNitem == NULL ){
		DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "FormulaNitem");
		return( M4_ERROR ) ;
	}
	
	
    iResult = GetNode( ai_pccNodeId, poNode ) ;
	
	if( iResult != M4_SUCCESS ){
		return( M4_ERROR ) ;
	}
	
	return GetNitem (poNode, ai_pccItemId, ai_rpoNitem ); //llamamos a la sobrecarga que recibe ya el poNode
    
}


//sobrecarga al que le pasan el puntero al nodo
m4return_t  ClTempNitemFormula ::GetNitem( ClTempNode* ai_poNode , m4pcchar_t ai_pccItemId, ClTempNitem* &ai_rpoNitem ) const
{
	
	m4uint8_t		iIsToCompile ;
	
	
	if( ai_poNode==0 )
	{
		DumpError (M4_MDFAC_WRONG_NODE_POINTER_ITEM_INS, ai_pccItemId);
		return( M4_ERROR ) ;
	}
	
	//JIT
	if( m_poFormulaNitem == NULL ){
		DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "FormulaNitem");
		return( M4_ERROR ) ;
	}
	
	
	
    ai_rpoNitem = ai_poNode->GetNitemById( ai_pccItemId, M4_TRUE ) ;
	
    if( ai_rpoNitem == NULL ){ //probamos como item de canal
		
		m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
		sprintf( acTemp, "Nitem# searching for <%s.%s>", ai_poNode->GetNodeId() , ai_pccItemId);
		DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, acTemp);
		return( M4_ERROR ) ;
	}
	
	
	iIsToCompile = ai_rpoNitem->GetIsToCompile() ;
	
	if( iIsToCompile == 0 )
	{
        DumpError (M4_MDFAC_NO_SUCH_ITEM_IN_VIEW, ai_pccItemId, ai_poNode->GetNodeId());
		return( M4_ERROR ) ;
	}
	
	
    return( M4_SUCCESS ) ;
}

//----------
//GetNode
//----------
m4return_t  ClTempNitemFormula::GetNode( m4pcchar_t ai_pccNodeId, ClTempNode* &ai_rpoNode ) const
{
	
	m4uint8_t		iIsToCompile ;
    ClTempNode      *poThisNode ;
    ClTempChannel   *poThisChannel ;
	
	
	if( m_poFormulaNitem == NULL ){
		DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "FormulaNitem");
		return( M4_ERROR ) ;
	}
	
    poThisNode = m_poFormulaNitem->GetNode() ;
	
	if( poThisNode == NULL ){
		DumpError (M4_MDFAC_WRONG_NODE_INS, "", NULL, M4_FALSE);
        ai_rpoNode = NULL ;
		return( M4_ERROR ) ;
	}
	
    if( ai_pccNodeId == NULL || *ai_pccNodeId == '\0' )
    {
        ai_rpoNode = poThisNode ;
        return( M4_SUCCESS ) ;
    }
	
    ai_rpoNode = poThisNode->GetAliasById( ai_pccNodeId, M4_TRUE ) ;
	
	if( ai_rpoNode != NULL )
	{
		iIsToCompile = ai_rpoNode->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			DumpError (M4_MDFAC_NO_SUCH_NODE_IN_VIEW, ai_pccNodeId);
			return( M4_ERROR ) ;
		}
		
		return( M4_SUCCESS ) ;
    }
	
    poThisChannel = poThisNode->GetChannel() ;
	
	if( poThisChannel == NULL ){
		DumpError (M4_MDFAC_WRONG_CHANNEL_INS, "", NULL, M4_FALSE);
		return( M4_ERROR ) ;
	}
	
    ai_rpoNode = poThisChannel->GetNodeByTiId( ai_pccNodeId, M4_TRUE, m_iLevel, m_poFormulaNitem ) ;
	
	if( ai_rpoNode != NULL )
	{
		iIsToCompile = ai_rpoNode->GetIsToCompile() ;
		
		if( iIsToCompile == 0 ){
			DumpError (M4_MDFAC_NO_SUCH_NODE_IN_VIEW, ai_pccNodeId);
			return( M4_ERROR ) ;
		}
		
		return( M4_SUCCESS ) ;
	}
	
    ai_rpoNode = poThisChannel->GetNodeById( ai_pccNodeId, M4_TRUE ) ;
	
	if( ai_rpoNode == NULL ){
		DumpError (M4_MDFAC_WRONG_NODE_INS, ai_pccNodeId);
		return( M4_ERROR ) ;
	}
	
	iIsToCompile = ai_rpoNode->GetIsToCompile() ;
	
	if( iIsToCompile == 0 )
	{
		DumpError (M4_MDFAC_NO_SUCH_NODE_IN_VIEW, ai_pccNodeId);
		return( M4_ERROR ) ;
	}
	
    return( M4_SUCCESS ) ;
}

//--------------------
//Rules
//--------------------
m4return_t  ClTempNitemFormula::GetRuleOrder( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, m4pcchar_t ai_pccRuleId, m4bool_t ai_bFather, m4uint16_t &ao_riOrder ) const
{
	
	m4return_t      iResult ;
	ClTempNitem     *poNitem ;
	
	
	
	if (m_poFormulaNitem!=NULL)
	{
		
		iResult = GetNitem( ai_pccNodeId, ai_pccItemId, poNitem ) ;
		
		if( iResult != M4_SUCCESS ){
			DumpError(M4_MDFAC_WRONG_ITEM_HANDLE_INS, ai_pccItemId, ai_pccNodeId);
			return( M4_ERROR ) ;
		}
		
		return GetRuleOrder (poNitem, ai_pccRuleId, ai_bFather, ao_riOrder); //llamamos a la sobrecarga con el poItem
	}
	else //para JIT 
	{
		m4uint32_t	hItem;
		
		iResult=GetNItemHandleforJIT( ai_pccNodeId, ai_pccItemId,hItem) ;
		if( iResult != M4_SUCCESS )
		{
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%s(JIT)", ai_pccItemId);
			DumpError(M4_MDFAC_WRONG_ITEM_HANDLE_INS, acTemp, ai_pccNodeId);
			return( M4_ERROR ) ;
		}
		
		ao_riOrder = 0; // EJECUTAMOS LA METAREGLA.
		iResult = M4_SUCCESS;
		
	}
	//
	return( iResult ) ;
}

//sobrecarga pasando el puntero al item
m4return_t  ClTempNitemFormula::GetRuleOrder(ClTempNitem* ai_poNitem , m4pcchar_t ai_pccRuleId, m4bool_t ai_bFather, m4uint16_t &ao_riOrder ) const
{
	
	m4return_t      iResult ;
	m4uint8_t       iInheritance ;
	ClTempRitem     *poRitem ;
	ClTempNode		*poNode ;
	
	if( ai_poNitem==0 ){
		DumpError (M4_MDFAC_WRONG_ITEM_POINTER_RULE_INS, ai_pccRuleId);
		return( M4_ERROR ) ;
	}
	
	if (m_poFormulaNitem!=NULL)
	{
		
		if( ai_poNitem == m_poFormulaNitem && strcmp( m_acFormulaId + 2, ai_pccRuleId ) == 0 )
		{
			if( ai_bFather == 1 )
			{
				iInheritance = *m_acFormulaId ;
			}
			/*else //control de recursividad
			{
			SETCODEF (M4_MDFAC_INVALID_CALL, ERRORLOG, "Invalid call. Recursive call. for Formula: %s, rule: %s", m_acFormulaId, ai_pccRuleId);
			return( M4_ERROR ) ;
		}*/
			else{
				iInheritance = 1 ;
			}
		}
		else
		{
			iInheritance = 1 ;
		}
		
		if( ai_bFather == 1 )
		{
			iInheritance++ ;
		}
		
		poRitem = ai_poNitem->GetRitem() ;
		poNode = ai_poNitem->GetNode() ;
		iResult = poRitem->GetFormulaOrderById( ai_pccRuleId, ai_poNitem, poNode, m_acFormulaId, m_poFormulaNitem, iInheritance, ao_riOrder ) ;
	}
	else { //no debe llamar a esta, sino a la de por nombres
		
		M4_ASSERT (0);
	}
	//
	return( iResult ) ;
}


//---------------------
//Nodes
//---------------------

m4return_t	ClTempNitemFormula::GetNodeIdHandle( m4pcchar_t ai_pccNodeId, m4uint32_t &ao_riHandle ) const
{
	
	ClTempNode      *poNode ;
	return GetNodeAndIdHandle(ai_pccNodeId, poNode, ao_riHandle); //el poNode se ignora
}


m4return_t	ClTempNitemFormula::GetNodeAndIdHandle( m4pcchar_t ai_pccNodeId, ClTempNode* &ai_rpoNode, m4uint32_t &ao_riHandle ) const
{
	
	m4return_t      iResult ;
	
	ai_rpoNode=0;
	//JIT
	if (m_poFormulaNitem!=NULL)
	{
		iResult = GetNode( ai_pccNodeId, ai_rpoNode ) ;
		
		if( iResult != M4_SUCCESS ){
			return( M4_ERROR ) ;
		}	
		
		ao_riHandle = ai_rpoNode->GetNodeIdHandle() ;
		
	}
	else 
	{
		ao_riHandle = m_poRecordSet->GetpNodeDef()->GetHandle();
		if (ai_pccNodeId==NULL||*ai_pccNodeId =='\0')
		{
			return M4_SUCCESS;
		}
		else
		{
			ao_riHandle = m_poCMCRforJIT ->GetNodeAliasNodeHandleByAlias(ao_riHandle,ai_pccNodeId, m_iLevel);
			if(ao_riHandle==0)
			{
				m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
				sprintf( acTemp, "%s (JIT)", ai_pccNodeId) ;
				DumpError (M4_MDFAC_WRONG_NODE_INS, acTemp);
				return( M4_ERROR ) ;
			}
		}
	}
	
	
	
	return( M4_SUCCESS ) ;
}

//
//GetThisNodeId
//
m4pcchar_t ClTempNitemFormula::GetThisNodeId( ClTempNode* ai_poNode/*=0*/) const
{
	
	m4return_t      iResult ;
	
	
	if (m_poFormulaNitem!=NULL)
	{
		if (!ai_poNode){ //si nos pasan el nodo->Mejor, lo usamos
			iResult = GetNode( "", ai_poNode ) ;
			
			if( iResult != M4_SUCCESS )
			{
				return( "" ) ;
			}
		}
		return ai_poNode->GetNodeId() ;
		
	}
	else
	{
		m4uint32_t hNodo;
		
		GetNodeIdHandle("",hNodo);
		
		return m_poCMCRforJIT->GetNodeId(hNodo);
	}
	
}


//
//GetThisitemId
//
m4pcchar_t ClTempNitemFormula::GetThisItemId( void) const{
	
	if (m_poFormulaNitem != 0){
		return m_poFormulaNitem->GetNitemId();
	}
	else{
		return m_poCMCRforJIT->GetItemId (m_hItemJIT);
	}
}


m4return_t  ClTempNitemFormula::GetThisChannelIdHandle( m4uint32_t &ao_riChannelId ) const
{
	
	m4return_t      iResult ;
	ClTempNode      *poNode ;
	ClTempChannel   *poChannel ;
	if (m_poFormulaNitem!=NULL)
	{
		
		iResult = GetNode( "", poNode ) ;
		
		if( iResult != M4_SUCCESS ){
			return( M4_ERROR ) ;
		}
		
		poChannel = poNode->GetChannel() ;
		
		if( poChannel == NULL ){
			DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "channel");
			return( M4_ERROR ) ;
		}
		
		ao_riChannelId = poChannel->GetChannelIdHandle() ;
	}
	else 
	{
		//¿Qué se guarda, el handle o la cadena?.
		ao_riChannelId= (m4uint32_t)m_poCMCRforJIT->GetChannelId();
	}
	
	
	return( M4_SUCCESS ) ;
}


m4pcchar_t  ClTempNitemFormula::GetThisChannelId( void ) const
{
	
	m4return_t      iResult ;
	ClTempNode      *poNode ;
	ClTempChannel   *poChannel ;
	if (m_poFormulaNitem!=NULL)
	{
		
		iResult = GetNode( "", poNode ) ;
		
		if( iResult != M4_SUCCESS )
		{
			return( "" ) ;
		}
		
		poChannel = poNode->GetChannel() ;
		
		if( poChannel == NULL ){
			DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "channel");
			return( "" ) ;
		}
		
		return poChannel->GetChannelId() ;
	}
	else 
	{
		
		return m_poCMCRforJIT->GetChannelId();
	}
	
	
}



m4return_t  ClTempNitemFormula::GetT3AliasInstance( m4pcchar_t ai_pccT3Alias, m4pcchar_t &ao_pccT3AliasInstance ) const
{
	
	ClTempNode      *poThisNode ;
	ClTempT3Alias   *poTempT3Alias;
	
	if( m_poFormulaNitem != NULL ) 
	{
		poThisNode = m_poFormulaNitem->GetNode() ;
		
		if( poThisNode == NULL )
		{
			DumpError (M4_MDFAC_WRONG_NODE_INS, "", NULL, M4_FALSE);
			return( M4_ERROR ) ;
		}
		
		if( ai_pccT3Alias == NULL || *ai_pccT3Alias == '\0' )
		{
			DumpError (M4_MDFAC_T3_ALIAS_ID_INS, "");
			return( M4_ERROR ) ; 
		}
		
		poTempT3Alias = poThisNode->GetT3AliasById( ai_pccT3Alias, M4_TRUE ) ;
		
		if( poTempT3Alias == NULL )
		{
			return( M4_ERROR ) ;
		}
		
		ao_pccT3AliasInstance = poTempT3Alias->GetInstanceId( );
	}
	else  //jit
	{
		m4uint32_t hNodo;
		
		
		GetNodeIdHandle("",hNodo);
		ao_pccT3AliasInstance=m_poCMCRforJIT->GetNodeT3AliasInstanceIdByAlias(hNodo,ai_pccT3Alias);
		//sabemos que se instala justo despues de compilar,
		//y si compila es que la instancia existe.
		//Nos deviuelve el Alias, porque lo que le hemos pasado puede
		//ser el nombre de una instancia que se ha definido con DefineInstance, y el MCR no 
		//tiene ni P. Idea de ella. Ese alias, estará entonces en la lista de definidos para jit.
		
	}
	
	
	return( M4_SUCCESS ) ;
}



//-----------------------------------------------------------
//Generar código de conexiones previas
//-----------------------------------------------------------

m4return_t	ClTempNitemFormula::GenerateConnectingCodePre (m4pchar_t ao_pszConnectingCodePre , 
														   m4int32_t ai_BufferSize){
	
	m4uint32_t		i , iNumOfConnected;
	m4int32_t       j; //cambio para evitar el warning de minus operator
	m4uint16_t		iConnectedNumArgs, iNumItemArgs;
	m4int8_t		ParamAction[M4CL_MAX_PARAM];
	m4char_t		pszFunctionName[M4CL_MAX_NOMBRE_FUNC + 1];
	m4uint16_t		iNumItemArgsByRef	;
	ClTempItem		*poThisItem, *poConnectedItem;
	ClTempRitem		*poThisRitem;
	ClTempRitem		*poConnectedRitem;
	ClTempNitem		*poConnectedNItem;
	ClTempNode              *poNode ;
	ClTempNitemConn			*poConector;
	StArgumentMappig_t		*stArgMap;
	ClTempArgument			*poArgument;
	ClTempConnector		*poNodeConector;
	m4pcchar_t		pccConnectedItemId;
	m4bool_t			bHasAfter;
	m4uint8_t			iItemType;
	m4uint8_t			iIsToCompile;
	
	bHasAfter=M4_FALSE;
	
	//item 
	poThisItem=m_poFormulaNitem -> GetItem () ;
	poThisRitem=m_poFormulaNitem -> GetRitem () ;
	
	//número de argumentos del item
	iNumItemArgs		= poThisItem -> GetNumberOfArguments ();
	iNumItemArgsByRef	= poThisItem -> GetNumberOfReferedArguments ();
	
	iNumOfConnected		= poThisRitem -> GetNumberOfConnectors ();
	
	//**********
	//***Previos
	//**********
	
	if (iNumOfConnected==0) return M4_SUCCESS; //si no hay nada que hacer salimos
	
	for (i=0;i<iNumOfConnected;i++){
		
		poConector = poThisRitem->GetConnectorByPosition( (m4uint16_t)i );
		
		iIsToCompile = poConector->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}
		
		//item conectado
		poConnectedNItem  = poConector -> GetSonNitem();
		
		iIsToCompile = poConnectedNItem->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}
		
		poConnectedItem   = poConnectedNItem -> GetItem () ;
		poConnectedRitem   = poConnectedNItem -> GetRitem () ;
		
		iItemType=poConnectedItem->GetType() ;
		if ( iItemType!= M4CL_ITEM_TYPE_METHOD && iItemType!= M4CL_ITEM_TYPE_CONCEPT){
			//si no es un metodo lo que tiene conectado 
			//probamos con el siguiente
			continue; 
		}
		
		if ( poConector ->GetPrecedence() == M4CL_ITEM_CONNECTOR_BEFORE){
			//si es de los previos
			
			// Para que el stepper pare antes de cada conectado
			if( m_bRemote == M4_FALSE )
			{
				strcat( ao_pszConnectingCodePre, "C0\n" ) ;
			}

			//argumentos del item connectado
			iConnectedNumArgs = poConnectedItem	 -> GetNumberOfArguments();

			//si tenemos algo conectado y somos de arg. variables->salimos con error
			/* Bug 0104301
			Sólo si el conectado tiene argumentos
			*/
			if (iConnectedNumArgs>0 && poThisItem->GetVariableArguments()!=0){
				DumpError (M4_MDFAC_CANT_CONNECT_ITEMS, "");
				return M4_ERROR;
			}
			
			//1º-meter los argumentos que necesite, teniendo en cuenta el mapeo
			for (j=0 ; j < iConnectedNumArgs ; j++){
				
				stArgMap = poConector -> GetArgMappingByPosition ( (m4uint16_t)j );
				poArgument = poConnectedItem -> GetArgumentByPosition ( (m4uint16_t)j );
				
				if ( stArgMap->m_iMappingType != M4CL_ARGUMENT_MAPPING_INPUT){
					DumpError (M4_MDFAC_WRONG_ARG_TYPE_MAPPING, "");
					return M4_ERROR;
				}
				else{ //hay que meter el argumento en la pila
					
					if (stArgMap->m_iFatherRealOrder > iNumItemArgs ){
						m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
						sprintf( acTemp, "%d",stArgMap->m_iFatherRealOrder);
						DumpError (M4_MDFAC_WRONG_ARG_MAPPING, acTemp);
						return M4_ERROR;
					}
					
					//****PUSH -iNumMappedArg-1 ---Crear instrucción
					GenerateInstructionPush(ao_pszConnectingCodePre  , -iNumItemArgs + (stArgMap->m_iFatherRealOrder) );	
					
					//según el tipo del mapeado y del original, al acabar habrá que hacer unas 
					//cosas u otras. Ahora determinamos que se hace y lo guardamos para 
					//luego generar las instrucciones correspondientes
					if ( stArgMap->m_iFatherArgumentType == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE   
						&& poArgument-> GetType () ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE  ) {
						
						ParamAction[j]=M4CL_PARAM_ACTION_ASSIGN+iNumItemArgs - (stArgMap->m_iFatherRealOrder); //hay que asignar el arg. que nos dejará en la pila; 
						//al argumento original		
					}
					else if ( stArgMap->m_iFatherArgumentType == M4CL_ITEM_ARGUMENT_TYPE_INPUT   
						&&  poArgument-> GetType () ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE  ) {
						
						ParamAction[j] = M4CL_PARAM_ACTION_PURGE	; //hay que purgar el arg. que nos dejará en la pila
					}
					else{
						ParamAction[j] = M4CL_PARAM_ACTION_NONE	;
					}
				}
			}
			
			
			//metemos el handle del item a ejecutar, que será un parametro de las funciones de propagate
			//Será el último argumento, así será mas facil llamar a la propagada, porque
			//los argumentos quedan en la pila 
			pccConnectedItemId=poConnectedItem->GetItemId();
			GenerateInstructionPushString ( ao_pszConnectingCodePre  ,  pccConnectedItemId);	
			//metemos el nombre del nodo
			
			// ojo con los items de canal
			
			poNode = poConnectedNItem->GetNode() ;
			
			if( poNode == 0 ){ //item de canal
				
				DumpError (M4_MDFAC_WRONG_NODE_POINTER_CONN, "");
				return( M4_ERROR ) ;
			}
			
			pccConnectedItemId=poNode->GetNodeId();
			GenerateInstructionPushString( ao_pszConnectingCodePre, pccConnectedItemId);	

			//metemos el CsType
			GenerateInstructionPushNum( ao_pszConnectingCodePre, poConector->GetCsType() );

			//metemos el SPIN
			GenerateInstructionPushNum( ao_pszConnectingCodePre, poConector->GetSpin() );

			//como la función que realiza la propagación es de argumentos variables
			//hay que meter en la polaca el número de argumentos
			GenerateInstructionPushNum( ao_pszConnectingCodePre, iConnectedNumArgs + 4);	
			
			
			//2º-Llamada a función que realiza la propagación
			strcpy (pszFunctionName , "PROPAGATE");
			
			//tipo de relación entre nodos
			poNodeConector = poConector->GetConnector();
			
			if( poNodeConector == NULL )
			{
				strcat (pszFunctionName , "FREE");
			}
			else
			{
				switch (poNodeConector->GetType()){
				case M4CL_NODE_CONNECTOR_TYPE_RB:
					{
						strcat (pszFunctionName , "RB");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_BB:
					{
						strcat (pszFunctionName , "BB");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_FREE:
					{
						strcat (pszFunctionName , "FREE");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_AUTO:
					{
						strcat (pszFunctionName , "FREE");
						break;
					}
				}
			}
			
			//tipo del metodo origen
			switch (poThisRitem -> GetScope () ){
			case M4CL_ITEM_SCOPE_NODE:
				{
					strcat (pszFunctionName , "N");
					break;
				}
			case M4CL_ITEM_SCOPE_BLOCK:
				{
					strcat (pszFunctionName , "B");
					break;
				}
			case M4CL_ITEM_SCOPE_REGISTER:
				{
					strcat (pszFunctionName , "R");
					break;
				}
			}

			//tipo del metodo destino
			switch (poConnectedRitem -> GetScope () ){
			case M4CL_ITEM_SCOPE_NODE:
				{
					strcat (pszFunctionName , "N");
					break;
				}
			case M4CL_ITEM_SCOPE_BLOCK:
				{
					strcat (pszFunctionName , "B");
					break;
				}
			case M4CL_ITEM_SCOPE_REGISTER:
				{
					strcat (pszFunctionName , "R");
					break;
				}
			}
			
			
			//****Llamada a función con todos los datos que haya que meter....->Nivel, número de args...
			//Crear instrucción
			GenerateInstructionFunctionCall( ao_pszConnectingCodePre  , pszFunctionName );	
			
			
			//3º-Asignamos a nuestros argumentos, los argumentos que por referencia que estén en
			//la pila, o si los de la principal eran por valor los purgamos
			
			for (j=0;j<iConnectedNumArgs;j++){
				if (ParamAction[j] == M4CL_PARAM_ACTION_PURGE){
					///***Generar instrucción de purgar pila
					GenerateInstructionPurge(ao_pszConnectingCodePre  , 1);
					
				}
				else if (ParamAction[j] >= M4CL_PARAM_ACTION_ASSIGN){
					
					///***Generar instrucción de Asignar a -ParamAction[j]
					GenerateInstructionAssign( ao_pszConnectingCodePre  , -ParamAction[j]);
				}
			}
			//4º-Purgamos el resultado
			
			///***Generar instrucción de purgar pila
			GenerateInstructionPurge( ao_pszConnectingCodePre  , 1);
			
		}
		else{ //si no es antes ==> ES de posterior
			bHasAfter=M4_TRUE; //hal menos hay un método posterior, será necesario guardar las variables
		}
	}
	
	if (bHasAfter){
		//hacemos una copia de todos los argumentos
		for (j=0; j < iNumItemArgsByRef ; j++){
			GenerateInstructionPush (ao_pszConnectingCodePre, -(j+1) ); //metemos el argumento
			GenerateInstructionAssign (ao_pszConnectingCodePre, m_iNumLocalVars+ iNumItemArgsByRef -(j+1)); //lo asignamos a la varible recien creada
			//**Ojo. Aunque ahora estamos aumentando el número de variables locales
			//no hace falta modificar la variable m_iNumLocalVars porque al instalarse la
			//formula se contarán las variables que se usan.
			//tambien se podría hacer ahora, pero sería más lio a la
			//hora de generar el código posterior, porque habría que usar dos variables...
		}
	}
	
	
	return M4_SUCCESS;
}


//-----------------------------------------------------------
//Generar código de conexiones posteriores
//-----------------------------------------------------------

m4return_t	ClTempNitemFormula::GenerateConnectingCodePost (m4pchar_t ao_pszConnectingCodePost , 
															m4int32_t ai_BufferSize){
	
	
	m4uint32_t		j, iNumOfConnected;
    m4int32_t       i; //cambio para evitar el warning de minus operator
	m4uint16_t		iConnectedNumArgs, iNumItemArgs;
	m4int8_t		ParamAction[M4CL_MAX_PARAM];
	m4char_t		pszFunctionName[M4CL_MAX_NOMBRE_FUNC + 1];
	m4uint16_t		iNumItemArgsByRef	;
	ClTempItem		*poThisItem, *poConnectedItem;
	ClTempRitem		*poThisRitem;
	ClTempRitem		*poConnectedRitem;
	ClTempNitem		*poConnectedNItem;
    ClTempNode              *poNode ;
	ClTempNitemConn			*poConector;
	StArgumentMappig_t		*stArgMap;
	ClTempArgument			*poArgument, *poTempArg;
	ClTempConnector		*poNodeConector;
	m4pcchar_t			pccConnectedItemId;
	m4bool_t			bHasAfter;
	m4uint8_t			iItemType;
	m4uint8_t			iIsToCompile;
	m4bool_t			bCommentAdded = M4_FALSE ;
	
	
	//item 
	poThisItem=m_poFormulaNitem -> GetItem () ;
	poThisRitem=m_poFormulaNitem -> GetRitem () ;
	
	//número de argumentos del item
	iNumItemArgs		= poThisItem -> GetNumberOfArguments ();
	iNumItemArgsByRef	= poThisItem -> GetNumberOfReferedArguments ();
	
	iNumOfConnected		= poThisRitem -> GetNumberOfConnectors ();
	bHasAfter=M4_FALSE;
	
	
	if (iNumOfConnected==0) return M4_SUCCESS; //si no hay nada que hacer salimos
	
	
	bHasAfter = M4_FALSE ;
	
	for (i=0;i< (m4int32_t)iNumOfConnected;i++){
		
		poConector = poThisRitem->GetConnectorByPosition( (m4uint16_t)i );
		
		iIsToCompile = poConector->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}
		
		//item conectado
		poConnectedNItem = poConector->GetSonNitem() ;
		
		iIsToCompile = poConnectedNItem->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}

		poConnectedItem = poConnectedNItem->GetItem() ;
		poConnectedRitem = poConnectedNItem->GetRitem() ;
		
		iItemType = poConnectedItem->GetType() ;

		if( iItemType!= M4CL_ITEM_TYPE_METHOD && iItemType!= M4CL_ITEM_TYPE_CONCEPT )
		{
			//Si no es método se sigue 
			continue; 
		}
		
		if( poConector->GetPrecedence() == M4CL_ITEM_CONNECTOR_AFTER )
		{
			bHasAfter = M4_TRUE ;
			break ;
		}
	}
	
	if (bHasAfter){
		//a)los argumentos por referencia que ha dejado el método "original"
		//los guardamos en su respectivo sitio
		for (i=iNumItemArgs; i> 0; i--){
			poTempArg=poThisItem->GetArgumentByPosition	( (m4uint16_t)i-1);
			if (poTempArg->GetType() ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){
				//asignamos a la variable que corresponda

				if( bCommentAdded == M4_FALSE )
				{
					if( m_bRemote == M4_FALSE )
					{
						strcat( ao_pszConnectingCodePost, "C0\n" ) ;
					}
					bCommentAdded = M4_TRUE ;
				}

				GenerateInstructionAssign(ao_pszConnectingCodePost  , -(i));	 // - [(i-1)+1]
			}
		}

		if( bCommentAdded == M4_FALSE )
		{
			if( m_bRemote == M4_FALSE )
			{
				strcat( ao_pszConnectingCodePost, "C0\n" ) ;
			}
			bCommentAdded = M4_TRUE ;
		}

		//b)Guardamos el resultado en la variable reservada al efecto
		GenerateInstructionAssign(ao_pszConnectingCodePost  , m_iNumLocalVars + iNumItemArgsByRef + 1);	
	}

	//**************
	//***Posteriores
	//**************
	
	
	for (i=0;i< (m4int32_t)iNumOfConnected;i++){
		
		poConector = poThisRitem->GetConnectorByPosition( (m4uint16_t)i );
		
		iIsToCompile = poConector->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}
		
		//item conectado
		poConnectedNItem  = poConector -> GetSonNitem();
		
		iIsToCompile = poConnectedNItem->GetIsToCompile() ;
		
		if( iIsToCompile == 0 )
		{
			continue ;
		}
		
		poConnectedItem   = poConnectedNItem -> GetItem () ;
		poConnectedRitem   = poConnectedNItem -> GetRitem () ;
		
		iItemType=poConnectedItem->GetType() ;
		if ( iItemType!= M4CL_ITEM_TYPE_METHOD && iItemType!= M4CL_ITEM_TYPE_CONCEPT ){
			//si no es un metodo lo que tiene conectado 
			//probamos con el siguiente
			continue; 
		}
		
		
		if ( poConector ->GetPrecedence() == M4CL_ITEM_CONNECTOR_AFTER){
			
			// Para que el stepper pare antes de cada conectado
			if( bCommentAdded == M4_FALSE )
			{
				if( m_bRemote == M4_FALSE )
				{
					strcat( ao_pszConnectingCodePost, "C0\n" ) ;
				}
			}
			else
			{
				bCommentAdded = M4_FALSE ;
			}

			//argumentos del item connectado
			iConnectedNumArgs = poConnectedItem	 -> GetNumberOfArguments();
			
			//si tenemos algo conectado y somos de arg. variables->salimos con error
			/* Bug 0104301
			Sólo si el conectado tiene argumentos
			*/
			if (iConnectedNumArgs>0 && poThisItem->GetVariableArguments()!=0){
				DumpError (M4_MDFAC_CANT_CONNECT_ITEMS, "");
				return M4_ERROR;
			}
			
			//1º-meter los argumentos que necesite, teniendo en cuenta el mapeo
			for (j=0 ; j < iConnectedNumArgs ; j++){
				
				stArgMap = poConector -> GetArgMappingByPosition ( (m4uint16_t)j );
				poArgument = poConnectedItem -> GetArgumentByPosition ( (m4uint16_t)j );
				
				if (stArgMap->m_iFatherRealOrder > iNumItemArgs ){
					m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
					sprintf( acTemp, "%d",stArgMap->m_iFatherRealOrder);
					DumpError (M4_MDFAC_WRONG_ARG_MAPPING, acTemp);
					return M4_ERROR;
				}
				
				
				switch (stArgMap->m_iMappingType ){
				case M4CL_ARGUMENT_MAPPING_INPUT:
					{
						//si el argumento en el item es por referencia habrá que leerlo
						//de la variable en la que lo hemos copiado
						//si es input, lo leemos de donde los argumentos (como los output)
						if (stArgMap->m_iFatherArgumentType == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){ 
							GenerateInstructionPush(ao_pszConnectingCodePost  , stArgMap->m_iFatherRealOrder + m_iNumLocalVars);	
						}
						else{ // es M4CL_ITEM_ARGUMENT_TYPE_INPUT
							GenerateInstructionPush(ao_pszConnectingCodePost  , -iNumItemArgs + (stArgMap->m_iFatherRealOrder) );	
						}
						break;
					}

				case M4CL_ARGUMENT_MAPPING_REFERENCE: //arg. a la salida
					{
						GenerateInstructionPush(ao_pszConnectingCodePost  , -iNumItemArgs + (stArgMap->m_iFatherRealOrder) );	
						break;
					}
					
				case M4CL_ARGUMENT_MAPPING_OUTPUT: //con el resultado
					{
						GenerateInstructionPush(ao_pszConnectingCodePost  , m_iNumLocalVars + iNumItemArgsByRef + 1);	
						break;
					}
				}
				
				
				//según el tipo del mapeado y del original, al acabar habrá que hacer unas 
				//cosas u otras. Ahora determinamos que se hace y lo guardamos para 
				//luego generar las instrucciones correspondientes
				if ( stArgMap->m_iFatherArgumentType == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE   
					&& poArgument-> GetType () ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE  ) {
					
					ParamAction[j]=M4CL_PARAM_ACTION_ASSIGN+iNumItemArgs - (stArgMap->m_iFatherRealOrder); //hay que asignar el arg. que nos dejará en la pila; 
					//ParamAction[j]=M4CL_PARAM_ACTION_ASSIGN+stArgMap->m_iFatherRealOrder+1; //hay que asignar el arg. que nos dejará en la pila
					
					//al argumento original		
				}
				else if ( stArgMap->m_iFatherArgumentType == M4CL_ITEM_ARGUMENT_TYPE_INPUT   
					&&  poArgument-> GetType () ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE  ) {
					
					ParamAction[j] = M4CL_PARAM_ACTION_PURGE	; //hay que purgar el arg. que nos dejará en la pila
				}
				else{
					ParamAction[j] = M4CL_PARAM_ACTION_NONE	;
				}
			}
			
			//metemos el handle del item a ejecutar, que será un parametro de las funciones de propagate
			//Será el último argumento, así será mas facil llamar a la propagada, proque
			//los argumetnos quedan en la pila 
			pccConnectedItemId = poConnectedItem->GetItemId();
			GenerateInstructionPushString ( ao_pszConnectingCodePost  ,  pccConnectedItemId);	
			//metemos el nombre del nodo
			
			// ojo con los items de canal
			
            poNode = poConnectedNItem->GetNode() ;
			
            if( poNode == NULL ){
				
				DumpError (M4_MDFAC_WRONG_NODE_POINTER_CONN, "");
				return( M4_ERROR ) ;
            }
			
			pccConnectedItemId=poNode->GetNodeId();
			GenerateInstructionPushString( ao_pszConnectingCodePost, pccConnectedItemId);	

			//metemos el CsType
			GenerateInstructionPushNum( ao_pszConnectingCodePost, poConector->GetCsType() );

			//metemos el SPIN
			GenerateInstructionPushNum( ao_pszConnectingCodePost, poConector->GetSpin() );	

			//como la función que realiza la propagación es de argumentos variables
			//hay que meter en la polaca el número de argumentos
			GenerateInstructionPushNum( ao_pszConnectingCodePost, iConnectedNumArgs + 4);	
			
			
			//2º-Llamada a función que realiza la propagación
			strcpy (pszFunctionName , "PROPAGATE");
			
			//tipo de relación entre nodos
			poNodeConector = poConector->GetConnector ();
			
            if( poNodeConector == NULL )
            {
				strcat (pszFunctionName , "FREE");
            }
            else
            {
				switch (poNodeConector->GetType() ){
				case M4CL_NODE_CONNECTOR_TYPE_RB:
					{
						strcat (pszFunctionName , "RB");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_BB:
					{
						strcat (pszFunctionName , "BB");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_FREE:
					{
						strcat (pszFunctionName , "FREE");
						break;
					}
				case M4CL_NODE_CONNECTOR_TYPE_AUTO:
					{
						strcat (pszFunctionName , "FREE");
						break;
					}
				}
            }
			
			//tipo del metodo origen
			switch (poThisRitem -> GetScope () ){
			case M4CL_ITEM_SCOPE_NODE:
				{
					strcat (pszFunctionName , "N");
					break;
				}
			case M4CL_ITEM_SCOPE_BLOCK:
				{
					strcat (pszFunctionName , "B");
					break;
				}
			case M4CL_ITEM_SCOPE_REGISTER:
				{
					strcat (pszFunctionName , "R");
					break;
				}
				
			}

			//tipo del metodo destino
			switch (poConnectedRitem -> GetScope () ){
			case M4CL_ITEM_SCOPE_NODE:
				{
					strcat (pszFunctionName , "N");
					break;
				}
			case M4CL_ITEM_SCOPE_BLOCK:
				{
					strcat (pszFunctionName , "B");
					break;
				}
			case M4CL_ITEM_SCOPE_REGISTER:
				{
					strcat (pszFunctionName , "R");
					break;
				}
			}
			
			
			//****Llamada a función con todos los datos que haya que meter....->Nivel, número de args...
			//Crear instrucción
			GenerateInstructionFunctionCall( ao_pszConnectingCodePost  , pszFunctionName );	
			
			
			
			//3º-Asignamos a nuestros argumentos, los argumentos que por referencia que estén en
			//la pila, o si los de la principal eran por valor los purgamos
			
			for (j=0;j<iConnectedNumArgs;j++){
				if (ParamAction[j] == M4CL_PARAM_ACTION_PURGE){
					///***Generar instrucción de purgar pila
					GenerateInstructionPurge(ao_pszConnectingCodePost  , 1);
				}
				else if (ParamAction[j] >= M4CL_PARAM_ACTION_ASSIGN){
					
					///***Generar instrucción de Asignar a -ParamAction[j]
					GenerateInstructionAssign( ao_pszConnectingCodePost  , -ParamAction[j]);
				}
			}
			//4º-Purgamos el resultado
			
			///***Generar instrucción de purgar pila
			GenerateInstructionPurge( ao_pszConnectingCodePost  , 1);
		}
	}
	
	if (bHasAfter){
		//c)Leemos de la variable reservada al efecto el resultado y la metemos en la pila
		GenerateInstructionPush(ao_pszConnectingCodePost  , m_iNumLocalVars + iNumItemArgsByRef + 1);	
		
		//d)Leemos los argumentos por referencia y los metemos en la pila
		for (i=0; i< iNumItemArgs; i++){
			poTempArg=poThisItem->GetArgumentByPosition	((m4uint16_t)i);
			if (poTempArg->GetType() ==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){
				//asignamos a la variable que corresponda
				GenerateInstructionPush(ao_pszConnectingCodePost  , -(i+1));	
			}
		}
	}

	return M4_SUCCESS;
}


//----------------------------------------------------
//Constar las instrucciones, para preinstalar en LN4
//----------------------------------------------------

m4return_t  ClTempNitemFormula ::CountInstructionsLN4 ( m4pcchar_t ai_Polish){
	
	
	m4int32_t iSourceCodeLine, iActualLine;
	m4uint32_t iNumEtiqueta;
	
	//Inicializamos contadores
	iSourceCodeLine=1;
	iActualLine=0;
	
	memset( m_NumInstructionsType, 0, sizeof( m4int32_t ) * M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES ) ;
	m_iNumVariantsAsInt=0;	
	
	
	//1º- HAcemos una pasada 
	//para saber donde están etiquetas e instrucciones
	
	while (ai_Polish!=NULL){
		switch(*ai_Polish){
			
		case 'L':{
			iNumEtiqueta = _ReadPositive( ai_Polish + 1, '\n' ) ;
			
			if (iNumEtiqueta>m_iLabelsSize-1){
				m_iLabelsSize*=2;

				// jcr. Y si la etiqueta aún es más grande?
				if( iNumEtiqueta >= m_iLabelsSize )
				{
					m_iLabelsSize = iNumEtiqueta + 1 ;
				}

				m_aiLabels.reserve(m_iLabelsSize); //como accedemos directamente, y sin hacer push_back, el gilipollas no se entera de que los elementos asignados son de verdad. Con resize le ponemos el tamaño bueno
				m_aiLabels.resize(m_iLabelsSize);
			}
			
			m_aiLabels[iNumEtiqueta]=iActualLine;
			break;	//No la contamos como instruccion
				 }
			
		case 'C':	{
			//Para instalar el comentario (Stepper)
			//contará como instrucción así que incrementamos
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL];
			break; //Ahora la contamos como instrucción
					}
			
		case 'V':	{
			//numero de variables
			m_iNumLocalVars = m4uint16_t( _ReadPositive( ai_Polish + 1, '\n' ) ) ;
			
			break;	//No la contamos como instruccion
					}
		case 'O':	{//si es un OR habra que incrementar el espacio
			//para los conditionants
			if ( *(ai_Polish+1) =='R' ){
				IncrementConditionantsDelta();
			}
			//si es un OA reservamso sitio para el asignado
			
			if ( *(ai_Polish+1) =='A'){
				IncrementAssignedItemDelta();
			}
			
			//si es ejecutado y es concepto lo añadimos incrementamos
			if ( *(ai_Polish+1) =='F' ){
				IncrementExecutedConceptDelta();
			}
			//contará como instrucción así que incrementamos
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM];
			
			break;
					}
		case 'H': {
			//contará como instrucción así que incrementamos
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM];
			break;
				  }				
		case 'F': {
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_SIMPLE];
			break;
				  }
			
		case 'D':
		case 'S':
		case 'N': {
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT];
			break;
				  }
		case 'j':
		case 'J':
		case 'A':
		case 'R':
		case 'P':
		case 'E': { //case  j,J,A,R,P,E
			iActualLine++;
			++ m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL];
			break;
				  }
		default:  {
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%c", *ai_Polish );
			DumpError (M4_MDFAC_NO_ITEM_FORMULA_PREGETSIZE, acTemp);
			return( M4_ERROR ) ;
				  }
		} // end switch
		ai_Polish=GetNextLine(ai_Polish);
		
	}
	
	m_NumInstructions=iActualLine;

	return M4_SUCCESS;
}

void ClTempNitemFormula ::PreGetNumVar(const m4char_t *ai_Polish){
	
	while (ai_Polish!=NULL){
		if (*ai_Polish=='V'){
			//numero de variables
			m_iNumLocalVars = m4uint16_t( _ReadPositive( ai_Polish + 1, '\n' ) ) ;
			
			return;
		}
		else{
			ai_Polish=GetNextLine(ai_Polish);
		}
	}
}


//-------------------------------
//Para los condicionantes
//-------------------------------
void    ClTempNitemFormula::IncrementConditionantsDelta( void )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		poThisRitem->AddConditionantDelta( 1 ) ;
	}
	
}

m4return_t  ClTempNitemFormula::AddConditionant( ClTempNitem *ai_poConditionant )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		return( poThisRitem->AddConditionant( ai_poConditionant ) ) ;
	}
	
	return M4_SUCCESS;
}
//-----------------------------------------------------
//Para los items que son asignados dentro de una formula
//-----------------------------------------------------
m4return_t  ClTempNitemFormula::AddAssignedItem( ClTempNitem *ai_poConditionant )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		return( poThisRitem->AddAssigned( ai_poConditionant ) ) ;
	}
	
	return M4_SUCCESS;
}


void    ClTempNitemFormula::IncrementAssignedItemDelta( void )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		poThisRitem->AddAssignedDelta( 1 ) ;
	}
}

//-----------------------------------------------------------
//Para los conceptos que son ejecutados dentro de una formula
//-----------------------------------------------------------
m4return_t  ClTempNitemFormula::AddExecutedConcept( ClTempNitem *ai_poConditionant )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		return( poThisRitem->AddExecuted( ai_poConditionant ) ) ;
	}
	
	return M4_SUCCESS;
}


void    ClTempNitemFormula::IncrementExecutedConceptDelta( void )
{
	ClTempRitem		*poThisRitem;
	
	//JIT
    if (m_poFormulaNitem!=NULL)
	{
		//
		poThisRitem=m_poFormulaNitem -> GetRitem () ;
		poThisRitem->AddExecutedDelta( 1 ) ;
	}
}




//------------------
//Size
//------------------
m4uint32_t	ClTempNitemFormula:: _GetFormulaSize()
{
	m4uint32_t iSize ;
	m4uint8_t  iDateAsFlags;

    // Cabecera de la Formula
    iSize = M4CL_MCR_CODE_FIXED_SIZE ;
	iSize = M4_ROUND_TO_8( iSize ) ;
	
	// Stream de offsets a las Instrucciones de tamaño variable
	iSize += m_NumInstructions * M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE;
	iSize = M4_ROUND_TO_8( iSize ) ;
	
	// Stream de instrucciones de tamaño variable
	iSize += (m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT]-m_iNumVariantsAsInt) * M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT	+
		m_iNumVariantsAsInt * M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT_AS_INT +
		m_NumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM   ] * M4CL_MCR_CODE_INSTRUCTION_SIZE_ITEM ;

	
	iDateAsFlags=CalcDatesByte();
	
	//si llevamos la fecha de inicio
	if (M4CL_MCR_INS_HAS_START_DAYS (iDateAsFlags) ){
		iSize	= M4_ROUND_TO_4( iSize ) ;
		iSize  += 4; 
		
		if (M4CL_MCR_INS_HAS_START_DAYS_AND_SECONDS (iDateAsFlags) ){
			iSize += 4; 
		}
	}
	
	//Si llevamos la fecha de fin
	if (M4CL_MCR_INS_HAS_END_DAYS (iDateAsFlags) ){
		iSize	= M4_ROUND_TO_4( iSize ) ;
		iSize  += 4; 
		
		if (M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS (iDateAsFlags) ){
			iSize += 4; 
		}
	}

	iSize ++; //el byte de las fechas
	
	return iSize;
}


//-------------------------------------------------------
//jit world
//-------------------------------------------------------


void ClTempNitemFormula::InitforJIT( IFuncTableDesc *ai_poFunctionTable, IAtributeFuncTable *ai_oAtributeMethodTable, m4uint32_t ai_hItem, m4uint8_t ai_iLevel, ClAccessRecordSet *ai_poRecordSet,ClCompiledMCR *ai_poCMCR,m4uint8_t ai_iType,m4uint8_t ai_iPriority,m4date_t ai_dStart,m4date_t ai_dEnd)
{
	m4return_t ret=M4_SUCCESS;
	
	m_poRecordSet=ai_poRecordSet;
	m_poCMCRforJIT=ai_poCMCR;
	
	m_iType=ai_iType;
	m_bIsDatabase = M4_FALSE ;
	m_iPriority=ai_iPriority;
	m_dStartDate=ai_dStart;
	m_dEndDate=ai_dEnd;
	m_iOrigin=m_iType;
	m_hItemJIT=ai_hItem;
	m_iLevel = ai_iLevel ;
	
	m_poFunctionTable = ai_poFunctionTable ;
	m_oAtributeMethodTable = ai_oAtributeMethodTable ;
}


m4return_t ClTempNitemFormula::CountInstructionsLN4forJIT( m4pchar_t ai_pszPolish,m4int32_t &ao_iNumInstruct)
{
	
	m4return_t ret=M4_SUCCESS;
	
	m_pcPolish=ai_pszPolish;
	ret=CountInstructionsLN4(ai_pszPolish);
	
	if (ret!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	ao_iNumInstruct=m_NumInstructions;
	return ret;
}	


m4return_t ClTempNitemFormula::GetNItemHandleforJIT( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId,m4uint32_t &ao_iItemHandle) const
{
	m4uint32_t	hNode=0;
	m4return_t ret=M4_SUCCESS;
	
	ret=GetNodeIdHandle(ai_pccNodeId,hNode);
	if( ret != M4_SUCCESS )
	{
		m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
		sprintf( acTemp, "%s (JIT)", ai_pccNodeId ) ;
		DumpDebugInfo (M4_MDFAC_WRONG_NODE_INS, acTemp);
		return( M4_ERROR ) ;
	}
	
	
	ao_iItemHandle=m_poCMCRforJIT->GetNodeItemHandleById(hNode,ai_pccItemId);
	if (ao_iItemHandle==0)
	{
		m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
        sprintf( acTemp, "%s(JIT)",ai_pccItemId);
		DumpError (M4_MDFAC_WRONG_NODE_POINTER_ITEM_INS, acTemp, ai_pccNodeId);
		return( M4_ERROR ) ;
	}
	
	return M4_SUCCESS;
}


//---------------------------------
// Check definitions
//---------------------------------
m4return_t	ClTempNitemFormula:: CheckCppDefinition(void)
{
	
	//si es de tipo CPP nos aseguramos que los argumentos que dice que
	//recibe y los que recibe la función del ejecutor sean "compatibles" en número
	
	m4uint16_t iNumPassedArg, i;
	m4uint16_t iId;
	m4int8_t iNumRealArgs, i8Type, iArgType,fFlags;
	m4char_t acParamDesc[M4CL_MAX_PARAM];		
	m4uint8_t	iVarArg;
	m4return_t ret;
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	if (m_poFormulaNitem){
		iNumPassedArg = m_poFormulaNitem ->GetItem()->GetNumberOfArguments();
		iVarArg		  = m_poFormulaNitem ->GetItem()->GetVariableArguments();
	}
	else{ //se lo preguntamos al metacanal (JIT)
		iNumPassedArg = m_poCMCRforJIT->GetItemNumberOfArguments(m_hItemJIT);
		iVarArg		  = m_poCMCRforJIT->GetItemVariableArguments(m_hItemJIT);
	}
	
	
	
	strcpy(acName,m_pccMethod);
	strupr(acName);
	
	
	ret=m_poFunctionTable->GetId(acName, iId, iNumRealArgs, acParamDesc, i8Type);
    if (ret!=M4_SUCCESS){
        DumpError (M4_MDFAC_FUNCTION_NOT_FOUND, m_pccMethod);
		return ret;
    }
	
	//si la función no es mappeable->salimos con error
	fFlags=m_poFunctionTable->GetFlags(iId);
	if (M4CL_HAS_FLAG(fFlags, M4CL_FLAG_FN_ITEM_NOT_MAPPEABLE) ){
		DumpError(M4_MDFAC_NOT_MAPPEABLE_FUNCTION, acName);
		return M4_ERROR;
	}
	
	//si la función tiene activado el flag de no asignar, quitamos el MustReturn
	if (M4CL_HAS_FLAG (fFlags, M4CL_FLAG_FN_NO_RETURN)){
		m_iMustReturn=0;
	}
	
	//miramos si ambos son de argumentos variables (los dos o ninguno)
	if  (iNumRealArgs < 0 && iVarArg==0 ) {
		DumpError (M4_MDFAC_WRONG_PARAM_NUM_INS_1, m_pccMethod);
		return M4_ERROR;
	}
	
	if  (iNumRealArgs>=0 && iVarArg != 0) {
		DumpError (M4_MDFAC_WRONG_PARAM_NUM_INS_2, m_pccMethod);
		return M4_ERROR;
	}
	
	
	//comprobamos argumentos 
	if (iNumRealArgs < 0 && (iNumRealArgs != -(M4CL_MAX_PARAM+1) )  ){ //si es de argumentos variables la definición del item debe contener >= número que la función c++
		if ( -iNumRealArgs > iNumPassedArg){
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%d", -iNumRealArgs );
			DumpError (M4_MDFAC_WRONG_PARAM_NUM_INS_3, m_pccMethod, acTemp);
			return M4_ERROR;
		}
	}
	
	if (iNumRealArgs >= 0){
		if ( iNumRealArgs != iNumPassedArg){
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%d", iNumRealArgs);
			DumpError (M4_MDFAC_WRONG_PARAM_NUM_INS_4, m_pccMethod, acTemp);
			return M4_ERROR;
		}
		
	}

	// Fix bug 0073592
	// it should be the minimum for both limits
	m4uint32_t iMaxTypeLen = strlen(acParamDesc);
	m4uint32_t iNArgsToCheck = (iNumPassedArg > iMaxTypeLen) ? iMaxTypeLen : iNumPassedArg;
	
	//comprobado el número vamos a los tipos (referencia / input)
	for (i=0 ; i< iNArgsToCheck ; ++i){
		if (m_poFormulaNitem){
			iArgType = m_poFormulaNitem ->GetItem()->GetArgumentByPosition(i)->GetType();
		}
		else{
			iArgType = m_poCMCRforJIT->GetItemArgumentTypeByPosition( m_hItemJIT, i );
		}
		
		if (acParamDesc[i]=='R' && iArgType!= M4CL_ITEM_ARGUMENT_TYPE_INPUT ) {
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%d", i);
			DumpError (M4_MDFAC_WRONG_PARAM_INPUT_INS, acTemp, m_pccMethod);
			return M4_ERROR;
		}
		if (acParamDesc[i]=='r' && iArgType!= M4CL_ITEM_ARGUMENT_TYPE_REFERENCE ) {
			m4char_t acTemp[ M4CL_TEMP_BUFFER_SIZE ] ;
			sprintf( acTemp, "%d", i);
			DumpError (M4_MDFAC_WRONG_PARAM_REF_INS, acTemp, m_pccMethod);
			return M4_ERROR;
		}
		
	}
	
	return M4_SUCCESS;
	
}

//------------------------------------------------
//Chequeo para ver si debe instalarse para remoto
//------------------------------------------------
m4return_t  ClTempNitemFormula::CheckForRemoteExecution		(m4bool_t &ao_bRemote){
	
	//tres condiciones para ejecución remota:
	//a) Canal front
	//b) Item mapped
	//c) O que el canal sea de depuración del stepper (canal back con todos los métodos mapped).
	
	if (m_poFormulaNitem != NULL)
	{
		/* Bug 0150731
		Ya no se tienen metacanales de debug, son de tipo back
		// Bug 0051846 y 0059918.
		// Cuando estamos creando un canal de depuración vamos a hacer el ítem EXECUTELN4JIT mapped.
		// Con esto conseguimos que todo lo que se ejecute en el inmediate window se haga en el servidor.
		// Antes se hacían todos los ítems mapped, pero esto es problemático porque perdemos información
		// sobre las fórmulas (ej. el número de variables, el código en las autogeneradas, ...)
		if (iChannelCsType == M4CL_CSTYPE_DEBUG) {
			const m4bool_t bIsExecuteLN4JitItem = strcmp("EXECUTELN4JIT", m_poFormulaNitem->GetNitemId());
			if (bIsExecuteLN4JitItem == 0) {
				ao_bRemote = M4_TRUE;
				return M4_SUCCESS;
			}
		}
		*/

		//item mapped
		if (m_poFormulaNitem->GetCsType() != M4CL_CSTYPE_MAPPED)
		{
			ao_bRemote = M4_FALSE; 
			return M4_SUCCESS;
		}

		//canal front
		ClTempChannel* poChannel = m_poFormulaNitem->GetChannel() ;

		if( poChannel == NULL )
		{
			DumpDebugInfo (M4_MDFAC_BAD_POINTER_INS, "channel# checking remore execution");
			return( M4_ERROR ) ;
		}

		if (poChannel->GetCsType() == M4CL_CSTYPE_FRONT)
		{
			ao_bRemote = M4_TRUE;
			return M4_SUCCESS;
		}
	}
	else
	{ //JIT
		if (m_poCMCRforJIT->GetItemCsType(m_hItemJIT) == M4CL_CSTYPE_MAPPED && m_poCMCRforJIT->GetChannelCsType() == M4CL_CSTYPE_FRONT )
		{
			/* Bug 0150731
			Ya no se tienen metacanales de debug, son de tipo back
			iChannelCsType == M4CL_CSTYPE_DEBUG) {
			*/
			ao_bRemote = M4_TRUE;
			return M4_SUCCESS;
		}
	}
	
	ao_bRemote = M4_FALSE;
	return M4_SUCCESS;
}

m4bool_t	ClTempNitemFormula::HasLN4VariableArgs(void)
{
	
	if (m_poFormulaNitem){
		return m_poFormulaNitem ->GetItem()->GetVariableArguments()!= 0 ?M4_TRUE:M4_FALSE;
	}
	else{ 
		
		//En código JIT solo se ejecutan items LN4 de argumentos fijos, por lo que retornamos siempre FALSE
		return M4_FALSE;
	}
}


//-----------------------------------------
//Generar codigo on-line para las formulas 
//que se generan on-the-fly
//-----------------------------------------

m4return_t	ClTempNitemFormula::GenerateInstructionPush	(m4pchar_t ao_pszConnectingCode , m4int32_t ai_iNumVar){
	
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	sprintf(pszLine, "R%d\n",ai_iNumVar);
	strcat (ao_pszConnectingCode , pszLine);
	return M4_SUCCESS;
}

m4return_t	ClTempNitemFormula::GenerateInstructionPushString (m4pchar_t ao_pszConnectingCode , m4pcchar_t ai_string){
	
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	sprintf(pszLine, "S%s\n",ai_string);
	strcat (ao_pszConnectingCode , pszLine);
	return M4_SUCCESS;
}

m4return_t	ClTempNitemFormula::GenerateInstructionPushNum	(m4pchar_t ao_pszConnectingCode , m4int32_t ai_iNum){
	
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
    m4char_t pszNumber[M4CL_MAX_LINEA_POLACA];
    m4uint32_t iLong;
	
    sprintf(pszNumber, "%.8lf",(double)ai_iNum);
	
    iLong=strlen (pszNumber);
    while (pszNumber[iLong-1] == '0' && pszNumber[iLong-1] !='.')
        --iLong;
	
    sprintf(pszLine,"N%.*s\n",iLong+1,pszNumber);
	strcat (ao_pszConnectingCode , pszLine);
	return M4_SUCCESS;
}


m4return_t	ClTempNitemFormula::GenerateInstructionPurge	(m4pchar_t ao_pszConnectingCode , m4uint32_t ai_iNumPurges){
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	
	sprintf(pszLine, "P%d\n",ai_iNumPurges);
	strcat (ao_pszConnectingCode , pszLine);
	return M4_SUCCESS;
	
}


m4return_t	ClTempNitemFormula::GenerateInstructionAssign (m4pchar_t ao_pszConnectingCode , m4int32_t ai_iNumVar){
	
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	
	sprintf(pszLine, "A%d\n",ai_iNumVar);
	strcat (ao_pszConnectingCode , pszLine);
	return M4_SUCCESS;
	
	
}


m4return_t	ClTempNitemFormula::GenerateInstructionReturn (m4pchar_t ao_pszConnectingCode ){
	
	
	strcat (ao_pszConnectingCode , "E\n");
	return M4_SUCCESS;
	
}


m4return_t	ClTempNitemFormula::GenerateInstructionFunctionCall(m4pchar_t ao_pszConnectingCode , m4pcchar_t ai_pszFuncName){
	
	m4return_t ret;
	m4uint16_t iNumFunc;
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	
	strcpy(acName,ai_pszFuncName);
	strupr(acName);
	
	ret=m_poFunctionTable -> GetId(acName, iNumFunc);
	if (ret!=M4_SUCCESS) {
		DumpError (M4_MDFAC_FUNCTION_NOT_FOUND, ai_pszFuncName);
		return ret;
	}
	
	sprintf(pszLine, "F%d\n",iNumFunc);
	strcat (ao_pszConnectingCode , pszLine);				
	
	return M4_SUCCESS;
				
}




m4return_t   ClTempNitemFormula::GenerateInstructionItemLevelCall(m4pchar_t ao_pszConnectingCode){
	
	
	m4char_t pszLine[M4CL_MAX_LINEA_POLACA];
	
	m4uint32_t iLevel=m_poFormulaNitem->GetItem()->GetMethodLevel();
	m4pcchar_t pszItemName = m_poFormulaNitem->GetItem()->GetItemId();
	sprintf (pszLine, "OF L%d N%s 0 I%s 0\n" ,iLevel, GetThisNodeId(), pszItemName );
	strcat (ao_pszConnectingCode , pszLine);				
	
	return M4_SUCCESS;
	
}

m4return_t   ClTempNitemFormula::RePushVars	 (m4pchar_t ai_pszMidCode){
	
	ClTempItem	*poThisItem		=m_poFormulaNitem->GetItem () ;
	m4uint16_t	iNumItemArgs	=poThisItem->GetNumberOfArguments ();
	m4uint16_t	i;
	
	
	
	for (i=iNumItemArgs;i > 0; --i){
		GenerateInstructionPush(ai_pszMidCode, -i);
	}
	return  M4_SUCCESS;
	
}


//-------------------------------
//DumpError
//-------------------------------
void		ClTempNitemFormula::DumpError(m4uint32_t ai_errorCode, m4pcchar_t ai_pccMessage1, m4pcchar_t ai_pccMessage2/*=NULL*/, m4bool_t ai_bShowItem/*=M4_TRUE*/) const
{
	g_oChLog<<BeginError(ai_errorCode);
	
	if (ai_bShowItem){
		g_oChLog << GetThisItemId() << GetThisNodeId() << GetThisChannelId() ;
	}
	g_oChLog << ai_pccMessage1;

	if( ai_pccMessage2 != NULL )
	{
		g_oChLog<< ai_pccMessage2;
	}
	g_oChLog << EndLog;
}


void		ClTempNitemFormula::DumpDebugInfo(m4uint32_t ai_errorCode,m4pcchar_t ai_pccMessage) const
{
#ifdef _DEBUG
	g_oChLog<<BeginDebug(ai_errorCode)<<GetThisItemId()<<GetThisNodeId()<<GetThisChannelId()<<ai_pccMessage<<EndLog;
#endif
}

//---------------------------------------------------------------------------------------------
//RUN TIME GENERATED CODE 
//---------------------------------------------------------------------------------------------
m4return_t	ClTempNitemFormula::SetInstructionsRTGC (ClInstruction *ai_pInstructions, m4uint32_t ai_iNumInstructions,  m4uint32_t *ai_piInsTypes, m4uint32_t ai_iInsTypesNum){
	
	if (ai_iInsTypesNum>M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES){
		return M4_ERROR;
	}
	
	memcpy( m_NumInstructionsType, ai_piInsTypes, sizeof( m4int32_t ) * ai_iInsTypesNum ) ;
	m_iNumVariantsAsInt=0;	
	
	m_NumInstructions=ai_iNumInstructions;
	m_pInstructions=ai_pInstructions;
	return M4_SUCCESS;
}




// Funciones varias ===========================================================

m4bool_t	ClTempNitemFormula :: IsEqual( ClSlicedElement *ai_poElement )
{
	
	ClTempNitemFormula	*poFormula = ( ClTempNitemFormula* ) ai_poElement ;
	
	// Si son las dos metareglas son iguales
	if( poFormula->GetIsMetarule() == 1 )
	{
		if( m_iIsMetarule == 1 )
		{
			return( M4_TRUE ) ;
		}
		
		return( M4_FALSE ) ;
	}
	
	if( m_iIsMetarule == 1 )
	{
		return( M4_FALSE ) ;
	}
	
	m4pcchar_t	pccString1 = GetFormulaId() - 1 ;
	m4pcchar_t	pccString2 = poFormula->GetFormulaId() - 1 ;
	
	if( *pccString1 != *pccString2 )
	{
		return( M4_FALSE ) ;
	}
	
    m4uchar_t uiSize1 = *pccString1 ;
	if( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) != 0 )
	{
		return( M4_FALSE ) ;
	}
	
	return( M4_TRUE ) ;
}


m4return_t	ClTempNitemFormula :: CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo )
{
	
	m4uint8_t			iIsRetroactivity ;
	m4int32_t			iDBOrder ;
    m4char_t			acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t			acEndDate  [ M4_SECOND_END + 1 ] ;
	ClTempNitem			*poNitem ;
	ClTempNitemFormula	*poFormula ;
	
	
	poFormula = ( ClTempNitemFormula* ) ai_poElement ;
	
	
	iDBOrder = poFormula->GetDBOrder() ;
	
	if( m_iDBOrder != iDBOrder )
	{
		poNitem = ( ClTempNitem* ) ai_pvInfo ;
		ClDateToString( GetStartDate(), acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( GetEndDate(),   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_RULE_ORDER, GetFormulaId() + 2 << acStartDate << acEndDate << m_iDBOrder << iDBOrder << LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( poNitem->iM4Obj(), poNitem->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}
	
	
	iIsRetroactivity = poFormula->GetIsRetroactivity() ;
	
	if( m_iIsRetroactivity != iIsRetroactivity )
	{
		poNitem = ( ClTempNitem* ) ai_pvInfo ;
		ClDateToString( GetStartDate(), acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( GetEndDate(),   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_INCOHERENT_RULE_RETRO, GetFormulaId() + 2 << acStartDate << acEndDate << m_iIsRetroactivity << iIsRetroactivity << LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( poNitem->iM4Obj(), poNitem->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}
	
	
	return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitemFormula :: CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo )
{
	return( M4_SUCCESS ) ;
}


m4uint8_t	ClTempNitemFormula :: CalcDatesByte(void){

	m4uint8_t iDateAsFlags=0;
	m4double_t dSeconds, dDays;
	dSeconds=modf(m_dStartDate,&dDays);

	if (m_dStartDate == M4CL_PLUS_INFINITE_IN_JULIAN){
		M4CL_MCR_INS_SET_START_PLUS_INF (iDateAsFlags);
	}
	else if (m_dStartDate == M4CL_MINUS_INFINITE_IN_JULIAN){
		M4CL_MCR_INS_SET_START_MINUS_INF (iDateAsFlags);
	}
	else if (dSeconds==0.0){
		M4CL_MCR_INS_SET_START_DAYS (iDateAsFlags);
	}
	else{
		M4CL_MCR_INS_SET_START_DAYS_AND_SECONDS (iDateAsFlags);
	}
	

	dSeconds=modf(m_dEndDate,&dDays);

	if (m_dEndDate == M4CL_PLUS_INFINITE_IN_JULIAN){
		M4CL_MCR_INS_SET_END_PLUS_INF (iDateAsFlags);
	}
	else if (m_dEndDate == M4CL_MINUS_INFINITE_IN_JULIAN){
		M4CL_MCR_INS_SET_END_MINUS_INF (iDateAsFlags);
	}
	else if (dSeconds==0.0){
		M4CL_MCR_INS_SET_END_DAYS (iDateAsFlags);
	}
	else{
		M4CL_MCR_INS_SET_END_DAYS_AND_SECONDS (iDateAsFlags);
	}

	return iDateAsFlags;
}