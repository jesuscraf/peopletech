#include "meminfo.hpp"
#include "access.hpp"
#include "computil.hpp"
#include "node_knl.hpp"
#include "compst_common.hpp"


//Funcion de uso interno en el modulo
m4return_t getmeminfo_block (ClBlockReference  &ai_br, void *);


ClChannelMemInfo::ClChannelMemInfo()
{
	Reset();
}

void
ClChannelMemInfo::Reset()
{
	m_nNodes=0;
	m_nBlocks=0;
	m_nProxyBlocks=0;
	m_nRegisters=0;
	m_nDataSize=0;
	m_nNulls=0;
	m_nTotalSize=0;
}

m4return_t 
ClChannelMemInfo::GetMemInfo(ClAccess *ai_pacc,stChannelMemInfo_t &ai_stMI)
{
	m4uint32_t i=0;
	ClNode *pnode;
	
// Primero los datos.

	if (ai_pacc==0)	{
		return M4_ERROR;
	}
	
	Reset();
	m_nNodes = ai_pacc->Node.Count();

	// Iteramos por todos los nodos.
	for (i=0;i<m_nNodes;++i)
	{
		pnode = ai_pacc->Node.Get((ClNodeIndex)i);
		if (_GetNodeMemInfo(pnode)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	ClSize oSize;
	if (ai_pacc->GetpChannel()->GetSize(oSize)!=M4_SUCCESS){
		return M4_ERROR;
	}
	m_nTotalSize = oSize;

	_UpdateMemInfo(ai_stMI);

	return M4_SUCCESS;
}

m4return_t 
ClChannelMemInfo::GetMemInfo(ClNode *ai_pn,stNodeMemInfo_t &ai_stMI)
{
	Reset();
	
	if (_GetNodeMemInfo(ai_pn)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ClSize oSize;
	if (ai_pn->GetTotalSize(oSize)!=M4_SUCCESS){
		return M4_ERROR;
	}
	m_nTotalSize = oSize;
	
	_UpdateMemInfo(ai_stMI);
	return M4_SUCCESS;
}

m4return_t 
ClChannelMemInfo::GetMemInfo(ClRecordSet *ai_RS,stRSMemInfo_t &ai_stMI)
{
	m4return_t ret = M4_ERROR;
	Reset();
	
	ret = _GetRSMemInfo(ai_RS);
	if (ret ==M4_SUCCESS){
		_UpdateMemInfo(ai_stMI);
	}
	return ret;
}

m4return_t 
ClChannelMemInfo::GetMemInfo(_ClRegister_Base *ai_pRg,stRgMemInfo_t &ai_stMI)
{
	m4return_t ret = M4_ERROR;
	Reset();

	ret =_GetRegisterMemInfo(ai_pRg);

	if (ret ==M4_SUCCESS){
		_UpdateMemInfo(ai_stMI);
	}
	return ret;

}

m4return_t 
ClChannelMemInfo::GetMemInfo(ClBlockReference &ai_br)
{
	ClRecordSet RS(0);
	RS.SetCurrentBlock(ai_br);

	return 	_GetRSMemInfo(&RS);
}


m4return_t getmeminfo_block (ClBlockReference  &ai_br, void *ai_MI)
{
	return ((ClChannelMemInfo *)ai_MI)->GetMemInfo(ai_br);
}


m4return_t 
ClChannelMemInfo::_GetNodeMemInfo(ClNode *ai_pn)
{

	if (ai_pn==0){
		return M4_ERROR;
	}
	// Items exclusivos del nodo.
	ClItem ItIt(ai_pn->Item);
	
	ItIt.SetMode(M4DM_SCOPE_NODE,M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
	
	if (_GetIIDataSize(ItIt)!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Iteramos por todos los bloques.

	// El numero de bloques lo calculo uno a uno para poder distinguir entre Proxys y Normales.
//	m_nBlocks+=ai_pn->GetpNode_Data()->GetpBlockStore()->Count();

	if (ClCStoreUtilities::ForEach(ai_pn->GetpNode_Data()->GetpBlockStore(),getmeminfo_block,this)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t 
ClChannelMemInfo::_GetRSMemInfo(ClRecordSet *ai_RS)
{

	m4uint32_t i=0,nRg=0;

	if (ai_RS->GetCurrentBlock()->IsValid()==M4_FALSE){
		m_nProxyBlocks++;
		return M4_SUCCESS;
	}
	else{
		// Si no es Proxy, tiene que ser ChannelBlock, porque lo hemos sacado del ComponentStore.
		m_nBlocks++;
	}
	
	
	// Items de bloque.
	if (ai_RS==0){
		return M4_ERROR;
	}

	ClRegister *pRg=&(ai_RS->Register);

	ClItem ItIt(pRg->Item);
	
	ItIt.SetMode(M4DM_SCOPE_BLOCK,M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);
	if (_GetIIDataSize(ItIt)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Iteramos por todos los registros.
	// En carga parcial el count me devuelve -1 si no hay registros.
	// Se lo pido al RecordSet ( me da el numero de registros que haya cargados en ese momento )
	// y hago el MoveToMM en lugar de MoveTo, porque me mueve al indice real : 
	// En carga parcial, el registro de indice 800, puede tener un mmindex distinto (siempre menor que la ventana).

	nRg=ai_RS->Count();
	m_nRegisters += nRg;

	for (i=0;i<nRg;++i)
	{
		pRg->MoveToMM(i);
		if (_GetRegisterMemInfo(pRg)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	pRg->MoveToEOF();
	return M4_SUCCESS;
}

m4return_t
ClChannelMemInfo::_GetRegisterMemInfo(_ClRegister_Base *pRg)
{
	
	// Simplemente items de registro.

	ClItem ItIt(pRg->Item);
	
	ItIt.SetMode(M4DM_SCOPE_REGISTER,M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);

	if (_GetIIDataSize(ItIt)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClChannelMemInfo::_GetIIDataSize(ClItem &ItIt)
{
	
	for (ItIt.Begin(); ItIt.GetIndex() != M4DM_EOF_INDEX; ItIt.Next())
	{
		// Me faltan los old values.
		if (_GetValueItDataSize(ItIt)!=M4_SUCCESS)
			return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t ClChannelMemInfo::_GetValueItDataSize(ClItem &Item)
{
	m4bool_t bIsNull;
	ClValue &Value = Item.Value;
	m4uint32_t NullSize = 0;
	m4uint8_t cpptype;
	
	cpptype = Item.ItemDef.CppType() ;

	if ( cpptype == M4CL_CPP_TYPE_NUMBER || cpptype == M4CL_CPP_TYPE_DATE ){
		NullSize=8;
	}

	for (Value.Begin(); Value.Index != M4DM_EOF_INDEX; Value.Next())
	{
		Value.IsNull(bIsNull);
		if (bIsNull)
		{
			++m_nNulls;
			m_nDataSize +=NullSize;
		}
		else
		{
			m_nDataSize += Value.Size();
		}
	}

	return M4_SUCCESS;
}


void 
ClChannelMemInfo::_UpdateMemInfo(stRgMemInfo_t &ai_MI)
{
	ai_MI.m_nDataSize = m_nDataSize;
	ai_MI.m_nTotalSize = m_nTotalSize;
	ai_MI.m_nNulls=m_nNulls;
}

void 
ClChannelMemInfo::_UpdateMemInfo(stRSMemInfo_t &ai_MI)
{
	ai_MI.m_nDataSize = m_nDataSize;
	ai_MI.m_nTotalSize = m_nTotalSize;
	ai_MI.m_nNulls=m_nNulls;
	ai_MI.m_nRegisters= m_nRegisters;
}

void 
ClChannelMemInfo::_UpdateMemInfo(stNodeMemInfo_t &ai_MI)
{
	ai_MI.m_nDataSize = m_nDataSize;
	ai_MI.m_nTotalSize = m_nTotalSize;
	ai_MI.m_nNulls=m_nNulls;
	ai_MI.m_nRegisters= m_nRegisters;
	ai_MI.m_nBlocks= m_nBlocks;
	ai_MI.m_nProxyBlocks= m_nProxyBlocks;
}

void 
ClChannelMemInfo::_UpdateMemInfo(stChannelMemInfo_t &ai_MI)
{
	ai_MI.m_nDataSize = m_nDataSize;
	ai_MI.m_nTotalSize = m_nTotalSize;
	ai_MI.m_nNulls=m_nNulls;
	ai_MI.m_nRegisters= m_nRegisters;
	ai_MI.m_nBlocks= m_nBlocks;
	ai_MI.m_nProxyBlocks= m_nProxyBlocks;
	ai_MI.m_nNodes= m_nNodes;
}






#if defined _WINDOWS && defined _DEBUG
m4int16_t 	CRT::SetCrtMemCheckPoint(void)
{
	if (m_nStates<MAX_CHECK_POINTS)
	{
		_CrtMemCheckpoint(&vMemState[m_nStates++]);
	}
	else {
		return -1;
	}
	return m_nStates-1;
}
m4return_t	CRT::SetCrtDumpFile(m4pchar_t ai_path)
{
	if (hF)
		CloseHandle(hF);
	SECURITY_ATTRIBUTES SecurityAttributes;	
    SecurityAttributes.nLength = sizeof( SecurityAttributes );
    SecurityAttributes.lpSecurityDescriptor = NULL;
    SecurityAttributes.bInheritHandle = FALSE;


	hF = CreateFile(ai_path,GENERIC_WRITE,FILE_SHARE_READ,&SecurityAttributes,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hF==INVALID_HANDLE_VALUE)
	{
		hF=0;
		return M4_ERROR;
	}
   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_WARN,hF );
   _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ERROR, hF);
   _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_ASSERT, hF);

	return M4_SUCCESS;
}


m4uint32_t  CRT::DumpCrtMem(m4int16_t check)
{
	if (check<0 && check>=m_nStates)
		return 0;


	_CrtMemDumpAllObjectsSince( &vMemState[check]);

	return GetAllocMem(check);
}
m4uint32_t  CRT::GetAllocMem(m4int16_t check1,m4int16_t check2)
{
	if (check1>=m_nStates)
		return 0;
	if (check2>=m_nStates)
		return 0;
	m4uint32_t TotMem=0;

	_CrtMemState MemStateDiff;
	if (check2 == -1)
	{
		// devolvemos la diferencia con el momento actual.
		_CrtMemState MemStateN;
		_CrtMemCheckpoint(&MemStateN);
		_CrtMemDifference(&MemStateDiff,&vMemState[check1],&MemStateN);
	}
	else 
	{
		_CrtMemDifference(&MemStateDiff,&vMemState[check1],&vMemState[check2]);
	}


	for (m4uint32_t i=0;i<4;++i) // Solo cambian los bloques de tipo _NORMAL_BLOCK (i=1), pero por si acaso.
		TotMem+=MemStateDiff.lSizes[i];

	m4int16_t iMemStates=GetNumStates();

	// Descuento todos los MemStates que haya reservado sin incluir el que me han  pedido.
	return TotMem;
}
m4int16_t		CRT::GetNumStates(void)
{
	return m_nStates;
}

#endif
