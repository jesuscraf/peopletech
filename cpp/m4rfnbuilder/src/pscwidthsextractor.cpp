
#include "rfn.h"
#include "rfndf.inl"
#include "clstr.h"
#include "pscwidthsextractor.h"

#define GROUP_ID RFN_TGI_OTHERS

m4bool_t WidthsExtractor(
	const m4char_t *ai_pcPFAFile, const m4char_t *ai_pcAFMFile,
	RFN_EncodingWidths_t *ao_EncodingWidths, RFN_GlyphWidths_t *ao_GlyphWidths)
{
	StMIT_Inspector *Insp=GET_INSP();
	ClMIT_File PFAFile, AFMFile ;
	m4bool_t swOk=M4_TRUE ;
	ClMIT_Str strPFA, strAFM, strName ;
	string sName;
	m4char_t *pos, *pos2 ;
	const m4char_t *errMsg=NULL ;
	size_t l ;
	const m4char_t *blanks=" \t\r\n" ;
	static const struct StTags {
		m4char_t * tag ;
		m4int16_t  nParams ;
	} tags[] = {
		{ "C", 1 },
		{ "CH", 1 },
		{ "WX", 1 },
		{ "W0X", 1 },
		{ "W1X", 1 },
		{ "WY", 1 },
		{ "W0Y", 1  },
		{ "W1Y", 1 },
		{ "W", 2 },
		{ "W0", 2 },
		{ "W1", 2 },
		{ "VV", 2 },
		{ "N", 1 },
		{ "B", 4  },
		{ "L", 1 },
		{ NULL, 0 },
	} ;
	const StTags *currTag ;
	m4char_t *args[20] ;
	m4int16_t B ;
	
	const m4char_t *encodingSeparator=" \t\r\n%]" ;
	RFN_GlyphWidths_t::iterator it ;

	//***Abre ficheros y los vuelca a memoria
	// UNICODE REPORTS.
	m4bool_t bAddBOM = M4_FALSE;
	if ( ! PFAFile.Open(ai_pcPFAFile, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN , bAddBOM ) ) {
		errMsg=ai_pcPFAFile;
		swOk=M4_FALSE ;
		goto exit ;
	} ;
	if ( ! AFMFile.Open(ai_pcAFMFile, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN , bAddBOM ) ) {
		errMsg=ai_pcAFMFile;
		swOk=M4_FALSE ;
		goto exit ;
	} ;

	PFAFile.DumpToString(&strPFA) ;
	AFMFile.DumpToString(&strAFM) ;

	//***Lee el AFM y monta una lista glypName-Width

	m4bool_t cFound, wFound ;
	m4int32_t width ;

	//Localiza comienzo bloque StartCharMetrics
	pos=strstr(strAFM.InChar(0), "StartCharMetrics") ;
	if (!pos) {
		errMsg="StartCharMetrics" ;
		swOk=M4_FALSE ;
		goto exit ;
	}  ;
	//Inicio de strtok. 
	//El primer token será StartCharMetrics
	pos = strtok(pos,blanks) ;  
	//Siguiente token , el StartCharMetrics ###
	pos = strtok(NULL,blanks) ; 

	//Bucle principal hasta encontrar el fin del bloque, 
	//	lee secuencias Tag Param,Param,... ;

	ao_GlyphWidths->clear() ;

	while ( M4_TRUE ) {

		//Lee tipo de tag o EndCharMetrics
		pos = strtok(NULL,blanks) ; 		
		if ( strcmp(pos,"EndCharMetrics" )==0 ) {
			break ;
		} ;

		//Segun el tipo de tag, busca cuantos argumentos leer
		for ( currTag=&tags[0]; ; ++currTag) {
			if ( !currTag->tag ) {
				errMsg=pos;
				swOk=M4_FALSE ;
				goto exit ;
			} ;
			if ( strcmp(pos,currTag->tag)==0 ) {
				break ;
			} ;
		} ;
		
		//Y lee los n argumentos
		for (B=0; B<currTag->nParams;++B) {
			args[B]=strtok(NULL,blanks) ;
		} ;

		//Testea y consume el ; final
		pos = strtok(NULL,blanks) ;
		if ( strcmp(pos,";" )!=0 ) {
			errMsg="; expected";
			swOk=M4_FALSE ;
			goto exit ;
		} ;

		//Ya estan leido el Tag. La info esta en currTag y args. Proceso la info.

		if ( strcmp(currTag->tag,"C")==0 || strcmp(currTag->tag,"CH")==0 ) {
			cFound=M4_TRUE ;

		} else if ( 
			strcmp(currTag->tag,"WX")==0 || strcmp(currTag->tag,"W0X")==0 ||
			strcmp(currTag->tag,"W")==0 || strcmp(currTag->tag,"W0")==0 
			) 
		{
			wFound=M4_TRUE ;
			if (!cFound) {
				errMsg="W found, not previous C found";
				swOk=M4_FALSE ;
				goto exit ;
			} ;
			width = atol(args[0]) ;

		} else if ( strcmp(currTag->tag,"N")==0 ) {

			if (!wFound) {
				errMsg="N found, not previous W found";
				swOk=M4_FALSE ;
				goto exit ;
			} ;
			strName.StrCpy(args[0]) ;
			sName = (m4char_t *)strName;
			ao_GlyphWidths->insert( RFN_GlyphWidths_t::value_type(sName,width) ) ;
			wFound=M4_FALSE ;
			cFound=M4_FALSE ;
		} ;
	
	} ;

	//*******En ao_GlyphWidths tenemos key=glyphName, value=width

	//Localiza comienzo bloque /Encoding
	pos=strstr(strPFA.InChar(0), "/Encoding") ;
	if (!pos) {
		errMsg="/Encoding" ;
		swOk=M4_FALSE ;
		goto exit ;
	}  ;

	//Desde ese comienzo, localiza el ] de cierre, y pone en el un finde cadena
	pos2 = strstr(pos,"]") ; 
	if (!pos2) {
		errMsg="/Encoding closing ] missing" ;
		swOk=M4_FALSE ;
		goto exit ;
	}  ;
	*pos2=M4_END_STR ;

	//Desde pos, tenemos una cadena con el array de glyphnames. Buscamos 
	//	explicitamente secuencias de /xxxxx para evitarnos procesar los % y similares

	ao_EncodingWidths->clear() ;
	while (M4_TRUE) {
		++pos ; //Salta el / del /Encoding
		pos += strcspn(pos,"/") ; //Se posiciona en / de definicion de caracter
		if (! *pos ) break ;
		l = strcspn(pos,encodingSeparator) ;
		if (l==0) {
			errMsg="/Encoding /Name missing" ;
			swOk=M4_FALSE ;
			goto exit ;
		} ;
		strName.ToASCIZ(pos+1,l-1) ; //+-1 para no incluir / 
		sName = (m4char_t *)strName;

		//Busca el width por GlyphName
		it = ao_GlyphWidths->find(sName) ;
		if (it==ao_GlyphWidths->end()) {
			errMsg=strName;
			swOk=M4_FALSE ;
			goto exit ;
		} ;
		ao_EncodingWidths->push_back( (*it).second ) ;

		pos += l ;
	} ;


exit:

	if (!swOk ) {
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, "Error leyendo metricas PFA/AFM, msg %0:s" ) 
			<< errMsg << SEND_MSG ;
		printf( "Error leyendo metricas PFA/AFM, error msg " ) ;
		printf( errMsg ) ;
		printf( "\n" ) ;
	} ;

	return  swOk ;

}
