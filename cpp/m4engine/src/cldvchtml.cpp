//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvhtml.cpp
// Project:		     engindll.dll
// Author:           Meta Software M.S. , S.A
// Date:             28/01/2000
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"
#include "sysinfo.h"

#include "font.h"
#include "clfile.h"
#include "cldvchtml.hmg"
#include "cldvchtml.h"
#include "rasterg.h"

#include "m4unicode.hpp"


//------------------- Delimitadores templates--------------------------------

const m4char_t * ClENG_DvcHTML::m_pcTagDelimOpen	= "<m4{" ;
const m4char_t * ClENG_DvcHTML::m_pcTagDelimClose	= "}m4>";
const m4char_t * ClENG_DvcHTML::m_pcValueDelim		= "}={" ;

//------------------- Ids de patterns para templates--------------------------
/*

    title		Scope: break
	-----

	<m4{title}m4>

 El resultado es un texto con el titulo del report.
 El alcance es de break, porque el numero de pagina actual forma parte del title.

    language	Scope : report
	--------

	<m4{language}m4>

 El resultado es el id del lenguaje con el que el report se ejecuta.
  
	reportroot	Scope : report
	----------

	<m4{reportroot}m4>

  El resultado es una url al root del report. Si se indico /WEB en la ejecucion, sera una url
relativa, si no, absoluta.

  
	glbfile		Scope : report
	-------

	<m4{glbfile}={logo.gif}m4>

 El resultado sera una url relativa, para acceder desde un fichero flush a un fichero
global. Es decir, el ejemplo anterior daria, si el report tiene breaks,  ../images/logo.gif
y si no los tuviese images/logo.gif. Además, causa que logo.gif se baje si no existe en 
images.
 El fichero especificado (logo.gif) debe ser a nivel raiz y sin / ni paths
 Puede ser cualquier tipo de fichero, no solo gifs. Eso si, son globales, es decir, el
contenido de esos ficheros NO se toca, es el mismo incluso para reports diferentes.
 

	brkfile		Scope : report, declaracion ficheros break			
	-------

  <m4{brkfile}={M4_INDEX.html}m4>

 Parecido a glbfile, pero para globales a break. El contenido de los ficheros sí contiene 
datos que varian segun el report y el break. brkfile puede ser un path o un path mas 
parte de nombre de fichero, segun los modo BREAK_FILE.
 El resultado sera una url relativa, para acceder desde un fichero flush a un fichero 
global a break. El ejemplo anterior daría para un report con breaks (cada uno a un dir)
M4_INDEX.html, y para uno sin breaks daria un HTMLFile_Thread_32_M4_INDEX.html. En ambos
casos, debe existir una template llamada M4_INDEX.html para ser cargada.
 Debe referenciar a ficheros de plantilla, no es correcto usar graficos o formatos binarios.
NO se baja ningun fichero (como ocuuria con glbfile), si no que se carga la plantilla, se 
busca en ella referencias glbfile para bajarlas, y se buscan otras referencias a otros 
brkfile para seguir cargandolos también recursivamente. Despues, por cada break, se 
generaran juegos de ficheros instanciando la plantilla. 
  El proceso tiene en cuenta posibles incursiones ciclicas y trata con ellas. 


	brkfilelang		Scope : report, declaracion ficheros break
	-----------

  <m4{brkfilelang}={m4_lang.html}m4>

  Exactamente igual que brkfile, pero con una salvedad: el fichero desde el que se lee la 
template es el indicado, pero con un _LeguajeId en el nombre. Por ejemplo, el ejemplo anterior
si estamos en la Mind en ingles, la plantilla se leeria desde m4_lang_2.html .


	brkindex , brkindexnode, brkindexleaf, brkindexclose	Scope : report, decl. fich. indice
	-----------------------------------------------------

  <m4{brkindex}		={m4_index.html}m4>
  <m4{brkindexnode}	={m4_index_node.html}m4>
  <m4{brkindexleaf}	={m4_index_leaf.html}m4>
  <m4{brkindexclose}={m4_index_close.html}m4>

  El proceso de generación de fichero TOC es especial. El engine construye un unico fichero ,
con el nombre correspondiente al fichero break indicado por brkindex. El fichero esta formado
por:
	1 instancia de la template brkindex
	n instancias de la template brkindexnode, una por nodo no hoja de la tabla TOC.
	n instancias de la template brkindexleaf, una por nodo hoja de la tabla TOC.
	1 instancia de la template brkindexclose

  Estas templates YA deben estar referenciadas en algun brkfile o brkfilelang. Estos tags lo
unico que hacen es indicar cuales de todas templates breaks cargadas ejercen la funcion de 
indices.
  Lo mas seguro y claro es poner en la cabecera del brkindex unos comentarios incluyendo los
brkfile y brkfilelang necesarios para forzar a cargarlos y despues indicar los brkindex, etc.

	indexlevel, indexlevelfather,  indexstring,  indexpage, indexfile, indextarget
	------------------------------------------------------------------------------
				Scope : templates indice brkindexnode y brkindexleaf
			

  <m4{indexlevel}m4>
  <m4{indexlevelfather}m4>
  <m4{indexstring}m4>
  <m4{indexpage}m4>
  <m4{indexfile}m4>
  <m4{indextarget}m4>

  Utiles solo para las templates brkindexnode y brkindexleaf, contienen la informacion de la
entrada TOC que se esta insertando.


	rootnavigation		Scope : report, declaracion fichero root de break
	--------------

  <m4{rootnavigation}={m4_main.html}m4>

  Es un parametro opcional. Si aparece, dice cual sera el fichero break que se marcara como 
main de navegacion. Como en brkindex, etc, este tag no carga el fichero, este debe 
ser cargado con brkfile, etc .

    firstfile		Scope : break
	---------

  <m4{firstfile}m4>

 El resultado sera una url relativa, para acceder desde un fichero flush o global a break 
a el primer fichero flush del break. El ejemplo anterior podria dar un HTMLFile_Thread_320_0.htm


	navigation, top_navigation, bottom_navigation, toc, zoom, search	Scope : report
	----------------------------------------------------------------

  <m4{navigation}m4>
  <m4{top_navigation}m4>
  <m4{bottom_navigation}m4>
  <m4{toc}m4>
  <m4{zoom}m4>
  <m4{search}m4>

  El resultado son flags true o false indicando si están o no activadas las correspondientes
caracteristicas. Util para usarlos en JavaScript y alterar templates.


  navigation_array			Scope : break
  ----------------

  <m4{navigation_array}m4>

  El resultado es una lista de ficheros flush correspondientes al actual break. Esta lista 
esta en formato listo para insertar y crear un array en JavaScript. Pe:
	"PRUEBA_1.htm", "PRUEBA_2.htm"

*/

#define M4_TITLE			"title"
#define M4_LANGUAGE			"language"
#define M4_REPORTROOT		"reportroot"
#define M4_GLB_FILE			"glbfile"
#define M4_BRK_FILE			"brkfile"
#define M4_BRK_FILE_LANG	"brkfilelang"
#define M4_FIRST_FILE		"firstfile"

#define M4_NAVIGATION			"navigation"
#define M4_TOP_NAVIGATION		"top_navigation"
#define M4_BOTTOM_NAVIGATION	"bottom_navigation"
#define M4_TOC					"toc"
#define M4_ZOOM					"zoom"
#define M4_SEARCH				"search"

#define M4_NAVIGATION_ARRAY		"navigation_array"

#define M4_BRK_INDEX			"brkindex"
#define M4_BRK_INDEX_NODE		"brkindexnode"
#define M4_BRK_INDEX_LEAF		"brkindexleaf"
#define M4_BRK_INDEX_CLOSE		"brkindexclose"
#define M4_ROOT_NAVIGATION		"rootnavigation"

#define M4_BRK_INDEX_LEVEL			"indexlevel"
#define M4_BRK_INDEX_LEVEL_FATHER	"indexlevelfather"
#define M4_BRK_INDEX_STRING			"indexstring"
#define M4_BRK_INDEX_PAGE			"indexpage"
#define M4_BRK_INDEX_FILE			"indexfile"
#define M4_BRK_INDEX_TARGET			"indextarget"

// UNICODE REPORTS
#define M4_CHARSET				"charset"



const m4char_t *ClENG_DvcHTML::m_pcPattenIds[] =
{
	M4_GLB_FILE,
	M4_TITLE,
	M4_LANGUAGE,
	M4_REPORTROOT,
	M4_BRK_FILE,
	M4_BRK_FILE_LANG,
	M4_FIRST_FILE,
	M4_NAVIGATION,
	M4_TOP_NAVIGATION,
	M4_BOTTOM_NAVIGATION,
	M4_TOC,
	M4_ZOOM,
	M4_SEARCH,
	M4_NAVIGATION_ARRAY,
	M4_BRK_INDEX,
	M4_BRK_INDEX_NODE,
	M4_BRK_INDEX_LEAF,
	M4_BRK_INDEX_CLOSE,
	M4_ROOT_NAVIGATION,
	M4_BRK_INDEX_LEVEL,
	M4_BRK_INDEX_LEVEL_FATHER,
	M4_BRK_INDEX_STRING,
	M4_BRK_INDEX_PAGE,
	M4_BRK_INDEX_FILE,
	M4_BRK_INDEX_TARGET,
	// UNICODE REPORTS
	M4_CHARSET,

	NULL
} ;

#define M4_PATTERN(idPattern) &(*m_oPatterns.find((idPattern))).second ;

//------------------- Especificaciones DHTML --------------------------------

//------------------- Offsets de pagina (Limites fisicos) -------------------

//#define OFFSET_Y 2000
//#define OFFSET_X 5000
#define OFFSET_GRAPH 1.334
#define BAR_CODE_OFFSET 1

//------------------- Auxiliares --------------------------------------------


#define HTML_LINE_JUMP "\r\n"
#define HTML_PAGE_SEPARATOR "<DIV STYLE='position:absolute; width: %lu;  color:#000000; left:%lupt; top:%lupt;'>\r\n<hr size=%lu>\r\n</DIV>\r\n"
#define HTML_PAGE_SEPARATOR_WIDTH ClMetric::PointToDvc(1)


// ------------------ Tratamiento para las PIs ------------------------------


#define HTML_PI_OFFSET 10


//------------------- Instancia ---------------------------------------------


