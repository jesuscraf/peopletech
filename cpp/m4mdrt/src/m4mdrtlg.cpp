
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrtlg.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este modulo implementa las funciones de log del metacanal compilado
//    
//
//
//==============================================================================


#include "m4mdrt.hpp"

#include "m4lang.hpp"
#include "m4objglb.hpp"


// Funciones-Manipulador con 2 arg (indirectas) - Dump CMCR Info

ChLog&	M4Obj_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNotUsed)
{
	// Output = M4ObjectId&&M4ObjectName
	aio_chlog	<< poCMCR->iM4Obj()			<< LogCatAnd << poCMCR->nM4Obj() ;
	return aio_chlog;
}

ChLog&	M4ObjExtended_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNotUsed)
{
	// Output = M4ObjectId&&M4ObjectName
	aio_chlog	<< poCMCR->iM4Obj()			<< LogCatAnd << poCMCR->nM4Obj() 
				<< M4ClCsType(poCMCR->GetCacheCsType()) << M4ClLanguage(poCMCR->GetChannelLanguage()) 
				<< poCMCR->GetSecurityMaskId() << DateLog(poCMCR->GetChannelVersion());
	return aio_chlog;
}

ChLog&	M4ObjNodeN_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNode)
{
	// Output = NodeId&&NodeName << M4ObjectId&&M4ObjectName	using Node Handle
	aio_chlog	<< poCMCR->iNodeN(hNode)	<< LogCatAnd << poCMCR->nNodeN(hNode)
				<< poCMCR->iM4Obj()			<< LogCatAnd << poCMCR->nM4Obj() ;
	return aio_chlog;
}

ChLog&	M4ObjNodeI_f		(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem)
{
	// Output = NodeId&&NodeName << M4ObjectId&&M4ObjectName	using Item Handle
	aio_chlog	<< poCMCR->iNodeI(hItem)	<< LogCatAnd << poCMCR->nNodeI(hItem)
				<< poCMCR->iM4Obj()			<< LogCatAnd << poCMCR->nM4Obj();
	return aio_chlog;
}

ChLog&	M4ObjNodeItemI_f	(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem)
{
	// Output = ItemId&&ItemName << NodeId&&NodeName << M4ObjectId&&M4ObjectName
	aio_chlog	<< poCMCR->iItemI(hItem)	<< LogCatAnd << poCMCR->nItemI(hItem)
				<< poCMCR->iNodeI(hItem)	<< LogCatAnd << poCMCR->nNodeI(hItem)
				<< poCMCR->iM4Obj()			<< LogCatAnd << poCMCR->nM4Obj() ;
	return aio_chlog;
}

ChLog&	NodeN_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hNode )
{
	// Output = NodeId&&NodeName	using Node Handle
	aio_chlog	<< poCMCR->iNodeN(hNode)	<< LogCatAnd << poCMCR->nNodeN(hNode);
	return aio_chlog;
}

ChLog&	NodeI_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem )
{
	// Output = NodeId&&NodeName	using Node Handle
	aio_chlog	<< poCMCR->iNodeI(hItem)	<< LogCatAnd << poCMCR->nNodeI(hItem);
	return aio_chlog;
}

ChLog&	NodeItemI_f			(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem )
{
	// Output = ItemId&&ItemName << NodeId&&NodeName
	aio_chlog	<< poCMCR->iItemI(hItem)	<< LogCatAnd << poCMCR->nItemI(hItem)
				<< poCMCR->iNodeI(hItem)	<< LogCatAnd << poCMCR->nNodeI(hItem) ;
	return aio_chlog;
}

ChLog&	ItemI_f				(ChLog& aio_chlog, ClCompiledMCR *poCMCR, m4uint32_t hItem )
{
	// Output = ItemId&&ItemName
	aio_chlog	<< poCMCR->iItemI(hItem)	<< LogCatAnd << poCMCR->nItemI(hItem);
	return aio_chlog;
}



// Funciones-Manipulador con 2 arg (directas) que llaman a la funcion manipuladora real con 2 args
ChLogManip2_mcr_t	M4Obj			( ClCompiledMCR *poCMCR )					{ return  ChLogManip2_mcr_t( M4Obj_f,			poCMCR, 0 ); }
ChLogManip2_mcr_t	M4ObjExtended	( ClCompiledMCR *poCMCR )					{ return  ChLogManip2_mcr_t( M4ObjExtended_f,	poCMCR, 0 ); }
ChLogManip2_mcr_t	M4ObjNodeN		( ClCompiledMCR *poCMCR, m4uint32_t hNode ) { return  ChLogManip2_mcr_t( M4ObjNodeN_f,		poCMCR, hNode ); }
ChLogManip2_mcr_t	M4ObjNodeI		( ClCompiledMCR *poCMCR, m4uint32_t hItem ) { return  ChLogManip2_mcr_t( M4ObjNodeI_f,		poCMCR, hItem ); }
ChLogManip2_mcr_t	M4ObjNodeItemI	( ClCompiledMCR *poCMCR, m4uint32_t hItem ) { return  ChLogManip2_mcr_t( M4ObjNodeItemI_f,	poCMCR, hItem ); }
ChLogManip2_mcr_t	NodeN			( ClCompiledMCR *poCMCR, m4uint32_t hNode ) { return  ChLogManip2_mcr_t( NodeN_f,			poCMCR, hNode ); }
ChLogManip2_mcr_t	NodeI			( ClCompiledMCR *poCMCR, m4uint32_t hItem ) { return  ChLogManip2_mcr_t( NodeI_f,			poCMCR, hItem ); }
ChLogManip2_mcr_t	NodeItemI		( ClCompiledMCR *poCMCR, m4uint32_t hItem ) { return  ChLogManip2_mcr_t( NodeItemI_f,		poCMCR, hItem ); }
ChLogManip2_mcr_t	ItemI			( ClCompiledMCR *poCMCR, m4uint32_t hItem ) { return  ChLogManip2_mcr_t( ItemI_f,			poCMCR, hItem ); }


