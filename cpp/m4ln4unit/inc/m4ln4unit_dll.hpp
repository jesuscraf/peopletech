
#ifndef M4LN4UNIT_DLL_HPP
#define M4LN4UNIT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LN4UNIT
			#define M4_DECL_M4LN4UNIT __declspec(dllexport)
	#else
			#define M4_DECL_M4LN4UNIT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LN4UNIT
#endif
#endif

