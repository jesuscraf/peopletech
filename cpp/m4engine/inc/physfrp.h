//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             physfrp.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//
//==============================================================================

#ifndef __PHYSFRP_H__
#define __PHYSFRP_H__

//Para el multimap
#include "m4stl.hpp"
#include "clfile.h"
#include "clstr.h"
#include "m4clbuf.h"
#include "replace.h"

class ClENG_PhysicalFileSet ;
class ClENG_Path ;


//=========================================================================ClENG_PhysicalFile

class ClENG_PhysicalFile
{
MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	friend class ClENG_PhysicalFileSet ;
	friend class ClENG_Path ;    

MIT_PRIVATE:
	
    ClENG_Path *m_FatherPath ;	//Link to owner
	ClMIT_Str m_FileName;
    ClMIT_File m_F ; 
	
MIT_PRIVATE:

	//---------------------------------------Initialize--------------------------------
	// All initilize methods are private; the facade class  control these issues.

	ClENG_PhysicalFile();
	virtual ~ClENG_PhysicalFile() { ClENG_PhysicalFile::End() ; }
	virtual void End() ; //Close the file if it is openned

	virtual size_t GetSizeof() { return sizeof(ClENG_PhysicalFile) ; }

    //The file MUST exists, it starts in NOT_PROCESSED (closed) status.
	m4bool_t Init( 
		ClENG_Path *ai_FatherPath, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);
	

MIT_PUBLIC:

	//---------------------------------------Gets--------------------------------------

    const char * GetFileName() { return m_FileName.InChar(0) ; }

    //Get the relative path witch the file states. This NOT include the file name.
    void GetRelativePath(ClMIT_Str *ao_Path);

    //Get the absolute path witch the file states. This NOT include the file name.
    void GetAbsolutePath(ClMIT_Str *ao_Path);

    //Get the relative path and file name.
    void GetRelativePathFile(ClMIT_Str *ao_Path);

    //Get the absolute path and file name. 
    void GetAbsolutePathFile(ClMIT_Str *ao_Path);

	m4bool_t IsOpen() {  return  m_F.IsOpen() ; }

	//---------------------------------Flow methods--------------------------------------

	//If the file is openned, a warning is thrown (but true is returned!).
	//Error if file can not be openned.
    m4bool_t Open(ClMIT_File::MODE_WRITE ai_Mode, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//If the file is closed, nothing happens
    void Close() ;

	//---------------------------------ClMIT_File methods exported-----------------------
	//Ver ClMIT_File

	void DumpBuffer( M4ClBuffer *ai_Buff ) { ai_Buff->Flush( M4ClBuffer::WFileManip, &m_F ) ; }

	void DumpBuffer( M4ClBuffer *ai_Buff, m4uint32_t ai_uiFrom, m4uint32_t ai_uiNBytes ) { 
		ai_Buff->Flush( M4ClBuffer::WFileManip, &m_F, ai_uiFrom, ai_uiNBytes   ) ; }

	void W( const void *ai_pvBuff, size_t ai_szNBytes ) { m_F.W( ai_pvBuff, ai_szNBytes ) ; }

	void WStr( const m4char_t * ai_pcStr ) { m_F.WStr( ai_pcStr ) ; }

	void WChar(m4char_t ai_pcChar) { m_F.WChar(ai_pcChar) ; }

	size_t R( void * ai_pvBuff, size_t ai_szNBytes ) 
		{ return m_F.R( ai_pvBuff, ai_szNBytes ) ; }

	m4bool_t WFile( const m4char_t * ai_pcFile, size_t *ao_szWritten=NULL ) 
		{ return m_F.WFile( ai_pcFile, ao_szWritten) ; }

	m4bool_t WFile( const m4char_t * ai_pcFile, m4uint32_t ai_From, size_t ai_Count,
		size_t *ao_szWritten=NULL ) 
		{ return m_F.WFile( ai_pcFile, ai_From, ai_Count,ao_szWritten ) ; }

	m4uint32_t GetPos() { return m_F.GetPos(); }

	void SetPos( m4uint32_t ai_uiOff ) { m_F.SetPos(ai_uiOff) ; } 

	m4uint32_t GetSize() { return m_F.GetSize() ; }

	m4bool_t IsEOF() { return m_F.IsEOF() ; }
	
	void Flush() { m_F.Flush() ; }

	m4bool_t Remove(m4bool_t ai_SwMsg=M4_TRUE)  ;

	//----------------------Other exported methods--------------------------------------

	m4bool_t WReplace( 
		ClMIT_CopyReplace *ai_Template, 
		ClMIT_CopyReplace::Patterns_t *ao_Patterns ) 
			{ return ai_Template->Replace(&m_F, ao_Patterns ) ;}

};

//=========================================================================ClENG_Path

class ClENG_Path
{
	friend class ClENG_PhysicalFileSet ;

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	ClENG_PhysicalFileSet *m_FileSet ;	//Link
	m4bool_t m_bAbsolute ;
	ClMIT_Str m_RelativePath ;
	
