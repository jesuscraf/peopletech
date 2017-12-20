//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpdfuni.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             12/08/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    This module manages the PDF Device (UNICODE)
//
//
//==============================================================================

#include "eng.h"

#include "cldvcpdfuni.h"
#include "m4java.h"
#include "itextproxy.h"

#include "engdf.inl"
#include "cldvcpdf.hmg"
#include "xapichn.h"
#include "sesschn.h"

#include "m4unicode.hpp"

#include "sysinfo.h"

#include <math.h>

extern int	M4IsUnicode( void ) ;

const m4int32_t MAX_PATH_SIZE = 1024;

ClENG_DvcPDFUnicode::ClENG_DvcPDFUnicode() 
{
	m_oiTextProxy = NULL;
	m_iPDFPage = NULL;
	m_poPagePrinter = NULL;
	m_poPDFFontNames = NULL;
	m_poPDFFontStyles = NULL;
}

m4bool_t ClENG_DvcPDFUnicode::Init(	
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Campos especificos

		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4bool_t ai_bTableOfContents, 
		m4bool_t ai_bForceUnicodeOutput, // Fuerza la utilización de fuentes Unicode (para textos)


		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	ClMIT_Flag::Value_t Flags ;
	m4bool_t SwOk = M4_TRUE;
	m4int16_t Page;

	ClENG_DvcPDFUnicode::End() ;

	ClFONT_M_Font* ai_poFont;
	ClMIT_Str FontPathEnv;
	ClMIT_Str FontPath;
	ClMIT_Str FontPathRFN;
	ClMIT_Str FontName;
	ClMIT_Str FontNameTmp;
	ClMIT_Str FontNameRaw;
	ClMIT_Str FontNameUnicode;
	ClMIT_Str TTFFontName;
	m4int16_t numFonts = ai_poDBFont->GetNumFonts();
	m4int16_t indexFont;
	ClMIT_File oMitFile;
	m4bool_t bIsFileVerified;
	int FontStyle;

	m4pchar_t pLocalPath;
	m4pchar_t pEnvValue;
	m4int32_t iEnvValueSizeIn;
	m4int32_t iEnvValueSize;
	//Activamos todos los Flags, puesto que siempre soporta color
	Flags =
		//NO activamos PreLoad, solo nos dara problemas y no merece la pena porque 
		//	optimizamos los graficos via objects unicos.
		//ClENG_OutDevice::PRELOAD_ON_DEVICE | 
		ClENG_OutDevice::COLOUR_SUPPORTED | 
		ClENG_OutDevice::COLOUR_ACTIVATED | 
		ClENG_OutDevice::RASTER_SUPPORTED ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	m_bIsRTL_Language = TaskEnv->m_APIChn.m_poMVC->GetSessChn()->IsRTL_Language();

	SwOk = ClENG_DvcFile::Init(
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		Flags, ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak,	
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_BreakList::SHARING_DUPLEX, 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR
	);

	if(!SwOk) goto exit;

	m_poDBFont = ai_poDBFont ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		         ClENG_PDF_Page, GetNumLayoutPages());

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oiTextProxy,
		         CliTextProxy*, GetNumLayoutPages());

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_iPDFPage,
		         int, GetNumLayoutPages());

	for(Page=0; Page<GetNumLayoutPages(); ++Page)
	{
		m_oiTextProxy[Page] = NULL;
		m_iPDFPage[Page] = -1;
	}

	// Obtiene los paths donde se buscaran las fuentes
