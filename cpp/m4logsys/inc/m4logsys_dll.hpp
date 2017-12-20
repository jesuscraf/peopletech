
#ifndef M4LOGSYS_DLL_HPP
#define M4LOGSYS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LOGSYS
			#define M4_DECL_M4LOGSYS	__declspec(dllexport)
	#else
			#define M4_DECL_M4LOGSYS	__declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LOGSYS
#endif
#endif

