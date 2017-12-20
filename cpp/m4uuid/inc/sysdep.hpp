
/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <sysdep.hpp>                                         |
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

#ifndef _SYSDEP__HPP_
#define _SYSDEP__HPP_

/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/

#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )
	#include <windows.h>
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/time.h>
	#include <sys/sysinfo.h>
	#include <netinet/in.h>
#endif

//--- MD5 sample implementation from RFC 1321
#include "md5.hpp"

/*
+-------------------------------------------------------------------------+
| Public Symbols                                                          |
+-------------------------------------------------------------------------+
*/

//--- Set the following to the number of 100ns ticks of the actual resolution of
//--- your system's clock.

#define UUIDS_PER_TICK 1024

//--- Set the following to a call to acquire a system wide global lock
#define LOCK
#define UNLOCK


//--- Set this to what your compiler uses for 64 bit data type.
#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )
	#define unsigned64_t unsigned __int64
	#define I64(C) C
#else
	#define unsigned64_t unsigned long long
	#define I64(C) C##LL
#endif

/*
+-------------------------------------------------------------------------+
| Public Types/Classes/Variables                                          |
+-------------------------------------------------------------------------+
*/

typedef m4uint64_t uuid_time_t;
	
typedef struct 
{
	char nodeID[6];
} uuid_node_t;

#define M4CL_UUID_NODEID_PATH "nodeid"
#define M4CL_UUID_MAX_DEBUG_DIRECTORY 500
// backslash
#ifdef _UNIX
#define M4_UUID_BACK_SLASH	"/"
#else
#define M4_UUID_BACK_SLASH	"\\"
#endif

#define M4_UUID_INIT_TEMP_DIR				                "TempDir"
#define M4_UUID_NEW_REGEDIT_KEY_CVM							"CVM"

/*
+-------------------------------------------------------------------------+
| Public Functions                                                        |
+-------------------------------------------------------------------------+
*/
void get_ieee_node_identifier		(uuid_node_t* node);
void get_system_time				(uuid_time_t* uuid_time);
void get_random_info				(char seed[16]);

int get_debug_dir_file				(char *ao_pszDebugDirFile, char*ai_pszFileName, int ai_iSize);
#endif
