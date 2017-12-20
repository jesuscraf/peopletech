
#ifndef __M4UXSTR_HPP__
#define __M4UXSTR_HPP__


#include <string.h>

#ifdef _UNIX
#include <strings.h>
#endif




//####################################################################################
//	Definicion "inline" C++ de las funciones tipicas de conversion de cadenas.
//####################################################################################


// Perform a lowercase comparison of strings.
// In:	a_pszString1 = Null-terminated string
// In:	a_pszString1 = Null-terminated string
// ret: < 0 string1 <  less than string2
//		==0 string1 == identical to string2	 
//		> 0 string1 >  greater than string2	
inline int stricmp( const char* a_pszString1, const char* a_pszString2 )
{
#if defined(_SOLARIS) ||  defined(_HP) || defined(_AIX)
	return ( strcasecmp(a_pszString1, a_pszString2) );	// Solaris 2.5.x,2.6, HP-UX10.20
#else
	// Resto de UNIX
    const char* pszStr1 = a_pszString1 ;
    const char* pszStr2 = a_pszString2 ;

    while( ( tolower( *pszStr1 ) == tolower( *pszStr2 ) ) && ( *pszStr1 != '\0' ) && ( *pszStr2 != '\0' )) {
        pszStr1++ ;
        pszStr2++ ;
    } 

    return( tolower( *pszStr1 ) - tolower( *pszStr2 ) ) ;
#endif
}

inline int strcmpi( const char* a_pszString1, const char* a_pszString2 )
{
	return stricmp( a_pszString1, a_pszString2 );
}

// Compare characters of two strings without regard to case.
// In:	a_pszString1 = Null-terminated string
// In:	a_pszString1 = Null-terminated string
// In:	a_szCount	 = Number of characters to compare
// ret: < 0 string1 <  less than string2
//		==0 string1 == identical to string2	 
//		> 0 string1 >  greater than string2	
inline int strnicmp( const char* a_pszString1, const char* a_pszString2, size_t a_szCount )
{
#if defined(_M4SOLSPARC_) || defined(_M4SOLX86_) || defined(_M4HPUX1020_)
	return ( strncasecmp(a_pszString1, a_pszString2, a_szCount) );	// Solaris 2.5.x,2.6, HP-UX10.20
#else
    size_t  sz = 0 ;
    const   char* pszStr1 = a_pszString1 ;
    const   char* pszStr2 = a_pszString2 ;

    while( ( tolower( *pszStr1 ) == tolower( *pszStr2 ) ) && ( *pszStr1 != '\0' ) && ( sz < a_szCount ) ) {
        pszStr1++ ;
        pszStr2++ ;
        sz++ ;
    } 

	return ( (sz == a_szCount) ? (0) : (tolower(*pszStr1)-tolower(*pszStr2)) ) ;    
#endif
}

// Compare characters of two strings without regard to case.
// In:	a_pszString1 = Null-terminated string
// In:	a_pszString1 = Null-terminated string
// In:	a_szCount	 = Number of characters to compare
// ret: < 0 string1 <  less than string2
//		==0 string1 == identical to string2	 
//		> 0 string1 >  greater than string2	
inline int strncmpi( const char* a_pszString1, const char* a_pszString2, size_t a_szCount )
{
	return strnicmp(a_pszString1, a_pszString2, a_szCount);
}

// Convert a string to uppercase.
// In:	a_pszString = Null-terminated string to convert
// ret: pointer to the altered string == a_pszString
inline char*   strupr( char* a_pszString )
{
    char*   pszStr = a_pszString ;
	for ( ;*pszStr != '\0';  pszStr++ )
		*pszStr = toupper( *pszStr ) ;
    return( a_pszString ) ;
}

// Convert a string to lowercase.
// In:	a_pszString = Null-terminated string to convert
// ret: pointer to the altered string == a_pszString
inline char*   strlwr( char* a_pszString ) 
{
    char*  pszStr = a_pszString ;
	for ( ;*pszStr != '\0';  pszStr++ )
		*pszStr = tolower( *pszStr ) ;
    return( a_pszString ) ;
} 



#endif	//__M4UXSTR_HPP__

