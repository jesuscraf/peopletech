//## begin module%34B4B5B80039.cm preserve=no
//## end module%34B4B5B80039.cm

//## begin module%34B4B5B80039.cp preserve=no
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
//## end module%34B4B5B80039.cp

//## Module: ClCache%34B4B5B80039; Package specification
//## Subsystem: M4Cache::inc%3796F4510086
//## Source file: D:\Work\v500\M4Cache\inc\clcache.hpp

#ifndef __CLCACHE__HPP__
#define __CLCACHE__HPP__ 1

//## begin module%34B4B5B80039.additionalIncludes preserve=no
//## end module%34B4B5B80039.additionalIncludes

//## begin module%34B4B5B80039.includes preserve=yes
#include "m4cache_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4date.hpp"
#include "m4cotypes.hpp"

#define NO_PERSIST        0
#define SERIALIZABLE      1
#define SELF_PERSISTIABLE 2

#define IOD_ONLY     0
#define IOD_AND_DISK 1
#define DISK_ONLY    2
//## end module%34B4B5B80039.includes

// syncro
#include <syncro.hpp>
// dumpcache
#include <dumpcache.hpp>
// ClSelfPersistiableObject
#include <clprsstb.hpp>
// ClSerializableObject
#include <clsrlzbl.hpp>

class ClCacheableObject;
class ClCacheKey;
class ClCacheableObjectFactory;

//## begin module%34B4B5B80039.declarations preserve=no
//## end module%34B4B5B80039.declarations

//## begin module%34B4B5B80039.additionalDeclarations preserve=yes
//## end module%34B4B5B80039.additionalDeclarations


//## begin ClCacheKey%3496E226038A.preface preserve=yes
//## end ClCacheKey%3496E226038A.preface

//## Class: ClCacheKey%3496E226038A
//	Clave asociada al objeto cacheable
//
//	 m_pszKey: Puntero a la clave
//	 m_uiSize: Tamaño de la clave
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE ClCacheKey 
{
  //## begin ClCacheKey%3496E226038A.initialDeclarations preserve=yes
  //## end ClCacheKey%3496E226038A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheKey%886420471
      ClCacheKey (m4pchar_t ai_pszKey = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCacheKey%892796790
      ~ClCacheKey ();


    //## Other Operations (specified)
      //## Operation: Serialize%893688336
      virtual m4return_t Serialize (ClGenericIODriver &  IOD);

      //## Operation: DeSerialize%893688337
      virtual m4return_t  DeSerialize (ClGenericIODriver &  IOD);

      //## Operation: Getm_pszKey%894362626
      m4pchar_t Getm_pszKey ();

      //## Operation: Dump%917950244
      virtual m4return_t Dump (ClDumpCacheVisitor &ai_oClDumpCacheVisitor);

    // Additional Public Declarations
      //## begin ClCacheKey%3496E226038A.public preserve=yes
      //## end ClCacheKey%3496E226038A.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheKey%3496E226038A.protected preserve=yes
      //## end ClCacheKey%3496E226038A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pszKey%3496E544034B
      //## begin ClCacheKey::m_pszKey%3496E544034B.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pszKey;
      //## end ClCacheKey::m_pszKey%3496E544034B.attr

      //## Attribute: m_uiSize%3496E544035B
      //## begin ClCacheKey::m_uiSize%3496E544035B.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_uiSize;
      //## end ClCacheKey::m_uiSize%3496E544035B.attr

    // Additional Private Declarations
      //## begin ClCacheKey%3496E226038A.private preserve=yes
      //## end ClCacheKey%3496E226038A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheKey%3496E226038A.implementation preserve=yes
      //## end ClCacheKey%3496E226038A.implementation

  //## begin ClCacheKey%3496E226038A.friends preserve=no
    friend class ClCacheKeyLess;
  //## end ClCacheKey%3496E226038A.friends
};

//## begin ClCacheKey%3496E226038A.postscript preserve=yes
//## end ClCacheKey%3496E226038A.postscript

//## begin ClCacheInterface%3496E1DD00AB.preface preserve=yes
//## end ClCacheInterface%3496E1DD00AB.preface

//## Class: ClCacheInterface%3496E1DD00AB
//	 Interface entre el usuario ClCacheTest y la cache ClCacheTree
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34D5A9BF001F;ClCacheKey { -> }
//## Uses: <unnamed>%34E4606E030C;ClCacheableObject { -> F}

class  M4_DECL_M4CACHE ClCacheInterface 
{
  //## begin ClCacheInterface%3496E1DD00AB.initialDeclarations preserve=yes
  //## end ClCacheInterface%3496E1DD00AB.initialDeclarations

  public:
    //## Class: eGetMode%3536348101B3
    //## Category: M4Cache%3496B4B10159
    //## Subsystem: M4Cache::inc%3796F4510086
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {GET_SHARED=0,GET_EXNBLOCK,GET_EXBLOCK} eGetMode;

    //## Constructors (specified)
      //## Operation: ClCacheInterface%896947319
      ClCacheInterface ();


    //## Other Operations (specified)
      //## Operation: GetObject%886420472; C++
      //	Permite obtener un puntero a objeto de la cache. Si el objeto no existe en
      //	la cache o se obtiene un puntero a NULL devuelve M4_ERROR.
      //
      //	Si el puntero a objeto buscado existe se incrementa el atributo de Cl
      //	CacheableObject m_iRefCounter para indicar que existe un usuario más del
      //	objeto.
      //
      //	Argumentos:
      //
      //	ClCacheKey * ai_pKey : Puntero a la clave asociada al objeto.
      //	ClCacheableObject * ao_pObject: Argumento de salida para el objeto buscado.
      //
      //	m4uint32_t ai_iTimeOut: Tiempo límite de búsqueda en la cache.
      virtual m4return_t GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode = GET_SHARED, m4int32_t  ai_iTimeOut = -1) = 0;

      //## Operation: PutObject%886420473
      //	Permite poner un objeto en la cache.
      //
      //	Puede ser usada para devolver un objeto a la cache después de ser usado,
      //	poner un puntero nulo ( éste será usado en la política de bloqueo), o para
      //	colocar un objeto por primera vez.
      //	 En los dos últimos casos no se decrementa m_iRefCounter.
      //
      //	Argumento:
      //
      //	ClCachebleObject * ai_pObject: objeto que se quiere colocar en la cache.
      virtual m4return_t PutObject (ClCacheableObject *ai_pObject);

      //## Operation: NoUpdateObject%894362622
      virtual m4return_t NoUpdateObject (ClCacheableObject *  ai_pObject);

      //## Operation: UpdateObject%886420475; C++
      //	Tiene la misma funcionalidad que PutObject pero nunca decrementa el número
      //	de referencias m_iRefCounter.
      //
      //	Se utiliza para actualizar un objeto en la cache
      virtual m4return_t UpdateObject (ClCacheableObject *ai_pObject);

      //## Operation: GetObjectCopy%886420474; C++
      virtual m4return_t GetObjectCopy (ClCacheKey *ai_pKey, ClCacheableObject *&ao_pObject);

      //## Operation: UpdateObjectKey%892745128
      virtual m4return_t UpdateObjectKey (ClCacheableObject *ai_poClCacheableObject, ClCacheKey *ai_poClCacheKey) = 0;

      //## Operation: RemoveAll%886420483; C++
      virtual m4return_t RemoveAll ();

      //## Operation: RemoveObject%886420476; C++
      //	Pasa un objeto no válido de la cache a la cola de borrado
      //
      //	Argumento:
      //
      //	ClCacheableObject * ai_pObject: Objeto que quiere ser pasado a la cola de
      //	borrado
      virtual m4return_t RemoveObject (ClCacheableObject *ai_pObject);

      //## Operation: SetCOFactory%893835153
      virtual m4return_t SetCOFactory ( ClCacheableObjectFactory *  ai_poClCOFactory) = 0;

      //## Operation: GetCOFactory%893835154
      virtual ClCacheableObjectFactory *  GetCOFactory () = 0;

      //## Operation: Recycle%894985403
      virtual m4return_t Recycle () = 0;

      //## Operation: RemoveAll%895824029
      virtual m4return_t RemoveAll (M4ClTimeStamp *  ai_poLimitDate) = 0;

      //## Operation: RemoveAll%895824030
      virtual m4return_t RemoveAll (m4pchar_t ai_pcPortionKey, m4puint32_t ai_piRemovedCount = NULL) = 0;

      //## Operation: Refresh%897316884
      virtual m4return_t Refresh (ClSelfPersistiableObject *m_poSelfPersistor);

      //## Operation: GetNext%910956608
      virtual m4return_t GetNext (ClCacheableObject * &  ao_poClCacheableObject, void * &  ai_poIterator) = 0;

      //## Operation: SetMaxNOfObjs%910956609
      virtual m4return_t SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs) = 0;

      //## Operation: SetMaxSizeCache%910956610
      virtual m4return_t SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache) = 0;

      //## Operation: RemoveByCondition%943372026
      virtual m4return_t  RemoveByCondition (void *  ai_pCondition) = 0;

      //## Operation: ReplaceObject%945346492; C++
      //	Tiene la misma funcionalidad que PutObject pero nunca decrementa el número
      //	de referencias m_iRefCounter.
      //
      //	Se utiliza para actualizar un objeto en la cache
      virtual m4return_t ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject);

      //## Operation: GetMutex%1003764700
      virtual ClMutex & GetMutex () = 0;

  public:
    // Additional Public Declarations
      //## begin ClCacheInterface%3496E1DD00AB.public preserve=yes
      //## end ClCacheInterface%3496E1DD00AB.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheInterface%3496E1DD00AB.protected preserve=yes
      //## end ClCacheInterface%3496E1DD00AB.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheInterface%3496E1DD00AB.private preserve=yes
      //## end ClCacheInterface%3496E1DD00AB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheInterface%3496E1DD00AB.implementation preserve=yes
      //## end ClCacheInterface%3496E1DD00AB.implementation

};

