
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_manager.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//
//==============================================================================



#ifndef __FILE_MANAGER_HPP__
#define __FILE_MANAGER_HPP__


#include "m4stl.hpp"
#include "file_misc.hpp"
#include "m4types.hpp"


#define M4_TEMPFILE_SUCCESS			0
#define M4_TEMPFILE_INVALID_DIR		1
#define M4_TEMPFILE_NO_PERMISSION	2
#define M4_TEMPFILE_NO_RESOURCES		3

	

class M4_DECL_M4FILEMANAGER ClFileManager 
{
private:
		typedef struct {
			m4bool_t bErase;
			m4uint32_t cRef;
		} st_Control;
		typedef map<ClFileName,st_Control,less<ClFileName> > ControlMap_t;
		typedef ControlMap_t::iterator itControlMap_t;
		typedef pair<itControlMap_t,bool > ControlPair_t;

public:

      virtual ~ClFileManager ();

      static ClFileManager* Instance ();

      m4return_t SetDefaultDirectory (const m4char_t* ai_pszDefaultTempDirectory);

      const m4char_t* GetDefaultDirectory ()
      {
			return m_oDefaultDir;
      }

      m4return_t SetInterestInFile (const ClFileName& ai_oFileName)
      {
			if (ai_oFileName.IsNull())
				return M4_SUCCESS;
			return SetInterest(ai_oFileName,M4_FALSE);
      }

      m4return_t CreateTempFile (ClFileName& ao_oFileName, const m4char_t* ai_pszPrefix, const m4char_t* ai_pszExtension, const m4char_t* ai_pszTempDirectory = NULL, m4pint32_t ao_pError=NULL);

      m4return_t UnSetInterestInFile (const ClFileName& ai_oFileName);

      m4return_t CollectGarbage ();

      m4return_t GetPendingGarbageList (ClFileList& ao_oFileNames) const;

      const void* GetUniqueId (const ClFileName& ai_oFileName);

      m4return_t SetFileManagerOwnerOfTheFile (const ClFileName& ai_oFileName, m4bool_t ai_bFileManagerIsOwnerOfTheFile);

      m4bool_t GetFileManagerOwnerOfTheFile (const ClFileName& ai_oFileName);

      m4return_t CreatePath (const m4char_t* ai_pszPathToCreate);

	  m4return_t GetTempDir(m4char_t* ao_pszTempDir);

protected:

      ClFileManager ();

      m4return_t SetInterest (const ClFileName& ai_oFileName, m4bool_t bErase);

private:

      ClFileName m_oDefaultDir;
      static ClFileManager m_oInstance;
      ControlMap_t m_mapControl;
      ClFileList m_oPendingGarbage;
};




/*************************************************************************

NAME: ClTemporalFileCreator
	
*************************************************************************/

class M4_DECL_M4FILEMANAGER ClTemporalFileCreator 
{
public:

	static m4return_t CreateTempFile (m4pchar_t ao_pszNewFileName, const m4char_t* ai_pszPrefix, const m4char_t* ai_pszExtension, const m4char_t* ai_pszDirectory, m4pint32_t ao_pError=NULL);

protected:

	static m4uint16_t m_usVarPart;
	static m4uint32_t m_cCont;
};




class M4_DECL_M4FILEMANAGER ClFileDirOperations 
{
public:

	static m4return_t CreatePath (const m4char_t* ai_pszPathToCreate);
	static m4return_t RemoveDirectory (const m4char_t *ai_pszDirectory, const m4bool_t ai_bRecursive=M4_TRUE);

private:

	static m4void_t _ActualizePathSlash (m4char_t* aio_pszPath);
};


#endif