#ifdef _WINDOWS
	// En Windows se buscaran primero según la variable de entorno M4_FONT_PATH
	// (incluida en la gestión de fuentes), posteriormente en %WINDIR%\FONTS
	// y finalmente en el directorio de RFN (realizando la descarga si procede)
	// En el caso de M4_FONT_PATH se establece en el mismo proceso por lo tanto
	// obliga a utilizar GetEnvironmentVariable ya que getenv no devuelve el valor.
	iEnvValueSizeIn = MAX_PATH_SIZE;
	pEnvValue = new m4char_t[iEnvValueSizeIn + 1];
	iEnvValueSize = GetEnvironmentVariable("M4_FONT_PATH", pEnvValue, iEnvValueSizeIn);
	if (iEnvValueSize > iEnvValueSizeIn)
	{
		// No hay espacio suficiente. Reajustamos el tamaño
		delete [] pEnvValue;
		iEnvValueSizeIn = iEnvValueSize;
		pEnvValue = new m4char_t[iEnvValueSizeIn + 1];
		iEnvValueSize = GetEnvironmentVariable("M4_FONT_PATH", pEnvValue, iEnvValueSizeIn);
	}
	if (iEnvValueSize > 0)
	{
		// Se ha encontrado la variable M4_FONT_PATH
		FontPathEnv.StrCpy(pEnvValue);
	}
	iEnvValueSize = GetEnvironmentVariable("WINDIR", pEnvValue, iEnvValueSizeIn);
	if (iEnvValueSize > iEnvValueSizeIn)
	{
		// No hay espacio suficiente. Reajustamos el tamaño
		delete [] pEnvValue;
		iEnvValueSizeIn = iEnvValueSize;
		pEnvValue = new m4char_t[iEnvValueSizeIn + 1];
		iEnvValueSize = GetEnvironmentVariable("WINDIR", pEnvValue, iEnvValueSizeIn);
	}
	if (iEnvValueSize > 0)
	{
		// Se ha encontrado la variable M4_FONT_PATH
		FontPath.StrCpy(getenv("WINDIR"));
		ClMIT_SysInfo::AppendToPath( &FontPath, "Fonts" ) ;
	}
	else
	{
		FontPath.StrCpy(".");
	}
	delete [] pEnvValue;
	// Se utiliza como último directorio de búsqueda de fuentes el directorio de RFNs
	// (y se descarga si es necesario en RW)
	if (M4_FALSE == m_poInsp->m_poSysInfo->GetValue(ENG_RK_RFN_PATH, &FontPathRFN))
	{
		FontPathRFN.StrCpy("");
	}
#else
	// En UNIX se buscaran donde están definidos los RFN
	// (el propio objeto StSoftFontData tiene la ruta completa incluyendo el fichero RFN,
	//  es preferible consultar la configuración de directorio de RFNs que manipular el que
	//  proporciona. Todos los RFNs están en el mismo directorio)
	if (M4_FALSE == m_poInsp->m_poSysInfo->GetValue(ENG_RK_RFN_PATH, &FontPath))
	{
		FontPath.StrCpy(".");
	}
