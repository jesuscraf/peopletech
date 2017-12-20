//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             physfrp.cpp
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
#include "physfrp.h"
#include "m4unicode.hpp"

#define GROUP_ID    ENG_TGI_CLDVCFI


//=========================================================================ClENG_PhysicalFile

ClENG_PhysicalFile::ClENG_PhysicalFile()
{
	m_poInsp=GET_INSP();
	m_FatherPath=NULL;
}


void ClENG_PhysicalFile::End() 
{	
	
	m_FatherPath=NULL;
	m_FileName.End() ;
	m_F.Close() ;

	//m_oPIList.clear() ;
}

m4bool_t ClENG_PhysicalFile::Init( 
		ClENG_Path *ai_FatherPath, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str faux ;

	ClENG_PhysicalFile::End();

	m_FatherPath = ai_FatherPath ;
	m_FileName.StrCpy(ai_FileName) ;

	GetAbsolutePathFile(&faux) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_PhysicalFile::Init(%0:s)") 
			<< faux.InChar(0) << SEND_MSG   ;
	};
	#endif


	if ( ! ClMIT_File::Exist(faux) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_PhysicalFile::Init for %0:s failed, file doesn't exists", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
		Return = M4_FALSE ;
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}


void ClENG_PhysicalFile::GetRelativePath(ClMIT_Str *ao_Path)
{
	m_FatherPath->GetRelativePath(ao_Path) ;
}

void ClENG_PhysicalFile::GetAbsolutePath(ClMIT_Str *ao_Path)
{
	m_FatherPath->GetAbsolutePath(ao_Path) ;
}

void ClENG_PhysicalFile::GetRelativePathFile(ClMIT_Str *ao_Path)
{
	m_FatherPath->GetRelativePath(ao_Path) ;
	ClMIT_File::ConcatToPath(ao_Path,m_FileName) ;
}

void ClENG_PhysicalFile::GetAbsolutePathFile(ClMIT_Str *ao_Path)
{
	m_FatherPath->GetAbsolutePath(ao_Path) ;
	ClMIT_File::ConcatToPath(ao_Path,m_FileName) ;
}

m4bool_t ClENG_PhysicalFile::Open(
	ClMIT_File::MODE_WRITE ai_Mode, 
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str faux ;

	GetAbsolutePathFile(&faux) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_PhysicalFile::Open(%0:s)") 
			<< faux.InChar(0) << SEND_MSG   ;
	};
	#endif

	if ( ! IsOpen() ) {

		if ( m_F.Open( faux, ClMIT_File::EXIST, ai_Mode, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
 
			;
			
		} else { 
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
				"ClENG_PhysicalFile::Open for %0:s failed", ClMIT_Msg::MIXED_OUT) 
				<<  faux.InChar(0) << SEND_MSG ;
			Return = M4_FALSE ;
		};

	} else {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID,
			"ClENG_PhysicalFile::Open for %0:s not allowed, file yet oppened", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

void ClENG_PhysicalFile::Close() 
{
	ClMIT_Str faux ;

	GetAbsolutePathFile(&faux) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_PhysicalFile::Close(%0:s)") 
			<< faux.InChar(0) << SEND_MSG   ;
	};
	#endif

	if ( IsOpen() ) {

		m_F.Close() ;

	} ;
}

m4bool_t ClENG_PhysicalFile::Remove(m4bool_t ai_SwMsg)  
{ 
	return m_FatherPath->Remove(this,ai_SwMsg) ; 
}


//=========================================================================ClENG_Path

ClENG_Path::ClENG_Path()
{
	m_poInsp=GET_INSP();

	m_FileSet=NULL;
}


void ClENG_Path::End() 
{	
	FileList_t::iterator it ;

	m_FileSet=NULL ;	
	m_RelativePath.End() ;

	for (it=m_oFileList.begin(); it!=m_oFileList.end(); ++it) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, (*it).second, ClENG_PhysicalFile ) ;
	} ;
	m_oFileList.clear();
}

