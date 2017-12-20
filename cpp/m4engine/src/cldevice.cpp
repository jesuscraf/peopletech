//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldevice.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/21/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernandez && 08/05/98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4string.hpp"

#define INCLUDING_FROM_CLDEVICE

#include "eng.h"
#include "cldevice.h"
#include "engdf.inl"

#include "cldevice.hmg"

#include "clreparg.h"
#include "region.h"

//========================================================================ClENG_Page


void ClENG_Page::Init( DvcUnit_t ai_SizeX, DvcUnit_t  ai_SizeY, 
		DvcUnit_t ai_OffsetX, DvcUnit_t  ai_OffsetY, size_t ai_szBlockBuffer) 
{
	ClENG_Page::End();
	m_OffsetX=ai_OffsetX ; m_OffsetY=ai_OffsetY; m_SizeX=ai_SizeX; m_SizeY=ai_SizeY ;
	m_BaseLineX=m_BaseLineY=0;
	m_oOpPrBuff.Init(ai_szBlockBuffer);
	m_oPrBuff.Init(ai_szBlockBuffer);
	m_oLayoutBuff.Init(ai_szBlockBuffer);
	m_poActualBuff=&m_oOpPrBuff ;
}

ClENG_Page::ClENG_Page() 
{
}

void ClENG_Page::End() 
{
	m_oOpPrBuff.End();
	m_oPrBuff.End();
	m_oLayoutBuff.End();
	m_poLastFont=NULL;
	m_uiLastIdColour=ClENG_Colour::ID_NULL ;
	m_uiPreserve=0;
	m_poActualBuff=NULL ; 
}


//========================================================================ClENG_Page

ClENG_IndexNode::ClENG_IndexNode()
{
	m_poInsp=GET_INSP();
	m_poFather=NULL;
}

void ClENG_IndexNode::Init(m4int32_t ai_iIndexLevel, m4uint32_t ai_uiTargetID, 
						   const m4char_t* ai_pcIndexString, m4int16_t ai_iLayoutPages, 
						   m4uint32_t ai_uiPrintedPages, m4int16_t ai_iNumPage, 
						   ClENG_IndexNode* ai_poFather)
{
	ClENG_IndexNode::End();

	m_iIndexLevel = ai_iIndexLevel;  
	m_uiTargetID = ai_uiTargetID;	
	m_oIndexString.StrCpy(ai_pcIndexString);	
	
	m_iLayoutPages = ai_iLayoutPages;
	m_uiPrintedPages = ai_uiPrintedPages;
	m_iNumPage = ai_iNumPage;

	m_poFather = ai_poFather;

	m_oAuxValue.StrCpy("") ;

}

void ClENG_IndexNode::End()
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	m_poFather = NULL;

	//Destruction of all the nodes
	IndexNodeList_t::iterator itListNode, ItNext ;

	for (itListNode = m_oIndexNodeList.begin (); itListNode!=m_oIndexNodeList.end(); itListNode=ItNext)
	{
		ItNext = itListNode ; ++ItNext ;
		if (((*itListNode).second) != NULL)	{
			(*itListNode).second->End();
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
				(*itListNode).second, ClENG_IndexNode 
			) ;
		} ;
	} ;

	m_oIndexNodeList.clear();

	m_oAuxValue.End() ;
};

void ClENG_IndexNode::InsertSonNode(ClENG_IndexNode* ai_poSonNode)
{
	m_oIndexNodeList.insert(IndexNodeList_t::value_type(
		ai_poSonNode->m_uiTargetID,ai_poSonNode) );
};

ClENG_IndexNode* ClENG_IndexNode::GetNode(m4uint32_t ai_iTargetID, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClENG_IndexNode* R ; 

	R = GetSonNodeAux(ai_iTargetID) ;

	m_poInsp->m_poTrace->TestEffect( M4_BOOL(R!=NULL), ai_ErrorEffect ) ; 
	return R ;		
}

ClENG_IndexNode* ClENG_IndexNode::GetSonNodeAux(m4uint32_t ai_iTargetID)
{
	ClENG_IndexNode* R ; 
	IndexNodeList_t::iterator it ;
	
	if (ai_iTargetID==m_uiTargetID) return this ;
	
	it = m_oIndexNodeList.find(ai_iTargetID);
	if (it != m_oIndexNodeList.end() ) {	
	
		return ((*it).second) ;	

	} else  {	

		for ( it=m_oIndexNodeList.begin(), R=NULL; it != m_oIndexNodeList.end(); ++it ) {
			R = ((*it).second)->GetSonNodeAux(ai_iTargetID) ;
			if (R) break ;
		} ;
		return R;
	}
}

