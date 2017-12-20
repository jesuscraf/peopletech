//## begin module%38EB1AC60281.cm preserve=no
//## end module%38EB1AC60281.cm

//## begin module%38EB1AC60281.cp preserve=no
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
//## end module%38EB1AC60281.cp

//## Module: TransportLayerContainer%38EB1AC60281; Package specification
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Source file: F:\integration\m4transport\inc\transportlayercontainer.hpp

#ifndef transportlayercontainer_h
#define transportlayercontainer_h 1

//## begin module%38EB1AC60281.additionalIncludes preserve=no
//## end module%38EB1AC60281.additionalIncludes

//## begin module%38EB1AC60281.includes preserve=yes
//## end module%38EB1AC60281.includes

// CommComun
#include <commcomun.hpp>
// TransportLayerBase
#include <transportlayerbase.hpp>
// tsap
#include <tsap.hpp>
// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// RecycleableCollector
#include <recycleablecollector.hpp>
//## begin module%38EB1AC60281.declarations preserve=no
//## end module%38EB1AC60281.declarations

//## begin module%38EB1AC60281.additionalDeclarations preserve=yes
//## end module%38EB1AC60281.additionalDeclarations


//## begin ClTransportLayerContainer%38EB0E6D0177.preface preserve=yes
//## end ClTransportLayerContainer%38EB0E6D0177.preface

