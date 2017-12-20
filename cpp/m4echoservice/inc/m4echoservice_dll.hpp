
#ifndef M4ECHOSERVICE_DLL_HPP
#define M4ECHOSERVICE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4ECHOSERVICE
			#define M4_DECL_M4ECHOSERVICE __declspec(dllexport)
	#else
			#define M4_DECL_M4ECHOSERVICE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4ECHOSERVICE
#endif
#endif

