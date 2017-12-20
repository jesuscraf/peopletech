
#ifndef M4CREDENTIALS_DLL_HPP
#define M4CREDENTIALS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CREDENTIALS
			#define M4_DECL_M4CREDENTIALS __declspec(dllexport)
	#else
			#define M4_DECL_M4CREDENTIALS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CREDENTIALS
#endif
#endif

