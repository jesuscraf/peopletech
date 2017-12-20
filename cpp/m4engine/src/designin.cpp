
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designin.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/21/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"

#include "file_manager.hpp"

#include "designin.h"
#include "clstr.h"

//=====================================================================ClENG_FileInterestList

ClENG_FileInterestList::ClENG_FileInterestList() 
{
	m_poInsp=GET_INSP();
	m_poFileManager=ClFileManager::Instance() ;
}

void ClENG_FileInterestList::End() 
{
	UnsetInterest() ;
}


void ClENG_FileInterestList::Init( ) 
{
	ClENG_FileInterestList::End();
}

void ClENG_FileInterestList::Init( ClENG_FileInterestList *ai_poList ) 
{
	FileList_t::iterator It ;

	ClENG_FileInterestList::End();

	for ( It=ai_poList->m_oFileList.begin(); It!=ai_poList->m_oFileList.end(); ++It ) {
		DeclareInterest( (*It) ) ;
	} ;
}

void ClENG_FileInterestList::DeclareInterest( m4char_t *ai_pcFile ) 
{
	ClMIT_Str Str ;

	Str.StrCpy(ai_pcFile) ;
	m_poFileManager->SetInterestInFile( Str.InChar(0) ) ;
	m_oFileList.push_back( Str ) ;
}

void ClENG_FileInterestList::UnsetInterest() 
{
	//Ojo, se llma desde End

	FileList_t::iterator It ;

	for ( It=m_oFileList.begin(); It!=m_oFileList.end(); ++It ) {
		m_poFileManager->UnSetInterestInFile( (*It).InChar(0) ) ;
	} ;
	m_oFileList.clear() ;
}

//=====================================================================>StFileParams


StFileParams::~StFileParams() 
{
	m_oPath.End() ;
	m_oExtension.End() ;
	m_oWebPath.End();
	m_oHTMLMenuFile.End();
	m_oFileNameExpression.End();
	m_poChnData = NULL;
}

void StFileParams::Init(const StFileParams * ai_From ) 
{
	m_oPath.StrCpy(			&ai_From->m_oPath) ;
	m_oWebPath.StrCpy(		&ai_From->m_oWebPath) ;
	m_oHTMLMenuFile.StrCpy( &ai_From->m_oHTMLMenuFile) ;
	m_iNumOfSecChars =		ai_From->m_iNumOfSecChars ;
	m_iNumOfPageChars =		ai_From->m_iNumOfPageChars ;
	m_cSeparator =			ai_From->m_cSeparator ;
	m_oExtension.StrCpy(	&ai_From->m_oExtension) ;
	m_iOffSet =				ai_From->m_iOffSet ;
	m_bIsFile =				ai_From->m_bIsFile ;
	m_bUseDelBreak =		ai_From->m_bUseDelBreak ;
	m_bDefaultDelBreak =	ai_From->m_bDefaultDelBreak ;
	m_bConsecutiveBreaks =	ai_From->m_bConsecutiveBreaks ;

	m_bPathLoaded =				ai_From->m_bPathLoaded ;
	m_bWebPathLoaded =			ai_From->m_bWebPathLoaded ;
	m_bHTMLMenuFileLoaded =		ai_From->m_bHTMLMenuFileLoaded ;
	m_bNumOfSecCharsLoaded =	ai_From->m_bNumOfSecCharsLoaded ;
	m_bNumOfPageCharsLoaded =	ai_From->m_bNumOfPageCharsLoaded ;
	m_bSeparatorLoaded =		ai_From->m_bSeparatorLoaded ;
	m_bExtensionLoaded =		ai_From->m_bExtensionLoaded ;
	m_bOffSetLoaded =			ai_From->m_bOffSetLoaded ;
	m_bIsFileLoaded =			ai_From->m_bIsFileLoaded ;
	m_bIgnoreFilePathPref =		ai_From->m_bIgnoreFilePathPref ;
	m_bUseDelBreakLoaded =		ai_From->m_bUseDelBreakLoaded ;
	m_bDefaultDelBreakLoaded =  ai_From->m_bDefaultDelBreakLoaded ;
	m_bConsecutiveBreaksLoaded = ai_From->m_bConsecutiveBreaksLoaded ;

	m_oFileNameExpression.StrCpy( &ai_From->m_oFileNameExpression) ;
	m_poChnData = ai_From->m_poChnData ;
}

void StFileParams::Init() {

	m_oPath.StrCpy("") ;
	m_oWebPath.StrCpy("");
	m_oHTMLMenuFile.StrCpy("");
	m_iNumOfSecChars = 0 ;
	m_iNumOfPageChars = -1 ;
	m_cSeparator  = 0 ;		
	m_oExtension.StrCpy("") ;
	m_iOffSet  = 0 ;
	m_bIsFile = M4_TRUE;
	m_bIgnoreFilePathPref = M4_FALSE;
	m_bUseDelBreak=M4_FALSE ;
	m_bDefaultDelBreak=M4_TRUE ;
	m_bConsecutiveBreaks=M4_TRUE ;

	m_bPathLoaded = m_bNumOfSecCharsLoaded = m_bNumOfPageCharsLoaded =
		m_bSeparatorLoaded = m_bExtensionLoaded = m_bOffSetLoaded = m_bIsFileLoaded = m_bWebPathLoaded = m_bHTMLMenuFileLoaded =
		m_bUseDelBreakLoaded = m_bDefaultDelBreakLoaded = m_bConsecutiveBreaksLoaded = M4_FALSE ;

	m_oFileNameExpression.StrCpy("") ;
	m_poChnData = NULL;

} 


