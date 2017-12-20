
#ifndef M4JSEXEJOB_DLL_HPP
#define M4JSEXEJOB_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JSEXEJOB
			#define M4_DECL_M4JSEXEJOB __declspec(dllexport)
	#else
			#define M4_DECL_M4JSEXEJOB __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JSEXEJOB
#endif
#endif

