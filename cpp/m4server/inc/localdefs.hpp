#ifndef _SERVER_LOCAL_DEFS_H_
#define _SERVER_LOCAL_DEFS_H_

//////////////////////////////////////////////////////////////////////////////
//// Funciones locales
//////////////////////////////////////////////////////////////////////////////


#define M4_SERVER_LOCAL_VERSION_STR ""


// isAlreadyRunning
// return if a instance of this process is already running from the current directory
// return   1 if there is another instance.
//          0 othewise.
int isAlreadyRunning();

#endif
