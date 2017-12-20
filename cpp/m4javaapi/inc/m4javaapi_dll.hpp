
#ifndef M4JAVAAPI_DLL_HPP
#define M4JAVAAPI_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JAVAAPI
			#define M4_DECL_M4JAVAAPI __declspec(dllexport)
	#else
			#define M4_DECL_M4JAVAAPI __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JAVAAPI
#endif
#endif

