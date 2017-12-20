//## begin module%3CF48A6E00CF.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CF48A6E00CF.cm

//## begin module%3CF48A6E00CF.cp preserve=no
//## end module%3CF48A6E00CF.cp

//## Module: sessioncconst%3CF48A6E00CF; Package specification
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Source file: F:\integration\m4serverbase\inc\sessioncconst.h

#ifndef sessioncconst_h
#define sessioncconst_h 1

//## begin module%3CF48A6E00CF.additionalIncludes preserve=no
//## end module%3CF48A6E00CF.additionalIncludes

//## begin module%3CF48A6E00CF.includes preserve=yes
#include <sessioncontext.hpp>
//## end module%3CF48A6E00CF.includes

//## begin module%3CF48A6E00CF.declarations preserve=no
//## end module%3CF48A6E00CF.declarations

//## begin module%3CF48A6E00CF.additionalDeclarations preserve=yes
//## end module%3CF48A6E00CF.additionalDeclarations


//## begin IContextConstructor%3CF4DA510068.preface preserve=yes
//## end IContextConstructor%3CF4DA510068.preface

//## Class: IContextConstructor%3CF4DA510068; Abstract
//## Category: M4ServerBase::session context%3CF3B3120047
//## Subsystem: M4ServerBase::Session Context::inc%3CF48A9401BA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class IContextConstructor 
{
  //## begin IContextConstructor%3CF4DA510068.initialDeclarations preserve=yes
  //## end IContextConstructor%3CF4DA510068.initialDeclarations

  public:
    //## Constructors (generated)
      IContextConstructor();


    //## Destructor (generated)
      ~IContextConstructor();


    // Additional Public Declarations
      //## begin IContextConstructor%3CF4DA510068.public preserve=yes
      //## end IContextConstructor%3CF4DA510068.public

  protected:
    // Additional Protected Declarations
      //## begin IContextConstructor%3CF4DA510068.protected preserve=yes
      //## end IContextConstructor%3CF4DA510068.protected

  private:
    // Additional Private Declarations
      //## begin IContextConstructor%3CF4DA510068.private preserve=yes
      //## end IContextConstructor%3CF4DA510068.private

  private: //## implementation
    // Data Members for Associations

      //## Association: M4ServerBase::session context::<unnamed>%3CF4E3D50129
      //## begin IContextConstructor::<the_ClSessionContext>%3CF4E3D600DB.role preserve=no  public: ClSessionContext { -> UHN}
      ClSessionContext * m_poSessionContext;
      //## end IContextConstructor::<the_ClSessionContext>%3CF4E3D600DB.role

    // Additional Implementation Declarations
      //## begin IContextConstructor%3CF4DA510068.implementation preserve=yes
      //## end IContextConstructor%3CF4DA510068.implementation

};

//## begin IContextConstructor%3CF4DA510068.postscript preserve=yes
//## end IContextConstructor%3CF4DA510068.postscript

// Class IContextConstructor 


//## begin module%3CF48A6E00CF.epilog preserve=yes
//## end module%3CF48A6E00CF.epilog


#endif
