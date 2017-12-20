//## begin module%3742D02003D7.cm preserve=no
//## end module%3742D02003D7.cm

//## begin module%3742D02003D7.cp preserve=no
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
//## end module%3742D02003D7.cp

//## Module: m4thread%3742D02003D7; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\Work\v600\m4ipcs\inc\m4thread.hpp

#ifndef _M4THREAD_HPP_
#define _M4THREAD_HPP_ 1

//## begin module%3742D02003D7.additionalIncludes preserve=no
//## end module%3742D02003D7.additionalIncludes

//## begin module%3742D02003D7.includes preserve=yes
#include <m4ipcs_dll.hpp>
#include "m4stl.hpp"
//## end module%3742D02003D7.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// CriticalSection
#include <criticalsection.hpp>
// longjmp
#include <setjmp.h>
// m4exception
#include <m4exception.hpp>
// m4condition
#include <m4condition.hpp>

class M4_DECL_M4IPCS M4Thread;

//## begin module%3742D02003D7.declarations preserve=no
//## end module%3742D02003D7.declarations

//## begin module%3742D02003D7.additionalDeclarations preserve=yes
#ifdef _UNIX
	#if defined ( _DEC )
	#else
		#include <unistd.h>
		#define M4_THREAD_RETURN void *
extern "C" {
    typedef   void *(func)(void*) ;
		typedef  func *  M4THREAD_FUNC;
}

		#ifdef _M4HPUX1020_
			typedef unsigned int m4iden_t;
		#else
			typedef pthread_t m4iden_t;
		#endif	
		typedef pthread_t m4thread_t;
	#endif
#else
	typedef unsigned (_stdcall  *func)(void *);
	#define M4THREAD_FUNC   func
	#define M4_THREAD_RETURN  unsigned _stdcall
	typedef	uintptr_t  m4thread_t;
	typedef DWORD  m4iden_t;

#endif

// defines for thread properties
#define M4_THREADPROP_SESSION_ID "M4_SESSION_ID"

//## end module%3742D02003D7.additionalDeclarations


//## Class: M4ThreadMap_t%3742E3FA00E5
//	Mapa que contiene: ident. thread && objeto M4Thread asocioado
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3742E5A70273;M4Thread { -> F}

typedef map< m4iden_t, M4Thread*, less<m4iden_t> > M4ThreadMap_t;

//## Class: M4UserHandler_t%3742E5EB0130
//	Tipo de Funcion a ejecutar en caso de doble GP
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef void (*M4UserHandler_t) (m4pvoid_t);

//## Class: M4ThrowFunction_t%3EF9CA60008C
//	Prototipo de la función de lanzamiento de excepciones
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef void (*M4ThrowFunction_t)(M4RuntimeException);

//## Class: M4PropertyBag_t%3EF9D0ED03E0
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< m4string_t, m4pvoid_t, less<m4string_t> > M4PropertyBag_t;

//## begin M4Thread%3742D02200CF.preface preserve=yes
//## end M4Thread%3742D02200CF.preface

//## Class: M4Thread%3742D02200CF
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3742E55C0275;M4ThreadMap_t { -> }
//## Uses: <unnamed>%3742E61100A8;M4UserHandler_t { -> }
//## Uses: <unnamed>%3EF9C60D01B6;M4RuntimeException { -> F}
//## Uses: <unnamed>%3EF9CB1A01CA;M4ThrowFunction_t { -> }
//## Uses: <unnamed>%3EF9D1470336;M4PropertyBag_t { -> }
//## Uses: <unnamed>%3EF9DA0000F2;m4string_t { -> }

class M4_DECL_M4IPCS M4Thread 
{
  //## begin M4Thread%3742D02200CF.initialDeclarations preserve=yes
#ifdef _UNIX
  public:
			m4uint32_t m_iNumException;
			m4uint32_t m_iLastException;
#endif
  //## end M4Thread%3742D02200CF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4Thread%1087019654; C++
      M4Thread ();

    //## Destructor (specified)
      //## Operation: ~M4Thread%39119088; C++
      //	constructor
      virtual ~M4Thread ();


