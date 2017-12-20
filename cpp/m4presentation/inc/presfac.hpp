//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presfac.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:  
//
//    This module defines the class of the factory of presentations
//
//
//==============================================================================

#ifndef __PRESFAC_HPP__
#define __PRESFAC_HPP__

#include "m4presentation_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "attributes.hpp"

class ClCache ;
class ClPresentationCache ;
class ClPresentation;
class ClVMBaseEnv;
class ClCacheDirectory;
class ClVMStatisticsWrapper;
class ClCsTrace;

class ClChannel;
class ClAccess;
class ClItem;
class ClChannelManager;

//-----------------------------------------------------------------------------
//Creamos una marcro pq queda mejor.
//Importante!!: En los tamaños NO incluye el NULL terminado!!.
const	m4char_t M4PRESENTATION_NOT_FOUND[]									= "NOT_FOUND";
const	m4int8_t M4PRESENTATION_NOT_FOUND_SIZE								= strlen(M4PRESENTATION_NOT_FOUND);

const	m4char_t M4PRESENTATION_GRANTS[]									= "Grants";
const	m4int8_t M4PRESENTATION_GRANTS_SIZE									= strlen(M4PRESENTATION_GRANTS);
const	m4char_t M4PRESENTATION_ID_CHANNEL[]								= "Idchannel";
const	m4int8_t M4PRESENTATION_ID_CHANNEL_SIZE								= strlen(M4PRESENTATION_ID_CHANNEL);
const	m4char_t M4PRESENTATION_ID_NODE[]									= "Idnode";
const	m4int8_t M4PRESENTATION_ID_NODE_SIZE								= strlen(M4PRESENTATION_ID_NODE);
const	m4char_t M4PRESENTATION_ID_ITEM[]									= "Iditem";
const	m4int8_t M4PRESENTATION_ID_ITEM_SIZE								= strlen(M4PRESENTATION_ID_ITEM);
const	m4char_t M4PRESENTATION_EXTENDS[]									= "Extends";
const	m4int8_t M4PRESENTATION_EXTENDS_SIZE								= strlen(M4PRESENTATION_EXTENDS);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Comunes.
const	m4char_t M4PRESENTATION_CLASSNAME[]									= "Classname";
const	m4int8_t M4PRESENTATION_CLASSNAME_SIZE								= strlen(M4PRESENTATION_CLASSNAME);
const	m4char_t M4PRESENTATION_ALIAS[]										= "Alias";
const	m4int8_t M4PRESENTATION_ALIAS_SIZE									= strlen(M4PRESENTATION_ALIAS);
const	m4char_t M4PRESENTATION_TYPE_PRESENTATION[]							= "Presentation";
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Etiquetas del OBL relacionadas con la expansion de idiomas para tiempo de disenio
const	m4char_t M4PRESENTATION_TRADUCTPROP[]								= "TraductProp";
const	m4int8_t M4PRESENTATION_TRADUCTPROP_SIZE							= strlen(M4PRESENTATION_TRADUCTPROP);

const	m4char_t M4PRESENTATION_TRADUCTPROPNAME[]							= "Name";
const	m4int8_t M4PRESENTATION_TRADUCTPROPNAME_SIZE						= strlen(M4PRESENTATION_TRADUCTPROPNAME);

const	m4char_t M4PRESENTATION_INHERITSTATUS[]								= "Inheritstatus";
const	m4int8_t M4PRESENTATION_INHERITSTATUS_SIZE							= strlen(M4PRESENTATION_INHERITSTATUS);
const	m4int8_t M4PRESENTATION_INHERITSTATUS_VALUE_NORMAL					= 1;
const	m4int8_t M4PRESENTATION_INHERITSTATUS_VALUE_OVERWRITTEN				= 2;
const	m4int8_t M4PRESENTATION_INHERITSTATUS_VALUE_INHERITED				= 3;

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//Etiquetas del OBL relacionadas con herencia.
const	m4char_t M4PRESENTATION_INHERIT_TYPE_PRESENTATION_DELETEDOBJECT[]	= "Deletedobject";

const	m4char_t M4PRESENTATION_INHERIT_PROP_POSITIONAFTER[]				= "Positionafter";
const	m4char_t M4PRESENTATION_INHERIT_PROP_POSITIONAFTER_BEGINVALUE[]		= "##BEGIN##";

const	m4char_t M4PRESENTATION_INHERIT_PROP_DEFAULT_VALUE[]				= "##DEFAULT_VALUE##";
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Etiquetas del OBL relacionadas con includes.
const	m4char_t M4PRESENTATION_INCLUDE_TYPE_PRESENTATION_INCLUDE[]			= "Includepanel";
const	m4char_t M4PRESENTATION_INCLUDE_TYPE_PRESENTATION_INCLUDE_EVENT[]	= "Includeevent";

const	m4char_t M4PRESENTATION_INCLUDE_TYPE_INCLUDECALL[]					= "Includecall";
const	m4char_t M4PRESENTATION_INCLUDE_TYPE_ARGUMENT[]						= "Argument";

const	m4char_t M4PRESENTATION_INCLUDE_PROP_IDINCLUDE[]					= "Idinclude";
const	m4char_t M4PRESENTATION_INCLUDE_PROP_INCLUDEALIAS[]					= "Idincludealias";
const	m4char_t M4PRESENTATION_INCLUDE_PROP_VALUEARG[]						= "Value";
//-----------------------------------------------------------------------------

class M4_DECL_M4PRESENTATION ClPresentationFactory
{
protected:
	ClChannelManager				*m_poChannelManager;
	ClPresentationCache				*m_poPresCache;
	ClVMStatisticsWrapper			*m_poSttWrapper;
	ClCacheDirectory				*m_poCacheDir;
	ClCsTrace						*m_poCsTrace;
	m4bool_t						m_bInit;

protected:
	m4return_t InitCache (ClCache * ai_poPresCache, ClVMBaseEnv * ai_poRegistry);
	m4return_t _End ();
	
	virtual m4return_t  _InternalBuildPresentationFromId( const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, ClPresentation * & ao_rpPresentation) = 0;
	m4return_t _debugPresentationToFile( ClPresentation * ai_rpPresentation, ClPresentationAttributes ai_ClPresentationAttributes);
	
public:

	ClPresentationFactory( void );
	~ClPresentationFactory( void );

	m4return_t  BuildPresentationFromId (m4pchar_t ai_pcIdPresentation, m4pcchar_t ai_pcOrganization, m4pcchar_t ai_pcRole, m4uint8_t ai_iLanguage, m4bool_t ai_bMappingAllowed, m4bool_t ai_bDefinition, ClPresentation * & ao_rpPresentation);
};

#endif