//## begin ClCacheInterface%3496E1DD00AB.postscript preserve=yes
//## end ClCacheInterface%3496E1DD00AB.postscript

//## begin ClCachePolicy%3496E28B02DE.preface preserve=yes
//## end ClCachePolicy%3496E28B02DE.preface

//## Class: ClCachePolicy%3496E28B02DE
//	Politica del objeto cacheable:
//	 -m_oStartDate: Fecha de inicio de validez del objeto
//	 -m_oEndDate: Fecha de fin de validez
//	 -m_oEndUnusedDate: Fecha de fin de validez por no utilización del objeto.
//	 -m_dMaxUnusedPeriod: Periodo (en dias) de máxima valided de un objeto desde
//	la última vez que se utilizó.
//
//	Funcion IsValid(): Permite evaluar si el objeto cacheado es valido. (Evalúa
//	su política)
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE ClCachePolicy 
{
  //## begin ClCachePolicy%3496E28B02DE.initialDeclarations preserve=yes
  //## end ClCachePolicy%3496E28B02DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCachePolicy%886420470
      ClCachePolicy (m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

      //## Operation: ClCachePolicy%959345793
      ClCachePolicy ();

    //## Destructor (specified)
      //## Operation: ~ClCachePolicy%888398554
      ~ClCachePolicy ();


    //## Other Operations (specified)
      //## Operation: IsValid%888398555
      m4bool_t IsValid (M4ClTimeStamp &  ai_oActualTime);

      //## Operation: Serialize%893835147
      m4return_t Serialize (ClGenericIODriver &  IOD);

      //## Operation: DeSerialize%893835148
      m4return_t DeSerialize (ClGenericIODriver &  IOD);

      //## Operation: Dump%917950246
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_pClDumpCacheVisitor);

      //## Operation: IsValidUnusedPeriod%959345794
      m4bool_t IsValidUnusedPeriod (M4ClTimeStamp &ai_oActualTime);

      //## Operation: IsValidMaxDate%959345795
      m4bool_t IsValidMaxDate (M4ClTimeStamp &ai_oActualTime);

      //## Operation: ActivateUnusedPeriod%959345796
      m4return_t ActivateUnusedPeriod ();

      //## Operation: DesactivateUnusedPeriod%959345797
      m4return_t DesactivateUnusedPeriod ();

    // Additional Public Declarations
      //## begin ClCachePolicy%3496E28B02DE.public preserve=yes
      //## end ClCachePolicy%3496E28B02DE.public

  protected:
    // Additional Protected Declarations
      //## begin ClCachePolicy%3496E28B02DE.protected preserve=yes
      //## end ClCachePolicy%3496E28B02DE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oStartDate%3496E63E004E
      //## begin ClCachePolicy::m_oStartDate%3496E63E004E.attr preserve=no  private: M4ClTimeStamp  {UA} 
      M4ClTimeStamp  m_oStartDate;
      //## end ClCachePolicy::m_oStartDate%3496E63E004E.attr

      //## Attribute: m_oEndDate%3496E6580222
      //## begin ClCachePolicy::m_oEndDate%3496E6580222.attr preserve=no  private: M4ClTimeStamp  {UA} 
      M4ClTimeStamp  m_oEndDate;
      //## end ClCachePolicy::m_oEndDate%3496E6580222.attr

      //## Attribute: m_oUnusedEndDate%3496E6590177
      //## begin ClCachePolicy::m_oUnusedEndDate%3496E6590177.attr preserve=no  private: M4ClTimeStamp  {UA} 
      M4ClTimeStamp  m_oUnusedEndDate;
      //## end ClCachePolicy::m_oUnusedEndDate%3496E6590177.attr

      //## Attribute: m_dMaxUnusedPeriod%3496E65A0177
      //## begin ClCachePolicy::m_dMaxUnusedPeriod%3496E65A0177.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_dMaxUnusedPeriod;
      //## end ClCachePolicy::m_dMaxUnusedPeriod%3496E65A0177.attr

    // Additional Private Declarations
      //## begin ClCachePolicy%3496E28B02DE.private preserve=yes
      //## end ClCachePolicy%3496E28B02DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCachePolicy%3496E28B02DE.implementation preserve=yes
      //## end ClCachePolicy%3496E28B02DE.implementation

};

//## begin ClCachePolicy%3496E28B02DE.postscript preserve=yes
//## end ClCachePolicy%3496E28B02DE.postscript

//## begin ClCacheStyle%34D5AA5B0218.preface preserve=yes
//## end ClCacheStyle%34D5AA5B0218.preface