    //## Other Operations (specified)
      //## Operation: IsCreated%927723379
      m4bool_t IsCreated ();

      //## Operation: Wait%927795535
      m4return_t Wait ();

      //## Operation: SetUserHandler%-71870237; C++
      void SetUserHandler (M4UserHandler_t ai_pFun, m4pvoid_t ai_param)
      {
        //## begin M4Thread::SetUserHandler%-71870237.body preserve=yes
			m_paramUserHandler = ai_param;
			m_pfUserHandler = ai_pFun;
        //## end M4Thread::SetUserHandler%-71870237.body
      }

      //## Operation: IsRunning%927723380
      m4bool_t IsRunning ();

      //## Operation: GetUserHandler%1657823385; C++
      M4UserHandler_t GetUserHandler (m4pvoid_t& ao_pParam)
      {
        //## begin M4Thread::GetUserHandler%1657823385.body preserve=yes
			ao_pParam = m_paramUserHandler;
			return m_pfUserHandler;
		
        //## end M4Thread::GetUserHandler%1657823385.body
      }

      //## Operation: RegisterThread%-323165801; C++
      //	Introducir datos en el mapa
      static void RegisterThread (M4Thread* ai_pThread);

      //## Operation: DeregisterThread%471052076; C++
      //	Sacar datos del mapa
      static void DeregisterThread (m4iden_t ai_Ident);

      //## Operation: GetInstance%353962315; C++
      //	Devuelve el objeto M4Thread correspondiente al identificador que se le pasa
      static m4return_t GetInstance (m4iden_t ai_Ident, M4Thread*& ao_pThread);

      //## Operation: SetExceptionMask%627372368; C++
      //	Funciones que establece y retorna la mascara de señales que queremos capturar
      void SetExceptionMask (m4uint32_t ai_uiExceptionMask)
      {
        //## begin M4Thread::SetExceptionMask%627372368.body preserve=yes
			m_uiExceptionMask = ai_uiExceptionMask;
        //## end M4Thread::SetExceptionMask%627372368.body
      }

      //## Operation: GetExceptionMask%760269201; C++
      m4uint32_t GetExceptionMask (void )
      {
        //## begin M4Thread::GetExceptionMask%760269201.body preserve=yes
			return m_uiExceptionMask;
        //## end M4Thread::GetExceptionMask%760269201.body
      }

      //## Operation: Create%631361272; C++
      //	destructor
      //	Lanzar la ejecucion de un thread.
      //	Devuelve SUCCESS o ERROR
      m4return_t Create (func lpSt, m4pvoid_t lpParameter = NULL);

      //## Operation: Start%356177253; C++
      //	Crea el thread y comienza a ejecutarlo
      m4return_t Start (m4pchar_t ai_StringsId = NULL);

      //## Operation: Run%1264651741; C++
      virtual void Run (void )
      {
        //## begin M4Thread::Run%1264651741.body preserve=yes
        //## end M4Thread::Run%1264651741.body
      }

      //## Operation: GetDestroy%1652594998; C++
      m4bool_t GetDestroy (void );

      //## Operation: Terminate%858475301; C++
      //	Devolvemos si queremos destruirnos o no
      //	Terminar la ejecucion de un thread. Asigna el resultado de la
      //	ejecucion en status. Este valor puede ser consultado por
      //	otro thread mediante la funcion Join
      m4return_t Terminate (m4bool_t destroy = M4_FALSE);

      //## Operation: Suspend%75941335; C++
      //	La siguiente funcion no esta disponible. Es posible implementarla
      //	en Solaris con thr_suspend, pero no en POSIX, con lo que no
      //	funcionaria en las otras plataformas Unix.
      m4return_t Suspend ();

      //## Operation: Resume%2123195767; C++
      //	La siguiente funcion no esta disponible. Es posible implementarla
      //	en Solaris con thr_continue, pero no en POSIX, con lo que no
      //	funcionaria en las otras plataformas Unix.
      m4return_t Resume ();

      //## Operation: IsExit%-1368781007; C++
      //	La  siguiente funcion nos indica si el thread se ha
      //	suicidado
      m4bool_t IsExit (void );

