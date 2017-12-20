
#ifndef M4TRANSPORT_DLL_HPP
#define M4TRANSPORT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4TRANSPORT
			#define M4_DECL_M4TRANSPORT __declspec(dllexport)
	#else
			#define M4_DECL_M4TRANSPORT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4TRANSPORT
#endif
#endif

