
#ifndef M4MAILNET_DLL_HPP
#define M4MAILNET_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MAILNET
			#define M4_DECL_M4MAILNET __declspec(dllexport)
	#else
			#define M4_DECL_M4MAILNET __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MAILNET
#endif
#endif