void StFileParams::Init( const StFileParams * ai_A, const StFileParams * ai_B, 
										   m4int16_t ai_DefNumOfSecChars) 
{
	Init() ; //Carga valores defecto, flags de Loaded a false.

	if ( ai_A->m_bPathLoaded && !ai_B->m_bIgnoreFilePathPref ) {  
		m_oPath.StrCpy(ai_A->m_oPath.InChar(0)) ; m_bPathLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bPathLoaded ) {  
		m_oPath.StrCpy(ai_B->m_oPath.InChar(0)) ; m_bPathLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bWebPathLoaded && !ai_B->m_bIgnoreFilePathPref ) {  
		m_oWebPath.StrCpy(ai_A->m_oWebPath.InChar(0)) ; m_bWebPathLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bWebPathLoaded ) {  
		m_oWebPath.StrCpy(ai_B->m_oWebPath.InChar(0)) ; m_bWebPathLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bHTMLMenuFileLoaded ) {  
		m_oHTMLMenuFile.StrCpy(ai_A->m_oHTMLMenuFile.InChar(0)) ; m_bHTMLMenuFileLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bHTMLMenuFileLoaded ) {  
		m_oHTMLMenuFile.StrCpy(ai_B->m_oHTMLMenuFile.InChar(0)) ; m_bHTMLMenuFileLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bNumOfSecCharsLoaded ) {  
		m_iNumOfSecChars = ai_A->m_iNumOfSecChars ; m_bNumOfSecCharsLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bNumOfSecCharsLoaded ) {  
		m_iNumOfSecChars = ai_B->m_iNumOfSecChars ; m_bNumOfSecCharsLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bNumOfPageCharsLoaded ) {  
		m_iNumOfPageChars = ai_A->m_iNumOfPageChars ; m_bNumOfPageCharsLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bNumOfPageCharsLoaded ) {  
		m_iNumOfPageChars = ai_B->m_iNumOfPageChars ; m_bNumOfPageCharsLoaded=M4_TRUE ; 
	} ;
	
	if ( ai_A->m_bSeparatorLoaded ) {  
		m_cSeparator = ai_A->m_cSeparator ; m_bSeparatorLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bSeparatorLoaded ) {  
		m_cSeparator = ai_B->m_cSeparator ; m_bSeparatorLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bExtensionLoaded ) {  
		m_oExtension.StrCpy(ai_A->m_oExtension.InChar(0)) ; m_bExtensionLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bExtensionLoaded ) {  
		m_oExtension.StrCpy(ai_B->m_oExtension.InChar(0)) ; m_bExtensionLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bOffSetLoaded ) {  
		m_iOffSet = ai_A->m_iOffSet ; m_bOffSetLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bOffSetLoaded ) {  
		m_iOffSet = ai_B->m_iOffSet ; m_bOffSetLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bIsFileLoaded && !ai_B->m_bIgnoreFilePathPref ) {  
		m_bIsFile = ai_A->m_bIsFile ; m_bIsFileLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bIsFileLoaded ) {  
		m_bIsFile = ai_B->m_bIsFile ; m_bIsFileLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bUseDelBreakLoaded ) {  
		m_bUseDelBreak = ai_A->m_bUseDelBreak ; m_bUseDelBreakLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bUseDelBreakLoaded ) {  
		m_bUseDelBreak = ai_B->m_bUseDelBreak ; m_bUseDelBreakLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bDefaultDelBreakLoaded ) {  
		m_bDefaultDelBreak = ai_A->m_bDefaultDelBreak ; m_bDefaultDelBreakLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bDefaultDelBreakLoaded ) {  
		m_bDefaultDelBreak = ai_B->m_bDefaultDelBreak ; m_bDefaultDelBreakLoaded=M4_TRUE ; 
	} ;

	if ( ai_A->m_bConsecutiveBreaksLoaded ) {  
		m_bConsecutiveBreaks = ai_A->m_bConsecutiveBreaks ; m_bConsecutiveBreaksLoaded=M4_TRUE ; 
	} else if ( ai_B->m_bConsecutiveBreaksLoaded ) {  
		m_bConsecutiveBreaks = ai_B->m_bConsecutiveBreaks ; m_bConsecutiveBreaksLoaded=M4_TRUE ; 
	} ;

	// Defecto m_iNumOfSecChars
	if ( ! m_bNumOfSecCharsLoaded )  {
		m_iNumOfSecChars = ai_DefNumOfSecChars ;  
	} ;

}
 