void ClENG_IndexNode::GetRecursiveNumSons(m4uint32_t* ai_piSons)
{
	IndexNodeList_t::iterator it ;

	*ai_piSons+=GetNumSons();

	for (it = m_oIndexNodeList.begin (); it != m_oIndexNodeList.end(); ++it) {
		(*it).second->GetRecursiveNumSons(ai_piSons);
	} ;
}

m4uint32_t ClENG_IndexNode::GetRecursiveNumSons()
{
	m4uint32_t iNumSons;

	iNumSons=0;
	GetRecursiveNumSons(&iNumSons);

	return iNumSons;
}

m4bool_t ClENG_IndexNode::IsRoot() 
{
	if (!m_poFather) {
		return M4_TRUE ;
	} else {
		return m_poFather->IsRoot() ;
	} ;
}

void ClENG_IndexNode::GetPathToThis( IndexNodeList_t *ao_poList ) 
{
	ClENG_IndexNode *node ;

	ao_poList->clear() ;

	if ( IsDummyRoot() ) return ;

	for ( node=GetFather() ; ! node->IsDummyRoot(); node = node->GetFather() ) {
		ao_poList->insert( IndexNodeList_t::value_type(node->GetTargetID(),node) ) ;
	} ;
}

 


//========================================================================ClENG_OutputDevice

ClENG_OutDevice::ClENG_OutDevice(void)
{
	m_poInsp=GET_INSP();
	m_poLayoutPages=NULL;
	m_poLastAccessedNode=NULL;
	m_iNLayoutPages=0;
}

void ClENG_OutDevice::End(void)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poLayoutPages, ClENG_Page, m_iNLayoutPages 
	) ;

	if(m_poLastAccessedNode){
		m_oIndexRootNode.End();
		m_poLastAccessedNode = NULL;
	};
	m_oBreakIndexRoots.clear() ;

	m_iNLayoutPages=0 ;

	m_oFGPalette.End() ;
}


