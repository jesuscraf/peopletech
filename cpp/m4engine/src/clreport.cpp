//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clreport.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Parte LOADER de la capa Core
//
//
//==============================================================================

#include "eng.h"
#include "engdf.inl"

#include "mitdates.h"
#include "analex.h"
#include "sysinfo.h"

#include "cldvcpsc.h"
#include "cldvcpdf.h"
#include "cldvcpdfuni.h"
#include "cldvcpcl.h"
#include "cldvchtml.h"
#include "cldvcrvw.h"
#include "cldvctxt.h"
#include "cldvcesc.h"
#include "clreparg.h"
#include "barcode.h"

#include "loader.hmg"
//#include "core.hmg"
#include "core.h"

#include "designin.h"
#include "designdt.h"

#include "rasterg.h"

//=============== DEFINES PARA EL ORDER BY ====================
#define ORDER_BY	"ORDER BY "
#define ITEM		"@"
#define SEPARATOR	", "
#define ASCENDENT	" ASC"
#define DESCENDENT	" DESC"
//=============== FIN DE LOS DEFINES DEL ORDER BY =============

//============================================================================ClENG_Report

ClENG_Report::ClENG_Report()
{
	m_poInsp = GET_INSP() ;
	m_poRoot=NULL ;
	m_oCntPage=NULL ;
	m_oTotalize=NULL ;
	m_oCT=NULL ;
	m_bSwPublish=M4_FALSE ;
	m_bEvaluateExpressionByBreak=M4_FALSE ;
} 

void ClENG_Report::End()
{
	m4int16_t B;

	if (m_oCntPage) {
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_oCntPage, 
			ClENG_CntGroup, m_iNContainers ) ;
	} ;

	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poRoot ) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
		m_oTotalize, ClENG_Tot, m_iCountTotalize
	) ;
	m_oTotIndex.End() ;


	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
		m_oCT, ClENG_KeyTot, m_oCTIndex.GetCount() ) ;
	m_oCTIndex.End() ;
	
	m_poBreakSec = NULL ;

	m_oReportEnv.m_poDBFont->End() ;
	m_oReportEnv.m_poDBFormat->End() ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc ) ;
	m_oFieldEnv.End() ;
	m_oReportEnv.m_poChnData->End() ;
	m_oReportEnv.m_poExeParams->End() ;
	m_oReportEnv.m_poFieldManager->End() ;	
	m_oFileInterest.End() ;

	
	m_bSwPublish=M4_FALSE ;

}

m4bool_t ClENG_Report::LoadDBFont(ClENG_DesignTree *ai_poDT)
{
	m4bool_t SwOk=M4_TRUE ;
	ClFONT_M_Font::LOAD_INFO TypeInfo ;
	ClFONT_M_Font *Font ;
	m4int16_t B ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDBFont()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	switch ( m_oReportEnv.m_poExeParams->m_Dvc )
	{
	case ClENG_ReportRequest::DVC_TXT:
	case ClENG_ReportRequest::DVC_TXT_UNICODE:
	
		for ( B=0, TypeInfo=ClFONT_M_Font::PCL_INFO; B < ai_poDT->m_oDBFont.GetNumFonts() ;++B ) {
			if ( ! ai_poDT->m_oDBFont.GetAddressFontByIndex(B)->
				IsImplementedFor( ClFONT_M_Font::PCL_INFO ) ) 
			{
				TypeInfo=ClFONT_M_Font::PSC_INFO ;
				break ;
			} ;

		} ;
		
		break ;

	case ClENG_ReportRequest::DVC_PCL:
		TypeInfo=ClFONT_M_Font::PCL_INFO ;
		break ;
	case ClENG_ReportRequest::DVC_PSC:
		TypeInfo=ClFONT_M_Font::PSC_INFO ;
		break ;
	case ClENG_ReportRequest::DVC_ESCP:
		TypeInfo=ClFONT_M_Font::ESCP_INFO ;
		break ;
	case ClENG_ReportRequest::DVC_RVW:
		TypeInfo=ClFONT_M_Font::RVW_INFO ;
		break ;
	case ClENG_ReportRequest::DVC_HTML:
		TypeInfo=ClFONT_M_Font::RVW_INFO ;
		break ;
	case ClENG_ReportRequest::DVC_PDF:
	case ClENG_ReportRequest::DVC_PDF_UNICODE:
		TypeInfo=ClFONT_M_Font::PSC_INFO ;
		break ;
	} ;

	SwOk = M4_BOOL( 
		      SwOk && m_oReportEnv.m_poDBFont->Init( &ai_poDT->m_oDBFont, TypeInfo, 
		      ClMIT_Trace::PROPAGATE_ERROR ) 
		   );
	if (!SwOk) goto exit ;

	for ( B=0; B < m_oReportEnv.m_poDBFont->GetNumFonts() ;++B ) {

		Font = m_oReportEnv.m_poDBFont->GetAddressFontByIndex(B) ;
		
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"IdFont %0:s FaceName %1:s DesignHeight %2:s points "
				"FixedSpace %3:s Underline %4:s Bold %5:s Italic %6:s RFN File in %7:s "
				"DesignHeight %8:s MaxHeight %8:s DesignMaxWidth %10:s MaxWidth %11:s" ) 			
				<< Font->GetId() << Font->GetFaceName() 
				<< Font->GetDvcDesignHeight() << Font->IsFixedSpace() 
				<< Font->IsUnderline() << Font->IsBold() << Font->IsItalic() 
				<< Font->GetRFN_FileName()
				<< Font->GetDvcDesignHeight()
				<< Font->GetDvcHeight()
				<< Font->GetDvcDesignMaxWidthChar()
				<< Font->GetDvcMaxWidthChar()			
				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif
	} ;

exit:
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDBFont()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}


m4bool_t ClENG_Report::LoadDBFormat(ClENG_DesignTree *ai_poDT)
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B, B2 ;
	ClENG_Format *Format ;
	ClENG_DesignTree::FormatList_t::iterator It ;
	StENG_DD_FormatData::PropertieList_t::iterator ItP ;
	ClMIT_IdNumDesc IdDesc ;

	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDBFormat", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1020 ] ) << END_LOC ;

	m_oReportEnv.m_poDBFormat->Init( ai_poDT->m_oFormatList.size() );

	for ( It=ai_poDT->m_oFormatList.begin(), B=0 ;  It != ai_poDT->m_oFormatList.end() ; ++It, ++B ) {

		IdDesc.Init((*It).m_uiIdFormat, (*It).m_oFormatName) ;

		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1030 ] ) 
			<< IdDesc << END_LOC ;

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"New format, Index %0:s Id %1:s NameFormat %2:s") 
				<< B << (*It).m_uiIdFormat 
				<< (char*)((*It).m_oFormatName)
				<< DETAILED_MSG << SEND_MSG   ;
			m_poInsp->m_poTrace->Indent(1) ;
		};
		#endif

		m_oReportEnv.m_poDBFormat->Save( B, &IdDesc, (*It).m_oIdCurrency ) ;
		Format=m_oReportEnv.m_poDBFormat->FindFormat( (*It).m_uiIdFormat ) ;

		for ( ItP=(*It).m_oProperties.begin(); ItP!=(*It).m_oProperties.end(); ++ItP ) {
			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Seting format propertie %0:s to %1:s") 
					<< (*ItP).m_oName.InChar(0) << (*ItP).m_oValue.InChar(0)
					<< DETAILED_MSG << SEND_MSG   ;
			};
			#endif

			Format->SetProperty( (*ItP).m_oName, (*ItP).m_oValue ) ;
		} ;

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			m_poInsp->m_poTrace->Unindent(1) ;	
		};
		#endif

		*m_poInsp->m_poTrace << POP_LOC ;

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDBFormat", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}