#define HTML_DIV_STYLE   "<DIV STYLE='position:absolute; width: %ipt; "
#define HTML_DIV_STYLE_Z "<script>\r\nvar OutStr=\"<DIV STYLE='position:absolute; width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; "


//#define HTML_DIV_PRELOAD "<DIV STYLE='position:absolute;"
#define HTML_DIV_CLOSE   "'>\r\n"
#define HTML_DIV_CLOSE_Z "'>\";\r\ndocument.write(OutStr);\r\n</script>\r\n"
#define HTML_DIV_END     "</DIV>\r\n"


//-------------------- Posicion absoluta ------------------------------------


#define HTML_LEFT        "left:%ipt; "
#define HTML_TOP         "top:%ipt;"
#define HTML_LEFT_Z	     "left:\"+parent.parent.frames['menu'].zoom*%i+\"pt; "
#define HTML_TOP_Z		 "top:\"+parent.parent.frames['menu'].zoom*%i+\"pt; "


//------------------- Tipos de fuente ---------------------------------------


#define HTML_FONT_FAMILY "font-family:%s; "
#define HTML_FONT_SIZE   "font-size:%ipt; "
#define HTML_FONT_SIZE_Z "font-size:\"+parent.parent.frames['menu'].zoom*%i+\"pt; "
#define HTML_COLOR       "color:#%.2X%.2X%.2X; "
#define HTML_FONT_ITALIC    "font-style:italic; "
#define HTML_FONT_BOLD      "font-weight:bold; "
#define HTML_FONT_UNDERLINE "text-decoration:underline; "


//------------------- Imagenes ----------------------------------------------

#define HTML_IMAGE_START	"<img border=0 src="
#define HTML_IMAGE_START_Z	"<script>\r\nvar outimg=\"<img border=0 src="
#define HTML_IMAGE_WIDTH	" width=%i"
#define HTML_IMAGE_HEIGHT	" height=%i"
#define HTML_IMAGE_WIDTH_Z	" width=\"+parent.parent.frames['menu'].zoom*%i+\""
#define HTML_IMAGE_HEIGHT_Z " height=\"+parent.parent.frames['menu'].zoom*%i+\""
#define HTML_IMAGE_END		">\r\n"
#define HTML_IMAGE_END_Z	">\";\r\ndocument.write(outimg);\r\n</script>"

//------------------- Indices ------------------------------------------------


#define HTML_INDEX_ENTRY   "<a name=%i>&nbsp;</a>"

//------------------- Search --------------------------------------------------

/*
#define HTML_SEARCH_PAGE   "<body bgcolor=#ffffff topmargin=10 leftmargin=5 marginheight=10 marginwidth=5>\r\n<FORM METHOD=POST ACTION=/cgi-bin/m4search.pl>\r\n	<INPUT TYPE=hidden NAME=base VALUE=./m4webservices/m4thinclientroot/reports/>\r\n	<INPUT TYPE=hidden NAME=rbase VALUE=/m4webservices/m4thinclientroot/reports/>\r\n	<INPUT TYPE=hidden NAME=lang VALUE=2>\r\n	<INPUT TYPE=hidden NAME=bgcolor VALUE=#FFFFFF>\r\n	<INPUT TYPE=hidden NAME=background VALUE=>\r\n	<INPUT TYPE=hidden NAME=text VALUE=#000000>\r\n	<INPUT TYPE=hidden NAME=link VALUE=#000080>\r\n	<INPUT TYPE=hidden NAME=alink VALUE=#ED074D>\r\n	<INPUT TYPE=hidden NAME=vlink VALUE=#000080>\r\n	<INPUT TYPE=hidden NAME=type VALUE=all>\r\n<table width=100% border=0 cellpadding=2 cellspacing=0>\r\n  <tr>\r\n    <td align=center bgcolor=#0879AD><font face=Arial size=2 color=#ffffff><b>SEARCH</font></td>\r\n  </tr>\r\n  <tr>\r\n    <td align=center><font face=Arial size=2 color=#000000><p align=justify>\r\n    <br>Enter keywords to search for in the report. You can perform a Boolean search, and have search results match all the words, or any of them.<br><br>\r\n    </td>\r\n  </tr>\r\n  <tr>\r\n  <td>\r\n  <table width=100% border=1 border-color=#0879AD cellpadding=0 cellspacing=0>\r\n   <tr>\r\n    <td align=center>\r\n      <font face=Arial size=2 color=#000000><br><b>Search</b><br><INPUT TYPE=TEXT NAME=sv SIZE=20 STYLE='font-family: Verdana; font-size: 11px'>\r\n      <br><br>\r\n      <font face=Arial size=2 color=#000000><b>Match</b>\r\n	      <SELECT NAME=type STYLE='font-family: Verdana; font-size: 11px'>\r\n		<OPTION VALUE=all SELECTED>All\r\n		<OPTION VALUE=any>Any\r\n	      </SELECT> <b>Words</b>\r\n      <br><br>	\r\n      <input type=submit value= Search  STYLE='font-family: Verdana; font-size: 11px'>&nbsp;&nbsp;\r\n      <input type=reset value= Clear  STYLE='font-family: Verdana; font-size: 11px'></font><br><br>\r\n  </tr>\r\n  </table>\r\n  </td></tr>\r\n </table>\r\n</table>\r\n</FORM>\r\n</body>"
*/

//------------------- Links --------------------------------------------------


#define HTML_LINK_BLANK "<a href=%s target=_blank>"
#define HTML_LINK_END   "</a>"

//#define HTML_LINK_LEFT  "<- Page %i"
//#define HTML_LINK_RIGHT "Page %i ->"

//------------------- Lines, Boxes & Fill Colours ---------------------------

// Bug 176528. Se vuelve a introducir el &nbsp en los DIV para el dibujado de las líneas y garantizar el comportamiento previo al SP4
// Más adelante se debería revisar ya que si el &nbsp y con Height superior la líneas laterales se muestran continuas cuando hay
// múltiples registros
#define HTML_VERT_LINE   "<DIV style='position:absolute; left:%ipt; top:%ipt; border-color:#%.2X%.2X%.2X; border-style:solid; border-top-width:0pt;  border-left-width:%fpt;border-right-width:0pt; border-bottom-width:0pt;'>\r\n<table height=%ipt width=0pt><tr><td>&nbsp;</td></tr></table></div>\r\n"
#define HTML_HORI_LINE   "<DIV style='position:absolute; left:%ipt; top:%ipt; border-color:#%.2X%.2X%.2X; border-style:solid; border-top-width:%fpt;  border-left-width:0pt;border-right-width:0pt; border-bottom-width:0pt;width:%ipt;'>\r\n<table height=0pt width=%ipt><tr><td>&nbsp;</td></tr></table></div>\r\n"
#define HTML_VERT_LINE_Z "<script>\r\nvar OutStr=\"<DIV style='position:absolute; left:\"+parent.parent.frames['menu'].zoom*%i+\"pt; top:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-color:#%.2X%.2X%.2X; border-style:solid; border-top-width:0pt;  border-left-width:\"+parent.parent.frames['menu'].zoom*%f+\"pt;border-right-width:0pt; border-bottom-width:0pt;'><table height=\"+parent.parent.frames['menu'].zoom*%i+\"pt width=0pt><tr><td>&nbsp;</td></tr></table>\";\r\ndocument.write(OutStr);\r\n</script>\r\n</div>\r\n"
#define HTML_HORI_LINE_Z "<script>\r\nvar OutStr=\"<DIV style='position:absolute; left:\"+parent.parent.frames['menu'].zoom*%i+\"pt; top:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-color:#%.2X%.2X%.2X; border-style:solid; border-top-width:\"+parent.parent.frames['menu'].zoom*%f+\"pt;  border-left-width:0pt;border-right-width:0pt; border-bottom-width:0pt;width:\"+parent.parent.frames['menu'].zoom*%i+\"pt;'><table height=0pt width=\"+parent.parent.frames['menu'].zoom*%i+\"pt><tr><td>&nbsp;</td></tr></table>\";\r\ndocument.write(OutStr);\r\n</script>\r\n</div>\r\n"
#define HTML_FILL		 "<DIV style='border-style:none; border-bottom-style:solid; border-bottom-width:1pt; border-left-style:solid; border-left-width:1pt; border-top-style:solid; border-top-width:1pt; border-right-style:solid; border-right-width:1pt; width:%ipt; height:%ipt; border-color:#%.2X%.2X%.2X; position:absolute; background-color:#%.2X%.2X%.2X; layer-background-color:#%.2X%.2X%.2X; left:%ipt; top:%ipt;'></DIV>\r\n"
#define HTML_FILL_Z		 "<script>\r\nvar OutStr=\"<DIV style='border-style:none; border-bottom-style:solid; border-bottom-width:1pt; border-left-style:solid; border-left-width:1pt; border-top-style:solid; border-top-width:1pt; border-right-style:solid; border-right-width:1pt; width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; height:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-color:#%.2X%.2X%.2X; position:absolute; background-color:#%.2X%.2X%.2X; layer-background-color:#%.2X%.2X%.2X; left:\"+parent.parent.frames['menu'].zoom*%i+\"pt; top:\"+parent.parent.frames['menu'].zoom*%i+\"pt;'>\";\r\ndocument.write(OutStr);\r\n</script>\r\n</DIV>\r\n"
#define HTML_BOX		 "<DIV style='border-color:#%.2X%.2X%.2X; border-style:none; border-bottom-style:solid; border-bottom-color:#%.2X%.2X%.2X; border-bottom-width:%ipt; border-left-style:solid; border-left-color:#%.2X%.2X%.2X; border-left-width:%ipt; border-top-style:solid; border-top-color:#%.2X%.2X%.2X; border-top-width:%ipt; border-right-style:solid; border-right-color:#%.2X%.2X%.2X; border-right-width:%ipt; width:%ipt; height:%ipt; border-color:#%.2X%.2X%.2X; position:absolute; left:%ipt; top:%ipt;'></DIV>\r\n"
#define HTML_BOX_Z		 "<script>\r\nvar OutStr=\"<DIV style='border-color:#%.2X%.2X%.2X; border-style:none; border-bottom-style:solid; border-bottom-color:#%.2X%.2X%.2X; border-bottom-width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-left-style:solid; border-left-color:#%.2X%.2X%.2X; border-left-width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-top-style:solid; border-top-color:#%.2X%.2X%.2X; border-top-width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-right-style:solid; border-right-color:#%.2X%.2X%.2X; border-right-width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; width:\"+parent.parent.frames['menu'].zoom*%i+\"pt; height:\"+parent.parent.frames['menu'].zoom*%i+\"pt; border-color:#%.2X%.2X%.2X; position:absolute; left:\"+parent.parent.frames['menu'].zoom*%i+\"pt; top:\"+parent.parent.frames['menu'].zoom*%i+\"pt;'>\";\r\ndocument.write(OutStr);\r\n</script>\r\n</DIV>\r\n"

