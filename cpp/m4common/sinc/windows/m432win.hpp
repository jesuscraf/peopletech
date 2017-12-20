#ifndef __M432WIN_HPP__ 
#define __M432WIN_HPP__ 


#define huge    
#define _export CALLBACK
#define __export


#define t_PID       DWORD
#define NULL_PID    NULL

#define t_PROCESS       HANDLE
#define NULL_PROCESS    NULL

#define GetCurrentTask() 	( GetCurrentProcessId() )
#define GetCurrentProcess() 	( GetCurrentProcess() )
#define hmemcpy          	( memcpy )



#endif