//## Class: ClTransportLayerContainer%38EB0E6D0177
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTransportLayerContainer 
{
  //## begin ClTransportLayerContainer%38EB0E6D0177.initialDeclarations preserve=yes
  //## end ClTransportLayerContainer%38EB0E6D0177.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportLayerContainer%956912723
      ClTransportLayerContainer ();

    //## Destructor (specified)
      //## Operation: ~ClTransportLayerContainer%958576517
      ~ClTransportLayerContainer ();


    //## Other Operations (specified)
      //## Operation: AddTransportLayerIdentifier%954835578
      m4return_t AddTransportLayerIdentifier (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier);

      //## Operation: RemoveTransportLayerIdentifier%954835579
      m4return_t RemoveTransportLayerIdentifier (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier);

      //## Operation: AddSelect%954835580
      m4return_t AddSelect (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier, m4bool_t ai_bRead, m4bool_t ai_bWrite);

      //## Operation: RemoveSelect%954835581
      m4return_t RemoveSelect (ClTransportLayerIdentifier *ai_pTransportLayerIdentifier, m4bool_t ai_bRead, m4bool_t ai_bWrite);

      //## Operation: Select%954835582
      int Select (unsigned long ai_Seconds, unsigned long ai_Useconds);

      //## Operation: AddAction%954835583
      m4return_t AddAction (ClTransportAction *ai_pAction);

      //## Operation: GetAction%954835584
      ClTransportAction * GetAction ();

      //## Operation: NextRead%954835585
      ClTransportLayerIdentifier * NextRead ();

      //## Operation: NextWrite%954835586
      ClTransportLayerIdentifier * NextWrite ();

      //## Operation: GetTransportLayerIdentifier%955466403
      ClTransportLayerIdentifier * GetTransportLayerIdentifier (ClTSAP *ai_pTSAP);

      //## Operation: WantStop%956912721
      m4bool_t WantStop ();

      //## Operation: SetWantStop%956912722
      void SetWantStop (ClIOEventHandler *ai_iIOEventHandler);

      //## Operation: GetWantStopIOEventHandler%956912725
      ClIOEventHandler * GetWantStopIOEventHandler ();

      //## Operation: RecoverAction%961597592
      IGarbageable * RecoverAction (int ai_iType);

      //## Operation: KeepAction%961597593
      m4return_t KeepAction (IGarbageable &ai_oGarbageable);

    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%3C7CB76B00C2
      //## Role: ClTransportLayerContainer::m_MapTransportLayerIdentifier%3C7CB76C0092
      //## begin ClTransportLayerContainer::m_MapTransportLayerIdentifier%3C7CB76C0092.role preserve=no  public: ClMapTransportLayerIdentifier { -> UHAN}
      ClMapTransportLayerIdentifier m_MapTransportLayerIdentifier;
      //## end ClTransportLayerContainer::m_MapTransportLayerIdentifier%3C7CB76C0092.role

    // Additional Public Declarations
      //## begin ClTransportLayerContainer%38EB0E6D0177.public preserve=yes
      //## end ClTransportLayerContainer%38EB0E6D0177.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38D210AF01BD
      //## Role: ClTransportLayerContainer::m_ListTransportAction%38D210B00056
      //## begin ClTransportLayerContainer::m_ListTransportAction%38D210B00056.role preserve=no  protected: ClListTransportAction { -> VHAN}
      ClListTransportAction m_ListTransportAction;
      //## end ClTransportLayerContainer::m_ListTransportAction%38D210B00056.role

      //## Association: m4transport::M4CTL::<unnamed>%38F2E66B02D9
      //## Role: ClTransportLayerContainer::m_TSAPSelect%38F2E66C01C3
      //## begin ClTransportLayerContainer::m_TSAPSelect%38F2E66C01C3.role preserve=no  protected: ClTSAPSelect { -> VHAN}
      ClTSAPSelect m_TSAPSelect;
      //## end ClTransportLayerContainer::m_TSAPSelect%38F2E66C01C3.role

      //## Association: m4transport::M4CTL::<unnamed>%390972650220
      //## Role: ClTransportLayerContainer::m_bWantStop%390972660019
      //## begin ClTransportLayerContainer::m_bWantStop%390972660019.role preserve=no  protected: m4bool_t { -> UHAN}
      m4bool_t m_bWantStop;
      //## end ClTransportLayerContainer::m_bWantStop%390972660019.role

      //## Association: m4transport::M4CTL::<unnamed>%3909793E03D9
      //## Role: ClTransportLayerContainer::m_pWantStopClIOEventHandler%3909793F01F0
      //## begin ClTransportLayerContainer::m_pWantStopClIOEventHandler%3909793F01F0.role preserve=no  protected: ClIOEventHandler { -> RHAN}
      ClIOEventHandler *m_pWantStopClIOEventHandler;
      //## end ClTransportLayerContainer::m_pWantStopClIOEventHandler%3909793F01F0.role

      //## Association: m4transport::M4CTL::<unnamed>%3950CDF2038B
      //## Role: ClTransportLayerContainer::m_RecycleableCollector%3950CDF4024E
      //## begin ClTransportLayerContainer::m_RecycleableCollector%3950CDF4024E.role preserve=no  protected: ClRecycleableCollector { -> VHAN}
      ClRecycleableCollector m_RecycleableCollector;
      //## end ClTransportLayerContainer::m_RecycleableCollector%3950CDF4024E.role

      //## Association: m4transport::M4CTL::<unnamed>%39572A390122
      //## Role: ClTransportLayerContainer::m_SyncListAction%39572A3903DF
      //## begin ClTransportLayerContainer::m_SyncListAction%39572A3903DF.role preserve=no  protected: ClCriticalSection { -> VHAN}
      ClCriticalSection m_SyncListAction;
      //## end ClTransportLayerContainer::m_SyncListAction%39572A3903DF.role

      //## Association: m4transport::M4CTL::<unnamed>%39572D3F012F
      //## Role: ClTransportLayerContainer::m_SyncGarbageableCollector%39572D40020D
      //## begin ClTransportLayerContainer::m_SyncGarbageableCollector%39572D40020D.role preserve=no  protected: ClCriticalSection { -> VHAN}
      ClCriticalSection m_SyncGarbageableCollector;
      //## end ClTransportLayerContainer::m_SyncGarbageableCollector%39572D40020D.role

    // Additional Protected Declarations
      //## begin ClTransportLayerContainer%38EB0E6D0177.protected preserve=yes
      //## end ClTransportLayerContainer%38EB0E6D0177.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pTransporIdentifierArray%3C7B61750187
      //## begin ClTransportLayerContainer::m_pTransporIdentifierArray%3C7B61750187.attr preserve=no  private: ClTransportLayerIdentifier ** {UA} 
      ClTransportLayerIdentifier **m_pTransporIdentifierArray;
      //## end ClTransportLayerContainer::m_pTransporIdentifierArray%3C7B61750187.attr

      //## Attribute: m_lTSAPId%3C7BB6BA01DA
      //## begin ClTransportLayerContainer::m_lTSAPId%3C7BB6BA01DA.attr preserve=no  private: static m4uint32_t {UA} 5
      static m4uint32_t m_lTSAPId;
      //## end ClTransportLayerContainer::m_lTSAPId%3C7BB6BA01DA.attr

    // Additional Private Declarations
      //## begin ClTransportLayerContainer%38EB0E6D0177.private preserve=yes
      //## end ClTransportLayerContainer%38EB0E6D0177.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportLayerContainer%38EB0E6D0177.implementation preserve=yes
      //## end ClTransportLayerContainer%38EB0E6D0177.implementation

};

//## begin ClTransportLayerContainer%38EB0E6D0177.postscript preserve=yes
//## end ClTransportLayerContainer%38EB0E6D0177.postscript

// Class ClTransportLayerContainer 

//## begin module%38EB1AC60281.epilog preserve=yes
//## end module%38EB1AC60281.epilog


#endif