//------------------- Inicializaciones --------------------------------------

ClENG_DvcHTML::ClENG_DvcHTML() 
{
	m_poPagePrinter = NULL;
	m_poYOffset = NULL ;
	m_poDBFont = NULL ;
}

void ClENG_DvcHTML::End() 
{
	BreakTemplates_t::iterator it ;
	m4int16_t i ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		            ClENG_PagePrinter, GetNumLayoutPages());
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poYOffset,
		            DvcUnit_t, GetNumLayoutPages());

	m_poDBFont = NULL ;
	m_oHTMLMenuFile.End() ;
	m_oReportName.End() ;

	m_oDataStart.End() ;
	m_oDataEnd.End()   ;
	m_oPatterns.clear() ;

	for ( it=m_oBreakTemplates.begin() ; it!=m_oBreakTemplates.end(); ++it )	{
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, (*it).second.m_poTemplate, 
			ClMIT_CopyReplace ) ;
	} ;

	m_oBreakTemplates.clear() ;

	m_oTmplIndex.End() ;
	m_oTmplIndexNode.End() ;
	m_oTmplIndexLeaf.End() ;
	m_oTmplIndexClose.End() ;

	for (i=0; i < NAME_COUNT; ++i) m_TemplateValues[i].End() ;

	ClENG_DvcFile::End();
}

m4bool_t ClENG_DvcHTML::Init( 
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Especificos de device

		ClFONT_M_DBFont* ai_poDBFont, 
		m4bool_t ai_bTableOfContents, 
		HTML_NAVIGATION  ai_NavigationMode, 
		HTML_MENU ai_HTMLMenu,
		m4bool_t ai_bZoom, 
		m4bool_t ai_bSearch, 
		m4language_t ai_Language,
		const m4char_t *ai_pcReportName,

		const m4char_t *ai_pcStartTemplate, const m4char_t *ai_pcEndTemplate,
		const m4char_t *ai_pcMainTemplate,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{

	m4bool_t SwOk = M4_TRUE;
	ClMIT_Str path, str ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t *pcBuff = TaskEnv->Dvc_g_pcBuff;
	ClMIT_ReplacePattern *pattern ;

	if (ai_Preserve==ClENG_ReportOuputRepository::PRESERVE_MOVE_NAME) {
		ai_Preserve = ClENG_ReportOuputRepository::PRESERVE_DIR ;
	} ;
	
	ClENG_DvcHTML::End() ;

	m_bZoom = ai_bZoom;
	m_bSearch = ai_bSearch;
	m_bTOC = ai_bTableOfContents ;
	m_Language = ai_Language;
	m_NavigationMode = ai_NavigationMode;
	m_poDBFont = ai_poDBFont ;
	m_oHTMLMenuFile.StrCpy(ai_FileParams->m_oHTMLMenuFile) ;
	m_oHTMLMenuFile.Trim() ;
	m_HTMLMenuPos = ai_HTMLMenu;
	m_oReportName.StrCpy( ai_pcReportName ) ;

	//En este device NO se permite el UseDelBreak

	ai_FileParams->m_bUseDelBreak=M4_FALSE ;
	
	//En este device NO se permite declarar interes

	ai_DeclareExtFile = NOT_DECLARED ;

	//Activamos todos los Flags, puesto que siempre soporta color
	
	SwOk = ClENG_DvcFile::Init(
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		//No se permite preload, graficos estaticos no se reposicionan en un HTML con 
		//	varias paginas
		//ClENG_OutDevice::PRELOAD_ON_DEVICE |  
		ClENG_OutDevice::COLOUR_SUPPORTED | 
		ClENG_OutDevice::COLOUR_ACTIVATED | 
		ClENG_OutDevice::RASTER_SUPPORTED,
		
		ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak, //MultiBreak
		ai_FileParams, ai_bDelete,
		ai_bForceMonoBreak ?  ClENG_BreakList::BREAK_TO_FILE : ClENG_BreakList::BREAK_TO_DIR,
		ai_NavigationMode==NAVIGATION_ACTIVE ? ClENG_BreakList::ATOMIC_DUPLEX :
			ClENG_BreakList::SHARING_DUPLEX,
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 
		ClMIT_Trace::PROPAGATE_ERROR

	);
	if(!SwOk) goto exit;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		         ClENG_PagePrinter, GetNumLayoutPages());
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poYOffset,
		         DvcUnit_t, GetNumLayoutPages());

	//Testeamos que existe el menu de navegacion, si es que se pasa
	//Si existe, lo modifica para que apunte al path absoluto

	if( m_oHTMLMenuFile.StrLen()>0 ) {
	
		m_poInsp->m_poSysInfo->GetValue(ENG_RK_GRAPHICS_PATH_HTML,&path) ;
		sprintf(pcBuff, "%i", m_Language);
		ClMIT_SysInfo::AppendToPath(&path, pcBuff);	
		ClMIT_SysInfo::AppendToPath(&path, m_oHTMLMenuFile.InChar(0) );
		m_oHTMLMenuFile.StrCpy(path) ;

		if ( ! ClMIT_File::Exist( m_oHTMLMenuFile.InChar(0)) ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163013, 
			"Unable to open navigation menu definition file: %0:s.", ClMIT_Msg::MIXED_OUT)
				<< m_oHTMLMenuFile.InChar(0) << SEND_MSG ;
			SwOk = M4_FALSE;
			goto exit;
		} ;
	};

	//Cargamos la lista de patterns

	ClMIT_CopyReplace::LoadPatternList(&m_oPatterns, m_pcPattenIds ) ;

	//***>Y cargamos los valores que permanecen invariables durante todo el report

	//M4_GLB_FILE contiene path WEB a globales, relativo, y con / final
	//Es importante crearlo en el Init para que graficos PreLoad tengan bien las referencias.
	//	(llamada a PrintRasterLine antes del breaks).

	m_oOutRepository.GetRelativePathFromFlushToGlobal(&str) ;
	ClMIT_File::ConvertToWebPath(&str) ;
	pattern = M4_PATTERN( M4_GLB_FILE ) ;
	pattern->ChangeValue(str) ;


	str.Accommodate(50) ;
	sprintf(str.InChar(0), "%lu", (unsigned long)ai_Language) ;
	str.OverStringValid() ;
	pattern = M4_PATTERN( M4_LANGUAGE ) ;
	pattern->ChangeValue(str) ;

	pattern = M4_PATTERN( M4_NAVIGATION ) ;
	pattern->ChangeValue( GetNavigationMode()==NAVIGATION_ACTIVE ? "true" : "false"  ) ;

	pattern = M4_PATTERN( M4_TOP_NAVIGATION ) ;
	pattern->ChangeValue( m_HTMLMenuPos==TOP ? "true" : "false"  ) ;

	pattern = M4_PATTERN( M4_BOTTOM_NAVIGATION ) ;
	pattern->ChangeValue( m_HTMLMenuPos==BOTTOM ? "true" : "false"  ) ;

	pattern = M4_PATTERN( M4_TOC ) ;
	pattern->ChangeValue( IsTOC() ? "true" : "false"  ) ;

	pattern = M4_PATTERN( M4_ZOOM ) ;
	pattern->ChangeValue( IsZoom() ? "true" : "false"  ) ;

	pattern = M4_PATTERN( M4_SEARCH ) ;
	pattern->ChangeValue( IsSearch() ? "true" : "false"  ) ;

	m_oOutRepository.GetNavigationRootPathMask(&str) ;
	ClMIT_File::ConvertToWebPath(&str) ;
	pattern = M4_PATTERN( M4_REPORTROOT ) ;
	pattern->ChangeValue( str ) ;

	//<***

	//Cargamos solo las templates que nos van a hacer falta

	SwOk = LoadTemplate( &m_oDataStart, ai_pcStartTemplate ) ;
	if (!SwOk) goto exit ;

	SwOk = LoadTemplate( &m_oDataEnd  , ai_pcEndTemplate ) ;
	if (!SwOk) goto exit ;

	if ( GetNavigationMode()==NAVIGATION_ACTIVE  ||  IsTOC() ) {
	
		//Carga templates break

		SwOk = LoadBreakTemplates( ai_pcMainTemplate, M4_FALSE ) ;
		if (!SwOk) goto exit ;

		//Enlaza templates especiales de indice

		SwOk = LinkToIndexTemplates() ;
		if (!SwOk) goto exit ;

	} ;

exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}


m4bool_t ClENG_DvcHTML::InitPage(m4int16_t ai_iIdPage,
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
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163001, 
			"Page number out of range", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit ;
	};

	m_poPagePrinter[ai_iIdPage].Init(ai_Orientation,ai_PageSize,ai_PaperSource,
		                             ai_SizeX, ai_SizeY);

	SwOk = ClENG_DvcFile::InitPage(
		ai_iIdPage, m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(), ai_OffsetX,
		ai_OffsetY, ai_szBlockBuffer, initializePhantom,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if(!SwOk) goto exit;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY);
		
	} ;


exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}


m4bool_t ClENG_DvcHTML::PostPagesInit() 
{
	ResetYOffsets() ;
	return M4_TRUE ;
}