#endif
	// Se obtiene el nombre de fuente unicode
	m_poInsp->m_poSysInfo->GetValue(ENG_RK_UNICODE_FONT_FILENAME, &FontNameUnicode) ;
	if ((M4_FALSE == m_poInsp->m_poSysInfo->GetValue(ENG_RK_UNICODE_FONT_FILENAME, &FontNameUnicode)) ||
		(FontNameUnicode.StrLen() == 0))
	{
		// Fuente por defecto
		//FontNameUnicode.StrCpy("arialuni.ttf");
		FontNameUnicode.StrCpy("code2000.ttf");
	}

	// Se analiza cada fuente del informe para obtener su nombre/ruta
	m_poPDFFontNames = new ClMIT_Str [numFonts];
	m_poPDFFontStyles = new int [numFonts];
	for (indexFont = 0; indexFont < numFonts; indexFont ++)
	{
		FontStyle = CliTextProxy::FONT_NORMAL;

		// Bug 199020. Se habilita el nuevo motor de PDF para entornos ANSI y en estos casos no
		// se debe realizar ningún cambio de fuente. Por lo tanto se añade la condición de estar
		// en modo unicode en las dos comprobaciones que se realizan para el cambio de fuente:
		// - Forzar el cambio para todas la fuentes excepto el código de barras
		// - Cambio de la fuente adobe Helvética (Arial) a la fuente Unicode
		ai_poFont = ai_poDBFont->GetAddressFontByIndex(indexFont);

		if (ai_poFont->IsUnderline())
			FontStyle |= CliTextProxy::FONT_UNDERLINE;
		if (ai_poFont->IsBold())
			FontStyle |= CliTextProxy::FONT_BOLD;
		if (ai_poFont->IsItalic())
			FontStyle |= CliTextProxy::FONT_ITALIC;

		FontNameRaw = ai_poFont->PSC_GetTypeFace();
		if (( M4_TRUE == ai_bForceUnicodeOutput ) &&
			( M4IsUnicode() == 1 ) &&
			( ClMIT_Str::StrCmpTrim(FontNameRaw, "AdvC39c") != 0 ) && // Fuente de código de barras
			( ClMIT_Str::StrCmpTrim(FontNameRaw, "Advi25c") != 0 ))	// Fuente de código de barras
		{
			// Si se fuerza la salida a fuentes unicode,
			// se tienen que cambiar todas la fuentes a Arial Unicode a excepción
			// de las de código de barras
			TTFFontName.StrCpy( FontNameUnicode ) ;
		}
		else
		{
			if (ai_poFont->IsSoftFont() && ai_poFont->GetTranslationType()!=ENG_SOFT_TRANS_TYPE_PSC_MIXED)
			{
				ClFONT_M_DBFont::StSoftFontData *sfData = m_poDBFont->GetSoftFontDataByIndex(ai_poFont->GeSoftFontIndex());
				// Se copia eliminado la extensión .pfa (Adobe Type 1)
				// En la generación de los rfn se transforma los .ttf a .pfa
				TTFFontName.ToASCIZ(sfData->m_oSofFontFile, sfData->m_oSofFontFile.StrLen() - 4);
				TTFFontName.StrCat(".ttf");
			}
			else
			{
				FontNameTmp.GetString(ClMIT_Str::TO_LIMIT, FontNameRaw, "-");
				if ((M4IsUnicode() == 1) && (0 == ClMIT_Str::StrICmpTrim(FontNameTmp, "Helvetica")))
				{
					// Se trata de Arial (TTF) Helvetica (Adobe Type 1)
					TTFFontName.StrCpy( FontNameUnicode ) ;
				}
				else
				{
					FontName.StrCpy(FontNameRaw);
					// Es una fuente interna. No se aplican los modificadores de estilo
					// Italic y Bold de la iText
					FontStyle &= ~ (CliTextProxy::FONT_ITALIC | CliTextProxy::FONT_BOLD);
				}
			}
		}

		if ( TTFFontName.StrLen() > 0 )
		{
			// Se busca el path siguiendo las prioridades y verificando
			// la existencia del archivo cuando sea necesario
			if ( FontPathEnv.StrLen() > 0 )
			{
				FontName.StrCpy( FontPathEnv );
				ClMIT_SysInfo::AppendToPath( &FontName, TTFFontName ) ;
				bIsFileVerified = ClMIT_File::Exist( FontName );
			}
			else
			{
				bIsFileVerified = M4_FALSE;
			}

			if ( M4_FALSE == bIsFileVerified )
			{
				FontName.StrCpy ( FontPath );
				ClMIT_SysInfo::AppendToPath( &FontName, TTFFontName ) ;

				if ( FontPathRFN.StrLen() > 0 )
				{
					bIsFileVerified = ClMIT_File::Exist( FontName );
					if ( M4_FALSE == bIsFileVerified )
					{
						FontName.StrCpy ( FontPathRFN );
						ClMIT_SysInfo::AppendToPath( &FontName, TTFFontName ) ;

						// Hay que intentar descargarlo si procede
						pLocalPath = new m4char_t[MAX_PATH_SIZE + 1];
						*pLocalPath = '\0';
						// TryToDownloadFromURL debería ser estático.
						// Se utiliza una instancia para acceder y no cambiar la mienv
						if (M4_TRUE == oMitFile.TryToDownloadFromURL(FontName.InChar(0), MAX_PATH_SIZE, pLocalPath))
						{
							FontName.StrCpy(pLocalPath);
							// TryToDownloadFromURL puede dejar el separador del fichero con la barra incorrecta
							ClMIT_File::ConvertToThisPlatform(&FontName);
						}
						delete [] pLocalPath;
					}
				}
			}
			TTFFontName.StrCpy("");
		}

		m_poPDFFontNames[indexFont].StrCpy(FontName);
		m_poPDFFontStyles[indexFont] = FontStyle;
		FontName.StrCpy("");
	}

