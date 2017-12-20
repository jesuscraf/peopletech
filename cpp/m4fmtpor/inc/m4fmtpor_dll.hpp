
#ifndef M4FMTPOR_DLL_HPP
#define M4FMTPOR_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4FMTPOR
			#define M4_DECL_M4FMTPOR __declspec(dllexport)
	#else
			#define M4_DECL_M4FMTPOR __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4FMTPOR
#endif
#endif