void ClENG_DvcHTML::ResetYOffsets() 
{
	m4int16_t B, page ;
	DvcUnit_t buffAcu, fileAcu ;


	for ( page=0; page < GetNumLayoutPages(); ++page ) {

		//Primero se acumula lo YA impreso en fichero al que corresponde page, 
		//	que es todo independientemente de la pagina que estemos tratando, siempre que
		//	esta comparta fichero.

		for ( B=0, fileAcu=0; B < GetNumLayoutPages(); ++B ) {
			if ( m_oOutRepository.ShareFileWith(B,page) ) {
				fileAcu +=  m_oOutRepository.GetPrintedPagesOnlyForPage(B) * GetSizeY(B) ;
			} ;
		} ;

		//Ahora hayamos el acumulado por offsets en buffer aun no impresos
		//Solo recorre hasta el que pedimos (incluido), porque los posteriores tambien 
		//	iran despues en el fichero.
	
		for ( B=0, buffAcu=0 ; B<page; ++B ) {
		
			//Solo contamos las paginas que van al mismo fichero que el reseteado,
			//	y que son anteriores a la tratada
			if ( m_oOutRepository.ShareFileWith(B,page) ) {
				buffAcu += GetSizeY(B) ;
			} ;
		} ;

		//El aoffset total es el del fichero + el del buffer
		m_poYOffset[page]=fileAcu+buffAcu ;
	} ;
}

m4bool_t ClENG_DvcHTML::LoadBreakTemplates(const m4char_t *ai_pcTemplateFileName, m4bool_t ai_bSwLangDependant)
{
	m4bool_t SwOk=M4_TRUE ;
	BreakTemplates_t::iterator it;
	ClMIT_CopyReplace *tmpl=NULL ;
	ClMIT_Str key, nameAux, strAux, extAux ;
	ClMIT_ReplacePattern *pattern ;
	ClMIT_ReplacePattern::MatchingVector_t::iterator matchIt, matchEnd ;
	ClMIT_ReplacePattern::MatchingVector_t matchings, matchingsLang ;
	StBreakSecond brkSecond ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::LoadBreakTemplates() , template file %0:s, is language dependant %1:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcTemplateFileName << ai_bSwLangDependant << SEND_MSG   ;
	};
	#endif

	//Si ya existe, evita recursion infinita 

	key.StrCpy(ai_pcTemplateFileName) ;
	it = m_oBreakTemplates.find(key) ;
	if ( it != m_oBreakTemplates.end() ) {
		goto exit ; 
	} ;

	//Crea y mete en map para que este se encargue del borrado.

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, tmpl, ClMIT_CopyReplace ) ;
	brkSecond.m_poTemplate = tmpl  ;
	brkSecond.m_bSwCreateFile = M4_TRUE ;
	m_oBreakTemplates.insert( BreakTemplates_t::value_type(key,brkSecond) ) ;

	//Carga template (baja graficos, etc)

	if (ai_bSwLangDependant) {
		ClMIT_File::SplitPath(ai_pcTemplateFileName, &strAux, &nameAux, &extAux ) ;
		strAux.Accommodate(20) ;
		sprintf( strAux.InChar(0), "%lu", (unsigned long)m_Language) ;
		strAux.OverStringValid() ;
		nameAux.StrCat("_") ;
		nameAux.StrCat( strAux ) ;
		ClMIT_File::ConcatFileExt(&nameAux,extAux) ;
	} else {
		nameAux.StrCpy(ai_pcTemplateFileName) ;
	} ;
	SwOk = LoadTemplate(tmpl, nameAux ) ;
	if (!SwOk) goto exit ;

	//Copia matchings por M4_BRK_FILE y M4_BRK_FILE_LANG porque llamadas recursivas las borran

	pattern = M4_PATTERN(M4_BRK_FILE) ;
	pattern->GetMatchings( &matchings ) ;
	
	pattern = M4_PATTERN(M4_BRK_FILE_LANG) ;
	pattern->GetMatchings( &matchingsLang ) ;

	//Carga recursiva por M4_BRK_FILE (no dependiente de lenguaje)

	for ( matchIt=matchings.begin() ; matchIt != matchings.end(); ++matchIt ) {
		SwOk = LoadBreakTemplates( (*matchIt), M4_FALSE ) ;
		if (!SwOk) goto exit ;
	} ;

	//Carga recursiva por M4_BRK_FILE_LANG (dependiente de lenguaje)
	
	for ( matchIt=matchingsLang.begin() ; matchIt != matchingsLang.end(); ++matchIt ) {
		SwOk = LoadBreakTemplates( (*matchIt), M4_TRUE ) ;
		if (!SwOk) goto exit ;
	} ;
exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::LoadBreakTemplates() SwOk %0:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	return SwOk ;

}

m4bool_t ClENG_DvcHTML::LoadTemplate(ClMIT_CopyReplace* ai_poTemplate, const m4char_t *ai_pcTemplateFileName)
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str htmlPath, path ;
	ClMIT_ReplacePattern *pattern ;
	ClMIT_ReplacePattern::MatchingVector_t matchings ;
	ClMIT_ReplacePattern::MatchingVector_t::iterator it, itEnd ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::LoadTemplate() from template %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcTemplateFileName << SEND_MSG   ;
	};
	#endif

	//*****Haya htmlPath para cargas globales

	m_poInsp->m_poSysInfo->GetValue(ENG_RK_GRAPHICS_PATH_HTML,&htmlPath) ;
	ClMIT_File::ConvertToPath(&htmlPath) ; 

	//*****Carga template

	path.StrCpy(htmlPath) ;
	ClMIT_File::ConcatToPath(&path,ai_pcTemplateFileName) ;

	SwOk = ai_poTemplate->InitFromFile( 
		path, m_pcTagDelimOpen, m_pcTagDelimClose, m_pcValueDelim ) ;
	if (!SwOk) goto exit ;

	//*****Busca matchings con glbfile

	SwOk = ai_poTemplate->GetMatching(&m_oPatterns) ;
	if (!SwOk) goto exit ;

	//*****Se baja todas referencias a glbfile

	pattern = M4_PATTERN(M4_GLB_FILE) ;
	pattern->GetMatchings(&matchings) ; 
	for ( it=matchings.begin() ; it != matchings.end(); ++it ) {
		if ( !  ThrowPublicFile( (*it) ) ) {
			SwOk =M4_FALSE ;
			goto exit ;
		} ;
	} ;

	//*****Carga valores globales, si los hay****

	pattern = M4_PATTERN(M4_BRK_INDEX) ;
	pattern->GetMatchings(&matchings) ; 
	it=matchings.begin() ;
	if (it != matchings.end() ) {
		m_TemplateValues[NAME_BRK_INDEX].StrCpy( (*it) ) ;
	} ;

	pattern = M4_PATTERN(M4_BRK_INDEX_NODE) ;
	pattern->GetMatchings(&matchings) ; 
	it=matchings.begin() ;
	if (it != matchings.end() ) {
		m_TemplateValues[NAME_BRK_INDEX_NODE].StrCpy( (*it) ) ;
	} ;

	pattern = M4_PATTERN(M4_BRK_INDEX_LEAF) ;
	pattern->GetMatchings(&matchings) ; 
	it=matchings.begin() ;
	if (it != matchings.end() ) {
		m_TemplateValues[NAME_BRK_INDEX_LEAF].StrCpy( (*it) ) ;
	} ;

	pattern = M4_PATTERN(M4_BRK_INDEX_CLOSE) ;
	pattern->GetMatchings(&matchings) ; 
	it=matchings.begin() ;
	if (it != matchings.end() ) {
		m_TemplateValues[NAME_BRK_INDEX_CLOSE].StrCpy( (*it) ) ;
	} ;
	
	pattern = M4_PATTERN(M4_ROOT_NAVIGATION) ;
	pattern->GetMatchings(&matchings) ; 
	it=matchings.begin() ;
	if (it != matchings.end() ) {
		m_TemplateValues[NAME_ROOT_NAVIGATION].StrCpy( (*it) ) ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::LoadTemplate() from template %0:s SwOk %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcTemplateFileName << SwOk << SEND_MSG   ;
	};
	#endif
	
	return SwOk ;
}