m4bool_t ClENG_Path::Init( 
		ClENG_PhysicalFileSet *ai_FileSet, 
		const char *ai_Path, m4bool_t ai_bAbsolute, m4bool_t ai_bCreateDir,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str faux ;

	ClENG_Path::End();

	m_FileSet = ai_FileSet ;
	m_RelativePath.StrCpy(ai_Path) ;
	m_bAbsolute = ai_bAbsolute ;

	GetAbsolutePath(&faux) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_Path::Init, relative path %0:s, absolute %1:s") 
			<< m_RelativePath.InChar(0) << faux.InChar(0) << SEND_MSG   ;
	};
	#endif

	if (ai_bCreateDir) {
		if ( ClMIT_File::CreatePath( faux, ClMIT_File::SILENT_TRY_CREATE ) == ClMIT_File::PATH_ERROR ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163030,
				"ClENG_Path::Init for %0:s failed (create dir %1:s)", ClMIT_Msg::MIXED_OUT) 
				<<  faux.InChar(0) << ai_bCreateDir << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	if ( ! ClMIT_File::Exist(faux) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163031,
			"ClENG_Path::Init for %0:s failed, directory doesn't exists (create dir %1:s)", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << ai_bCreateDir << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

void ClENG_Path::GetRelativePath(ClMIT_Str *ao_Path)
{
	ao_Path->StrCpy(m_RelativePath) ;
}

void ClENG_Path::GetAbsolutePath(ClMIT_Str *ao_Path)
{
	if ( IsAbsolute() ) {
		ao_Path->StrCpy(m_RelativePath) ; 
	} else {
		m_FileSet->GetRootPath(ao_Path) ;
		ClMIT_File::ConcatToPath(ao_Path,m_RelativePath) ;
	} ;
	
}

void ClENG_Path::MakeAbsolutePathFile(ClMIT_Str *ao_Path, const char *ai_FileName )
{
	GetAbsolutePath(ao_Path) ;
	ClMIT_File::ConcatToPath(ao_Path,ai_FileName) ;
}

ClENG_PhysicalFile * ClENG_Path::CreateFile( const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str faux ;
	ClMIT_File f ;
	pair<FileList_t::iterator,bool> Pair ;
	m4bool_t SwInserted =M4_FALSE ;

	ClMIT_Str key ;
	ClENG_PhysicalFile * value=NULL ;

	MakeAbsolutePathFile(&faux,ai_FileName) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_Path::CreateFile %0:s") 
			<< faux.InChar(0) << SEND_MSG   ;
	};
	#endif

	if ( ! f.Open(faux,ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::SILENT_TRY_OPEN, M4_FALSE) ) {

		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163032,
			"ClENG_Path::CreateFile for %0:s failed, can not create the file", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;
	f.Close() ;

	key.StrCpy(ai_FileName) ; 
	M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, value, ClENG_PhysicalFile );
	if ( ! value->Init(this,ai_FileName,ClMIT_Trace::PROPAGATE_ERROR) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163032,
			"ClENG_Path::CreateFile for %0:s failed", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
	} ;
	
	Pair = m_oFileList.insert( FileList_t::value_type(key,value) ) ;
	if ( ! Pair.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163032,
			"ClENG_Path::CreateFile for %0:s failed, file exists at logical level", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} else {
		SwInserted=M4_TRUE ;
	} ;

exit:

	if (!Return) {
		if (SwInserted) {
			m_oFileList.erase( Pair.first ) ;
		} ;
		if(value) {
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, value, ClENG_PhysicalFile ) ;
		} ;
	} ;

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return value ;		
}

ClENG_PhysicalFile * ClENG_Path::LinkToFile( const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return = M4_TRUE ;
	ClMIT_Str faux ;
	pair<FileList_t::iterator,bool> Pair ;
	m4bool_t SwInserted =M4_FALSE ;

	ClMIT_Str key ;
	ClENG_PhysicalFile * value=NULL ;

	MakeAbsolutePathFile(&faux,ai_FileName) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_Path::LinkToFile %0:s") 
			<< faux.InChar(0) << SEND_MSG   ;
	};
	#endif

	key.StrCpy(ai_FileName) ; 
	M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, value, ClENG_PhysicalFile );
	if ( ! value->Init(this,ai_FileName,ClMIT_Trace::PROPAGATE_ERROR) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163033,
			"ClENG_Path::LinkToFile for %0:s failed", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
	} ;
	
	Pair = m_oFileList.insert( FileList_t::value_type(key,value) ) ;
	if ( ! Pair.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163033,
			"ClENG_Path::LinkToFile for %0:s failed, file exists at logical level", ClMIT_Msg::MIXED_OUT) 
			<<  faux.InChar(0) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} else {
		SwInserted=M4_TRUE ;
	} ;