m4bool_t ClENG_Report::LoadDvc(
	ClENG_DesignTree *ai_poDT, m4int16_t ai_PageLayoutCount, m4bool_t ai_bReplicatePages)
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClENG_PagePrinter::PAPER_SOURCE PaperSource;
	m4int32_t PaperSourceCode = -1;
	StFileParams stFile ;
	StENG_DT_PageData *Page ;
	m4pchar_t pcBreakExpressionPrefix = "\"*\"&&";
	m4int16_t iBreakExpressionPrefixLength = strlen( pcBreakExpressionPrefix ) ;

	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDvc()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1040 ] ) << END_LOC ;

	//--------->Calculo FileParams
	
	stFile.Init( 
		&ai_poDT->m_oGlobal.m_stDefaultFileParams, 
		&m_oReportEnv.m_poExeParams->m_FileParams,
		m_Break==ClENG_Report::NO_BREAK ? 0 : -1  ) ;

	// es expresión sólo si especificó en de las propiedades
	// si vino de parámetros no y no se ha especificado ignorarlas
	if (ai_poDT->m_oGlobal.m_stDefaultFileParams.m_bPathLoaded &&
		!m_oReportEnv.m_poExeParams->m_FileParams.m_bIgnoreFilePathPref)
	{
		ClENG_Expression oExpr ;
		ClENG_Analex::StExpressionEnv StEnv ;
		ClENG_Analex Analex ;
		ClMIT_ChnValue res ;

		// se inicializa el entorno
		StEnv.m_poChn=m_oReportEnv.m_poChnData ;

		// Sólo se evalua expresiones por ruptura con FILE (no con PATH)
		if (ai_poDT->m_oGlobal.m_stDefaultFileParams.m_bIsFile &&
			strnicmp(stFile.m_oPath.InChar(0), pcBreakExpressionPrefix, iBreakExpressionPrefixLength) == 0)
		{
			// Ruptura por bloque y reevaluación
			stFile.m_oFileNameExpression.StrCpy(stFile.m_oPath.InChar(iBreakExpressionPrefixLength));
			stFile.m_poChnData = m_oReportEnv.m_poChnData;
			// Se analiza ya aqui la expresión, pero no se evalua, para comprobar que es correcta
			oExpr.Init(stFile.m_oFileNameExpression.InChar(0),"Path expression") ;
			SwOk = Analex.Init(&StEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR);
			if (!SwOk) goto exit ;
			if (m_oReportEnv.m_poExeParams->m_FileParams.m_bPathLoaded)
			{
				stFile.m_oPath.StrCpy(m_oReportEnv.m_poExeParams->m_FileParams.m_oPath.InChar(0));
				// Se debe actualizar m_bIsFile. Si viene expresión será True siempre
				// pero realmente debe utilizar m_oReportEnv.m_poExeParams ya que en
				// caso contrario no funcionaría con rutas absolutas
				stFile.m_bIsFile = m_oReportEnv.m_poExeParams->m_FileParams.m_bIsFile;
			}
			else
			{
				stFile.m_oPath.StrCpy("");
			}
			// Flag para condicionar código nuevo sólo cuando sea aplicable
			m_bEvaluateExpressionByBreak = M4_TRUE;
		}
		else
		{
			m_bEvaluateExpressionByBreak = M4_FALSE;
			oExpr.Init(stFile.m_oPath.InChar(0),"Path expression") ;
			SwOk = Analex.Init(&StEnv, &oExpr, ClMIT_Trace::PROPAGATE_ERROR);
			if (!SwOk) goto exit ;

			SwOk = oExpr.Evaluate (&res, ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!SwOk) goto exit ;
				
			
			stFile.m_oPath.StrCpy(res.GetString()) ;
		}

	} ;

	//<---------Calculo FileParams

	//---------->Carga Dvc
	switch (m_oReportEnv.m_poExeParams->m_Dvc) 
	{
	case ClENG_ReportRequest::DVC_PCL:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a PCL device" 
				"OutputPath <%0:s.%1:s> Preserve %2:s NumLayoutPages %3:s Resolution %4:s "
				"Copies %5:s Duplex %6:s Delete %7:s") 

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve
				<< ai_PageLayoutCount
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Resolution << m_oReportEnv.m_poExeParams->m_iCopies
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex << m_oReportEnv.m_poExeParams->m_bDelete
				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcPCL ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPCL )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			& ai_poDT->m_oFGPalette,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_Duplex, 
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Param especificos de device

			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_bSwColour, 
			m_oReportEnv.m_poExeParams->m_iCopies,
			m_oReportEnv.m_poExeParams->m_Resolution, 
			m_oReportEnv.m_poExeParams->m_Tumble,

			ClMIT_Trace::PROPAGATE_ERROR
		);

		break;

	case ClENG_ReportRequest::DVC_PSC:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a PSC device" 
				"OutputPath <%0:s.%1:s> Preserve %2:s NumLayoutPages %3:s Resolution %4:s "
				"Copies %5:s Duplex %6:s Delete %7:s") 

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve
				<< ai_PageLayoutCount
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Resolution << m_oReportEnv.m_poExeParams->m_iCopies
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex << m_oReportEnv.m_poExeParams->m_bDelete
				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcPSC ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPSC )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			& ai_poDT->m_oFGPalette,

		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_Duplex, 
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Especificos del device
	
			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_PSCResolution, 
			m_oReportEnv.m_poExeParams->m_iCopies,

			ClMIT_Trace::PROPAGATE_ERROR
		);

		break;

	case ClENG_ReportRequest::DVC_ESCP:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a ESCP device "
				"OutputPath %0:s.%1:s Preserve %2:s Delete %3:s NumLayoutPages %4:s "
				"Duplex %5:s XFactor %6:s YFactor %7:s JPageSize %8:s" )

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve << m_oReportEnv.m_poExeParams->m_bDelete
				<< ai_PageLayoutCount << (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex
				<< m_oReportEnv.m_poExeParams->m_iXFactor << m_oReportEnv.m_poExeParams->m_iYFactor
				<< m_oReportEnv.m_poExeParams->m_JPageSize.InChar(0)

				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcESC ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcESC )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_Duplex, 

			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Campos especificos de este Dvc

			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_iXFactor, 
			m_oReportEnv.m_poExeParams->m_iYFactor,
			m_oReportEnv.m_poExeParams->m_JPageSize.InChar(0),

			ClMIT_Trace::PROPAGATE_ERROR
		) ;
		break;

	case ClENG_ReportRequest::DVC_TXT:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a TXT device"
				"OutputPath %0:s.%1:s Preserve %2:s Delete %3:s NumLayoutPages %4:s "
				"Duplex %5:s m_uiFont %6:s SwSelectFirstFont %7:s IgnoreTxtWarning: %8:s" )

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve << m_oReportEnv.m_poExeParams->m_bDelete
				<< ai_PageLayoutCount << (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex
				<< m_oReportEnv.m_poExeParams->m_uiIdFont 
				<< M4_BOOL(m_oReportEnv.m_poExeParams->m_uiIdFont==-1) 
				<< ai_poDT->m_oGlobal.m_bIgnoreTxtWarning

				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcTxt ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcTxt )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_Duplex, 
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Especificos del device

			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_uiIdFont, 
			ai_poDT->m_oGlobal.m_bIgnoreTxtWarning,
			M4_BOOL( m_oReportEnv.m_poExeParams->m_uiIdFont == -1 ),
			m_oReportEnv.m_poExeParams->m_oEOLSeq, 
			m_oReportEnv.m_poExeParams->m_oEOPSeq,
			m_oReportEnv.m_poExeParams->m_cFillChar, 
			m_oReportEnv.m_poExeParams->m_bClipColumn, 
			m_oReportEnv.m_poExeParams->m_bTXTSwPageFill,

			ClMIT_Trace::PROPAGATE_ERROR 
		) ;
		break;

	case ClENG_ReportRequest::DVC_TXT_UNICODE:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a TXT UNICODE device"
				"OutputPath %0:s.%1:s Preserve %2:s Delete %3:s NumLayoutPages %4:s "
				"Duplex %5:s m_uiFont %6:s SwSelectFirstFont %7:s IgnoreTxtWarning: %8:s" )

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve << m_oReportEnv.m_poExeParams->m_bDelete
				<< ai_PageLayoutCount << (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex
				<< m_oReportEnv.m_poExeParams->m_uiIdFont 
				<< M4_BOOL(m_oReportEnv.m_poExeParams->m_uiIdFont==-1) 
				<< ai_poDT->m_oGlobal.m_bIgnoreTxtWarning

				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcTxtUnicode ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcTxtUnicode )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_Duplex, 
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Especificos del device

			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_uiIdFont, 
			ai_poDT->m_oGlobal.m_bIgnoreTxtWarning,
			M4_BOOL( m_oReportEnv.m_poExeParams->m_uiIdFont == -1 ),
			m_oReportEnv.m_poExeParams->m_oEOLSeq, 
			m_oReportEnv.m_poExeParams->m_oEOPSeq,
			m_oReportEnv.m_poExeParams->m_cFillChar, 
			m_oReportEnv.m_poExeParams->m_bClipColumn, 
			m_oReportEnv.m_poExeParams->m_bTXTSwPageFill,
			m_oReportEnv.m_poExeParams->m_bPreprocessForExcel,
			m_oReportEnv.m_poExeParams->m_cFieldDelimiter,

			ClMIT_Trace::PROPAGATE_ERROR 
		) ;
		break;


	case ClENG_ReportRequest::DVC_RVW:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a RVW device "
				"OutputPath %0:s.%1:s Preserve %2:s Delete %3:s NumLayoutPages %4:s " )

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve << m_oReportEnv.m_poExeParams->m_bDelete 
				<< ai_PageLayoutCount

				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcRVW ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcRVW )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			&ai_poDT->m_oFGPalette,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			ClMIT_Trace::PROPAGATE_ERROR
		);
		break;


	case ClENG_ReportRequest::DVC_HTML:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating an HTML device" 
				"OutputPath <%0:s.%1:s> Preserve %2:s NumLayoutPages %3:s Resolution %4:s "
				"Copies %5:s NavigationMode %6:s Menu %7:s Delete %8:s") 

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve
				<< ai_PageLayoutCount
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Resolution << m_oReportEnv.m_poExeParams->m_iCopies
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_HTMLNavigation 
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_HTMLMenu
				<< m_oReportEnv.m_poExeParams->m_bDelete
				<< SEND_MSG   ;
		};
		#endif

		m4language_t Lang;
		GET_TASK_ENV()->m_APIChn.m_poMVC->GetLanguage(&Lang);

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcHTML ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcHTML )->Init( 
			
			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			&ai_poDT->m_oFGPalette,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,	//ForceMonoBreak
			&stFile, 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Especificos de device

			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_bTableOfContents, 
			m_oReportEnv.m_poExeParams->m_HTMLNavigation, 
			m_oReportEnv.m_poExeParams->m_HTMLMenu,
			m_oReportEnv.m_poExeParams->m_bHTMLZoom, 
			m_oReportEnv.m_poExeParams->m_bHTMLSearch, 
			Lang,
			GetReportName(),
			m_oReportEnv.m_poExeParams->m_oHTML_StartTemplate,
			m_oReportEnv.m_poExeParams->m_oHTML_EndTemplate,
			m_oReportEnv.m_poExeParams->m_oHTML_BreakTemplate,

			ClMIT_Trace::PROPAGATE_ERROR
		);
		break;


	case ClENG_ReportRequest::DVC_PDF:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a PDF device" 
				"OutputPath <%0:s.%1:s> Preserve %2:s NumLayoutPages %3:s Resolution %4:s "
				"Copies %5:s Duplex %6:s Delete %7:s") 

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve
				<< ai_PageLayoutCount
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Resolution << m_oReportEnv.m_poExeParams->m_iCopies
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex << m_oReportEnv.m_poExeParams->m_bDelete
				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcPDF ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPDF )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			&ai_poDT->m_oFGPalette,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&(stFile), 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Campos especificos
	
			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_bTableOfContents, 
		
			ClMIT_Trace::PROPAGATE_ERROR
		);
			
		break;

	case ClENG_ReportRequest::DVC_PDF_UNICODE:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Creating a PDF Unicode device" 
				"OutputPath <%0:s.%1:s> Preserve %2:s NumLayoutPages %3:s Resolution %4:s "
				"Copies %5:s Duplex %6:s Delete %7:s") 

				<< stFile.m_oPath.InChar(0) << stFile.m_oExtension.InChar(0)
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Preserve
				<< ai_PageLayoutCount
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Resolution << m_oReportEnv.m_poExeParams->m_iCopies
				<< (m4int16_t)m_oReportEnv.m_poExeParams->m_Duplex << m_oReportEnv.m_poExeParams->m_bDelete
				<< SEND_MSG   ;
		};
		#endif

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc, ClENG_DvcPDFUnicode ) ;
		SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPDFUnicode )->Init( 

			//Campos para ClENG_OutDevice::Init

			ai_PageLayoutCount,
			&ai_poDT->m_oFGPalette,
		
			//Campos para ClENG_ReportOuputRepository::Init
			// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

			m_Break==ClENG_Report::NO_BREAK,
			&(stFile), 
			m_oReportEnv.m_poExeParams->m_bDelete,
			m_oReportEnv.m_poExeParams->m_DeclareExtFile,
			m_oReportEnv.m_poExeParams->m_bCreateDir,
			m_oReportEnv.m_poExeParams->m_Preserve, 
			m_oReportEnv.m_poExeParams->m_oToPrinter, 

			//Campos especificos
	
			m_oReportEnv.m_poDBFont, 
			m_oReportEnv.m_poExeParams->m_Tumble,
			m_oReportEnv.m_poExeParams->m_bTableOfContents,
			m_oReportEnv.m_poExeParams->m_bForceUnicodeOutput, // Fuerza la utilización de fuentes Unicode (para textos)
			
			ClMIT_Trace::PROPAGATE_ERROR
		);
			
		break;

	} ;
	if (!SwOk) goto exit ;

	m_oFieldEnv.m_poDvc = m_oReportEnv.m_poDvc ;

	//<----------Carga Dvc

	//---------->Carga datos pagina
	
	for ( B=0 ; B < ai_PageLayoutCount ; ++B ) {

		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1050 ] ) 
			<< (m4int16_t)(B+1) << END_LOC ;
		
		Page = &ai_poDT->m_oPageData[ ai_bReplicatePages ? 0 : B ] ;
	
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Loading page %0:s") << B << DETAILED_MSG << SEND_MSG   ;
			m_poInsp->m_poTrace->Indent(1) ;
		};
		#endif

		//Inicializacion de las paginas
		PaperSource = m_oReportEnv.m_poExeParams->GetBin(B);
		if (m_oReportEnv.m_poExeParams->m_Dvc == ClENG_ReportRequest::DVC_PCL)
		{
			PaperSourceCode = m_oReportEnv.m_poExeParams->GetBinPcl(B);

			// Podría darse el caso en el que, estando en PCL, no se haya especificado
			// el parámetros BINPCL. En este caso, paperSource será el valor elegido por
			// el usuario (manual, upper, lower). Si el usuario ha especificado un valor
			// para BINPCL, se asigna paperSource a USER_DEFINED, para que en el device PCL
			// se construya el código de la bandeja a partir del código elegido por el 
			// usuario.
			if (PaperSourceCode != -1)
			{
				PaperSource = ClENG_PagePrinter::USER_DEFINED;
			}
		}


		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"OffsetX %0:s OffsetY %1:s Orientation %2:s PageSize %3:s PaperSource %4:s PaperSourceCode %5:s "
				"SizeX %5:s SizeY %6:s" ) 
			
				<< ClMetric::DvcToMilim(Page->m_OffsetX) << ClMetric::DvcToMilim(Page->m_OffsetY)
				<< (m4int16_t)Page->m_Orientation << (m4int16_t)Page->m_PageSize  
				<< (m4int16_t) PaperSource << PaperSourceCode
				<< ClMetric::DvcToMilim(Page->m_SizeX) << ClMetric::DvcToMilim(Page->m_SizeY)
			
				<< DETAILED_MSG << SEND_MSG   ;
			m_poInsp->m_poTrace->Unindent(1);
		};

		#endif

		switch (m_oReportEnv.m_poExeParams->m_Dvc) 
		{
		case ClENG_ReportRequest::DVC_PCL:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPCL )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_PCL_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;

			// Para PCL: establecer código de la bandeja.
			M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPCL)->SetPaperSourceCode(PaperSourceCode);

			break ;

		case ClENG_ReportRequest::DVC_PSC:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPSC )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_PSC_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ;


		case ClENG_ReportRequest::DVC_ESCP:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcESC )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_ESC_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ;

		case ClENG_ReportRequest::DVC_TXT:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcTxt )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, 
				Page->m_Orientation, Page->m_PageSize, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ;

		case ClENG_ReportRequest::DVC_TXT_UNICODE:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcTxtUnicode )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, 
				Page->m_Orientation, Page->m_PageSize, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ;

		case ClENG_ReportRequest::DVC_RVW:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcRVW )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_RVW_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ;

		case ClENG_ReportRequest::DVC_HTML:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcHTML )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_PSC_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR) ;
			break ;
			
		case ClENG_ReportRequest::DVC_PDF:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPDF )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_PDF_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ; 

		case ClENG_ReportRequest::DVC_PDF_UNICODE:

			SwOk=M4_VIRTUAL_OBJECT(m_oReportEnv.m_poDvc, ClENG_DvcPDFUnicode )->InitPage( 
				B, Page->m_OffsetX, Page->m_OffsetY, ENG_PDF_INTERBLOCK,
				Page->m_Orientation, Page->m_PageSize, PaperSource, 
				Page->m_SizeX, Page->m_SizeY, ClMIT_Trace::PROPAGATE_ERROR ) ;
			break ; 

		} ;
		if (!SwOk) break  ;

		*m_poInsp->m_poTrace << POP_LOC ;
	} ;

	// Bug 0102841. Establecer si existen en el device páginas replicadas o no.
	m_oReportEnv.m_poDvc->SetReplicatePages(ai_bReplicatePages);

	//<-----------Carga datos pagina

	//Llamada a PostPagesInit

	if ( ! m_oReportEnv.m_poDvc->PostPagesInit() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

exit:

	if (!SwOk) 	{
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc ) ;
		m_oFieldEnv.m_poDvc=NULL ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadDvc()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

m4bool_t ClENG_Report::LoadCT( 
	ClENG_DesignTree *ai_poDT, m4char_t *ai_pcCTId , ClENG_KeyTot *ai_poKeyTot, m4int16_t *ao_piPageCount ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData *CT ;
	ClMIT_Args Arg ;
	ClMIT_Str Str ;
	m4int16_t B, Count ;
	StCTLevelData *stData=NULL ;
	ClENG_CntSample CTSample ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1120 ] ) 
		<< ai_pcCTId << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadCT() CT %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << SEND_MSG   ;
	};
	#endif

	//Busco datos CT

	CT = ai_poDT->FindCT(ENG_EXIST_REQUIRED,ai_pcCTId);
	if (!CT) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Inicializo KeyTot

	SwOk = M4_BOOL( SwOk && ai_poKeyTot->Init( 
		CT->m_iLevelCount, CT->m_oTreeList, CT->m_oTotList, ClMIT_Trace::PROPAGATE_ERROR) ) ;
	if (!SwOk) goto exit ;


	//Cargo lista Trees 

	Arg.Init( CT->m_oFillTree, '.', " ," ) ; 
	Count=Arg.GetArgCount()  ;

	//Construccion del array de Nodos e Items

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, stData, StCTLevelData, Count ) ;
	for (B=0;B<Count;++B) {
		Arg.GetString(B,0,&Str) ;
		stData[B].NodeKey=m_oReportEnv.m_poChnData->GetNode(Str,ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!stData[B].NodeKey) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		SwOk=SwOk&&Arg.GetString(B,1,&Str,M4_FALSE) ;
		if (!SwOk) goto exit ;
		SwOk=SwOk && ( stData[B].ItemKey.Init(stData[B].NodeKey, Str, 
			ClMIT_Trace::PROPAGATE_ERROR ) == ClMIT_ChnItem::ITEM_OK ) ;
		if (!SwOk) goto exit ;

		if ( Arg.GetSubArgCount(B)==2  ) {
			stData[B].NodeHeader=stData[B].NodeKey ;
			stData[B].ItemHeader.Init(&stData[B].ItemKey) ;
		} else {
			if (Arg.GetSubArgCount(B)>3) {
				SwOk=SwOk&&Arg.GetString(B,2,&Str,M4_FALSE) ;
				stData[B].NodeHeader=m_oReportEnv.m_poChnData->GetNode(Str,ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!stData[B].NodeHeader) {
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				SwOk=SwOk&&Arg.GetString(B,3,&Str,M4_FALSE) ;
			} else {
				stData[B].NodeHeader=stData[B].NodeKey ;
				SwOk=SwOk&&Arg.GetString(B,2,&Str,M4_FALSE) ;
			} ;
			if (!SwOk) goto exit ;
			SwOk=SwOk && ( stData[B].ItemHeader.Init(stData[B].NodeHeader, Str, 
				ClMIT_Trace::PROPAGATE_ERROR ) == ClMIT_ChnItem::ITEM_OK ) ;
			if (!SwOk) goto exit ;
		} ;
	} ;

	//Recorrido y carga del arbol de  claves

	SwOk=SwOk&&LoadCTLevel(ai_poKeyTot, stData, 0, Count-1 ) ; 
	if (!SwOk) goto exit ;

	//Carga de derivados

	SwOk=SwOk && ai_poKeyTot->InitDerived( CT->m_oDerivedList,ClMIT_Trace::PROPAGATE_ERROR) ; 
	if (!SwOk) goto exit ;

	ai_poKeyTot->Trace() ;

	//Sample para obtener el numero de paginas que ocupa

	SwOk = M4_BOOL( SwOk && CTSample.Init(
			ai_poDT, ai_pcCTId, ai_poKeyTot, "", M4_FALSE, CT->m_iTogLevel, M4_FALSE,
			ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) goto exit ;

	*ao_piPageCount = CTSample.GetLayoutPageCount() ;

exit:

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, stData, StCTLevelData, Count ) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadCT() CT %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcCTId << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;

	return SwOk ;
}

