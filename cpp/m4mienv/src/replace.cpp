
#include "mit.h"
#include "mitdefs.cpp"

#include "replace.h"

#define GROUP_ID    MIT_TGI_CL_STR


//========================================================================ClMIT_ReplacePattern

ClMIT_ReplacePattern::ClMIT_ReplacePattern(const m4char_t *ai_Pattern) 
{
	m_oPattern.StrCpy(ai_Pattern) ;
	m_oValue.StrCpy("") ;
	m_oMatching.clear() ;
}

//========================================================================ClMIT_ReplacePattern

ClMIT_CopyReplace::ClMIT_CopyReplace() 
{
	m_poInsp=GET_INSP();
}

void ClMIT_CopyReplace::End() 
{
	m_oTemplate.End() ;
	m_oTagDelim[0].End() ;
	m_oTagDelim[1].End() ;
	m_oValueDelim.End() ;
}


m4bool_t ClMIT_CopyReplace::InitFromFile( const m4char_t *ai_TemplateFile, 
	const m4char_t *ai_TagDelimOpen, const m4char_t *ai_TagDelimClose, 
	const m4char_t *ai_ValueDelim ) 
{
	m4bool_t swOk=M4_TRUE;
	ClMIT_File F ;
	size_t sz ;
	
	ClMIT_CopyReplace::End() ;

	//***Abre fichero

	if ( ! F.Open(ai_TemplateFile, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
		swOk=M4_FALSE ;
		goto exit ;
	} ;

	//***Lee en m_Template todo el fichero de entrada

	F.DumpToString(&m_oTemplate) ;

	//***Copia resto de valores

	m_oTagDelim[0].StrCpy(ai_TagDelimOpen) ;
	m_oTagDelim[1].StrCpy(ai_TagDelimClose) ;
	m_oValueDelim.StrCpy(ai_ValueDelim) ;

	F.Close() ;
exit:

	return swOk ;
}

void ClMIT_CopyReplace::InitFromString( const m4char_t *ai_TemplateFile, 
	const m4char_t *ai_TagDelimOpen, const m4char_t *ai_TagDelimClose, 
	const m4char_t *ai_ValueDelim ) 
{
	
	ClMIT_CopyReplace::End() ;

	m_oTemplate.StrCpy(ai_TemplateFile) ;
	m_oTagDelim[0].StrCpy(ai_TagDelimOpen) ;
	m_oTagDelim[1].StrCpy(ai_TagDelimClose) ;
	m_oValueDelim.StrCpy(ai_ValueDelim) ;
}


m4bool_t ClMIT_CopyReplace::Replace( ClMIT_Str *ao_Str, 
		Patterns_t *ao_Patterns, m4bool_t ai_bSwStrCat) 
{
	m4bool_t swOk=M4_TRUE;
	size_t lenTag ;
	Patterns_t::iterator it ;
	ClMIT_Str matching, key, replaceV ;
	ClMIT_ReplacePattern *patternFound=NULL, *pattern ;
	
	const m4char_t *copyFrom, *aux, *aux2, *tagDelim[2], *valueDelim;
	size_t copyCount ;
	size_t tagDelimLen[2], valueDelimLen, tagParamLen ;


	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_CopyReplace::Replace()", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
	
	//***Reset valores entrada

	if (!ai_bSwStrCat) ao_Str->StrCpy("") ;

	for ( it=ao_Patterns->begin(); it != ao_Patterns->end(); ++it) {
		pattern = &(*it).second ;
		pattern->m_oMatching.clear() ;
	} ;

	//***Copia en destino reemplazando ocurrencias de patterns

	tagDelim[0] = m_oTagDelim[0].InChar(0) ; 	tagDelimLen[0] = m_oTagDelim[0].StrLen() ;
	tagDelim[1] = m_oTagDelim[1].InChar(0) ; 	tagDelimLen[1] = m_oTagDelim[1].StrLen() ;
	valueDelim = m_oValueDelim.InChar(0) ; valueDelimLen = m_oValueDelim.StrLen() ;

	//El template ni crece ni decrece, podemos recorrer con aritmetica de punteros

	for ( copyFrom=m_oTemplate.InChar(0); M4_TRUE ; ) {

				
		if ( strncmp(copyFrom, tagDelim[0], tagDelimLen[0])==0 ) {

			//***Vamos a procesar un <<, buscar replace, copiar valor y posicionar en 
			//	siguiente pasada
			
			copyFrom += tagDelimLen[0] ;	//Salta <<


			//Busca siguiente >> ó == para saber lenTag
			//Hay que cojer el que encuentre antes

			aux = strstr(copyFrom,valueDelim) ;
			aux2 = strstr(copyFrom,tagDelim[1]) ;
			if (aux && aux2 ) {
				aux = M4_MIN(aux,aux2) ;
			} else if ( !aux ) {
				aux = aux2 ;
			} ;
			if (!aux) {
				*GET_INSP()->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 4259895,
					"ClMIT_CopyReplace::Replace, secuencia de cierre para valor %0:s no encontrada", 
					ClMIT_Msg::MIXED_OUT) 
					<< copyFrom	<< SEND_MSG ;
					swOk=M4_FALSE ;
					goto exit ;
			} ;
			lenTag = aux - copyFrom ;
			
			//Ahora que se lentag, busco en lista

			key.ToASCIZ(copyFrom,lenTag) ;
			it = ao_Patterns->find(key) ;
			if ( it != ao_Patterns->end() ) {
				patternFound = &(*it).second ;
			} else {
				patternFound = NULL ;
				*GET_INSP()->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 4259896,
					"ClMIT_CopyReplace::Replace, propiedad %0:s no encontrada", 
					ClMIT_Msg::MIXED_OUT) 
					<< copyFrom	<< SEND_MSG ;
				swOk=M4_FALSE ;
				goto exit ;
			} ;

			//Test modo << >>, o modo << == >>, o error (busco >> o ==)

			if ( strncmp( &copyFrom[lenTag], tagDelim[1], tagDelimLen[1])==0 ) {

				//Tag sin value, es un << >>

				copyFrom += lenTag+tagDelimLen[1] ; //Salta Tag y >>

				ao_Str->StrCat( patternFound->GetValue() ) ;

				#ifdef MIT_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
						"Pattern %0:s, replace value %1:s ") 
						<<  patternFound->GetPattern() 
						<<  patternFound->GetValue() 
						<< DETAILED_MSG << SEND_MSG   ;
				};
				#endif
		
			} else if ( strncmp( &copyFrom[lenTag], valueDelim, valueDelimLen)==0 ) {
			
				//Tag con value, es un << == >> 

				copyFrom += lenTag+valueDelimLen ; //Salta Tag y == intermedio

				//Busco >> de cierre

				aux = strstr(copyFrom,tagDelim[1]) ;
				if ( !aux ) {
					*GET_INSP()->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 4259895,
						"ClMIT_CopyReplace::Replace, secuencia de cierre para valor %0:s no encontrada", 
						ClMIT_Msg::MIXED_OUT) 
						<< copyFrom	<< SEND_MSG ;
					swOk=M4_FALSE ;
					goto exit ;
				} ;
				tagParamLen = aux-copyFrom ;

				matching.ToASCIZ(copyFrom,tagParamLen) ;

				copyFrom += tagParamLen+tagDelimLen[1] ; //Salta TagParam y >> final

				replaceV.StrCpy( patternFound->GetValue() ) ; //Sustitucion value ..
				replaceV.StrCat(matching ) ;				//... y matching 
				ao_Str->StrCat( replaceV ) ; 
				patternFound->m_oMatching.push_back( matching ) ; //notifico matching

				#ifdef MIT_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
						"Pattern %0:s, param %1:s, replace value %2:s ") 
						<<  patternFound->GetPattern() 
						<<  matching.InChar(0)
						<<  replaceV.InChar(0) 
						<< DETAILED_MSG << SEND_MSG   ;
				};
				#endif

			} else {
			
				*GET_INSP()->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 4259895,
					"ClMIT_CopyReplace::Replace, secuencia de cierre para valor %0:s no encontrada", 
					ClMIT_Msg::MIXED_OUT) 
					<< copyFrom	<< SEND_MSG ;
				swOk=M4_FALSE ;
				goto exit ;
			} ;


		} else {

			//***No estamos en un <<, hayamos el siguiente <<

			aux = strstr(copyFrom,tagDelim[0]) ;
			if ( !aux ) {
				
				//***No hay mas <<, copiamos todo y acabamos

				ao_Str->StrCat( copyFrom ) ;
				break ;

			} else {

				//***Si hay mas <<, copiamos hasta el y nos posicionamos para otra pasada

				copyCount = aux-copyFrom ;
				matching.ToASCIZ(copyFrom,copyCount) ;
				ao_Str->StrCat( matching ) ;
				copyFrom += copyCount ;
			} ;
		} ;

	} ;

