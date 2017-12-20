//## begin module%37E7419B0253.cm preserve=no
//## end module%37E7419B0253.cm

//## begin module%37E7419B0253.cp preserve=no
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
//## end module%37E7419B0253.cp

//## Module: ClParametersHandler%37E7419B0253; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: e:\m4server\m4jsexejob\inc\clparametershandler.hpp

#ifndef clparametershandler_h
#define clparametershandler_h 1

//## begin module%37E7419B0253.additionalIncludes preserve=no
//## end module%37E7419B0253.additionalIncludes


class ClAccess;
class ClNode;
class ClJSJob;

//## begin module%37E7419B0253.declarations preserve=no
//## end module%37E7419B0253.declarations

//## begin module%37E7419B0253.additionalDeclarations preserve=yes
//## end module%37E7419B0253.additionalDeclarations


//## begin ClJSParametersHandlers%37E738CE01F0.preface preserve=yes
//## end ClJSParametersHandlers%37E738CE01F0.preface

//## Class: ClJSParametersHandlers%37E738CE01F0
//	Esta clase se ocupa de manejar los parámetros de las tareas que ejecuta el
//	JS .Es decir tiene métodos que permiten recuperar los parámetros de entrada
//	y salida asociados a una tarea.
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E8D30D023C;IExecutor { -> F}
//## Uses: <unnamed>%37E8EA310353;M4ClTimeStamp { -> F}
//## Uses: <unnamed>%3821A70B02D1;ClJSParameter { -> F}

class ClJSParametersHandlers 
{
  //## begin ClJSParametersHandlers%37E738CE01F0.initialDeclarations preserve=yes
  //## end ClJSParametersHandlers%37E738CE01F0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSParametersHandlers%937902673
      ClJSParametersHandlers (ClAccess *ai_poAccessJExec);

    //## Destructor (specified)
      //## Operation: ~ClJSParametersHandlers%937902674
      ~ClJSParametersHandlers ();


    //## Other Operations (specified)
      //## Operation: RecoverParams%937902675
      //	Este es el método que se encarga de recuperar los parámetros de entrada y
      //	salida de la definición de la tarea y de almacenarlos en los soportes
      //	adecuados (pila de la máquina virtual, canal de datos , etc.)
      m4return_t RecoverParams ();

      //## Operation: AddParams%937902676
      //	Este es el método encargado de añadir el valor de los parámetros de salida
      //	de la tarea a la definición de la ejecución actual . Este valor se
      //	almacenará en el canal de deficiones de tareas.
      m4return_t AddParams (m4VariantType ai_oCode, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF);

      //## Operation: SetExecutionAccess%937902678
      //	Una vez que hemos cargado en canal de ejecución del JIT con la seguridad
      //	correspodiente tenemos que almacenar este valor en uno de los atributos de
      //	esta clase para ser utilizado este valor posteriormente. Este es el método
      //	que permite realizar esta operación.
      m4return_t SetExecutionAccess (ClAccess *ai_poAccess);

      //## Operation: GetNumberParameters%937996396
      m4uint32_t GetNumberParameters ();

      //## Operation: GetFirstOutParam%938420311
      //	Esta  clase se encarga de recuperar el primer parámetro de salida de la
      //	tarea en ejecución después de haber ejecutado el código jit y haberle
      //	asignado un valor de salida
      m4return_t GetFirstOutParam (M4ClString &ao_sParamName, M4ClString &ao_sParamType, M4ClString &ao_sParamValue);

      //## Operation: SetBox%941717918
      void SetBox (ClJSJob *ai_poBox);

      //## Operation: Initialize%941717919
      m4return_t Initialize ();

      //## Operation: RecoverCompoundJobParams%958997022
      //	Este método se encargará de recuperar y propagar los valores de entrada y
      //	salida de la tarea compuesta y grabarlos en base de datos.
      //
      //	Este método tiene dos parámetros, por defecto igual a 1. El primer parámetro
      //	nos indica si debemos grabar los parámetros de entrada. El segundo nos
      //	indica qué valor del parámetro de salida debemos grabar: null o el valor que
      //	tenga.
      m4return_t RecoverCompoundJobParams (m4uint32_t ai_bInParamValue = 1, m4uint32_t ai_bOutParamValue = 1);

    // Additional Public Declarations
      //## begin ClJSParametersHandlers%37E738CE01F0.public preserve=yes
      //## end ClJSParametersHandlers%37E738CE01F0.public

  protected:
    // Additional Protected Declarations
      //## begin ClJSParametersHandlers%37E738CE01F0.protected preserve=yes
      //## end ClJSParametersHandlers%37E738CE01F0.protected

