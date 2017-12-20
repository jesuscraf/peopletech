
#ifndef M4PARSER_DLL_HPP
#define M4PARSER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4PARSER
			#define M4_DECL_M4PARSER __declspec(dllexport)
	#else
			#define M4_DECL_M4PARSER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4PARSER
#endif
#endif

