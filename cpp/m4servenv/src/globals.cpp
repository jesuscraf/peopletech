
#include <servenv.hpp>


m4return_t GetEnvironCpp (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ao_szValue, m4uint32_t ai_iBufflen)
{
	ClServEnv oServEnv ;

	return oServEnv.GetEnviron ( ai_szPath, ai_szProperty, ao_szValue, ai_iBufflen) ;
}

m4return_t SetEnvironCpp (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ai_szValue , m4bool_t ai_bPersistable = M4_FALSE )
{
	ClServEnv oServEnv ;

	return oServEnv.SetEnviron ( ai_szPath, ai_szProperty, ai_szValue , ai_bPersistable ) ;
}

m4return_t GetEnvironDoubleCpp (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4double_t & ao_dPropertyValue) 
{
	ClServEnv oServEnv ;

	return oServEnv.GetEnviron ( ai_szPath, ai_szProperty, ao_dPropertyValue ) ;
}


void SetNavigatorCpp (ClNavigatorFirewall *ai_poNavigator)
{
	ClServEnv::SetNavigator (ai_poNavigator);
}



M4_DECL_M4SERVENV m4return_t GetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ao_szValue, m4uint32_t ai_iBufflen)
{
	return GetEnvironCpp (ai_szPath, ai_szProperty, ao_szValue, ai_iBufflen) ;
}

M4_DECL_M4SERVENV m4return_t SetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ai_szValue , m4bool_t ai_bPersistable = M4_FALSE )
{
	return SetEnvironCpp (ai_szPath, ai_szProperty, ai_szValue, ai_bPersistable ) ;
}

M4_DECL_M4SERVENV void SetNavigator (ClNavigatorFirewall *ai_poNavigator)
{
	  SetNavigatorCpp (ai_poNavigator);
}

M4_DECL_M4SERVENV m4return_t GetEnvironDouble (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4double_t & ao_dPropertyValue)
{
	return GetEnvironDoubleCpp (ai_szPath, ai_szProperty, ao_dPropertyValue) ;
}



