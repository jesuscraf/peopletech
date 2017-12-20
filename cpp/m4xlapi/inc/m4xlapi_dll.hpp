
#ifndef M4XLAPI_DLL_HPP
#define M4XLAPI_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4XLAPI
			#define M4_DECL_M4XLAPI __declspec(dllexport)
	#else
			#define M4_DECL_M4XLAPI __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4XLAPI
#endif
#endif

