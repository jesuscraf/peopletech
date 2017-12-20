
#ifndef M4ENGPRN_DLL_HPP
#define M4ENGPRN_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4ENGPRN
			#define M4_DECL_M4ENGPRN __declspec(dllexport)
	#else
			#define M4_DECL_M4ENGPRN __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4ENGPRN
#endif
#endif