exit:

	if (!Return) {
		if (SwInserted) {
			m_oFileList.erase( Pair.first ) ;
		} ;
		if ( value ) {
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, value, ClENG_PhysicalFile ) ;
		} ;
	} ;

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return value ;		
}

m4bool_t ClENG_Path::DeleteDirectory()
{
	ClMIT_Str str ;

	GetAbsolutePath(&str) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_Path::DeleteDirectory %0:s") 
			<< str.InChar(0) << SEND_MSG   ;
	};
	#endif

	return M4RemoveDirectory(str) ;
}

m4bool_t ClENG_Path::Remove(ClENG_PhysicalFile *ai_poFile, m4bool_t ai_SwMsg) 
{
	m4bool_t SwOk=M4_TRUE ;
	FileList_t::iterator it ;
	ClMIT_Str key ;
	ClMIT_Str str ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_DEBUG, GROUP_ID, 
			"ClENG_Path::Remove the file with logical id %0:s") 
			<< ai_poFile->GetFileName() << SEND_MSG   ;
	};
	#endif

	ai_poFile->GetAbsolutePathFile(&str) ;
	SwOk=ClMIT_File::Remove(str,ai_SwMsg) ;
	if (!SwOk) goto exit ;

	key.StrCpy(ai_poFile->GetFileName()) ;
	it = m_oFileList.find(key) ;
	if (it==m_oFileList.end()) {
		//FER !!!
		//*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
		//	"ClENG_Path::Unlink , key %0:s at logical level not found", ClMIT_Msg::MIXED_OUT) 
		//	<<  key.InChar(0) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_oFileList.erase(it) ;

exit:
	return SwOk ;
} ;

//=========================================================================ClENG_PhysicalFileSet

ClENG_PhysicalFileSet::ClENG_PhysicalFileSet()
{
	m_poInsp=GET_INSP();
}


void ClENG_PhysicalFileSet::End() 
{	
	PathVector_t::iterator it ;

	m_Root.End() ;

	for (it=m_oPathList.begin(); it!=m_oPathList.end(); ++it) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, (*it), ClENG_Path ) ;
	} ;
	m_oPathList.clear();
}

m4bool_t ClENG_PhysicalFileSet::Init( 
		const m4char_t *ai_pcAbsolutePath, m4bool_t ai_bCreateDir,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4bool_t Return = M4_TRUE ;

	ClENG_PhysicalFileSet::End();

	Return = m_Root.Init(this, ai_pcAbsolutePath, M4_TRUE,ai_bCreateDir, ai_ErrorEffect ) ;

	return Return ;		

}

void ClENG_PhysicalFileSet::GetRootPath(ClMIT_Str *ao_Path) 
{
	m_Root.GetAbsolutePath(ao_Path) ;	
}

ClENG_Path * ClENG_PhysicalFileSet::NewRelativeDir(
	const m4char_t *ai_pcPath, m4bool_t ai_bCreateDir,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	ClENG_Path * Path=NULL  ;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, Path, ClENG_Path ) ;
	if ( Path->Init(this,ai_pcPath,M4_FALSE,ai_bCreateDir,ai_ErrorEffect) ) {
		m_oPathList.push_back(Path) ;
	} else {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, Path, ClENG_Path ) ;
	} ;

	return Path ;
} ;


ClENG_PhysicalFile * ClENG_PhysicalFileSet::CreateFile( ClENG_Path * ai_Path, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	return ai_Path->CreateFile(ai_FileName,ai_ErrorEffect) ;
}

ClENG_PhysicalFile * ClENG_PhysicalFileSet::LinkToFile( ClENG_Path * ai_Path, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	return ai_Path->LinkToFile(ai_FileName,ai_ErrorEffect) ;
}



