
#ifndef M4XMLUTIL_DLL_HPP
#define M4XMLUTIL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4XMLUTIL
			#define M4_DECL_M4XMLUTIL __declspec(dllexport)
	#else
			#define M4_DECL_M4XMLUTIL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4XMLUTIL
#endif
#endif