m4bool_t ClENG_Report::LoadCTLevel( ClENG_KeyTot *ai_poKeyTot,
	StCTLevelData *ai_poLevel, 
	m4int16_t ai_iActLevel, m4int16_t ai_iTopLevel ) 
{
	if (ai_iActLevel>ai_iTopLevel) return M4_TRUE ;

	ai_poLevel[ai_iActLevel].NodeKey->GoFirst() ;	
	while ( ! ai_poLevel[ai_iActLevel].NodeKey->IsEOF() ) {
		ai_poLevel[ai_iActLevel].ItemKey.Load() ;		
		ai_poLevel[ai_iActLevel].ItemHeader.Load() ;		
		ai_poKeyTot->GetValueKey(ai_iActLevel)->Init(&ai_poLevel[ai_iActLevel].ItemKey.m_oValue) ;
		if ( !ai_poKeyTot->NewKey(ai_iActLevel+1,&ai_poLevel[ai_iActLevel].ItemHeader.m_oValue ) ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_140 ] ) << SEND_MSG ;
			return M4_FALSE ;
		} ;
		if (ai_iActLevel!=ai_iTopLevel) {
			if ( !LoadCTLevel(ai_poKeyTot,ai_poLevel,ai_iActLevel+1,ai_iTopLevel) ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ LOADER_MSG_140 ] ) << SEND_MSG ;
				return M4_FALSE ;
			} ;
		} ;
		ai_poLevel[ai_iActLevel].NodeKey->Next() ;
	} ;

	return M4_TRUE ;
}


ClENG_Container * ClENG_Report::CreateContainer( 
	StENG_DT_CntData *ai_poCntData, DvcUnit_t ai_OffsetTop, DvcUnit_t ai_OffsetLeft,
	m4bool_t ai_bSwTestCnt, m4bool_t  ai_bSwTestField ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_Container *Cnt=NULL ; 
	ClENG_Field *Field=NULL ;
	ENG_DT_FieldList_t::iterator ItF ;
	DvcUnit_t Height, Width ;
	ClMIT_Str setFrame ;


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::CreateContainer()", ClMIT_Msg::ENTRY_FORMAT) 
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	Height = m_oReportEnv.m_poDvc->AdjustHeight(ai_poCntData->m_iPage,ai_poCntData->m_Height) ;
	Width  = m_oReportEnv.m_poDvc->AdjustWidth( ai_poCntData->m_iPage,ai_poCntData->m_Width ) ;

	setFrame.StrCpy( ! ai_bSwTestCnt ? (m4pchar_t) ai_poCntData->m_oRegionBorder :
		(m4pchar_t) "010000100000010000100000010000100000010000100000" ) ;

	M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_CORE, Cnt, ClENG_Container ) ;

	if ( ai_poCntData->m_IsFixed ) {

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Fixed container Page %0:s X %1:s Y %2:s Height %3:s Width %4:s NumFields %5:s "
				"MergeMode %6:s MergeCT %7:s "
				"Cancel ? %8:s ModeMethod %9:s SetFrame %10:s FillColour %11:s") 
			
				<< ai_poCntData->m_iPage
				<< ClMetric::DvcToMilim(ai_poCntData->m_Left) << ClMetric::DvcToMilim(ai_poCntData->m_Top)
				<< ClMetric::DvcToMilim(Height) << ClMetric::DvcToMilim(Width)
				<< (m4int16_t)ai_poCntData->m_oFieldList.size() 
				<< (m4int16_t)ai_poCntData->m_MergeMode 
				<< ai_poCntData->m_oCTId.InChar(0) << 0L << 0L 
				<< setFrame.InChar(0) << ai_poCntData->m_uiRegionColour
			
				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif

		M4_VIRTUAL_OBJECT(Cnt,ClENG_Container)->InitFixed( 
			&m_oReportEnv,
			ai_poCntData->m_iPage, ai_poCntData->m_Left, ai_poCntData->m_Top, 
			Height,  Width,
			ai_poCntData->m_oCancelCnt ) ;

	} else {

		// Un container de pagina, nunca es variable.

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"Not Fixed container Page %0:s X %1:s Y %2:s Height %3:s Width %4:s "
				"SizeYLimit %5:s NumFields %6:s MergeMode %7:s MergeCT %8:s "
				"Cancel ? %9:s Together ? %10:s SetFrame %11:s FillColour %12:s" )
			
				<< ai_poCntData->m_iPage
				<< ClMetric::DvcToMilim(ai_poCntData->m_Left)
				<< ClMetric::DvcToMilim(ai_poCntData->m_Top)
				<< ClMetric::DvcToMilim(ai_poCntData->m_Height)
				<< ClMetric::DvcToMilim(ai_poCntData->m_Width)
				<< ClMetric::DvcToMilim(ai_poCntData->m_LowerLimit)
				<< (m4int16_t)ai_poCntData->m_oFieldList.size() 
				<< (m4int16_t)ai_poCntData->m_MergeMode
				<< ai_poCntData->m_oCTId.InChar(0) << 0L << ai_poCntData->m_bSwTogether
				<< setFrame.InChar(0) << ai_poCntData->m_uiRegionColour

				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif

		M4_VIRTUAL_OBJECT(Cnt,ClENG_Container)->InitNotFixed(
			&m_oReportEnv,
			ai_poCntData->m_iPage, ai_poCntData->m_Left, 
			Height, Width, 
			ai_poCntData->m_Top, ai_poCntData->m_LowerLimit,
			ai_poCntData->m_bSwTogether, ai_poCntData->m_oCancelCnt ) ;
	} ;

	Cnt->m_oRegion.SetFrame( setFrame ) ;
	Cnt->m_oRegion.SetFillColour( ai_poCntData->m_uiRegionColour ) ;

	//------------->Carga de los fields normales (NO fusionados de ninguna forma)

	for ( ItF = ai_poCntData->m_oFieldList.begin(); ItF != ai_poCntData->m_oFieldList.end(); ++ItF )
	{
		//---Si el field pertenece a un container de fusion y tiene IdCtField lo ignora.

		if ( (*ItF).m_oIdCtField.StrLenTrim()>0 &&
			ai_poCntData->m_MergeMode != ENG_CT_NO_MERGE ) continue ;

		//---Creacion del field
		
		Field = CreateField(&(*ItF), ai_OffsetTop, ai_OffsetLeft, ai_bSwTestField ) ;
		if (!Field) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		M4_VIRTUAL_OBJECT(Cnt,ClENG_Container)->InitField( Field ) ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::CreateContainer()", ClMIT_Msg::EXIT_FORMAT) 
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	if (!SwOk) {	
		M4_DELETE_VIRTUAL_ITEM( m_poInsp->m_poMemProf, ENG_MGI_CORE, Cnt ) ;
	} ;

	return Cnt ;
}


