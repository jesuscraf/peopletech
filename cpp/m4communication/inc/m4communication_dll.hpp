
#ifndef M4COMMUNICATION_DLL_HPP
#define M4COMMUNICATION_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4COMMUNICATION
			#define M4_DECL_M4COMMUNICATION __declspec(dllexport)
	#else
			#define M4_DECL_M4COMMUNICATION __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4COMMUNICATION
#endif
#endif

