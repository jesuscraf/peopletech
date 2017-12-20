//## begin module%3912A6BD0329.cm preserve=no
//## end module%3912A6BD0329.cm

//## begin module%3912A6BD0329.cp preserve=no
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
//## end module%3912A6BD0329.cp

//## Module: m4glbdat%3912A6BD0329; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\m4glbdat.cpp

//## begin module%3912A6BD0329.additionalIncludes preserve=no
//## end module%3912A6BD0329.additionalIncludes

//## begin module%3912A6BD0329.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                m4glbdat.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las tablas de datos
//
//
//
//==============================================================================



#include "m4objglb.hpp"
//## end module%3912A6BD0329.includes

//## begin module%3912A6BD0329.declarations preserve=no
//## end module%3912A6BD0329.declarations

//## begin module%3912A6BD0329.additionalDeclarations preserve=yes
//## end module%3912A6BD0329.additionalDeclarations


//## begin module%3912A6BD0329.epilog preserve=yes




//=================================================================================
// Array de mappings de items conectados
//=================================================================================

m4uint8_t	g_aiScopeArray[ M4CL_NODE_CONNECTOR_MAX_TYPE ][ M4CL_ITEM_MAX_SCOPE ][ M4CL_ITEM_MAX_SCOPE ] =
{
// RB
    {

//						hijo nodo				hijo bloque			hijo registro
/*padre nodo*/		{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_NODE,  M4CL_ITEM_SCOPE_NODE  },
/*padre bloque*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_BLOCK },
/*padre registro*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_BLOCK }
    },

// BB
    {
//						hijo nodo				hijo bloque			hijo registro
/*padre nodo*/		{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_NODE,  M4CL_ITEM_SCOPE_NODE     },
/*padre bloque*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_BLOCK    },
/*padre registro*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_REGISTER }
    },

// FREE
    {
//						hijo nodo				hijo bloque			hijo registro
/*padre nodo*/		{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_NODE,  M4CL_ITEM_SCOPE_NODE     },
/*padre bloque*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_BLOCK    },
/*padre registro*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_REGISTER }
    },

// AUTO, es como el free
    {
//						hijo nodo				hijo bloque			hijo registro
/*padre nodo*/		{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_NODE,  M4CL_ITEM_SCOPE_NODE },
/*padre bloque*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_BLOCK },
/*padre registro*/	{ M4CL_ITEM_SCOPE_NODE, M4CL_ITEM_SCOPE_BLOCK, M4CL_ITEM_SCOPE_REGISTER }
    }

} ;


//## end module%3912A6BD0329.epilog
