
#ifndef M4EXEFORCHANNEL_DLL_HPP
#define M4EXEFORCHANNEL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4EXEFORCHANNEL
			#define M4_DECL_M4EXEFORCHANNEL __declspec(dllexport)
	#else
			#define M4_DECL_M4EXEFORCHANNEL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4EXEFORCHANNEL
#endif
#endif