  private:

    //## Other Operations (specified)
      //## Operation: RecoverParamOut%937902679
      //	Este es el método que dado un parámetro de salida se encarga de almacenarlo
      //	en el canal de ejecuciones del JIT
      m4return_t RecoverParamOut (m4pcchar_t ai_pcParamName, m4pcchar_t ai_pcParamType);

      //## Operation: RecoverParamIn%937902680
      //	Este es el método que se encarga de recuperar una parámetro de entrada de la
      //	ejecución de la tarea si este parámetro está redefinido para ese ejecución ,
      //	o de la definición de la tarea si no está redefinido  y almacenarlo en la
      //	pila de la máquina virtual para que sea utilizado en la ejecución del código
      //	JIT
      m4return_t RecoverParamIn (m4pcchar_t ai_pcParamName, m4pcchar_t ai_pcParamType, m4pcchar_t ai_pcParamValue);

      //## Operation: GetParamOut%937902681
      //	Este método se encarga de recuperar del canal de ejecuciones del JIT  el
      //	valor que se haya asignado al parámetro de salida cuyo nombre nos
      //	proporciona el método
      m4return_t GetParamOut (M4ClString &ao_sParamName, M4ClString &ao_sParamType, M4ClString &ao_sParamValue);

      //## Operation: StorageParamOut%937902682
      //	Método que se encarga de almacenar en el canal de ejecuciones el valor que
      //	se le ha asignado al parámetro de salida cuyo nombre referenciamos
      m4return_t StorageParamOut (M4ClString ai_sParamName, M4ClString ai_sParamValue, M4ClString ai_sParamType);

      //## Operation: GetParam%937902683
      void GetParam ();

    // Data Members for Class Attributes

      //## Attribute: m_uiNumberParameters%37E8C9660199
      //## begin ClJSParametersHandlers::m_uiNumberParameters%37E8C9660199.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_uiNumberParameters;
      //## end ClJSParametersHandlers::m_uiNumberParameters%37E8C9660199.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%37E7553201FB
      //## Role: ClJSParametersHandlers::m_poNodeJExec%37E75532035A
      //## begin ClJSParametersHandlers::m_poNodeJExec%37E75532035A.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeJExec;
      //## end ClJSParametersHandlers::m_poNodeJExec%37E75532035A.role

      //## Association: M4JSExeJob::<unnamed>%37E88AD401F1
      //## Role: ClJSParametersHandlers::m_poNodeParamJSJIT%37E88AD403D2
      //## begin ClJSParametersHandlers::m_poNodeParamJSJIT%37E88AD403D2.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poNodeParamJSJIT;
      //## end ClJSParametersHandlers::m_poNodeParamJSJIT%37E88AD403D2.role

      //## Association: M4JSExeJob::<unnamed>%37E761490179
      //## Role: ClJSParametersHandlers::m_poAccessExec%37E761490350
      //## begin ClJSParametersHandlers::m_poAccessExec%37E761490350.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poAccessExec;
      //## end ClJSParametersHandlers::m_poAccessExec%37E761490350.role

      //## Association: M4JSExeJob::<unnamed>%37E7618A02C7
      //## Role: ClJSParametersHandlers::m_poAccessJSJIT%37E7618B02E6
      //## begin ClJSParametersHandlers::m_poAccessJSJIT%37E7618B02E6.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poAccessJSJIT;
      //## end ClJSParametersHandlers::m_poAccessJSJIT%37E7618B02E6.role

      //## Association: M4JSExeJob::<unnamed>%382172210304
      //## Role: ClJSParametersHandlers::m_poBox%3821722200C1
      //## begin ClJSParametersHandlers::m_poBox%3821722200C1.role preserve=no  private: ClJSJob { -> RFHAN}
      ClJSJob *m_poBox;
      //## end ClJSParametersHandlers::m_poBox%3821722200C1.role

    // Additional Private Declarations
      //## begin ClJSParametersHandlers%37E738CE01F0.private preserve=yes
      //## end ClJSParametersHandlers%37E738CE01F0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSParametersHandlers%37E738CE01F0.implementation preserve=yes
      //## end ClJSParametersHandlers%37E738CE01F0.implementation

};

//## begin ClJSParametersHandlers%37E738CE01F0.postscript preserve=yes
//## end ClJSParametersHandlers%37E738CE01F0.postscript

// Class ClJSParametersHandlers 

//## begin module%37E7419B0253.epilog preserve=yes
//## end module%37E7419B0253.epilog


#endif