m4bool_t ClENG_DvcHTML::LinkToIndexTemplates() 
{
	m4bool_t SwOk=M4_TRUE ;
	BreakTemplates_t::iterator it ;

	it = m_oBreakTemplates.find( m_TemplateValues[NAME_BRK_INDEX] ) ;
	if ( it==m_oBreakTemplates.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163016,
			"Template file for index not specified", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	(*it).second.m_bSwCreateFile = M4_FALSE ;
	m_oTmplIndex.StrCpy( (*it).first ) ;  

	it = m_oBreakTemplates.find( m_TemplateValues[NAME_BRK_INDEX_NODE] ) ;
	if ( it==m_oBreakTemplates.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163017,
			"Template file for index node not specified", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	(*it).second.m_bSwCreateFile = M4_FALSE ;
	m_oTmplIndexNode.StrCpy( (*it).first ) ;    

	it = m_oBreakTemplates.find( m_TemplateValues[NAME_BRK_INDEX_LEAF] ) ;
	if ( it==m_oBreakTemplates.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163018,
			"Template file for index leaf not specified", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	(*it).second.m_bSwCreateFile = M4_FALSE ;
	m_oTmplIndexLeaf.StrCpy( (*it).first ) ;  
	
	it = m_oBreakTemplates.find( m_TemplateValues[NAME_BRK_INDEX_CLOSE] ) ;
	if ( it==m_oBreakTemplates.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163019,
			"Template file for index close not specified", ClMIT_Msg::MIXED_OUT)
				<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	(*it).second.m_bSwCreateFile = M4_FALSE ;
	m_oTmplIndexClose.StrCpy( (*it).first ) ;  
	
exit:

	return SwOk ;
}


ClENG_PhysicalFile * ClENG_DvcHTML::ThrowPublicFile(const m4char_t* ai_pcFileNameId)
{
	m4bool_t SwOk=M4_TRUE;

	ClENG_PhysicalFile * F ;
	m4bool_t swCreated ;

	//Obtiene la referencia

	F = m_oOutRepository.GetGlobalReference(ai_pcFileNameId, M4_TRUE, &swCreated) ;
	if (!F) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( !swCreated ) {
		
		//Si no lo ha creado porque ya existía, no hay nada más que hacer
		;
	} else {
		
		//Si se ha creado, está vacío. Copiar desde el original.

		ClMIT_Str path;

		//Obtengo en path el fichero original

		m_poInsp->m_poSysInfo->GetValue( ENG_RK_GRAPHICS_PATH_HTML, &path) ;
		ClMIT_SysInfo::AppendToPath(&path, ai_pcFileNameId );	

		//Y copio el fichero

		F->Open( ClMIT_File::READ_WRITE,ClMIT_Trace::PROPAGATE_ERROR);
		SwOk = F->WFile(path) ;
		F->Close() ;
		if (!SwOk) goto exit ;
	} ;
exit:

	return SwOk ? F : NULL ;
}


m4bool_t ClENG_DvcHTML::PreReset()
{
	m4bool_t SwOk=M4_TRUE;
	m4bool_t SwFilesOpenned ;

	//------->NewBreak puede o no abrir ficheros (segun ATOMIC_MODE)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak(&SwFilesOpenned) );	
	if (!SwOk) goto exit ;
	
	//Pueden cambiar si NewBreak cierra y abre ficheros
	ResetYOffsets() ;

	//------->Solo procesará ficheros si los acaba de crear
	
	if ( SwFilesOpenned ) {
		SwOk = UpdateNewFlushFiles() ;
		if (!SwOk) goto exit ;
	};

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}

m4bool_t ClENG_DvcHTML::UpdateNewFlushFiles() 
{
	m4int16_t Page;
	ClMIT_Str str, str2 ;
	m4bool_t SwOk=M4_TRUE ;
	//size_t Len;
	//M4ClBuffer::StCopy Param;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	ClMIT_ReplacePattern *pattern ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;


	//------>Proceso de patterns

	pattern = M4_PATTERN( M4_FIRST_FILE ) ;
	// Si hay un fichero de ruptura con expresión siempre se tiene que reevaluar
	if ( strlen( pattern->GetValue() )==0 || m_oOutRepository.HasBreakFileExpression() ) {
		F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;
		str.StrCpy( F->GetFileName() ) ;		
		m_oOutRepository.GetRelativePathFromFlushToGlobalToBreak(&str2) ;
		ClMIT_File::ConcatToPath(&str2,str) ;
		ClMIT_File::ConvertToWebPath(&str2) ;
		pattern->ChangeValue(str2) ;
	} ;

	//<-----
	
	//------>Datos que solo van una vez al principio del fichero (o ficheros)
	
	for(Page=0; Page<GetNumLayoutPages();++Page){
		
		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		if (!swRedirect) {

			//Template comienzo de fichero

			sprintf(pcBuff, "%s% - lu", m_oReportName.InChar(0), (unsigned long)GetNumPageDvc(Page) );
			pattern = M4_PATTERN( M4_TITLE ) ;
			pattern->ChangeValue(pcBuff) ;

			// UNICODE REPORTS
			// obtener el charset.
			pattern = M4_PATTERN(M4_CHARSET) ;
			pattern->ChangeValue(M4CharSet()) ;
			
			if ( ! F->WReplace( &m_oDataStart, &m_oPatterns ) ) {
				SwOk=M4_FALSE ;
			} ;
		} ;

		//Volcar informacion de Optimized Preload
		//Se hace en todas paginas, esten o no redirijidas

		F->DumpBuffer(&m_poLayoutPages[Page].m_oOpPrBuff) ;
		F->DumpBuffer(&m_poLayoutPages[Page].m_oPrBuff) ;
	};
	return SwOk ;
}

void ClENG_DvcHTML::PostReset()
{
	//------>Datos que solo van una vez al final del fichero (o ficheros)

	if(  m_oOutRepository.IsGoingToCloseFlushSet() ) {
	
		UpdateClosingFlushFiles(M4_TRUE) ;

		//Crear ficheros navegacion, solo necesitan la lista de flush files del break

		//Crear ficheros TOC
	};
	
	CreateTemplateBreakFiles() ;

	//---------->CloseBreak puede o no abrir ficheros (segun ATOMIC mode)

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}

void ClENG_DvcHTML::UpdateClosingFlushFiles(m4bool_t ai_bPostResetCalling) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t *pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4int16_t Page, NumLayoutPages;
	M4ClBuffer *PageBuffer ;
	m4bool_t SwOk=M4_TRUE ;

	ClMIT_Str strAux ;

	//Llamado desde flush, vuelca a buffer para que en la salida tengamos:
	//		<DatosBufferMetodosAction><DatosPostReset>
	//	ya que los <DatosBufferMetodosAction> aun no se han volcado.
	//En PostReset es al reves, escribimos directamente a file.


	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		if (swRedirect) continue ;

		//Template fin de fichero
		if ( m_oDataEnd.Replace( &strAux, &m_oPatterns ) ) {
			if ( ai_bPostResetCalling ) {
				F->WStr(strAux) ;
			} else {
				PageBuffer=m_poLayoutPages[Page].m_poActualBuff ;
				PageBuffer->AppendStr( strAux ) ;
			} ;
		} else {
			SwOk=M4_FALSE ;
		} ;
	};

} ;

m4bool_t ClENG_DvcHTML::CreateTemplateBreakFiles() 
{

	m4bool_t SwOk=M4_TRUE ;

	BreakTemplates_t::iterator tmplIt ; 
	ClMIT_CopyReplace *tmpl ;
	const ClMIT_Str *tmplKey ;
	ClENG_PhysicalFile *F ;
	ClMIT_ReplacePattern *pattern ;
	ClMIT_Str strAux, strAux2, strList ;
	m4bool_t swFirst, swAux ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::CreateTemplateBreakFiles()", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	//Si no hay templates, no perdemos el tiempo

	//NO poner en forma de exit, porque el compiler se sun genera codigo erroneo
	if ( m_oBreakTemplates.size()!=0) {
	
		//Actualizo pattern brkfile y brkfilelang

		m_oOutRepository.GetRelativeMaskFromGlobalToBreakToFlush(&strAux) ;
		ClMIT_File::ConvertToWebPath(&strAux) ;

		pattern = M4_PATTERN( M4_BRK_FILE ) ;
		pattern->ChangeValue(strAux) ;

		pattern = M4_PATTERN( M4_BRK_FILE_LANG ) ;
		pattern->ChangeValue(strAux) ;

		//Actualizo navigation array
	
		ClENG_BreakGroup::MainDataList_t::iterator mainIt, mainItEnd ;
	

		for (
			m_oOutRepository.GetMainDataIt(mainIt,mainItEnd), 
				swFirst=M4_TRUE, strList.StrCpy(""),
				m_oOutRepository.GetRelativePathFromFlushToGlobalToBreak(&strAux) ; 
			mainIt!=mainItEnd ; 
			++mainIt ) 
		{
			F = (*mainIt) ;
			strAux2.StrCpy(strAux) ;
			ClMIT_File::ConcatToPath(&strAux2, F->GetFileName() ) ;
			ClMIT_File::ConvertToWebPath(&strAux2) ;

			strAux2.StrInsertChar(0,1,'\"') ;
			strAux2.StrCat("\"") ;
			if (!swFirst) {
				strAux2.StrInsertChar(0,1,',') ;
			} else {
				swFirst=M4_FALSE ;
			} ;

			strList.StrCat(strAux2) ;
		} ;
	
		pattern = M4_PATTERN( M4_NAVIGATION_ARRAY ) ;
		pattern->ChangeValue(strList) ;



		//Creo todos ficheros excepto los que no tengan el flag

		for ( tmplIt=m_oBreakTemplates.begin() ; tmplIt!=m_oBreakTemplates.end(); ++tmplIt ) {

			if ( ! (*tmplIt).second.m_bSwCreateFile ) continue ;

			tmplKey		= &(*tmplIt).first ;
			tmpl		= (*tmplIt).second.m_poTemplate ;
	
			if ( ! tmpl->Replace( &strAux, &m_oPatterns ) ) {
				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Failed replacing template %0:s" ) 
						<< tmplKey->InChar(0) << DETAILED_MSG << SEND_MSG   ;
				};
				#endif
				SwOk=M4_FALSE ;
				continue ;
			} else {
				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Replacing template %0:s" ) 
						<< tmplKey->InChar(0) << DETAILED_MSG << SEND_MSG   ;
				};
				#endif
				;
			} ;
			
			//Creo fichero global a break y grabo el contenido
	
			F = m_oOutRepository.NewGlobalToBreakReference(tmplKey->InChar(0)) ;
			if (!F) {
				SwOk=M4_FALSE ;
				continue ;
			} ;

			if ( ! F->Open(ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ) {
				SwOk=M4_FALSE ;
				continue ;
			} ;
			F->WStr(strAux) ;
			F->Close() ;

		} ;

		//Y ahora creo indice si procede

		if ( IsTOC() ) {
			if ( ! PrintIndex() ) {
				SwOk=M4_FALSE ;
			} ;
		} ;

		//Finalmente, si hay marcado algun fichero como root naveg, lo marco
	
		if ( m_TemplateValues[NAME_ROOT_NAVIGATION].StrLen()>0 ) {
			F = m_oOutRepository.GetGlobalToBreakReference(m_TemplateValues[NAME_ROOT_NAVIGATION]) ;
			if ( !F ) {
				SwOk=M4_FALSE ;
			} else {;
				m_oOutRepository.SetAsMainFileForCurrentBreak(F) ;
			} ;
		} ;
	} ;


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_DvcHTML::CreateTemplateBreakFiles() SwOk %0:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif
	
	return SwOk ;
}



m4bool_t ClENG_DvcHTML::RefreshLayout()
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4int16_t Page;
	ClMIT_File *F;
	ClMIT_File GIF;
	ClMIT_Str PathAux, NameAux;

	m4bool_t SwFilesOpenned ;
	m4bool_t SwOk=M4_TRUE ;

	//---------->Notifica a FileRep, puiede o no abrir files (segun ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush(&SwFilesOpenned) ;
	if (!SwOk) goto exit;

	//Pueden cambiar si NewFlush cierra y abre ficheros
	ResetYOffsets() ;

	//------->Solo procesará ficheros si los acaba de crear
	
	if ( SwFilesOpenned ) {
		SwOk = UpdateNewFlushFiles() ;
		if (!SwOk) goto exit;
	
	};

	//---------->Propaga
exit:
	return SwOk && ClENG_OutDevice::RefreshLayout() ;
};

m4bool_t ClENG_DvcHTML::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//------>Datos que solo van una vez al final del fichero (o ficheros)

	if(  m_oOutRepository.IsGoingToCloseFlushSet() ) {
	
		UpdateClosingFlushFiles(M4_FALSE) ;

	};

	//---------->Vuelca buffers a files. Puede o no cerrar files (segun ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;
	
	//Resetea YOffsets, CloseFlush incrementa los datos
	ResetYOffsets() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}

