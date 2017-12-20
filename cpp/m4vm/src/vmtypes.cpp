//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                vmtypes.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-01-1998
// Language:            C++
// Operating System:    WINDOWS
//
// Definition:
//
//     This module contains functions related to common classes
//     for the virtual machine
//
//==============================================================================

#include "vmtypes.hpp"
#include "vmres.hpp"
#include "m4mdrt.hpp"
#include "access.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "recorset.hpp"
#include "cldmcontext.hpp"
#include "m4regimpl.hpp"
#include "m4unicode.hpp"
#include "m4logfile.hpp"
#include "execlogs.hpp"


void dumpOltp( m4pcchar_t ai_pccChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pcchar_t ai_pccOrg, m4pcchar_t ai_pccRole )
{
	M4ThreadLogger*	pThreadLogger = NULL;

	if( ClVMDebugEnv::getInstance()->getDebugLevel() < 1 )
	{
		return ;
	}

	if( ai_pccOrg == NULL )
	{
		ai_pccOrg = "";
	}

	if( ai_pccRole == NULL )
	{
		ai_pccRole = "";
	}

	// m4oltp.log trace.
	pThreadLogger = M4ThreadLogger::GetInstance( OLTP_LOG );

	if( pThreadLogger != NULL )
	{
		pThreadLogger->InitLogFile( "m4oltp", 5 * 1024 * 1024, "TickCount\tThreadId\tMeta4Object\tNode\tItem\tOrganization\tRole\tSessionKey\tServerHost\tServerPort\tSessionId\tUserClick\tReqUniqueID", "log", M4_TRUE, M4_FALSE, M4_TRUE );
		pThreadLogger->WriteLine( "\t%s\t%s\t%s\t%s\t%s", ai_pccChannel, ai_pccNode, ai_pccItem, ai_pccOrg, ai_pccRole );
	}
}



/* jcr efv
Para filtrar en el reproductor de PDUs
Si no se ha construido la lista de métodos la construye
Despues hace una búsqueda lineal
*/
class ex_list_t {

protected:
	m4bool_t mode;
	m4char_t filter[3][100];

public:
	ex_list_t() {
		mode = M4_FALSE;
		memset (filter,0,sizeof(filter));
	}

	void set (m4bool_t ai_exec, m4pcchar_t ai_channel, m4pcchar_t ai_node, m4pcchar_t ai_item) {
		mode = ai_exec;
		strcpy(filter[0],ai_channel);
		strcpy(filter[1],ai_node);
		strcpy(filter[2],ai_item);

		for (m4uint32_t i=0; i<3; i++) {
			if (strcmp(filter[i],"-") == 0) {
				*filter[i] = '\0';
			}
		}
	}

	void check (m4pcchar_t ai_channel, m4pcchar_t ai_node, m4pcchar_t ai_item, m4uint32_t &ao_level, m4bool_t &ao_exec) {

		m4uint32_t level = 0;

		m4pcchar_t input[3] = {ai_channel, ai_node, ai_item};

		for (m4uint32_t i=0; i<3; i++) {
		
			if (filter[i][0] != '\0') {
				
				level=i;

				if (input[i] == NULL || strcmpi(filter[i],input[i]) != 0) {
					return;
				}
			}
		}

		// checks the level
		if (level < ao_level) {
			return;
		}

		ao_level = level;
		ao_exec = mode;
	}
};

const m4uint32_t maxElm = 1000;

class ex_list_check {

protected: 
	static ex_list_t *ex_list;
	static m4uint32_t nElm;

public:
	void addElm(m4bool_t ai_exec, m4pcchar_t ai_channel, m4pcchar_t ai_node, m4pcchar_t ai_item) {
		if (nElm == maxElm) {

			cout << "Too many entries. Ignoring input paramater for obj " << ai_channel << "." <<
				ai_node << "." << ai_item << " mode " << ai_exec << endl;
			return;
		}

		ex_list[nElm].set(ai_exec,ai_channel,ai_node,ai_item);
		nElm++;
	}

	m4bool_t check_exec(m4pcchar_t ai_channel, m4pcchar_t ai_node, m4pcchar_t ai_item) {
		
		m4uint32_t level = 0;
		m4bool_t exec = M4_TRUE;

		for (m4uint32_t i=0; i<nElm; i++) {
			ex_list[i].check(ai_channel,ai_node,ai_item,level,exec);
		}

		return exec;
	}
};

// static members
ex_list_t *ex_list_check::ex_list = new ex_list_t[maxElm];
m4uint32_t ex_list_check::nElm = 0;