//## Class: ClCacheStyle%34D5AA5B0218
//	Clase que representa las operaciones a nivel general de la cache
//
//	El constructor permite asignar una  política por defecto a la cache
//	inicializando el puntero m_poDefaultPolicy
//
//	IncrNumOfRef: función amiga de la clase ClCacheableObject que permite
//	incrementar en una el numero de referencias.
//
//	DecrNumOfRef: función amiga de la clase ClCacheableObject que permite
//	decrementar en una el numero de referencias.
//
//	SetDefaultPolicy: Permite actualizar la política por defecto de la cache
//	asignandosela a m_poDefaultPolicy.
//
//
//	GetDefaultPolicy: Devuelve la politica por defecto de la cache con el
//	puntero m_poDefaultPolicy
//
//	Recycle: Permite  recorrer la cache con una variable tipo iterador para
//	evaluar si existe algun objeto obsoleto con la función IsValid(). Si es así,
//	se elimina de la cache y se pasa a la cola de borrado con la función Remove
//	Object(). Después evalúa la cola de borrado. Si existe algún objeto en ella
//	que tiene su m_iRefCounter a cero lo elimina.
//
//	Shutdown():Permite cerrar la caché
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE ClCacheStyle : public ClCacheInterface  //## Inherits: <unnamed>%34D5AB1000D8
{
  //## begin ClCacheStyle%34D5AA5B0218.initialDeclarations preserve=yes
  //## end ClCacheStyle%34D5AA5B0218.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheStyle%886420478
      ClCacheStyle (m4double_t ai_dMaxPeriod = 0, m4double_t ai_dMaxUnusedPeriod = 0, m4uint32_t ai_ulMaxSizeCache = 10000000, m4uint32_t ai_ulMaxNOfObjs = 1000, m4uint32_t ai_ulRefreshRatio = 50);

    //## Destructor (specified)
      //## Operation: ~ClCacheStyle%886420485
      virtual ~ClCacheStyle ();


    //## Other Operations (specified)
      //## Operation: Recycle%886420486
      virtual m4return_t Recycle () = 0;

      //## Operation: Lock%886497414
      virtual m4return_t Lock ();

      //## Operation: Unlock%886497415
      virtual m4return_t Unlock ();

      //## Operation: Transfer%886497413
      virtual m4return_t Transfer (ClCacheStyle *ai_poClCacheStyle);

      //## Operation: SetMaxPeriod%888398559
      m4return_t SetMaxPeriod (m4double_t ai_dMaxPeriod);

      //## Operation: GetMaxPeriod%888398560
      virtual m4double_t GetMaxPeriod ();

      //## Operation: SetMaxUnusedPeriod%960820227
      m4return_t SetMaxUnusedPeriod (m4double_t ai_dMaxUnusedPeriod);

      //## Operation: GetMaxUnusedPeriod%959587676
      virtual m4double_t GetMaxUnusedPeriod ();

      //## Operation: GetCacheStats%889461538
      void  *  GetCacheStats ();

      //## Operation: SetCacheStats%893330111
      m4return_t SetCacheStats (void *   ai_poClCacheStats);

      //## Operation: GetNOfRef%889694501
      m4uint32_t GetNOfRef ();

      //## Operation: GetNObjInQueue%889694502
      m4uint32_t GetNObjInQueue ();

      //## Operation: GetNObjInCache%889694503
      m4uint32_t GetNObjInCache ();

      //## Operation: GetHitsRatio%889863962
      m4float_t GetHitsRatio ();

      //## Operation: Serialize%893751386
      virtual m4return_t Serialize (ClGenericIODriver &  IOD);

      //## Operation: DeSerialize%893751387
      virtual m4return_t DeSerialize (ClGenericIODriver &  IOD, m4pchar_t ai_pcEnvId);

      //## Operation: GetSizeCache%895680864
      m4uint32_t GetSizeCache ();

      //## Operation: GetMaxNOfObjs%897498322
      m4uint32_t GetMaxNOfObjs ();

      //## Operation: GetMaxSizeCache%897498323
      m4uint32_t GetMaxSizeCache ();

      //## Operation: GetRefreshRatio%897498324
      m4uint32_t GetRefreshRatio ();

      //## Operation: SetMaxSizeCache%897553670
      m4return_t SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache);

      //## Operation: SetMaxNOfObjs%897553671
      m4return_t SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs);

      //## Operation: SetRefreshRatio%897553672
      m4return_t SetRefreshRatio (m4uint32_t ai_ulRefreshRatio);

      //## Operation: GetNOfObjInLRU%910956611
      m4uint32_t GetNOfObjInLRU ();

      //## Operation: Dump%917950248
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor);

      //## Operation: SetCacheName%943372019
      m4return_t SetCacheName (m4pchar_t ai_pcCacheName);

      //## Operation: GetMutex%1003764699
      virtual ClMutex & GetMutex ();

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%34F6DFDB0388
      //## Role: ClCacheStyle::m_oMutexCounter%34F6DFDD0100
      //## begin ClCacheStyle::m_oMutexCounter%34F6DFDD0100.role preserve=no  public: ClMutex { -> VHAN}
      ClMutex m_oMutexCounter;
      //## end ClCacheStyle::m_oMutexCounter%34F6DFDD0100.role

      //## Association: M4Cache::<unnamed>%3795EB9D038A
      //## Role: ClCacheStyle::m_oMutex%3795EBA20214
      //## begin ClCacheStyle::m_oMutex%3795EBA20214.role preserve=no  public: ClMutex { -> VHAN}
      ClMutex m_oMutex;
      //## end ClCacheStyle::m_oMutex%3795EBA20214.role

    // Additional Public Declarations
      //## begin ClCacheStyle%34D5AA5B0218.public preserve=yes
      //## end ClCacheStyle%34D5AA5B0218.public

  protected:

    //## Other Operations (specified)
      //## Operation: RemoveReference%886501490
      virtual m4return_t RemoveReference (ClCacheableObject *ai_poClCacheableObject);

  public:      //## Operation: IncrNumOfRef%886509528
      m4uint32_t IncrNumOfRef (ClCacheableObject *aio_poClCacheableObject);

      //## Operation: DecrNumOfRef%886509529
      m4uint32_t DecrNumOfRef (ClCacheableObject *aio_poClCacheableObject);

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulNumOfObjInCache%350419EE030B
      //## begin ClCacheStyle::m_ulNumOfObjInCache%350419EE030B.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulNumOfObjInCache;
      //## end ClCacheStyle::m_ulNumOfObjInCache%350419EE030B.attr

      //## Attribute: m_ulNumOfObjInDelQueue%35041A1003AA
      //## begin ClCacheStyle::m_ulNumOfObjInDelQueue%35041A1003AA.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulNumOfObjInDelQueue;
      //## end ClCacheStyle::m_ulNumOfObjInDelQueue%35041A1003AA.attr

      //## Attribute: m_ulTotalNumOfRef%35041A2D0293
      //## begin ClCacheStyle::m_ulTotalNumOfRef%35041A2D0293.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulTotalNumOfRef;
      //## end ClCacheStyle::m_ulTotalNumOfRef%35041A2D0293.attr

      //## Attribute: m_oHitsRatioCache%350A48B602B3
      //## begin ClCacheStyle::m_oHitsRatioCache%350A48B602B3.attr preserve=no  protected: m4float_t {UA} 0
      m4float_t m_oHitsRatioCache;
      //## end ClCacheStyle::m_oHitsRatioCache%350A48B602B3.attr

      //## Attribute: m_uiTotalHitsCache%350A62380078
      //## begin ClCacheStyle::m_uiTotalHitsCache%350A62380078.attr preserve=no  protected: m4float_t {UA} 0
      m4float_t m_uiTotalHitsCache;
      //## end ClCacheStyle::m_uiTotalHitsCache%350A62380078.attr

      //## Attribute: m_uiTotalAccessCache%350A626603DB
      //## begin ClCacheStyle::m_uiTotalAccessCache%350A626603DB.attr preserve=no  protected: m4float_t {UA} 0
      m4float_t m_uiTotalAccessCache;
      //## end ClCacheStyle::m_uiTotalAccessCache%350A626603DB.attr

      //## Attribute: m_ulSizeCache%354EE9430205
      //## begin ClCacheStyle::m_ulSizeCache%354EE9430205.attr preserve=no  protected: m4uint32_t  {UA} 
      m4uint32_t  m_ulSizeCache;
      //## end ClCacheStyle::m_ulSizeCache%354EE9430205.attr

      //## Attribute: m_ulMaxSizeCache%35509E61017F
      //## begin ClCacheStyle::m_ulMaxSizeCache%35509E61017F.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulMaxSizeCache;
      //## end ClCacheStyle::m_ulMaxSizeCache%35509E61017F.attr

      //## Attribute: m_ulMaxNOfObjects%35509E7C034A
      //## begin ClCacheStyle::m_ulMaxNOfObjects%35509E7C034A.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulMaxNOfObjects;
      //## end ClCacheStyle::m_ulMaxNOfObjects%35509E7C034A.attr

      //## Attribute: m_ulDelRatioMem%3562F76E0250
      //## begin ClCacheStyle::m_ulDelRatioMem%3562F76E0250.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulDelRatioMem;
      //## end ClCacheStyle::m_ulDelRatioMem%3562F76E0250.attr

      //## Attribute: m_ulNumOfUpdateObjs%3574254603C7
      //## begin ClCacheStyle::m_ulNumOfUpdateObjs%3574254603C7.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulNumOfUpdateObjs;
      //## end ClCacheStyle::m_ulNumOfUpdateObjs%3574254603C7.attr

      //## Attribute: m_ulDelRatioObjs%357429970037
      //## begin ClCacheStyle::m_ulDelRatioObjs%357429970037.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulDelRatioObjs;
      //## end ClCacheStyle::m_ulDelRatioObjs%357429970037.attr

      //## Attribute: m_ulRefreshRatio%3576728A0285
      //## begin ClCacheStyle::m_ulRefreshRatio%3576728A0285.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulRefreshRatio;
      //## end ClCacheStyle::m_ulRefreshRatio%3576728A0285.attr

      //## Attribute: m_ulNumOfObjInLRU%364C26CE0178
      //## begin ClCacheStyle::m_ulNumOfObjInLRU%364C26CE0178.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulNumOfObjInLRU;
      //## end ClCacheStyle::m_ulNumOfObjInLRU%364C26CE0178.attr

      //## Attribute: m_pcCacheName%383BB64D0385
      //## begin ClCacheStyle::m_pcCacheName%383BB64D0385.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcCacheName;
      //## end ClCacheStyle::m_pcCacheName%383BB64D0385.attr

      //## Attribute:  m_ulNumOfObjectsDisabled%383BC0CD0249
      //## begin ClCacheStyle:: m_ulNumOfObjectsDisabled%383BC0CD0249.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_ulNumOfObjectsDisabled;
      //## end ClCacheStyle:: m_ulNumOfObjectsDisabled%383BC0CD0249.attr

      //## Attribute: m_ulNumOfObjInDisk%39E1E0690336
      //## begin ClCacheStyle::m_ulNumOfObjInDisk%39E1E0690336.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulNumOfObjInDisk;
      //## end ClCacheStyle::m_ulNumOfObjInDisk%39E1E0690336.attr

    // Additional Protected Declarations
      //## begin ClCacheStyle%34D5AA5B0218.protected preserve=yes
      //## end ClCacheStyle%34D5AA5B0218.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poClCacheStats%354068D10308
      //## begin ClCacheStyle::m_poClCacheStats%354068D10308.attr preserve=no  private: void *  {UA} 
      void *  m_poClCacheStats;
      //## end ClCacheStyle::m_poClCacheStats%354068D10308.attr

      //## Attribute: m_dMaxPeriod%37429A490234
      //## begin ClCacheStyle::m_dMaxPeriod%37429A490234.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_dMaxPeriod;
      //## end ClCacheStyle::m_dMaxPeriod%37429A490234.attr

      //## Attribute: m_dMaxUnusedPeriod%3932233500B4
      //## begin ClCacheStyle::m_dMaxUnusedPeriod%3932233500B4.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_dMaxUnusedPeriod;
      //## end ClCacheStyle::m_dMaxUnusedPeriod%3932233500B4.attr

    // Additional Private Declarations
      //## begin ClCacheStyle%34D5AA5B0218.private preserve=yes
      //## end ClCacheStyle%34D5AA5B0218.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheStyle%34D5AA5B0218.implementation preserve=yes
      //## end ClCacheStyle%34D5AA5B0218.implementation

};

