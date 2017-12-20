
#include <fileutil.hpp>	
#include "m4unicode.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _SOLARIS
#include <sys/resource.h>
#endif




void SetFileDescriptorsLimit( m4uint32_t ai_iLimit )
{

#if defined (_SOLARIS)

	struct rlimit rlim;

	rlim.rlim_cur = ai_iLimit; /* current (soft) limit */
	rlim.rlim_max = ai_iLimit; /* hard limit */

	setrlimit( RLIMIT_NOFILE , &rlim); 
#endif					
}


m4return_t GetSizeFile ( m4pchar_t ai_szFile , m4uint32_t & ao_iFileLen )
{
	struct stat buf;

	// UNICODE FILE
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( ai_szFile, iLength ) ;
	int ret = stat(pcFileName, &buf) ;
	delete [] pcFileName ;

	if (ret == -1)
	{
		return M4_ERROR ;
	}

	ao_iFileLen = buf.st_size;
	
	return M4_SUCCESS;
}

