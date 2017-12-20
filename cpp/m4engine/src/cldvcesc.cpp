//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcesc.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 23/04/98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "eng.h"
#include "engdf.inl"

#include "cldvcesc.hmg"
#include "cldvcesc.h"
#include "sysinfo.h"

#include "m4unicode.hpp"


#define ESC_PRINTER_RESET "\x1B@"
#define ESC_EOP "\x0C"
#define ESC_EOP_L 1

#define ESC_FONT "\x1Bk%c"	
#define ESC_FONT_L 3			//Ojo, usar esta len: ultimo caracter puede ser 0

#define ESC_MODE_FONT "\x1B!%c"	
#define ESC_MODE_FONT_L 3		//Ojo, usar esta len: ultimo caracter puede ser 0

//Parametrizacion de pagina

					//Ponderado en Inch
#define ESC_PAGE_SIZE "\033C\000%c"
#define ESC_PAGE_SIZE_L 4 //Ojo, usar esta len: ultimos 2 caracteres pueden ser 0

// Sumar, y aplicar a ESC_MODE_FONT, ...

#define ESC_MODEF_12CPI 1	//Si no activo, paso 10
#define ESC_MODEF_PROP  2
#define ESC_MODEF_BOLD  8
#define ESC_MODEF_ITALIC  64
#define ESC_MODEF_UNDERLINE  128
#define ESC_MODEF_DOUBLEX  32

//	....hasta aqui.

#define ESC_DRAFT_ON  "\x1Bx\x00"
#define ESC_DRAFT_OFF "\x1Bx\x01"
#define ESC_DRAFT_L	3		//Ojo, usar esta len: ultimo caracter puede ser 0

#define ESC_DOUBLEY_ON  "\x1Bw\x01"
#define ESC_DOUBLEY_OFF "\x1Bw\x00"
#define ESC_DOUBLEY_L	3		//Ojo, usar esta len: ultimo caracter puede ser 0

#define ESC_COMPR_ON   "\x0F"
#define ESC_COMPR_OFF  "\x12"
 	
#define ESC_GOX "\x1B$%c%c"
#define ESC_GOX_0 "\x1B$\x00\x00"
#define ESC_GOX_L 4				//Ojo, usar esta len: ultimos 2 caracteres pueden ser 0

#define ESC_SKIPY "\x1BJ%c"
#define ESC_FULL_SKIPY "\x1BJ\xFF"	//Salta todo lo que puede
#define ESC_SKIPY_L 3			//Ojo, usar esta len: ultimo caracter puede ser 0


ClENG_DvcESC::ClENG_DvcESC()
{
	m_poPageESC=NULL ;
}

void ClENG_DvcESC::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPageESC, ClENG_PageESC, GetNumLayoutPages() 
	) ;
	ClENG_DvcFile::End() ;
}

m4bool_t ClENG_DvcESC::Init(	
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 

		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Campos especificos de este Dvc

		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4int16_t ai_iXFactor, 
		m4int16_t ai_iLFFactor,
		m4char_t *ai_JPageSize,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t Return=M4_TRUE ;
	m4int16_t i ;
	size_t l ;
	m4char_t *p, saveChar ;
	m4uint32_t lastv ;
	static const m4char_t *digit="0123456789" ;

	
	ClENG_DvcESC::End();

	Return = ClENG_DvcFile::Init(

		//Campos para ClENG_OutDevice::Init

		ai_iNumPages, ClMIT_Flag::NONE_BITS, NULL,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak,	
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_DvcFile::GetMappingToFileDuplex(ai_Duplex), 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR 
	) ;

	if (!Return) goto exit ;

	m_iXFactor=ai_iXFactor ;
	m_iLFFactor=ai_iLFFactor ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPageESC, ClENG_PageESC, GetNumLayoutPages() 
	) ;

	for (i=0, p=ai_JPageSize, lastv=0; i<GetNumLayoutPages(); ++i ) {

		p += strcspn(p,digit) ; //Salta separadores

		l=strspn(p,digit) ;	//l=len cadena compuesta de numeros

		if (l) {
			saveChar=p[l] ;
			p[l]=M4_END_STR ;
			lastv = atol(p) ;
			p[l]=saveChar ;
			p += l ;
		} ;
	
		m_poPageESC[i].m_JPageSize=lastv ;
	} ;
	

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		

}