//## begin ClCacheStyle%34D5AA5B0218.postscript preserve=yes
//## end ClCacheStyle%34D5AA5B0218.postscript

//## begin ClCacheableObject%3496E277009C.preface preserve=yes
//## end ClCacheableObject%3496E277009C.preface

//## Class: ClCacheableObject%3496E277009C
//	  Implementación del objeto Cacheable.
//	Los objetos que se introducen en la cache tienen que ser objetos de una
//	clase derivada de esta.
//	El objeto puede tener su politica. Si la tiene se inicializan las variables
//	miembro de ClCachePolicy.
//
//	Funcionalidad:
//	 -SetPolicy: Se asocia la politica del objeto
//	 -GetPolicy: Se obtiene la politica
//	 -SetKey: Se asocia la clave al objeto
//	 -GetKey: Se obtiene la clave asociada al objeto
//	 -IsReferenced: Devuelve si el objeto cacheado está referenciado
//	 -GetNumOfRef: Devuelve el número de referencias al objeto cacheado
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34D70E21023E;ClCacheStyle { -> }
//## Uses: <unnamed>%36B6F84D02E0;ClDumpCacheVisitor { -> }

class M4_DECL_M4CACHE ClCacheableObject : public ClSerializableObject  //## Inherits: <unnamed>%3548A64F02C9
{
  //## begin ClCacheableObject%3496E277009C.initialDeclarations preserve=yes
  //## end ClCacheableObject%3496E277009C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheableObject%886420462
      //	Constructor que permite pasar la política al objeto cacheable
      ClCacheableObject (ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheKey *  ai_poClCacheKey = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCacheableObject%886595261
      virtual ~ClCacheableObject ();


    //## Other Operations (specified)
      //## Operation: SetPolicy%888398565
      virtual m4return_t SetPolicy (ClCachePolicy *  ai_poPolicy);

      //## Operation: SetKey%888398552
      m4return_t SetKey (ClCacheKey *ai_poClCacheKey);

      //## Operation: GetKey%886497412
      m4return_t GetKey (ClCacheKey * &  ao_poClCacheKey);

      //## Operation: GetPolicy%886420463; C++
      virtual m4return_t GetPolicy (ClCachePolicy * &  ao_poPolicy);

      //## Operation: operator=%886420464; C++
      virtual ClCacheableObject & operator = (ClCacheableObject &ai_oObject);

      //## Operation: IsReferenced%886420465
      m4bool_t IsReferenced ();

      //## Operation: GetNumOfRef%886509525
      m4uint32_t GetNumOfRef ();

      //## Operation: Lock%888566514
      void Lock ();

      //## Operation: Unlock%888566515
      void Unlock ();

      //## Operation: Enable%892745123
      void Enable ();

      //## Operation: Disable%892745124
      void Disable ();

      //## Operation: IsDisabled%892745125
      m4bool_t IsDisabled ();

      //## Operation: Serialize%893688329
      virtual m4return_t Serialize (ClGenericIODriver &   IOD);

      //## Operation: DeSerialize%893688330
      virtual m4return_t DeSerialize (ClGenericIODriver &  IOD);

      //## Operation: GetType%893688331
      virtual m4uint8_t  GetType (void  );

      //## Operation: GetSize%893952352
      m4uint32_t  GetSize ();

      //## Operation: SetSize%893952353
      void  SetSize (m4uint32_t  ai_iSizeObject);

      //## Operation: GetPersistianceLevel %893952355
      virtual m4uint8_t  GetPersistianceLevel ();

      //## Operation: SetLRUDate %894447701
      m4return_t  SetLRUDate (M4ClTimeStamp *   ai_poLRUDate);

      //## Operation: GetLRUDate %894447702
      M4ClTimeStamp *  GetLRUDate ();

      //## Operation: Setm_bDisk%894639698
      m4return_t Setm_bDisk (m4bool_t  ai_bDisk);

      //## Operation: Getm_bDisk%894639699
      m4bool_t Getm_bDisk ();

      //## Operation: LoadValueFromDisk%894884181
      virtual m4return_t LoadValueFromDisk ();

      //## Operation: SaveValueToDisk%894884182
      virtual m4return_t SaveValueToDisk ();

      //## Operation: DeleteValueFromDisk%894884183
      virtual m4return_t DeleteValueFromDisk ();

      //## Operation: DeleteValueFromRam%896181065
      virtual m4return_t DeleteValueFromRam ();

      //## Operation: GetInsertionDate%895764959
      M4ClTimeStamp  GetInsertionDate ();

      //## Operation: SetInsertionDate%895764960
      m4return_t SetInsertionDate (M4ClTimeStamp  ai_oInsertionDate);

      //## Operation: Setm_bRam%896181066
      m4return_t Setm_bRam (m4bool_t ai_bRam);

      //## Operation: Getm_bRam%896181067
      m4bool_t Getm_bRam ();

      //## Operation: Dump%917950245
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor);