exit:

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_CopyReplace::Replace() SwOj %0:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< swOk << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	return swOk ;
}


m4bool_t ClMIT_CopyReplace::Replace( ClMIT_File *ai_F, Patterns_t *ao_Patterns ) 
{
	ClMIT_Str str ;

	if ( ! Replace( &str, ao_Patterns) ) {
		return M4_FALSE ;
	} else {
		ai_F->WStr(str) ;
		return M4_TRUE ;
	} ;
}


m4bool_t ClMIT_CopyReplace::Replace( const m4char_t *ai_OutFile, Patterns_t *ao_Patterns ) 
{
	ClMIT_File F ;

	if ( ! F.Open(ai_OutFile, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN, M4_FALSE ) ) {
		return M4_FALSE ;
	} else {
		return Replace( &F, ao_Patterns ) ;
	} ;

}

m4bool_t ClMIT_CopyReplace::GetMatching( Patterns_t *ao_Patterns ) 
{
	ClMIT_Str str ;

	return Replace( &str, ao_Patterns, M4_FALSE ) ;
}

void ClMIT_CopyReplace::LoadPatternList(Patterns_t *ao_Pattern, const m4char_t *ai_pcPattenIds[], 
	m4bool_t ai_bSwAppend ) 
{
	ClMIT_ReplacePattern pattern("KK") ;

	if ( !ai_bSwAppend ) ao_Pattern->clear() ;

	for ( ; *ai_pcPattenIds; ++ai_pcPattenIds) {
		pattern.m_oPattern.StrCpy(*ai_pcPattenIds) ;
		pattern.m_oPattern.Trim() ;
		if ( pattern.m_oPattern.StrLen()==0 ) break ;

		ao_Pattern->insert( Patterns_t::value_type(pattern.m_oPattern,pattern) ) ;
	} ;
}