      //## Operation: GetIdThread%811424171; C++
      //	La siguiente función nos permite obtener el identificador
      //	del thread
      m4iden_t GetIdThread (void )
      {
        //## begin M4Thread::GetIdThread%811424171.body preserve=yes
		 return m_identifier;
        //## end M4Thread::GetIdThread%811424171.body
      }

      //## Operation: SetIdThread%-81462516; C++
      void SetIdThread (m4iden_t ai_identifier)
      {
        //## begin M4Thread::SetIdThread%-81462516.body preserve=yes
			m_identifier = ai_identifier;
        //## end M4Thread::SetIdThread%-81462516.body
      }

      //## Operation: GetCurrentIdThread%-1698837353; C++
      //	-- {AddDecl: 52} region.generated [8348..8566]
      //	Esta función es estática .Lo que devuelve es el
      //	identificador del thread en que se llame
      static m4iden_t GetCurrentIdThread (void );

      //## Operation: GetStringsId%710888536; C++
      //	Función que retorna la cadena que identifica el thread
      m4pchar_t GetStringsId (void )
      {
        //## begin M4Thread::GetStringsId%710888536.body preserve=yes
			 return m_pcStringsId;
		 
        //## end M4Thread::GetStringsId%710888536.body
      }

      //## Operation: SetUseException%1034054397; C++
      //	Función que permite seleccionar si queremos excepciones o no
      void SetUseException (m4bool_t ai_bUseException)
      {
        //## begin M4Thread::SetUseException%1034054397.body preserve=yes
 			m_UseException=ai_bUseException;
        //## end M4Thread::SetUseException%1034054397.body
      }

      //## Operation: GetUseException%-777469131; C++
      m4bool_t GetUseException (void )
      {
        //## begin M4Thread::GetUseException%-777469131.body preserve=yes
			return m_UseException; 
        //## end M4Thread::GetUseException%-777469131.body
      }

      //## Operation: GetId%1068587649; C++
      m4uint32_t GetId (void )
      {
        //## begin M4Thread::GetId%1068587649.body preserve=yes
			 return m_uIdPersonal;
        //## end M4Thread::GetId%1068587649.body
      }

      //## Operation: SetId%801625668; C++
      void SetId (m4uint32_t ai_IdPersonal)
      {
        //## begin M4Thread::SetId%801625668.body preserve=yes
			 m_uIdPersonal=ai_IdPersonal;
        //## end M4Thread::SetId%801625668.body
      }

      //## Operation: SetExit%927126297; C++
      m4void_t SetExit (m4bool_t ai_bExit = M4_TRUE);

      //## Operation: GetThrowFunction%1056555596
      M4ThrowFunction_t GetThrowFunction ()
      {
        //## begin M4Thread::GetThrowFunction%1056555596.body preserve=yes
			return( m_pfThrowFunction ) ;
        //## end M4Thread::GetThrowFunction%1056555596.body
      }

      //## Operation: AddProperty%1056555598
      static m4return_t AddProperty (m4pcchar_t ai_pcName, m4pvoid_t ai_pValue);

      //## Operation: GetProperty%1056555599
      static m4pvoid_t GetProperty (m4pcchar_t ai_pcName);

    // Additional Public Declarations
      //## begin M4Thread%3742D02200CF.public preserve=yes
      //## end M4Thread%3742D02200CF.public

  protected:

    //## Other Operations (specified)
      //## Operation: IsSuspend%1234446964; C++
      m4bool_t IsSuspend (void );

      //## Operation: IsTerminate%-139898326; C++
      m4bool_t IsTerminate (void );

      //## Operation: SetThrowFunction%1056555597
      M4ThrowFunction_t SetThrowFunction (M4ThrowFunction_t ai_pfThrowFunction)
      {
        //## begin M4Thread::SetThrowFunction%1056555597.body preserve=yes
		  M4ThrowFunction_t pfOldThrowFunction = m_pfThrowFunction ;
		  m_pfThrowFunction = ai_pfThrowFunction ;
		  return( pfOldThrowFunction ) ;
        //## end M4Thread::SetThrowFunction%1056555597.body
      }