exit:
	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}

void ClENG_DvcPDFUnicode::End()
{
	m4int16_t Page;

	if (m_poPDFFontNames)
	{
		delete [] m_poPDFFontNames;
		m_poPDFFontNames = NULL;
	}

	if (m_poPDFFontStyles)
	{
		delete [] m_poPDFFontStyles;
		m_poPDFFontStyles = NULL;
	}

	if (m_oiTextProxy)
	{
		Clear_iTextProxy();
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_oiTextProxy,
		            *CliTextProxy, GetNumLayoutPages());

		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_iPDFPage,
		            int, GetNumLayoutPages());

	}

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		            ClENG_PDF_Page, GetNumLayoutPages());

	ClENG_DvcFile::End();
}

void ClENG_DvcPDFUnicode::Clear_iTextProxy()
{
	TextProxyList_t::iterator it ;
	m4int16_t Page;

	for (it=m_oRealTextProxy.begin(); it!=m_oRealTextProxy.end(); ++it)
	{
		(*it).second->Close();
		delete (*it).second;
	} ;

	m_oRealTextProxy.clear() ;

	if (m_oiTextProxy)
	{
		for(Page=0; Page<GetNumLayoutPages(); ++Page)
		{
			m_oiTextProxy[Page] = NULL;
			m_iPDFPage[Page] = -1;
		}
	}
}

m4bool_t ClENG_DvcPDFUnicode::InitPage(m4int16_t ai_iIdPage,
							DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY,
							size_t ai_szBlockBuffer,
							ClENG_PagePrinter::ORIENTATION  ai_Orientation,
							ClENG_PagePrinter::PAPER_SIZE   ai_PageSize,
							ClENG_PagePrinter::PAPER_SOURCE ai_PaperSource,
							DvcUnit_t ai_SizeX, DvcUnit_t ai_SizeY,
							ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t SwOk = M4_TRUE;
	m4bool_t initializePhantom ;

	if(!M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPDF_MSG_10 ] ) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit ;
	};

	m_poPagePrinter[ai_iIdPage].Init(ai_Orientation,ai_PageSize,ai_PaperSource,
		                             ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY );

	SwOk = ClENG_DvcFile::InitPage(
		ai_iIdPage, m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(), ai_OffsetX,
		ai_OffsetY, ai_szBlockBuffer, initializePhantom,
		 ClMIT_Trace::PROPAGATE_ERROR) ;
	if(!SwOk) goto exit;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY);
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}

