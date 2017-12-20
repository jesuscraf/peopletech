
#ifndef M4MDTRANSFER_DLL_HPP
#define M4MDTRANSFER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MDTRANSFER
			#define M4_DECL_M4MDTRANSFER __declspec(dllexport)
	#else
			#define M4_DECL_M4MDTRANSFER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MDTRANSFER
#endif
#endif