    // Data Members for Class Attributes

      //## Attribute: s_ListThread%3742D02200EF
      //	Mapa de threads activos
      //## begin M4Thread::s_ListThread%3742D02200EF.attr preserve=no  protected: static M4ThreadMap_t {VA} 
      static M4ThreadMap_t s_ListThread;
      //## end M4Thread::s_ListThread%3742D02200EF.attr

      //## Attribute: m_pfUserHandler%3742D02200F5
      //	Funcion a ejecutar en caso de doble GP
      //## begin M4Thread::m_pfUserHandler%3742D02200F5.attr preserve=no  protected: M4UserHandler_t {VA} 
      M4UserHandler_t m_pfUserHandler;
      //## end M4Thread::m_pfUserHandler%3742D02200F5.attr

      //## Attribute: m_thid%3742D0220101
      //	Contiene la mascara de las excepciones que se desean capturar
      //## begin M4Thread::m_thid%3742D0220101.attr preserve=no  protected: m4thread_t {VA} 
      m4thread_t m_thid;
      //## end M4Thread::m_thid%3742D0220101.attr

      //## Attribute: m_identifier%3742D0220106
      //## begin M4Thread::m_identifier%3742D0220106.attr preserve=no  protected: m4iden_t {VA} 
      m4iden_t m_identifier;
      //## end M4Thread::m_identifier%3742D0220106.attr

      //## Attribute: m_pfThrowFunction%3EF9CD4C0114
      //## begin M4Thread::m_pfThrowFunction%3EF9CD4C0114.attr preserve=no  protected: M4ThrowFunction_t {UA} 
      M4ThrowFunction_t m_pfThrowFunction;
      //## end M4Thread::m_pfThrowFunction%3EF9CD4C0114.attr

      //## Attribute: m_oPropertyBag%3EF9D1940083
      //## begin M4Thread::m_oPropertyBag%3EF9D1940083.attr preserve=no  protected: M4PropertyBag_t {UA} 
      M4PropertyBag_t m_oPropertyBag;
      //## end M4Thread::m_oPropertyBag%3EF9D1940083.attr

    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CD4C0151
      //## Role: M4Thread::m_exit%3743CD4D02C5
      //## begin M4Thread::m_exit%3743CD4D02C5.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_exit;
      //## end M4Thread::m_exit%3743CD4D02C5.role

      //## Association: M4Ipcs::<unnamed>%3743CD9C017E
      //## Role: M4Thread::m_destroy%3743CD9E025E
      //## begin M4Thread::m_destroy%3743CD9E025E.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_destroy;
      //## end M4Thread::m_destroy%3743CD9E025E.role

      //## Association: M4Ipcs::<unnamed>%3743CDD3032C
      //## Role: M4Thread::m_UseException%3743CDD5028F
      //## begin M4Thread::m_UseException%3743CDD5028F.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_UseException;
      //## end M4Thread::m_UseException%3743CDD5028F.role

      //## Association: M4Ipcs::<unnamed>%3743D1370348
      //## Role: M4Thread::m_create%3743D138026E
      //## begin M4Thread::m_create%3743D138026E.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_create;
      //## end M4Thread::m_create%3743D138026E.role

      //## Association: M4Ipcs::<unnamed>%3743D14C01F3
      //## Role: M4Thread::m_suspend%3743D14D00E8
      //## begin M4Thread::m_suspend%3743D14D00E8.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_suspend;
      //## end M4Thread::m_suspend%3743D14D00E8.role

      //## Association: M4Ipcs::<unnamed>%3743D1700304
      //## Role: M4Thread::m_terminate%3743D17101B1
      //## begin M4Thread::m_terminate%3743D17101B1.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_terminate;
      //## end M4Thread::m_terminate%3743D17101B1.role

      //## Association: M4Ipcs::<unnamed>%3743D1F900C6
      //## Role: M4Thread::m_uiExceptionMask%3743D1FA0118
      //## begin M4Thread::m_uiExceptionMask%3743D1FA0118.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_uiExceptionMask;
      //## end M4Thread::m_uiExceptionMask%3743D1FA0118.role

