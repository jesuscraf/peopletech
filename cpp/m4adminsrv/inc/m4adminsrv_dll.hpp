
#ifndef M4ADMINSRV_DLL_HPP
#define M4ADMINSRV_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4ADMINSRV
			#define M4_DECL_M4ADMINSRV __declspec(dllexport)
	#else
			#define M4_DECL_M4ADMINSRV __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4ADMINSRV
#endif
#endif