m4bool_t	M4PDUHasToExecute( m4pcchar_t ai_pccM4Obj, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{

	m4bool_t				bResult = M4_TRUE ;

	static  m4bool_t		bLoaded = M4_FALSE;
	static  ex_list_check	ex_list;
	static	ClMutex			s_oMutex( M4_TRUE ) ;


	if( ai_pccM4Obj == NULL || ai_pccNode == NULL || ai_pccItem == NULL )
	{
		return( M4_TRUE ) ;
	}


// Primero se carga el fichero

	s_oMutex.Lock() ;

	if( bLoaded == M4_FALSE) 
	{
		bLoaded = M4_TRUE;

		string	s = ClVMDebugEnv::getInstance()->getDebugDir();
		s += M4_BACK_SLASH "pdu_filter.lst" ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		FILE	*fInput = M4Fopen( s.c_str(), M4UniRead, eEncoding ) ;

		if( fInput != NULL )
		{
			int		iNumber = 0 ;
			size_t	iLength = 0 ;
			char	*pcLine ;
			char	acMode [ 100 ] ;
			char	acM4Obj[ 100 ] ;
			char	acNode [ 100 ] ;
			char	acItem [ 100 ] ;
			char	acLine [ 1000 ] ;

			for (;;) {
				pcLine = fgets( acLine, 1000, fInput ) ;

				if (pcLine == NULL) {
					break;
				}

				strcpy(acMode,"-");
				strcpy(acM4Obj,"-");
				strcpy(acNode,"-");
				strcpy(acItem,"-");

				iNumber = sscanf( acLine, "%s %s %s %s", acMode, acM4Obj, acNode, acItem ) ;

				if (iNumber == EOF || iNumber < 1) {
					continue;
				}

				m4bool_t bMode = *acMode == '+' ? M4_TRUE : M4_FALSE;
				
				ex_list.addElm(bMode,acM4Obj,acNode,acItem);

			}

			fclose( fInput ) ;
		}
	}
	
	s_oMutex.Unlock() ;


// Después se busca
	bResult = ex_list.check_exec (ai_pccM4Obj, ai_pccNode, ai_pccItem);

	return( bResult ) ;
}

m4return_t m4VMState_t::Serialize(ClGenericIODriver& IOD, m4bool_t ai_bLocals, m4int32_t ai_iNotSentVariables)
{
    m4return_t RetVal;
    m4bool_t exist;
	m4pcchar_t pszItem;
	m4pcchar_t pszNode;
    // Serializamos el access lo primero poruqe en la deserializacion se necesita >>>>>>>>>>>>>>>>>
    if ((RetVal = IOD.Write(m_poAccess, &exist)) != M4_SUCCESS)
        return RetVal;
    
    if (!exist)
        return M4_ERROR;
    
    if ((RetVal = IOD.Write(m_poRecordSet, &exist)) != M4_SUCCESS) 
        return RetVal;
    
    if (!exist)
    {
        ClNode_Data* pNodeData = m_poRecordSet->GetpNode_Data();
        ClCompiledMCR* pmcr    = pNodeData->GetpChannel_Data()->GetpChannelDef()->GetpCMCR();
        m4uint32_t NodeHandle  = pNodeData->GetpNodeDef()->GetHandle();
        
        DUMP_CHLOG_ERRORF(M4_VM_ERROR_SERIALIZE_RECORDSET_IN_M4VMSTATE, M4ObjNodeN(pmcr, NodeHandle));
        
        return M4_ERROR;
    }

    RetVal = IOD.Write(m_oRegisterIndex);
    if (RetVal != M4_SUCCESS) return RetVal;

    // El metacanal compilado lo coneguimos del access que tiene el m4VMState_t
    ClCompiledMCR* poCMCR  = m_poAccess->GetpChannel()->GetpCMCR();
    
	if (m_hItem!=-1){
		pszItem = poCMCR->GetItemId( m_hItem );
		pszNode = poCMCR->GetNodeId( poCMCR->GetItemNodeHandle(m_hItem) );
    }
	else{
		pszItem ="";
		pszNode ="";
	}

    if ((RetVal = IOD.Write(pszNode)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Write(pszItem)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Write(m_iRule)) != M4_SUCCESS)
        return RetVal;
    
    
	if (ai_bLocals){

		//el flag de no ejecutable nos permite :
		//Cuando vamos del server al cliente en una pausa marcar los estados como que no se pueden ejecutar
		//y si leugo los llevamos otra vez al server desmarcarlos .
		if (M4CL_HAS_STATE_NOT_EXE_FLAG(m_Flags)){
			M4CL_RESET_STATE_NOT_EXE_FLAG(m_Flags);
		}
		else{
			M4CL_SET_STATE_NOT_EXE_FLAG(m_Flags);
		}
		if ((RetVal = IOD.Write(m_Flags)) != M4_SUCCESS)
			return RetVal;
		if ((RetVal = IOD.Write(m_iSourceLine)) != M4_SUCCESS)	
			return RetVal;
		if ((RetVal = IOD.Write(m_iCurrent)) != M4_SUCCESS)
		    return RetVal;
		if ((RetVal = IOD.Write(m4int32_t(m_iStepperType))) != M4_SUCCESS)	
			return RetVal;
	}
	else{ //si no llevamos las locales-> empezamos en la primera linea, intrucción 0, los estados son ejecutables
		M4CL_RESET_STATE_INIT_FLAG(m_Flags);
		M4CL_RESET_STATE_NOT_EXE_FLAG(m_Flags);
		if ((RetVal = IOD.Write(m_Flags)) != M4_SUCCESS)
			return RetVal;

		if ((RetVal = IOD.Write(m4int32_t(1))) != M4_SUCCESS)	
			return RetVal;
		if ((RetVal = IOD.Write(m4int32_t(0))) != M4_SUCCESS)	
			return RetVal;
		if ((RetVal = IOD.Write(m4int32_t(StepperTypeNormal))) != M4_SUCCESS)	
			return RetVal;
	}
	
	if ((RetVal = IOD.Write(m_iParentStatePos) ) != M4_SUCCESS)	
			return RetVal;
    if ((RetVal = IOD.Write(m_dDate)) != M4_SUCCESS)
        return RetVal;
    
	/* Bug 0117879
	Si sacamos el estado y han quedado variables en la pila hay que disminuir la referencia
	en el número de variables que se han quedado.
	Así la referencia viaja coherente respecto a las variables que viajan
	*/
    if ((RetVal = IOD.Write(m_iOldStackReference - ai_iNotSentVariables)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Write(m_dBeginDate)) != M4_SUCCESS)
        return RetVal;

    if ((RetVal = IOD.Write(m_dEndDate)) != M4_SUCCESS)
        return RetVal;


	/* Bug 0065665
	Hay que mandar la fecha de la fórmula para recuperar correctamente el código
	*/
	/* Bug 0250784
	Se manda también la fecha fin
	*/
	m4date_t	dStartDate = 0 ;
	m4date_t	dEndDate = 0 ;

	if( m_poFormula.IsNull() == M4_FALSE )
	{
		dStartDate = m_poFormula.GetStartDate() ;
		dEndDate = m_poFormula.GetEndDate() ;
	}

    if ((RetVal = IOD.Write(dStartDate)) != M4_SUCCESS)
        return RetVal;

    if ((RetVal = IOD.Write(dEndDate)) != M4_SUCCESS)
        return RetVal;


	// jcr Arreglo para que se pueda depurar JIT
	// Mandamos el puntero a la fórmula JIT
	// En principio, aunque parece peligroso no es problemático,
	// ya que en cliente no se puede ejecutar y en server no
	// debería realocarse el puntero
	if (M4CL_HAS_STATE_EXE_JIT_FLAG(m_Flags)){
	    m4pcchar_t pCode=m_poFormula.GetFormulaHandle();
		if ((RetVal = IOD.Write((void*)&pCode, sizeof(void*))) != M4_SUCCESS)
			return RetVal;

		/* Bug 0071482
		Se manda el handle remoto, que puede ser nulo
		*/
	    pCode=m_poFormula.GetRemoteFormulaHandle();
		if ((RetVal = IOD.Write((void*)&pCode, sizeof(void*))) != M4_SUCCESS)
			return RetVal;
	}
    
    return M4_SUCCESS;
}

m4return_t m4VMState_t::DeSerialize(ClGenericIODriver& IOD, m4int32_t ai_iVariablesInStack)
{
    m4return_t RetVal;
    m4bool_t exist;
	m4int32_t iType;
    
    // Serializamos el access lo primero poruqe en la deserializacion se necesita >>>>>>>>>>>>>>>>>>>
    if ((RetVal = IOD.Read((void**)&m_poAccess, &exist)) != M4_SUCCESS)
        return RetVal;
    
    if (!exist)
        return M4_ERROR;
    
    if ((RetVal = IOD.Read((void**)&m_poRecordSet, &exist)) != M4_SUCCESS)
        return RetVal;
    
    if (!exist)
        return M4_ERROR;
    
    RetVal = IOD.Read(m_oRegisterIndex);
    if (RetVal != M4_SUCCESS) return RetVal;
    // Serializamos el access lo primero poruqe en la deserializacion se necesita <<<<<<<<<<<<<<<<<<<
    
    m4char_t pszItem[100];
    m4char_t pszNode[100];
    m4uint32_t hNodo;
    
    if ((RetVal = IOD.Read(pszNode)) != M4_SUCCESS)
        return RetVal;
    if ((RetVal = IOD.Read(pszItem)) != M4_SUCCESS)
        return RetVal;
    
#ifdef _DEBUG
    cout << "Executing method " << pszItem << " in node " << pszNode << endl;
    cout.flush();
#endif
    
    // El metacanal compilado lo coneguimos del access que tiene el m4VMState_t
    ClCompiledMCR* poCMCR = m_poAccess->GetpChannel()->GetpCMCR();
    
	if (pszNode[0]==0){
		m_hItem=-1;
	}
	else{
		hNodo = poCMCR->GetChannelNodeHandleById(pszNode);
		M4_MSG_ASSERT(hNodo != 0,"Trying to execute a mapped method from a non mapped node");
		if (hNodo == 0) return M4_ERROR;
		
		m_hItem = poCMCR->GetNodeItemHandleById (hNodo, pszItem);
		if (m_hItem == 0) return M4_ERROR;
    }

	// add oltp traces
	m4bool_t	bHasToExecute = M4PDUHasToExecute( poCMCR->GetChannelId(), pszNode, pszItem ) ;

	if( bHasToExecute == M4_FALSE )
	{
		cerr << "Not executing method " << pszItem << " in node " << pszNode << " in m4obj " << poCMCR->GetChannelId() << endl ;
		return( M4_ERROR ) ;
	}

	m4pchar_t	pcOrg;
	m_poAccess->GetpChannel()->Organization.Get( pcOrg );

	m4pchar_t	pcRole;
	m_poAccess->GetpChannel()->Role_ID.Get( pcRole );

	dumpOltp(poCMCR->GetChannelId(), pszNode, pszItem, pcOrg, pcRole);
	// end oltp traces

    if ((RetVal = IOD.Read(m_iRule)) != M4_SUCCESS)
        return RetVal;

    if ((RetVal = IOD.Read(m_Flags)) != M4_SUCCESS)
        return RetVal;

    if ((RetVal = IOD.Read(m_iSourceLine)) != M4_SUCCESS)
    	return RetVal;

	if ((RetVal = IOD.Read(m_iCurrent)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Read(iType)) != M4_SUCCESS)
    	return RetVal;

   	m_iStepperType=stStateStepperType(iType) ;

	if ((RetVal = IOD.Read(m_iParentStatePos)) != M4_SUCCESS)
        return RetVal;
	
    if ((RetVal = IOD.Read(m_dDate)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Read(m_iOldStackReference)) != M4_SUCCESS)
        return RetVal;

	/* Bug 0117879
	Si metemos el estado donde ya hay variables hay que aumentar la referencia
	en el número de variables que ya hay
	*/
	m_iOldStackReference += ai_iVariablesInStack;
    
    if ((RetVal = IOD.Read(m_dBeginDate)) != M4_SUCCESS)
        return RetVal;
    
    if ((RetVal = IOD.Read(m_dEndDate)) != M4_SUCCESS)
        return RetVal;
    

	/* Bug 0065665
	Hay que leer las fechas de la fórmula para recuperar correctamente el código
	*/
	m4date_t	dStartDate = 0 ;
	m4date_t	dEndDate = 0 ;

    if ((RetVal = IOD.Read(dStartDate)) != M4_SUCCESS)
        return RetVal;

    if ((RetVal = IOD.Read(dEndDate)) != M4_SUCCESS)
        return RetVal;

    
    m4pcchar_t pCode=0;
    
	// jcr Arreglo para que se pueda depurar JIT
	// Recuperamos el puntero a la fórmula JIT
	if (M4CL_HAS_STATE_EXE_JIT_FLAG(m_Flags)){
		m4uint32_t iSize=0;
		if ((RetVal = IOD.Read((void*)&pCode, iSize)) != M4_SUCCESS)
			return RetVal;

		/* Bug 0071482
		Se asigna como remoto el original y como original el remoto
		*/
		m_poFormula.SetRemoteFormulaHandle (pCode);

		if ((RetVal = IOD.Read((void*)&pCode, iSize)) != M4_SUCCESS)
			return RetVal;
	}
	else
	{
		if (pszNode[0]!=0){
			
			/* Bug 0065665
			Se utiliza siempre la fecha que llega
			*/
			/* Bug 0250784
			Se busca por fecha inicio y fecha fin de definición
			*/
			pCode = poCMCR->GetItemCodeByRuleDates( m_hItem, m_iRule, dStartDate, dEndDate );
		}
	}

	m_poFormula.Init (pCode);
    
    return M4_SUCCESS;
}

ostream & operator <<(ostream &out, m4VMState_t ai_data)
{
	out<<"m4VMState_OutputNotImplemented";
	return out;
}
 
