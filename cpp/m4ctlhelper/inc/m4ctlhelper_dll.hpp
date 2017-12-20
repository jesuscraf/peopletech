
#ifndef M4CTLHELPER_DLL_HPP
#define M4CTLHELPER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CTLHELPER
			#define M4_DECL_M4CTLHELPER __declspec(dllexport)
	#else
			#define M4_DECL_M4CTLHELPER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CTLHELPER
#endif
#endif

