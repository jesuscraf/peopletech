//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           cldbgutil.hpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:             21 ene 99
// Language:        C++ 
// Operating System: WINDOWS, UNIX
// Design Document:  
//
// 
// Definition: 
//  
//    Utility tools box
//
//==============================================================================


#ifndef _CLDBGUTIL_HPP_
#define _CLDBGUTIL_HPP_

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"

//Declarations
class ClAccess;
class ClChannelManager;
class ClChannel;
class m4VariantType;
class ClVMBaseEnv;


//#################################
//###	 Cajon de Utilidades	###
//#################################

class ClDbgUtil {

protected:
	m4char_t	*m_pcWS;		//Workspace			(working buffer)
	m4uint32_t	m_iSizeWS;		//Workspace Size	(bytes reserved)
	m4uint32_t	m_iLengthWS;	//Workspace Length	(length string)

public:
	//Constructor's
	ClDbgUtil(void) {
		m_pcWS=NULL;
		m_iSizeWS=0;
		m_iLengthWS=0;
	}

	ClDbgUtil(m4uint32_t ai_iSize) {
		_ResizeWS(ai_iSize);
	}

	ClDbgUtil(const m4char_t *ai_pcString) {
		if (!SetWS(ai_pcString))
			_ResetWS();
	}

	//Copy-Constructor
	ClDbgUtil(ClDbgUtil& ai_oUtil) {
		if ( !SetWS(ai_oUtil.GetWS()) )
			_ResetWS();
	}

	//Destructor
	~ClDbgUtil(void) {
		_ResetWS();
	}

	//Assign Operator
	ClDbgUtil& operator= (ClDbgUtil& ai_oUtil) {
		if ( !SetWS(ai_oUtil.GetWS()) )
			_ResetWS();
		return *this;
	}

	//*** To Work with the WorkSpace area ***
	m4bool_t	IsNotEmptyWS(void)	{ return ((m_iLengthWS!=0) ? M4_TRUE : M4_FALSE); }
	m4bool_t	IsEmptyWS	(void)	{ return ((m_iLengthWS==0) ? M4_TRUE : M4_FALSE); }
	m4bool_t	IsNotUsedWS	(void)	{ return ((m_iSizeWS  ==0) ? M4_TRUE : M4_FALSE); }
	m4bool_t	IsUsedWS	(void)	{ return ((m_iSizeWS  !=0) ? M4_TRUE : M4_FALSE); }
	m4char_t*	GetWS		(void)	{ return m_pcWS; }	
	m4uint32_t	GetLengthWS	(void)	{ return m_iLengthWS; }
	m4uint32_t	GetSizeWS	(void)	{ return m_iSizeWS; }
	m4bool_t	SetWS		(const m4char_t *ai_pcString) {	//String
		if (!ai_pcString)
			return M4_FALSE;
		m4uint32_t iLength=strlen(ai_pcString);
		return SetWS(ai_pcString, iLength);
	}
	m4bool_t	SetWS		(const m4char_t *ai_pcString, m4uint32_t ai_iLength);	//String + LongString
	m4bool_t	AppendWS	(const m4char_t *ai_pcString);	//String
	m4bool_t	InsertWS	(const m4char_t *ai_pcString);	//String

	//Otras funciones:
	//m4return_t		DumpTreePath(ClAccess* ai_poAccess, m4uint32_t ai_hNode);
	//const m4char_t* DumpTreePathString(ClAccess* ai_poAccess, m4uint32_t ai_hNode);

protected:
	//*** Internal functions to Work with the WorkSpace area ***
	m4bool_t _ResizeWS		(m4uint32_t ai_iSize)
	{
		_ResetWS();
		m_pcWS=new m4char_t[ai_iSize];
		if (!m_pcWS)
			return M4_FALSE;
		*m_pcWS='\0';
		m_iSizeWS=ai_iSize;
		m_iLengthWS=0;
		return M4_TRUE;
	}

	void _ResetWS(void)
	{
		if (m_pcWS) {
			delete [] m_pcWS;
			m_pcWS=NULL;
		}
		m_iSizeWS=0;
		m_iLengthWS=0;
	}

	void _SetWS(const m4char_t *ai_pcString, m4uint32_t ai_iLength) {
		memcpy(m_pcWS, ai_pcString, ai_iLength);
		m_pcWS[ai_iLength]='\0';
		m_iLengthWS=ai_iLength;
	}
};



//#####################################################
//###			Funciones Globales					###
//#####################################################

//Dump Info
string		M4_DECL_M4DM	GetTreePath			(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister = M4_FALSE);
	
	// Volcado basado en le Current del nodo actual
string		M4_DECL_M4DM	DumpTreePath					(ClAccess* ai_poAccess, m4uint32_t ai_hNode);
	// Vuelca ademas el item donde esta posicionado (Utiliza el Current).
string		M4_DECL_M4DM	DumpTreePathItem				(ClAccess* ai_poAccess, m4uint32_t ai_hNode);
	// Volcado basado en el Register del nodo actual
string		M4_DECL_M4DM	DumpTreePathRegister			(ClAccess* ai_poAccess, m4uint32_t ai_hNode);
	// Vuelca el item donde esta posicionado (Utiliza el Register).
string		M4_DECL_M4DM	DumpTreePathRegisterItem		(ClAccess* ai_poAccess, m4uint32_t ai_hNode);

//Gestion de Errores

/* Bug 0079117
El identificador del error debe ser unsigned
*/
m4return_t	M4_DECL_M4DM	FindOrCreateChannel	(ClChannelManager *ai_pChannelManager, m4pcchar_t ai_pcChannelId, ClChannel* &ao_pChannel);
m4return_t	M4_DECL_M4DM	ChannelErrorCodeByParams(m4uint32_t ai_uiCodErr, m4VariantType *ai_vaParam, m4int32_t ai_iNumArgs, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &ai_uiBufferSize, m4language_t ai_uiLang);
m4return_t	M4_DECL_M4DM	ChannelErrorCode(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &ai_uiBufferSize);

m4return_t	M4_DECL_M4DM	M4GetErrorString	(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &ai_uiBufferSize,m4language_t ai_uiLang);
m4return_t	M4_DECL_M4DM	M4GetErrorStringByParams(m4uint32_t ai_uiCodErr, m4VariantType *ai_vaParam, m4int32_t ai_iNumArgs, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &ai_uiBufferSize,m4language_t ai_uiLang);
			


void		M4_DECL_M4DM	M4ErrorLookup		(ClChannelManager *ai_pChannelManager, m4puint32_t ai_puErrorCodes, m4uint32_t ai_uNumErrors);


//Debug FileName
string		M4_DECL_M4DM	M4VMGetDebugFileName(m4pcchar_t ai_pszFileName, ClVMBaseEnv *ai_poEnv);


#endif	// _CLDBGUTIL_HPP_

