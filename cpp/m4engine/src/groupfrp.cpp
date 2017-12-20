//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             groupfrp.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 07 abr 98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"

#include "sysinfo.h"
#include "mitdates.h"
#include "m4clbuf.h"
#include "xapichn.h"
#include "m4engprn.hpp"

#include "groupfrp.h"

// UNICODE
#include "m4unicode.hpp"

// Reevaluación de expresiones
#include "analex.h"

#define GROUP_ID    ENG_TGI_CLDVCFI


//================================================================ClENG_GlobalGroup

ClENG_GlobalGroup::ClENG_GlobalGroup()
{
	m_poInsp=GET_INSP();
	m_poOwner = NULL ;
}


void ClENG_GlobalGroup::End() 
{	
	m_oPhysPath=NULL ;
	m_oFileList.clear() ;
	m_poOwner = NULL ;
	m_oRelativePath.End() ;
	m_oPhysPath=NULL ;
}

m4bool_t ClENG_GlobalGroup::Init( 
	ClENG_ReportOuputRepository *ai_poOwner,
	const m4char_t *ai_pcRelativePath,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return = M4_TRUE ;

	ClENG_GlobalGroup::End();

	m_oRelativePath.StrCpy(ai_pcRelativePath) ;
	m_poOwner = ai_poOwner ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;
} 


ClENG_PhysicalFile * ClENG_GlobalGroup::GetGlobalReference(
	const m4char_t *ai_pcFileNameId, 
	m4bool_t ai_bSwCommonForAllBreaks, 
	m4bool_t *ao_SwCreated) 
{
	ClENG_PhysicalFile *R=NULL ;
	ClMIT_Str key, fullPath ;
	FileList_t::iterator it ;
	m4bool_t created=M4_FALSE ;
	StSecondFileList second, *psecond ;

	if (!m_oPhysPath) {
		if ( !(m_oPhysPath=m_poOwner->m_oFileSet.NewRelativeDir( 
			m_oRelativePath.InChar(0), M4_TRUE, ClMIT_Trace::PROPAGATE_ERROR ) ) ) 
		{
			goto exit ;
		} ;
	} ;

	key.StrCpy(ai_pcFileNameId) ;
	key.ToLower() ;

	it = m_oFileList.find(key) ;
	if ( it != m_oFileList.end() ) {

		//Referencia existe, devolverla y ya esta
		created=M4_FALSE ;
		R = (*it).second.m_poFile ;
		psecond = &(*it).second ;

	} else {

		//Referencia NO existe

		m_oPhysPath->MakeAbsolutePathFile(&fullPath,ai_pcFileNameId) ;
		if ( ClMIT_File::Exist(fullPath) ) {

			//Fichero existe, crear solo referencia a el
			created=M4_FALSE ;
			R = GetOutputRepository()->m_oFileSet.LinkToFile(
				m_oPhysPath, ai_pcFileNameId, ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!R) goto exit ;
		
		} else {

			//Fichero NO existe, crearlo
			created=M4_TRUE ;
			R = GetOutputRepository()->m_oFileSet.CreateFile(
				m_oPhysPath, ai_pcFileNameId, ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!R) goto exit ;
		} ;
		
		//Meter en lista para futuras referencias

		second.m_poFile = R ;
		second.m_bSwForAllBreaks = ai_bSwCommonForAllBreaks ;
		psecond = &second ;
		m_oFileList.insert( FileList_t::value_type(key,second) ) ;

	} ;

	//Test de concordancia para llamdas mas alla de la primera

	if (!created) {
		if ( ! ai_bSwCommonForAllBreaks && psecond->m_bSwForAllBreaks ) {
			//Si ahora privado y antes publico, fuerza a publico
			ai_bSwCommonForAllBreaks=M4_TRUE ;
		} else if ( ai_bSwCommonForAllBreaks && ! psecond->m_bSwForAllBreaks ) {
			//Si ahora publico y antes privado, error (no puedo notificar a privados)
			R->GetAbsolutePathFile(&fullPath) ; 
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163020,
				"El fichero %0:s se declaro como privado para algunos breaks y luego como publico para todos.",
				ClMIT_Msg::MIXED_OUT) 
				<< fullPath.InChar(0) << SEND_MSG ;
		} ;
	
	} ;

	//Proceso de notificacion para privado

	if ( ! ai_bSwCommonForAllBreaks ) {
			
		if ( ! m_poOwner->m_oBreaks.m_poCurrentBreak ) {
			//NO existe el break
			
			if (created) {
				//Si es PRIMERA VEZ, no hay nada creado, forzar TRUE
				psecond->m_bSwForAllBreaks=M4_TRUE ;
			} else {
				//No es primera vez, error.
				R->GetAbsolutePathFile(&fullPath) ; 
					*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163021,
					"El fichero %0:s no puede declararse privado al break porque no hay break actual.",
					ClMIT_Msg::MIXED_OUT) 
					<< fullPath.InChar(0) << SEND_MSG ;
			} ;
		} else {

			//Existe el break
			//METER SOLO SI NO EXISTE YA !!!

			m_poOwner->m_oBreaks.m_poCurrentBreak->m_oGlobalDependencies.insert(
				ClENG_BreakGroup::GlobalDependencies_t::value_type(R,R)) ;
			
		} ;
	} ;


exit:
	if (ao_SwCreated) *ao_SwCreated=created ;
	return R ;
}

m4bool_t ClENG_GlobalGroup::Remove() 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_PhysicalFile *F ;

	FileList_t::iterator it ;
	for ( it=m_oFileList.begin(); it!=m_oFileList.end(); ++it ) {
		F = (*it).second.m_poFile ;
		if ( ! F->Remove() ) {
			SwOk=M4_FALSE ;
		} ;
	} ;
	return SwOk ;
}


//================================================================ClENG_BreakList

ClENG_BreakList::ClENG_BreakList()
{
	m_poInsp=GET_INSP();
	m_poCurrentBreak=NULL ;
	m_oCurrentFlush=NULL ;
	m_oRedirectList=NULL ;
	m_oPrintedPages=NULL ;
	m_SharingFlushPages=NULL ;
	m_poOwner=NULL ;
}


void ClENG_BreakList::End() 
{	
	BreakList_t::iterator it ;
	
	m_poCurrentBreak=NULL ;

	m_oExtension.End() ;

	for (it=m_oBreakList.begin(); it!=m_oBreakList.end(); ++it) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, (*it).second, ClENG_BreakGroup ) ;
	} ;
	m_oBreakList.clear() ;
	m_oWaitingBreaks.clear() ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oCurrentFlush, ClENG_PhysicalFile *, m_PageLayoutCount ) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oRedirectList, m4int16_t, m_PageLayoutCount ) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oPrintedPages, m4uint32_t, m_PageLayoutCount ) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_SharingFlushPages, m4uint32_t, m_PageLayoutCount ) ;


	m_poOwner=NULL ;
}

m4bool_t ClENG_BreakList::Init( 

		ClENG_ReportOuputRepository *ai_poOwner,
		
		//General flow modifiers

		BREAK_MODE	ai_BreakMode, 
		DUPLEX_MODE ai_DuplexMode,

		//Name file modifiers

		m4int16_t ai_iNumOfSecChars,
		m4int16_t ai_iNumOfPageChars,
		m4char_t  ai_cSeparator,
		m4char_t  ai_cBreakSeparator,
		const m4char_t *ai_pcExtension,

		//Break output management

		m4int16_t ai_iOffset,
		m4bool_t ai_bUseDelBreak,
		m4bool_t ai_bDefaultDelBreak,
		m4bool_t ai_bConsecutiveBreaks,

		//Flush management
		m4int16_t ai_PageLayoutCount,

		// UNICODE REPORTS
		m4bool_t ai_bIsUTF16,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )

{
	m4bool_t Return = M4_TRUE ;
	m4int16_t B ;

	ClENG_BreakList::End();

	m_poOwner = ai_poOwner ;

	//Carga props

	m_BreakMode = ai_BreakMode ;
	m_DuplexMode = ai_DuplexMode ;
	m_PageLayoutCount = ai_PageLayoutCount ;

	m_iNumOfSecChars  =  ai_iNumOfSecChars;
	m_iNumOfPageChars = ai_iNumOfPageChars ;
	m_cSeparator = ai_cSeparator ;
	m_cBreakSeparator = ai_cBreakSeparator ;
	m_oExtension.StrCpy(ai_pcExtension) ;

	m_iOffset = ai_iOffset ;
	m_bUseDelBreak = ai_bUseDelBreak ;
	m_bDefaultDelBreak = ai_bDefaultDelBreak ;
	m_bConsecutiveBreaks = ai_bConsecutiveBreaks ;
	m_uiDeletedBreaks = 0 ;
	m_uiPublishedBreaks = 0 ;

	m_bIsUTF16 = ai_bIsUTF16;

	//Calculos duplex e inicializacion arrays Flush 

	if ( m_DuplexMode==EVEN_ODD_DUPLEX && m_PageLayoutCount>1 && m_PageLayoutCount%2 == 1 ) {
		m_bPhantomPage=M4_TRUE ;
		++m_PageLayoutCount ;
	} else {
		m_bPhantomPage=M4_FALSE ;
	} ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oCurrentFlush, ClENG_PhysicalFile *, m_PageLayoutCount ) ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oRedirectList, m4int16_t, m_PageLayoutCount ) ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_oPrintedPages, m4uint32_t, m_PageLayoutCount ) ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_SharingFlushPages, m4uint32_t, m_PageLayoutCount ) ;
	

	for (B=0;B<m_PageLayoutCount;++B) {
		m_oCurrentFlush[B] = NULL ;
		m_oPrintedPages[B] = 0 ;
		switch (m_DuplexMode) {
		case SHARING_DUPLEX:
			m_oRedirectList[B] = 0 ;
			m_SharingFlushPages[B] = m_PageLayoutCount ;
			break ;
		case EVEN_ODD_DUPLEX:
			m_oRedirectList[B] = B%2 ;
			m_SharingFlushPages[B] = m_PageLayoutCount/2  ;
			break ;
		case ATOMIC_DUPLEX:
		default :
			m_oRedirectList[B] = B ;
			m_SharingFlushPages[B]=1 ;
			break ;
		} ;
	} ;

	m_uiCurrentBreakSequence = m_iOffset ;
	m_uiPhysicalPage = 0 ;
	m_uiPhysicalPagePerBreak = 0 ;
exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;
} 