      //## Operation: MatchCondition%943372022
      virtual m4bool_t MatchCondition (void *  ai_pCondition);

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%3564424800D5
      //## Role: ClCacheableObject::m_oInsertionDate%3564424902A3
      //## begin ClCacheableObject::m_oInsertionDate%3564424902A3.role preserve=no  public: M4ClTimeStamp { -> VFHAN}
      M4ClTimeStamp m_oInsertionDate;
      //## end ClCacheableObject::m_oInsertionDate%3564424902A3.role

    // Additional Public Declarations
      //## begin ClCacheableObject%3496E277009C.public preserve=yes
	  virtual m4return_t NotifyCacheDestroy ();

	  void SetEnvId(m4pchar_t ai_pcEnvId);

	  m4pchar_t GetEnvId(void);
      //## end ClCacheableObject%3496E277009C.public
  protected:
    // Data Members for Class Attributes

      //## Attribute: m_bDisk%354EF4BB033D
      //## begin ClCacheableObject::m_bDisk%354EF4BB033D.attr preserve=no  protected: m4bool_t  {UA} 
      m4bool_t  m_bDisk;
      //## end ClCacheableObject::m_bDisk%354EF4BB033D.attr

      //## Attribute: m_bRam%356AA1F4001C
      //## begin ClCacheableObject::m_bRam%356AA1F4001C.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bRam;
      //## end ClCacheableObject::m_bRam%356AA1F4001C.attr

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%3496E6EC001F
      //## Role: ClCacheableObject::m_poPolicy%3496E6EC02EE
      //## begin ClCacheableObject::m_poPolicy%3496E6EC02EE.role preserve=no  protected: ClCachePolicy {1 -> 1RHAN}
      ClCachePolicy *m_poPolicy;
      //## end ClCacheableObject::m_poPolicy%3496E6EC02EE.role

      //## Association: M4Cache::<unnamed>%3552CF23012D
      //## Role: ClCacheableObject::m_poClCacheKey%3552CF230386
      //## begin ClCacheableObject::m_poClCacheKey%3552CF230386.role preserve=no  protected: ClCacheKey { -> RFHAN}
      ClCacheKey *m_poClCacheKey;
      //## end ClCacheableObject::m_poClCacheKey%3552CF230386.role

	  ClEnvironmentId *m_pEnvId;

    // Additional Protected Declarations
      //## begin ClCacheableObject%3496E277009C.protected preserve=yes
      //## end ClCacheableObject%3496E277009C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iRefCounter%34D70B08008F
      //## begin ClCacheableObject::m_iRefCounter%34D70B08008F.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iRefCounter;
      //## end ClCacheableObject::m_iRefCounter%34D70B08008F.attr

      //## Attribute: m_bIsDisabled%3536333E0122
      //## begin ClCacheableObject::m_bIsDisabled%3536333E0122.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bIsDisabled;
      //## end ClCacheableObject::m_bIsDisabled%3536333E0122.attr

      //## Attribute: m_iSizeObject%3548B23302BC
      //## begin ClCacheableObject::m_iSizeObject%3548B23302BC.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iSizeObject;
      //## end ClCacheableObject::m_iSizeObject%3548B23302BC.attr

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%35503FA801C7
      //## Role: ClCacheableObject::m_poLRUDate%35503FA9025E
      //## begin ClCacheableObject::m_poLRUDate%35503FA9025E.role preserve=no  private: M4ClTimeStamp { -> RHAN}
      M4ClTimeStamp *m_poLRUDate;
      //## end ClCacheableObject::m_poLRUDate%35503FA9025E.role

    // Additional Private Declarations
      //## begin ClCacheableObject%3496E277009C.private preserve=yes
	  friend m4uint32_t ClCacheStyle::IncrNumOfRef(ClCacheableObject *aio_poClCacheableObject); 
	  friend m4uint32_t ClCacheStyle::DecrNumOfRef(ClCacheableObject *aio_poClCacheableObject); 
      //## end ClCacheableObject%3496E277009C.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheableObject%3496E277009C.implementation preserve=yes
      //## end ClCacheableObject%3496E277009C.implementation

};

//## begin ClCacheableObject%3496E277009C.postscript preserve=yes
//## end ClCacheableObject%3496E277009C.postscript

//## begin ClCache%34D5A9D7002E.preface preserve=yes
//## end ClCache%34D5A9D7002E.preface

//## Class: ClCache%34D5A9D7002E
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%392ACCCD03C2;ClMutBlock { -> F}

class M4_DECL_M4CACHE ClCache : public ClCacheInterface, //## Inherits: <unnamed>%34D5AA09022E
    	public ClSelfPersistiableObject  //## Inherits: <unnamed>%35780C0F02F2
{
  //## begin ClCache%34D5A9D7002E.initialDeclarations preserve=yes
  //## end ClCache%34D5A9D7002E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCache%886497410
      //	Constructor con el que podemos inicializar el puntero
      //
      //	ClCacheStyle *m_poClCacheStyle
      ClCache (ClCacheStyle *ai_poClCacheStyle);

      //## Operation: ClCache%886595255
      ClCache ();

    //## Destructor (specified)
      //## Operation: ~ClCache%886420484
      virtual ~ClCache ();


    //## Other Operations (specified)
      //## Operation: SetCacheStyle%886497411
      virtual m4return_t SetCacheStyle (ClCacheStyle *ai_poClCacheStyle) = 0;

      //## Operation: Serialize%893835151
      m4return_t Serialize (ClGenericIODriver &  IOD);

      //## Operation: DeSerialize%893835152
      m4return_t DeSerialize (ClGenericIODriver &  IOD);

      //## Operation: Persist%897034127
      m4return_t Persist ();

      //## Operation: DePersist%897034128
      m4return_t DePersist ();

      //## Operation: Delete%897034129
      m4return_t Delete ();

      //## Operation: Getm_poPersistor%897034130
      ClPersistor *  Getm_poPersistor ();

      //## Operation: Setm_poPersistor%897034131
      m4return_t Setm_poPersistor (ClPersistor *  ai_poPersistor);

      //## Operation: Dump%917950247
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor);

      //## Operation: GetNObjInCache%921662495
      m4uint32_t GetNObjInCache ();

      //## Operation: GetSizeCache%945090197
      m4uint32_t GetSizeCache ();

      //## Operation: GetMutex%1003764701
      virtual ClMutex & GetMutex ();

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%35780C3A025D
      //## Role: ClCache::m_poPersistor%35780C3D01C1
      //## begin ClCache::m_poPersistor%35780C3D01C1.role preserve=no  public: ClPersistor { -> RHAN}
      ClPersistor *m_poPersistor;
      //## end ClCache::m_poPersistor%35780C3D01C1.role

    // Additional Public Declarations
      //## begin ClCache%34D5A9D7002E.public preserve=yes
      //## end ClCache%34D5A9D7002E.public

  protected:
    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%34D6DC3503DC
      //## Role: ClCache::m_poClCacheStyle%34D6DC3702F9
      //## begin ClCache::m_poClCacheStyle%34D6DC3702F9.role preserve=no  protected: ClCacheStyle {1 -> 1RHAN}
      ClCacheStyle *m_poClCacheStyle;
      //## end ClCache::m_poClCacheStyle%34D6DC3702F9.role

    // Additional Protected Declarations
      //## begin ClCache%34D5A9D7002E.protected preserve=yes
      //## end ClCache%34D5A9D7002E.protected

  private:
    // Additional Private Declarations
      //## begin ClCache%34D5A9D7002E.private preserve=yes
      //## end ClCache%34D5A9D7002E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCache%34D5A9D7002E.implementation preserve=yes
      //## end ClCache%34D5A9D7002E.implementation

};