m4bool_t ClENG_DvcPDFUnicode::PreReset()
{
	m4return_t iResult = M4_ERROR;
	m4bool_t SwOk = M4_TRUE;
	m4int16_t Page;
	m4int16_t B  ;
	TextProxyList_t::iterator it;
	ClMIT_Str SystemDebugEnableValue;
	m4int16_t iTextProxyDebugLevel;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	StENG_TaskEnv * TaskEnv = NULL;
	ClMIT_SessionChn * SesChn = NULL;
	ClChannelManager *poChannelManager = NULL;
	ClVMBaseEnv *poEnvironment = NULL;
	m4bool_t bReadOnly = M4_TRUE;

	// Establece el nivel de depuración de iText
#ifdef _DEBUG
	iTextProxyDebugLevel = CliTextProxy::DEBUG_INFO;
#else
	iTextProxyDebugLevel = CliTextProxy::DEBUG_NONE;
#endif

	m_poInsp->m_poSysInfo->GetValue(MIT_RK_SYSTEM_DEBUG_ENABLE, &SystemDebugEnableValue) ;
	if (atoi(SystemDebugEnableValue) == 1)
	{
		iTextProxyDebugLevel |= CliTextProxy::DEBUG_TRACE;
	}

	TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	if (TaskEnv != NULL && TaskEnv->m_APIChn.m_poMVC != NULL)
	{
		SesChn = TaskEnv->m_APIChn.m_poMVC->GetSessChn();
		if (SesChn != NULL)
		{
			bReadOnly = TaskEnv->m_APIChn.m_poMVC->GetSessChn()->IsReadOnlyPDF();
		}
		
		if (m_oiTextContext.IsInitialized() == M4_FALSE)
		{
			poChannelManager = TaskEnv->m_APIChn.m_poMVC->GetChannelManager();

			if (poChannelManager != NULL)
			{
				poEnvironment = poChannelManager->GetEnvironment();

				if (poEnvironment != NULL)
				{
					iResult = m_oiTextContext.Init(poEnvironment, poChannelManager->GetpExecutor(), m_poInsp->m_poTrace);

					if (iResult != M4_SUCCESS)
					{
						SwOk = M4_FALSE;
						goto exit;
					}
				}
			}
		}
	}
	
	//Abre todos los ficheros (Breaks)
	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;

	//------>Datos que solo van una vez al principio del fichero (o ficheros)

	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		ClMIT_Str filename;
		F->GetAbsolutePathFile(&filename);

		if (swRedirect)
		{
			// El fichero ya está asociado a otra página
			it=m_oRealTextProxy.find(filename);
			if (it == m_oRealTextProxy.end())
			{
				SwOk = M4_FALSE;
				goto exit;
			}
			else
			{	
				m_oiTextProxy[Page] = (*it).second ;	
			}
		}
		else
		{
			// Nuevo fichero
			m_oiTextProxy[Page] = new CliTextProxy(m_oiTextContext, filename, bReadOnly, iTextProxyDebugLevel, m_poInsp->m_poTrace);
			m_oRealTextProxy.insert(TextProxyList_t::value_type(filename,m_oiTextProxy[Page]));
			if (m_oiTextProxy[Page]->IsReady() == M4_FALSE)
			{
				SwOk = M4_FALSE;
				// No se puede inicializar el generador de PDF
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY_UNICODE[ CLDVCPDF_UNICODE_MSG_1 ] ) << SEND_MSG ;
				goto exit;
			}

			// Se deshabilita siempre el control de límites
			m_oiTextProxy[Page]->SetCheckLimits( M4_FALSE );

			if (m_bIsRTL_Language == M4_TRUE)
			{
				m_oiTextProxy[Page]->SetTextDirection(CliTextProxy::TEXT_RTL);
			}
		}
	};

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	m_poInsp->m_poTrace->TestEffect(SwOk,ClMIT_Trace::PROPAGATE_ERROR);
	return SwOk ;
}

void ClENG_DvcPDFUnicode::FixPreload()
{
	ClENG_OutDevice::FixPreload();
}

void ClENG_DvcPDFUnicode::PostReset()
{
	Clear_iTextProxy();
	//---------->Cierra ficheros (no hay ATOMIC mode)<<

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}

m4bool_t  ClENG_DvcPDFUnicode::RefreshLayout()
{
	m4bool_t SwOk = M4_TRUE;
	m4int16_t Page;

	for(Page=0; Page<GetNumLayoutPages();++Page)
	{
		m_oiTextProxy[Page]->SetActualPage (m_oiTextProxy[Page]->GetNumPages());
		m_iPDFPage[Page] = m_oiTextProxy[Page]->AddNewPage(m_poPagePrinter[Page].GetMediaBoxX(), m_poPagePrinter[Page].GetMediaBoxY());
	}

	//---------->Propaga
	return SwOk && ClENG_OutDevice::RefreshLayout() ;
};

m4bool_t ClENG_DvcPDFUnicode::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE;

	//---------->Corre EOPs
	RunAllEOP() ;


	//---------->Propaga
	return SwOk && ClENG_OutDevice::FlushLayout();
}

void ClENG_DvcPDFUnicode::PrintNChar(m4int16_t	ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
						  m4pchar_t ai_pcStr, size_t ai_szNChar,
						  m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
						  ClENG_OutDevice::StPIParams *ai_pstPI)
{
	Point_t PosX, PosY;

	// Solo se añade texto si no es un PI. Realmente siempre vendrá a nulo
	if (!ai_pstPI)
	{
		PosX = m_poPagePrinter[ai_iPage].GetPhysicalX( ai_iX ) ;
		PosY = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_iY, GetLastFont(ai_iPage) );

		m_oiTextProxy[ai_iPage]->SetActualPage(m_iPDFPage[ai_iPage]);
		m_oiTextProxy[ai_iPage]->AddText(PosX,PosY,ai_pcStr, ai_szNChar);
	}
}