m4bool_t ClENG_BreakList::NewBreak(m4bool_t *ao_bNewFlushFilesOpenned)
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str path ;
	ClMIT_Str *alternativeMaskName = NULL ;
	ClENG_Path *pathObj=NULL ;
	ClENG_BreakGroup *brkGroup=NULL ;
	pair<BreakList_t::iterator,bool> Pair ;
	m4bool_t SwInserted =M4_FALSE ;

	if ( m_poOwner->IsMonoBreak() && m_oBreakList.size()>0) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163022,
			"El report es mono break y se está intentando generar más de un break", ClMIT_Msg::MIXED_OUT) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	if ( m_BreakMode== BREAK_TO_DIR ) {
		
		//Obtengo nuevo directorio para break

		SwOk = NewBreakDirectory(&path) ;
		if (!SwOk) goto exit ;

		//Debo crear nuevo ClENG_Path a nivel fisico

		if ( ! (pathObj = m_poOwner->m_oFileSet.NewRelativeDir(
			path, M4_TRUE, ClMIT_Trace::PROPAGATE_ERROR) ) ) 
		{
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} else {
	
		//El ClENG_Path a usar es el raiz

		pathObj = m_poOwner->m_oFileSet.GetRootPathReference() ;


	} ;

	if ( path.StrLen() > 0 && m_poOwner->HasBreakFileExpression() )
	{
		alternativeMaskName = &path;
	}

	//Creo nuevo BreakGroup con el path pasado

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, brkGroup, ClENG_BreakGroup ) ;
	if ( ! brkGroup->Init(this, m_oBreakList.size(), pathObj, alternativeMaskName, ClMIT_Trace::PROPAGATE_ERROR) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Inserto en lista

	Pair = m_oBreakList.insert( BreakList_t::value_type(brkGroup->GetBreakId(),brkGroup) ) ;
	if ( ! Pair.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163023,
			"ClENG_BreakList::NewBreak for BreakId %0:s failed, break exists", ClMIT_Msg::MIXED_OUT) 
			<<  brkGroup->GetBreakId() << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	}  else {
		SwInserted=M4_TRUE ;
	} ;

	//Actual break es este

	m_poCurrentBreak = brkGroup ;

	//Abre FlushFiles solo para modo NO ATOMIC (todos flush comprten ficheros).

	if  (m_DuplexMode != ATOMIC_DUPLEX )  {
		if ( ! OpenFlushFiles()  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( ao_bNewFlushFilesOpenned )  *ao_bNewFlushFilesOpenned=M4_TRUE ;
	} else {
		if ( ao_bNewFlushFilesOpenned )  *ao_bNewFlushFilesOpenned=M4_FALSE ;
	} ;

	//Incrementa para futuros breaks 

	++m_uiCurrentBreakSequence ;

	if  (m_DuplexMode == ATOMIC_DUPLEX ) {
		m_uiPhysicalPagePerBreak=0 ;
	} ;
exit:

	if (!SwOk) {
		if (SwInserted) {
			m_oBreakList.erase(Pair.first) ;
		} ;
		if (brkGroup) {
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, brkGroup, ClENG_BreakGroup ) ;
		} ;
	} ;
	return SwOk ;
}


m4bool_t ClENG_BreakList::CloseBreak(m4bool_t ai_bIsValidOuput)
{
	m4bool_t SwOk=M4_TRUE, SwAux ;
	WaitingBreaks_t::iterator it, nextIt ;
	ClENG_BreakGroup *brk ; 

	//Cierra los ficheros, antes de cualquier otra cosa

	if  (m_DuplexMode != ATOMIC_DUPLEX )  {
		CloseFlushFiles() ;
	} ;

	//Testea el estado para el resto de los que esperan

	for (it=m_oWaitingBreaks.begin() ; it!=m_oWaitingBreaks.end(); it=nextIt) {
		nextIt=it ; nextIt++ ;
		brk = (*it) ;
		if ( brk->IsReadyToClose( &SwAux ) ) {
	
			//Cierra y saca de la lista
			if ( ! brk->CloseBreak(ai_bIsValidOuput) ) {
				SwOk=M4_FALSE ;
			} ;
			m_oWaitingBreaks.erase( it ) ;
			
		} ;
		if (!SwAux) SwOk=M4_FALSE ;
	} ;

	//Pasa el actual break a WAITING o CLOSED

	if ( m_poCurrentBreak->IsReadyToClose( &SwAux ) ) {
	
		//Cierra
		
		if ( ! m_poCurrentBreak->CloseBreak(ai_bIsValidOuput) ) {
			SwOk=M4_FALSE ;
		} ;

	} else {
	
		//Pasa a Waiting y mete en lista de espera
		m_poCurrentBreak->ChangeToWaiting() ;
		m_oWaitingBreaks.push_back(m_poCurrentBreak) ;
	
	} ;
	if (!SwAux) SwOk=M4_FALSE ;

	//El actual break ya no es el current

	m_poCurrentBreak = NULL ;

exit:
	return SwOk ;
} ;



