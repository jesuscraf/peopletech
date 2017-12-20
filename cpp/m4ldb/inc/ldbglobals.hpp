#ifndef _LDBGLOBALS_HPP_
#define _LDBGLOBALS_HPP_

#include "m4types.hpp"
#include "medvarin.hpp"

m4return_t M4SetUserKey( m4pcchar_t ai_pUserPwd );

// Función de conversión
m4return_t	M4DataConvert	( m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t & ao_pcOutString, m4int_t &ao_riCopied );
m4return_t	M4DataDeConvert	( m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t & ao_pcOutString, m4int_t &ao_riCopied );
m4return_t	M4DeConvert		( m4pcchar_t ai_pccInString, m4pchar_t & ao_pcOutString );

m4pvoid_t ConvertValue (m4VariantType *aio_pValue, m4int32_t ai_m4Type);

// Para ficheros.
m4return_t M4ConvertFile	(m4pchar_t ai_pFileIn, m4pchar_t ai_pFileOut);
m4return_t M4DeConvertFile	(m4pchar_t ai_pFileIn, m4pchar_t ai_pFileOut);


#endif