void ClENG_DvcPDFUnicode::PrintNCharPi(
	ClMIT_Str *ai_poIdPi, m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY,
	size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	if (ai_szNChar)
	{
		m4int_t iActualPage = (m4int_t) m_oiTextProxy[ai_iPage]->GetActualPage();
		m_oPIRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, iActualPage, 
			ai_iX, ai_iY, GetLastFont(ai_iPage), GetLastColour(ai_iPage),
			ai_szNChar, ai_pcHyperlink);
	}
}


void ClENG_DvcPDFUnicode::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPiUnicode_t piData;
	size_t iSize;
	StPiUnicode_t oldStatus;

	if ( m_oPIRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ))
	{
		// Se salvaguarda el estado actual
		oldStatus.m_NNumPage = m_iPDFPage[piData.m_NPage];
		oldStatus.m_oFont = GetLastFont(piData.m_NPage);
		oldStatus.m_Color = GetLastColour(piData.m_NPage);
		// Se calcula el tamaño del texto
		// (no es necesario verificar el límite puesto que no existe un buffer reservado)
		iSize = M4StrLen(ai_pcStr, -1);
		// Se fija la página
		m_iPDFPage[piData.m_NPage] = piData.m_NNumPage;
		// Se establece la fuente (sólo si es necesario)
		SelectFont ( piData.m_NPage, piData.m_oFont );
		// Se establece el color (sólo si es necesario)
		SelectColour ( piData.m_NPage, piData.m_Color );
		// Se añade el texto
		PrintNChar ( piData.m_NPage, piData.m_X, piData.m_Y, ai_pcStr, 
			iSize, M4_FALSE, piData.m_oHyperlink);
		// Se restaura el estado a la situación inicial
		m_iPDFPage[piData.m_NPage] = oldStatus.m_NNumPage;
		SelectFont ( piData.m_NPage, oldStatus.m_oFont);
		SelectColour ( piData.m_NPage, oldStatus.m_Color);
	}
}

void ClENG_DvcPDFUnicode::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont)
{
	m4int16_t numFonts;
	m4int16_t indexFont;
	m4pchar_t sFileName;
	ClMIT_Str TTFFileName("");
	ClMIT_Str FontName;

	int FontStyle;
	ClFONT_M_Font* poDBFont; 
	m4int32_t index = m_poDBFont->FindIndexFont(ai_poFont->GetId());

	if (JNI_FALSE == m_oiTextProxy[ai_iPage]->SetFont(m_poPDFFontNames[index],
		ai_poFont->PSC_GetHeight(), m_poPDFFontStyles[index]))
	{
		if (m_poPDFFontNames[index].StrLen() > 0)
		{
			// Truco para obtener el separador de directorio en función del SO
			ClMIT_SysInfo::AppendToPath( &TTFFileName, "" ) ;
			// Se elimina el path del TTF
			sFileName = strrchr(m_poPDFFontNames[index], TTFFileName[0]);
			// Se toma el nombre
			TTFFileName.StrCpy(sFileName ? sFileName + 1 : (m4pchar_t) m_poPDFFontNames[index]);
			// No se encuentra el archivo de fuentes
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY_UNICODE[ CLDVCPDF_UNICODE_MSG_5 ] )
				<< (m4pchar_t)TTFFileName << SEND_MSG ;
			// Se borra el nombre de la fuente para evitar sucesivos avisos por la misma fuente
			// (el efecto es el mismo que enviar un nombre de fuente vacio)
			numFonts = m_poDBFont->GetNumFonts();

			FontName = m_poPDFFontNames[index];

			// - Se borran los nombres de todos los Id fuentes que apuntan a la misma
			// - Se establecen los modificadores de estilo que tengan definidos
			for (indexFont = 0; indexFont < numFonts; indexFont ++)
			{
				if (0 == ClMIT_Str::StrCmpTrim(FontName, m_poPDFFontNames[indexFont]))
				{
					poDBFont = m_poDBFont->GetAddressFontByIndex(indexFont);
					FontStyle = CliTextProxy::FONT_NORMAL;

					if (poDBFont->IsUnderline())
						FontStyle |= CliTextProxy::FONT_UNDERLINE;
					if (poDBFont->IsBold())
						FontStyle |= CliTextProxy::FONT_BOLD;
					if (poDBFont->IsItalic())
						FontStyle |= CliTextProxy::FONT_ITALIC;

					m_poPDFFontNames[indexFont].StrCpy("");
					m_poPDFFontStyles[indexFont] = FontStyle;

				}
			}
		}
	}
}

