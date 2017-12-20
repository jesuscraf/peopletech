#ifndef __M4BUILDVERSION__
#define __M4BUILDVERSION__

#include "m4buildversion_dll.hpp"

#include "m4system.hpp"
#include "m4types.hpp"


class m4VariantType;



#define	M4_M2_MAX_STRING_VERSION					15


#ifdef _WINDOWS
extern "C" M4_DECL_M4BUILDVERSION m4uint32_t WINAPI GetBuildVersionVB (m4pchar_t bv,int& l);
#endif

extern "C" M4_DECL_M4BUILDVERSION m4return_t GetBuildVersion( m4pchar_t bv ) ;
extern "C" M4_DECL_M4BUILDVERSION m4return_t GetLN4BuildVersion( m4VariantType *ai_pvArgumentList, m4uint32_t ai_ulArgumentCount, m4VariantType& ao_vReturn, m4pvoid_t ai_pContext ) ;
extern "C" M4_DECL_M4BUILDVERSION m4pcchar_t GetMindVersion( void ) ;
extern "C" M4_DECL_M4BUILDVERSION m4uint16_t GetM2VersionNumber( void ) ;
extern "C" M4_DECL_M4BUILDVERSION m4pcchar_t GetM2VersionString( void ) ;
extern "C" M4_DECL_M4BUILDVERSION m4uint16_t ConvertM2ToNumericVersion( m4pcchar_t ai_pccStringVersion ) ;
extern "C" M4_DECL_M4BUILDVERSION m4pcchar_t GetMindDBVersion( void ) ;
extern "C" M4_DECL_M4BUILDVERSION void SetMindDBVersion( m4pcchar_t ai_pccVersionsList ) ;

M4_DECL_M4BUILDVERSION m4return_t M4ComposeRegistryPath( m4pcchar_t ai_pccSide, m4pcchar_t ai_pccSection, m4pchar_t ai_pcBuffer, m4uint32_t ai_iInSize, m4uint32_t &ao_riOutSize );

#endif