m4bool_t ClENG_DvcHTML::PrintIndex() 
{
	m4uint32_t i ;
	ClMIT_Str strAux ;
	ClENG_PhysicalFile *F=NULL ;
	m4bool_t SwOk=M4_TRUE ;
	BreakTemplates_t::iterator it ;
	ClMIT_CopyReplace *templ ;
	IndexNodeList_t nodeList ;
	IndexNodeList_t::iterator nodeIt ;

	//Crea fichero con parte inicial

	it = m_oBreakTemplates.find(m_oTmplIndex) ;
	templ = (*it).second.m_poTemplate ;
	if ( ! templ->Replace( &strAux, &m_oPatterns ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	F = m_oOutRepository.NewGlobalToBreakReference( (*it).first ) ;
	if (!F) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! F->Open(ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	F->WStr(strAux) ;

	//Si los seleccionados en m_oBreakIndexRoots no son raices, añadir camino hasta el nivel
	//	al que corresponden.

	if ( m_oBreakIndexRoots.size()>0 ) {
		m_oBreakIndexRoots[0]->GetPathToThis( &nodeList ) ;
		for ( nodeIt=nodeList.begin() ; nodeIt!=nodeList.end(); ++nodeIt ) {
			PrintIndexEntry( F, (*nodeIt).second, M4_FALSE ) ;
		} ;
	} ;
	
	//Imprime el resto de informacion

	for ( i=0; i< m_oBreakIndexRoots.size(); ++i ) {
		PrintIndexEntry( F, m_oBreakIndexRoots[i], M4_TRUE ) ;
	} ;

	//Escribe informacion de cierre

	templ = (*m_oBreakTemplates.find(m_oTmplIndexClose)).second.m_poTemplate ;
	if ( ! templ->Replace( &strAux, &m_oPatterns ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	F->WStr(strAux) ;

	//Cierra fichero
	F->Close() ;
	F=NULL ;

exit:

	if (F) F->Close() ;
	return SwOk ;
} 

m4bool_t ClENG_DvcHTML::PrintIndexEntry(
	ClENG_PhysicalFile *ai_F, ClENG_IndexNode* ai_poIndexNode, m4bool_t ai_bSwRecursive) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str strAux ;
	IndexNodeList_t::iterator it ;
	ClMIT_ReplacePattern *pattern ;
	ClMIT_CopyReplace *templ ;

	strAux.Accommodate(50) ;	//Para numeros

	//Actualizo los valores de pattern referentes a la entrada

	sprintf( strAux.InChar(0), "%lu", (unsigned long)ai_poIndexNode->GetIndexLevel() ) ;
	strAux.OverStringValid() ;
	pattern = M4_PATTERN(M4_BRK_INDEX_LEVEL) ;
	pattern->ChangeValue(strAux) ;

	sprintf( strAux.InChar(0), "%lu", (unsigned long)ai_poIndexNode->GetFather()->GetIndexLevel() ) ;
	strAux.OverStringValid() ;
	pattern = M4_PATTERN(M4_BRK_INDEX_LEVEL_FATHER) ;
	pattern->ChangeValue(strAux) ;

	pattern = M4_PATTERN(M4_BRK_INDEX_STRING) ;
	pattern->ChangeValue( ai_poIndexNode->GetIndexString() ) ;

	sprintf( strAux.InChar(0), "%lu", (unsigned long)ai_poIndexNode->GetNumPage() ) ;
	strAux.OverStringValid() ;
	pattern = M4_PATTERN(M4_BRK_INDEX_PAGE) ;
	pattern->ChangeValue(strAux) ;

	pattern = M4_PATTERN(M4_BRK_INDEX_FILE) ;
	pattern->ChangeValue( ai_poIndexNode->GetAuxValue() ) ;

	sprintf( strAux.InChar(0), "%lu", (unsigned long)ai_poIndexNode->GetTargetID() ) ;
	strAux.OverStringValid() ;
	pattern = M4_PATTERN(M4_BRK_INDEX_TARGET) ;
	pattern->ChangeValue(strAux) ;

	
	//Escribe en fichero la entrada

	if ( ai_poIndexNode->IsLeaveNode() ) {
		templ = (*m_oBreakTemplates.find(m_oTmplIndexLeaf)).second.m_poTemplate ;
	} else {
		templ = (*m_oBreakTemplates.find(m_oTmplIndexNode)).second.m_poTemplate ;
	} ;
	if  ( ! templ->Replace(&strAux, &m_oPatterns) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	ai_F->WStr(strAux) ;

	//Si es recursivo proceso hijos

	if (ai_bSwRecursive) {
		for ( it=ai_poIndexNode->GetNodeList()->begin() ; 
			it!=ai_poIndexNode->GetNodeList()->end(); ++it ) 
		{
			if ( ! PrintIndexEntry( ai_F, (*it).second, M4_TRUE ) ) {
				SwOk=M4_FALSE ;
				continue ;
			} ;
		} ;
	} ;
exit:
	return SwOk ;
}

void ClENG_DvcHTML::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
}


void ClENG_DvcHTML::SetUpOutputProperties(m4int16_t	ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, DvcUnit_t ai_iWidth, m4pcchar_t ai_pcStr)
{
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4uint32_t uiIdColour;
	ClFONT_M_Font *Font;
	m4uint32_t iChar = 0;	
	m4uint32_t uiSpacesNum = 0;
	m4uint32_t uiLeft = 0;
	m4char_t cCharacter = '\0';
	DvcUnit_t iSpacesWidth = 0;

	if( GetNavigationMode()==NAVIGATION_OFF || !IsZoom()) {
		sprintf(pcBuff, HTML_DIV_STYLE, (int)ClMetric::DvcToPoint(ai_iWidth) + (int)(ClMetric::DvcToPoint(ai_iWidth)*5)/10 + BAR_CODE_OFFSET);
	} else {
		sprintf(pcBuff, HTML_DIV_STYLE_Z, (int)ClMetric::DvcToPoint(ai_iWidth) + (int)(ClMetric::DvcToPoint(ai_iWidth)*5)/10+ BAR_CODE_OFFSET);
	};

	//Selecciona la fuente
	Font = GetLastFont(ai_iPage);

	if(Font) {
		sprintf(&pcBuff[strlen(pcBuff)], HTML_FONT_FAMILY, Font->RVW_GetFaceName()); //PUENTEADO
		
		if(GetNavigationMode()==NAVIGATION_OFF || !IsZoom()) {
			sprintf(&pcBuff[strlen(pcBuff)], HTML_FONT_SIZE, (int)Font->RVW_GetHeight()); //PUENTEADO
		} else {
			sprintf(&pcBuff[strlen(pcBuff)], HTML_FONT_SIZE_Z, (int)Font->RVW_GetHeight()); //PUENTEADO
		};

		//Propiedades de la fuente
		if (Font->IsUnderline()) {
			strcat(pcBuff, HTML_FONT_UNDERLINE);
		};

		if (Font->IsBold()) {
			strcat(pcBuff, HTML_FONT_BOLD);
		};

		if (Font->IsItalic()) {
			strcat(pcBuff, HTML_FONT_ITALIC);
		};

		//Selecciona el color
		uiIdColour = GetLastColour(ai_iPage) ;

		sprintf(&pcBuff[strlen(pcBuff)], HTML_COLOR, (long)m_oFGPalette.GetColourById(uiIdColour)->GetR(), 
				(long)m_oFGPalette.GetColourById(uiIdColour)->GetG(), (long)m_oFGPalette.GetColourById(uiIdColour)->GetB());
		ClMIT_Str::OverStringValid(pcBuff, StENG_TaskEnv::DvcHTML_MAX_SIZE);
	};

	// Bug 0142780. Justificar adecuandamente el campo. Los espacios en blanco son irrelevantes
	// para el navegador. Se debe incrementear la propiedad left para obtener el efecto
	// de tener espacios delante de la cadena que se va a imprimir.
	if (ai_pcStr != NULL)
	{
		for (iChar = 0; iChar < strlen(ai_pcStr) - 1; iChar++)
		{
			cCharacter = ai_pcStr[iChar];
			if (cCharacter == ' ')
			{
				uiSpacesNum++;
			}
			else
			{
				break;
			}
		}
	}

	// Calcular el incremento en pts que se debe sumar a la propiedad left.
	uiLeft = (int)ClMetric::DvcToPoint(ai_iX);
	if (uiSpacesNum != 0)
	{
		iSpacesWidth = Font->GetDvcLenStr(uiSpacesNum);
		uiLeft += (int)ClMetric::DvcToPoint(iSpacesWidth);
	}
	
	//Selecciona posicion
	if(GetNavigationMode()==NAVIGATION_OFF || !IsZoom()) {
		sprintf(&pcBuff[strlen(pcBuff)], HTML_LEFT, uiLeft);
		sprintf(&pcBuff[strlen(pcBuff)], HTML_TOP, (int)ClMetric::DvcToPoint(ai_iY));
		strcat(pcBuff, HTML_DIV_CLOSE);
	} else {
		sprintf(&pcBuff[strlen(pcBuff)], HTML_LEFT_Z, uiLeft);
		sprintf(&pcBuff[strlen(pcBuff)], HTML_TOP_Z, (int)ClMetric::DvcToPoint(ai_iY));
		strcat(pcBuff, HTML_DIV_CLOSE_Z);
	};
	
	PageBuffer->AppendStr(pcBuff);
};

void ClENG_DvcHTML::PrintNChar(m4int16_t	ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
						  m4pchar_t ai_pcStr, size_t ai_szNChar, 
						  m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink,
						  m4uint16_t ai_iTargetID, ClENG_OutDevice::StPIParams *ai_pstPI)
{
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff = TaskEnv->Dvc_g_pcBuff;

	m4uint32_t lValueOffset = 0;
	DvcUnit_t Width;
	ClFONT_M_Font *Font;
	size_t Length = 0;
	ClMIT_Str strConvert ;

	
	Font = GetLastFont(ai_iPage);
	Width = Font->GetDvcLenStr(strlen(ai_pcStr));

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_iX += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_iY += GetOffsetY(ai_iPage)+m_poYOffset[ai_iPage] ;

	//Lanzamos las propiedades del objeto salida
	SetUpOutputProperties(ai_iPage, ai_iX, ai_iY, Width, ai_pcStr);

	//Testeamos hyperlink e imprimimos la cadena
	ClMIT_SysInfo::EscapeToHTML(ai_pcStr, ai_szNChar, &strConvert) ;
	if(strlen(ai_pcHyperlink)){
		sprintf(m_pcBuff, HTML_LINK_BLANK, ai_pcHyperlink);
		strcat(m_pcBuff, strConvert.InChar(0) ); //Ya es ASCIZ
		strcat(m_pcBuff, HTML_LINK_END);
	} else {
		strcpy(m_pcBuff, strConvert.InChar(0)); //Ya es ASCIZ
	};

	//Testeamos indexacion y lanzamos ancla
	if(ai_iTargetID!=ClENG_OutDevice::DVC_NO_INDEX_TARGET && IsTOC()) {
		ChangeAuxIndexValue(ai_iTargetID);
		sprintf(&m_pcBuff[strlen(m_pcBuff)], HTML_INDEX_ENTRY, ai_iTargetID); 
	};

	strcat(m_pcBuff, HTML_LINE_JUMP HTML_DIV_END);
	Length=strlen(m_pcBuff);

	//No utilizamos strlen() por que la cadena puede tener codigos especiales, que incluyan el 0
	if((Length+1)>StENG_TaskEnv::DvcPSC_MAX_SIZE){
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163002, 
			"Buffer has been overwritten", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError();
	};

	//Calculamos el Offset, posicion del buffer antes de que escriba
	lValueOffset = PageBuffer->GetTotal() + Length - HTML_PI_OFFSET - ai_szNChar;

	PageBuffer->Append(m_pcBuff,Length);

	if(ai_pstPI){
		ai_pstPI->m_uiOff=lValueOffset;
		ai_pstPI->m_szNChars=ai_szNChar;
	};
}


void ClENG_DvcHTML::PrintStr(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, m4pchar_t ai_pcStr, 
							 m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	DvcUnit_t Width;

	ClFONT_M_Font *Font;
	size_t Length;
	ClMIT_Str strConvert ;

	Font = GetLastFont(ai_iPage);
	Width = Font->GetDvcLenStr(strlen(ai_pcStr));

    //Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_iX += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_iY += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;

	//Lanzamos las propiedades del objeto salida
	SetUpOutputProperties(ai_iPage, ai_iX, ai_iY, Width, ai_pcStr);

	//Testeamos hyperlink e imprimimos la cadena
	ClMIT_SysInfo::EscapeToHTML(ai_pcStr, &strConvert) ;
	if(strlen(ai_pcHyperlink)){
		sprintf(m_pcBuff, HTML_LINK_BLANK, ai_pcHyperlink);
		Length=sprintf(&m_pcBuff[strlen(m_pcBuff)], strConvert.InChar(0));
		strcat(m_pcBuff, HTML_LINK_END);
	} else {
		Length=sprintf(m_pcBuff, strConvert.InChar(0));
	};

	//Testeamos indexacion y lanzamos ancla
	if(ai_iTargetID!=ClENG_OutDevice::DVC_NO_INDEX_TARGET && IsTOC()) {
		ChangeAuxIndexValue(ai_iTargetID);
		sprintf(&m_pcBuff[strlen(m_pcBuff)], HTML_INDEX_ENTRY, ai_iTargetID); 
	};

	strcat(m_pcBuff, HTML_LINE_JUMP HTML_DIV_END);
	Length=strlen(m_pcBuff) ;

	if((Length+1)>StENG_TaskEnv::DvcPSC_MAX_SIZE){
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163002, 
			"Buffer has been overwritten", ClMIT_Msg::MIXED_OUT) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError();
	};

	PageBuffer->Append(m_pcBuff,Length);
}

void ClENG_DvcHTML::PrintNCharPi(ClMIT_Str *ai_poIdPi, m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY,
								 size_t ai_szNChar, m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams;
	ClMIT_Str StrAux;

	//El proceso es el mismo que en PCL
	StrAux.StrDuplicateChar(0, ai_szNChar,'X');

	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage, ai_iX, ai_iY, StrAux, ai_szNChar, ai_bSwRawText, ai_pcHyperlink, ai_iTargetID, &PIParams);

	//Añadimos PI a la lista
	if(PIParams.m_szNChars){
		m_oOutRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, 
			PIParams.m_uiOff, PIParams.m_szNChars ) ;
	};
}


void ClENG_DvcHTML::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;
	
	// Tomo el Pi a imprimir ( le digo que imprima el error si no lo encuentra)
	if ( m_oOutRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ) )
	{
		if (strlen(ai_pcStr) > piData.m_NBytes) {
			// Sacar mensaje de que se va a truncar
		   *m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163004, 
			   "Strings overlapped, pre-printing value too long: Page %0:s, Only %1:s bytes reserved for the string %2:s", ClMIT_Msg::MIXED_OUT) 
				<< (m4int16_t)piData.m_NPage << (m4uint32_t)piData.m_NBytes 
				<< ai_pcStr << SEND_MSG ;
		   //Efecto Local
		} ;
			
		// Creo la cadena a imprimir
		// -- si sobra espacion relleno con blancos hasta PiNBytes 
		// -- si falta se trunca
		StrAux.Accommodate(piData.m_NBytes+1) ;
 		Len = sprintf(StrAux.InChar(0),"%-*.*s", piData.m_NBytes, piData.m_NBytes, ai_pcStr);
		StrAux.OverStringValid() ;	//No haria falta, se pone por si acaso

	    // Imprimo la cadena asociada en RAM o en fichero, según dónde de encuentre el Pi
		if ( piData.m_Place == StPi_t::PI_IN_RAM) {  
			// Imprimir en el buffer
			M4ClBuffer::StCopy Param = { StrAux.InChar(0), piData.m_NBytes };	 
			
			m_poLayoutPages[ piData.m_NPage ].m_poActualBuff->Flush(
				M4ClBuffer::MemToBuffManip, &Param, piData.m_Offset, piData.m_NBytes );

		} else  { 
			
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
			piData.m_poFile->W(StrAux.InChar(0), piData.m_NBytes );
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed){
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			};
		} ;

  }

  return;
}


