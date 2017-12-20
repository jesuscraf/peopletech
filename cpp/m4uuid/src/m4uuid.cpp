/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <m4uuid.cpp>                                         |
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

/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/

#include "m4uuid.hpp"

// Base 64 header.
#include "_base64.h"
/*
+-------------------------------------------------------------------------+
| Private Constants                                                       |
+-------------------------------------------------------------------------+
*/

//--- Version String for some platforms
static m4char_t *M4_versionString = "@(#) m4uuid 31.170.1202";

#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )

//--- 
//--- WINDOWS SPECIFIC GUID ALGORITHM IMPLEMENTATION
//--- 

#include <objbase.h>
#include <rpcdce.h>

m4return_t GetUUID (m4char_t *ao_pszUUID, m4uint32_t ai_iStrSize){
	
	m4return_t	ret_value=M4_SUCCESS;
	GUID        guid;  
	
	ret_value = M4_SUCCESS;
	   
	CoCreateGuid(&guid);
	  
/*	m4uchar_t   *pResult;
	m4uint32_t  strSize;

	if (UuidToString(&guid,&pResult) == RPC_S_OK){
		
		strSize=strlen((const m4char_t*)pResult);
		
	//si no han reservado aún, o pasan poco, nos llaman para saber el tamaño, pues se lo damos
		if (ai_iStrSize<strSize+1 || !ao_pszUUID){ //si no cabe
			ret_value = M4_ERROR;
		}
		else{
			strcpy ( ao_pszUUID,(const m4char_t*)pResult) ;
			ret_value = M4_SUCCESS;
		}
		RpcStringFree(&pResult);
		
	}
	else {
		ret_value = M4_ERROR;
	}
	*/

	if (ai_iStrSize<M4CL_VM_SIZE_UUID+1 || !ao_pszUUID){ //si no cabe
		ret_value = M4_ERROR;
	}
	else{
		
		// Copy guid as raw data into a binary string.
		m4int8_t len1  = sizeof(guid.Data1);
		m4int8_t len2  = sizeof(guid.Data2);
		m4int8_t len3  = sizeof(guid.Data3);
		m4int8_t len41 = sizeof(guid.Data4[1]);
		m4int8_t len = len1+len2+len3+8*len41;
		m4pchar_t pBin = new char[len];
		
		memcpy(pBin,&guid.Data1,len1); 
		memcpy(pBin+len1,&guid.Data2,len2); 
		memcpy(pBin+len1+len2,&guid.Data3,len3); 
		for (m4int_t k=0;k<8;k++){
			memcpy(pBin+len1+len2+len3+k*len41,&guid.Data4[k],len41); 
		}
		
		C_Base64 oBase;
		m4pchar_t pcCoded = 0;
		oBase.Code(pBin,len,pcCoded);
		strcpy (ao_pszUUID,pcCoded);
		// Free memory.
		free (pcCoded); //lo alocan en el objeto y hay que liberarlo aqui! Si!! 
		delete []pBin;

	}

    return ret_value;
}

#else

//--- 
//--- OTHER PLATFORMS SPECIFIC GUID ALGORITHM IMPLEMENTATION
//--- 

#include "uuid.hpp"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
//--- Name used for UUID name-based algorithm.
   const m4char_t *lpsz_Name = "www.meta4.com";  
   const m4char_t iNameLength = 13;

   //--- DNS Context used for UUID name-based algorithm
   uuid_t NameSpace_DNS = 
   { 
	   /* 6ba7b810-9dad-11d1-80b4-00c04fd430c8 */
	   0x6ba7b810, 0x9dad, 0x11d1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 
   };

m4return_t GetUUID (m4char_t *ao_pszUUID, m4uint32_t ai_iStrSize){
	   
	m4return_t	ret_value=M4_SUCCESS;
	uuid_t		u;

 
	//--- If you want a name-based UUID just remove the remark on this line and
	//--- coment the other.
	//--- uuid_create_from_name(&u, NameSpace_DNS, (void *)lpsz_Name, iNameLength);
	
	//si no han reservado aún, o pasan poco, nos llaman para saber el tamaño, pues se lo damos
	
	if (ai_iStrSize<M4CL_VM_SIZE_UUID+1 || !ao_pszUUID){ //si no cabe
		ret_value = M4_ERROR;
	}
	else{
		uuid_create(&u);
		puid(u, ao_pszUUID );
		ret_value = M4_SUCCESS;
	}
	
	return ret_value;
}


void puid(uuid_t u, m4char_t *result) {

		// Copy uuid as raw data into a binary string.
	m4int8_t len1  = sizeof(u.time_low);
	m4int8_t len2  = sizeof(u.time_mid);
	m4int8_t len3  = sizeof(u.time_hi_and_version);
	m4int8_t len4  = sizeof(u.clock_seq_hi_and_reserved);
	m4int8_t len5  = sizeof(u.clock_seq_low);
	m4int8_t len61 = sizeof(u.node[1]);
	m4int8_t len = len1+len2+len3+len4+len5+6*len61;
	m4pchar_t pBin = new char[len];

	memcpy(pBin,&u.time_low,len1);
	memcpy(pBin+len1,&u.time_mid,len2);
	memcpy(pBin+len1+len2,&u.time_hi_and_version,len3);
	memcpy(pBin+len1+len2+len3,&u.clock_seq_hi_and_reserved,len4);
	memcpy(pBin+len1+len2+len3+len4,&u.clock_seq_low,len5);
	for(m4int_t k=0;k<6;k++){
		memcpy(pBin+len1+len2+len3+len4+len5+k*len61,&u.node[k],len61);	
	}


	// Common code... no matter how the binary pointer was obtained, we code it.
	C_Base64 oBase;
	m4pchar_t pcCoded= 0;
	oBase.Code(pBin,len,pcCoded);
   	strcpy (result,pcCoded);
	
	//freedom
	free (pcCoded); //lo alocan en el objeto y hay que liberarlo aqui! Si!! 
	delete []pBin;

	/*sprintf(result, 
		     "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x", 
		     u.time_low, u.time_mid,
		     u.time_hi_and_version, u.clock_seq_hi_and_reserved,
		     u.clock_seq_low,u.node[0],u.node[1],u.node[2],u.node[3],u.node[4],u.node[5]);
	*/
}

#endif