m4bool_t ClENG_DvcESC::InitPage(	m4int16_t ai_iIdPage,
				DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
				size_t ai_szBlockBuffer, 
				ClENG_PagePrinter::ORIENTATION   ai_Orientation,
				ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
				DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
				ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return=M4_TRUE ;
	m4bool_t initializePhantom ;
	
	if ( !M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCESC_MSG_10 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

	m_poPageESC[ai_iIdPage].oPagePrinter.Init( ai_Orientation, ai_PageSize, ai_PaperSource, 
		ai_SizeX, ai_SizeY);

	Return=ClENG_DvcFile::InitPage( 
		ai_iIdPage, 
		m_poPageESC[ai_iIdPage].oPagePrinter.GetSizeX(),
		m_poPageESC[ai_iIdPage].oPagePrinter.GetSizeY(),
		ai_OffsetX, ai_OffsetY, 
		ai_szBlockBuffer, initializePhantom,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!Return) goto exit ;

	if ( initializePhantom )  {
		m_poPageESC[ GetNumLayoutPages()-1 ].oPagePrinter.Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY);
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}


m4bool_t ClENG_DvcESC::PreReset() 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Page ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;
	
	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;

	//------>Datos que solo van una vez al principio del fichero (o ficheros)

	for (Page=0;Page<GetNumLayoutPages();++Page){
		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		if ( ! swRedirect ) {
			F->WStr( ESC_PRINTER_RESET ) ;	
		} ;
	} ;

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}

void     ClENG_DvcESC::PostReset() 
{
	//---------->Cierra ficheros (no hay ATOMIC mode)

	m_oOutRepository.CloseBreak(IsValidOutput()) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}


m4bool_t  ClENG_DvcESC::RefreshLayout() 
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
}


m4bool_t ClENG_DvcESC::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}


void ClENG_DvcESC::NewEntry( 
	m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y , 
	const char *ai_pcStr, size_t ai_NChar, StPi_t *ai_Pi) 
{
	ClENG_PageESC * Page = &m_poPageESC[ai_iPage] ;
	Data_t Data ;

	Data.m_NBytes = ai_NChar ;
	Data.m_oStr.StrNCpy(ai_pcStr,Data.m_NBytes) ;
	Data.m_pi=ai_Pi;
	if (Data.m_pi) {
		Data.m_PiHandler = Data.m_pi->m_Handler ;
	} else {
		Data.m_PiHandler=-1 ;
	} ;
	Data.m_X=ai_X ;
	Data.m_poFont = GetLastFont(ai_iPage ) ;
	ai_Y += Data.m_poFont->GetDvcHeight() ;	//Ajuste de linea-base

	Page->Map.insert( Map_t::value_type(ai_Y,Data) ) ;
}


void ClENG_DvcESC::SelectFontDevice( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont ) 
{
}

void ClENG_DvcESC::SelectFontAux( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont, M4ClBuffer *ai_Buff ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4int16_t IdFont=ai_poFont->ESC_GetFontCode() ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;

	sprintf(m_pcBuff, ESC_MODE_FONT, (unsigned char)( 
		( ai_poFont->ESC_GetPitch() == ClFONT_M_Font::ESC_PITCH_10 ? 0 : ESC_MODEF_12CPI )+
		( ai_poFont->IsFixedSpace() ? 0 : ESC_MODEF_PROP )+
		( ai_poFont->IsBold() ? ESC_MODEF_BOLD : 0 )+
		( ai_poFont->IsItalic() ? ESC_MODEF_ITALIC : 0 )+
		( ai_poFont->IsUnderline() ? ESC_MODEF_UNDERLINE : 0 )+
		( ai_poFont->ESC_IsDoubleX() ? ESC_MODEF_DOUBLEX : 0 )
	) ) ;
	ai_Buff->Append(m_pcBuff, ESC_MODE_FONT_L );

	if ( IdFont == ClFONT_M_Font::ESC_FONT_DRAFT ) {

		ai_Buff->Append(ESC_DRAFT_ON, ESC_DRAFT_L );
		

	} else {

		ai_Buff->Append(ESC_DRAFT_OFF, ESC_DRAFT_L );
		sprintf(m_pcBuff, ESC_FONT, (unsigned char)IdFont) ;
		ai_Buff->Append(m_pcBuff, ESC_FONT_L );

	} ;

	ai_Buff->Append( 
		ai_poFont->ESC_IsDoubleY() ? ESC_DOUBLEY_ON : ESC_DOUBLEY_OFF, ESC_DOUBLEY_L 
	);

	ai_Buff->AppendStr( 
		ai_poFont->ESC_IsCompr() ? ESC_COMPR_ON : ESC_COMPR_OFF
	);
}