m4bool_t ClENG_Report::LoadPageContainers(
	ClENG_DesignTree *ai_poDT, m4bool_t ai_bReplicatePages)
{
	m4bool_t SwOk=M4_TRUE, SwFirst, SwEnd ;
	m4int16_t B ;
	StENG_DT_CntData *CntData ;
	ClENG_Container *Cnt ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadPageContainers()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	for (B=0; B<m_iNContainers ; ++B ) {
		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1060 ] ) 
			<< (m4int16_t)(B+1) << END_LOC ;

		CntData = & ai_poDT->m_oPageData[ ai_bReplicatePages ? 0 : B ].m_oCnt ;
		m_oCntPage[B].Init( &m_oReportEnv, "Page" ) ;

		// Bug 0100489: Update page number within container data. We need to extract
		// container data from position 0 within m_oPageData, but if we have "virtual"
		// pages because CTs do not fit within a page, these data have to be inserted
		// for page B (1, 2, ...) and not for the first page (0).
		if (ai_bReplicatePages)
		{
			CntData->m_iPage = B;
		}

		Cnt=CreateContainer( CntData, 0, 0, 
			M4_FALSE, m_oReportEnv.m_poExeParams->m_bTestField ) ;
		if (!Cnt) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		m_oCntPage[B].AddCnt(Cnt) ;

		*m_poInsp->m_poTrace << POP_LOC ;
	} ;
exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadPageContainers()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return SwOk ;
}

ClENG_Field * ClENG_Report::CreateField( StENG_DT_FieldData *ai_poFieldData, 
	DvcUnit_t ai_OffsetTop, DvcUnit_t ai_OffsetLeft, m4bool_t ai_bSwTestField ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_Field *Field=NULL ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;
	DvcUnit_t Top, Left, LineTop, LineLeft, LineTop2, LineLeft2 ;
	ClENG_Format  *Format = NULL ;
	ClFONT_M_Font *Font   = NULL ;
	ClMIT_IdNumDesc IdDesc ;
	ClMIT_Str ExprAux ;
	ClMIT_ChnItem SaveItem ;
	ClMIT_ChnNode *NodeAux ;
	ClMIT_Str SetFrame, SetLine ;
	m4char_t *frameDef ;

	IdDesc.Init( ai_poFieldData->m_uiIdField, ai_poFieldData->m_oFieldName ) ; 

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1080 ] ) 
		<< IdDesc << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::CreateField()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	//---Cargas especificas de campos

	Top			= ai_poFieldData->m_Top + ai_OffsetTop ;
	Left		= ai_poFieldData->m_Left+ ai_OffsetLeft ;
	LineTop		= ai_poFieldData->m_Y1  + ai_OffsetTop ;
	LineLeft	= ai_poFieldData->m_X1  + ai_OffsetLeft ;
	LineTop2	= ai_poFieldData->m_Y2  + ai_OffsetTop ;
	LineLeft2	= ai_poFieldData->m_X2  + ai_OffsetLeft ;

	Font	= m_oReportEnv.m_poDBFont->GetAddressFontById(  
		ai_poFieldData->m_stDefaultData.GetIdFont(), M4_FALSE ) ;
	Format	= ai_poFieldData->m_stDefaultData.GetIdFormat() == ClENG_Format::ID_NULL ?
		NULL : 
		m_oReportEnv.m_poDBFormat->FindFormat( 
		ai_poFieldData->m_stDefaultData.GetIdFormat(), ClMIT_Trace::PROPAGATE_ERROR ) ;

	SetLine.StrCpy("") ;
	//Cantara despues si es erroneo, tanto en tipos Linea como en otros.
	SetFrame.StrCpy(ai_poFieldData->m_oRegionBorder) ;	

	frameDef = !ai_bSwTestField ?  SetFrame.InChar(0) : "010000100000010000100000010000100000010000100000" ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
	
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"Field %0:s %1:s, FieldType %2:s Expression %3:s IdPi %4:s Top %5:s Left %6:s "
			"Width %7:s Height %8:s "
			"AlignX %9:s AlignY %10:s SwClip ? %11:s IdFont %12:s IdFormat %13:s "
			"Static ? %14:s Duplicated ? %15:s Optimize ? %16:s "
			"SetFrame %17:s SetLine %18:s"
			) 

			<< ai_poFieldData->m_uiIdField << ai_poFieldData->m_oFieldName.InChar(0)
			<< (m4int16_t)ai_poFieldData->m_FldType << ai_poFieldData->m_oExpr.InChar(0) 
			<< ai_poFieldData->m_oIdPi.InChar(0)
			<< ClMetric::DvcToMilim(Top) 
			<< ClMetric::DvcToMilim(Left)
			<< ClMetric::DvcToMilim(ai_poFieldData->m_Width) 
			<< ClMetric::DvcToMilim(ai_poFieldData->m_Height) 
			<< (m4int16_t)ai_poFieldData->m_stDefaultData.GetAlignX()
			<< (m4int16_t)ai_poFieldData->m_stDefaultData.GetAlignY() 
			<< ai_poFieldData->m_stDefaultData.IsClip()
			<< Font->GetId() << (m4int32_t)(Format ? Format->GetId() : -1 )
			<< ai_poFieldData->m_bSwStatic << ai_poFieldData->m_bSwDuplicated 
			<< ai_poFieldData->m_bSwOptimize
			<< SetFrame.InChar(0) << SetLine.InChar(0)
			
			<< CRAZY_MSG <<  SEND_MSG   ;
	} ;
	#endif

	switch (ai_poFieldData->m_FldType)
	{
	case ENG_FLD_STRING:

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_FieldStr ) ;
		M4_VIRTUAL_OBJECT( Field, ClENG_FieldStr )->Init( 
			&m_oFieldEnv,
			&IdDesc, Top, Left, 
			ai_poFieldData->m_Width, ai_poFieldData->m_Height,  
			ai_poFieldData->m_oExpr.InChar(0), 
			ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
			ai_poFieldData->m_oHyperlink.InChar(0),
			ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
			ai_poFieldData->m_bSwOptimize,

			frameDef, ai_poFieldData->m_uiRegionColour,
			
			ai_poFieldData->m_stDefaultData.GetAlignX(), 
			ai_poFieldData->m_stDefaultData.GetAlignY(), 
			ai_poFieldData->m_stDefaultData.GetIdColour(), Font, Format, 
			ai_poFieldData->m_stDefaultData.IsClip(), 
			ai_poFieldData->m_FillMode, ai_poFieldData->m_cFillChar,
			ai_poFieldData->m_stDefaultData.GetNULLDef(), 
			ai_poFieldData->m_stDefaultData.GetNULLFormatDef(), 
			ai_poFieldData->m_stDefaultData.IsFormatNULL(),
            ai_poFieldData->m_oIdPi
		) ;
		break;
	
	case ENG_FLD_BOX:

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_FieldStrBox ) ;
		M4_VIRTUAL_OBJECT( Field, ClENG_FieldStrBox )->Init( 
			&m_oFieldEnv,
			&IdDesc,Top, Left, 
			ai_poFieldData->m_Width,  
			ai_poFieldData->m_oExpr.InChar(0),
			ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
			ai_poFieldData->m_oHyperlink.InChar(0), ai_poFieldData->m_iMaxBoxes, 
			ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
			ai_poFieldData->m_bSwOptimize,

			frameDef, ai_poFieldData->m_uiRegionColour,

			ai_poFieldData->m_stDefaultData.GetAlignX(), 
			ai_poFieldData->m_stDefaultData.GetIdColour(), Font, Format, 
			ai_poFieldData->m_FillMode, ai_poFieldData->m_cFillChar,
			ai_poFieldData->m_stDefaultData.GetNULLDef(), 
			ai_poFieldData->m_stDefaultData.GetNULLFormatDef(), 
			ai_poFieldData->m_stDefaultData.IsFormatNULL()
		) ;
		break;

	case ENG_FLD_MULTIL:

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_FieldMultiL ) ;
		M4_VIRTUAL_OBJECT( Field, ClENG_FieldMultiL )->Init( 
			&m_oFieldEnv,
			&IdDesc, Top, Left, 
			ai_poFieldData->m_Width, 
			ai_poFieldData->m_Height, ai_poFieldData->m_Interline, 
			ai_poFieldData->m_bIsVarHeight,
			ai_poFieldData->m_bIsJustified, ai_poFieldData->m_oExpr.InChar(0), 
			ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
			ai_poFieldData->m_oHyperlink.InChar(0),	ai_poFieldData->m_bSwStatic, 
			ai_poFieldData->m_bSwDuplicated, ai_poFieldData->m_bSwOptimize, 

			frameDef, ai_poFieldData->m_uiRegionColour,

			ai_poFieldData->m_stDefaultData.GetAlignX(), 
			ai_poFieldData->m_stDefaultData.GetAlignY(), 

			ai_poFieldData->m_stDefaultData.GetIdColour(), 
			Font, ai_poFieldData->m_stDefaultData.IsClip() 
		) ;
		break;

	case ENG_FLD_RASTERG:

		if ( m_oReportEnv.m_poDvc->IsRasterSupported() )  {
			if ( ai_poFieldData->m_bEmbebedRaster ) {
				ExprAux.StrCpy("\"") ;
				if ( m_oReportEnv.m_poDvc->IsColourOn() ) {
					if ( ai_poFieldData->m_oColourRaster.StrLenTrim()!=0 ) {
						ExprAux.StrCat(ai_poFieldData->m_oColourRaster) ;
					} else {
						ExprAux.StrCat(ai_poFieldData->m_oBNRaster) ;
					} ;
				} else {
					ExprAux.StrCat(ai_poFieldData->m_oBNRaster) ;
				} ;
				ExprAux.StrCat("\"") ;
			} else {
				ExprAux.StrCpy(ai_poFieldData->m_oExpr) ;
			} ; 
		} else {
			ExprAux.StrCpy("\"Not supported raster map\"") ;
		} ;

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_RasterG ) ;
		M4_VIRTUAL_OBJECT( Field, ClENG_RasterG )->Init( 
			&m_oFieldEnv,
			&IdDesc,
			Top, Left, ai_poFieldData->m_Width, 
			ai_poFieldData->m_Height, ExprAux,
			ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
			ai_poFieldData->m_oHyperlink.InChar(0),
			ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
			ai_poFieldData->m_bSwOptimize,

			frameDef, ai_poFieldData->m_uiRegionColour,

			ai_poFieldData->m_stDefaultData.IsGraphicPath(),
			ai_poFieldData->m_bTempWebImage
		);
		break ;

	case ENG_FLD_BARCODE:

		switch (ai_poFieldData->m_BarCodeType) {
		
		case ENG_BAR_CODE_39:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_BarCode39 ) ;
			M4_VIRTUAL_OBJECT( Field, ClENG_BarCode39 )->Init( 
				&m_oFieldEnv,
				&IdDesc, Top, Left, ai_poFieldData->m_Width, 
				ai_poFieldData->m_Height, ai_poFieldData->m_oExpr.InChar(0),
				ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
				ai_poFieldData->m_oHyperlink.InChar(0),
				ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
				ai_poFieldData->m_bSwOptimize,

				frameDef, ai_poFieldData->m_uiRegionColour,

				ai_poFieldData->m_stDefaultData.GetAlignX(), 
				ai_poFieldData->m_stDefaultData.GetAlignY(), 
				ai_poFieldData->m_stDefaultData.GetIdColour(), 
				Font, ai_poFieldData->m_stDefaultData.IsClip(), 
			    ai_poFieldData->m_stDefaultData.GetNULLDef(), 
				ai_poFieldData->m_oIdPi	
			);
			break ;

		case ENG_BAR_CODE_25_INTERLEAVED:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_BarCodeI25 ) ;
			M4_VIRTUAL_OBJECT( Field, ClENG_BarCodeI25 )->Init( 
				&m_oFieldEnv,
				&IdDesc, Top, Left, ai_poFieldData->m_Width, 
				ai_poFieldData->m_Height, ai_poFieldData->m_oExpr.InChar(0),
				ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
				ai_poFieldData->m_oHyperlink.InChar(0),
				ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
				ai_poFieldData->m_bSwOptimize,

				frameDef, ai_poFieldData->m_uiRegionColour,

				ai_poFieldData->m_stDefaultData.GetAlignX(), 
				ai_poFieldData->m_stDefaultData.GetAlignY(), 
				ai_poFieldData->m_stDefaultData.GetIdColour(), 
				Font, ai_poFieldData->m_stDefaultData.IsClip(), 
			    ai_poFieldData->m_stDefaultData.GetNULLDef(), 
				ai_poFieldData->m_oIdPi	
			);
			break ;

		case ENG_BAR_CODE_128:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_BarCode128 ) ;
			M4_VIRTUAL_OBJECT( Field, ClENG_BarCode128 )->Init( 
				&m_oFieldEnv,
				&IdDesc, Top, Left, ai_poFieldData->m_Width, 
				ai_poFieldData->m_Height, ai_poFieldData->m_oExpr.InChar(0),
				ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
				ai_poFieldData->m_oHyperlink.InChar(0),
				ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
				ai_poFieldData->m_bSwOptimize,

				frameDef, ai_poFieldData->m_uiRegionColour,

				ai_poFieldData->m_stDefaultData.GetAlignX(), 
				ai_poFieldData->m_stDefaultData.GetAlignY(), 
				ai_poFieldData->m_stDefaultData.GetIdColour(), 
				Font, ai_poFieldData->m_stDefaultData.IsClip(), 
			    ai_poFieldData->m_stDefaultData.GetNULLDef(), 
				ai_poFieldData->m_oIdPi	
			);
			break ;

		case ENG_BAR_CODE_POSTNET:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_PostNet ) ;
			M4_VIRTUAL_OBJECT( Field, ClENG_PostNet )->Init( 
				&m_oFieldEnv,
				&IdDesc, Top, Left, ai_poFieldData->m_Width, 
				ai_poFieldData->m_Height, ai_poFieldData->m_oExpr.InChar(0),
				ai_poFieldData->m_oIndexExpr.InChar(0), ai_poFieldData->m_iIndexLevel,
				ai_poFieldData->m_oHyperlink.InChar(0),
				ai_poFieldData->m_bSwStatic, ai_poFieldData->m_bSwDuplicated, 
				ai_poFieldData->m_bSwOptimize,

				frameDef, ai_poFieldData->m_uiRegionColour,

				ai_poFieldData->m_stDefaultData.GetAlignX(), 
				ai_poFieldData->m_stDefaultData.GetAlignY(), 
				ai_poFieldData->m_stDefaultData.GetIdColour(), 
				Font, ai_poFieldData->m_stDefaultData.IsClip(), 
			    ai_poFieldData->m_stDefaultData.GetNULLDef(), 
				ai_poFieldData->m_oIdPi,
				
				-1
			);
			break ;

		} ;
		
		break ;

	case ENG_FLD_LINE:

		if ( ai_poFieldData->m_oRegionBorder.StrLenTrim()==ClENG_Line::GetSizeStrLine()*5 ) {
			SetLine.ToASCIZ( ai_poFieldData->m_oRegionBorder, ClENG_Line::GetSizeStrLine() ) ;
			SetFrame.ToASCIZ( 
				ai_poFieldData->m_oRegionBorder.InChar(ClENG_Line::GetSizeStrLine()), 
				ClENG_Line::GetSizeStrLine()*4 ) ;

		} else {
			SetLine.StrCpy("") ;
			SetFrame.StrCpy(ai_poFieldData->m_oRegionBorder) ;	//Cantara despues
		} ;

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field, ClENG_FieldLine ) ;
		M4_VIRTUAL_OBJECT( Field, ClENG_FieldLine )->Init( 
			&m_oFieldEnv, &IdDesc, ai_poFieldData->m_bSwStatic, 
			ai_poFieldData->m_bSwOptimize,

			frameDef, ai_poFieldData->m_uiRegionColour,

			Top, Left, ai_poFieldData->m_Width, ai_poFieldData->m_Height,  
			LineTop, LineLeft, LineTop2, LineLeft2, ai_poFieldData->m_bAdjustLineToContainer, 
			1, ClENG_OutDevice::SOLID, 0, M4_TRUE	//Datos linea se ponen luego
		) ;
		
		M4_VIRTUAL_OBJECT( Field, ClENG_FieldLine )->SetLine(SetLine) ;

		break;
	} ;

	if (ai_poFieldData->m_oSaveNode.StrLenTrim()>0) {
		NodeAux=m_oReportEnv.m_poChnData->GetNode( ai_poFieldData->m_oSaveNode, ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!NodeAux) {
			SwOk=M4_FALSE ;
			goto exit;
		} ;
		SwOk = M4_BOOL( 
			      SwOk && (
				  SaveItem.Init( NodeAux, ai_poFieldData->m_oSaveItemName, ClMIT_Trace::PROPAGATE_ERROR ) == 
				  ClMIT_ChnItem::ITEM_OK ) 
			   );
		if (!SwOk) goto exit ;
		Field->SetSave( &SaveItem ) ;
	} else {
		Field->SetSave( NULL ) ;
	} ;


exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::CreateField()", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	if (!SwOk) {
		M4_DELETE_VIRTUAL_ITEM( m_poInsp->m_poMemProf, ENG_MGI_FIELD, Field ) ;
	} ;

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return Field ;
}

ClENG_Section * ClENG_Report::LoadTreeSection( 
	ClENG_DesignTree *ai_poDT,
	StENG_DT_SectionData *ai_poSection, ClENG_Section *ai_poFather,
	m4bool_t ai_bSwSync, m4int16_t ai_iExeOrder ) 
{
	// Se desaloca si error y esta alocada
	ClENG_Section *Sec=NULL, *SonSection=NULL ;
	ClENG_Container *Cnt=NULL ;
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t PushState = m_poInsp->m_poTrace->GetPushState() ;
	m4int16_t B ;
	ClMIT_IdNumDesc IdDesc ;
	StENG_DT_SectionData::CntList_t::iterator ItC ;
	ClMIT_ChnNode *RecordNode=NULL, *GuideNode=NULL, *OneOneNode=NULL ;
	ClMIT_ChnItem GuideItem ;
	ClMIT_ChnItem *ItemGK=NULL ;
	ClENG_VR::StGroup::GroupKey_t GroupKey ;
	ClENG_VR::StGroup::GroupKey_t::iterator ItGK ;
	ClMIT_Args Arg ;
	ClENG_VR::OneOneList_t OneOne ;

	ClENG_CntDisplay CntDisplay ;
	ClENG_KeyTot * KeyTotAux=NULL ;
	StENG_DT_CTData *CTAux=NULL ;
	StENG_DT_CntData CntDataAux ;

	IdDesc.Init( ai_poSection->m_iIdSection, ai_poSection->m_oSectionName ) ;

	//Hay que cargar SOLO los containers propios de la seccion, segun su tipo, o se
	//  quedaran containers alocados que no se asocian a ninguna seccion y que nunca seran
	//  borrados.

	ClENG_CntGroup *CntGroup[ENG_CNT_COUNT] ;

	for (B=0;B<ENG_CNT_COUNT;++B) CntGroup[B] = NULL ;
	
	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< IdDesc << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadTreeSection Sec %0:s Sync %1:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< IdDesc << ai_bSwSync << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << POP_LOC << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< IdDesc << END_LOC ;

	for ( ItC = ai_poSection->m_oCnt.begin() ; ItC != ai_poSection->m_oCnt.end(); ++ItC ) {
		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
			CntGroup[(*ItC).first], ClENG_CntGroup ) ;
		CntGroup[(*ItC).first]->Init( &m_oReportEnv, 
			(m4char_t *)ClENG_DesignTree::GetCntTraceStr((*ItC).first)) ;

		if ( (*ItC).second.m_MergeMode==ENG_CT_NO_MERGE ) {

			//Forma de crear containers normales

			Cnt = CreateContainer( &(*ItC).second, 0, 0, 
				m_oReportEnv.m_poExeParams->m_bTestCnt, m_oReportEnv.m_poExeParams->m_bTestField ) ;
			if (!Cnt) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			CntGroup[(*ItC).first]->AddCnt(Cnt) ;
		
		} else {
		
			//Crear Containers de fusion
			
			KeyTotAux = GetKeyTotalize( (*ItC).second.m_oCTId, ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!KeyTotAux) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;

			CTAux=ai_poDT->FindCT( ENG_EXIST_REQUIRED, (*ItC).second.m_oCTId ) ;
			if (!CTAux) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;


			SwOk = M4_BOOL( SwOk && CntDisplay.Init( 	
				ai_poSection->m_iIdSection, (*ItC).first, &(*ItC).second, 
				&CTAux->m_oTemplate, KeyTotAux, ClENG_CntDisplay::TOG_LOAD_FROM_DESIGN ) 
			) ;
			if (!SwOk) goto exit ;

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				CntDisplay.Trace() ;
			};
			#endif

			for ( B=0; B< CntDisplay.GetPageCount() ;++B )  {

				SwOk = M4_BOOL( SwOk && CntDisplay.CreateCntData( B, ai_poDT, &CntDataAux, 
					m_oReportEnv.m_poExeParams->m_bTestCell ) ) ; 
				if (!SwOk) goto exit ;

				Cnt = CreateContainer( &CntDataAux, 0, 0, 
					m_oReportEnv.m_poExeParams->m_bTestCnt, m_oReportEnv.m_poExeParams->m_bTestField ) ;
				if (!Cnt) {
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

				CntGroup[(*ItC).first]->AddCnt(Cnt) ;
			} ;
		
		} ;
	} ;
	
	//Creacion de la seccion actual
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		
		ClMIT_Str GoTTxt, InhTxt, RangeTxt ;

		ClENG_VR::GetTraceStr( ai_poSection->m_GoT,&GoTTxt) ;
		ClENG_VR::GetTraceStr( ai_poSection->m_RangeMode,&RangeTxt) ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"New SECTION Section %0:s %1:s, ExeOrder %2:s FatherSec %3:s SwSync %4:s "
			"InheritMode  %5:s GoT %6:s RangeMode %7:s NextVRNode %8:s "
			"GuideItemItem %9:s Group %10:s OneOne %11:s CancelRecord ? %12:s "
			"BreakRecord ? %13:s Together ? %14:s"
		
			"CancelSection? %15:s "
			"PreVRContainer? %16:s VRContainer? %17:s PostVRContainer? %18:s "
			"BeforeContainer ? %19:s AfterContainer ? %20:s "
			"BeforePageBreak %21:s AfterPageBreak %22:s "
			"HeaderContainer ? %23:s FooterContainer ? %24:s "
			"HeaderMode %25:s FooterMode %26:s") 

			<< ai_poSection->m_iIdSection << ai_poSection->m_oSectionName.InChar(0)
			<< ai_iExeOrder
			<< ( ai_poFather ? ai_poFather->GetIdSec(): 0 )
			<< ai_bSwSync << InhTxt.InChar(0) << GoTTxt.InChar(0) << RangeTxt.InChar(0)
		
			<< ai_poSection->m_RecordsetNodeName.InChar(0)
			<< ai_poSection->m_GuideItemName.InChar(0)
			<< ai_poSection->m_oGroupKey.InChar(0)
			<< ai_poSection->m_oOneOneStr.InChar(0)

			<< 0L << ai_poSection->m_bSwBreakVR << ai_poSection->m_oTogether.InChar(0) 
			<< 0L
			<< M4_BOOL(CntGroup[ENG_CNT_PRERECORD]!=NULL) 
			<< M4_BOOL(CntGroup[ENG_CNT_RECORD]!=NULL) << M4_BOOL(CntGroup[ENG_CNT_POSTRECORD]!=NULL)
			<< M4_BOOL(CntGroup[ENG_CNT_BEFORE]!=NULL) << M4_BOOL(CntGroup[ENG_CNT_AFTER]!=NULL)
			<< (m4int16_t)ai_poSection->m_BeforePBreak << (m4int16_t)ai_poSection->m_AfterPBreak
			<< M4_BOOL(CntGroup[ENG_CNT_HEADER]!=NULL) << M4_BOOL(CntGroup[ENG_CNT_FOOTER]!=NULL)
			<< (m4int16_t)ai_poSection->m_HeaderMode << (m4int16_t)ai_poSection->m_FooterMode
		
			<< SEND_MSG   ;
	} ;
	#endif

	//Haya RecordNode

	RecordNode=m_oReportEnv.m_poChnData->GetNode( ai_poSection->m_RecordsetNodeName, 
			ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!RecordNode) {
		SwOk=M4_FALSE ;
		goto exit;
	} ;

	//Haya GuideItem si procede. Necesita de RecordNode

	if ( ai_poSection->m_GoT==ClENG_VR::SLICE_GOT ) {
		if (ai_poSection->m_GuideNode.StrLenTrim()>0 ) {
			GuideNode=m_oReportEnv.m_poChnData->GetNode( ai_poSection->m_GuideNode, 
					ClMIT_Trace::PROPAGATE_ERROR) ;
			if (!GuideNode) {
				SwOk=M4_FALSE ;
				goto exit;
			} ;
		} else GuideNode=RecordNode;

		SwOk = M4_BOOL (GuideItem.Init( GuideNode, ai_poSection->m_GuideItemName, 
				        ClMIT_Trace::PROPAGATE_ERROR ) == ClMIT_ChnItem::ITEM_OK ) ;
		if (!SwOk) goto exit ;
	} ;

	//Carga GroupKey 
	
	Arg.Init( ai_poSection->m_oGroupKey, ':', " ," ) ;
	GroupKey.clear() ;
	for ( B=0; B<Arg.GetArgCount(); ++B ) {

		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, ItemGK, ClMIT_ChnItem ) ;
		GroupKey.push_back(ItemGK) ;

		if ( ItemGK->Init(RecordNode,Arg.GetArg(B),ClMIT_Trace::PROPAGATE_ERROR) != 
			ClMIT_ChnItem::ITEM_OK ) 
		{
			SwOk=M4_FALSE ;
			if (!SwOk) goto exit ;	//HAY QUE BORRAR LOS ITEMS ALOCADOS
		} ;
	} ;

	// Carga OneOne

    Arg.Init( ai_poSection->m_oOneOneStr, ':', " ," ) ;
	OneOne.clear() ;

	for ( B=0; B<Arg.GetArgCount(); ++B ) {
		OneOneNode=m_oReportEnv.m_poChnData->GetNode( Arg.GetArg(B), ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!OneOneNode) {
			SwOk=M4_FALSE ;
			goto exit;
		} ;
		OneOne.insert(ClENG_VR::OneOneList_t::value_type(B,OneOneNode) ) ;
	} ;

	//Y crea el container

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, Sec, ClENG_Section ) ;
	SwOk=M4_VIRTUAL_OBJECT( Sec, ClENG_Section )->Init( 
		this,
		&m_oReportEnv,
		&IdDesc, ai_poFather, 
		ai_poSection->m_iSonSectionCount,  
		ai_poSection->m_NoPrintEmpty, 
			
		CntGroup[ENG_CNT_BEFORE],	ai_poSection->m_BeforePBreak,
		CntGroup[ENG_CNT_AFTER],	ai_poSection->m_AfterPBreak,
		CntGroup[ENG_CNT_HEADER],	ai_poSection->m_HeaderMode,
		CntGroup[ENG_CNT_FOOTER],	ai_poSection->m_FooterMode,

		CntGroup[ENG_CNT_PRERECORD], CntGroup[ENG_CNT_RECORD], CntGroup[ENG_CNT_POSTRECORD],

		ai_poSection->m_oPrepareRecordset, 
		ai_poSection->m_oCancelSection, 
		ai_poSection->m_oResetHeaderTotals, 
		ai_poSection->m_oCancelVR, 
		ai_poSection->m_oResetBeforeTotal, 
		ai_poSection->m_oSetPostVRTotal, 

		ai_poSection->m_GoT, 
		ai_poSection->m_RangeMode,
		RecordNode, &GuideItem,

		&GroupKey,

		ai_bSwSync, ai_poSection->m_bSwBreakVR, ai_poSection->m_bNullOnSons,
		ai_poSection->m_MaskTogether, 

		&OneOne,

		ClMIT_Trace::PROPAGATE_ERROR
	) ;

	if (!SwOk) goto exit ;
		
	if ( ai_poSection->m_iSonSectionCount >0 ) {

		//------->Carga recursiva de secciones hijas 

		for ( B=0; B < ai_poSection->m_iSonSectionCount; ++B ) {

			SonSection=	LoadTreeSection( 
				ai_poDT, &ai_poSection->m_oSonSection[B].m_oSection, Sec, 
				ai_poSection->m_oSonSection[B].m_bIsSync, 
				ai_poSection->m_oSonSection[B].m_iExeOrder ) ;
			if (!SonSection) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			M4_VIRTUAL_OBJECT( Sec, ClENG_Section )->
				InitLinks( B, SonSection, ai_poSection->m_oSonSection[B].m_iExeOrder ) ;
		} ;

	} ;

