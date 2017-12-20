
#ifndef M4REQUEST_DLL_HPP
#define M4REQUEST_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4REQUEST
			#define M4_DECL_M4REQUEST __declspec(dllexport)
	#else
			#define M4_DECL_M4REQUEST __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4REQUEST
#endif
#endif

