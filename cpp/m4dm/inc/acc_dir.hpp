//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             acc_dir.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             05/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4stl.hpp"
#include "index.hpp"

//## end module.includes preserve=yes

#ifndef __ACC_DIR__HPP__
#define __ACC_DIR__HPP__

//## begin module.additionalDeclarations preserve=yes
class ClAccess;
class ClChannel;
class ClSize;
//## end module.additionalDeclarations

class M4_DECL_M4DM ClAccessDirectory
{
public:
	ClAccessDirectory();
	~ClAccessDirectory();
	
	ClAccess *operator [] (const ClAccessIndex ai_index);
	ClAccess &operator * (void);
	ClAccess *operator -> (void);
	
	m4bool_t MoveTo(const ClAccessIndex ai_index);
	
	m4bool_t Begin(void);
	m4bool_t End(void);
	
	m4bool_t Prev(void);
	m4bool_t Next(void);
	
	m4uint32_t Count(void);
	
	m4return_t SetAutoLoadMode(m4uint8_t ai_iautoload);
	
	m4return_t GetSize(ClSize &ao_size);
	
private:
	/** Índice del acceso sobre el que estamos trabajando dentro del directorio de accesos. */
	ClAccessIndex Index;
	
	vector<ClAccess *> m_vpaccess;
	typedef vector<ClAccess *>::iterator accessvectorit;
	
	m4return_t _Add (ClAccess *ai_paccess);
	m4return_t _Remove (ClAccess *ai_paccess);
	
	m4return_t _DeleteAll (void);
	void       _DetachAll (void);
	
	static ClAccess *ms_pNihilAccess;
	
friend class ClNode;
friend class ClChannel;
friend class ClAccess;
friend class ClProxySpace ;

};
//## begin module.epilog preserve=yes


//## end module.epilog
#endif // __ACC_DIR__HPP__