m4bool_t ClENG_BreakList::NewFlush(m4bool_t *ao_bNewFlushFilesOpenned)
{
	m4bool_t SwOk=M4_TRUE ;


	//Abre FlushFiles solo para modo ATOMIC (un juego de ficheros por flush).

	if  (m_DuplexMode == ATOMIC_DUPLEX )  {
		if ( ! OpenFlushFiles()  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( ao_bNewFlushFilesOpenned )  *ao_bNewFlushFilesOpenned=M4_TRUE ;
	} else {
		if ( ao_bNewFlushFilesOpenned )  *ao_bNewFlushFilesOpenned=M4_FALSE ;
	} ;

exit:
	return SwOk ;
} ;

	

m4bool_t ClENG_BreakList::CloseFlush(M4ClBuffer **ai_poBuffArray)
{
	m4bool_t SwOk=M4_TRUE ;

	m4int16_t B ;
	m4uint32_t FilePos;
	ClENG_PhysicalFile *F ;
	
	for (B=0; B<m_PageLayoutCount; ++B ) {

		F = GetFlushFile(B) ;
		m_oPrintedPages[ m_oRedirectList[B] ] ++ ;
		FilePos = F->GetPos();
		
		GetOutputRepository()->m_oPI_Manager.MoveAllPiToFile(B, F, FilePos) ;
		F->DumpBuffer( ai_poBuffArray[B] ) ;
	} ;

	if  (m_DuplexMode == ATOMIC_DUPLEX )  {
		CloseFlushFiles() ;
		m_uiPhysicalPage += m_PageLayoutCount ;
		m_uiPhysicalPagePerBreak += m_PageLayoutCount ;
	} ;

exit:
	return SwOk ;
} ;

m4bool_t ClENG_BreakList::IsGoingToCloseFlushSet() 
{
	if ( GetOutputRepository()->m_State==ClENG_ReportOuputRepository::OUT_OF_FLUSH ) {

		return m_DuplexMode != ATOMIC_DUPLEX ;

	} else if ( GetOutputRepository()->m_State==ClENG_ReportOuputRepository::PROCESSING_FLUSH ) {

		return m_DuplexMode == ATOMIC_DUPLEX ;

	} else {

		return M4_FALSE ;

	} ;
}


void ClENG_BreakList::CloseFlushFiles() 
{
	m4int16_t B ;
	ClENG_PhysicalFile *fi ;

	for (B=0;B<m_PageLayoutCount;++B) {
		fi = m_oCurrentFlush[B] ;
		if ( ! IsRedirected(B) && fi ) {
			if ( fi->IsOpen() ) fi->Close() ;
		} ;
		m_oCurrentFlush[B]=NULL ;
	} ;

}

m4bool_t ClENG_BreakList::OpenFlushFiles() 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClENG_PhysicalFile *fi ;
	ClMIT_Str path ;
	m4bool_t SwExist ;

	//Crea nuevos ficheros (no redirected)

	for (B=0;B<m_PageLayoutCount;++B) {
		if ( ! IsRedirected(B) ) {

			if ( ! NewBreakFile(&path, &SwExist, B) ) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;

			fi = m_poCurrentBreak->CreateMainDataFile(path) ;
			if (!fi) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			if ( ! fi->Open( ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;

			// UNICODE REPORTS.
			// Insertar la marca UTF16 en el fichero, sólo en el dispositivo de texto.
			if (m_bIsUTF16 == M4_TRUE)
			{
				fi->W(acUTF16BOM, M4UNI_UTF16_BOM_SIZE);
			}

			m_oCurrentFlush[B]=fi ;
		} ;
	} ;


	//Asigna redirected

	for (B=0;B<m_PageLayoutCount;++B) {
		m_oPrintedPages[B]=0 ;
		if ( IsRedirected(B) ) {
			m_oCurrentFlush[B]=m_oCurrentFlush[m_oRedirectList[B]] ;
		} ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_BreakList::NewBreakDirectory(ClMIT_Str *ao_Path) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str path[2];
	ClMIT_Str aux, aux2 ;
	size_t lroot ;
	m4uint32_t j ;
	m4uint32_t iNumPaths = 1 ;
	m4uint32_t iValidIndex = 0 ;
	m4bool_t bExist;
	ClENG_ReportOuputRepository::EXPRESSION_RESULT expressionResult;

	//Monta rootPath\makName_BREAK
	
	m_poOwner->GetRootPath(&path[0]) ;
	ClMIT_File::ConvertToPath(&path[0]) ;
	lroot = path[0].StrLen() ;
	m_poOwner->GetMaskName(&aux) ;
	ClMIT_File::ConcatToPath(&path[0],aux) ;

	// Se evula la expresión, si existe
	expressionResult = m_poOwner->ResolveExpressionFile(&aux) ;

	switch (expressionResult)
	{
		case ClENG_ReportOuputRepository::EXPRESSION_ERROR:
			SwOk = M4_FALSE;
			goto exit ;
		case ClENG_ReportOuputRepository::EXPRESSION_OK:
			path[0].StrCat(aux);
			break;
		default: //case ClENG_ReportOuputRepository::NO_EXPRESSION:
			path[0].StrCat("_BREAK") ;
			break;
	}

	//Si procede, añade el numero de break
	if ( ! m_poOwner->IsMonoBreak() )
	{
		SwOk = GetNumOfSecCharsAddString(&aux)  ;
		if (!SwOk) goto exit ;

		if (expressionResult == ClENG_ReportOuputRepository::EXPRESSION_OK)
		{
			path[1].StrCpy(path[0]);
			iNumPaths = 2;
		}
		
		path[iNumPaths - 1].StrCat(aux) ;
	} ;

	//------------->Seccion critica
	m_poOwner->m_oMutexPreserveMove.Lock() ;

	// Se recorre las distintas alternativas de fichero buscando uno que no exista
	for (j = 0 ; j < iNumPaths; j++)
	{
		iValidIndex = j ;
		bExist = ClMIT_File::Exist(path[iValidIndex])  ;

		if ( !bExist ) break;
	}

	//Ya tenemos el path, ahora hay que comprobar modos preserve

	switch ( m_poOwner->GetPreserveMode() )
	{
	case ClENG_ReportOuputRepository::PRESERVE_DIR:
	case ClENG_ReportOuputRepository::NO_PRESERVE:

		if ( ! bExist ) {
			if ( ClMIT_File::CreatePath( path[iValidIndex].InChar(0), ClMIT_File::TRY_CREATE ) != ClMIT_File::PATH_CREATED ) {
				SwOk=M4_FALSE ;
			} ;
		} ;
		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_FIX_NAME:

		if ( bExist  ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163024,
				"Path %0 existe, y se especifico PRESERVE", ClMIT_Msg::MIXED_OUT) 
				<<  path[iValidIndex].InChar(0) << SEND_MSG ;
			SwOk=M4_FALSE ;
		} else {
			if ( ClMIT_File::CreatePath( path[iValidIndex].InChar(0), ClMIT_File::TRY_CREATE ) != ClMIT_File::PATH_CREATED ) {
				SwOk=M4_FALSE ;
			} ;
		} ;

		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_MOVE_NAME:

		if ( bExist ) {
			while (M4_TRUE) {
				aux.StrCpy(path) ;
				aux.StrCat("_");		
				ClMIT_Dates::DateToFileString( &aux2, ClMIT_Dates::ActualTimeStamp() ) ;
				aux.StrCat( aux2 ) ;
				if ( ! ClMIT_File::Exist(aux) ) {
					path[iValidIndex].StrCpy(aux) ;
					break ;
				} ;
			} ;
		} ;
		if ( ClMIT_File::CreatePath( path[iValidIndex].InChar(0), ClMIT_File::TRY_CREATE ) != ClMIT_File::PATH_CREATED ) {
			SwOk=M4_FALSE ;
		} ;
			
		break;
	} ;
	
	//<-------------
	m_poOwner->m_oMutexPreserveMove.Unlock() ;

	
	ao_Path->StrCpy(path[iValidIndex].InChar(lroot)) ;
exit:
	return SwOk ;
}

m4bool_t ClENG_BreakList::NewBreakFile(ClMIT_Str *ao_Path, m4bool_t * ao_bExist, m4int16_t ai_iPage) 
{
	m4bool_t SwOk=M4_TRUE ;
	// En el caso de evaluación de expresiones path[1] tendrá
	// un valor alternativo que permitirá gestionar valores repetidos
	ClMIT_Str path[2] ;
	ClMIT_Str pathWithoutExtension[2] ;
	ClMIT_Str aux, aux2 ;
	size_t lroot ;
	m4uint32_t i, j ;
	m4uint32_t iNumPaths = 1 ;
	m4uint32_t iValidIndex = 0 ;
	ClENG_ReportOuputRepository::EXPRESSION_RESULT expressionResult;

	//Monta rootPath\makName
	
	m_poOwner->GetRootPath(&path[0]) ;
	ClMIT_File::ConvertToPath(&path[0]) ;
	lroot = path[0].StrLen() ;
	m_poOwner->GetMaskName(&aux) ;
	ClMIT_File::ConcatToPath(&path[0],aux) ;

	// Se evula la expresión, si existe
	expressionResult = m_poOwner->ResolveExpressionFile(&aux) ;

	switch (expressionResult)
	{
		case ClENG_ReportOuputRepository::EXPRESSION_ERROR:
			SwOk = M4_FALSE;
			goto exit ;
		case ClENG_ReportOuputRepository::EXPRESSION_OK:
			path[0].StrCat(aux);
			break;
	}

	//Si procede, añade el numero de break
	if ( ! m_poOwner->IsMonoBreak() && m_BreakMode == BREAK_TO_FILE )
	{
		SwOk = GetNumOfSecCharsAddString(&aux)  ;
		if (!SwOk) goto exit ;

		if (expressionResult == ClENG_ReportOuputRepository::EXPRESSION_OK)
		{
			path[1].StrCpy(path[0]);
			iNumPaths = 2;
		}
		
		path[iNumPaths - 1].StrCat(aux) ;
	} ;

	//Si procede añade el numero de pag layout o de pagina fisica

	if ( m_DuplexMode==EVEN_ODD_DUPLEX ) {
		
		//Page es pagina de layout, en este caso se llama con 0 o 1
		
		SwOk = GetDuplexAddString(&aux,ai_iPage)  ;
		if (!SwOk) goto exit ;

		for (j = 0; j < iNumPaths; j++)
		{
			if (m_cSeparator) path[j].StrInsertChar(ClMIT_Str::STR_CAT,1,m_cSeparator) ;

			path[j].StrCat(aux) ;
		}

	}  else if (m_DuplexMode==ATOMIC_DUPLEX ) {

		//Hay que obtener la pagina fisica pero dentro del break actual o global, depende del
		//	modo de manejo de breaks TO_DIR o TO_FILE.

		if ( m_BreakMode==BREAK_TO_FILE ) {
			i = m_uiPhysicalPage+ai_iPage ;
		} else {
			i = m_uiPhysicalPagePerBreak+ai_iPage ;
		} ;
		SwOk = GetDuplexAddString(&aux, i ) ;
		if (!SwOk) goto exit ;

		for (j = 0; j < iNumPaths; j++)
		{
			if (m_cSeparator) path[j].StrInsertChar(ClMIT_Str::STR_CAT,1,m_cSeparator) ;

			path[j].StrCat(aux) ;
		}
	} ;

	//Se añade extension
	for (j = 0; j < iNumPaths; j++)
	{
		pathWithoutExtension[j].StrCpy(path[j]) ;
		ClMIT_File::ConcatFileExt(&path[j],m_oExtension) ;
	}

	//------------->Seccion critica
	m_poOwner->m_oMutexPreserveMove.Lock() ;

	// Se recorre las distintas alternativas de fichero buscando uno que no exista
	for (j = 0 ; j < iNumPaths; j++)
	{
		iValidIndex = j ;
		*ao_bExist = ClMIT_File::Exist(path[iValidIndex])  ;

		if ( !*ao_bExist ) break;
	}

	//Ya tenemos el file, ahora hay que comprobar modos preserve.
	switch ( m_poOwner->GetPreserveMode() )
	{
	case ClENG_ReportOuputRepository::PRESERVE_DIR:
	case ClENG_ReportOuputRepository::NO_PRESERVE:

		;

		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_FIX_NAME:

		if ( *ao_bExist  ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163025,
				"Ficher %0 existe, y se especifico PRESERVE", ClMIT_Msg::MIXED_OUT) 
				<<  path[iValidIndex].InChar(0) << SEND_MSG ;
			SwOk=M4_FALSE ;
		} ;

		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_MOVE_NAME:

		if ( *ao_bExist ) {
			while (M4_TRUE) {
				aux.StrCpy(pathWithoutExtension[iValidIndex]) ;
				aux.StrCat("_");		
				ClMIT_Dates::DateToFileString( &aux2, ClMIT_Dates::ActualTimeStamp() ) ;
				aux.StrCat( aux2 ) ;
				aux.StrCat(".") ;
				aux.StrCat(m_oExtension) ;
				if ( ! ClMIT_File::Exist(aux) ) {
					*ao_bExist = M4_FALSE ;
					path[iValidIndex].StrCpy(aux) ;
					break ;
				} ;
			} ;
		} ;
			
		break;
	} ;
	
	//<-------------
	m_poOwner->m_oMutexPreserveMove.Unlock() ;

	ao_Path->StrCpy(path[iValidIndex].InChar(lroot)) ;
exit:
	return SwOk ;
}


m4bool_t ClENG_BreakList::GetNumOfSecCharsAddString( ClMIT_Str *ao_Str ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4uint32_t breakSequence ;

	if ( ! m_bConsecutiveBreaks ) {
		breakSequence = m_uiCurrentBreakSequence ;
	} else {
		breakSequence = m_uiCurrentBreakSequence - m_uiDeletedBreaks ;
	} ;


	ao_Str->Accommodate(100) ;

	if ( m_iNumOfSecChars<0) {

		//Ajuste auto

		sprintf(ao_Str->InChar(0), "%lu", (unsigned long)breakSequence );

	} else if ( m_iNumOfSecChars==0 ) {
	
		//NO hay chars

		if (m_oBreakList.size()>0) {
			// Error local, si hay 0 caracteres y se genera mas de un grupo
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162995, 
				"Not enough design characters to manage all the break files", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
			SwOk=M4_FALSE ;		
			//Aunque haya error, el path puede seguir rellenandose
		} ;
		ao_Str->StrCpy("") ;

	} else {

		//Ajuste fijo

		sprintf(ao_Str->InChar(0), "%lu", (unsigned long)(breakSequence) ); 
		if (ao_Str->StrLen() > m_iNumOfSecChars) {
			// Error local
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162995, 
				"Not enough design characters to manage all the break files", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
			SwOk=M4_FALSE ;		
			//Aunque haya error, el path puede seguir rellenandose
		} ;
		sprintf(ao_Str->InChar(0), "%*.*lu", 
			(int)m_iNumOfSecChars, (int)m_iNumOfSecChars, 
			(unsigned long)(breakSequence) ); 
	} ;

	if (m_cBreakSeparator) {
		ao_Str->StrInsertChar(0,1,m_cBreakSeparator) ;
	} ;

exit:
	ao_Str->OverStringValid() ;
	return SwOk ;
}


m4bool_t ClENG_BreakList::GetDuplexAddString( ClMIT_Str *ao_Str, m4int16_t ai_iNPage ) 
{
	m4bool_t SwOk=M4_TRUE ;

	ao_Str->Accommodate(100) ;

	if ( m_iNumOfPageChars<0) {

		//Ajuste auto

		sprintf(ao_Str->InChar(0), "%i", (int)ai_iNPage);

	} else if ( m_iNumOfPageChars==0 ) {
	
		//NO hay chars

		// Error local, si hay 0 caracteres y se genera mas de un grupo
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162997, 
			"No characters to manage layout page files", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		SwOk=M4_FALSE ;		
		//Aunque haya error, el path puede seguir rellenandose
		ao_Str->StrCpy("") ;

	} else {

		//Ajuste fijo

		sprintf(ao_Str->InChar(0), "%i", (int)ai_iNPage ); 
		if (ao_Str->StrLen() > m_iNumOfPageChars) {
			// Error local
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162996, 
				"Not enough design characters to manage all the page layouts", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
			SwOk=M4_FALSE ;		
			//Aunque haya error, el path puede seguir rellenandose
		} ;
		sprintf(ao_Str->InChar(0), "%*.*i", 
			(int)m_iNumOfPageChars, (int)m_iNumOfPageChars, (int)ai_iNPage ); 
	} ;


exit:
	ao_Str->OverStringValid() ;
	return SwOk ;


}


void ClENG_BreakList::GetNavigationPathMask(ClMIT_Str *ao_Str, m4bool_t ai_bOnlyMainMode) 
{
	BreakList_t::iterator it ;

	ClENG_BreakGroup::MainDataList_t::iterator itData ;
	ClENG_BreakGroup *brk ;
	ClENG_PhysicalFile *f ;
	m4bool_t swEmpty ;
	ClMIT_Str strAux ;

	ao_Str->StrCpy("") ;

	for ( it = m_oBreakList.begin(), swEmpty=M4_TRUE ; it != m_oBreakList.end(); ++it ) {
		brk = (*it).second ;
		if ( brk->GetState()==ClENG_BreakGroup::REMOVED ) continue ;
		if ( ai_bOnlyMainMode ) {
			
			//Main mode; ignorar los que no tengan main

			f = brk->GetMainFile() ;
			if ( ! f ) continue ;

			if ( swEmpty ) {
				
				f->GetAbsolutePathFile(ao_Str) ;
				swEmpty=M4_FALSE ;
				
				continue ;
			} ;

			//Hayar mask con ao_Str

			f->GetAbsolutePathFile(&strAux) ;
			CollatePathMask(ao_Str, &strAux ) ;

		} else {

			//Data mode, procesar todos los ficheros de datos main

			for ( itData=brk->m_oMainDataList.begin() ; 
				itData!=brk->m_oMainDataList.end(); ++itData )
			{
				f = (*itData) ;

				if ( swEmpty ) {
					
					f->GetAbsolutePathFile(ao_Str) ;
					swEmpty=M4_FALSE ;
				
					continue ;
				} ;

				f->GetAbsolutePathFile(&strAux) ;
				CollatePathMask(ao_Str, &strAux ) ;
			} ;
		} ;
	} ;
}

void ClENG_BreakList::CollatePathMask(ClMIT_Str *ao_Mask, ClMIT_Str *ai_File) 
{
	m4char_t *pMask, *pFile ;
	m4char_t *pStart, *pEnd ;
	size_t lMask, lFile,  szBeg, szEnd, i ;

	lMask = ao_Mask->StrLen() ;
	lFile = ai_File->StrLen() ;
	
	//Busco len indicando cuantos caracteres son iguales desde el comienzo
	for ( 
		pMask=ao_Mask->InChar(0), pFile=ai_File->InChar(0), szBeg=0 ;  
		*pMask && *pFile ;
		++pMask, ++pFile )
	{
		if ( toupper(*pMask) != toupper(*pFile) ) {
			break ;
		} else {
			++szBeg ;
		} ;
	} ;

	if (szBeg==0) {
	
		//No coincide nada
		ao_Mask->StrCpy("") ;
	
	}  else if (szBeg==lMask==lFile){
		
		//Coincidencia completa
		;

	} else if (szBeg==lMask)  {
		
		//En Mask todos chars coinciden, pero lFile tiene +chars

		ao_Mask->StrCat("*") ;


	} else {

		//En Mask hay chars no coincidentes

		if ( *(ao_Mask->InChar(lMask-1))=='*' ) {
			
			//Si el final ya tiene *, solo podemos recortar la Mask poniendo *

			*(ao_Mask->InChar(szBeg-1))='*' ;
		
		} else {

			ClMIT_Str strAux ;

			//Concatenaremos la parte coincidente incial + '*' + parte coincidente final

			//Busco len indicando cuantos caracteres son iguales desde el final de ambas 
			//	cadenas	hasta la primera posicion no coincidente (seguro que hay alguna).
			for ( 
				pMask=ao_Mask->InChar(lMask-1), pFile=ai_File->InChar(lFile-1), szEnd=0, 
					i=M4_MIN(lMask,lFile) ;  
				i ;
				--pMask, --pFile, --i )
			{
				if ( toupper(*pMask) != toupper(*pFile) ) {
					break ;
				} else {
					++szEnd ;
				} ;
			} ;

			strAux.ToASCIZ( ao_Mask->InChar(0), szBeg ) ;
			if ( *(strAux.InChar(strAux.StrLen()-1)) != '*' ) strAux.StrCat("*") ;
			ao_Mask->StrCpy(strAux) ;
			if (szEnd) {
				// Interesa mantener los separadores de directorios intermedios
				// (esta casuística puede darse en salida con ruputra por directorio
				// con expresiones).
				for ( pStart = ai_File->InChar(szBeg),
					  pEnd = ai_File->InChar(ai_File->StrLen()-szEnd);
						pStart < pEnd; pStart ++ )
				{
					if (*pStart == '/' || *pStart == '\\')
					{
						ao_Mask->StrCatChar(*pStart) ;
						ao_Mask->StrCat("*") ;
					}
				}
				strAux.ToASCIZ( ai_File->InChar( ai_File->StrLen()-szEnd), szEnd ) ;
				ao_Mask->StrCat( strAux ) ;
			} ;
		} ;
	} ;

}


ClENG_BreakGroup *ClENG_BreakList::GetFirstNotDeleted() 
{
	ClENG_BreakGroup *R=NULL ;
	ClENG_BreakGroup *b ;
	BreakList_t::iterator it;

	for ( it=m_oBreakList.begin(); it!=m_oBreakList.end(); ++it ) {
		b = (*it).second ;
		if ( b->GetState()!=ClENG_BreakGroup::REMOVED ) {
			R=b ;
			break ;
		} ;
	} ;
	return R ;
}

m4bool_t ClENG_BreakList::RemoveAllBreaks() 
{
	m4bool_t SwOk=M4_TRUE ;
	BreakList_t::iterator it;
	ClENG_BreakGroup *brk ;

	for ( it=m_oBreakList.begin(); it!=m_oBreakList.end(); ++it ) {
		brk = (*it).second ;
		if ( ! brk->RemoveBreak() ) {
			SwOk=M4_FALSE ;
		} ;
	} ;
	return SwOk ;
}

m4int16_t ClENG_BreakList::GetOrdinalInFile(m4int16_t ai_iFlushPage) 
{
	m4int16_t brotherIdx, R, B ;
	
	for ( brotherIdx = m_oRedirectList[ai_iFlushPage], R=0, B=0; B<ai_iFlushPage ; ++B ) {
		if ( m_oRedirectList[B]==brotherIdx ) ++R ;
	} ;
	return R ;
}



//================================================================ClENG_BreakGroup

ClENG_BreakGroup::ClENG_BreakGroup()
{
	m_poInsp=GET_INSP();

	m_oPath=NULL ;
	m_poOwner=NULL ;
	m_poMain=NULL ;
	m_poPublishChn=NULL ;
	m_oPathMask.End() ;
}


void ClENG_BreakGroup::End() 
{	
	MainDataList_t::iterator itMainData ;
	
	m_oPath=NULL ;
	m_poOwner=NULL ;

	m_oMainDataList.clear() ;
	m_oGlobalToBreakList.clear() ;
	m_oGlobalDependencies.clear() ;
	m_poMain=NULL ;
	m_poPublishChn=NULL ;
}

m4bool_t ClENG_BreakGroup::Init( 
	ClENG_BreakList * ai_poOwner,
	m4uint32_t ai_uiBreakId,
	ClENG_Path *ai_oPath,
	ClMIT_Str *ai_oAlternativeMaskName,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return = M4_TRUE ;

	ClENG_BreakGroup::End();

	m_poOwner = ai_poOwner ;
	m_uiBreakId = ai_uiBreakId ;
	m_oPath = ai_oPath ;
	m_State = CURRENT_BREAK ;
	m_uiPublishGroupId=-1 ;
	m_poPublishChn=NULL ;
	m_bRT_DelBreakFlag = m_poOwner->GetOutputRepository()->DefaultDelBreak() ;
	if ( ! ComputeRelativeMask(ai_oAlternativeMaskName) ) Return=M4_FALSE ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;
} 

ClENG_PhysicalFile * ClENG_BreakGroup::CreateMainDataFile(const char *ai_pcFileName) 
{
	ClENG_PhysicalFile * fi=NULL ;

	//Creo nuevo fichero en el path asociado a este break

	fi = m_poOwner->GetOutputRepository()->m_oFileSet.CreateFile(m_oPath,ai_pcFileName,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!fi) goto exit ;

	//Inserto en lista

	m_oMainDataList.push_back( fi ) ;

exit:
	return fi ;
}

ClENG_PhysicalFile * ClENG_BreakGroup::NewGlobalReference(
		const m4char_t *ai_pcFileId) 
{
	ClENG_PhysicalFile *R=NULL ;
	GlobalToBreakList_t::iterator it ;
	ClMIT_Str path, key ;
	m4bool_t swExist ;

	key.StrCpy(ai_pcFileId) ;
	it = m_oGlobalToBreakList.find(key) ;
	if ( it != m_oGlobalToBreakList.end() ) {

		//Referencia existe, error

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163026,
			"ClENG_BreakGroup::NewGlobalReference() error, referencia %0 existe", ClMIT_Msg::MIXED_OUT) 
			<< ai_pcFileId << SEND_MSG ;
		R=NULL;
		goto exit ;

	} else {

		//Referencia NO existe, crear fichero y asociar


		if ( ! NewGlobalToBreakFile( &path, &swExist, ai_pcFileId) ) {
			R=NULL ;
			goto exit ;
		}
		
		R = GetBreakList()->GetOutputRepository()->m_oFileSet.CreateFile(
			m_oPath, path, ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!R) goto exit ;

		//Meter en lista para futuras referencias

		m_oGlobalToBreakList.insert( GlobalToBreakList_t::value_type(key,R) ) ;

	} ;

exit:
	return R ;
}