void ClENG_DvcPDFUnicode::SelectColourDevice(m4int16_t ai_iPage, m4uint32_t ai_uiIdColour)
{
	m4int16_t PIndex;
	ClENG_Colour* pColour;

	PIndex = m_oFGPalette.GetIndex(ai_uiIdColour);
	pColour = m_oFGPalette.GetColourByIndex(PIndex);

	m_oiTextProxy[ai_iPage]->SetColor(pColour->GetR(), pColour->GetG(), pColour->GetB());
}

void ClENG_DvcPDFUnicode::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{

	Point_t PosX1, PosY1, PosX2, PosY2, LineWidth;

	PosX1 = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X1) ;
	PosX2 = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X2) ;

	PosY1 = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y1, NULL) - 1;
	PosY2 = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y2, NULL) - 1;

	LineWidth = ClMetric::DvcToPoint(ai_LineWidth);

	m_oiTextProxy[ai_iPage]->SetActualPage(m_iPDFPage[ai_iPage]);
	m_oiTextProxy[ai_iPage]->AddLine(PosX1, PosY1, PosX2, PosY2, LineWidth, ai_Style);
}

void ClENG_DvcPDFUnicode::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	Point_t PosX, PosY, Width, Height;

	Width = ClMetric::DvcToPoint(ai_Width) ;
	Height = ClMetric::DvcToPoint(ai_Height) ;
	PosX = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	PosY = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y, NULL) - Height - 1;

	m_oiTextProxy[ai_iPage]->SetActualPage(m_iPDFPage[ai_iPage]);
	m_oiTextProxy[ai_iPage]->FillZone(PosX, PosY, Width, Height);
}	

void ClENG_DvcPDFUnicode::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	Point_t PosX, PosY, Width, Height, LineWidth;

	Width = ClMetric::DvcToPoint(ai_Width);
	Height = ClMetric::DvcToPoint(ai_Height);
	PosX = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	PosY = (m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y, NULL) - Height - 1);
	LineWidth = ClMetric::DvcToPoint(ai_LineWidth);

	m_oiTextProxy[ai_iPage]->SetActualPage(m_iPDFPage[ai_iPage]);
	m_oiTextProxy[ai_iPage]->AddRectangle(PosX, PosY, Width, Height, LineWidth, ai_Style);
}

void ClENG_DvcPDFUnicode::PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID, 	m4bool_t m_bIsTempWebPath )
{
	Point_t PosX, PosY, Width, Height;
#ifdef _WINDOWS
	m4pchar_t pLocalPath;
	ClMIT_File oMitFile;
#endif

	PosX = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	PosY = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y+ai_SizeY, NULL) ;
	Width = ClMetric::DvcToPoint(ai_SizeX);
	Height = ClMetric::DvcToPoint(ai_SizeY);

	m_oiTextProxy[ai_iPage]->SetActualPage(m_iPDFPage[ai_iPage]);

#ifdef _WINDOWS
	// Hay que intentar descargarlo si procede
	pLocalPath = new m4char_t[MAX_PATH_SIZE + 1];
	*pLocalPath = '\0';
	// TryToDownloadFromURL debería ser estático.
	// Se utiliza una instancia para acceder y no cambiar la mienv
	if (M4_TRUE == oMitFile.TryToDownloadFromURL(ai_pcPath, MAX_PATH_SIZE, pLocalPath))
	{
		m_oiTextProxy[ai_iPage]->AddImage(pLocalPath, PosX, PosY, Width, Height);
	}

	delete [] pLocalPath;
#else
	m_oiTextProxy[ai_iPage]->AddImage(ai_pcPath, PosX, PosY, Width, Height);
#endif
}