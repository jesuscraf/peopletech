//## begin module%377C88460055.cm preserve=no
//## end module%377C88460055.cm

//## begin module%377C88460055.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================



#ifndef __FICHERO1_HPP__
#define __FICHERO1_HPP__ 1


#include "m4log.hpp"
#include <debuglog.hpp>
#include <m4string.hpp>
#include "m4stl.hpp"
#include <stdio.h>


class ClLogManager;
class ClAppItem;
class M4LogFile;




#define	M4_STORAGE_TYPE_FILE 1
#define	M4_STORAGE_TYPE_QUEUE 2
#define	M4_STORAGE_TYPE_GLOBAL_QUEUE 3

#define M4_LOGFILE_LOG_FORMAT	 0
#define M4_LOGFILE_HTML_FORMAT	 1




class M4_DECL_M4LOG ClPersistentStorage 
{
  public:

      ClPersistentStorage (ClLogManager* plogManager, m4uint16_t ai_uiStorageType)
	  {
			m_pLogManager = plogManager;
			m_uiStorageType = ai_uiStorageType;
	  }

      virtual ~ClPersistentStorage () {}

      virtual m4return_t AddAppoint (ClAppItem* pAppItem)
	  {
		  return M4_SUCCESS;
	  }

      virtual m4int16_t GetType()
	  {
		  return m_uiStorageType;
	  }

  protected:

      ClLogManager *m_pLogManager;
      m4uint16_t m_uiStorageType;

};




class ClFileStorage : public ClPersistentStorage  //## Inherits: <unnamed>%377C8850035B
{
  public:

      ClFileStorage (ClLogManager* ai_plogManager, m4pcchar_t ai_pccName, m4int16_t ai_iFileFormat, m4pcchar_t ai_pccStorageName);
      ~ClFileStorage ();

      m4return_t AddAppoint (ClAppItem* pAppItem);

  protected:

      M4LogFile	*m_poLogFile;
      m4int16_t	m_iFileFormat;
};



typedef list < ClPersistentStorage * > STORAGELIST;


class M4_DECL_M4LOG  ClStorageList 
{
  public:
      ClStorageList ();
      virtual ~ClStorageList ();

      m4uint32_t CheckforErrorQueue ();
      m4bool_t StoreToLocalQueue (ClAppItem* pappItem);
      m4return_t AddStorage (ClPersistentStorage* pstorage);
      ClPersistentStorage* GetStorage (m4int16_t ipos);
      m4return_t StoreTo (m4uint32_t sflags, ClAppItem* pappItem);

  protected:

      STORAGELIST m_lstorages;

      void DestroyStorages ();

    friend class M4_DECL_M4LOG ClLogManager;
};



class ClPrivateLocalQueue : public ClPersistentStorage
{
  public:
      ClPrivateLocalQueue (ClLogManager* ai_plogManager)
		: ClPersistentStorage( ai_plogManager, M4_STORAGE_TYPE_QUEUE )
	  {
	  }

      ~ClPrivateLocalQueue () {} ;
};




#endif
