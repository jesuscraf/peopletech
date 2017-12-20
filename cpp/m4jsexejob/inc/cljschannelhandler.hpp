//## begin module%380F4A100234.cm preserve=no
//## end module%380F4A100234.cm

//## begin module%380F4A100234.cp preserve=no
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
//## end module%380F4A100234.cp

//## Module: ClJSChannelHandler%380F4A100234; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: D:\Fuentes\m4jsexejob\inc\cljschannelhandler.hpp

#ifndef cljschannelhandler_h
#define cljschannelhandler_h 1

//## begin module%380F4A100234.additionalIncludes preserve=no
//## end module%380F4A100234.additionalIncludes

//## begin module%380F4A100234.includes preserve=yes
#include <m4var.hpp>
#include "m4string.hpp"
//## end module%380F4A100234.includes

// boolcondition
#include <boolcondition.hpp>

class ClStatus;
class ClM4ObjService;
class ClNode;
class ClAccess;
class ClChannel;
class ClChannelManager;

typedef list< m4VariantType * > ClListParameterVariant;


//## begin module%380F4A100234.declarations preserve=no
//## end module%380F4A100234.declarations

//## begin module%380F4A100234.additionalDeclarations preserve=yes
//## end module%380F4A100234.additionalDeclarations


//## begin ClJSChannelHandler%380F3439039D.preface preserve=yes
//## end ClJSChannelHandler%380F3439039D.preface

//## Class: ClJSChannelHandler%380F3439039D
//	Esta es la clase encargada de manejar canales.Proporcionará al ejecutor
//	todas las operaciones que necesite realizar con canales:
//
//	-Crear canales
//	-Cargar canales
//	-Crear nodos
//	-Almacenar datos  en el canal
//	-Obtener datos del canal
//	-Persistir canales
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%380F435A002B;ClNode { -> F}
//## Uses: <unnamed>%380F438C03E4;ClAccess { -> F}
//## Uses: <unnamed>%380F43A4035C;ClChannel { -> F}
//## Uses: <unnamed>%381037BB0137;ClListParameterVariant { -> F}

class ClJSChannelHandler 
{
  //## begin ClJSChannelHandler%380F3439039D.initialDeclarations preserve=yes
  //## end ClJSChannelHandler%380F3439039D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSChannelHandler%940526264
      ClJSChannelHandler ();

    //## Destructor (specified)
      //## Operation: ~ClJSChannelHandler%940526265
      ~ClJSChannelHandler ();


    //## Other Operations (specified)
      //## Operation: BuildEnvToChannels%940526266
      //	Construye el entorno que necesitamos para poder trabajar con los Meta4Object
      //	. Con el construimo el enviroment y el channel manager
      m4return_t BuildEnvToChannels (ClM4ObjService*& aio_pM4ObjService, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pMapCache, ClCache *ai_pCacheData);

      //## Operation: BuildChannel%940526267
      //	Construye el canal cuyo nombre le indicamos como parámetro y nos devuelve el
      //	acceso al canal y el puntero al propio canal
      virtual m4return_t BuildChannel (m4pcchar_t ai_pccChannel, ClChannel *&ao_poChannel, ClAccess *&ao_poAccess) = 0;

      //## Operation: LoadChannel%940526268
      //	Carga el canal cuyo puntero le pasamos como parámetro
      m4return_t LoadChannel (m4pcchar_t ai_pccNodeRoot, ClChannel *ai_poChannel, ClAccess *ai_poAccess, ClNode * &ao_poNode,  ClListParameterVariant &ai_oParameterFilter);

      //## Operation: ReleaseChannel%940526272
      //	Descarga el canal que le pasamos como parámetro de los datos que tuviera
      //	cargado
      m4return_t ReleaseChannel (ClChannel *ai_poChannel);

      //## Operation: ExecuteMethod%940526270
      //	Ejecuta el método del canal que le pasamos como parámetro .El nombre del
      //	método que queremos ejecutar también se le pasa como parámetro
      m4return_t ExecuteMethod (m4pcchar_t ai_pccMethod, ClListParameterVariant ai_oParameterMethod, ClNode *ai_poNode, ClAccess *ai_poAccess, ClListParameterVariant &ao_oListParameterOut, m4uint32_t ai_uiNumParamRef = 0);