//## begin ClCache%34D5A9D7002E.postscript preserve=yes
//## end ClCache%34D5A9D7002E.postscript

//## begin ClCacheTest%34D5AA2002E6.preface preserve=yes
//## end ClCacheTest%34D5AA2002E6.preface

//## Class: ClCacheTest%34D5AA2002E6
//	Usuario de la Cache
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE ClCacheTest : public ClCache  //## Inherits: <unnamed>%34D8971303D5
{
  //## begin ClCacheTest%34D5AA2002E6.initialDeclarations preserve=yes
  //## end ClCacheTest%34D5AA2002E6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCacheTest%886420479
      //	Este constructor permite inicializar el puntero
      //
      //	ClCacheStyle *m_poClCacheStyle
      //
      //	Este puntero sirve de puente entre el usuario (ClCacheTest)
      //
      //	y la cache (ClCacheTree)
      ClCacheTest (ClCacheStyle *ai_poClCacheStyle);

      //## Operation: ClCacheTest%886595256
      ClCacheTest ();

    //## Destructor (specified)
      //## Operation: ~ClCacheTest%886420480
      ~ClCacheTest ();


    //## Other Operations (specified)
      //## Operation: SetCacheStyle%886595259
      m4return_t SetCacheStyle (ClCacheStyle *ai_poClCacheStyle);

      //## Operation: PutObject%888398556
      m4return_t PutObject (ClCacheableObject *ai_pObject);

      //## Operation: RemoveObject%888398557
      m4return_t RemoveObject (ClCacheableObject *  ai_pObject);

      //## Operation: UpdateObject%888398558
      m4return_t UpdateObject (ClCacheableObject *ai_pObject);

      //## Operation: GetObject%892745126; C++
      //	Permite obtener un puntero a objeto de la cache. Si el objeto no existe en
      //	la cache o se obtiene un puntero a NULL devuelve M4_ERROR.
      //
      //	Si el puntero a objeto buscado existe se incrementa el atributo de Cl
      //	CacheableObject m_iRefCounter para indicar que existe un usuario más del
      //	objeto.
      //
      //	Argumentos:
      //
      //	ClCacheKey * ai_pKey : Puntero a la clave asociada al objeto.
      //	ClCacheableObject * ao_pObject: Argumento de salida para el objeto buscado.
      //
      //	m4uint32_t ai_iTimeOut: Tiempo límite de búsqueda en la cache.
      m4return_t GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode = GET_SHARED, m4int32_t  ai_iTimeOut = -1);

      //## Operation: UpdateObjectKey%892745129
      m4return_t UpdateObjectKey (ClCacheableObject *ai_poClCacheableObject, ClCacheKey *ai_poClCacheKey);

      //## Operation: SetCOFactory%893835149
      m4return_t SetCOFactory (ClCacheableObjectFactory *  ai_poClCOFactory);

      //## Operation: GetCOFactory%893835150
      ClCacheableObjectFactory *  GetCOFactory ();

      //## Operation: NoUpdateObject%894362623
      m4return_t NoUpdateObject (ClCacheableObject *   ai_pObject);

      //## Operation: Recycle%894985404
      m4return_t Recycle ();

      //## Operation: RemoveAll%895824025
      m4return_t RemoveAll (M4ClTimeStamp *  ai_poLimitDate);

      //## Operation: RemoveAll%895824026
      m4return_t RemoveAll (m4pchar_t ai_pcPortionKey, m4puint32_t ai_piRemovedCount = NULL);

      //## Operation: RemoveAll%896690042
      m4return_t RemoveAll ();

      //## Operation: GetNext%910956612
      m4return_t GetNext (ClCacheableObject * &  ao_poClCacheableObject, void * &  ai_poIterator);

      //## Operation: SetMaxNOfObjs%910956613
      m4return_t SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs);

      //## Operation: SetMaxSizeCache%910956614
      m4return_t SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache);

      //## Operation:  RemoveByCondition %943372023
      m4return_t RemoveByCondition (void *  ai_pCondition);

      //## Operation: ReplaceObject%945346493; C++
      //	Tiene la misma funcionalidad que PutObject pero nunca decrementa el número
      //	de referencias m_iRefCounter.
      //
      //	Se utiliza para actualizar un objeto en la cache
      m4return_t ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject);

    // Additional Public Declarations
      //## begin ClCacheTest%34D5AA2002E6.public preserve=yes
      //## end ClCacheTest%34D5AA2002E6.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheTest%34D5AA2002E6.protected preserve=yes
      //## end ClCacheTest%34D5AA2002E6.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheTest%34D5AA2002E6.private preserve=yes
      //## end ClCacheTest%34D5AA2002E6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheTest%34D5AA2002E6.implementation preserve=yes
      //## end ClCacheTest%34D5AA2002E6.implementation

};

//## begin ClCacheTest%34D5AA2002E6.postscript preserve=yes
//## end ClCacheTest%34D5AA2002E6.postscript

//## begin ClCacheKeyLess%34E03F430191.preface preserve=yes
//## end ClCacheKeyLess%34E03F430191.preface

//## Class: ClCacheKeyLess%34E03F430191
//	Realiza la busqueda dentro del map.
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E06B9C01AA;friend ClCacheKey { -> }

class M4_DECL_M4CACHE ClCacheKeyLess 
{
  //## begin ClCacheKeyLess%34E03F430191.initialDeclarations preserve=yes
  //## end ClCacheKeyLess%34E03F430191.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator ( )%888398564
      //	 Sobrecarga del operador () para realizar la busqueda dentro del map
      m4bool_t operator ( ) (const ClCacheKey *ai_pKeyA, const ClCacheKey *  ai_pKeyB) const;

    // Additional Public Declarations
      //## begin ClCacheKeyLess%34E03F430191.public preserve=yes
      //## end ClCacheKeyLess%34E03F430191.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheKeyLess%34E03F430191.protected preserve=yes
      //## end ClCacheKeyLess%34E03F430191.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheKeyLess%34E03F430191.private preserve=yes
      //## end ClCacheKeyLess%34E03F430191.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheKeyLess%34E03F430191.implementation preserve=yes
      //## end ClCacheKeyLess%34E03F430191.implementation

};

//## begin ClCacheKeyLess%34E03F430191.postscript preserve=yes
//## end ClCacheKeyLess%34E03F430191.postscript

//## begin ClCacheableObjectFactory%3544ABEC03B3.preface preserve=yes
//## end ClCacheableObjectFactory%3544ABEC03B3.preface