void ClENG_DvcESC::SkipToJ(m4uint32_t *ao_SkippedJ, m4uint32_t ai_SkipToJ, M4ClBuffer *ai_Buff )
{
	m4char_t buffAux[ESC_SKIPY_L+40] ;
	m4int32_t SkipY= ai_SkipToJ - *ao_SkippedJ ;

	if (SkipY<=0) return ;

	while (SkipY>255) {
		ai_Buff->Append( ESC_FULL_SKIPY, ESC_SKIPY_L ) ;
		SkipY  -= 255;
		(*ao_SkippedJ) += 255 ;
	} ;
	if (SkipY) {
		sprintf( buffAux, ESC_SKIPY, (unsigned char)SkipY ) ;
		ai_Buff->Append( buffAux, ESC_SKIPY_L ) ;
		(*ao_SkippedJ) += SkipY ;
	} ;
}


void ClENG_DvcESC::PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText,
		m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
		ClENG_OutDevice::StPIParams *ai_pstPI ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	
	ai_iX += GetOffsetX(ai_iPage) ;
	ai_iY += GetOffsetY(ai_iPage) ;
	
	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	m4pchar_t pcString = NULL;
	m4pchar_t pcInputString = new m4char_t[ai_szNChar + 1];
	int szNChar = 0;

	memcpy(pcInputString, ai_pcStr, ai_szNChar);
	pcInputString[ai_szNChar] = '\0';

	szNChar = strlen(pcInputString);
	pcString = M4CppToANSI(pcInputString, szNChar);

	strncpy(m_pcBuff, pcString, szNChar) ; 
	
	delete [] pcString;
	delete [] pcInputString;

	if (! ai_bRawText )
	{
		ClMIT_SysInfo::ANSIToASCII(m_pcBuff, szNChar) ;
	}

	ClMIT_Str::OverStringValid(m_pcBuff,StENG_TaskEnv::DvcESC_MAX_SIZE,szNChar) ;

	if (ai_pstPI) {

		ai_pstPI->m_szNChars=szNChar ;

		if (ai_pstPI->m_szNChars) {

			StPi_t *pi ;

	
			// Añado el Pi, offset da igual (RAM=modif este item, FILE=actualizar en EOP)

			pi = m_oOutRepository.AddNewPi( ai_pstPI->m_oIdPi.InChar(0), ai_iPage, 
				0, ai_pstPI->m_szNChars ) ;

			// Ahora a la lista auxiliar!!!

			NewEntry(ai_iPage, ai_iX, ai_iY, m_pcBuff, szNChar, pi ) ;


		} else {

			//Como si no hubiese PI

			NewEntry(ai_iPage, ai_iX, ai_iY, m_pcBuff, szNChar, NULL ) ;
		} ;
		


	} else {

		NewEntry(ai_iPage, ai_iX, ai_iY, m_pcBuff, szNChar, NULL ) ;

	} ;

}



void  ClENG_DvcESC::PrintNCharPi(ClMIT_Str *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		 size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams ;
	ClMIT_Str StrAux ;

	// Creo la cadena con tantas X como ai_sNChar indica
	StrAux.StrDuplicateChar(0,ai_szNChar,'X') ;

	// PrintNChar añadira el PI
	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage,ai_iX,ai_iY,StrAux,ai_szNChar, ai_bRawText, ai_pcHyperlink, ai_iTargetID, &PIParams);
}




void ClENG_DvcESC::ResolvePi(ClMIT_Str *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;

	StPi_t piData ;
	Data_t *ESC_Data ;
	
	m4pchar_t pcString;
	
	// Tomo el Pi a imprimir ( le digo que imprima el error si no lo encuentra)
	if ( m_oOutRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ) )
	{
		// UNICODE REPORTS. Bug 163475
		// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
		// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
		// wide char.
		Len = -1;
		pcString = M4CppToANSI(ai_pcStr, (int&)Len);
	    if (Len > piData.m_NBytes) {

		   // Sacar mensaje de que se va a truncar
		   *m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCESC_MSG_30 ] ) 
				<< (m4int16_t)piData.m_NPage << (m4uint32_t)piData.m_NBytes 
				<< ai_pcStr << SEND_MSG ;
		   //Efecto Local
		}
			
	   // Creo la cadena a imprimir
		// -- si sobra espacion relleno con blancos hasta PiNBytes 
		// -- si falta se trunca
		StrAux.Accommodate(piData.m_NBytes+1) ;
 		Len = sprintf(StrAux.InChar(0),"%-*.*s", piData.m_NBytes, piData.m_NBytes, pcString);
		StrAux.OverStringValid() ;	//No haria falta, se pone por si acaso

		delete [] pcString;

		/*if (! ai_bRawText )*/ ClMIT_SysInfo::ANSIToASCII(StrAux) ;

	    // Imprimo la cadena asociada en RAM o en fichero, según dónde de encuentre el Pi
		if ( piData.m_Place == StPi_t::PI_IN_RAM )  {  

			// Si este en RAM, buscar en la lista el item asociado y copiar el valor
			// El EOP volcara a buffer a posteriori.
			// Marcar en la lista el pi a NULL, ya que se ha resuelto

			Map_t::iterator it;
			
			for ( it = m_poPageESC[ piData.m_NPage ].Map.begin() ; 
				it != m_poPageESC[ piData.m_NPage ].Map.end(); ++it ) 
			{
				ESC_Data = &(*it).second ;
				if ( ESC_Data->m_pi && ESC_Data->m_PiHandler == piData.m_Handler )  {
					ESC_Data->m_oStr.StrNCpy(StrAux.InChar(0), ESC_Data->m_NBytes ) ;
					ESC_Data->m_pi = NULL ;
					break ;
				} ;
			} ;

		} else { 
			// Imprimir directamente en el fichero
			m4bool_t bWasClosed=M4_FALSE;
			//Tomo la posicion actual del fichero, y me posiciono en el offset
			if(  ! piData.m_poFile->IsOpen() ) {
				bWasClosed=M4_TRUE;
				piData.m_poFile->Open( ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ;
			};

			PosActual = piData.m_poFile->GetPos();
			piData.m_poFile->SetPos( piData.m_Offset );

			// Imprimo y dejo en fichero en la posicion inicial
			piData.m_poFile->W(StrAux.InChar(0), piData.m_NBytes);
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed){
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			};
		} ;

	} ;
}
 
