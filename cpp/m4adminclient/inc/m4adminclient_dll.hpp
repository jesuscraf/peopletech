
#ifndef M4ADMINCLIENT_DLL_HPP
#define M4ADMINCLIENT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4ADMINCLIENT
			#define M4_DECL_M4ADMINCLIENT __declspec(dllexport)
	#else
			#define M4_DECL_M4ADMINCLIENT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4ADMINCLIENT
#endif
#endif