//## Class: ClCacheableObjectFactory%3544ABEC03B3
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE ClCacheableObjectFactory 
{
  //## begin ClCacheableObjectFactory%3544ABEC03B3.initialDeclarations preserve=yes
  //## end ClCacheableObjectFactory%3544ABEC03B3.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: SerializeObject%893688332
      virtual m4return_t SerializeObject (ClCacheableObject *  ai_poObject, ClGenericIODriver *  IOD, m4uint8_t  ai_iMode);

      //## Operation: DeSerializeObject%893688333
      virtual m4return_t  DeSerializeObject (ClCacheableObject * &  aio_poObject, ClGenericIODriver *  IOD, m4uint8_t  ai_iMode);

      //## Operation: Dump%917950250
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_oClDumpCacheVisitor);

    // Additional Public Declarations
      //## begin ClCacheableObjectFactory%3544ABEC03B3.public preserve=yes
      //## end ClCacheableObjectFactory%3544ABEC03B3.public

  protected:
    // Additional Protected Declarations
      //## begin ClCacheableObjectFactory%3544ABEC03B3.protected preserve=yes
      //## end ClCacheableObjectFactory%3544ABEC03B3.protected

  private:
    // Additional Private Declarations
      //## begin ClCacheableObjectFactory%3544ABEC03B3.private preserve=yes
      //## end ClCacheableObjectFactory%3544ABEC03B3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheableObjectFactory%3544ABEC03B3.implementation preserve=yes
      //## end ClCacheableObjectFactory%3544ABEC03B3.implementation

};

//## begin ClCacheableObjectFactory%3544ABEC03B3.postscript preserve=yes
//## end ClCacheableObjectFactory%3544ABEC03B3.postscript

//## begin ClLRUDateLess%354F1BC200B9.preface preserve=yes
//## end ClLRUDateLess%354F1BC200B9.preface

//## Class: ClLRUDateLess%354F1BC200B9
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CACHE  ClLRUDateLess 
{
  //## begin ClLRUDateLess%354F1BC200B9.initialDeclarations preserve=yes
  //## end ClLRUDateLess%354F1BC200B9.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator ()%894362625
      m4bool_t  operator () ( M4ClTimeStamp *     ai_oLRUDateA,  M4ClTimeStamp  *  ai_oLRUDateB) const;

    // Additional Public Declarations
      //## begin ClLRUDateLess%354F1BC200B9.public preserve=yes
      //## end ClLRUDateLess%354F1BC200B9.public

  protected:
    // Additional Protected Declarations
      //## begin ClLRUDateLess%354F1BC200B9.protected preserve=yes
      //## end ClLRUDateLess%354F1BC200B9.protected

  private:
    // Additional Private Declarations
      //## begin ClLRUDateLess%354F1BC200B9.private preserve=yes
      //## end ClLRUDateLess%354F1BC200B9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLRUDateLess%354F1BC200B9.implementation preserve=yes
      //## end ClLRUDateLess%354F1BC200B9.implementation

};

//## begin ClLRUDateLess%354F1BC200B9.postscript preserve=yes
//## end ClLRUDateLess%354F1BC200B9.postscript

//## begin ClCacheTree%34D5ABD4018E.preface preserve=yes
//## end ClCacheTree%34D5ABD4018E.preface

//## Class: ClCacheTree%34D5ABD4018E
//	Implementacion de la cache mediante un map y la cola de borrado con un
//	vector:
//
//	 -CacheOfObjects: Variable miembro tipo <map> con la que se construye la
//	cache de objetos
//
//	 -ObjectDeletionQueue:Variable miembro tipo <vector> con la que se construye
//	la cola de borrado de objetos obsoletos.
//
//	Las funcionalidades hasta ahora implementadas:
//
//	       -PutObject: Mete un Objeto en la Cache. Solo decrementa la referencia
//	si el objeto existe en la cache. Si es una inicializacion o en la posicion
//	del objeto hay un puntero nulo no decrementa la el número de referencias.
//
//	       -GetObject:Saca un objeto de la Cache. Incrementa el numero de
//	referencias
//
//	       -RemoveObject:Elimina un objeto de la Cache y lo introduce en la cola
//	de borrado
//
//	       -UpdateObject: Actualiza un objeto de la Cache sin decrementar su
//	referencia
//
//	       -Recycle:Recycle: Permite  recorrer la cache para evaluar si existe
//	algun objeto obsoleto con la función IsValid(). Si es así, se elimina de la
//	cache y se pasa a la cola de borrado con la función RemoveObject(). Después
//	evalúa la cola de borrado. Si existe algún objeto en ella que tiene su m_iRef
//	Counter a cero lo elimina.
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E06B840097;ClCacheKeyLess { -> }
//## Uses: <unnamed>%354F1C090101;ClLRUDateLess { -> }

class M4_DECL_M4CACHE ClCacheTree : public ClCacheStyle  //## Inherits: <unnamed>%34D897160130
{
  //## begin ClCacheTree%34D5ABD4018E.initialDeclarations preserve=yes
  //## end ClCacheTree%34D5ABD4018E.initialDeclarations

  public:
    //## begin ClCacheTree::CacheableObject2Map%35363B2701CE.preface preserve=yes
    //## end ClCacheTree::CacheableObject2Map%35363B2701CE.preface

    //## Class: CacheableObject2Map%35363B2701CE; Instantiated Class
    //## Category: M4Cache%3496B4B10159
    //## Subsystem: M4Cache::inc%3796F4510086
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef multimap< const ClCacheKey *,ClCacheableObject *,ClCacheKeyLess > CacheableObject2Map;

    //## begin ClCacheTree::CacheableObject2Map%35363B2701CE.postscript preserve=yes
    //## end ClCacheTree::CacheableObject2Map%35363B2701CE.postscript

    //## begin ClCacheTree::CacheableObject2List%35363CF5033B.preface preserve=yes
    //## end ClCacheTree::CacheableObject2List%35363CF5033B.preface

    //## Class: CacheableObject2List%35363CF5033B; Instantiated Class
    //## Category: M4Cache%3496B4B10159
    //## Subsystem: M4Cache::inc%3796F4510086
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef list< ClCacheableObject * > CacheableObject2List;

    //## begin ClCacheTree::CacheableObject2List%35363CF5033B.postscript preserve=yes
    //## end ClCacheTree::CacheableObject2List%35363CF5033B.postscript

    //## begin ClCacheTree::CacheableObject2LRUMap%354F58FC011B.preface preserve=yes
    //## end ClCacheTree::CacheableObject2LRUMap%354F58FC011B.preface

    //## Class: CacheableObject2LRUMap%354F58FC011B; Instantiated Class
    //## Category: M4Cache%3496B4B10159
    //## Subsystem: M4Cache::inc%3796F4510086
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef multimap< M4ClTimeStamp *,ClCacheableObject * ,ClLRUDateLess  > CacheableObject2LRUMap;

    //## begin ClCacheTree::CacheableObject2LRUMap%354F58FC011B.postscript preserve=yes
    //## end ClCacheTree::CacheableObject2LRUMap%354F58FC011B.postscript

    //## Constructors (specified)
      //## Operation: ClCacheTree%886420481
      ClCacheTree (m4double_t ai_dMaxPeriod = 0, m4double_t ai_dMaxUnusedPeriod = 0, m4uint32_t ai_ulMaxSizeCache = 10000000, m4uint32_t ai_ulMaxNOfObjs = 1000, m4uint32_t ai_ulRefreshRatio = 50);

    //## Destructor (specified)
      //## Operation: ~ClCacheTree%886420482
      ~ClCacheTree ();


    //## Other Operations (specified)
      //## Operation: Recycle%886595260
      m4return_t Recycle ();

