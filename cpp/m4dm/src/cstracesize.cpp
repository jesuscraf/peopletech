//## begin module%3935423E031F.cm preserve=no
//## end module%3935423E031F.cm

//## begin module%3935423E031F.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%3935423E031F.cp

//## Module: cstracesize%3935423E031F; Package body
//## Subsystem: M4Dm::src%3819B13101F2
//## Source file: D:\evol\m4dm\src\cstracesize.cpp

//## begin module%3935423E031F.additionalIncludes preserve=no
//## end module%3935423E031F.additionalIncludes

//## begin module%3935423E031F.includes preserve=yes
//## end module%3935423E031F.includes

#include "m4stl.hpp"
// cstracesize
#include <cstracesize.hpp>


//## begin module%3935423E031F.declarations preserve=no
//## end module%3935423E031F.declarations

//## begin module%3935423E031F.additionalDeclarations preserve=yes
//## end module%3935423E031F.additionalDeclarations


// Class ClCsTraceSize 










ClCsTraceSize::ClCsTraceSize (ClCsTraceSize* const ai_pParent, const m4char_t* const ai_id, const m4int8_t ai_traceLevel, const eDeep ai_deep)
  //## begin ClCsTraceSize::ClCsTraceSize%959793049.hasinit preserve=no
      : m_size(0), m_offsetIni(-1), m_childCount(0), m_deep((eDeep)(ai_deep + 1)), m_traceLevel(ai_traceLevel - 1), m_id(ai_id), m_pParent(ai_pParent)
  //## end ClCsTraceSize::ClCsTraceSize%959793049.hasinit
  //## begin ClCsTraceSize::ClCsTraceSize%959793049.initialization preserve=yes
  //## end ClCsTraceSize::ClCsTraceSize%959793049.initialization
{
  //## begin ClCsTraceSize::ClCsTraceSize%959793049.body preserve=yes
  //## end ClCsTraceSize::ClCsTraceSize%959793049.body
}


ClCsTraceSize::~ClCsTraceSize ()
{
  //## begin ClCsTraceSize::~ClCsTraceSize%959793050.body preserve=yes
	ClCsTraceSizeVector::iterator it = m_childVector.begin();
	for (; it != m_childVector.end(); it++) {
		delete (*it);
	}
  //## end ClCsTraceSize::~ClCsTraceSize%959793050.body
}



//## Other Operations (implementation)
ClCsTraceSize* const ClCsTraceSize::CreateChild (const m4char_t* const ai_id)
{
  //## begin ClCsTraceSize::CreateChild%959793054.body preserve=yes
	/*
	 * Si ya he llegado al nivel de profundidad mínimo me devuelvo a mi mismo
	 * y acumulo yo los tamaños de mis hijos.
	 * Si me pasan un id lo busco por si ya existe, si no lo encuentor lo creo.
	 * Si no me pasan un id, lo creo directamente.
	 */
	if (m_traceLevel == 0) {
		return this;
	}

	if (ai_id) {
		ClCsTraceSizeVector::iterator it = m_childVector.begin();
		for (; it != m_childVector.end(); it++) {
			if (strcmp((*it)->m_id, ai_id) == 0) {
				return (*it);
			}
		}
	}

	ClCsTraceSize* pChild = new ClCsTraceSize(this, ai_id, m_traceLevel, m_deep);
	m_childVector.push_back(pChild);
	m_childCount++;
	return pChild;
  //## end ClCsTraceSize::CreateChild%959793054.body
}

void ClCsTraceSize::WriteToFile (FILE* const ai_file, const m4int32_t ai_deep)
{
  //## begin ClCsTraceSize::WriteToFile%959866854.body preserve=yes
	if (!ai_file) return;

	fprintf(ai_file, "\t\t");
	for (m4int32_t i = 0; i < ai_deep; i++) fprintf(ai_file, "\t");

	if (m_id) {
		fprintf(ai_file, "Id = %s", m_id);
	} else {
		switch (ai_deep) {
		case 3:
			fprintf(ai_file, "Id = Block");
			break;
		case 4:
			fprintf(ai_file, "Id = Register");
			break;
		}
	}
	
	fprintf(ai_file, " \tSize = %d", m_size);
	
	if (m_childCount) {
		fprintf(ai_file, " \tChild count = %d", m_childCount);
	}
	fprintf(ai_file, "\n");

	ClCsTraceSizeVector::iterator it = m_childVector.begin();
	for (; it != m_childVector.end(); it++) {
		(*it)->WriteToFile(ai_file, ai_deep + 1);
	}
  //## end ClCsTraceSize::WriteToFile%959866854.body
}

// Additional Declarations
  //## begin ClCsTraceSize%39353F2202D5.declarations preserve=yes
  //## end ClCsTraceSize%39353F2202D5.declarations

//## begin module%3935423E031F.epilog preserve=yes
//## end module%3935423E031F.epilog
