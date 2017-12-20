

#include "m4buildversion.hpp"
#include "m4versioninfo.hpp"
#include "m4regdef.hpp"
#include "m4var.hpp"


#if defined( _WINDOWS ) || defined ( WINDOWS )
	static const char *propertyValue = NULL;
#else
	static const char *propertyValue = "M4R&D";
#endif




m4return_t GetBuildVersion (m4pchar_t bv)
{
//UNDER WIN32 WORKS AS IT SHOULD DO, BUT UNDER UNIX ALWAYS RETURN "M4R&D".
#if defined( _WINDOWS ) || defined ( WINDOWS )
	if ( ! propertyValue )
	{
		// this variable must be called equal that its content.
		// The name is the name of file (without extension) where
		// we want to read version information.
		const char *m4buildversion = "m4buildversion";
		
		const M4_VS_VERSION_INFO *versionInfo = NULL;
//		const char *propertyValue = NULL;
		
		// get versionInfo
		M4GetComponentVersionInformation(versionInfo, m4buildversion);
		if (NULL == versionInfo)
		{
			cout << "Problems in m4buildversion in M4GetComponentVersionInformation" << endl;
			return M4_ERROR;
		}
		
		// get specialbuild.
		propertyValue = versionInfo -> getVersionProperty("SpecialBuild");
		if (NULL == propertyValue)
		{
			cout << "No SpecialBuild Inserted" << endl;
			return M4_ERROR;
		}
		
	}
#endif	//END ifdef _WINDOWS

	//if bv is NULL means we just want to calculate build version
	if ( bv )
		strcpy (bv,propertyValue);
	
	return M4_SUCCESS;
}

#ifdef _WINDOWS
m4uint32_t WINAPI GetBuildVersionVB (m4pchar_t bv, int& l)
{

	if ( M4_ERROR == GetBuildVersion (NULL) )
		return M4_ERROR;

	//CONVERSION TO WIDE CHAR, ie, UNICODE
	//mbstowcs(bv,propertyValue,strlen(propertyValue)+1);
	strcpy (bv,(char*)propertyValue);
	l = (int) strlen (propertyValue);

	return M4_SUCCESS;
}
#endif


m4return_t GetLN4BuildVersion (m4VariantType *ai_pvArgumentList, m4uint32_t ai_ulArgumentCount, m4VariantType& ao_vReturn, m4pvoid_t ai_pContext)
{
	if ( M4_ERROR == GetBuildVersion (NULL) )
		return M4_ERROR;

	if ( ai_ulArgumentCount < 1 )
		return M4_ERROR;

	ai_pvArgumentList[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	//i pass myself the constness by the lining of the balls
	ai_pvArgumentList[0].Data.PointerChar = (char*)propertyValue;

	return M4_SUCCESS;
}


// Para convertir la version de cadena a número
m4uint16_t	ConvertM2ToNumericVersion( m4pcchar_t ai_pccStringVersion )
{

	m4uint16_t	iResult ;
	m4uint16_t	iVersion ;
	m4uint16_t	iSubVersion ;
	size_t		iLength ;
	m4pchar_t	pcSubVersion ;
	m4pchar_t	pcFinSubVersion ;
	m4char_t	acString[ M4_M2_MAX_STRING_VERSION + 1 ] ;


	iLength = strlen( ai_pccStringVersion ) ;

	if( iLength > M4_M2_MAX_STRING_VERSION )
	{
		return( 0 ) ;
	}

	memcpy( acString, ai_pccStringVersion, iLength + 1 ) ;


	pcSubVersion = strchr( acString, '.' ) ;

	if( pcSubVersion == NULL )
	{
		return( 0 ) ;
	}

	*pcSubVersion = '\0' ;
	pcSubVersion++ ;

	pcFinSubVersion = strchr( pcSubVersion, '.' ) ;

	if( pcFinSubVersion != NULL )
	{
		*pcFinSubVersion = '\0' ;
	}

	iVersion = atoi( acString ) ;
	iSubVersion = atoi( pcSubVersion ) ;

	iResult = iSubVersion + ( iVersion << 6 ) ;

	return( iResult ) ;
}


m4return_t	M4ComposeRegistryPath( m4pcchar_t ai_pccSide, m4pcchar_t ai_pccSection, m4pchar_t ai_pcBuffer, m4uint32_t ai_iInSize, m4uint32_t &ao_riOutSize )
{
	ao_riOutSize = m4uint32_t( strlen( M4_NEW_REGEDIT_KEY ) + strlen( ai_pccSide ) + strlen( ai_pccSection ) ) ;

	if( ao_riOutSize > ai_iInSize )
	{
		*ai_pcBuffer = '\0' ;
		ao_riOutSize = 0 ;
		return( M4_ERROR ) ;
	}

	strcpy( ai_pcBuffer, M4_NEW_REGEDIT_KEY ) ;
	strcat( ai_pcBuffer, ai_pccSide ) ;
	strcat( ai_pcBuffer, ai_pccSection ) ;

	return( M4_SUCCESS ) ;
}

