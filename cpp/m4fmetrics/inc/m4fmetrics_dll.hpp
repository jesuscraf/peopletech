
#ifndef M4FMETRICS_DLL_HPP
#define M4FMETRICS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4FMETRICS
			#define M4_DECL_M4FMETRICS __declspec(dllexport)
	#else
			#define M4_DECL_M4FMETRICS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4FMETRICS
#endif
#endif

