//## begin module%3935423402AD.cm preserve=no
//## end module%3935423402AD.cm

//## begin module%3935423402AD.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3935423402AD.cp

//## Module: cstracesize%3935423402AD; Package specification
//## Subsystem: M4Dm::inc%3819B125003C
//## Source file: D:\evol\m4dm\inc\cstracesize.hpp

#ifndef cstracesize_h
#define cstracesize_h 1

//## begin module%3935423402AD.additionalIncludes preserve=no
//## end module%3935423402AD.additionalIncludes

//## begin module%3935423402AD.includes preserve=yes
#include <stdio.h>
#include "m4dm_dll.hpp"
#include "m4stl.hpp"
//## end module%3935423402AD.includes

// m4types
#include <m4types.hpp>

class M4_DECL_M4DM ClCsTraceSize;

//## begin module%3935423402AD.declarations preserve=no
//## end module%3935423402AD.declarations

//## begin module%3935423402AD.additionalDeclarations preserve=yes
//## end module%3935423402AD.additionalDeclarations


//## begin ClCsTraceSizeVector%393540FE00DA.preface preserve=yes
//## end ClCsTraceSizeVector%393540FE00DA.preface

//## Class: ClCsTraceSizeVector%393540FE00DA; Instantiated Class
//## Category: M4Dm%38199213031A
//## Subsystem: M4Dm::inc%3819B125003C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39354ED30371;ClCsTraceSize { -> F}

typedef vector< ClCsTraceSize* > ClCsTraceSizeVector;

//## begin ClCsTraceSizeVector%393540FE00DA.postscript preserve=yes
//## end ClCsTraceSizeVector%393540FE00DA.postscript

//## begin ClCsTraceSize%39353F2202D5.preface preserve=yes
//## end ClCsTraceSize%39353F2202D5.preface

