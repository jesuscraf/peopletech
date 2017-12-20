/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
| Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.           |
| Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &         |
| Digital Equipment Corporation, Maynard, Mass.                           |
| Copyright (c) 1998 Microsoft.                                           |
| To anyone who acknowledges that this file is provided "AS IS"           |
| without any express or implied warranty: permission to use, copy,       |
| modify, and distribute this file for any purpose is hereby              |
| granted without fee, provided that the above copyright notices and      |
| this notice appears in all source code copies, and that none of         |
| the names of Open Software Foundation, Inc., Hewlett-Packard            |
| Company, or Digital Equipment Corporation be used in advertising        |
| or publicity pertaining to distribution of the software without         |
| specific, written prior permission.  Neither Open Software              |
| Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital       |
| Equipment Corporation makes any representations about the suitability   |
| of this software for any purpose.                                       |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] Meta4 UUID Generator                        |
| File             : <uuid.hpp>                                           |
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
|                                                                         |
+-------------------------------------------------------------------------+
*/
#ifndef _UUID__HPP_
#define _UUID__HPP_

/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/
#include "sysdep.hpp"


/*
+-------------------------------------------------------------------------+
| Public Constants                                                        |
+-------------------------------------------------------------------------+
*/
#define M4CL_UUID_STATE_PATH "m4uuidstate"

/*
+-------------------------------------------------------------------------+
| Public Types/Classes/Variables                                          |
+-------------------------------------------------------------------------+
*/
#undef uuid_t

typedef struct _uuid_t
{
	m4uint32_t          time_low;
	m4uint16_t          time_mid;
	m4uint16_t          time_hi_and_version;
	m4uint8_t           clock_seq_hi_and_reserved;
	m4uint8_t           clock_seq_low;
	m4uint8_t           node[6];
} uuid_t;


/*
+-------------------------------------------------------------------------+
| Public Functions                                                        |
+-------------------------------------------------------------------------+
*/

//--- Generate a UUID.
int uuid_create(uuid_t* uuid);

//--- Create a UUID using a "name" from a "name space".
void uuid_create_from_name ( uuid_t* uuid,	// Resulting UUID.
									  uuid_t  nsid,	// UUID to serve as context, so identical
															// names from different name spaces generate
															// different UUIDs.
									  void*   name,	// The name from which to generate a UUID.
									  int     namelen	// The length of the name.
	  );

//--- Compare two UUID's "lexically" and return.
//--- -1   u1 is lexically before u2
//---  0   u1 is equal to u2
//---  1   u1 is lexically after u2
//--- Note:   lexical ordering is not temporal ordering!
int uuid_compare (uuid_t* u1, uuid_t* u2);


#endif