      //## Association: M4Ipcs::<unnamed>%3743D21B0273
      //## Role: M4Thread::m_uIdPersonal%3743D21C02E3
      //## begin M4Thread::m_uIdPersonal%3743D21C02E3.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_uIdPersonal;
      //## end M4Thread::m_uIdPersonal%3743D21C02E3.role

      //## Association: M4Ipcs::<unnamed>%3743D5910137
      //## Role: M4Thread::m_paramUserHandler%3743D5910373
      //## begin M4Thread::m_paramUserHandler%3743D5910373.role preserve=no  protected: m4pvoid_t { -> 1VHAN}
      m4pvoid_t m_paramUserHandler;
      //## end M4Thread::m_paramUserHandler%3743D5910373.role

      //## Association: M4Ipcs::<unnamed>%3743D5A80158
      //## Role: M4Thread::m_pcStringsId%3743D5A803C6
      //## begin M4Thread::m_pcStringsId%3743D5A803C6.role preserve=no  protected: m4pchar_t { -> VHAN}
      m4pchar_t m_pcStringsId;
      //## end M4Thread::m_pcStringsId%3743D5A803C6.role

      //## Association: M4Ipcs::<unnamed>%374D139200BF
      //## Role: M4Thread::m_oCondVar%374D139202DC
      //## begin M4Thread::m_oCondVar%374D139202DC.role preserve=no  protected: ClCondition { -> 1RHAN}
      ClCondition *m_oCondVar;
      //## end M4Thread::m_oCondVar%374D139202DC.role

      //## Association: M4Ipcs::<unnamed>%3923F6D80344
      //## Role: M4Thread::m_hHandleMutex%3923F6D8036C
      //## begin M4Thread::m_hHandleMutex%3923F6D8036C.role preserve=no  protected: ClCriticalSection { -> 1VHAN}
      ClCriticalSection m_hHandleMutex;
      //## end M4Thread::m_hHandleMutex%3923F6D8036C.role

      //## Association: M4Ipcs::<unnamed>%3923F6DB00E5
      //## Role: M4Thread::s_oMutexRegister%3923F6DB00E6
      //## begin M4Thread::s_oMutexRegister%3923F6DB00E6.role preserve=no  protected: static ClCriticalSection { -> VHAN}
      static ClCriticalSection s_oMutexRegister;
      //## end M4Thread::s_oMutexRegister%3923F6DB00E6.role

    // Additional Protected Declarations
      //## begin M4Thread%3742D02200CF.protected preserve=yes
      //## end M4Thread%3742D02200CF.protected

  private:
    // Additional Private Declarations
      //## begin M4Thread%3742D02200CF.private preserve=yes
      //## end M4Thread%3742D02200CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4Thread%3742D02200CF.implementation preserve=yes
      //## end M4Thread%3742D02200CF.implementation

};

//## begin M4Thread%3742D02200CF.postscript preserve=yes
//## end M4Thread%3742D02200CF.postscript

//## begin M4CheckJump%3EFA98BC01B1.preface preserve=yes
//## end M4CheckJump%3EFA98BC01B1.preface