	//Key, FileName
	typedef map<ClMIT_Str, ClENG_PhysicalFile *, less<ClMIT_Str> > FileList_t;
	FileList_t m_oFileList;

MIT_PRIVATE:
	
	//---------------------------------------Initialize--------------------------------
	// All initialize methods are private; the facade class  control these issues.

	ClENG_Path();
	virtual ~ClENG_Path() { ClENG_Path::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_Path) ; }

    //If the directory is not absolute, error if it is not relative to the absolute path.
	//If the directory doesn't exist:
	//		-ai_bCreateDir = false. Error.
	//		-ai_bCreateDir = true. The directory will be created. Error if this operation fails.
	//If the directory exists, this method is sucessful.
	//So on, a true return means that  finally the directory exists.

	m4bool_t Init( 
		ClENG_PhysicalFileSet *ai_FileSet, 
		const char *ai_Path, m4bool_t ai_bAbsolute, m4bool_t ai_bCreateDir,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);
	
	
MIT_PUBLIC:

	//---------------------------------------Gets---------------------------------------------

	m4bool_t IsAbsolute() { return m_bAbsolute; }

    void GetRelativePath(ClMIT_Str *ao_Path);

    void GetAbsolutePath(ClMIT_Str *ao_Path);

    //Create an absolute path and file name for this path. 
	//This file doesn't reflect any object reference; it only create a full path in order to
	//	work
    void MakeAbsolutePathFile(ClMIT_Str *ao_Path, const char *ai_FileName );


	m4bool_t DeleteDirectory(); 

	//Remove physically a file. The attached path will not track this file anymore.
	m4bool_t Remove(ClENG_PhysicalFile *ai_poFile, m4bool_t ai_SwMsg=M4_TRUE) ;

MIT_PRIVATE:

	//---------------------------------------Flow methods-------------------------------------
	// All flow methods are private; the facade class  control these issues.

    //Error if the file can not be created. If a previous file exists, it will be deleted. 
	//Returns the new object or NULL if they are any error.
	//If the file already exists in the list, a error will be thrown.

    ClENG_PhysicalFile * CreateFile( const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

    //Error if the file doesn't exist.
	//Returns the new object or NULL if they are any error.
	//If the file already exists in the list, a error will be thrown.

    ClENG_PhysicalFile * LinkToFile( const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

} ;

//=======================================================================ClENG_PhysicalFileSet

class ClENG_PhysicalFileSet
{
MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link


	ClENG_Path m_Root ;	

	typedef vector<ClENG_Path *> PathVector_t;
	PathVector_t m_oPathList;

MIT_PRIVATE:
	

MIT_PUBLIC:

	//---------------------------------------Initialize--------------------------------

	ClENG_PhysicalFileSet();
	virtual ~ClENG_PhysicalFileSet() { ClENG_PhysicalFileSet::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_PhysicalFileSet) ; }

	m4bool_t Init( 
		const m4char_t *ai_pcAbsolutePath, m4bool_t ai_bCreateDir,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);

	//---------------------------------------Gets---------------------------------------------

	void GetRootPath(ClMIT_Str *ao_Path) ;
	ClENG_Path * GetRootPathReference() { return &m_Root ; }
	
	//---------------------------------------Flow methods-------------------------------

	ClENG_Path * NewRelativeDir(const m4char_t *ai_pcPath, m4bool_t ai_bCreateDir,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;


	//View ClPath::CreateFile
    ClENG_PhysicalFile * CreateFile( ClENG_Path * ai_Path, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//View ClPath::LinkToFile
    ClENG_PhysicalFile * LinkToFile( ClENG_Path * ai_Path, const char *ai_FileName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

} ;

#endif