      //## Operation: DestroyChannel%940526273
      //	Destruye el acceso al canal y el propio canal y borra los punteros asociados
      //	tanto al acceso como al canal
      m4return_t DestroyChannel (ClAccess* &aio_poAccess, ClChannel* &aio_poChannel);

      //## Operation: DestroyEnvToChannels%940526274
      //	Destruye el entorno de trabajo con los canales , es decir , el enviroment  y
      //	el channel manager
      m4return_t DestroyEnvToChannels ();

      //## Operation: PersistChannel%940526275
      //	Graba los datos que hubieramos almacenado en el canal cuyo puntero le
      //	pasamos en el parámetro hasta ese momento
      m4return_t PersistChannel (ClChannel *ai_poChannel);

      //## Operation: GetObjService%940526276
      //	Obtiene el entono de trabajo de los canales
      m4return_t GetObjService (ClM4ObjService*& aio_pObjService);

      //## Operation: GetChannelManager%940526278
      //	Obtiene el  channel manager
      ClChannelManager* GetChannelManager (void);

      //## Operation: ResetMVC%940526280
      //	Desactiva el time out que establecimos en la MVC  y deternina si ha expirado
      //	el time out o si se ha pedido una cancelación de la ejecución
      m4return_t ResetMVC (m4bool_t &ao_bIsCancel, m4bool_t &ao_bIsTimeOut);

      //## Operation: GetData%940526282
      //	Obtiene el dato almacenado  en un campo de un nodo de un canal
      m4return_t GetData (ClNode *ai_poNode, m4pcchar_t ai_pccData, m4VariantType &ao_oData, const m4VariantType *ai_pvDefault );

      //## Operation: SetData%940526283
      //	Modifica el dato almacenado  en un campo de un nodo de un canal.
      //
      //	Devuelve un M4_WARNING si el  valor que recuperamos de la base de datos es
      //	nulo.
      m4return_t SetData (ClNode *ai_poNode, m4pcchar_t ai_pccData, const m4VariantType &ai_roData);

      //## Operation: SetTimeOutInMVC%940526281
      //	Activa el time out de la máquina virtual con el tiempo que le pasemos como
      //	parámetro
      m4return_t SetTimeOutInMVC (m4uint32_t ai_uiTimeOut);

      //## Operation: GetNode%940526269
      //	Obtiene el nodo cuyo nombre le pasamos como parámetro del canal cuyo puntero
      //	le pasamos también como parámetro
      m4return_t GetNode (m4pcchar_t ai_pccNode, ClAccess *ai_poAccess, ClNode* &ao_poNode);

      //## Operation: ReloadChannelExecution%940526271
      //	Descarga el canal que tuvieramos cargado y lo vuelve a cargar con los
      //	nuevos datos que se hubieran creado desde la carga anterior.Realmente con
      //	este método conseguimos refrescar los datos cargados
      m4return_t ReloadChannelExecution (ClChannel *ai_poChannel, ClAccess *ai_poAccess, m4pcchar_t ai_pccNodeRoot, ClNode *ao_poNode, m4pcchar_t ai_pccNodeChild, ClListParameterVariant ai_oParameterToFilter, ClNode *ao_poNodeChild);

      //## Operation: CreateNewData%941010642
      m4return_t CreateNewData (ClNode * &ai_poNode);

      //## Operation: IsEndedNode%941096540
      //	Esta función nos indica si estamos situado al final del nodo o no
      m4bool_t IsEndedNode (ClNode *ai_poNode);

      //## Operation: GetNextData%941096541
      //	Este método nos permite avanzar una posición en el nodo
      m4return_t GetNextData (ClNode * &ai_poNode);

      //## Operation: ExecuteCodeLN4%941541646
      //	Método que invoca la función que ejecuta el método LN4 en modo JIT
      m4return_t ExecuteCodeLN4 (ClNode *ai_poNode, ClAccess *ai_poAccess, ClListParameterVariant &ao_oListParameter, m4uint32_t ai_uiNumberParameter);

