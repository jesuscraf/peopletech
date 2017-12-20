/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <sysdep.cpp>                                         |
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
#include <stdio.h>
#include <string.h>
#include "sysdep.hpp"
#include "regedit.hpp"
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "m4regdef.hpp"


/*
+-------------------------------------------------------------------------+
| Functions Implementation                                                |
+-------------------------------------------------------------------------+
*/

//--- System dependent call to get IEEE node ID.
//--- This sample implementation generates a random node ID.

void get_ieee_node_identifier (uuid_node_t* node)
{
	char   seed[16];
	FILE*  fd=0;
	static int inited = 0;
	static uuid_node_t saved_node;

	if (!inited)
	{
		 char acNodeIdPath[M4CL_UUID_MAX_DEBUG_DIRECTORY];
				 
		 if(get_debug_dir_file(acNodeIdPath,M4CL_UUID_NODEID_PATH ,M4CL_UUID_MAX_DEBUG_DIRECTORY) ){
			//si no podemos leer->  a capon /tmp/nodeid
			 strcpy(acNodeIdPath,M4_UUID_BACK_SLASH);
			 strcat(acNodeIdPath,"tmp");
			 strcat(acNodeIdPath,M4_UUID_BACK_SLASH);
			 strcat(acNodeIdPath,M4CL_UUID_NODEID_PATH);
		 }

    	 // UNICODE FILE
    	 eUniFileEncodingType_t eEncoding = M4UniANSI ;
    	 fd = M4Fopen(acNodeIdPath, M4UniReadBinary, eEncoding);

		 if (fd)
		 {
				fread(&saved_node, sizeof(uuid_node_t), 1, fd);
				fclose(fd);
		 }
		 else
		 {
				get_random_info(seed);
				seed[0] |= 0x80;
				memcpy(&saved_node, seed, sizeof(uuid_node_t));
				// UNICODE FILE
				eEncoding = M4UniANSI ;
				fd = M4Fopen(acNodeIdPath, M4UniWriteBinary, eEncoding);
				if (fd)
				{
						 fwrite(&saved_node, sizeof(uuid_node_t), 1, fd);
						 fclose(fd);
				}
		 }
		 inited = 1;
	}
	*node = saved_node;
}


//--- System dependent call to get the current system time.
//--- Returned as 100ns ticks since Oct 15, 1582, but resolution may be
//--- less than 100ns.

#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )

void get_system_time (uuid_time_t* uuid_time) 
{
	ULARGE_INTEGER time;

	GetSystemTimeAsFileTime((FILETIME *)&time);

	/* NT keeps time in FILETIME format which is 100ns ticks since
	Jan 1, 1601.  UUIDs use time in 100ns ticks since Oct 15, 1582.
	The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
	+ 18 years and 5 leap days. */

	time.QuadPart +=
		  (unsigned __int64) (1000*1000*10)       // seconds
		* (unsigned __int64) (60 * 60 * 24)       // days
		* (unsigned __int64) (17+30+31+365*18+5); // # of days

	*uuid_time = time.QuadPart;
};

void get_random_info (char seed[16]) 
{
	MD5_CTX c;

	typedef struct
	{
		MEMORYSTATUS m;
		SYSTEM_INFO s;
		FILETIME t;
		LARGE_INTEGER pc;
		DWORD tc;
		DWORD l;
		char hostname[MAX_COMPUTERNAME_LENGTH + 1];
	} randomness;

	randomness r;

	MD5Init(&c);

	// memory usage stats.
	GlobalMemoryStatus(&r.m);

	// random system stats.
	GetSystemInfo(&r.s);

	// 100ns resolution (nominally) time of day.
	GetSystemTimeAsFileTime(&r.t);

	// high resolution performance counter.
	QueryPerformanceCounter(&r.pc);

	// milliseconds since last boot.
	r.tc = GetTickCount();
	r.l = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(r.hostname, &r.l );
	MD5Update(&c, (m4puchar_t) &r, sizeof(randomness));
	MD5Final((m4puchar_t) seed, &c);
}

#else

#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

void get_system_time (uuid_time_t* uuid_time)
{
	struct timeval tp;

	gettimeofday(&tp, (struct timezone *)0);

  /* Offset between UUID formatted times and Unix formatted times.
     UUID UTC base time is October 15, 1582.
     Unix base time is January 1, 1970.  */
	*uuid_time = (tp.tv_sec * 10000000) + (tp.tv_usec * 10) +
   I64(0x01B21DD213814000);
}

void get_random_info(char seed[16]) 
{
	MD5_CTX c;
	typedef struct
	{
		char s[257];
		long r;
		struct timeval t;
		char hostname[257];
	} randomness;

	randomness r;
    struct utsname sysinfo;

	MD5Init(&c);
	//sysinfo(SI_HW_SERIAL,r.s,257);
	if (uname (&sysinfo)!= -1){
		strcpy (r.s,sysinfo.nodename);
	}
	r.r = random();
	gettimeofday(&r.t, (struct timezone *)0);
	gethostname(r.hostname, 256);
	MD5Update(&c, (unsigned char *) &r, sizeof(randomness));
	MD5Final((unsigned char *) seed, &c);
}

#endif

//0=OK, <0 -> Error
int get_debug_dir_file	(char *ao_pszDebugDirFile, char*ai_pszFileName, int ai_iSize ){

	// Obtenemos el directorio de temporal
    m4char_t    acPathReg[ M4CL_UUID_MAX_DEBUG_DIRECTORY + 1 ] ;
	char		pszPath[ M4CL_UUID_MAX_DEBUG_DIRECTORY + 1 ] ;
	m4uint32_t  keySize;

	M4ComposeRegistryPath(M4_NEW_REGEDIT_KEY_SERVER, M4_UUID_NEW_REGEDIT_KEY_CVM, acPathReg, M4CL_UUID_MAX_DEBUG_DIRECTORY, keySize ) ;
    memset( pszPath, '\0', M4CL_UUID_MAX_DEBUG_DIRECTORY + 1 ) ;
	
	if( GetRegKeyData( acPathReg, M4_UUID_INIT_TEMP_DIR, pszPath ) == FALSE )
    {
        *ao_pszDebugDirFile = '\0' ;
		return -1;
	}

	
	int iSize=strlen(pszPath)+2+strlen(ai_pszFileName); // path+slash+filename+null_terminator
	
	if (ai_iSize<iSize){
		return -1;
	}

	strcpy(ao_pszDebugDirFile,pszPath); //directorio
	strcat(ao_pszDebugDirFile,M4_UUID_BACK_SLASH);   // /
	strcat(ao_pszDebugDirFile,ai_pszFileName); //file
	return 0;
}
