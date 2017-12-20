// serverprocess.cpp


#include <stdio.h>
#include "m4types.hpp"
#include "m4trace.hpp"

#ifdef _WINDOWS
#   include <windows.h>
#	include <psapi.h>
#	include <tlhelp32.h>
#else
#   ifdef _SOLARIS
#       include <procfs.h>
#   else
#       include <sys/procfs.h>
#   endif
#   include <unistd.h>
    typedef unsigned long   DWORD;
#endif


/**
 * return the current process pid.
 */
DWORD GetPid()
{
#ifdef _WINDOWS
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}


/**
 * return the current process startup time.
 */
#ifdef _WIN32

DWORD GetStartupTime(HANDLE ai_hProcess)
{
    FILETIME ct;    // created time
    FILETIME unused;
    GetProcessTimes(ai_hProcess , &ct, &unused, &unused, &unused);

    return ct.dwLowDateTime;
}

#else

DWORD GetStartupTime(DWORD ai_pid)
{
    DWORD retCode = 0;

    // open psinfo file.
    char filename[1024];
    sprintf (filename, "/proc/%lu/psinfo", ai_pid);
	// UNICODE FILE revised
    FILE* fd = fopen(filename, "rb");
    if (NULL == fd)
    {
        m4Trace(cout << "process: " << filename << " does not exist." << endl);
        return 0;
    }

    psinfo psi;
    int nitems = fread(&psi, sizeof(psinfo), 1, fd);
    if (0 == nitems)
    {
        m4Trace(cout << "Cannot read file " << filename <<  endl);
        retCode = 0;
    }
    else
    {
        retCode = psi.pr_start.tv_sec;
    }

    fclose(fd);
    return retCode;
}

#endif



/**
 * Checks if another copy of current program is already running.
 *
 * @return 
 *  1 if another copy of current program is already running
 *  0 otherwise.
 */
int processExist(DWORD ai_pid, DWORD ai_startupTime)
{
    int retCode = 0;

#ifdef _WINDOWS
	// Bug 142868. Changes in the way to find the PID
	PROCESSENTRY32 pe32;

	// Get a Snapshot handle to list all processes
	HANDLE  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if (INVALID_HANDLE_VALUE == hProcessSnap)
	{
		m4Trace(cout << "Error gathering process information." << endl);
	}
	else
	{
		pe32.dwSize = sizeof(pe32);

		// Get first process
		if ( ! Process32First(hProcessSnap, &pe32) )
		{
			m4Trace(cout << "Error gathering first process information." << endl);
		}
		else
		{
			// Examine all processes until the PID is found or the list ended
			while ((pe32.th32ProcessID != ai_pid) && Process32Next(hProcessSnap, &pe32));

			if (pe32.th32ProcessID == ai_pid)
			{
				// open process
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ai_pid);
				// hProcess shouldn't be NULL because it is in the process list
				if (NULL != hProcess)
				{
					// get creation time.
					DWORD ct = GetStartupTime(hProcess);
					if (ct == ai_startupTime)
					{
						m4Trace(cout << "process: " << ai_pid << " with startup time " << ai_startupTime << " exist." << endl);
						retCode = 1;
					}
					else
					{
						m4Trace(cout << "process: " << ai_pid << " exist, but startuptime is different (current-saved): " << ct << " - " << ai_startupTime << endl);
						retCode = 0;
					}
					CloseHandle (hProcess);
				}
			}
			else
			{
				m4Trace(cout << "process: " << ai_pid << " does not exist." << endl);
			}
		}
		CloseHandle (hProcessSnap);
	}


#else

    // open psinfo file.
    char filename[1024];
    sprintf (filename, "/proc/%lu/psinfo", ai_pid);
	// UNICODE FILE revised
    FILE* fd = fopen(filename, "rb");
    if (NULL == fd)
    {
        m4Trace(cout << "process: " << ai_pid << " does not exist." << endl);
        return 0;
    }
    fclose(fd);

    // get creation time.
    DWORD ct = GetStartupTime(ai_pid);
    if (ct == ai_startupTime)
    {
        m4Trace(cout << "process: " << ai_pid << " with startup time " << ai_startupTime << " exist." << endl);
        retCode = 1;
    }
    else
    {
        m4Trace(cout << "process: " << ai_pid << " exist, but startuptime is different (current-saved): " << ct << " - " << ai_startupTime << endl);
        retCode = 0;
    }

#endif

    return retCode;
}



/**
 * Checks if another copy of current program is already running.
 *
 * @return 
 *  1 if another copy of current program is already running
 *  0 otherwise.
 */
int isAlreadyRunning()
{
    const char* filename = "pid";
    FILE* fd;

    DWORD pid = 0;
    DWORD startupTime = 0;

    // if file exist, try to delete it.
	// UNICODE FILE revised
    fd = fopen(filename, "r");
    if (NULL != fd)
    {
        // the file exist. read pid and startup time;
        fscanf(fd, "%lu", &pid);
        m4Trace(cout << "pid: "<< pid << endl);
        fscanf(fd, "%lu", &startupTime);
        m4Trace(cout << "startupTime: "<< startupTime << endl);
        fclose(fd);

        if (processExist(pid, startupTime))
        {
            return 1;
        }
    }

    // open a file to write the current pid.
	// UNICODE FILE revised
    fd = fopen(filename, "w");
    if (NULL == fd)
    {
        // cannot create the file. I don't known if there is another process.
        return 0;
    }

    // writes the current pid in the file.
    pid = GetPid();
#ifdef _WINDOWS
    startupTime = GetStartupTime(GetCurrentProcess());
#else
    startupTime = GetStartupTime(pid);
#endif
    fprintf(fd, "%lu %lu\n", pid, startupTime);

    // close the file.
    fclose(fd);

    return 0;
}