void ClENG_DvcHTML::EOP(m4int16_t ai_iPage)
{
	
	//Separador de paginas solo si salen varias paginas HTML a un solo fichero HTML
	if ( m_oOutRepository.GetDuplexMode() != ClENG_BreakList::ATOMIC_DUPLEX) {
		
		StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
		M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
		m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcHTML_MAX_SIZE) ;
		
		DvcUnit_t topAbs, leftAbs, widthAbs ;

		//Se le suma GetSizeY porque queremos el final de la pag actual, no el comienzo
		//HTML_PAGE_SEPARATOR_WIDTH*n para que haya margen, mejor que la linea coincida 
		//	con finales de pag (que casi nunca se imprime hasta ahi) que con comienzos.
		topAbs = m_poYOffset[ai_iPage] + GetSizeY(ai_iPage) -
			HTML_PAGE_SEPARATOR_WIDTH * 6 ;
		widthAbs = GetSizeX(ai_iPage) ;
		leftAbs  = (GetSizeX(ai_iPage)-widthAbs)%2   ;

		sprintf ( pcBuff, HTML_PAGE_SEPARATOR,
			(unsigned long)ClMetric::DvcToPoint( widthAbs ),					
			(unsigned long)ClMetric::DvcToPoint( leftAbs ),					 
			(unsigned long)ClMetric::DvcToPoint( topAbs ),  					
			(unsigned long)ClMetric::DvcToPoint(HTML_PAGE_SEPARATOR_WIDTH) ) ;
		PageBuffer->AppendStr( pcBuff ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcHTML_MAX_SIZE) ;
	} ;

	ClENG_OutDevice::EOP(ai_iPage);
}

//-------------------------- Calculos ------------------------------------

void ClENG_DvcHTML::ChangeAuxIndexValue(m4uint32_t ai_uiTargetID)
{
	ClENG_IndexNode *node ;
	ClMIT_Str str;
	ClENG_PhysicalFile *F ;

	node = m_oIndexRootNode.GetNode(ai_uiTargetID, ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!node) return ;

	F = m_oOutRepository.GetFlushFile(0) ;
	if (!F) return ;

	m_oOutRepository.GetRelativePathFromFlushToGlobalToBreak(&str) ;
	ClMIT_File::ConcatToPath( &str, F->GetFileName() ) ;
	ClMIT_File::ConvertToWebPath( &str ) ;

	node->SetAuxValue(str) ;
};



//---------------------- LINEAS, FONDOS y BORDES -----------------------

void ClENG_DvcHTML::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	DvcUnit_t Size, Start;
	m4uint32_t uiIdColour;
	ClENG_Colour *Colour ;
	m4int16_t PIndex;

	//Selecciona el color
	uiIdColour = GetLastColour(ai_iPage) ;
	PIndex = m_oFGPalette.GetIndex(uiIdColour);
	Colour = m_oFGPalette.GetColourByIndex(PIndex);

	//Normalizamos a 1 pixel
	if (ai_LineWidth>0 && ai_LineWidth<254) {
		ai_LineWidth=255;
	};

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X1 += GetOffsetX(ai_iPage);
	ai_X2 += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_Y1 += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;
	ai_Y2 += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;

	if(ai_X1==ai_X2){
		//Linea Vertical
		Start = ai_Y2>ai_Y1? ai_Y1 : ai_Y2;
		Size = ai_Y2>ai_Y1? ai_Y2-ai_Y1 : ai_Y1-ai_Y2;
		if(GetNavigationMode()==NAVIGATION_OFF) 
		{
			sprintf(pcBuff, HTML_VERT_LINE, (int)ClMetric::DvcToPoint(ai_X1), (int)ClMetric::DvcToPoint(Start),
			    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
				ClMetric::DvcToPoint(ai_LineWidth), (int)ClMetric::DvcToPoint(Size));
		}
		else
		{
			sprintf(pcBuff, HTML_VERT_LINE_Z, (int)ClMetric::DvcToPoint(ai_X1), (int)ClMetric::DvcToPoint(Start),
			    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
				ClMetric::DvcToPoint(ai_LineWidth), (int)(ClMetric::DvcToPoint(Size)*OFFSET_GRAPH));
		};
		PageBuffer->AppendStr(pcBuff);
    } else if (ai_Y1==ai_Y2){
		//Linea horizontal
		Start = ai_X2>ai_X1? ai_X1 : ai_X2;
		Size= ai_X2>ai_X1? ai_X2-ai_X1 : ai_X1-ai_X2;
		if(GetNavigationMode()==NAVIGATION_OFF) 
		{
			sprintf(pcBuff, HTML_HORI_LINE, (int)ClMetric::DvcToPoint(Start), (int)ClMetric::DvcToPoint(ai_Y1),
			    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
				ClMetric::DvcToPoint(ai_LineWidth), (int)ClMetric::DvcToPoint(Size), (int)ClMetric::DvcToPoint(Size));
		}
		else
		{
			sprintf(pcBuff, HTML_HORI_LINE_Z, (int)ClMetric::DvcToPoint(Start), (int)ClMetric::DvcToPoint(ai_Y1),
			    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
				ClMetric::DvcToPoint(ai_LineWidth), (int)ClMetric::DvcToPoint(Size), (int)ClMetric::DvcToPoint(Size));
		};
		PageBuffer->AppendStr(pcBuff);
	};
}

