
#ifndef M4SNMPADMIN_DLL_HPP
#define M4SNMPADMIN_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SNMPADMIN
			#define M4_DECL_M4SNMPADMIN __declspec(dllexport)
	#else
			#define M4_DECL_M4SNMPADMIN __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SNMPADMIN
#endif
#endif

