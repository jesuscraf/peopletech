
#ifndef M4LOG_DLL_HPP
#define M4LOG_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LOG
			#define M4_DECL_M4LOG __declspec(dllexport)
	#else
			#define M4_DECL_M4LOG __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LOG
#endif
#endif