exit:

	if (!SwOk) {

		if (Sec) {
			// Si hay error, pero la seccion se creó,  la seccion borrara los datos heredados.
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, Sec ) ;
		} else {
			for (B=0;B<ENG_CNT_COUNT;++B) {
				M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, CntGroup[B]) ;
			} ;
			for (ItGK=GroupKey.begin(); ItGK!=GroupKey.end(); ++ItGK) {
				M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, *ItGK) ;
			} ;
			GroupKey.clear() ;		
		} ;
		Sec=NULL ;
	} ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::LoadTreeSection Sec %0:s Sync %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< IdDesc << ai_bSwSync << SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState) ;

	return Sec ;
}

ClENG_Section * ClENG_Report::FindSec(m4uint32_t  ai_iIdSec, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClENG_Section * Ret ;
	Ret = FindSec( m_poRoot, ai_iIdSec );	
	if ( !Ret && ai_ErrorEffect==ClMIT_Trace::IMMEDIATE_ERROR ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162756, 
				"Section not found", ClMIT_Msg::MIXED_OUT) 	<< SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return Ret ;
}

ClENG_Section * ClENG_Report::FindSec( ClENG_Section *ai_poSec, m4uint32_t ai_iIdSec) 
{
	m4int16_t B ;
	
	if (ai_poSec->GetIdSec()==ai_iIdSec) {

		return ai_poSec ;
	
	} else {

		ClENG_Section * Ret=NULL ;
	
		for (B=0; B < ai_poSec->GetNumSons() && !Ret; ++B ) {
			Ret=FindSec(ai_poSec->m_pstSon[B].m_poSon, ai_iIdSec ) ;
		} ;
	
		return Ret ;
	} ;
}


// Podria optimizarse para que no recorriese todo elarbol, sino la hebra actual. 
//	Sin embargo, puede hav¡ber problemas pues esto requiere que SonReentry siempre este
//	correcto.

void ClENG_Report::BLResetNotSync( ClENG_Section * ai_poSec ) 
{
	m4int16_t B, Top ;

	if ( !ai_poSec->IsSynchronized() ) {

		Top=m_oReportEnv.m_poDvc->GetNumLayoutPages() ;
		for ( B=0; B<Top; ++B ) ai_poSec->m_BLPrivate[B] =0 ;
	} ;

	if ( !ai_poSec->IsEndSection() ) {
		Top=ai_poSec->m_iNSons ;
		for ( B=0; B<Top; ++B ) {
			BLResetNotSync(ai_poSec->m_pstSon[B].m_poSon) ;
		} ;
	} ;
}

ClENG_Tot * ClENG_Report::GetTotalize(const m4char_t * ai_pcTotalize,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Index ;

	Index=m_oTotIndex.GetIndex(ai_pcTotalize,NULL,ClMIT_Trace::PROPAGATE_ERROR) ;
	SwOk = M4_BOOL( SwOk && Index!=ClENG_StrIndex::INDEX_EOF );

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	return SwOk ? &m_oTotalize[Index] : NULL ;
}

ClENG_KeyTot * ClENG_Report::GetKeyTotalize(const m4char_t * ai_pcCTId,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Index ;

	Index=m_oCTIndex.GetIndex(ai_pcCTId,NULL,ClMIT_Trace::PROPAGATE_ERROR) ;
	SwOk = M4_BOOL( SwOk && Index!=ClENG_StrIndex::INDEX_EOF );

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	return SwOk ? &m_oCT[Index] : NULL ;
}


m4bool_t ClENG_Report::PreAnalex(ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_Analex::StExpressionEnv StEnv ;
	m4int16_t B ;

	//*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5040 ] ) << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX, "ClENG_Report::PreAnalex()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif


	StEnv.m_poChn			= m_oReportEnv.m_poChnData ;
	StEnv.m_poReport		= this ;
	StEnv.m_poDvc			= m_oReportEnv.m_poDvc ;
	StEnv.m_poFieldManager	= m_oReportEnv.m_poFieldManager ;
	StEnv.m_poExeParams		= m_oReportEnv.m_poExeParams ;
	StEnv.m_poDBFormat		= m_oReportEnv.m_poDBFormat ; 
	StEnv.m_poTrLit			= m_oReportEnv.m_poTrLit ;

	//Preanaliza arbol de secciones

	SwOk = m_poRoot->PreAnalex(&StEnv) ;
	if (!SwOk) goto exit ;

	//Preanaliza containers de pagina

	StEnv.m_poDefaultNode=NULL ;
	StEnv.m_LoadMode = ClMIT_ChnItem::ITEM_LOAD ; 
	StEnv.m_poItemExternProp= NULL ;
	StEnv.m_poFieldExternProp= NULL ;
	StEnv.m_poSect=NULL ;
	for (B=0; B<m_iNContainers; ++B ) {
		SwOk = M4_BOOL( SwOk && m_oCntPage[B].PreAnalex(&StEnv) );
	} ;
exit:

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX, "ClENG_Report::PreAnalex() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;

	return SwOk ;		
}