//## Class: ClCsTraceSize%39353F2202D5
//## Category: M4Dm%38199213031A
//## Subsystem: M4Dm::inc%3819B125003C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClCsTraceSize 
{
  //## begin ClCsTraceSize%39353F2202D5.initialDeclarations preserve=yes
  //## end ClCsTraceSize%39353F2202D5.initialDeclarations

  public:
    //## Class: eDeep%3940D0C80381
    //	Niveles posibles de profundidad en la jerarquía.
    //## Category: M4Dm%38199213031A
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { NONE = 0, TRANSACTION, CHANNEL, NODE, BLOCK, REGISTER } eDeep;

    //## Constructors (specified)
      //## Operation: ClCsTraceSize%959793049
      ClCsTraceSize (ClCsTraceSize* const ai_pParent, const m4char_t* const ai_id, const m4int8_t ai_traceLevel, const eDeep ai_deep);

    //## Destructor (specified)
      //## Operation: ~ClCsTraceSize%959793050
      ~ClCsTraceSize ();


    //## Other Operations (specified)
      //## Operation: SetOffsetIni%959793051
      void SetOffsetIni (const m4int32_t ai_offset);

      //## Operation: SetOffsetEnd%959793052
      void SetOffsetEnd (const m4int32_t ai_offset);

      //## Operation: CreateChild%959793054
      ClCsTraceSize* const CreateChild (const m4char_t* const ai_id = 0);

      //## Operation: GetpParent%959793055
      //	Devuelve el ClCsTraceSize padre.
      //	Si se especifica un nivel de profundidad buscará en la jerarquía al padre de
      //	ese nivel. Si no se especifica nada devuelve el inmediatamente superior.
      //
      //	@param ai_deep    Profundidad del padre que se quiere obtener.
      ClCsTraceSize* const GetpParent (const eDeep ai_deep = NONE);

      //## Operation: WriteToFile%959866854
      //	Escribe toda la estructura en unfichero. Lo hace de forma recursiva.
      //
      //	@param ai_file   Fichero donde vamos a escribir.
      //	@param ai_deep   Profundidad de la recursión. Sirve para determinar la
      //	indentación.
      void WriteToFile (FILE* const ai_file, const m4int32_t ai_deep = 0);

  public:
    // Additional Public Declarations
      //## begin ClCsTraceSize%39353F2202D5.public preserve=yes
      //## end ClCsTraceSize%39353F2202D5.public

  protected:
    // Additional Protected Declarations
      //## begin ClCsTraceSize%39353F2202D5.protected preserve=yes
      //## end ClCsTraceSize%39353F2202D5.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_size%39353F5D027F
      //## begin ClCsTraceSize::m_size%39353F5D027F.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_size;
      //## end ClCsTraceSize::m_size%39353F5D027F.attr

      //## Attribute: m_offsetIni%39353F6603AF
      //## begin ClCsTraceSize::m_offsetIni%39353F6603AF.attr preserve=no  private: m4int32_t {UA} -1
      m4int32_t m_offsetIni;
      //## end ClCsTraceSize::m_offsetIni%39353F6603AF.attr

      //## Attribute: m_childCount%39353F900156
      //## begin ClCsTraceSize::m_childCount%39353F900156.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_childCount;
      //## end ClCsTraceSize::m_childCount%39353F900156.attr

      //## Attribute: m_deep%393FA8A800EF
      //	Indica el nivel de profundidad de la traza.
      //	Este puede ser:
      //	    1 Transaction.
      //	    2 Channel.
      //	    3 Node.
      //	    4 Block.
      //	    5 Register.
      //
      //	De esta forma podemos saber en que nivel estamos. Esto nos va a servir para
      //	buscar un padre de un determinado nivel.
      //## begin ClCsTraceSize::m_deep%393FA8A800EF.attr preserve=no  private: eDeep {UA} (eDeep)(ai_deep + 1)
      eDeep m_deep;
      //## end ClCsTraceSize::m_deep%393FA8A800EF.attr

      //## Attribute: m_traceLevel%3940CDB001CA
      //	Indica en nivel de traza que queremos obtener.
      //	Según vamos bajando niveles este disminuye, de forma que al llegar a 0
      //	dejamos de sacar más niveles de detalle.
      //## begin ClCsTraceSize::m_traceLevel%3940CDB001CA.attr preserve=no  private: m4int8_t {UA} ai_traceLevel - 1
      m4int8_t m_traceLevel;
      //## end ClCsTraceSize::m_traceLevel%3940CDB001CA.attr

      //## Attribute: m_id%39353F380146
      //## begin ClCsTraceSize::m_id%39353F380146.attr preserve=no  private: const m4char_t* const {UA} ai_id
      const m4char_t* const m_id;
      //## end ClCsTraceSize::m_id%39353F380146.attr

    // Data Members for Associations

      //## Association: M4Dm::<unnamed>%3935413801B0
      //## Role: ClCsTraceSize::m_childVector%3935413803AF
      //## begin ClCsTraceSize::m_childVector%3935413803AF.role preserve=no  private: ClCsTraceSizeVector { -> 1VHAN}
      ClCsTraceSizeVector m_childVector;
      //## end ClCsTraceSize::m_childVector%3935413803AF.role

      //## Association: M4Dm::<unnamed>%3935417302F5
      //## Role: ClCsTraceSize::m_pParent%39354176015F
      //## begin ClCsTraceSize::m_pParent%39354176015F.role preserve=no  private: ClCsTraceSize { -> 1RHAN}
      ClCsTraceSize *m_pParent;
      //## end ClCsTraceSize::m_pParent%39354176015F.role

    // Additional Private Declarations
      //## begin ClCsTraceSize%39353F2202D5.private preserve=yes
      //## end ClCsTraceSize%39353F2202D5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCsTraceSize%39353F2202D5.implementation preserve=yes
      //## end ClCsTraceSize%39353F2202D5.implementation

};

//## begin ClCsTraceSize%39353F2202D5.postscript preserve=yes
//## end ClCsTraceSize%39353F2202D5.postscript

// Class ClCsTraceSize 


//## Other Operations (inline)
inline void ClCsTraceSize::SetOffsetIni (const m4int32_t ai_offset)
{
  //## begin ClCsTraceSize::SetOffsetIni%959793051.body preserve=yes
	m_offsetIni = ai_offset;
  //## end ClCsTraceSize::SetOffsetIni%959793051.body
}

inline void ClCsTraceSize::SetOffsetEnd (const m4int32_t ai_offset)
{
  //## begin ClCsTraceSize::SetOffsetEnd%959793052.body preserve=yes
	if (m_offsetIni == -1) return;

	m_size += ai_offset - m_offsetIni;
	m_offsetIni = -1;
  //## end ClCsTraceSize::SetOffsetEnd%959793052.body
}

inline ClCsTraceSize* const ClCsTraceSize::GetpParent (const eDeep ai_deep)
{
  //## begin ClCsTraceSize::GetpParent%959793055.body preserve=yes
	if (ai_deep >= m_deep) return this;

	if (ai_deep == NONE || m_pParent == 0) return m_pParent;

	return m_pParent->GetpParent(ai_deep);
  //## end ClCsTraceSize::GetpParent%959793055.body
}

//## begin module%3935423402AD.epilog preserve=yes
//## end module%3935423402AD.epilog


#endif