void ClENG_DvcHTML::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	DvcUnit_t Size, Start;
	m4uint32_t uiIdColour;
	ClENG_Colour *Colour ;
	m4int16_t PIndex;

	//Selecciona el color
	uiIdColour = GetLastColour(ai_iPage) ;
	PIndex = m_oFGPalette.GetIndex(uiIdColour);
	Colour = m_oFGPalette.GetColourByIndex(PIndex);

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_Y += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;

	//Caja
	if(GetNavigationMode()==NAVIGATION_OFF) {
		sprintf(pcBuff, HTML_FILL, (int)ClMetric::DvcToPoint(ai_Width), (int)ClMetric::DvcToPoint(ai_Height), 
		    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
		    (int)ClMetric::DvcToPoint(ai_X), (int)ClMetric::DvcToPoint(ai_Y));
	} else {
		sprintf(pcBuff, HTML_FILL_Z, (int)ClMetric::DvcToPoint(ai_Width), (int)ClMetric::DvcToPoint(ai_Height), 
		    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(), 
		    (int)ClMetric::DvcToPoint(ai_X), (int)ClMetric::DvcToPoint(ai_Y));
	};

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcHTML_MAX_SIZE) ;
	PageBuffer->AppendStr(pcBuff);
}	

void ClENG_DvcHTML::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	DvcUnit_t Size, Start;
	m4uint32_t uiIdColour;
	ClENG_Colour *Colour ;
	m4int16_t PIndex;

	//Normalizamos a 1 pixel
	if (ai_LineWidth>0 && ai_LineWidth<254) {
		ai_LineWidth=255;
	};

	//Selecciona el color
	uiIdColour = GetLastColour(ai_iPage) ;
	PIndex = m_oFGPalette.GetIndex(uiIdColour);
	Colour = m_oFGPalette.GetColourByIndex(PIndex);

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_Y += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;

	//Caja
	if(GetNavigationMode()==NAVIGATION_OFF) {
		sprintf(pcBuff, HTML_BOX, (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
		    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (int)ClMetric::DvcToPoint(ai_Width), (int)ClMetric::DvcToPoint(ai_Height), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_X), (int)ClMetric::DvcToPoint(ai_Y));
	} else {
		sprintf(pcBuff, HTML_BOX_Z, (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
		    (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_LineWidth), (int)ClMetric::DvcToPoint(ai_Width), (int)ClMetric::DvcToPoint(ai_Height), 
			(long)Colour->GetR(), (long)Colour->GetG(), (long)Colour->GetB(),
			(int)ClMetric::DvcToPoint(ai_X), (int)ClMetric::DvcToPoint(ai_Y));
	};

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcHTML_MAX_SIZE) ;
	PageBuffer->AppendStr(pcBuff);
}



//--------------------- Graficos ---------------------------------------


void ClENG_DvcHTML::SelectColourDevice(m4int16_t ai_iPage, m4uint32_t ai_uiIdColour)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
}


size_t ClENG_DvcHTML::SetActivePalette (ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize)
{
	m4int16_t PIndex;
	ClENG_Colour* pColour;
	size_t Written;

	Written=ai_szFrom;

	return Written;
}

void ClENG_DvcHTML::PrintRasterFile(
		m4int16_t ai_iPage,
		::DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID, 	m4bool_t ai_bIsTempWebPath )
{
	M4ClBuffer* PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	ClENG_RasterMap::FILE_FORMATS fileFormat ;
	ClMIT_Str pathAux, fileNameAux, extAux, glbName, strAux, url ;
	ClMIT_ReplacePattern *pattern ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4bool_t SwOk=M4_TRUE ;

	ClENG_PhysicalFile *F ;
	m4bool_t swCreated ;

	//*********Calculos

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X += GetOffsetX(ai_iPage);
	
	//Se añade el Offset del limite fisico de la pagina por arriba
	ai_Y += GetOffsetY(ai_iPage) + m_poYOffset[ai_iPage] ;

	//*********>>>>>>>Bajadas y conversiones de graficos segun ai_bIsTempWebPath

	if(!ai_bIsTempWebPath){
		
		//*********GIF NO CORPORATIVO, reside en OutRepository: convertir, copiar, notificar.
		//	jpg, gif -> se bajan sin convertir, persisten como jpg o gif
		//	bmp		 -> se baja y convierte a gif, persiste como gif

		//---Primero formo nombre a nivel de repository para ver si existe

		fileFormat = ClENG_RasterMap::GetFileFormat(ai_pcPath) ;
		ClMIT_File::SplitPath(ai_pcPath, &pathAux, &fileNameAux, &extAux);
		glbName.StrCpy(fileNameAux) ;
		if ( fileFormat==ClENG_RasterMap::BMP ) {
			ClMIT_File::ConcatFileExt(&glbName, 
				ClENG_RasterMap::GetExtension(ClENG_RasterMap::GIF) ) ;
		} else if (fileFormat==ClENG_RasterMap::UNKNOWN) {
			SwOk=M4_FALSE ;
			goto exit ;
		} else {
			ClMIT_File::ConcatFileExt(&glbName,extAux) ;
		} ;

		//---Ahora lo notifico. Puede que ya estuviese o no.

		F = m_oOutRepository.GetGlobalReference(glbName, M4_FALSE, &swCreated) ;
		if (!F) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//---Y solo si no existe, lo bajo, ya sea convirtiendolo o copiandolo

		if (swCreated) {

			if (fileFormat==ClENG_RasterMap::BMP) {

				//Es BMP, hay que convertir

				ClENG_RasterMap map ;

				SwOk = map.LoadFromFile(ai_pcPath);
				if (!SwOk) goto exit ;

				F->GetAbsolutePathFile(&strAux) ;
				SwOk = map.SaveAsGIF(strAux) ;
				if (!SwOk) goto exit ;
			
			} else {

				//Es GIF o JPG, simplemente copiarlo

				F->Open( ClMIT_File::READ_WRITE,ClMIT_Trace::PROPAGATE_ERROR);
				F->WFile(ai_pcPath) ;
				F->Close() ;

			} ;
		} ;

		//---Ahora obtengo la URL para el HTML en strAux

		pattern = M4_PATTERN( M4_GLB_FILE ) ;
		url.StrCpy( pattern->GetValue() ) ;	//GLB FILE ya es URL acabada en / ...
		url.StrCat( glbName ) ;				// .. por eso basta con concatenar

	} else {
		
		//*********GIF CORPORATIVO, NO reside en OutRepository, mero link, NO notificar.

		//Hayar la URL relativa al registro

		ClMIT_File::SplitPath(ai_pcPath, &pathAux, &fileNameAux);
		m_poInsp->m_poSysInfo->GetValue(ENG_RK_TEMP_WEB_IMAGES, &url);
		ClMIT_File::ConcatToPath(&url,fileNameAux);
		ClMIT_File::ConvertToWebPath(&url) ;

	};
	
	//*********Tratamiento comun a todos 

	//Lanzamos las propiedades del objeto salida
	SetUpOutputProperties(ai_iPage, ai_X, ai_Y, ai_SizeX, NULL);
		
	//Testeamos hyperlink
	if(strlen(ai_pcHyperlink)){
		sprintf(m_pcBuff, HTML_LINK_BLANK, ai_pcHyperlink);
		PageBuffer->AppendStr(m_pcBuff);
	};

	if(!IsZoom() || m_NavigationMode==NAVIGATION_OFF){
		PageBuffer->AppendStr(HTML_IMAGE_START);
	} else {
		PageBuffer->AppendStr(HTML_IMAGE_START_Z);
	};
	PageBuffer->AppendStr( url );

	if(GetNavigationMode()==NAVIGATION_OFF || !IsZoom()) {
		sprintf(m_pcBuff, HTML_IMAGE_WIDTH, (int)(ClMetric::DvcToPoint(ai_SizeX)*OFFSET_GRAPH));
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, HTML_IMAGE_HEIGHT, (int)(ClMetric::DvcToPoint(ai_SizeY)*OFFSET_GRAPH));
		PageBuffer->AppendStr(m_pcBuff);
	} else {
		sprintf(m_pcBuff, HTML_IMAGE_WIDTH_Z, (int)(ClMetric::DvcToPoint(ai_SizeX)*OFFSET_GRAPH));
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, HTML_IMAGE_HEIGHT_Z, (int)(ClMetric::DvcToPoint(ai_SizeY)*OFFSET_GRAPH));
		PageBuffer->AppendStr(m_pcBuff);
	};

	if(!IsZoom() || m_NavigationMode==NAVIGATION_OFF){
		PageBuffer->AppendStr(HTML_IMAGE_END);
	} else {
		PageBuffer->AppendStr(HTML_IMAGE_END_Z);
	};
	//Testeamos hyperlink
	if(strlen(ai_pcHyperlink)){
		PageBuffer->AppendStr(HTML_LINK_END);
	};

	//Testeamos indice y lanzamos ancla
	if(ai_iTargetID!=ClENG_OutDevice::DVC_NO_INDEX_TARGET && IsTOC() ) {
		ChangeAuxIndexValue(ai_iTargetID);
		sprintf(m_pcBuff, HTML_INDEX_ENTRY, ai_iTargetID); 
		PageBuffer->AppendStr(m_pcBuff);
	};

	PageBuffer->AppendStr(HTML_DIV_END);	
exit:
	;
}
