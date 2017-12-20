//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clformat.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/09/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    API con la DLL de formateos
//
//
//==============================================================================

#include "clstr.h"
#include "strindex.h"
#include "formattp.h"

#ifndef __CLFORMAT__H__
#define __CLFORMAT__H__

#include "m4stl.hpp"

class ClENG_DBFormat ;

//=========================================================================ClENG_Format
class ClENG_Format
{
MIT_PRIVATE:
	ClMIT_Str m_oDesc ;
	ClMIT_IdNumDesc m_oLocation ;

	StMIT_Inspector* m_poInsp;	//Simple link

	ClENG_DBFormat *m_poOwner ;
MIT_PUBLIC:

	/// Tipos propios de la DLL

	enum OTHERS { MAX_SIZE_PROP = 40, MAX_SIZE_FORMAT_NAME = 255, NULL_HANDLE=-1, 
		ID_NULL=-1, MAX_NUM_PROPS=45 };

MIT_PRIVATE:

	m4uint32_t m_iIdFormat; // Id del formato del canal, necesario para buscar poste-
	                        // riormente por ese id en la ClENG_DBFormat
	m4int16_t m_iHandle;    // Handle correspondiente a ese formato que devuelve la dll
						    // de formatos
	m4uint32_t m_iTaskId;   // Id de la tarea que esta gestionando el MIEnv en el modulo
	                        // de formatos
MIT_PUBLIC:
	 
	// ------------------------- Inicialización ----------------------------------	

	ClENG_Format() ;
	virtual	~ClENG_Format() { ClENG_Format::End();}
	virtual void End() ;

	// A Init se le pasan el id del formato y su nombre, por si se trata de WINDOWS, 
	// ISO o uno particular. Creará un nuevo formato vacío al que posteriormente ha-
	// brá que asignarle las propiedades que queramos.

	void Init( ClENG_DBFormat *ai_poOwner, ClMIT_IdNumDesc *ai_poIdDesc ) ;
	
	// Lista de properties validas y su nombre, en documentacion de DLL.

	void SetProperty (m4pchar_t ai_pcProperty, m4pchar_t ai_pcValue );

	// ------------------------------Gets & Sets --------------------------------------			


	m4uint32_t GetId() { return m_iIdFormat ; }

	// ------------------------------Aplicacion de formateos---------------------------			

	void Apply( const m4char_t *ai_pcInStr, FORMAT_DATA_TYPE ai_Type, ClMIT_Str *ao_poOutStr );
};

//=========================================================================ClENG_DBFormat

class ClENG_DBFormat
{
	// Este objeto es similar la ClFONT_M_DBFont, se cargará al principio y se descargará
	// al final
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	typedef map<ClMIT_Str, ClENG_Format*, less<ClMIT_Str> > KeyMap_t;
	typedef KeyMap_t::iterator itKeyMap;

	// Array dinámico de Formats
	ClENG_Format * m_poFormat;
	m4int16_t m_iNFormats;

	KeyMap_t m_oMap;     // Map que relaciona cada IdFormat con un IdCurrency (si este
	   				     //	se ha definido


MIT_PUBLIC:
	// ------------------- Contructor y Destructor -------------------------------	

	ClENG_DBFormat() ;
	virtual	~ClENG_DBFormat() { ClENG_DBFormat::End(); }
	virtual void End() ;

	// ------------------------ Inicialización -----------------------------------	

	// Admisible que no haya ningun formato (osea, ai_iNFormats==0)

	void Init(m4int16_t ai_iNFormats);
	

	// --------------------- Operaciones con formats ------------------------------	

	// El formato de id ai_iIdFormat es creado e incluido en el array de fonts con el 
	// índice ai_iIndex. Si indice fuera de rango, error immediato.

	void Save(	m4int16_t ai_iIndex, ClMIT_IdNumDesc *ai_poIdDesc, m4pchar_t ai_pcIdCurrency ) ;

	// Devuelve NULL o el puntero al formato correspondiente al id que se indica.
	// Si SwOblig, da error immediato si no se encuentra

	ClENG_Format* FindFormat( m4uint32_t ai_uiIdFormat, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	// Devuelve NULL o el puntero correspondiente al formato asignado por defecto a un Currency
	// Si SwOblig genera un mensaje de error en caso de no encontrar el formato.
	ClENG_Format* GetCurDefaultFormat( m4pchar_t ai_pcIdCurrency, m4bool_t ai_bSwOblig=M4_TRUE,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );

};

#endif
