
#ifndef M4OBJREG_DLL_HPP
#define M4OBJREG_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OBJREG
			#define M4_DECL_M4OBJREG __declspec(dllexport)
	#else
			#define M4_DECL_M4OBJREG __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OBJREG
#endif
#endif