      //## Operation: SetConnection%941807672
      m4return_t SetConnection ();

      //## Operation: AbortMVC%942683108
      //	Este método es el encargado de abortar la  ejecución que tenga lugar en la
      //	máquina virtual
      m4return_t AbortMVC ();

      //## Operation: GetUsedMemory%951385117
      m4uint32_t GetUsedMemory ();

    // Additional Public Declarations
      //## begin ClJSChannelHandler%380F3439039D.public preserve=yes
      //## end ClJSChannelHandler%380F3439039D.public

  protected:

    //## Other Operations (specified)
      //## Operation: PushStackMVC%940526284
      //	Almacena un dato en la cola  de máquina virtual
      m4return_t PushStackMVC (ClAccess *ai_poAccess, m4VariantType ai_poData);

      //## Operation: PopStackMVC%940526285
      //	Obtiene un dato de la pila de máquina virtual
      m4return_t PopStackMVC (ClAccess *ai_poAccess, m4VariantType &ao_oParam);

    // Data Members for Class Attributes

      //## Attribute: m_uiTimeOut%38BC164C0051
      //## begin ClJSChannelHandler::m_uiTimeOut%38BC164C0051.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiTimeOut;
      //## end ClJSChannelHandler::m_uiTimeOut%38BC164C0051.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%380F42E600D8
      //## Role: ClJSChannelHandler::m_poChannelManager%380F42E6026A
      //## begin ClJSChannelHandler::m_poChannelManager%380F42E6026A.role preserve=no  protected: ClChannelManager { -> RFHAN}
      ClChannelManager *m_poChannelManager;
      //## end ClJSChannelHandler::m_poChannelManager%380F42E6026A.role

      //## Association: M4JSExeJob::<unnamed>%38141D350375
      //## Role: ClJSChannelHandler::m_poObjService%38141D360146
      //## begin ClJSChannelHandler::m_poObjService%38141D360146.role preserve=no  protected: ClM4ObjService { -> RFHAN}
      ClM4ObjService *m_poObjService;
      //## end ClJSChannelHandler::m_poObjService%38141D360146.role

      //## Association: M4JSExeJob::<unnamed>%38141DF00125
      //## Role: ClJSChannelHandler::m_poStatusTimeOut%38141DF00338
      //## begin ClJSChannelHandler::m_poStatusTimeOut%38141DF00338.role preserve=no  protected: ClStatus { -> RFHAN}
      ClStatus *m_poStatusTimeOut;
      //## end ClJSChannelHandler::m_poStatusTimeOut%38141DF00338.role

      //## Association: M4JSExeJob::<unnamed>%38B517CE0262
      //## Role: ClJSChannelHandler::m_oBCStatus%38B517D0012F
      //## begin ClJSChannelHandler::m_oBCStatus%38B517D0012F.role preserve=no  protected: ClBooleanCondition { -> VHAN}
      ClBooleanCondition m_oBCStatus;
      //## end ClJSChannelHandler::m_oBCStatus%38B517D0012F.role

    // Additional Protected Declarations
      //## begin ClJSChannelHandler%380F3439039D.protected preserve=yes
      //## end ClJSChannelHandler%380F3439039D.protected

  private:
    // Additional Private Declarations
      //## begin ClJSChannelHandler%380F3439039D.private preserve=yes
      //## end ClJSChannelHandler%380F3439039D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSChannelHandler%380F3439039D.implementation preserve=yes
      //## end ClJSChannelHandler%380F3439039D.implementation

};

//## begin ClJSChannelHandler%380F3439039D.postscript preserve=yes
//## end ClJSChannelHandler%380F3439039D.postscript

//## begin ClListParameterVariant%381035570137.preface preserve=yes
//## end ClListParameterVariant%381035570137.preface

//## Class: ClListParameterVariant%381035570137; Instantiated Class
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef list< m4VariantType * > ClListParameterVariant;

//## begin ClListParameterVariant%381035570137.postscript preserve=yes
//## end ClListParameterVariant%381035570137.postscript

// Class ClJSChannelHandler 

//## begin module%380F4A100234.epilog preserve=yes
//## end module%380F4A100234.epilog


#endif
