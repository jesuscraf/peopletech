/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <m4uuid.hpp>                                         |
| Project          : Meta4Mind®©                                          |
| Author           : Meta4 Spain, S.A.                                    |
| Date             : 08/Jun/1999                                          |
| Language         : C++                                                  |
| Operating System : All                                                  |
| Design Document  :                                                      |
| Description      :                                                      |
|                                                                         |
+-------------------------------------------------------------------------+
| History                                                                 |
|                                                                         |
|     <Ver>  <Programer>     <Date>                                       |
|        1    J.G.L.          08/Jun/1999                                 |
+-------------------------------------------------------------------------+
*/

#ifndef _M4UUID__HPP_
#define _M4UUID__HPP_ 


/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/

#include "m4uuid_dll.hpp"
#include "m4types.hpp"
#include "m4typesext.hpp"
#include "multipla.hpp"
#include "uuid.hpp"

#define M4CL_VM_SIZE_UUID 24
/*
+-------------------------------------------------------------------------+
| Public Functions                                                        |
+-------------------------------------------------------------------------+
*/

//--- This functions gives a UUID based on time.
//--- ai_pVArg[0] : OUT : String (length 36) containing the UUID formated.

M4_DECL_M4UUID m4return_t	GetUUID (m4char_t *ao_pszUUID, m4uint32_t ai_iStrSize);
M4_DECL_M4UUID void	puid(uuid_t u, m4char_t *result);

#endif
