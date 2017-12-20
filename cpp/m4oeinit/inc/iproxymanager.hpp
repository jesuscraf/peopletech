//## begin module%390956250034.cm preserve=no
//## end module%390956250034.cm

//## begin module%390956250034.cp preserve=no
//	=============================================================================
//
//	(c) Copyright  1991-1997 Meta Software M.S., S.A
//	All rights reserved.
//
//	Module:            <module>
//	File:              iproxymanager.hpp
//	Project:           M4OeInit
//	Author:            Meta Software M.S. , S.A
//	Date:
//	Language:          C++
//	Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	Design Document:
//
//	Definition:
//
//	   This module defines...
//	   El interface de un proxy manager.
//
//	=============================================================================
//## end module%390956250034.cp

//## Module: IProxyManager%390956250034; Subprogram specification
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Source file: D:\Work\v600\m4oeinit\inc\iproxymanager.hpp

#ifndef iproxymanager_h
#define iproxymanager_h 1

//## begin module%390956250034.additionalIncludes preserve=no
//## end module%390956250034.additionalIncludes

//## begin module%390956250034.includes preserve=yes
#include "m4types.hpp"
#include "m4oeinit_dll.hpp"
//## end module%390956250034.includes


class ClProxySpace;
class ClCache;

//## begin module%390956250034.declarations preserve=no
//## end module%390956250034.declarations

//## begin module%390956250034.additionalDeclarations preserve=yes
//## end module%390956250034.additionalDeclarations


//## begin IProxyManager%3821CD430145.preface preserve=yes
//## end IProxyManager%3821CD430145.preface

//## Class: IProxyManager%3821CD430145; Abstract
//	Interfaz del gestor de espacios proxy.
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3826F564005A;ClCache { -> F}
//## Uses: <unnamed>%39212ED503E6;ClProxySpace { -> F}

class IProxyManager 
{
  //## begin IProxyManager%3821CD430145.initialDeclarations preserve=yes
  //## end IProxyManager%3821CD430145.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~IProxyManager%942225406
      //	Destructor. Permito derivaciones.
      virtual ~IProxyManager ();


    //## Other Operations (specified)
      //## Operation: GetSpace%941739751
      //	Devuelve un espacio proxy. Este se selecciona por el parámetro ai_pcClProxy
      //	SpaceID.
      //	Si el espacio proxy no existe se crea uno nuevo si ai_bCreateNew == M4_TRUE.
      //
      //	@param ai_pcClProxySpaceID    Id del espacio proxy que se quiere.
      //	@param ai_bCreateNew    Si no existe el espacio proxy y se pasa el valor M4_
      //	TRUE --> se crea un nuevo espacio proxy.
      //	@return ClProxySpace    Espacio proxy. Si no se encontro y ai_bCreateNew ==
      //	M4_FALSE --> se devuelve NULL.
      virtual ClProxySpace* const GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew = M4_TRUE) = 0;

      //## Operation: ReleaseSpace%941739752
      //	Libera el espacio proxy que se pasa como parametro.
      virtual m4return_t ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock = M4_TRUE) = 0;

      //## Operation: SetCaches%941739753
      //	Fija las caches para el gestor de espacios proxy.
      virtual void SetCaches (ClCache* const ai_poCMCRCache, ClCache* const ai_poCSCRCache, ClCache* const ai_poPresentationCache, ClCache* const ai_poMapCache, ClCache* const ai_poDatacache) = 0;

      //## Operation: GetCaches%941739754
      //	Devuelve las caches del gestor de espacios proxy.
      virtual void GetCaches (ClCache * &ao_poCMCRCache, ClCache * &ao_poCSCRCache, ClCache * &ao_poPresentationCache, ClCache * &ao_poMapCache, ClCache * &ao_poDatacache) const = 0;

      //## Operation: IsServerMode%941739755
      //	Indica si estamos en el lado servidor (válido para DIC o SM).
      virtual m4bool_t IsServerMode () const = 0;

      //## Operation: SendEvent%941739756
      virtual m4return_t SendEvent () = 0;

      //## Operation: ReleaseActiveSpace%949597707
      virtual m4return_t ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID) = 0;

      //## Operation: Reset%994166233
      //	Hace un reset de los miembros internos después de un logoff, por ejemplo.
      virtual void Reset () = 0;

      //## Operation: EraseAllL2Instances%1025256482
      virtual m4return_t EraseAllL2Instances (const m4char_t* const ai_pcClProxySpaceID, m4uint32_t ai_iRemoteId) = 0;

    // Additional Public Declarations
      //## begin IProxyManager%3821CD430145.public preserve=yes
      //## end IProxyManager%3821CD430145.public

  protected:
    //## Constructors (specified)
      //## Operation: IProxyManager%942225407
      //	Crea un nuevo IProxyManager. No permitimos que nadie llame al constructor.
      IProxyManager ();

    // Additional Protected Declarations
      //## begin IProxyManager%3821CD430145.protected preserve=yes
      //## end IProxyManager%3821CD430145.protected

  private:
    // Additional Private Declarations
      //## begin IProxyManager%3821CD430145.private preserve=yes
      //## end IProxyManager%3821CD430145.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IProxyManager%3821CD430145.implementation preserve=yes
      //## end IProxyManager%3821CD430145.implementation

};

//## begin IProxyManager%3821CD430145.postscript preserve=yes
//## end IProxyManager%3821CD430145.postscript

// Class IProxyManager 

inline IProxyManager::IProxyManager ()
  //## begin IProxyManager::IProxyManager%942225407.hasinit preserve=no
  //## end IProxyManager::IProxyManager%942225407.hasinit
  //## begin IProxyManager::IProxyManager%942225407.initialization preserve=yes
  //## end IProxyManager::IProxyManager%942225407.initialization
{
  //## begin IProxyManager::IProxyManager%942225407.body preserve=yes
  //## end IProxyManager::IProxyManager%942225407.body
}


inline IProxyManager::~IProxyManager ()
{
  //## begin IProxyManager::~IProxyManager%942225406.body preserve=yes
  //## end IProxyManager::~IProxyManager%942225406.body
}


//## begin module%390956250034.epilog preserve=yes
//## end module%390956250034.epilog


#endif
