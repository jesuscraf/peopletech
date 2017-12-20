//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              admindatatypes.cpp
//	 Project:           m4adminsrv
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the data types used to manage administration
//		information.
//
//	==============================================================================
//## end module.cp

//## Module: AdminDataTypes; Package body
//	This component holds the administration data types
//	typedef and classes used to manage every subsystem.
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\src\admindatatypes.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "m4trace.hpp"
//## end module.includes

// AdminDataTypes
#include <admindatatypes.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClAdminAttributeSyntax 







ClAdminAttributeSyntax::ClAdminAttributeSyntax (M4AdminAttrDescriptor ai_attDescriptor, const M4ClString &ai_attName, M4AdminAttrType ai_attType, M4AdminAttrAccessMode ai_attAccessMode, M4AdminAttrAccessPermissions ai_attAccessPermissions)
  //## begin ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.hasinit preserve=no
  //## end ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.hasinit
  //## begin ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.initialization preserve=yes
  : m_id(ai_attDescriptor), 
	m_name(ai_attName),
	m_type(ai_attType),
	m_accessMode(ai_attAccessMode), 
	m_accessPermissions(ai_attAccessPermissions)
  //## end ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.initialization
{
  //## begin ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminAttribute::ClAdminAttribute(...)");
  //## end ClAdminAttributeSyntax::ClAdminAttributeSyntax%919702361.body
}


ClAdminAttributeSyntax::~ClAdminAttributeSyntax ()
{
  //## begin ClAdminAttributeSyntax::~ClAdminAttributeSyntax%919702362.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminAttribute::~ClAdminAttribute ()");
  //## end ClAdminAttributeSyntax::~ClAdminAttributeSyntax%919702362.body
}


// Additional Declarations
  //## begin ClAdminAttributeSyntax.declarations preserve=yes
  //## end ClAdminAttributeSyntax.declarations



// Class ClAdminMemoryManagement 

void * ClAdminMemoryManagement::operator new (size_t ai_size)
{
  //## begin ClAdminMemoryManagement::operator new%919702367.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminTagBase::operator new(size_t ai_size)");

	return ::operator new(ai_size);
  //## end ClAdminMemoryManagement::operator new%919702367.body
}

void * ClAdminMemoryManagement::operator new (size_t ai_size, void *ao_p)
{
  //## begin ClAdminMemoryManagement::operator new%919702368.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminTagBase::operator new(size_t ai_size, void *ao_p)");

	return ::operator new(ai_size, ao_p);
  //## end ClAdminMemoryManagement::operator new%919702368.body
}

void ClAdminMemoryManagement::operator delete (void *ai_p)
{
  //## begin ClAdminMemoryManagement::operator delete%919702369.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminTagBase::operator delete(void *ai_p)");

	::operator delete(ai_p);
  //## end ClAdminMemoryManagement::operator delete%919702369.body
}


// Additional Declarations
  //## begin ClAdminMemoryManagement.declarations preserve=yes
ClAdminMemoryManagement::~ClAdminMemoryManagement ()
{
	m4AutoTraceLevel(5, "ClAdminMemoryManagement::~ClAdminMemoryManagement");
}
  //## end ClAdminMemoryManagement.declarations


// Class ClAdminCommandSyntax 





ClAdminCommandSyntax::ClAdminCommandSyntax (M4AdminCommandDescriptor ai_attDescriptor, const M4ClString &ai_attName, m4bool_t ai_hasParameters, M4AdminAttrAccessPermissions ai_attAccessPermissions)
  //## begin ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.hasinit preserve=no
  //## end ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.hasinit
  //## begin ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.initialization preserve=yes
  : m_id(ai_attDescriptor), 
	m_name(ai_attName),
	m_hasParameters(ai_hasParameters), 
	m_accessPermissions(ai_attAccessPermissions)
  //## end ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.initialization
{
  //## begin ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandSyntax::ClAdminCommandSyntax");
  //## end ClAdminCommandSyntax::ClAdminCommandSyntax%919702372.body
}


ClAdminCommandSyntax::~ClAdminCommandSyntax ()
{
  //## begin ClAdminCommandSyntax::~ClAdminCommandSyntax%919702373.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandSyntax::~ClAdminCommandSyntax");
  //## end ClAdminCommandSyntax::~ClAdminCommandSyntax%919702373.body
}


// Additional Declarations
  //## begin ClAdminCommandSyntax.declarations preserve=yes
  //## end ClAdminCommandSyntax.declarations



//## begin search_by_id_typedef.instantiation preserve=yes
/*
 * commented due to MSVC gives the next warning:
 * warning C4660: template-class specialization 
 *	'unary_function<class ClAdminAttribute *,bool>' 
 * is already instantiated
 */
//template class unary_function< ClAdminAttribute*,m4bool_t >;
//## end search_by_id_typedef.instantiation


// Class search_by_id 


search_by_id::search_by_id (M4AdminAttrDescriptor ai_id)
  //## begin search_by_id::search_by_id%919702374.hasinit preserve=no
  //## end search_by_id::search_by_id%919702374.hasinit
  //## begin search_by_id::search_by_id%919702374.initialization preserve=yes
  : m_key(ai_id)
  //## end search_by_id::search_by_id%919702374.initialization
{
  //## begin search_by_id::search_by_id%919702374.body preserve=yes
  //## end search_by_id::search_by_id%919702374.body
}



//## Other Operations (implementation)
m4bool_t search_by_id::operator () (const ClAdminAttributeSyntax *ai_poAdminAtt) const
{
  //## begin search_by_id::operator()%919702375.body preserve=yes
	m4bool_t retCode = M4_FALSE;
	if(ai_poAdminAtt->m_id == m_key)
		retCode = M4_TRUE;

	return retCode;
  //## end search_by_id::operator()%919702375.body
}

// Additional Declarations
  //## begin search_by_id.declarations preserve=yes
  //## end search_by_id.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
