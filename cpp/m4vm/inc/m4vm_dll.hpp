
#ifndef M4VM_DLL_HPP
#define M4VM_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4VM
			#define M4_DECL_M4VM __declspec(dllexport)
	#else
			#define M4_DECL_M4VM __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4VM
#endif
#endif