      //## Operation: PutObject%888398561
      virtual m4return_t PutObject (ClCacheableObject *ai_pObject);

      //## Operation: RemoveObject%888398562
      m4return_t RemoveObject (ClCacheableObject *  ai_pObject);

      //## Operation: UpdateObject%888398563
      m4return_t UpdateObject (ClCacheableObject *  ai_pObject);

      //## Operation: GetObject%892745127; C++
      //	Permite obtener un puntero a objeto de la cache. Si el objeto no existe en
      //	la cache o se obtiene un puntero a NULL devuelve M4_ERROR.
      //
      //	Si el puntero a objeto buscado existe se incrementa el atributo de Cl
      //	CacheableObject m_iRefCounter para indicar que existe un usuario más del
      //	objeto.
      //
      //	Argumentos:
      //
      //	ClCacheKey * ai_pKey : Puntero a la clave asociada al objeto.
      //	ClCacheableObject * ao_pObject: Argumento de salida para el objeto buscado.
      //
      //	m4uint32_t ai_iTimeOut: Tiempo límite de búsqueda en la cache.
      m4return_t GetObject (ClCacheKey *ai_pKey, ClCacheableObject * &  ao_pObject, eGetMode ai_eGetMode = GET_SHARED, m4int32_t  ai_iTimeOut = -1);

      //## Operation: UpdateObjectKey%892745130
      m4return_t UpdateObjectKey (ClCacheableObject *ai_poClCacheableObject, ClCacheKey *ai_poClCacheKey);

      //## Operation: SetCOFactory%893688334
      m4return_t SetCOFactory (ClCacheableObjectFactory  *  ai_poClCOFactory);

      //## Operation: GetCOFactory%893688335
      ClCacheableObjectFactory *  GetCOFactory ();

      //## Operation: Serialize%893751388
      m4return_t Serialize (ClGenericIODriver &  IOD);

      //## Operation: DeSerialize%893751389
      m4return_t DeSerialize (ClGenericIODriver &  IOD, m4pchar_t ai_pcEnvId);

      //## Operation: RemoveAll%894362621
      m4return_t  RemoveAll ();

      //## Operation: NoUpdateObject%894362624
      m4return_t  NoUpdateObject (ClCacheableObject *  ai_pObject);

      //## Operation: DestroyAll%895764957
      m4return_t DestroyAll ();

      //## Operation: RemoveAll%895824027
      m4return_t RemoveAll (M4ClTimeStamp *  ai_poLimitDate);

      //## Operation: RemoveAll%895824028
      m4return_t RemoveAll (m4pchar_t ai_pcPortionKey, m4puint32_t ai_piRemovedCount = NULL);

      //## Operation: Refresh%897292450
      m4return_t Refresh (ClSelfPersistiableObject *   ai_poSelfPersistor);

      //## Operation: GetNext%911234942
      m4return_t GetNext (ClCacheableObject *  &  ao_poClCacheableObject, void * &  ai_poIterator);

      //## Operation: Dump%917950249
      virtual m4return_t Dump (ClDumpCacheVisitor &  ai_pClDumpCacheVisitor);

      //## Operation: SizeControl%943372020
      virtual m4return_t SizeControl ();

      //## Operation: NumOfObjectsControl%943372021
      virtual m4return_t NumOfObjectsControl ();

      //## Operation: RemoveByCondition%943372024
      m4return_t RemoveByCondition (void *  ai_pCondition);

      //## Operation: ReplaceObject%945346494; C++
      //	Tiene la misma funcionalidad que PutObject pero nunca decrementa el número
      //	de referencias m_iRefCounter.
      //
      //	Se utiliza para actualizar un objeto en la cache
      virtual m4return_t ReplaceObject (ClCacheableObject *ai_pOldObject, ClCacheableObject *ai_pNewObject);

    // Data Members for Class Attributes

      //## Attribute: CacheOfObjects%34E03E9F0091
      //## begin ClCacheTree::CacheOfObjects%34E03E9F0091.attr preserve=no  public: CacheableObject2Map {UA} 
      CacheableObject2Map CacheOfObjects;
      //## end ClCacheTree::CacheOfObjects%34E03E9F0091.attr

  public:
    // Additional Public Declarations
      //## begin ClCacheTree%34D5ABD4018E.public preserve=yes
      //## end ClCacheTree%34D5ABD4018E.public

  protected:

    //## Other Operations (specified)
      //## Operation: RemoveObjectInternalNonBlock%945346495
      m4return_t RemoveObjectInternalNonBlock (ClCacheableObject *  ai_pObject);

      //## Operation: UpdateObjectInternalNonBlock%945346496
      m4return_t UpdateObjectInternalNonBlock (ClCacheableObject *  ai_pObject);

    // Data Members for Class Attributes

      //## Attribute: ObjectDeletionQueue%34E1DE6D00D2
      //## begin ClCacheTree::ObjectDeletionQueue%34E1DE6D00D2.attr preserve=no  protected: CacheableObject2List {UA} 
      CacheableObject2List ObjectDeletionQueue;
      //## end ClCacheTree::ObjectDeletionQueue%34E1DE6D00D2.attr

      //## Attribute: LRUDateMap%354F5B0103BD
      //## begin ClCacheTree::LRUDateMap%354F5B0103BD.attr preserve=no  protected: CacheableObject2LRUMap {UA} 
      CacheableObject2LRUMap LRUDateMap;
      //## end ClCacheTree::LRUDateMap%354F5B0103BD.attr

    // Additional Protected Declarations
      //## begin ClCacheTree%34D5ABD4018E.protected preserve=yes
      //## end ClCacheTree%34D5ABD4018E.protected

  private:

    //## Other Operations (specified)
      //## Operation: RemoveObjectNonBlock%888566517
      m4return_t RemoveObjectNonBlock ( ClCacheableObject *ai_pObject);

      //## Operation: Find%892796789
      ClCacheableObject * Find (ClCacheKey *ai_poClCacheKey, eGetMode ai_eGetMode);

    // Data Members for Associations

      //## Association: M4Cache::<unnamed>%3544B5FB0348
      //## Role: ClCacheTree::m_poClCacheableObjectFactory%3544B6000327
      //## begin ClCacheTree::m_poClCacheableObjectFactory%3544B6000327.role preserve=no  private: ClCacheableObjectFactory { -> RFHAN}
      ClCacheableObjectFactory *m_poClCacheableObjectFactory;
      //## end ClCacheTree::m_poClCacheableObjectFactory%3544B6000327.role

    // Additional Private Declarations
      //## begin ClCacheTree%34D5ABD4018E.private preserve=yes
      //## end ClCacheTree%34D5ABD4018E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCacheTree%34D5ABD4018E.implementation preserve=yes
      //## end ClCacheTree%34D5ABD4018E.implementation

};

//## begin ClCacheTree%34D5ABD4018E.postscript preserve=yes
//## end ClCacheTree%34D5ABD4018E.postscript

// Class ClCacheKey 

// Class ClCacheInterface 

// Class ClCachePolicy 

// Class ClCacheStyle 


//## Other Operations (inline)
inline ClMutex & ClCacheStyle::GetMutex ()
{
  //## begin ClCacheStyle::GetMutex%1003764699.body preserve=yes
	return m_oMutex;
  //## end ClCacheStyle::GetMutex%1003764699.body
}

// Class ClCacheableObject 

// Class ClCache 

// Class ClCacheTest 

// Class ClCacheKeyLess 

// Class ClCacheableObjectFactory 

// Class ClLRUDateLess 

// Class ClCacheTree 

//## begin module%34B4B5B80039.epilog preserve=yes
//## end module%34B4B5B80039.epilog


#endif