m4bool_t ClENG_OutDevice::Init( m4int16_t ai_iNumPages, 
	ClMIT_Flag::Value_t ai_Flags,
	ClENG_Palette *ai_poFGPalette,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return=M4_TRUE ;
	ClENG_OutDevice::End();


	if (ai_iNumPages < 1) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDEVICE_MSG_10 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;
	m_uiNPageDvc=0;
	m_uiNPageBB=0;
	m_uiBreakCount=0;
	m_iNLayoutPages=ai_iNumPages;
	m_iNPhysicalPages=0;
	m_uiIndexTotal=0;

	m_oFlag.Reset(ai_Flags) ;
	if ( m_oFlag.False(COLOUR_SUPPORTED) ) {
		m_oFlag.Off(COLOUR_ACTIVATED) ;
	} ;
	if ( m_oFlag.False(RASTER_SUPPORTED) ) {
		m_oFlag.Off(RASTER_PRELOADED) ;
	} ;
	
	m_oFlag.On(VALID_OUTPUT) ;

	if (ai_poFGPalette) {
		m_oFGPalette.Init(ai_poFGPalette) ;
	} else {
		if ( ! m_oFGPalette.Init(0,ClMIT_Trace::PROPAGATE_ERROR) ) {
			Return=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poLayoutPages, ClENG_Page, m_iNLayoutPages 
	) ;

	//Inicializa el nodo raiz del arbol de indices
	m_oIndexRootNode.Init(ClENG_Field::ROOT_ENTRY, 0, "", m_iNLayoutPages, 
		GetPagesDvcPrinted(), 0, NULL );

	m_poLastAccessedNode=&m_oIndexRootNode;


exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

void ClENG_OutDevice::FixOptimizedPreload() 
{
	m4int16_t B ;
	ClFONT_M_Font *Font ;
	m4uint32_t Colour ;

	for (B=0;B<m_iNLayoutPages;++B) {
		m_poLayoutPages[B].m_poActualBuff=&m_poLayoutPages[B].m_oPrBuff ;

		//El ForceSelectFont se hace tras el cambio de m_poActualBuff, para que tenga
		//	efecto en el buffer deseado.
		Font = GetLastFont(B) ;
		if (Font) {
			ForceSelectFont(B, Font) ;
		} ;

		Colour = GetLastColour(B) ;
		if (Colour!=ClENG_Colour::ID_NULL) {
			ForceSelectColour(B, Colour) ;
		} ;
	} ;
}


void ClENG_OutDevice::FixPreload() 
{
	m4int16_t B ;

	for (B=0;B<m_iNLayoutPages;++B) {
		m_poLayoutPages[B].m_poActualBuff=&m_poLayoutPages[B].m_oLayoutBuff ;
	} ;
}

m4bool_t ClENG_OutDevice::PreReset()
{ 
	m4int16_t B ;

	m_uiNPageBB=m_uiNPageDvc ;
	++m_uiBreakCount ;
	m_oBreakIndexRoots.clear()  ;

	for (B=0; B<m_iNLayoutPages; ++B) {
		m_poLayoutPages[B].m_uiPreserve=m_poLayoutPages[B].m_oLayoutBuff.GetTotal();
	} ;
	return M4_TRUE ;
}

void ClENG_OutDevice::PostReset(m4bool_t ai_bEmptyPreserveData)
{ 
	m4int16_t B ;

	if (ai_bEmptyPreserveData) {
		for (B=0; B<m_iNLayoutPages; ++B) {
			m_poLayoutPages[B].m_uiPreserve=0 ;
		} ;
	} ;
}


m4bool_t ClENG_OutDevice::RefreshLayout() 
{
	
	m4int16_t B ;	
	ClFONT_M_Font* Font ;
	m4uint32_t Colour ;

	for (B=0; B<m_iNLayoutPages; ++B ) {
		
		// al inicio de cada página el primer tag es la última 
		// fuente usada
		Font = GetLastFont(B) ;
		if (Font) {
			ForceSelectFont(B, Font) ;
		} ;

		Colour = GetLastColour(B) ;
		if (Colour!=ClENG_Colour::ID_NULL) {
			ForceSelectColour(B, Colour) ;
		} ;

	} ;

	m_iNPhysicalPages++;

	return M4_TRUE ;
}

m4bool_t ClENG_OutDevice::FlushLayout() 
{
	m4int16_t B ;

	for (B=0; B<m_iNLayoutPages; ++B) {
		m_poLayoutPages[B].m_oLayoutBuff.InitPreserve( m_poLayoutPages[B].m_uiPreserve );
	} ;
	m_uiNPageDvc += m_iNLayoutPages ;

	return M4_TRUE ;
}

void ClENG_OutDevice::SelectFont( m4int16_t ai_iPage,  ClFONT_M_Font *ai_poFont  ) 
{
	if ( m_poLayoutPages[ai_iPage].m_poLastFont != ai_poFont ) {
		ForceSelectFont(ai_iPage,ai_poFont) ;
	} ;
	
}

void ClENG_OutDevice::ForceSelectFont( m4int16_t ai_iPage,  ClFONT_M_Font *ai_poFont  ) 
{
	SelectFontDevice( ai_iPage, ai_poFont);
	m_poLayoutPages[ai_iPage].m_poLastFont = ai_poFont  ;
}


void ClENG_OutDevice::SelectColour( m4int16_t ai_iPage,  m4uint32_t ai_uiIdColour ) 
{
//	if ( IsColourOn() ) {
		if ( m_poLayoutPages[ai_iPage].m_uiLastIdColour != ai_uiIdColour ) 
		{
			ForceSelectColour(ai_iPage,ai_uiIdColour) ;
		} ;
//	} ;
}

void ClENG_OutDevice::ForceSelectColour( m4int16_t ai_iPage,  m4uint32_t ai_uiIdColour ) 
{
//	if ( IsColourOn() ) {
		SelectColourDevice( ai_iPage, ai_uiIdColour);
		m_poLayoutPages[ai_iPage].m_uiLastIdColour = ai_uiIdColour  ;
//	} ;
}



void ClENG_OutDevice::PrintStr( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
	m4pchar_t ai_pcStr, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID ) 
{ 	
	PrintNChar( ai_iPage, ai_X, ai_Y, ai_pcStr, 
		strlen(ai_pcStr), ai_bRawText, ai_pcHyperlink, ai_iTargetID );
}

void ClENG_OutDevice::RunAllEOP()
{
	m4int16_t B ;

	for (B=0; B<m_iNLayoutPages; ++B) {
		EOP(B) ;
	} ;
}

void ClENG_OutDevice::PrintFrame( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineWidth, STYLE ai_Style
	) 
{
	ai_X += GetOffsetX(ai_iPage) ;
	ai_Y += GetOffsetY(ai_iPage) ;

	PrintLine( ai_iPage, ai_X, ai_Y, ai_X+ai_Width-1, ai_Y, ai_LineWidth, ai_Style ) ;
	PrintLine( ai_iPage, ai_X, ai_Y+ai_Height-1, ai_X+ai_Width-1, ai_Y+ai_Height-1, ai_LineWidth, ai_Style ) ;
	PrintLine( ai_iPage, ai_X, ai_Y, ai_X, ai_Y+ai_Height-1, ai_LineWidth, ai_Style ) ;
	PrintLine( ai_iPage, ai_X+ai_Width-1, ai_Y, ai_X+ai_Width-1, ai_Y+ai_Height-1, ai_LineWidth, ai_Style ) ;
}


void ClENG_OutDevice::ShowRegion( 
		ClENG_Region *ai_poRegion,
		m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase ) 
{
	ai_poRegion->ShowOnDvc(ai_iNumPage,ai_XBase,ai_YBase ) ;
}

void ClENG_OutDevice::ShowRegion( 
		ClENG_Region *ai_poRegion,
		m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, 
		DvcUnit_t ai_HeightPrinted, DvcUnit_t ai_HeightToPrint ) 
{
	ai_poRegion->ShowOnDvc(ai_iNumPage,ai_XBase,ai_YBase,ai_HeightPrinted,ai_HeightToPrint) ;
}


m4bool_t ClENG_OutDevice::NewIndexEntry(m4int16_t ai_iNumPage, m4int32_t ai_iIndexLevel, const m4char_t* ai_pcIndexString, m4uint16_t* ai_iTargetID)
{
	ClENG_IndexNode* NewIndexNode;
	m4bool_t SwOk = M4_TRUE;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, NewIndexNode, ClENG_IndexNode);

	if(m_poLastAccessedNode) {
		m_uiIndexTotal++;

		//Comprobamos que no llega una entrada de indice 0
		if(ai_iIndexLevel==0) {
			*ai_iTargetID = DVC_NO_INDEX_TARGET;
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163010, "Error building table of contents. The index entry 0 is reserved for the report root and cannot be used.", ClMIT_Msg::MIXED_OUT) 
				<< ai_iIndexLevel << ai_pcIndexString << m_poLastAccessedNode->GetIndexLevel() << SEND_MSG ;
			SwOk=M4_FALSE;
			goto exit;
		};

		//Chequeamos que el orden de llegada ha sido correcto (no puede llegar un ROOT ni
		// puede llegar un segundo nivel inferior al ultimo accedido
		if(ai_iIndexLevel<1 || ai_iIndexLevel>m_poLastAccessedNode->GetIndexLevel()+1) {
			//ERROR DE LOGICA
			*ai_iTargetID = DVC_NO_INDEX_TARGET;
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163007, "Error building table of contents. The index entry of level %0:s and contents %1:s is incompatible with the last accessed entry of level %2:s.", ClMIT_Msg::MIXED_OUT) 
				<< ai_iIndexLevel << ai_pcIndexString << m_poLastAccessedNode->GetIndexLevel() << SEND_MSG ;
			SwOk=M4_FALSE;
			goto exit;
		};

		//Actualizacion del ultimo nodo accedido (pr si hay que volver hacia atras)
		while (ai_iIndexLevel<m_poLastAccessedNode->GetIndexLevel()+1) {
			m_poLastAccessedNode = m_poLastAccessedNode->GetFather();
		};

		//Inicializamos el nodo hijo y se lo asignamos al ultimo nodo accedido
		NewIndexNode->Init(ai_iIndexLevel, m_uiIndexTotal, ai_pcIndexString, GetNumLayoutPages(), 
			               GetPagesDvcPrinted(), GetNumPageDvc(ai_iNumPage), m_poLastAccessedNode);

		m_poLastAccessedNode->InsertSonNode(NewIndexNode);

		//Actualizamos el ultimo nodo accedido a este
		m_poLastAccessedNode = NewIndexNode;

		//Actualiza el valor del ID de salida
		*ai_iTargetID = m_uiIndexTotal;

		//Actualiza m_poBreakRootNode si es el primero tras un break

		if (m_oBreakIndexRoots.size()==0) {
			m_oBreakIndexRoots.push_back(m_poLastAccessedNode)  ;
		} else if (  m_poLastAccessedNode->IsBrother( m_oBreakIndexRoots[0] ) ) {
			m_oBreakIndexRoots.push_back(m_poLastAccessedNode)  ;
		} ;

	} else {
		//ERROR DE ACCESO
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163008, "Error building table of contents. Unable to access index entry of level %0:s and contents %1:s.", ClMIT_Msg::MIXED_OUT) 
				<< ai_iIndexLevel << ai_pcIndexString << SEND_MSG ;
		*ai_iTargetID = DVC_NO_INDEX_TARGET;
		SwOk = M4_FALSE;
		goto exit;
	};

exit:

	if(!SwOk) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, NewIndexNode, ClENG_IndexNode);
	};

	return SwOk;
}