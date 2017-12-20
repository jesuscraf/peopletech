//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#ifndef	_M4MODKEY_XMLTAGLABEL_HPP_
#define _M4MODKEY_XMLTAGLABEL_HPP_

#include "m4stl.hpp"
#include "globdec.hpp"

const string XMLTAGLABEL_DOUBLE_QUOTE																("\"");

const string XMLTAGLABEL_FILE_COPYRIGHT ("<!--\nCopyright 1991-2009 by Meta4, Inc.\nCentro Europa Empresarial - Edf. Roma\nC/ Rozabella, 8\n28230 Las Rozas - Madrid\nSpain\n\nPrivate and Confidential\nThe information contained in this document is the property of Meta4.\nIt is for the exclusive use of designated employees\nand not for distribution without prior written authorization.\n-->\n\n");

//-----------------------------------------------------------------------------------
//Tags de modulo-objeto.
const m4int16_t	XMLTAGLABEL_OBJMOD_ROOT_INDEX																= 0;
const string XMLTAGLABEL_OBJMOD_ROOT																("modkey");
const string XMLTAGLABEL_OBJMOD_ATT_FILEVERSION														("fileversion");

const m4int16_t	XMLTAGLABEL_OBJMOD_OBJECTSMODULES_INDEX														= 1;
const string XMLTAGLABEL_OBJMOD_OBJECTSMODULES														("objectsmodules");

const m4int16_t	XMLTAGLABEL_OBJMOD_OBJECTMODULE_INDEX														= 2;
const string XMLTAGLABEL_OBJMOD_OBJECTMODULE														("objectmodule");
const string XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_ID													("id");
const string XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC												("desc");
const string XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE									("");

const m4int16_t	XMLTAGLABEL_OBJMOD_OBJECT_INDEX																= 3;
const string XMLTAGLABEL_OBJMOD_OBJECT																("object");
const string XMLTAGLABEL_OBJMOD_OBJECT_ATT_NS														("ns");
const string XMLTAGLABEL_OBJMOD_OBJECT_ATT_ITEM														("item");
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//Tags de Licencia.

const m4int16_t	XMLTAGLABEL_LICENSE_ROOT_INDEX																= 4;
const string XMLTAGLABEL_LICENSE_ROOT																("modkey");
const string XMLTAGLABEL_LICENSE_ATT_FILEVERSION													("fileversion");

const m4int16_t XMLTAGLABEL_LICENSE_LICENSE_INDEX															= 5;
const string XMLTAGLABEL_LICENSE_LICENSE															("license");
const string XMLTAGLABEL_LICENSE_LICENSE_ATT_LICENSEVERSION											("licenseversion");
const string XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION												("expiration");
const string XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION_DEFAULT_VALUE								("");

const m4int16_t XMLTAGLABEL_LICENSE_GENERATIONINFO_INDEX													= 6;
const string XMLTAGLABEL_LICENSE_GENERATIONINFO														("generationinfo");
const string XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_IDLICENSE										("idlicense");
const string XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONVERSION								("generationversion");
const string XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONDATE									("generationdate");
const string XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_HOSTNAME										("hostname");

const m4int16_t XMLTAGLABEL_LICENSE_CUSTOMERINFO_INDEX														= 7;
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO														("customerinfo");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_ID												("id");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_NAME												("name");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_COUNTRY											("country");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_GEODIV											("geodiv");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC												("desc");
const string XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC_DEFAULT_VALUE								("");

const m4int16_t XMLTAGLABEL_LICENSE_NPS_INDEX																= 8;
const string XMLTAGLABEL_LICENSE_NPS																("nps");

const m4int16_t XMLTAGLABEL_LICENSE_NP_INDEX																= 9;
const string XMLTAGLABEL_LICENSE_NP																	("np");

const m4int16_t XMLTAGLABEL_LICENSE_SELECTS_INDEX															= 10;
const string XMLTAGLABEL_LICENSE_SELECTS															("selects");

const m4int16_t XMLTAGLABEL_LICENSE_SELECT_INDEX															= 11;
const string XMLTAGLABEL_LICENSE_SELECT																("select");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_DESC													("desc");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_EXEC													("exec");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED												("authorized");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED_DEFAULT_VALUE								("1000000000");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED									("effectiveauthorized");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED_DEFAULT_VALUE						("1000000000");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT													("limit");
const string XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT_DEFAULT_VALUE										(XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED_DEFAULT_VALUE);

const m4int16_t XMLTAGLABEL_LICENSE_SECURIZED_INDEX															= 12;
const string XMLTAGLABEL_LICENSE_SECURIZED															("securized");

const m4int16_t XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_INDEX												= 13;
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED												("securizednotallowed");

const m4int16_t XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_INDEX								= 14;
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE									("objectmodule");
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_ID							("id");
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC						("desc");
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE			("");

const m4int16_t	XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_INDEX							= 15;
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT							("object");
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS					("ns");
const string XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM					("item");

const m4int16_t XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_INDEX													= 16;
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED													("securizedallowed");

const m4int16_t XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_INDEX									= 17;
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE										("objectmodule");
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_ID								("id");
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC							("desc");
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE				("");

const m4int16_t	XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_INDEX							= 18;
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT							("object");
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS					("ns");
const string XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM					("item");

//-----------------------------------------------------------------------------------

//Vector para obtener el nombre traducido.
const string XMLTAGLABEL_OBJMOD_OBJECT_GET_LABEL[] = 
		{
		//Modulo-Objeto. [0..3]
		XMLTAGLABEL_OBJMOD_ROOT,
			XMLTAGLABEL_OBJMOD_OBJECTSMODULES,
				XMLTAGLABEL_OBJMOD_OBJECTMODULE,
					XMLTAGLABEL_OBJMOD_OBJECT,

		//Licencia. [4..16]
		XMLTAGLABEL_LICENSE_ROOT,
			XMLTAGLABEL_LICENSE_LICENSE,
				XMLTAGLABEL_LICENSE_GENERATIONINFO,
				XMLTAGLABEL_LICENSE_CUSTOMERINFO,
				//[8..11]
				XMLTAGLABEL_LICENSE_NPS,
					XMLTAGLABEL_LICENSE_NP,
						XMLTAGLABEL_LICENSE_SELECTS,
							XMLTAGLABEL_LICENSE_SELECT,
				//[12..18]
				XMLTAGLABEL_LICENSE_SECURIZED,
					XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED,
						XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE,
							XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT,
					XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED,
						XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE,
							XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT
		};

#endif


