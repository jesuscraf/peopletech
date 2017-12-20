//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4win2ux
// File:                m4win2ux.inl
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                04-11-1998
// Language:            C++
// Operating System:    WINDOWS, UNIX (Solaris, HP-UX)
// Design Document:     XXX.DOC
//
//
// Definition:
//     Implementation of inline Windows functions in Unix
//
//==============================================================================

#include <errno.h>

// ************************************ //
// M4 - Functions						//
// ************************************ //

// #################################################
// ##   FUNCIONES DE MANIPULACION DE DIRECTORIOS  ##
// #################################################

// Las siguientes funciones pueden ser utilizadas:
// int M4_mkdir(const char *dirname, int &ao_iError);
// int M4_gethostname(char *name, int namelen);


#define M4_WIN2UX_MAX_LONG_NUMBER	256

// ############################
// Mapeo de algunas Funciones
// ############################

inline M4_DECL_M4WIN2UX int M4_gethostname(char *name, int namelen) {
#ifdef _UNIX
	#if defined(SVR4) || defined(_SOLARIS_2_5_1)
			return ( (sysinfo(SI_HOSTNAME, name, namelen)<0) ? -1 : 0) ;
	#else	//BSD
			return gethostname(name, namelen);
	#endif	//SVR4	
#else
	return gethostname(name, namelen);
#endif
}




// #################################################
// ##   OTRAS FUNCIONES							  ##
// #################################################

#ifdef _UNIX
// Flushes all streams; 
// clears all buffers.
inline M4_DECL_M4WIN2UX int flushall()
{
	return fflush(NULL);	// all files open for writing are flushed
}
#endif //_UNIX

#ifdef _UNIX
// Convert an integer to a string
// In:	value	= Number to be converted
// Out:	string	= String result
// In:	radix	= Base of value; must be in the range 2 - 36
inline M4_DECL_M4WIN2UX char* itoa(int value, char *string, int radix)
{
	char *pcNumberInAscii;

	// Check radix value
	if (radix<2 || radix>36)
		radix = 10;	//Default base 10

#if defined (_M4HPUX1020_)
	// HP-UX 10.20
	// int ltostr_r(long value, int radix, char *buffer, int buflen); 
	// lltostr()'s behavior is undefined if value is negative
	char buffer[M4_WIN2UX_MAX_LONG_NUMBER];
	memset( (void *)buffer, 0, M4_WIN2UX_MAX_LONG_NUMBER );		// Set 0's in buffer
	ltostr_r( value, radix, buffer, M4_WIN2UX_MAX_LONG_NUMBER);	// Re-entrant version	
	pcNumberInAscii = buffer;
	strcpy(string, pcNumberInAscii);
#elif defined(_M4HPUX1100_)
	// HP-UX 11.xx
	// char *ltostr(long value, int radix); 
	// lltostr()'s behavior is undefined if value is negative
	pcNumberInAscii = ltostr( value, radix);	// Re-entrant version
	strcpy(string, pcNumberInAscii);
#elif defined(_M4SOLSPARC_) || defined(_M4SOLX86_) 
	// Solaris Sparc e Intel
	// Warning radix must be 10 in Solaris
	//assert(radix == 10)
	// Esto esta mal :
	//		pcNumberInAscii = lltostr( value, end_buffer ) ;
	//porque concatena pcNumberInAscii+string

	// lltostr()'s behavior is undefined if value is negative
	char buffer[M4_WIN2UX_MAX_LONG_NUMBER];

	/* Bug 0072082
	Hay que null-terminar toda la cadena
	*/
	memset( (void *)buffer, 0, M4_WIN2UX_MAX_LONG_NUMBER );	// Set 0's in buffer
	pcNumberInAscii = lltostr( value, (buffer+M4_WIN2UX_MAX_LONG_NUMBER-1) ) ;
	strcpy(string, pcNumberInAscii);
#else
//	error "Falta definir 'itoa' para la plataforma"
	sprintf(string, "%d", value);
#endif

	return string;
}
#endif //_UNIX


#ifdef _UNIX
// Converts a floating-point value to a string, which it stores in a buffer
// In:	value	= Value to be converted
// In:	digits	= Number of significant digits stored
// Out: buffer	= Storage location for result
inline M4_DECL_M4WIN2UX char* _gcvt(double value, int digits, char* buffer)
{
	return gcvt( value, digits, buffer);	// Reentrante Win32 & UNIX
}
#endif //_UNIX

#ifdef _UNIX
// Converts a floating-point value to a string, which it stores in a buffer
// NO-REENTRANT, NO-THREAD-SAFE
// In:	value	= Value to be converted
// In:	count	= Number of digits after decimal point
// Out: dec		= Pointer to stored decimal-point position
// Out: sign	= Pointer to stored sign indicator
inline M4_DECL_M4WIN2UX char *_fcvt( double value, int count, int* dec, int* sign )
{
	return fcvt( value, count, dec, sign);	// No reentrante en Win32 ni en Unix
}
#endif //_UNIX


