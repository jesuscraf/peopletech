
#ifndef M4UNICODE_DLL_HPP
#define M4UNICODE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4UNICODE
			#define M4_DECL_M4UNICODE __declspec(dllexport)
	#else
			#define M4_DECL_M4UNICODE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4UNICODE
#endif
#endif

