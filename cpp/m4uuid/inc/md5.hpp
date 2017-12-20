/*
+-------------------------------------------------------------------------+
| Copyright (C) 1991-2, RSA Data Security, Inc.                           |
| Created 1991. All rights reserved.                                      | 
+-------------------------------------------------------------------------+
| License to copy and use this software is granted provided that it       |
| is identified as the "RSA Data Security, Inc. MD5 Message-Digest        |
| Algorithm" in all material mentioning or referencing this software      |
| or this function.													                 |
+-------------------------------------------------------------------------+
| License is also granted to make and use derivative works provided       |
| that such works are identified as "derived from the RSA Data            |
| Security, Inc. MD5 Message-Digest Algorithm" in all material            |
| mentioning or referencing the derived work.                             |
+-------------------------------------------------------------------------+
| RSA Data Security, Inc. makes no representations concerning either      |
| the merchantability of this software or the suitability of this         |
| software for any particular purpose. It is provided "as is"             |              
| without express or implied warranty of any kind.                        |
+-------------------------------------------------------------------------+
| These notices must be retained in any copies of any part of this        |
| documentation and/or software.                                          |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <md5.hpp>                                            |
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
#ifndef _MD5__HPP_
#define _MD5__HPP_


/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/
#include "m4types.hpp"


/*
+-------------------------------------------------------------------------+
| Public Types/Classes/Variables                                          |
+-------------------------------------------------------------------------+
*/

//--- MD5 context. 
typedef struct {
  m4uint32_t state[4];        // state (ABCD). 
  m4uint32_t count[2];        // number of bits, modulo 2^64 (lsb first). 
  unsigned char buffer[64];   // input buffer. 
} MD5_CTX;


/*
+-------------------------------------------------------------------------+
| Public Functions                                                        |
+-------------------------------------------------------------------------+
*/

void MD5Init	(MD5_CTX*);
void MD5Update	(MD5_CTX*, unsigned char*, unsigned int);
void MD5Final	(unsigned char[16], MD5_CTX*);

#endif
