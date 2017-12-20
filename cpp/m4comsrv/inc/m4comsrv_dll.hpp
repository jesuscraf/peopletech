
#ifndef M4COMSRV_DLL_HPP
#define M4COMSRV_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4COMSRV
			#define M4_DECL_M4COMSRV __declspec(dllexport)
	#else
			#define M4_DECL_M4COMSRV __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4COMSRV
#endif
#endif

