
#ifndef M4HSPARSER_DLL_HPP
#define M4HSPARSER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4HSPARSER
			#define M4_DECL_M4HSPARSER __declspec(dllexport)
	#else
			#define M4_DECL_M4HSPARSER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4HSPARSER
#endif
#endif