m4bool_t ClENG_Report::PrepareExecution( 
	ClENG_DesignTree *ai_poDT, ClENG_ReportArgs	*ai_poExeParams,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk = M4_TRUE ;
	m4int16_t B, PAux, PageLayout ;
	m4bool_t SwReplicatePages ;
	m4char_t *CTIdAux ;


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_Report::PrepareExecution()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	ClENG_Report::End() ;

	m_oReportEnv.m_poFieldManager->Init() ;
	*m_oReportEnv.m_poTrLit = ai_poDT->m_oTrLit ;

	//Sincronización punteros m_oFieldEnv. Excepto m_poDvc, los demás son objetos siempre 
	//	alocados, aunque no se hayan incializado podemos apuntar a ellos.

	m_oFieldEnv.m_poFieldsManager		= m_oReportEnv.m_poFieldManager ; 
	m_oFieldEnv.m_poDBFormat			= m_oReportEnv.m_poDBFormat ;
	m_oFieldEnv.m_poDvc					= NULL ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Attaching to channels                                *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
	};
	#endif

	//------Hereda interes

	m_oFileInterest.Init( &ai_poDT->m_oFileInterest ) ;

	//------Carga ExeParams

	m_oReportEnv.m_poExeParams->Init(ai_poExeParams) ;

	//Recargo los ExeParams modificados con el parametro ExecutionPars ...
	SwOk = m_oReportEnv.m_poExeParams->ReLoad(ai_poDT->m_oGlobal.m_oExecutionPars, ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!SwOk) goto exit ;
	//...y continuo tal cual

	m_bSwPublish = m_oReportEnv.m_poExeParams->m_bSwPublish;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"Report info: " 
			"Report name %0:s ModeMetric %1:s PagLatout %2:s NumFonts %3:s NumFormats %4:s "
			"RootSection %5:s RootSync ? %6:s TotalizeList %7:s CTList %8:s BreakMode %9:s "
			"BreakSection %10:s ExecutionPars %11:s") 

			<< ai_poDT->m_oGlobal.m_oReportName.InChar(0) 
			<< (m4int16_t)ai_poDT->m_oGlobal.m_ModeMetric
			<< ai_poDT->m_iNumPageLayout << m_oReportEnv.m_poDBFont->GetNumFonts()
			<< (m4uint32_t)ai_poDT->m_oFormatList.size() 
			<< ai_poDT->m_oGlobal.m_iIdRootSection
			<< ai_poDT->m_oGlobal.m_bSwRootSync
			<< ai_poDT->m_oGlobal.m_oTotalizeList.InChar(0)
			<< ai_poDT->m_oGlobal.m_oCTList.InChar(0)
			<< (m4int16_t)ai_poDT->m_oGlobal.m_BreakMode 
			<< ai_poDT->m_oGlobal.m_iIdBreakSection
			<< ai_poDT->m_oGlobal.m_oExecutionPars.InChar(0)
			<< SEND_MSG   ;
	};	
	#endif

	//------Enganche al canal de datos

	SwOk = M4_BOOL( SwOk &&  InitChnData( 
		            m_oReportEnv.m_poExeParams,  m_oReportEnv.m_poChnData, ai_poDT, this ) );
	if (!SwOk) goto exit ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Loading Design in Memory                             *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
	};
	#endif

	//----Inicializacion DBFont
		
	SwOk=LoadDBFont(ai_poDT) ;
	if (!SwOk) goto exit ;

	//----Inicializacion DBFormat
	
	SwOk=LoadDBFormat(ai_poDT) ;
	if (!SwOk) goto exit ;

	//------Inicializacion Defaults Currency para el Report (mejor dicho, para el thread)
	//    Carga la moneda destino, fecha y tipo de cambio
	//	

	ClMIT_ChnValue::SetDefExchCurr( m_oReportEnv.m_poExeParams->m_oDestCurrency) ;
	ClMIT_ChnValue::SetDefExchDate( m_oReportEnv.m_poExeParams->m_dExchangeDate) ;
	ClMIT_ChnValue::SetDefExchType( m_oReportEnv.m_poExeParams->m_oExchangeType) ;


	//-----Carga lista CTs, arbol de datos CT, y paginas a cargar (posible aumento por CT)

	//CTIndex 
	
	SwOk = SwOk && m_oCTIndex.Init( ai_poDT->m_oGlobal.m_oCTList, ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!SwOk) goto exit ;

	if (m_oCTIndex.GetCount()>0) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
			m_oCT, ClENG_KeyTot, m_oCTIndex.GetCount() ) ;
		for (B=0; B<m_oCTIndex.GetCount(); ++B ) m_oCT[B].End() ;
	} ;
	
	//Carga datos del arbol
	//m_oChnData ya debe estar creado y con los datos cargados
	//Calcula tambien las paginas que ocupa el CT con el maximo numero de paginas 

	for (B=0, PageLayout=0; B<m_oCTIndex.GetCount(); ++B ) {
		CTIdAux = m_oCTIndex.GetKey(B, ClMIT_Trace::PROPAGATE_ERROR)  ;
		if ( ! CTIdAux ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		SwOk = M4_BOOL ( SwOk && LoadCT( ai_poDT, CTIdAux, &m_oCT[B], &PAux ) );

		PageLayout = M4_MAX( PageLayout, PAux ) ;
	} ;
	if (!SwOk) goto exit ;	

	//Calculo SwReplicatePages y PageLayout. Debe usarse PageLayout (NO el dato de DT) para
	//	crear el report.

	SwReplicatePages = PageLayout > 1 ;
	PageLayout = M4_MAX( PageLayout, ai_poDT->m_iNumPageLayout ) ;


	//------Actualizacion partametros break. El reseteo se hace despues porque 
	//	necesita de las secciones cargadas.

	m4uint32_t BreakSec ;

	if (m_oReportEnv.m_poExeParams->m_bLoadBreakDesign) {
		m_Break = ai_poDT->m_oGlobal.m_BreakMode ;
		BreakSec= ai_poDT->m_oGlobal.m_iIdBreakSection ;
	} else {
		m_Break = m_oReportEnv.m_poExeParams->m_Break ;
		BreakSec= m_oReportEnv.m_poExeParams->m_iBreakSec ;
	} ;

	m_oReportId.StrCpy(  m_oReportEnv.m_poExeParams->m_oReportId) ;
	m_oReportName.StrCpy(ai_poDT->m_oGlobal.m_oReportName) ;
	m_poBreakSec=NULL ;	//Se inicializa luego

	//------Inicializacion device (puede actuar sobre DBFont poniendo modo MonoFont)
	//		Carga paginas (no Cnt). 
	//		Necesita del canal de datos si analiza la expresion de Path.

	SwOk= M4_BOOL( SwOk &&LoadDvc(ai_poDT,PageLayout,SwReplicatePages) ) ;
	if (!SwOk) goto exit ;	


	//----Creacion de Report / Carga de containers de pagina 

	// Debe de haberse inicializado el Dvc, se usan datos de este.

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"Inicializating report with Id %0:s, Name %1:s, DataChn %2:s "
			"TotalizeList %3:s CTList %4:s" ) 

			<< m_oReportEnv.m_poExeParams->m_oReportId.InChar(0) 
			<< ai_poDT->m_oGlobal.m_oReportName.InChar(0)
			<< m_oReportEnv.m_poChnData->GetIdDesc() 
			<< ai_poDT->m_oGlobal.m_oTotalizeList.InChar(0) 
			<< ai_poDT->m_oGlobal.m_oCTList.InChar(0)
			
			<< SEND_MSG   ;
	};
	#endif

	//------Array m_oCntPage

	m_iNContainers=PageLayout ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
		m_oCntPage, ClENG_CntGroup, m_iNContainers
	) ;

	//------TotIndex

	SwOk = M4_BOOL( SwOk && m_oTotIndex.Init(ai_poDT->m_oGlobal.m_oTotalizeList, ClMIT_Trace::PROPAGATE_ERROR) );
	if (!SwOk) goto exit ;
	m_iCountTotalize= m_oTotIndex.GetCount();
	if (m_iCountTotalize>0) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, 
			m_oTotalize, ClENG_Tot, m_iCountTotalize ) ;
		for (B=0; B<m_iCountTotalize; ++B ) m_oTotalize[B].Init() ;
	} ;

	//Cnt de pagina

	SwOk=LoadPageContainers(ai_poDT,SwReplicatePages) ;
	if (!SwOk) goto exit ;

	//----Inicializacion arbol de secciones

	m_poRoot=LoadTreeSection( ai_poDT, &ai_poDT->m_oRootSection, NULL, 
		ai_poDT->m_oGlobal.m_bSwRootSync, 0 ) ;
	if ( !m_poRoot ) {		
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//-----Actualización Break, necesita de secciones cargadas

	if ( m_Break != NO_BREAK ) {
		m_poBreakSec=FindSec( BreakSec, ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!m_poBreakSec) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162758, 
				"Break section %0:s not found", ClMIT_Msg::MIXED_OUT) 	
				<< BreakSec  << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} else m_poBreakSec=NULL ;
		
	//-----Pre-analiza el report

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Preanalex on expressions                             *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && PreAnalex(ClMIT_Trace::PROPAGATE_ERROR) );
	if (!SwOk) goto exit ;


	if ( ! m_oReportEnv.m_poExeParams->m_bTestOnlyDesign ) {

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Management Preload Fields                            *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		};
		#endif

		if ( m_oReportEnv.m_poDvc->IsPreloadOnDevice() ) {
			for (B=0; B<m_iNContainers; ++B ) {
				m_oCntPage[B].PrintPreload(M4_TRUE) ;
			} ;
		} ;
		m_oReportEnv.m_poDvc->FixOptimizedPreload() ;

		if ( m_oReportEnv.m_poDvc->IsPreloadOnDevice() ) {
			for (B=0; B<m_iNContainers; ++B ) {
				m_oCntPage[B].PrintPreload(M4_FALSE) ;
			} ;
		} ;
		m_oReportEnv.m_poDvc->FixPreload() ;	

		m_poRoot->m_Reentry=ClENG_Section::NONE_REENTRY;

		if ( m_Break != NO_BREAK ) m_poBreakSec->PrepareBreakSection();

	} ;


exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Ready to print                                       *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
	};
	#endif

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::PrepareExecution(), SwOk %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk;
}


m4bool_t ClENG_Report::ExecuteBreak( 
	m4bool_t ai_bExecuteAllBreaks, m4uint32_t *ao_uiBreakCount,
	ClENG_PhysicalFile **ao_poFile) 
{
	m4bool_t SwReportEnd, SwBreakEnd, SwOk=M4_TRUE ;
	ClENG_Section::RETURN_SECTION SectionEvent ;
	m4int16_t	B ;
	m4int32_t	iPageNum = 0;
	
	if ( ! m_oReportEnv.m_poExeParams->m_bTestOnlyDesign ) {

		// Establece un arbol de llamadas de proceso que se destruye segun se levanta un EOP
		//	y se reconstruye cuando vuelve al punto que origino el EOP.
		// Cuando una seccion se acaba de imprimir o se cancela, es una vuelta de proceso sin
		//	reconstruccion.

		for (SwReportEnd=M4_FALSE; !SwReportEnd ;) {

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing Break %0:s", ClMIT_Msg::ENTRY_FORMAT) 
					<< m_oReportEnv.m_poDvc->GetBreakCount() << SEND_MSG   ;
			};
			#endif

			// Se verifica la posicion en la secciones para la evaluación
			// de las expresiones del break
			// (sólo si la expresión de fichero se debe evaluar por cada ruptura)
			if ( m_bEvaluateExpressionByBreak && !m_poRoot->CheckRecordSetPosition() )
			{
				// No hay registros
				SwReportEnd = M4_TRUE;
				goto exit ;
			}

			SwOk = M4_BOOL( SwOk && m_oReportEnv.m_poDvc->PreReset() );
			if (!SwOk) goto exit ;

			iPageNum = 0;
			for (SwBreakEnd=M4_FALSE;!SwBreakEnd; ) {
		
				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing pages from page %0:s to page %1:s", ClMIT_Msg::ENTRY_FORMAT) 
						<< m_oReportEnv.m_poDvc->GetPagesDvcPrinted()+1
						<< m_oReportEnv.m_poDvc->GetPagesDvcPrinted()+m_oReportEnv.m_poDvc->GetNumLayoutPages()
						<< SEND_MSG   ;
				};
				#endif

				BLResetNotSync( m_poRoot ) ;
	
				SwOk = M4_BOOL( SwOk && m_oReportEnv.m_poDvc->RefreshLayout() );
				if (!SwOk)
				{
					break ;
				}
				else
				{
					if (m_oReportEnv.m_poExeParams->m_bDuplexEmulation == M4_TRUE)
					{
						iPageNum += m_oReportEnv.m_poDvc->GetNumLayoutPages();
					}
				}

				if (ao_poFile) *ao_poFile = m_oReportEnv.m_poDvc->HasFlushFile(0) ;

				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing page containers", ClMIT_Msg::ENTRY_FORMAT) 
						<< DETAILED_MSG << SEND_MSG   ;
				};
				#endif

				for (B=0; B<m_iNContainers; ++B ) {
					m_oCntPage[B].Print() ;
				} ;
				
				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing page containers", ClMIT_Msg::EXIT_FORMAT) 
						<< DETAILED_MSG << SEND_MSG   ;
				};
				#endif

				SectionEvent=m_poRoot->PrintSection() ;

				if (m_oReportEnv.m_poExeParams->m_bDuplexEmulation == M4_TRUE)
				{
					if ((iPageNum % 2 != 0) && (SectionEvent == ClENG_Section::NOT_END_SEC_EOP_DUPLEX || SectionEvent == ClENG_Section::END_SEC_NOT_EOP_DUPLEX))
					{
						m_oReportEnv.m_poDvc->RefreshLayout();
						iPageNum += m_oReportEnv.m_poDvc->GetNumLayoutPages();
					}
				}

				if (SectionEvent == ClENG_Section::END_SEC_EOP || SectionEvent ==  ClENG_Section::END_SEC_NOT_EOP || SectionEvent ==  ClENG_Section::END_SEC_NOT_EOP_DUPLEX)
				{
					SwReportEnd = M4_TRUE ;
					SwBreakEnd = M4_TRUE ;
				} ;

				if ( m_Break != NO_BREAK ) {
					SwBreakEnd = M4_BOOL( SwBreakEnd || m_poBreakSec->IsBreakFlag() );
					m_poBreakSec->ClearBreakFlag() ;
				} ;

				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing pages", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
				};
				#endif
		
				SwOk = M4_BOOL( SwOk && m_oReportEnv.m_poDvc->FlushLayout() );
				if (!SwOk) break ;

			} ;	//Fin de break

			if ( ao_uiBreakCount ) *ao_uiBreakCount=m_oReportEnv.m_poDvc->GetBreakCount() ;

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_CORE,"Printing Break", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
			};
			#endif

			if (!SwOk) goto exit ;

			m_oReportEnv.m_poDvc->PostReset() ;

			if ( ! ai_bExecuteAllBreaks ) break ;

		} ;


	} else {

		SwReportEnd=M4_TRUE ;
	} ;

exit:

	if (SwOk && SwReportEnd && ! m_oReportEnv.m_poExeParams->m_bTestOnlyDesign ) {
		m_oReportEnv.m_poDvc->NotifyLastReset() ;
	} ;

	return M4_BOOL( ! SwReportEnd && SwOk );
}


