
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4mdfac.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz abstracto de la factoría de metadatos
//
//
//==============================================================================


#include "m4mdfac_dll.hpp"

#include <string.h>

#include "m4types.hpp"
#include "cldefine.hpp"
#include "cldates.hpp"
#include "clgeniod.hpp"
#include "cllstr.hpp"


#ifndef __M4MDFAC_HPP__
#define __M4MDFAC_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClAccess ;
class	ClCompiledMCR ;
class	ClBuidParameters ;
class	ClCMCRWrapper ;
class	ClCSCRWrapper ;
class	ClMappings ;
class	ClCMCRCache ;
class	ClCSCRCache ;
class	ClMapCache ;
class	m4VariantType ;




//=============================================================================
// Defines de los valores de las máscaras
//=============================================================================

#define	M4XMASK_ALL					0x000000001		// Todos
#define	M4XMASK_VERSION				0x000000002		// Versión. Para SOAP
#define	M4XMASK_NET					0x000000004		// Dataset de .NET
#define	M4XMASK_SOAP				0x000000008		// Soap
#define	M4XMASK_JSAPI				0x000000010		// JS API
#define	M4XMASK_OTHER_6				0x000000020		// Otro 6
#define	M4XMASK_OTHER_7				0x000000040		// Otro 7
#define	M4XMASK_OTHER_8				0x000000080		// Otro 8




//=================================================================================
//
// ClCMCRFactory
//
// Clase para el interfaz abstracto de la factoría
//
//=================================================================================

class	ClCMCRFactory
{

protected:

		ClCMCRFactory( void ) {} ;

public:

		virtual	~ClCMCRFactory( void ) {} ;


