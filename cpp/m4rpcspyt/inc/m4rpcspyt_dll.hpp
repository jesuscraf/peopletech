
#ifndef M4RPCSPYT_DLL_HPP
#define M4RPCSPYT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4RPCSPYT
			#define M4_DECL_M4RPCSPYT __declspec(dllexport)
	#else
			#define M4_DECL_M4RPCSPYT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4RPCSPYT
#endif
#endif