void ClENG_DvcESC::EOP(m4int16_t ai_iPage) 
{
	Map_t::iterator Item ;
	ClENG_PageESC * Page = &m_poPageESC[ai_iPage] ;
	DvcUnit_t X ;
	ClFONT_M_Font *LastFont=NULL ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;
	m4uint32_t jSkipped, jPageSize ;

	Point_t KK1 ;
	m4uint8_t KK2 ;

	DvcUnit_t itY ;
	Data_t *itData ;

	// Escribe tamaño de pagina 
	KK1=ClMetric::DvcToInch(GetSizeY(ai_iPage)) ;
	KK2=KK1 ;
	if ( KK1-((long)KK1) > 0.5 ) ++KK2 ; 
	
	jSkipped = 0 ;
	if ( Page->m_JPageSize ) {
		jPageSize = Page->m_JPageSize ;
	} else {
		jPageSize = ClMetric::DvcToInch( GetSizeY(ai_iPage) )*m_iLFFactor ;
	} ;

	//sprintf(m_pcBuff, ESC_PAGE_SIZE, (unsigned char)KK2 ) ;
	//m_pcBuff[ESC_PAGE_SIZE_L-1]=(unsigned char)KK2;//No hace el printf por el FIn de cadena !!!
	//F->W( m_pcBuff, ESC_PAGE_SIZE_L) ;

	for ( Item=Page->Map.begin(); Item != Page->Map.end(); Item++ ) {
		
		itY = (*Item).first ;
		itData = &(*Item).second ;

		//	Primero salta de Y, si debe de hacerlo.
		
		//Contamos jSkipped, que es el espacio real que la impresora mueve.
		//Hay que ser muy exacto, al no poder hacer EOP, porque cualquier error se 
		//	acumulara pagina a pagina.¡ al hacer EOPs manuales saltando con EscJ

		SkipToJ( &jSkipped, ClMetric::DvcToInch( itY )*m_iLFFactor, PageBuffer ) ;

		//	Ahora me muevo en el eje X.

		X= (DvcUnit_t)(ClMetric::DvcToInch( itData->m_X )*m_iXFactor) ;
		sprintf( m_pcBuff, ESC_GOX, 
			(unsigned char)((m4uint32_t)X%256), (unsigned char)((m4uint32_t)X/256)  ) ;

		PageBuffer->Append( m_pcBuff, ESC_GOX_L ) ;

		// Cambio de fuente, si debo
		//OJO!! invalida Dvc_g_pcBuff, y por ello m_pcBuff

		if ( LastFont != (*Item).second.m_poFont ) {
			LastFont = (*Item).second.m_poFont ;
			SelectFontAux(ai_iPage, LastFont, PageBuffer ) ;
		} ;

		// Actualizo los Pi para que apunten al offset actual
		if ( itData->m_pi ) {
			// Actualizar el offset
			itData->m_pi->m_Offset = PageBuffer->GetTotal() ;
		} ;

		//	Y por ultimo escribo la cadena

		PageBuffer->Append( itData->m_oStr, itData->m_NBytes ) ;

	} ;
	Page->Map.clear() ;

	// Inserta un EOP que si sera grabado.
	//F->W(ESC_EOP,ESC_EOP_L);
	
	//Saltamos hasta el final de pagina. Ponderamos en Js restantes a saltar.

	SkipToJ( &jSkipped, jPageSize, PageBuffer ) ;
	
	ClENG_OutDevice::EOP(ai_iPage) ;
}



