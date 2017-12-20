
#ifndef M4OBJGLB_DLL_HPP
#define M4OBJGLB_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OBJGLB
			#define M4_DECL_M4OBJGLB __declspec(dllexport)
	#else
			#define M4_DECL_M4OBJGLB __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OBJGLB
#endif
#endif