//## Class: M4CheckJump%3EFA98BC01B1
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS M4CheckJump 
{
  //## begin M4CheckJump%3EFA98BC01B1.initialDeclarations preserve=yes
  //## end M4CheckJump%3EFA98BC01B1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4CheckJump%1056610573
      M4CheckJump ();

    //## Destructor (specified)
      //## Operation: ~M4CheckJump%1056610574
      virtual ~M4CheckJump ();


    //## Other Operations (specified)
      //## Operation: Check%1056610577
      m4void_t Check ();

      //## Operation: Throw%1056610572
      static m4void_t Throw (M4RuntimeException e);

      //## Operation: CheckThrow%1057245449
      static M4CheckJump* CheckThrow ();

      //## Operation: getException%1057564334
      const M4RuntimeException & getException ()
      {
        //## begin M4CheckJump::getException%1057564334.body preserve=yes
		  return m_oException;
        //## end M4CheckJump::getException%1057564334.body
      }

    // Additional Public Declarations
      //## begin M4CheckJump%3EFA98BC01B1.public preserve=yes
      //## end M4CheckJump%3EFA98BC01B1.public

  protected:

    //## Other Operations (specified)
      //## Operation: Register%1056610575
      m4void_t Register ();

      //## Operation: DeRegister%1056610576
      m4void_t DeRegister ();

      //## Operation: isExceptionCached%1057576409
      m4bool_t isExceptionCached () const
      {
        //## begin M4CheckJump::isExceptionCached%1057576409.body preserve=yes
		  return m_bExceptionCached;
        //## end M4CheckJump::isExceptionCached%1057576409.body
      }

    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3EFA9EAC01CB
      //## Role: M4CheckJump::m_oJumpBuffer%3EFA9EAD00D2
      //## begin M4CheckJump::m_oJumpBuffer%3EFA9EAD00D2.role preserve=no  protected: jmp_buf { -> VHAN}
      jmp_buf m_oJumpBuffer;
      //## end M4CheckJump::m_oJumpBuffer%3EFA9EAD00D2.role

      //## Association: M4Ipcs::<unnamed>%3EFAA7C102C8
      //## Role: M4CheckJump::m_oException%3EFAA7C201D9
      //## begin M4CheckJump::m_oException%3EFAA7C201D9.role preserve=no  protected: M4RuntimeException { -> VHAN}
      M4RuntimeException m_oException;
      //## end M4CheckJump::m_oException%3EFAA7C201D9.role

    // Additional Protected Declarations
      //## begin M4CheckJump%3EFA98BC01B1.protected preserve=yes
      //## end M4CheckJump%3EFA98BC01B1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bInitialized%3EFAA00C03DA
      //## begin M4CheckJump::m_bInitialized%3EFAA00C03DA.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bInitialized;
      //## end M4CheckJump::m_bInitialized%3EFAA00C03DA.attr

      //## Attribute: m_poNext%3EFABBED0292
      //## begin M4CheckJump::m_poNext%3EFABBED0292.attr preserve=no  private: M4CheckJump * {UA} NULL
      M4CheckJump *m_poNext;
      //## end M4CheckJump::m_poNext%3EFABBED0292.attr

      //## Attribute: m_poPrevious%3EFABC060107
      //## begin M4CheckJump::m_poPrevious%3EFABC060107.attr preserve=no  private: M4CheckJump * {UA} NULL
      M4CheckJump *m_poPrevious;
      //## end M4CheckJump::m_poPrevious%3EFABC060107.attr

      //## Attribute: m_bExceptionCached%3F09556D03E1
      //## begin M4CheckJump::m_bExceptionCached%3F09556D03E1.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bExceptionCached;
      //## end M4CheckJump::m_bExceptionCached%3F09556D03E1.attr

    // Additional Private Declarations
      //## begin M4CheckJump%3EFA98BC01B1.private preserve=yes
      //## end M4CheckJump%3EFA98BC01B1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4CheckJump%3EFA98BC01B1.implementation preserve=yes
      //## end M4CheckJump%3EFA98BC01B1.implementation

};

//## begin M4CheckJump%3EFA98BC01B1.postscript preserve=yes
//## end M4CheckJump%3EFA98BC01B1.postscript

//## begin module%3742D02003D7.epilog preserve=yes
class M4_DECL_M4IPCS M4IThreadEventHandler 
{
public:
	virtual ~M4IThreadEventHandler () {};

	virtual void EndThread() = 0;
};

typedef map< m4iden_t, M4IThreadEventHandler*, less<m4iden_t> > M4ThreadDispatcherMap_t;


class M4_DECL_M4IPCS M4ThreadEventDispatcher 
{
public:
	static void AddHandler(M4IThreadEventHandler *ai_oHandler);
	static void Dispatch();

private:
	static M4ThreadDispatcherMap_t m_oMap;
	static ClMutex m_oMutex;
};

//## end module%3742D02003D7.epilog
#endif


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin M4CheckJump::getObject%1057768805.body preserve=no
		  return m_pObject;
//## end M4CheckJump::getObject%1057768805.body

#endif