// Converts a floating-point value to a string, which it stores in a buffer
// REENTRANT, THREAD-SAFE
// In:	value	= Value to be converted
// In:	count	= Number of digits after decimal point
// Out: dec		= Pointer to stored decimal-point position
// Out: sign	= Pointer to stored sign indicator
// Out: buffer	= Storage location for result
// In:	buflen	= Length of buffer
extern M4_DECL_M4WIN2UX ClMutex g_oM4fcvtMutex;	// Mutex para region critica
inline M4_DECL_M4WIN2UX char *M4_fcvt( double value, int count, int* dec, int* sign, char* buf, int buflen=M4_WIN2UX_MAX_LONG_NUMBER)
{

#if defined(_M4SOLSPARC_) || defined(_M4SOLX86_) 

	return fconvert(value, count, dec, sign, buf);			// Si reentrante en SOLARIS, pero necesita buffer local

#elif defined(_M4HPUX1020_)  || defined (_AIX)
	 return (fcvt_r(value, count, dec, sign, buf, buflen) ? "" : buf);	// Si reentrante en HP, pero necesita buffer local y length buffer

#elif defined(_M4HPUX1100_)
	  return (fcvt(value, count, dec, sign) ? "" : buf);	// Si reentrante en HP, pero necesita buffer local y length buffer

#elif defined(_WINDOWS)
	// Win32
	g_oM4fcvtMutex.Lock();	// Seccion critica BEGIN
		//memset( (void *)buf, 0, buflen );		// Set 0's in buffer
		strncpy(buf, fcvt( value, count, dec, sign), buflen );	// No reentrante en Win32 ni en Unix
		buf[buflen-1]=0;
	g_oM4fcvtMutex.Unlock();// Seccion critica END
	return buf;

#else
	error "Falta definir 'M4_fcvt' para la plataforma"

#endif

}

// Converts a double value to a string, which it stores in a buffer
// REENTRANT, THREAD-SAFE
// In:	value	= Value to be converted
// In:	count	= Number of digits stored (integer part + decimal part)
// Out: dec		= Pointer to stored decimal-point position
// Out: sign	= Pointer to stored sign indicator
// Out: buffer	= Storage location for result
// In:	buflen	= Length of buffer
extern M4_DECL_M4WIN2UX ClMutex g_oM4ecvtMutex;	// Mutex para region critica
inline M4_DECL_M4WIN2UX char *M4_ecvt( double value, int count, int* dec, int* sign, char* buf, int buflen=M4_WIN2UX_MAX_LONG_NUMBER)
{

#if defined(_M4SOLSPARC_) || defined(_M4SOLX86_) 
	return econvert(value, count, dec, sign, buf);			// Si reentrante en SOLARIS, pero necesita buffer local

#elif defined(_M4HPUX1020_)  || defined (_AIX)
	return (ecvt_r(value, count, dec, sign, buf, buflen) ? "" : buf);	// Si reentrante en HP, pero necesita buffer local y length buffer
    
#elif defined(_M4HPUX1100_) 
    return (ecvt(value, count, dec, sign) ? "" : buf);	// Si reentrante en HP, pero necesita buffer local y length buffer

#elif defined(_WINDOWS)
	// Win32	
	g_oM4ecvtMutex.Lock();	// Seccion critica BEGIN
		//memset( (void *)buf, 0, buflen );		// Set 0's in buffer
		strncpy(buf, ecvt( value, count, dec, sign), buflen );	// No reentrante en Win32 ni en Unix
		buf[buflen-1]=0;
	g_oM4ecvtMutex.Unlock();// Seccion critica END
	return buf;
#else
	error "Falta definir 'M4_ecvt' para la plataforma"

#endif
}




// Find the next token in a string.
// In:	ai_pszStrToken		= String containing token(s)
// In:	ai_pszStrDelimit	= Set of delimiter characters
// Out: ai_pszLasts			= pointer  to a string placeholder lasts
// ret: pointer to the next token found in ai_pszStrToken. return NULL when no more tokens are found. 
//		Each call modifies strToken by substituting a NULL character for each delimiter that is encountered.
inline M4_DECL_M4WIN2UX char *M4_strtok( char *ai_pszStrToken, const char *ai_pszStrDelimit, char **ai_pszLasts )
{
#ifdef _UNIX
	return strtok_r(ai_pszStrToken, ai_pszStrDelimit, ai_pszLasts);		//POSIX.4a Draft  #6 document
#else 
	//_WINDOWS
	return (*ai_pszLasts=strtok(ai_pszStrToken, ai_pszStrDelimit)) ;	//Win32 (Use static buffer per thread)
#endif // _UNIX
}

