//## begin module.includes preserve=yes

#include "m4tratini.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes




m4int32_t	M4GetPrivateProfileString(
						M4ClString lpAppName, // points to section name
						M4ClString lpKeyName, // points to key name
						M4ClString lpDefault, // points to default string
						M4ClString &lpReturnedString, // points to destination buffer
//						m4int32_t nSize, // size of destination buffer
						M4ClString lpFileName // points to initialization filename
					)
{
	m4int32_t	ret;


	m4char_t	lpReturnedString1[256];
//	lpReturnedString1 = new m4char_t[32000];
//	lpReturnedString.reserve(32000);
	ret = GetPrivateProfileString( lpAppName, lpKeyName, lpDefault,
					lpReturnedString1, 256, lpFileName );
	lpReturnedString = lpReturnedString1;

	if ( ret > 0 )
		return M4_SUCCESS;

	return M4_ERROR;
}


m4int32_t	M4GetPrivateProfileInt( M4ClString lpAppName,

					// address of section name
							M4ClString lpKeyName,
					// address of key name
							m4int32_t nDefault,
					// return value if key name is not found
							M4ClString lpFileName
					// address of initialization filename
						)
{
	m4int32_t	ret;

	ret = GetPrivateProfileInt( lpAppName, lpKeyName, nDefault,
					lpFileName );

	return ret;
}


m4int32_t  M4GetPrivateProfileSection(m4pchar_t  lpAppName,
									  m4pchar_t &lpReturnedString,
									  m4int32_t  nSize,
									  m4pchar_t lpFileName)
{

		m4int32_t	ret;
		
		ret = GetPrivateProfileSection (lpAppName,lpReturnedString,nSize,lpFileName);
		return ret;

}
 
//## end module.epilog