m4bool_t ClENG_Report::TerminateExecution( ClMIT_Str *ao_NavigationPathMask ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_TaskEnv * TaskEnv = GET_TASK_ENV() ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"*                                Exit Phase                                           *", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"***************************************************************************************", ClMIT_Msg::NO_WRAP) << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"") << SEND_MSG   ;
	};
	#endif


	if (m_bSwPublish) {
		if ( m_oReportEnv.m_poDvc->GetPublishedBreakCount()==0 ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_WARNING, GROUP_ID, 2163034,
				"Se especificó publicación y no se ha publicado nada. Puede ser que el report no tenga datos.", 
				ClMIT_Msg::MIXED_OUT) 
				<< SEND_MSG ;
		} ;
	} ;


	EndChnData( m_oReportEnv.m_poExeParams, m_oReportEnv.m_poChnData ) ;

	TaskEnv->m_APIChn.Chn_m_poChnCurrency=NULL ;

	m_oReportEnv.m_poDBFont->End() ;
	m_oReportEnv.m_poDBFormat->End() ;
	
	if (m_oReportEnv.m_poDvc ) {
		if(ao_NavigationPathMask) m_oReportEnv.m_poDvc->GetNavigationPathMask(ao_NavigationPathMask) ; 
		m_oReportEnv.m_poDvc->End() ;	
	} ;

	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oReportEnv.m_poDvc ) ;
	m_oFieldEnv.m_poDvc=NULL ;

	ClENG_Report::End() ;	//No se hace antes para secuenciar la destruccion de ReportEnv
	
	return SwOk ;
}

m4bool_t ClENG_Report::Execute(ClENG_DesignTree *ai_poDT, ClENG_ReportArgs *ai_poExeParams,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_Report::Execute()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && PrepareExecution( ai_poDT, ai_poExeParams, ClMIT_Trace::PROPAGATE_ERROR) );
	if (SwOk)  ExecuteBreak(M4_TRUE) ;
	SwOk = M4_BOOL( SwOk && TerminateExecution() );

exit:
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::Execute(), SwOk %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk;
}

//========= TRATAMIENTO DEL ORDER BY =============================

m4bool_t ClENG_Report::UpdateOrderByItem(ClENG_DesignTree *ai_poDT) 
{
	m4bool_t SwOk=M4_TRUE;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg(GROUP_ID, "ClENG_Report::UpdateOrderByItem()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	//Comprobamos que hay RootSection
	if(ai_poDT->m_oRootSection.m_iIdSection!=ENG_SECION_ID_EMPTY) {
		if ( ! TreeSectionForOrderBy(&ai_poDT->m_oRootSection, 0) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} else {
		SwOk=M4_FALSE;
	};
	
exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::UpdateOrderByItem()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif
 
	return SwOk ;
}


m4bool_t  ClENG_Report::TreeSectionForOrderBy(StENG_DT_SectionData *ai_poSection, m4uint16_t ai_uiOrderCount, m4char_t* ai_pcNodeName)
{
	m4uint16_t iCounter, B, uiOrderByCount = ai_uiOrderCount;
	m4int16_t PushState=m_poInsp->m_poTrace->GetPushState();
	ClMIT_Str oStrSQLSentence, StrAux, StrNodeName;
	ClMIT_ChnNode *poSectionNode;
	ClMIT_Args Arg, SQLArg;
	m4bool_t SwOk=M4_TRUE ;
	
	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ LOADER_MSG_1070 ] ) 
		<< (m4uint32_t)ai_poSection->m_iIdSection << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Report::TreeSectionForOrderBy Sec %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_poSection->m_iIdSection << CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	StrNodeName.StrCpy(ai_poSection->m_RecordsetNodeName);
	
	//Obtiene el Nodo del que tira la seccion
	if(!ai_pcNodeName || ai_poSection->m_RangeMode!=ClENG_VR::SUBGROUP_VR) {
		poSectionNode = m_oReportEnv.m_poChnData->GetNode(StrNodeName.InChar(0), ClMIT_Trace::PROPAGATE_ERROR);
	} else {
		poSectionNode = m_oReportEnv.m_poChnData->GetNode(ai_pcNodeName);
	};
	if (!poSectionNode) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Si Existe el Item OrderBy actualizamos la sentencia, sino no hacemos nada
	if(!m4uint32_t(poSectionNode->GetOrderByHandle())){
		goto exit ;
	} ;
		
	if(ai_poSection->m_oGroupKey.StrLen()>0 && ai_poSection->m_RangeMode!=ClENG_VR::SUBGROUP_VR){
		//1.- ES GRUPO PERO NO SUBGRUPO (Raiz del arbol de agrupaciones
		//Leo el contenido del Item OrderBy
		poSectionNode->GetOrderByItem(&oStrSQLSentence);
		uiOrderByCount=0;
		//Si esta vacio puedo hacer las cosas con normalidad
		if(oStrSQLSentence.StrLen()==0){
			Arg.Init(ai_poSection->m_oGroupKey, ':', " ,");
			for (iCounter=0; iCounter<Arg.GetArgCount(); ++iCounter, uiOrderByCount++) {
				StrAux.StrCpy(Arg.GetArg(iCounter));
				if(uiOrderByCount==0){
					oStrSQLSentence.StrCpy(ORDER_BY);
				} else {
					oStrSQLSentence.StrCat(SEPARATOR);
				};
				oStrSQLSentence.StrCat(ITEM);
				oStrSQLSentence.StrCat(StrAux.InChar(0));
				oStrSQLSentence.StrCat(ASCENDENT);
				poSectionNode->SetOrderByItem(oStrSQLSentence.InChar(0));
			} ;
		} else {
			//Si no esta vacio debo ir comprobando los contenidos
			//Leo el contenido del Item OrderBy
			poSectionNode->GetOrderByItem(&oStrSQLSentence);
			SQLArg.Init(oStrSQLSentence.InChar(0), ':' ," @,");

			Arg.Init(ai_poSection->m_oGroupKey, ':', " ,");
			for (iCounter=0; iCounter<Arg.GetArgCount(); ++iCounter, uiOrderByCount++) {
				StrAux.StrCpy(Arg.GetArg(iCounter));
				//Si el numero de argumentos de la cadena SQL es menor, o coincide en la posicion el argumento
				if( ((SQLArg.GetArgCount()/2)-1)< uiOrderByCount+1) {
					oStrSQLSentence.StrCat(SEPARATOR);
					oStrSQLSentence.StrCat(ITEM);
					oStrSQLSentence.StrCat(StrAux.InChar(0));
					oStrSQLSentence.StrCat(ASCENDENT);
					poSectionNode->SetOrderByItem(oStrSQLSentence.InChar(0));
				} else if ((SQLArg.GetArgCount()/2)-1>= uiOrderByCount && strcmp(SQLArg.GetArg((2*(uiOrderByCount-1))+2),StrAux.InChar(0))){
					//ERROR LOCAL
					SwOk=M4_FALSE ;
					*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162983, 
						"Error logico en las agrupaciones.", ClMIT_Msg::MIXED_OUT)
					<< SEND_MSG ;
					goto exit ;
				};
			} ;
		};
	} else if(ai_poSection->m_RangeMode==ClENG_VR::SUBGROUP_VR){
		//2.-ES SUBGRUPO, implica que hay un padre GRUPO en el arbol de recursividad
		// El padre inmediato puede ser subgrupo o GRUPO raiz

		//Leo el contenido del Item OrderBy
		poSectionNode->GetOrderByItem(&oStrSQLSentence);
		SQLArg.Init(oStrSQLSentence.InChar(0), ':' ," @,");

		Arg.Init(ai_poSection->m_oGroupKey, ':', " ,");
		for (iCounter=0; iCounter<Arg.GetArgCount(); ++iCounter, uiOrderByCount++) {
			StrAux.StrCpy(Arg.GetArg(iCounter));
			//Si el numero de argumentos de la cadena SQL es menor, o coincide en la posicion el argumento
			if( ((SQLArg.GetArgCount()/2)-1)< uiOrderByCount+1) {
				oStrSQLSentence.StrCat(SEPARATOR);
				oStrSQLSentence.StrCat(ITEM);
				oStrSQLSentence.StrCat(StrAux.InChar(0));
				oStrSQLSentence.StrCat(ASCENDENT);
				poSectionNode->SetOrderByItem(oStrSQLSentence.InChar(0));
			} else if ((SQLArg.GetArgCount()/2)-1>= uiOrderByCount && strcmp(SQLArg.GetArg((2*(uiOrderByCount-1))+2),StrAux.InChar(0))){
				//ERROR LOCAL
				SwOk=M4_FALSE ;
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162983,
					"Error logico en las agrupaciones.", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
				goto exit ;
			};
		} ;

	} else {
		//3.-No es ni grupo ni subgrupo, no hacemos nada y actualizamos a cero el contador
		uiOrderByCount = 0;
	};
	
	//RECORRIDO RECURSIVO DE LAS SECCIONES HIJAS
	for ( B=0; B < ai_poSection->m_iSonSectionCount; ++B ) {
		if ( ! TreeSectionForOrderBy(&ai_poSection->m_oSonSection[B].m_oSection, uiOrderByCount, StrNodeName.InChar(0))  )
		{
			SwOk=M4_FALSE ;
			goto exit ;
		};
	} ;


exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Loader::TreeSectionForOrderBy Sec %0:s SwOk %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< ai_poSection->m_iIdSection << SwOk << CRAZY_MSG << SEND_MSG  ;
	};
	#endif

	m_poInsp->m_poTrace->RestorePushState(PushState);

	return SwOk ;
}

//========= FIN DE TRATAMIENTO DEL ORDER BY =====================


m4bool_t ClENG_Report::InitChnData( 
	ClENG_ReportArgs *ai_poExeParams,  ClMIT_Chn *ai_poChnData, ClENG_DesignTree *ai_poDT, 
	ClENG_Report *ai_poReport ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_TaskEnv * TaskEnv = GET_TASK_ENV() ;

	//------Enganche al canal de datos
	// Si el modo es LINK_DATA, le pasamos el ID del Data Language

	if ( ai_poExeParams->m_LinkData == ClENG_ReportRequest::BUILD_DATA ) {
		SwOk = ai_poChnData->InitOwn( 
			! ai_poExeParams->m_bRePrinter ? 
			ai_poDT->m_oGlobal.m_oDataChn : ai_poDT->m_oGlobal.m_oDataChnReprinter, 
			ai_poExeParams->m_iDataLang,
			ClMIT_Trace::PROPAGATE_ERROR ) ;
		if (!SwOk) goto exit ;
	} else {
		SwOk = ai_poChnData->InitCommonAccess( 
			ClMIT_MVC::GetAccessFromRunContext(&ai_poExeParams->m_oDataContext), 
			ClMIT_Trace::PROPAGATE_ERROR ) ;
		if (!SwOk) goto exit ;
	} ; 

	ai_poChnData->SetAutoLoad( ai_poExeParams->m_DataAutoload ) ;
	if ( !(ai_poExeParams->m_LinkData == ClENG_ReportRequest::LINK_DATA || 
		ai_poExeParams->m_LinkData == ClENG_ReportRequest::LINK_UNLOAD_DATA ) &&
		ai_poReport )
	{
		//Lanzamos el proceso del ORDER_BY
		SwOk = ai_poReport->UpdateOrderByItem(ai_poDT);	
		if (!SwOk) goto exit ;

		ai_poChnData->Load() ;
	} ;

	TaskEnv->m_APIChn.Chn_m_poChnCurrency=ai_poChnData ;	
	
	// Pack de datos

	if ( ai_poExeParams->m_bPackData ) {
		#ifdef RPT_ENGINE_TRACE
		if(!GET_INSP()->m_poTrace->IsTraceCodeRipped()){
			*GET_INSP()->m_poTrace << ClMIT_Msg( GROUP_ID,"Performing ChnData.CheckPoint()") << SEND_MSG   ;
		};
		#endif
		ai_poChnData->CheckPoint() ;
	} ;
exit:
	return SwOk ;
	
}

void ClENG_Report::EndChnData( ClENG_ReportArgs *ai_poExeParams, ClMIT_Chn *ai_poChnData ) 
{
	if ( ai_poExeParams->m_LinkData == ClENG_ReportRequest::LINK_UNLOAD_DATA || 
		 ai_poExeParams->m_LinkData == ClENG_ReportRequest::LINK_LOAD_UNLOAD_DATA ) 
	{
		if ( ai_poChnData->IsLinked() ) ai_poChnData->Unload() ;
	} ;
	ai_poChnData->End() ;
}

