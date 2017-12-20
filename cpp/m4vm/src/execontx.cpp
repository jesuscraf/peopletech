
#include "execontx.hpp"
#include "fixstack.hpp"
#include "m4mdrt.hpp"
#include "m4vmstate_t.hpp"
#include "vmachin1.h"


m4return_t	GetExecutionLine( ClVMRunContext* ai_pvContext, m4int32_t &ao_riLine, m4pcchar_t &ao_rpccItemId, m4pcchar_t &ao_rpccItemName, m4pcchar_t &ao_rpccNodeId, m4pcchar_t &ao_rpccNodeName, m4pcchar_t &ao_rpccM4ObjId, m4pcchar_t &ao_rpccM4ObjName )
{

	m4return_t					iResult = M4_ERROR ;
	m4uint32_t					hItem = 0 ;
	ClVMachine1					*poMachine = NULL ;
	ClAccess					*poAccess = NULL ;
	ClCompiledMCR				*poCMCR = NULL ;
	tpFixedStack<m4VMState_t>	*poStateStack = NULL ;
	m4VMState_t					oState ;


	ao_riLine = -1 ;
	ao_rpccItemId = NULL ;
	ao_rpccItemName = NULL ;
	ao_rpccNodeId = NULL ;
	ao_rpccNodeName = NULL ;
	ao_rpccM4ObjId = NULL ;
	ao_rpccM4ObjName = NULL ;

	if( ai_pvContext == NULL )
	{
		return( M4_ERROR ) ;
	}

	poMachine = ai_pvContext->m_pVM ;

	if( poMachine != NULL )
	{
		poStateStack = poMachine->GetStateStack() ;

		if( poStateStack != NULL )
		{
			// Datos del estado anterior
			iResult = poStateStack->Top( oState, 1 ) ;

			if( iResult != M4_SUCCESS )
			{
				// Datos del estado actual
				iResult = poStateStack->Top( oState, 0 ) ;
			}

			if( iResult == M4_SUCCESS )
			{
				poAccess = oState.m_poAccess ;

				if( poAccess != NULL )
				{
					poCMCR = poAccess->GetpCMCR() ;

					if( poCMCR != NULL )
					{
						hItem = oState.m_hItem ;
						ao_riLine = oState.m_iSourceLine - 1 ;
					}
				}
			}
		}
	}

	if( poCMCR == NULL )
	{
		// Datos del contexto
		poAccess = ai_pvContext->m_pAccess ;

		if( poAccess != NULL )
		{
			poCMCR = poAccess->GetpCMCR() ;

			if( poCMCR != NULL )
			{
				hItem = ai_pvContext->m_hItem ;
				ao_riLine = 1 ;
			}
		}
	}

	if( poCMCR != NULL )
	{
		ao_rpccItemId = poCMCR->iItemI( hItem ) ;
		ao_rpccItemName = poCMCR->nItemI( hItem ) ;
		ao_rpccNodeId = poCMCR->iNodeI( hItem ) ;
		ao_rpccNodeName = poCMCR->nNodeI( hItem ) ;
		ao_rpccM4ObjId = poCMCR->iM4Obj() ;
		ao_rpccM4ObjName = poCMCR->nM4Obj() ;
	}

	return( M4_SUCCESS ) ;
}


ChLog&	DumpVMContextf( ChLog& aio_chlog, ClVMRunContext* ai_pvContext )
{

	m4return_t	iResult = M4_ERROR ;
	m4int32_t	iLine = -1 ;
	m4pcchar_t	pccItemId = NULL ;
	m4pcchar_t	pccItemName = NULL ;
	m4pcchar_t	pccNodeId = NULL ;
	m4pcchar_t	pccNodeName = NULL ;
	m4pcchar_t	pccM4ObjId = NULL ;
	m4pcchar_t	pccM4ObjName = NULL ;
	ClVMachine1	*poMachine = NULL ;


	if( aio_chlog.IsDumpActive() == M4_FALSE )
	{
		return( aio_chlog ) ;
	}

	iResult = GetExecutionLine( ai_pvContext, iLine, pccItemId, pccItemName, pccNodeId, pccNodeName, pccM4ObjId, pccM4ObjName ) ;

	if( iResult == M4_SUCCESS )
	{
		aio_chlog << iLine ;
		aio_chlog << LOG_CAT_AND( pccItemId, pccItemName ) ;
		aio_chlog << LOG_CAT_AND( pccNodeId, pccNodeName ) ;
		aio_chlog << LOG_CAT_AND( pccM4ObjId, pccM4ObjName ) ;
	}

	poMachine = ai_pvContext->m_pVM ;

	if( poMachine != NULL )
	{
		aio_chlog << poMachine->DumpCallStack() ;
	}

	return( aio_chlog ) ;
}


ChLogManip_vmcontext_dump_t	DumpVMContext( ClVMRunContext* ai_pvContext )
{
	return( ChLogManip_vmcontext_dump_t( DumpVMContextf, ai_pvContext )  );
}