ClENG_PhysicalFile * ClENG_BreakGroup::GetGlobalReference(const m4char_t *ai_pcFileId) 
{
	GlobalToBreakList_t::iterator it ;
	ClMIT_Str key ;

	key.StrCpy(ai_pcFileId) ;
	it = m_oGlobalToBreakList.find(key) ;
	if ( it != m_oGlobalToBreakList.end() ) {

		return (*it).second ;

	} else {

		return NULL ;

	} ;

}

m4bool_t ClENG_BreakGroup::NewGlobalToBreakFile(
	ClMIT_Str *ao_Path, m4bool_t * ao_bExist, const m4char_t *ai_pcFileName) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str path, str ;
	ClENG_ReportOuputRepository *rep=m_poOwner->GetOutputRepository() ;

	//Monta name

	path.StrCpy( m_oPathMask ) ;
	path.StrCat( ai_pcFileName ) ;
	
	//------------->Seccion critica
	rep->m_oMutexPreserveMove.Lock() ;

	//Ya tenemos el file, ahora hay que comprobar modos preserve

	*ao_bExist = ClMIT_File::Exist(path)  ;

	switch ( rep->GetPreserveMode() )
	{
	case ClENG_ReportOuputRepository::PRESERVE_DIR:
	case ClENG_ReportOuputRepository::NO_PRESERVE:

		;

		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_FIX_NAME:

		if ( *ao_bExist  ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163025,
				"Fichero %0 existe, y se especifico PRESERVE", ClMIT_Msg::MIXED_OUT) 
				<<  path.InChar(0) << SEND_MSG ;
			SwOk=M4_FALSE ;
		} ;

		break ;
	
	case ClENG_ReportOuputRepository::PRESERVE_MOVE_NAME:

		/*
		if ( *ao_bExist ) {
			while (M4_TRUE) {
				aux.StrCpy(pathWithoutExtension) ;
				aux.StrCat("_");		
				ClMIT_Dates::DateToFileString( &aux2, ClMIT_Dates::ActualTimeStamp() ) ;
				aux.StrCat( aux2 ) ;
				aux.StrCat(".") ;
				aux.StrCat(ext) ;
				if ( ! ClMIT_File::Exist(aux) ) {
					*ao_bExist = M4_FALSE ;
					path.StrCpy(aux) ;
					break ;
				} ;
			} ;
		} ;
		*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163027,
			"PRESERVE_MOVE_NAME no permitido para fichero de break %0s", ClMIT_Msg::MIXED_OUT) 
			<<  path.InChar(0) << SEND_MSG ;
		SwOk=M4_FALSE ;

			
		break;
	} ;
	
	//<-------------
	rep->m_oMutexPreserveMove.Unlock() ;

	ClMIT_File::SplitPath( path, &str, ao_Path ) ;
exit:
	return SwOk ;
}

m4bool_t ClENG_BreakGroup::ComputeRelativeMask(ClMIT_Str *ai_oAlternativeMaskName)
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str path, aux ;
	ClENG_ReportOuputRepository *rep=m_poOwner->GetOutputRepository() ;

	//Monta rootPath\makName
	
	m_oPath->GetAbsolutePath(&path) ;
	ClMIT_File::ConvertToPath(&path) ;
	rep->GetMaskName(&aux) ;
	if ( ai_oAlternativeMaskName != NULL && aux.StrLen() == 0 )
	{
		// En el caso de grupo con expresiones, y si la máscara está vacía,
		// se debe propagar el resultado de la expresión al grupo
		ClMIT_File::ConcatToPath(&path,*ai_oAlternativeMaskName) ;
	}
	else
	{
		ClMIT_File::ConcatToPath(&path,aux) ;
	}

	//Si procede, añade el numero de break

	if ( ! rep->IsMonoBreak() && m_poOwner->m_BreakMode == ClENG_BreakList::BREAK_TO_FILE ) {
		SwOk = m_poOwner->GetNumOfSecCharsAddString(&aux)  ;
		if (!SwOk) goto exit ;
		path.StrCat(aux) ;
	} ;
	path.StrCat("_") ;

	m_oPathMask.StrCpy(path) ;

exit:
	return SwOk ;
}

void ClENG_BreakGroup::GetRelativeMaskFromGlobalToBreakToFlush(ClMIT_Str *ao_Str)  
{
	ClMIT_Str path ;

	//Siempre residen en el mismo dir, obtener la mask de nombre
	ClMIT_File::SplitPath( m_oPathMask, &path, ao_Str ) ;

}


m4bool_t ClENG_BreakGroup::DelThisBreak() 
{ 
	return m_bRT_DelBreakFlag && m_poOwner->GetOutputRepository()->IsUseDelBreak() ; 
}

m4bool_t ClENG_BreakGroup::HasSomethingPending() 
{
	MainDataList_t::iterator mainIt ;
	GlobalToBreakList_t::iterator glbIt ;
	ClENG_PhysicalFile *F ;
	ClENG_PI_Manager *PI_Manager ;

	PI_Manager = &m_poOwner->GetOutputRepository()->m_oPI_Manager ;

	if ( GetState()==CLOSED || GetState()==REMOVED ) return M4_FALSE ;

	//Test para main

	for (mainIt=m_oMainDataList.begin(); mainIt!=m_oMainDataList.end(); ++mainIt ) {
		F = (*mainIt) ;
		if ( F->IsOpen() ) return M4_TRUE ;
		if ( PI_Manager->HasPendingPIs(F) ) return M4_TRUE ;

	} ;

	//Test para globales

	for (glbIt=m_oGlobalToBreakList.begin(); glbIt!=m_oGlobalToBreakList.end(); ++glbIt ) {
		F = (*glbIt).second ;
		if ( F->IsOpen() ) return M4_TRUE ;
		if ( PI_Manager->HasPendingPIs(F) ) return M4_TRUE ;
	} ;

	return M4_FALSE ;
}

m4bool_t ClENG_BreakGroup::IsReadyToClose(m4bool_t *ai_SwOk) 
{
	m4bool_t swClose ;
	
	*ai_SwOk=M4_TRUE ;

	if ( GetState()==CLOSED || GetState()==REMOVED ) return M4_TRUE ;

	swClose = ! HasSomethingPending() ;

	if ( !swClose && DelThisBreak() ) {
		//FER
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163028,
			"Can not change a break to WAITING state if it has pending PIs or any file openned and it has to del the break", 
			ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		*ai_SwOk=M4_FALSE ;
		return M4_FALSE ;
	} else {
		return M4_TRUE ;
	} ;
}

m4bool_t ClENG_BreakGroup::CloseBreak(m4bool_t ai_bIsValidOuput) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_ReportOuputRepository *rep = m_poOwner->GetOutputRepository() ;
	MainDataList_t::iterator listIt ;
	MainDataList_t list ;

	ClENG_PhysicalFile *F ;
	ClMIT_Str strAux ;

	if ( !(GetState()==WAITING || GetState()==CURRENT_BREAK) ) {
		goto exit ;
	} ;

	//Cambia a closed

	m_State = CLOSED ;

	//Operaciones con breaks

	if ( ! DelThisBreak()  && ai_bIsValidOuput && ! m_poInsp->m_poTrace->IsCancelNotified() ) {

		//Procesos que solo se hacen si los ficheros son validos y no se van a borrar por break

		//**Impresion**
		if ( rep->m_oPrinter.StrLen()>0 ) {
		
			//Imprimir solo los de datos

			GetFileList(&list, M4_TRUE, M4_FALSE, M4_FALSE) ;

			for (listIt=list.begin(); listIt!=list.end(); ++listIt) {
				F = (*listIt) ;
				F->GetAbsolutePathFile(&strAux) ;
				if ( ! M4_SendFilePathToPrinter( rep->m_oPrinter, strAux ) ) {
					*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163000, "Error printing %0:s in %1:s", ClMIT_Msg::MIXED_OUT)
						<< strAux.InChar(0) <<  rep->m_oPrinter.InChar(0) << SEND_MSG ;
					SwOk=M4_FALSE ;
				} ; 			
			} ;

		};

		//**Publicacion**

		if ( m_uiPublishGroupId!=-1 ) {
			if ( ! PublishGroup() ) {
				SwOk=M4_FALSE ;
			} ;
		} ;


	} ;

	if ( ! DelThisBreak() && ! rep->m_bDeleteOnExit ) {

		//Procesos que solo se hacen si los ficheros no se van a borrar por break o report.
		//Da igual que la salida sea valida

		//**Declare interes**

		if ( rep->m_DeclareExternalMode != NOT_DECLARED ) {
			ClENG_MVC *mvc = GET_TASK_ENV()->m_APIChn.m_poMVC ;

			//Declarar interes sobre todos

			GetFileList(&list, M4_TRUE, M4_TRUE, M4_TRUE) ;

			for (listIt=list.begin(); listIt!=list.end(); ++listIt) {
				F = (*listIt) ;
				F->GetAbsolutePathFile(&strAux) ;
				if ( ! mvc->DeclareExternalFile(rep->m_DeclareExternalMode, strAux ) ) {
					SwOk=M4_FALSE ;
				} ;
			} ;

		} ;
	} ;

	//Borrado de break

	if ( DelThisBreak() ) {
		if ( ! RemoveBreak() ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

exit:
	return SwOk ;
}

m4bool_t ClENG_BreakGroup::RemoveBreak() 
{
	m4bool_t SwOk=M4_TRUE ;
	MainDataList_t::iterator listIt ;
	MainDataList_t list ;
	ClENG_PhysicalFile *F ;
	ClMIT_Str strAux ;

	#ifdef RPT_ENGINE_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_BreakGroup::RemoveBreak()", ClMIT_Msg::ENTRY_FORMAT ) 
			<< SEND_MSG ;
	#endif

	if ( GetState() != CLOSED ) {
		goto exit ;
	} ;

	//Primero borro todos los ficheros para evitar lios con directorios
	//NO borro los globales

	GetFileList(&list, M4_TRUE, M4_TRUE, M4_FALSE) ;

	for (listIt=list.begin(); listIt!=list.end(); ++listIt) {
		F = (*listIt) ;
		if ( ! F->Remove() ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

	//Y ahora, solo para modos BREAK_TO_DIR borra el directorio

	if ( m_poOwner->GetOutputRepository()->GetBreakMode()==ClENG_BreakList::BREAK_TO_DIR ) {
		if ( ! m_oPath->DeleteDirectory() ) {
			SwOk=M4_FALSE ;
		} ;
	} ;

	//Paso a estado de borrado y contabilizo en lista

	m_State = REMOVED ;

	m_poOwner->m_uiDeletedBreaks++ ;

exit:
	
	#ifdef RPT_ENGINE_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_BreakGroup::RemoveBreak()", ClMIT_Msg::EXIT_FORMAT ) 
			<< SEND_MSG ;
	#endif

	return SwOk ;
}

void ClENG_BreakGroup::PreparePublishGroup(
	ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId)
{
	ClMIT_Str rootPath ;

	if ( m_uiPublishGroupId!=-1 ) return ;

	m_poPublishChn=ai_poChn ;
	//El path para publicar siempre tiene que ser el raiz de report porque todo es relativo a este
	m_poOwner->GetOutputRepository()->m_oFileSet.GetRootPath(&rootPath) ;
	m_uiPublishGroupId = m_poPublishChn->NewPublishGroup( ai_pcReportId, rootPath ) ;

	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_BreakGroup::NewPublishGroup() ReportId %0:s RootPath %1:s PublishId %2:s" ) 
			<< ai_pcReportId << rootPath.InChar(0) << m_uiPublishGroupId << SEND_MSG ;

exit:
	return ;

}

void ClENG_BreakGroup::GetFileList(
	ClENG_BreakGroup::MainDataList_t *ai_List, 
	m4bool_t ai_bSwMainDataFiles, m4bool_t ai_bGloablToBreakFiles,
	m4bool_t ai_bGlobalFiles )
{
	MainDataList_t::iterator mainIt ;
	GlobalDependencies_t::iterator privateIt ;
	GlobalToBreakList_t::iterator glbIt ;
	ClENG_GlobalGroup::FileList_t::iterator allGlbIt ;
	ClENG_GlobalGroup *globalGroup ;

	ai_List->clear() ;

	//Datos main
	if (ai_bSwMainDataFiles) {
		for (mainIt=m_oMainDataList.begin(); mainIt!=m_oMainDataList.end(); ++mainIt ) {
			ai_List->push_back( (*mainIt) ) ;
		} ;
	} ;

	//Globales a break
	if (ai_bGloablToBreakFiles) {
		for (glbIt=m_oGlobalToBreakList.begin(); glbIt!=m_oGlobalToBreakList.end(); ++glbIt ) {
			ai_List->push_back( (*glbIt).second ) ;
		} ;
	} ;

	//Globales 

	if ( ai_bGlobalFiles ) {
 		
		//Globales privados de este break
		for (privateIt=m_oGlobalDependencies.begin(); privateIt!=m_oGlobalDependencies.end(); ++privateIt) {
			ai_List->push_back( (*privateIt).second ) ;
		} ;

		//Globales comunes a todos breaks
		globalGroup = &m_poOwner->GetOutputRepository()->m_oGlobalGroup ;
		for (allGlbIt=globalGroup->m_oFileList.begin(); allGlbIt!=globalGroup->m_oFileList.end(); ++allGlbIt) {
			if ( (*allGlbIt).second.m_bSwForAllBreaks ) {
				ai_List->push_back( (*allGlbIt).second.m_poFile ) ;
			} ;
		} ;
	} ;
}


m4bool_t ClENG_BreakGroup::PublishGroup()
{
	m4bool_t SwOk=M4_TRUE ;
	MainDataList_t::iterator listIt;
	ClENG_PhysicalFile *F ;
	ClMIT_Str strAux ;
	m4bool_t swRoot ;
	
	MainDataList_t list ;

	if ( m_uiPublishGroupId==-1 ) return M4_TRUE ;


	#ifdef RPT_ENGINE_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_BreakGroup::PublishGroup() for PublishId %0:s", 
			ClMIT_Msg::ENTRY_FORMAT ) 
			<< m_uiPublishGroupId << SEND_MSG ;
	#endif

	//*****NotifyFileToPublish

	GetFileList(&list, M4_TRUE, M4_TRUE, M4_TRUE ) ;

	//Publicacion

	for (listIt=list.begin(); listIt!=list.end(); ++listIt ) {
		F = (*listIt) ;
		F->GetAbsolutePathFile(&strAux) ;
		if (m_poMain) {
			swRoot = F ==m_poMain ;
		} else {
			if ( m_oMainDataList.size()==1 ) {
				swRoot= *(m_oMainDataList.begin()) == F ;
			} else {
				swRoot=M4_FALSE ;
			} ;
		} ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Notifyng file %0:s IsRoot %1:s" ) 
			<< strAux.InChar(0) << swRoot << SEND_MSG ;
		if ( ! m_poPublishChn->NotifyFileToPublish( m_uiPublishGroupId, strAux, swRoot ) ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163015, 
				"La llamada a la función de notificación de publicación del fichero %0:s, opción root a %1:s, ha fallado.", ClMIT_Msg::MIXED_OUT) 
				<< strAux.InChar(0) << swRoot << SEND_MSG ;
			SwOk=M4_FALSE ;
		} ;
	} ;

	//*****EndPublishGroup
	if ( ! m_poPublishChn->EndPublishGroup(m_uiPublishGroupId) ) {
		SwOk=M4_FALSE ;
	} ;

	//*****Actualizar count

	m_poOwner->m_uiPublishedBreaks++ ;

exit:

	#ifdef RPT_ENGINE_TRACE
	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_BreakGroup::PublishGroup() for PublishId %0s SwOk %1:s", 
			ClMIT_Msg::EXIT_FORMAT ) << m_uiPublishGroupId << SwOk << SEND_MSG ;
	#endif

	return SwOk ;
}

void ClENG_BreakGroup::ChangeToWaiting()
{
	if (m_State==CURRENT_BREAK) m_State=WAITING ;
}

//================================================================ClENG_ReportOuputRepository

ClMutex ClENG_ReportOuputRepository::m_oMutexPreserveMove(M4_TRUE) ;

ClENG_ReportOuputRepository::ClENG_ReportOuputRepository()
{
	m_poInsp=GET_INSP();
	m_State = NOT_INIT ;
	m_DeclareExternalMode=NOT_DECLARED;
	m_poMain=NULL ;
	m_bDeleteOnExit=M4_FALSE ;
	m_oFileExpressionObj=NULL ;
}


void ClENG_ReportOuputRepository::End() 
{	
	m_oFileSet.End() ;
	m_oGlobalGroup.End() ;
	m_oMaskName.End() ;
	if (m_oFileExpressionObj != NULL)
	{
		delete m_oFileExpressionObj;
		m_oFileExpressionObj = NULL;
	}
	m_oBreaks.End() ;
	m_State = NOT_INIT ;
	m_oPI_Manager.End() ;
	m_poMain=NULL ;
	m_oWebPath.End() ;
	m_oPrinter.End() ;

	if(m_DeclareExternalMode!=NOT_DECLARED){
		GET_TASK_ENV()->m_APIChn.m_poMVC->DestroyDeclareExtFileAccess();
		m_DeclareExternalMode=NOT_DECLARED;
	};
	
	if (m_bDeleteOnExit) {
		RemoveOuput() ;
	} ;
	
	m_bDeleteOnExit=M4_FALSE ;

}

m4bool_t ClENG_ReportOuputRepository::Init( 
		//General flow modifiers

		m4bool_t ai_MonoBreak,
		ClENG_BreakList::BREAK_MODE ai_BreakMode,
		ClENG_BreakList::DUPLEX_MODE ai_DuplexMode,
		
		//Root Path modifiers
		
		const m4char_t * ai_pcPathMask,
		m4bool_t ai_bIsFile,
		m4bool_t ai_bCreateDir,

		//File expression for break output
		const m4char_t * ai_pcFileExpression,

		// Acceso al canal de datos para evaluación de expresiones
		const ClMIT_Chn * ai_poChnData,
		
		//PRESERVE_DIR is a root path modifier, other modes are file name modifiers

		PRESERVE_MODE ai_PreserveMode,
		
		//Name file modifiers

		m4int16_t ai_iNumOfSecChars,
		m4int16_t ai_iNumOfPageChars,
		m4char_t  ai_cSeparator,
		m4char_t  ai_cBreakSeparator,
		const m4char_t *ai_pcExtension,

		//Break output management

		m4int16_t ai_iOffset,
		m4bool_t ai_bUseDelBreak,
		m4bool_t ai_bDefaultDelBreak,
		m4bool_t ai_bConsecutiveBreaks,
		const m4char_t *ai_pcToPrinter,

		//Flush management

		m4int16_t ai_PageLayoutCount,

		//Report output management

		m4bool_t ai_bDeleteOnExit,
		DECLARE_EXTERNAL_FILE ai_DeclareExternalMode,

		//Other specifics purposes

		const m4char_t *ai_pcWebPath, 

		// UNICODE REPORTS
		m4bool_t ai_bUTF16_BOM,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str rootPath;
	ClMIT_Str oActivePathMask;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::Init() "
			
			"MonoBreak %0:s BreakMode %1:s ai_DuplexMode %2:s PathMask %3:s "
			"IsFile %4:s CreateDir %5:s PreserveMode %6:s NumOfSecChars %7:s "
			"NumOfPageChars %8:s ai_cSeparator %9:s Extension  %10:s Offset %11:s "
			"UseDelBreak %12:s DefaultDelBreak %13:s ConsecutiveBreaks %14:s "
			"ToPrinter %15:s PageLayoutCount %16:s DeleteOnExit %17:s "
			"DeclareExternalFile %18:s  cBreakSeparator %19:s "
			"FileExpression %20:s", 

			ClMIT_Msg::ENTRY_FORMAT) 
				
			<< ai_MonoBreak << (m4int16_t)ai_BreakMode 	<< (m4int16_t)ai_DuplexMode << ai_pcPathMask
			<< ai_bIsFile << ai_bCreateDir << (m4int16_t)ai_PreserveMode << ai_iNumOfSecChars
			<< ai_iNumOfPageChars << ai_cSeparator << ai_pcExtension  << ai_iOffset
			<< ai_bUseDelBreak << ai_bDefaultDelBreak << ai_bConsecutiveBreaks
			<< ai_pcToPrinter << ai_PageLayoutCount << ai_bDeleteOnExit 
			<< (m4int16_t)ai_DeclareExternalMode
			<< ai_cBreakSeparator
			<< ai_pcFileExpression
			
			<<  SEND_MSG   ;
	};
	#endif

	ClENG_ReportOuputRepository::End();

	//Almaceno propiedades 

	m_MonoBreak = ai_MonoBreak ;
	m_PreserveMode = ai_PreserveMode ;
	m_DeclareExternalMode = ai_DeclareExternalMode ;
	m_oWebPath.StrCpy(ai_pcWebPath) ;
	m_oWebPath.Trim() ;
	m_oPrinter.StrCpy(ai_pcToPrinter) ;
	m_oPrinter.Trim() ;
	m_bDeleteOnExit = ai_bDeleteOnExit ;

	if ( ai_poChnData != NULL && ai_pcFileExpression != NULL && strlen(ai_pcFileExpression) > 0 )
	{
		m_oFileExpressionObj = new ClENG_Expression();

		ClENG_Analex::StExpressionEnv StEnv ;
		ClENG_Analex Analex ;

		// se inicializa el entorno
		StEnv.m_poChn= (ClMIT_Chn*)ai_poChnData ;
		m_oFileExpressionObj->Init(ai_pcFileExpression,"File expression") ;
		if ( ! Analex.Init(&StEnv, m_oFileExpressionObj, ClMIT_Trace::PROPAGATE_ERROR) )
		{
			Return = M4_FALSE ;
			goto exit ;
		}
	}

	//Obtiene rootPath y maskName, inicializa m_FileSet

	oActivePathMask.StrCpy(ai_pcPathMask);
	if ( m_oFileExpressionObj != NULL )
	{
		// Si hay expresión con ruptura por bloque se considera siempre como directorio
		oActivePathMask.StrCatChar('\\');

		ai_bCreateDir = M4_TRUE; // En este modo se asegura siempre la creación de directorios
	}

	ClMIT_File::ConvertToThisPlatform(&oActivePathMask);

	if ( !  GetRootPath( &rootPath, &m_oMaskName, oActivePathMask, ai_bIsFile, 
		ai_bCreateDir, m_PreserveMode )  )
	{
		Return = M4_FALSE ;
		goto exit ;
	}
	if ( ! m_oFileSet.Init(rootPath,M4_FALSE,ClMIT_Trace::PROPAGATE_ERROR) ) {
		Return = M4_FALSE ;
		goto exit ;
	} ;

	//Inicializa el GlobalGroup, siempre tiene un nombre fijo y cuelga del root.

	if ( ! m_oGlobalGroup.Init(this, "images", ClMIT_Trace::PROPAGATE_ERROR) ) {
		Return = M4_FALSE ;
		goto exit ;
	} ;

	//Inicializa la BreakList

	if ( ! m_oBreaks.Init(
		this,
		ai_BreakMode, ai_DuplexMode,
		ai_iNumOfSecChars, ai_iNumOfPageChars, ai_cSeparator, ai_cBreakSeparator, 
		ai_pcExtension,
		ai_iOffset, ai_bUseDelBreak, ai_bDefaultDelBreak, ai_bConsecutiveBreaks,
		ai_PageLayoutCount, 
		ai_bUTF16_BOM,
		ClMIT_Trace::PROPAGATE_ERROR) ) 
	{
		Return = M4_FALSE ;
		goto exit ;
	} ;

	//Init del manager de PIs

	m_oPI_Manager.Init() ;

	//Cambia de estado

	m_State = OUT_OF_BREAK ;

	//Init canal declaraciones
	if(m_DeclareExternalMode!=NOT_DECLARED){
		GET_TASK_ENV()->m_APIChn.m_poMVC->StartUpDeclareExtFile();
	};

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::Init()"
			"return %0:s", ClMIT_Msg::EXIT_FORMAT) 

			<< Return <<  SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}


m4bool_t ClENG_ReportOuputRepository::GetRootPath( ClMIT_Str *ao_Path, ClMIT_Str *ao_NameMask,
		const m4char_t * ai_pcPathMask, m4bool_t ai_bIsFile, m4bool_t ai_bCreateDir,
		PRESERVE_MODE ai_PreserveMode )  
{
	m4bool_t SwOk=M4_TRUE ;
	m4bool_t SwExist ;
	ClMIT_Str sAux, sAux2 ;
	StMIT_Inspector* poInsp = GET_INSP() ;	

	m_oMutexPreserveMove.Lock() ;
	//------->Bloque sincronizado, juego con paths unicos

	ClMIT_File::SplitPath(ai_pcPathMask, ao_Path, ao_NameMask);
	if (ai_bIsFile) {
		poInsp->m_poSysInfo->GetValue( ENG_RK_WORKDIR, &sAux ) ;
		ClMIT_SysInfo::AppendToPath(&sAux, ao_Path->InChar(0) ) ;
		ao_Path->StrCpy(sAux) ;
	} ;
	SwExist = ClMIT_File::Exist(ao_Path->InChar(0)) ; 

	if (ai_PreserveMode==PRESERVE_DIR) {

		//Modo PRESERVER_DIR, Si dir existe, hayar otro con TS y crearlo. Si no existe, crearlo.

		if( ! ai_bCreateDir ) {
			*poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163009, "Parameters PRESERVE_DIR and NCREATE_DIR are not compatible", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
			SwOk=M4_FALSE;
			goto exit ;
		} ;
		if ( SwExist ) {
			while (M4_TRUE) {
				sAux.StrCpy(ao_Path) ;
				ClMIT_File::ExpandPathName(&sAux,"_") ;
				ClMIT_Dates::DateToFileString( &sAux2, ClMIT_Dates::ActualTimeStamp() ) ;
				ClMIT_File::ExpandPathName( &sAux, sAux2 ) ;
				if ( ! ClMIT_File::Exist(sAux) ) {
					ao_Path->StrCpy(sAux) ;
					break ;
				} ;
			
			} ;
 		} ;

		if ( ClMIT_File::CreatePath( ao_Path->InChar(0), ClMIT_File::TRY_CREATE ) != ClMIT_File::PATH_CREATED ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

	} else  {

		//Modo normal, si dir existe se usa, si no, se crea si ai_bCreateDir o da error

		if ( SwExist ) {
			; //Ok
		} else {
			if (ai_bCreateDir) {
				if ( ClMIT_File::CreatePath( ao_Path->InChar(0), ClMIT_File::TRY_CREATE ) != 
					ClMIT_File::PATH_CREATED ) 
				{
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
			} else { ;
				*poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163029,
					"Path %0 no existe, y se especifico NCREATEDIR", ClMIT_Msg::MIXED_OUT) 
					<<  ao_Path->InChar(0) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} ;

	} ;

exit:
	//<-------
	m_oMutexPreserveMove.Unlock() ;

	return SwOk ;
}

ClENG_ReportOuputRepository::EXPRESSION_RESULT ClENG_ReportOuputRepository::ResolveExpressionFile(ClMIT_Str *ao_ExpressionResult)
{
	m4char_t *Pos, *Beg ;
	ClMIT_ChnValue res ;

	if ( m_oFileExpressionObj != NULL )
	{
		// Se evlua la expresión
		if ( !m_oFileExpressionObj->Evaluate (&res, ClMIT_Trace::PROPAGATE_ERROR) )
		{
			return ClENG_ReportOuputRepository::EXPRESSION_ERROR ;
		}
		ao_ExpressionResult->StrCpy(res.GetString()) ;
		// Se revisa el resultado para asegurar que es un fichero válido
		if ( ao_ExpressionResult->StrLen() == 0 )
		{
			ao_ExpressionResult->StrCpy("_"); // Contenido mínimo
		}
		else
		{
			// Se sustituyen caracteres no válidos
			for (Pos=Beg=ao_ExpressionResult->InChar(0); *Pos!=M4_END_STR; Pos++)
			{
				if ( strchr("\\/:*?\"<>|", *Pos) ) *Pos = '_' ;
			}
		}

		return ClENG_ReportOuputRepository::EXPRESSION_OK;
	}
	else
	{
		return ClENG_ReportOuputRepository::NO_EXPRESSION ;
	}
}


m4bool_t ClENG_ReportOuputRepository::NewBreak(m4bool_t *ao_bNewFlushFilesOpenned)
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::NewBreak()",
			ClMIT_Msg::ENTRY_FORMAT) <<  SEND_MSG   ;
	};
	#endif

	if ( m_State != OUT_OF_BREAK ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::NewBreak not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;

	} ;

	if ( ! m_oBreaks.NewBreak(ao_bNewFlushFilesOpenned) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
	m_State = OUT_OF_FLUSH ;

exit:
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::NewBreak()"
			"return %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk <<  SEND_MSG   ;
	};
	#endif

	return SwOk ;
} ;

m4bool_t ClENG_ReportOuputRepository::CloseBreak(m4bool_t ai_bIsValidOuput)
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::CloseBreak()",
			ClMIT_Msg::ENTRY_FORMAT) <<  SEND_MSG   ;
	};
	#endif

	if ( m_State != OUT_OF_FLUSH ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::CloseBreak not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! m_oBreaks.CloseBreak(ai_bIsValidOuput) ) {
		SwOk=M4_FALSE ;
		//Dejar seguir para que cambie el estatus
	} ;

	
	m_State = OUT_OF_BREAK ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::CloseBreak()"
			"return %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk <<  SEND_MSG   ;
	};
	#endif

	return SwOk ;
} ;


m4bool_t ClENG_ReportOuputRepository::NewFlush(m4bool_t *ao_bNewFlushFilesOpenned)
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::NewFlush()",
			ClMIT_Msg::ENTRY_FORMAT) <<  SEND_MSG   ;
	};
	#endif

	if ( m_State != OUT_OF_FLUSH ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::NewFlush not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;

	} ;

	if ( ! m_oBreaks.NewFlush(ao_bNewFlushFilesOpenned) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_State = PROCESSING_FLUSH ;

exit:
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::NewFlush()"
			"return %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk <<  SEND_MSG   ;
	};
	#endif

	return SwOk ;
} ;

m4bool_t ClENG_ReportOuputRepository::CloseFlush(M4ClBuffer **ai_poBuffArray)
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::CloseFlush()",
			ClMIT_Msg::ENTRY_FORMAT) <<  SEND_MSG   ;
	};
	#endif

	if ( m_State != PROCESSING_FLUSH ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::CloseFlush not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! m_oBreaks.CloseFlush(ai_poBuffArray) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
	m_State = OUT_OF_FLUSH ;

exit:
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportOuputRepository::CloseFlush()"
			"return %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk <<  SEND_MSG   ;
	};
	#endif

	return SwOk ;
} ;


ClENG_PhysicalFile * ClENG_ReportOuputRepository::GetFlushFile( 
	m4int16_t ai_iFlushPage, m4bool_t *ao_bRedirected ) 
{
	ClENG_PhysicalFile * R=NULL ;

	R = m_oBreaks.GetFlushFile(ai_iFlushPage) ;
	if ( ! R  ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::GetFlushFile not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=NULL ;
		goto exit ;
	} ;

	if (ao_bRedirected) *ao_bRedirected=m_oBreaks.IsRedirected(ai_iFlushPage) ;

exit:
	return R ;
}

ClENG_PhysicalFile *  ClENG_ReportOuputRepository::GetGlobalReference(
	const m4char_t *ai_pcFileNameId, m4bool_t ai_bSwForAllBreaks, m4bool_t *ao_SwCreated) 
{
	ClENG_PhysicalFile * R=NULL;

	if ( m_State == NOT_INIT ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::GetGlobalReference not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=NULL ;
		goto exit ;
	} ;

	R = m_oGlobalGroup.GetGlobalReference(ai_pcFileNameId, ai_bSwForAllBreaks, ao_SwCreated) ;
	if (!R) goto exit ;

exit:
	return R ;
}

ClENG_PhysicalFile *  ClENG_ReportOuputRepository::NewGlobalToBreakReference( 
		const m4char_t *ai_pcFileId) 
{
	ClENG_PhysicalFile * R=NULL ;

	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::NewGlobalToBreakReference not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=NULL ;
		goto exit ;
	} ;

	R = m_oBreaks.m_poCurrentBreak->NewGlobalReference(ai_pcFileId) ;
	if (!R) goto exit ;

exit:
	return R ;
}

ClENG_PhysicalFile * ClENG_ReportOuputRepository::GetGlobalToBreakReference(
	const m4char_t *ai_pcFileId) 
{
	ClENG_PhysicalFile *R=NULL ;

	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::GetGlobalToBreakReference not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=NULL ;
		goto exit ;
	} ;

	R = m_oBreaks.m_poCurrentBreak->GetGlobalReference(ai_pcFileId) ;
	if (!R) goto exit ;

exit:
	return R ;
}

void  ClENG_ReportOuputRepository::SetAsMainFileForCurrentBreak(ClENG_PhysicalFile *ai_File) 
{
	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::SetAsMainFileForCurrentBreak not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
	} else {
		m_oBreaks.m_poCurrentBreak->SetAsMainFile(ai_File) ;
	} ;

}

m4bool_t ClENG_ReportOuputRepository::PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) 
{
	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::PreparePublishGroup not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		return M4_FALSE ;
	} else {
		m_oBreaks.m_poCurrentBreak->PreparePublishGroup(ai_poChn,ai_pcReportId) ;
		return M4_TRUE ;
	} ;
}

m4bool_t ClENG_ReportOuputRepository::IsDelBreak() 
{
	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::IsDelBreak not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		return M4_FALSE ;
	} else {
		return m_oBreaks.m_poCurrentBreak->IsDelBreak() ;
	} ;
}

void ClENG_ReportOuputRepository::SetDelBreak(m4bool_t ai_bDelBreak) 
{
	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::SetDelBreak not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
	} else {
		m_oBreaks.m_poCurrentBreak->SetDelBreak(ai_bDelBreak) ;
	} ;
}


StPi_t *  ClENG_ReportOuputRepository::AddNewPi( 
	const char *ai_pcIdPI, m4int16_t ai_iPage, m4uint32_t ai_Offset, size_t ai_NBytes) 

{ 
	return m_oPI_Manager.AddNewPi( ai_pcIdPI, ai_iPage, ai_Offset, ai_NBytes) ; 
} 

m4bool_t ClENG_ReportOuputRepository::ExtractPi( const char *ai_pcIdPI, StPi_t *ao_poData ) 
{
	return m_oPI_Manager.ExtractPi( ai_pcIdPI, ao_poData ) ;
}

StPi_t * ClENG_ReportOuputRepository::FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound) 
{
	return m_oPI_Manager.FindPi( ai_pcIdPI, ai_bSwErrorIfNotFound) ;
}

void ClENG_ReportOuputRepository::GetPiIterators( 
	ClENG_PI_Manager::PI_List_t::iterator &ao_Beg, 
	ClENG_PI_Manager::PI_List_t::iterator &ao_End )  
{ 
	m_oPI_Manager.GetIterators( ao_Beg, ao_End ) ;
} 


const char * ClENG_ReportOuputRepository::GetTraceString(STATE ai_State) 
{
	switch(ai_State)
	{
	case NOT_INIT:
		return "NOT_INIT" ; 
		break ;
	case OUT_OF_BREAK:
		return "OUT_OF_BREAK" ;
		break ;
	case OUT_OF_FLUSH:
		return "OUT_OF_FLUSH" ;
		break ;
	case PROCESSING_FLUSH:
		return "PROCESSING_FLUSH" ;
		break ;
	default:
		return "UNKNOW STATE" ;
		break ;
	} ;
}

void ClENG_ReportOuputRepository::GetNavigationPathMask(ClMIT_Str *ao_Str) 
{
	ClENG_BreakList::BreakList_t::iterator it ;
	ClENG_BreakGroup *breakG ;
	m4uint32_t breakCount =  m_oBreaks.GetBreakCount();
	
	if ( m_poMain ) {

		//Hay main de report

		m_poMain->GetAbsolutePathFile(ao_Str) ;

	} else if ( breakCount == 0 ) {

		//Ni hay main de report ni breaks

		ao_Str->StrCpy("") ;
	
	} else {

		//Hay alguno seguro porque si no GetBreakCount seria 0
		breakG = m_oBreaks.GetFirstNotDeleted() ;

		if ( breakG->GetMainFile() ) {
		
			//El primer break tiene main
	
			if ( breakCount==1 ) {

				//Ademas solo existe ese break

				breakG->GetMainFile()->GetAbsolutePathFile(ao_Str) ;
				
			} else {

				//Existen mas breaks, hayar mascara de ficheros main

				m_oBreaks.GetNavigationPathMask( ao_Str, M4_TRUE ) ;

			} ;

		} else {

			//El primer break no tiene main, hayar mascara de ficheros data files

			m_oBreaks.GetNavigationPathMask( ao_Str, M4_FALSE ) ;
		} ;
		
	} ;

	//Proceso WebPath si hay

	if (m_oWebPath.StrLen()>0) {

		ClMIT_Str webPath ;

		webPath.StrCpy(m_oWebPath) ;
		ClMIT_File::ConvertToPath(&webPath) ;

		if ( ClMIT_Str::StrIStr(ao_Str->InChar(0), webPath.InChar(0) ) == ao_Str->InChar(0) ) {
			ao_Str->StrDeleteSegment( 0, webPath.StrLen()-1 ) ;	//-1 para que empiece por /
			ClMIT_File::ConvertToWebPath(ao_Str) ;
		} ;
	} ;

}

void ClENG_ReportOuputRepository::GetNavigationRootPathMask(ClMIT_Str *ao_Str) 
{

	m_oFileSet.GetRootPath(ao_Str) ;
	ClMIT_File::ConvertToPath(ao_Str) ;

	if (m_oWebPath.StrLen()>0) {

		ClMIT_Str webPath ;

		webPath.StrCpy(m_oWebPath) ;
		ClMIT_File::ConvertToPath(&webPath) ;

		if ( ClMIT_Str::StrIStr(ao_Str->InChar(0), webPath.InChar(0) ) == ao_Str->InChar(0) ) {
			ao_Str->StrDeleteSegment( 0, webPath.StrLen()-1 ) ; //-1 para que empiece por /
			ClMIT_File::ConvertToWebPath(ao_Str) ;
		} ;
	} ;
}

m4bool_t ClENG_ReportOuputRepository::GetRelativeMaskFromGlobalToBreakToFlush( ClMIT_Str *ao_poStr ) 
{
	m4bool_t R=M4_TRUE ;

	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::GetRelativeMaskFromGlobalToBreakToFlush not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=M4_FALSE ;
		ao_poStr->StrCpy("") ;
		goto exit ;
	} ;

	m_oBreaks.m_poCurrentBreak->GetRelativeMaskFromGlobalToBreakToFlush(ao_poStr) ;

exit:
	return R ;
}

m4bool_t  ClENG_ReportOuputRepository::GetMainDataIt( 
		ClENG_BreakGroup::MainDataList_t::iterator &ao_ItBeg, 
		ClENG_BreakGroup::MainDataList_t::iterator &ao_ItEnd ) 
{
	m4bool_t R=M4_TRUE ;

	if ( !( m_State==OUT_OF_FLUSH || m_State==PROCESSING_FLUSH) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_ReportOuputRepository::GetMainDataIt not allowed from status %0:s", ClMIT_Msg::MIXED_OUT) 
			<<  GetTraceString(m_State) << SEND_MSG ;
		R=M4_FALSE ;
		goto exit ;
	} ;

	m_oBreaks.m_poCurrentBreak->GetMainDataIt( ao_ItBeg, ao_ItEnd ) ;

exit:
	return R ;
}



void ClENG_ReportOuputRepository::GetRelativePathFromFlushToGlobal( ClMIT_Str *ao_poStr ) 
{
	ClMIT_Str str ;

	if (m_oBreaks.GetBreakMode()==ClENG_BreakList::BREAK_TO_DIR) {
		ao_poStr->StrCpy("..\\") ;
		str.StrCpy( m_oGlobalGroup.GetRelativePath() ) ;
		ao_poStr->StrCat( str ) ;
		ClMIT_File::ConvertToPath(ao_poStr) ;
	} else {
		ao_poStr->StrCpy( m_oGlobalGroup.GetRelativePath() ) ;
		ClMIT_File::ConvertToPath(ao_poStr) ;
	} ;
} ;


void ClENG_ReportOuputRepository::GetRelativePathFromFlushToGlobalToBreak( ClMIT_Str *ao_poStr ) 
{
	ao_poStr->StrCpy("") ; //Siempre están en el mismo directorio
}

m4bool_t ClENG_ReportOuputRepository::RemoveOuput() 
{
	m4bool_t SwOk=M4_TRUE ;

	//Borro los breaks ..

	if (! m_oBreaks.RemoveAllBreaks() ) {
		SwOk=M4_FALSE ;
	} ;

	//Y ahora borro los globales ...
	if ( ! m_oGlobalGroup.Remove() ) {
		SwOk=M4_FALSE ;
	} ;

	return SwOk ;

}

