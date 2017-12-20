//## begin module%38EAF8A400F2.cm preserve=no
//## end module%38EAF8A400F2.cm

//## begin module%38EAF8A400F2.cp preserve=no
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
//## end module%38EAF8A400F2.cp

//## Module: queuebasedss%38EAF8A400F2; Subprogram specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\queuebasedss.hpp

#ifndef queuebasedss_h
#define queuebasedss_h 1

//## begin module%38EAF8A400F2.additionalIncludes preserve=no
//## end module%38EAF8A400F2.additionalIncludes

//## begin module%38EAF8A400F2.includes preserve=yes
//## end module%38EAF8A400F2.includes

// ServiceQ
#include <serviceq.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%38EAF8A400F2.declarations preserve=no
//## end module%38EAF8A400F2.declarations

//## begin module%38EAF8A400F2.additionalDeclarations preserve=yes
//## end module%38EAF8A400F2.additionalDeclarations


//## Class: ClQueueBasedSS%38EAF6A001CD
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClQueueBasedSS : public ClSSImplementation  //## Inherits: <unnamed>%38EAF7B400B1
{
  public:

    //## Other Operations (specified)
      //## Operation: SetServiceQ%954922417
      //	Asigna la variable miembro m_poServiceQ que es la cola de servicio de la que
      //	está enganchado el ejecutor para recibir por ella las peticiones que va a
      //	procesar.
      void SetServiceQ (ClServiceQ *ai_poServiceQ)
      {
        //## begin ClQueueBasedSS::SetServiceQ%954922417.body preserve=yes
		  if (NULL != ai_poServiceQ)	m_poServiceQ = ai_poServiceQ;
        //## end ClQueueBasedSS::SetServiceQ%954922417.body
      }

      //## Operation: GetServiceQ%954922419
      //	Asigna la variable miembro m_poServiceQ que es la cola de servicio de la que
      //	está enganchado el ejecutor para recibir por ella las peticiones que va a
      //	procesar.
      ClServiceQ * GetServiceQ () const
      {
        //## begin ClQueueBasedSS::GetServiceQ%954922419.body preserve=yes
		  return m_poServiceQ;
        //## end ClQueueBasedSS::GetServiceQ%954922419.body
      }

      //## Operation: Pop%958560334
      m4return_t Pop (ClBaseRequest *&ao_poRequest)
      {
        //## begin ClQueueBasedSS::Pop%958560334.body preserve=yes
		  return m_poServiceQ -> Pop ( ao_poRequest ) ;
        //## end ClQueueBasedSS::Pop%958560334.body
      }

      //## Operation: Push%967113586
      virtual m4return_t Push (ClBaseRequest *ai_poRequest)
      {
        //## begin ClQueueBasedSS::Push%967113586.body preserve=yes
			return m_poServiceQ -> Push (ai_poRequest);
        //## end ClQueueBasedSS::Push%967113586.body
      }

      //## Operation: Reply%967113587
      virtual m4return_t Reply (ClBaseRequest *ai_poRequest) = 0;

  protected:
    //## Constructors (specified)
      //## Operation: ClQueueBasedSS%954922422
      ClQueueBasedSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
        //## begin ClQueueBasedSS::ClQueueBasedSS%954922422.hasinit preserve=no
        //## end ClQueueBasedSS::ClQueueBasedSS%954922422.hasinit
        //## begin ClQueueBasedSS::ClQueueBasedSS%954922422.initialization preserve=yes
		: ClSSImplementation(ai_poConfiguratoin, ai_poTables, ai_poNavigator)
        //## end ClQueueBasedSS::ClQueueBasedSS%954922422.initialization
      {
        //## begin ClQueueBasedSS::ClQueueBasedSS%954922422.body preserve=yes
        //## end ClQueueBasedSS::ClQueueBasedSS%954922422.body
      }

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37D76711008D
      //## Role: ClQueueBasedSS::m_poServiceQ%37D7671102F0
      //## begin ClQueueBasedSS::m_poServiceQ%37D7671102F0.role preserve=no  protected: ClServiceQ { -> RHAN}
      ClServiceQ *m_poServiceQ;
      //## end ClQueueBasedSS::m_poServiceQ%37D7671102F0.role

  private:
  private: //## implementation
};

// Class ClQueueBasedSS 



//## begin module%38EAF8A400F2.epilog preserve=yes
//## end module%38EAF8A400F2.epilog


#endif


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClQueueBasedSS::SetOutputQ%954922418.body preserve=no
		  if (NULL != ai_poOutputQ)		m_poOutputQ = ai_poOutputQ;
//## end ClQueueBasedSS::SetOutputQ%954922418.body

//## begin ClQueueBasedSS::GetOutputQ%954922420.body preserve=no
		  return m_poOutputQ;
//## end ClQueueBasedSS::GetOutputQ%954922420.body

#endif
