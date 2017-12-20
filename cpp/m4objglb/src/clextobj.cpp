//## begin module%3912A6BD02D8.cm preserve=no
//## end module%3912A6BD02D8.cm

//## begin module%3912A6BD02D8.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%3912A6BD02D8.cp

//## Module: clextobj%3912A6BD02D8; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\clextobj.cpp

//## begin module%3912A6BD02D8.additionalIncludes preserve=no
//## end module%3912A6BD02D8.additionalIncludes

//## begin module%3912A6BD02D8.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             clextobj.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
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

#include "clextobj.hpp"
#include "clextlib.hpp"

//## end module%3912A6BD02D8.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A6BD02D8.declarations preserve=no
//## end module%3912A6BD02D8.declarations

//## begin module%3912A6BD02D8.additionalDeclarations preserve=yes
//## end module%3912A6BD02D8.additionalDeclarations


// Class M4ClExternObjectMapEntry 




M4ClExternObjectMapEntry::M4ClExternObjectMapEntry (void )
  //## begin M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.hasinit preserve=no
  //## end M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.hasinit
  //## begin M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.initialization preserve=yes
  //## end M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.initialization
{
  //## begin M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.body preserve=yes
	m_iObjectType = 0;

	m_iLinks = 0;
	
	m_pExtObj = NULL;
  //## end M4ClExternObjectMapEntry::M4ClExternObjectMapEntry%716586242.body
}


M4ClExternObjectMapEntry::~M4ClExternObjectMapEntry (void )
{
  //## begin M4ClExternObjectMapEntry::~M4ClExternObjectMapEntry%-56678893.body preserve=yes
	if ( m_pExtObj != NULL )
	{
		switch ( m_iObjectType )
		{
			case M4CL_INTERN_OBJECT_DLL:
				delete ( m_pExtObj );
				break;

			default:
				return; //ERROR
				break;
		}
	}
  //## end M4ClExternObjectMapEntry::~M4ClExternObjectMapEntry%-56678893.body
}



//## Other Operations (implementation)
m4return_t M4ClExternObjectMapEntry::Init (m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType)
{
  //## begin M4ClExternObjectMapEntry::Init%-1727756540.body preserve=yes
	m4return_t result;
	m_iObjectType = ai_iObjectType;

	m_iLinks = 1;
	
	switch ( m_iObjectType )
	{
		case M4CL_INTERN_OBJECT_DLL:
			m_pExtObj = new M4ClExternLibrary ( );
			result = m_pExtObj ->Init ( ai_pccObjectName );
			return result;
			break;

		default:
			m_pExtObj = NULL;
			return M4_ERROR; //ERROR
			break;
	}
  //## end M4ClExternObjectMapEntry::Init%-1727756540.body
}

// Additional Declarations
  //## begin M4ClExternObjectMapEntry%3912A6BA01E4.declarations preserve=yes
  //## end M4ClExternObjectMapEntry%3912A6BA01E4.declarations

//## begin module%3912A6BD02D8.epilog preserve=yes
//## end module%3912A6BD02D8.epilog