	//=================================================================================
	// Genera un metacanal compilado a partir de un fichero
	//=================================================================================
    virtual	m4return_t	MDFac_BuildFromFile		(
													m4pcchar_t		ai_pccFileName,
													m4uint8_t		ai_iCsType,
													m4uint8_t		ai_iUseMcrs,
													m4pchar_t		ai_pccPath,
													ClCompiledMCR*	&ao_rpCMCR
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Genera un metacanal compilado a partir de un acceso
	//=================================================================================
    virtual	m4return_t	MDFac_BuildFromAccess	(
													ClAccess		*ai_poAccess,
													m4uint8_t		ai_iCsType,
													m4uint8_t		ai_iRegister,
													m4uint8_t		ai_iLanguage,
													m4bool_t		ai_bAddSystem,
													ClCompiledMCR*	&ao_rpCMCR
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Genera un metacanal compilado a partir de su modelo en base de datos
	//=================================================================================
    virtual	m4return_t  MDFac_BuildFromId		(
													ClBuidParameters	&ai_roBP,
													ClCompiledMCR*		&ao_rpCMCR
												)
	{
		return( M4_ERROR ) ;
	}


	//=================================================================================
	// Genera un metacanal compilado en formato xml a partir de su modelo en base de datos
	//=================================================================================
    virtual	m4return_t  MDFac_BuildXML			(
													ClCompiledMCR		*ai_poCMCR,
													m4pcchar_t			ai_pccOrganization,
													m4uint32_t			ai_iMask,
													ClLongString		&ao_roXML,
													m4bool_t			ai_bGenHeader
												)
	{
		return( M4_ERROR ) ;
	}


	//=================================================================================
	// Genera un wrapper de metacanal compilado a partir de su modelo en base de datos
	//=================================================================================
	virtual	m4return_t  MDFac_BuildMDWrapper	(
													m4pcchar_t		ai_pccM4ObjId,
													m4pcchar_t		ai_pccOrgId,
													m4uint8_t		ai_iCsType,
													m4VariantType	&ai_roStartDate,
													m4VariantType	&ai_roEndDate,
													m4VariantType	&ai_roCorDate,
													m4uint8_t		ai_iLanguage,
													m4bool_t		ai_bApplyMappings,
													ClCMCRWrapper*	&ao_rpoMDWrap
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Genera un wrapper de seguridad compilada a partir de su modelo en base de datos
	//=================================================================================
	virtual	m4return_t	MDFac_BuildSecWrapper	(
													m4pcchar_t		ai_pccM4ObjId,
													m4uint8_t		ai_iCsType,
													m4pcchar_t		ai_pccMask,
													m4uint8_t		ai_iLanguage,
													ClCSCRWrapper*	&ai_poSecWrap
												)
	{
		return( M4_ERROR ) ;
	}


	//=================================================================================
	// Obtiene un tipo concreto de mapeos
	//=================================================================================
	virtual	m4return_t	MDFac_BuildMappings		(
													m4pcchar_t		ai_pccMapId,
													ClMappings*		&ai_rpoMappings
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Obtiene un mapeo para un tipo de mapeo, una sociedad y un origen
	//=================================================================================
	virtual	m4pchar_t	MDFac_GetMappingById	(
													m4pcchar_t		ai_pccMapId,
													m4pcchar_t		ai_pccOrganization,
													m4pcchar_t		ai_pccSource,
													m4bool_t		ai_bConvert
												)
	{
		return( NULL ) ;
	}

	//=================================================================================
	// Determina si 2 sociedades son equivalentes para un tipo de mapeo y un destino
	//=================================================================================
	virtual	m4return_t	MDFac_IsEqualOrg		(
													m4pcchar_t		ai_pccMapId,
													m4pcchar_t		ai_pccOrganization1,
													m4pcchar_t		ai_pccOrganization2,
													m4pcchar_t		ai_pccDestination,
													m4bool_t		ai_bConvert,
													m4bool_t&		ao_rbIsEquivalent
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Asocia una máscara de seguridada a un metacanal compilado
	//=================================================================================
    virtual	m4return_t  MDFac_AttachRSM			(
													ClCompiledMCR	*ai_poCMCR,
													m4pcchar_t		ai_pccRSM
												)
	{
		return( M4_ERROR ) ;
	}


	//=================================================================================
	// Construye accesos nuevos sobre los metacanales del diccionario
	//=================================================================================
	virtual	m4return_t	MDFac_BuildDictAccess	(
													ClAccess*	&ai_rpoDictAccess,
													ClAccess*	&ai_rpoTransAccess
												)
	{
		return( M4_ERROR ) ;
	}

	//=================================================================================
	// Borra accesos sobre los metacanales del diccionario
	//=================================================================================
    virtual	m4return_t	MDFac_RemoveDictAccess	(
													ClAccess	*ai_poDictAccess,
													ClAccess	*ai_poTransAccess
												)
	{
		return( M4_ERROR ) ;
	}


	//=================================================================================
	// Obtención de la caché de metadatos
	//=================================================================================
    virtual	ClCMCRCache*	MDFac_GetMDCache( void )
	{
		return( NULL )  ;
	}


	//=================================================================================
	// Obtención de la caché de seguridad
	//=================================================================================
    virtual	ClCSCRCache*	MDFac_GetSecCache( void )
	{
		return( NULL )  ;
	}


	//=================================================================================
	// Obtención de la caché de mapeos
	//=================================================================================
    virtual	ClMapCache*	MDFac_GetMapCache( void )
	{
		return( NULL )  ;
	}

} ;






//=================================================================================
//
// ClLaneArgumnent
//
// Clase para meter los parámetros del carril en la build
//
//=================================================================================


class	ClLaneArgumnent
{

protected:

	void	_Delete( m4pchar_t &ai_rpccPointer )
	{
		if( ai_rpccPointer != NULL )
		{
			delete [] ai_rpccPointer ;
			ai_rpccPointer = NULL ;
		}
	}

	m4return_t	_Serialize( ClGenericIODriver& IOD, m4pchar_t ai_pccPointer )
	{
		if( ai_pccPointer == NULL )
		{
			return( M4_ERROR ) ;
		}

		if( IOD.Write( (void*) ai_pccPointer, strlen( ai_pccPointer ) ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		return( M4_SUCCESS );
	}

	m4return_t	_DeSerialize( ClGenericIODriver& IOD, m4pchar_t &ai_rpccPointer )
	{

		m4uint32_t iSize ;


		if( IOD.ReadSize( iSize ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		ai_rpccPointer = new m4char_t[ iSize + 1 ] ;

		if( ai_rpccPointer == NULL )
		{
			return( M4_ERROR ) ;
		}

		if ( IOD.ReadBuffer( (void*) ai_rpccPointer ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		ai_rpccPointer[ iSize ] = '\0' ;

		return( M4_SUCCESS );
	}

public:

   ClCompiledMCR* m_poFatherCMCR ;
   m4pchar_t      m_pccFather ;
   m4pchar_t      m_pccFatherNode ;
   m4pchar_t      m_pccNode ;


		ClLaneArgumnent( void )
		{
		   Reset( M4_FALSE ) ;
		}
   
		~ClLaneArgumnent( void )
		{
		   Reset( M4_TRUE ) ;
		}


	void Reset( m4bool_t ai_bReleaseMemory = M4_TRUE )
	{
		m_poFatherCMCR = NULL ;

		if ( ai_bReleaseMemory == M4_TRUE )
		{
			_Delete( m_pccFather ) ;
			_Delete( m_pccFatherNode ) ;
			_Delete( m_pccNode ) ;
		}
		else
		{
			m_pccFather = NULL ;
			m_pccFatherNode = NULL ;
			m_pccNode = NULL ;
		}
	}

	// m_poFatherCMCR: no serializo el puntero al metacanal
	// porque en el otro lado no me vale para nada

	m4return_t Serialize( ClGenericIODriver& IOD )
	{
		if( _Serialize( IOD, m_pccFather ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( _Serialize( IOD, m_pccFatherNode ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( _Serialize( IOD, m_pccNode ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		return( M4_SUCCESS );
	}

	// m_poFatherCMCR: no deserializo el puntero al metacanal
	// porque en este lado no me vale para nada

	m4return_t DeSerialize ( ClGenericIODriver& IOD )
	{
		Reset( M4_TRUE ) ;

		if( _DeSerialize( IOD, m_pccFather ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( _DeSerialize( IOD, m_pccFatherNode ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( _DeSerialize( IOD, m_pccNode ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		return( M4_SUCCESS );
	}

} ;




//=================================================================================
//
// ClBuidParameters
//
// Clase para poner los argumentos del BuildFromId
//
//=================================================================================


class	ClBuidParameters
{

public:

	ClLaneArgumnent		*m_poLane ;
	m4pcchar_t			m_pccM4ObjId ;
	m4pcchar_t			m_pccIdOrg ;
	m4pcchar_t			m_pccRSM ;
	m4date_t			m_dStartDate ;
	m4date_t			m_dEndDate ;
	m4date_t			m_dCorDate ;
	m4uint8_t			m_iCsType ;
	m4uint8_t			m_iLanguage ;
	m4bool_t			m_bConvertRSM ;
	m4bool_t			m_bBuildSecurity ;
	m4bool_t			m_bApplyMappings ;


	   ClBuidParameters( void )
	   {
			m_poLane = NULL ;
			m_pccM4ObjId = NULL ;
			m_pccIdOrg = NULL ;
			m_pccRSM = NULL ;
			m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
			m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
			m_dCorDate = ClActualDate() ;
			m_iCsType = M4CL_CSTYPE_BOTH ;
			m_iLanguage = M4CL_LANGUAGE_NONE ;
			m_bConvertRSM = M4_FALSE ;
			m_bBuildSecurity = M4_FALSE ;
			m_bApplyMappings = M4_TRUE ;
	   }
   
	   ~ClBuidParameters( void ) {} ;

} ;



#endif

