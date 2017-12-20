
#ifndef M4TSAP_DLL_HPP
#define M4TSAP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4TSAP
			#define M4_DECL_M4TSAP __declspec(dllexport)
	#else
			#define M4_DECL_M4TSAP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4TSAP
#endif
#endif

