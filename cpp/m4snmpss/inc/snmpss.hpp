//## begin module%407BC93A03D3.cm preserve=no
//## end module%407BC93A03D3.cm

//## begin module%407BC93A03D3.cp preserve=no
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
//## end module%407BC93A03D3.cp

//## Module: snmpss%407BC93A03D3; Package specification
//## Subsystem: M4SnmpSS::inc%407BC8EC01FA
//## Source file: z:\m4snmpss\inc\snmpss.hpp

#ifndef snmpss_h
#define snmpss_h 1

//## begin module%407BC93A03D3.additionalIncludes preserve=no
//## end module%407BC93A03D3.additionalIncludes

//## begin module%407BC93A03D3.includes preserve=yes
#include "m4snmpss_dll.hpp"
//## end module%407BC93A03D3.includes

// clssimplementacion
#include <clssimplementacion.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
//## begin module%407BC93A03D3.declarations preserve=no
//## end module%407BC93A03D3.declarations

//## begin module%407BC93A03D3.additionalDeclarations preserve=yes
//## end module%407BC93A03D3.additionalDeclarations


//## begin M4SnmpSS%4072868E0271.preface preserve=yes
//## end M4SnmpSS%4072868E0271.preface

//## Class: M4SnmpSS%4072868E0271
//## Category: M4SnmpSS%407286820237
//## Subsystem: M4SnmpSS::inc%407BC8EC01FA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4073AD3B00B1; { -> }
//## Uses: <unnamed>%407BCBB001F8;ClOblConfiguration { -> }

class M4_DECL_M4SNMPSS M4SnmpSS : public ClSSImplementation  //## Inherits: <unnamed>%407286AD0149
{
  //## begin M4SnmpSS%4072868E0271.initialDeclarations preserve=yes
  //## end M4SnmpSS%4072868E0271.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4SnmpSS%1081248170
      M4SnmpSS (ClConfiguration *ai_poConfiguration, ClTables* ai_poTables, ClNavigatorFirewall* ai_poNavigator);

    //## Destructor (generated)
      virtual ~M4SnmpSS();


    //## Other Operations (specified)
      //## Operation: Initialize%1081322813
      m4return_t Initialize ();

      //## Operation: Stop%1081862814
      //	Cuando se inicaia el proceso de ShutDown se notifica a todos los subsistemas
      //	mediante este método. Como parámetro se pasa en segundos el timepo que queda
      //	hasta el shutdown definitivo y cada subsistema debe de ir haciendo en los
      //	pasos previos según corresponda las aaciones pertinentes para dejar todo
      //	consistente antes de ese momento o al menos en la última llamada (tiempo = 0)
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: ShutDown%1081862815
      //	Shuts down this subsystem and all the subsystems matched to it
      virtual m4return_t ShutDown ();

      //## Operation: Start%1081862816
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      virtual m4return_t Start ();

      //## Operation: IsReadyToClose%1081862817
      //	Durante el proceso de ShutDown, este métodose llamará para todos los
      //	subsistemas con objeto de verificar el momento en el que sea posible pasar a
      //	la destrucción de todos los subsistemas.
      //	Aquel subsistema que precise de una deshabitación previa a la destucción,
      //	como puede ser la finalización de los threads asociados, hará esto en un
      //	paso previo (Stop) y a la llamada de este método, devolverá true o false
      //	dependiendo de si el thread a finalizado ya o no, es decr dependiendo de si
      //	el subsistema está listo o no para ser destruido.
      virtual m4bool_t IsReadyToClose ();

    // Additional Public Declarations
      //## begin M4SnmpSS%4072868E0271.public preserve=yes
      //## end M4SnmpSS%4072868E0271.public

  protected:
    // Additional Protected Declarations
      //## begin M4SnmpSS%4072868E0271.protected preserve=yes
      //## end M4SnmpSS%4072868E0271.protected

  private:
    // Additional Private Declarations
      //## begin M4SnmpSS%4072868E0271.private preserve=yes
      //## end M4SnmpSS%4072868E0271.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4SnmpSS%4072868E0271.implementation preserve=yes
      //## end M4SnmpSS%4072868E0271.implementation

};

//## begin M4SnmpSS%4072868E0271.postscript preserve=yes
//## end M4SnmpSS%4072868E0271.postscript

// Class M4SnmpSS 

//## begin module%407BC93A03D3.epilog preserve=yes
//## end module%407BC93A03D3.epilog


#endif
