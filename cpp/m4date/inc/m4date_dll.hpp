
#ifndef M4DATE_DLL_HPP
#define M4DATE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4DATE
			#define M4_DECL_M4DATE __declspec(dllexport)
	#else
			#define M4_DECL_M4DATE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4DATE
#endif
#endif

