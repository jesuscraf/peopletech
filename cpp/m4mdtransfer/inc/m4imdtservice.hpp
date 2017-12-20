
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4imdtservice.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                14-04-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene el interfaz de la clase del servicio de la librería
//    m4mdtransfer
//
//==============================================================================


#include "m4mdtransfer_dll.hpp"

#include "m4types.hpp"
#include "m4stl.hpp"
#include "xercesc/util/XMLString.hpp"


XERCES_CPP_NAMESPACE_USE



#ifndef __M4IMDTSERVICE_HPP__
#define __M4IMDTSERVICE_HPP__



//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class IMDTReadParameterManager;
class IMDTWriteParameterManager;
class IMDTReadWriteParameterManager;
class IMDTPresentationTool;
class M4IODBC;
class IMDTDictionary;
class IMDTTransformer;



//=================================================================================
//
// IMDTService
//
// Interfaz del gestor de servicios
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTService
{

public :

	//=================================================================================
	// Obtiene la fecha de la sesión
	//
	// @return
	//
	//			La fecha de sesión
	//
	//=================================================================================
	virtual	const string&	GetSessionDate( void ) const = 0 ;

	//=================================================================================
	// Obtiene el directorio temporal
	//
	// @return
	//
	//			El directorio temporal
	//
	//=================================================================================
	virtual	const string&	GetDebugDir( void ) const = 0 ;

	//=================================================================================
	// Obtiene el manejador de parámetros de lectura
	//
	// @return
	//
	//			El manejador de parámetros en su formato de lectura
	//
	//=================================================================================
	virtual const IMDTReadParameterManager*	GetIReadParameterManager( void ) const = 0 ;

	//=================================================================================
	// Obtiene el manejador de parámetros de escritura
	//
	// @return
	//
	//			El manejador de parámetros en su formato de escritura
	//
	//=================================================================================
	virtual IMDTWriteParameterManager*	GetIWriteParameterManager( void ) = 0 ;

	//=================================================================================
	// Obtiene el manejador de parámetros de lectura escritura escritura
	//
	// @return
	//
	//			El manejador de parámetros en su formato de lectura escritura
	//
	//=================================================================================
	virtual IMDTReadWriteParameterManager*	GetIReadWriteParameterManager( void ) = 0 ;

	//=================================================================================
	// Obtiene la tool de presentaciones
	//
	// @return
	//
	//			La tool de presentaciones
	//			NULL si la librería no ha sido inicializada
	//
	//=================================================================================
	virtual IMDTPresentationTool*	GetIPresentationTool( void ) = 0 ;

	//=================================================================================
	// Obtiene el manejador de base de datos
	//
	// @return
	//
	//			El manejador de base de datos
	//			NULL si la librería no ha sido inicializada
	//
	//=================================================================================
	virtual M4IODBC*	GetIODBC( void ) = 0 ;

	//=================================================================================
	// Obtiene el diccionario
	//
	// @return
	//
	//			El diccionario
	//			NULL si la librería no ha sido inicializada
	//
	//=================================================================================
	virtual IMDTDictionary*	GetIDictionary( void ) = 0 ;

	//=================================================================================
	// Obtiene el transformador
	//
	// @return
	//
	//			El transformador
	//			NULL si la librería no ha sido inicializada
	//
	//=================================================================================
	virtual IMDTTransformer*	GetITransformer( void ) = 0 ;



//=================================================================================
// Constantes de nombres de los atributos que se buscan
//=================================================================================

	static	XMLCh*		sm_sXMLChContentType ;
	static	XMLCh*		sm_sXMLChDB ;
	static	XMLCh*		sm_sXMLChInsert ;
	static	XMLCh*		sm_sXMLChIsNull ;
	static	XMLCh*		sm_sXMLChIsPrintable ;
	static	XMLCh*		sm_sXMLChMultiline ;
	static	XMLCh*		sm_sXMLChName ;
	static	XMLCh*		sm_sXMLChSelect ;
	static	XMLCh*		sm_sXMLChOperation ;
	static	XMLCh*		sm_sXMLChUpdateFunctions ;
	static	XMLCh*		sm_sXMLChValue ;
	static	XMLCh*		sm_sXMLChVisible ;
	static	XMLCh*		sm_sXMLChAlias ;
	static	XMLCh*		sm_sXMLChCaption ;
	static	XMLCh*		sm_sXMLChDelete ;
	static	XMLCh*		sm_sXMLChDeleteNumber ;
	static	XMLCh*		sm_sXMLChFieldPosition ;
	static	XMLCh*		sm_sXMLChHasWhere ;
	static	XMLCh*		sm_sXMLChInsertNumber ;
	static	XMLCh*		sm_sXMLChInternalField ;
	static	XMLCh*		sm_sXMLChLogicField ;
	static	XMLCh*		sm_sXMLChLogicFields ;
	static	XMLCh*		sm_sXMLChLogicPosition ;
	static	XMLCh*		sm_sXMLChLogicObject ;
	static	XMLCh*		sm_sXMLChLookup ;
	static	XMLCh*		sm_sXMLChLookups ;
	static	XMLCh*		sm_sXMLChLookupRegister ;
	static	XMLCh*		sm_sXMLChM4Type ;
	static	XMLCh*		sm_sXMLChPkPosition ;
	static	XMLCh*		sm_sXMLChPrecision ;
	static	XMLCh*		sm_sXMLChRealField ;
	static	XMLCh*		sm_sXMLChRealObject ;
	static	XMLCh*		sm_sXMLChRealObjects ;
	static	XMLCh*		sm_sXMLChScale ;
	static	XMLCh*		sm_sXMLChTablePosition ;
	static  XMLCh*		sm_sXMLChTable ;
	static  XMLCh*		sm_sXMLChRegister ;
	static  XMLCh*		sm_sXMLChField ;

} ;



#endif


