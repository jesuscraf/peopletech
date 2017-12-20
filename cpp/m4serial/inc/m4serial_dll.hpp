
#ifndef M4SERIAL_DLL_HPP
#define M4SERIAL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SERIAL
			#define M4_DECL_M4SERIAL __declspec(dllexport)
	#else
			#define M4_DECL_M4SERIAL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SERIAL
#endif
#endif

