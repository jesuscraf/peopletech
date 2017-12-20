//## begin module%3A8A6FEA0280.cm preserve=no
//## end module%3A8A6FEA0280.cm

//## begin module%3A8A6FEA0280.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%3A8A6FEA0280.cp

//## Module: m4dmschannelhandler%3A8A6FEA0280; Package specification
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Source file: C:\m4server\m4dmsxml\inc\m4dmschannelhandler.hpp

#ifndef m4dmschannelhandler_h
#define m4dmschannelhandler_h 1

//## begin module%3A8A6FEA0280.additionalIncludes preserve=no
//## end module%3A8A6FEA0280.additionalIncludes

//## begin module%3A8A6FEA0280.includes preserve=yes
// ClCompiledMCR
#include <m4mdrt.hpp>
//## end module%3A8A6FEA0280.includes

#include "m4stl.hpp"
// node_x
#include <node_x.hpp>
// access
#include <access.hpp>
// channel
#include <channel.hpp>

class ClChannelManager;
struct stChannelElementInfo;

//## begin module%3A8A6FEA0280.declarations preserve=no
//## end module%3A8A6FEA0280.declarations

//## begin module%3A8A6FEA0280.additionalDeclarations preserve=yes
//## end module%3A8A6FEA0280.additionalDeclarations


//## begin ClDMSChannelHandler%3A8A690E007C.preface preserve=yes
//## end ClDMSChannelHandler%3A8A690E007C.preface

//## Class: ClDMSChannelHandler%3A8A690E007C
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A8A69C0030D;ClChannel { -> }
//## Uses: <unnamed>%3A8A69CB0042;ClAccess { -> }
//## Uses: <unnamed>%3AACF5E00156;string { -> }
//## Uses: <unnamed>%3B0B64B90299;ClNode { -> }
//## Uses: <unnamed>%3B0B6C0303AB;stChannelElementInfo { -> F}
//## Uses: <unnamed>%3B1B64150284;ClCompiledMCR { -> F}

class ClDMSChannelHandler 
{
  //## begin ClDMSChannelHandler%3A8A690E007C.initialDeclarations preserve=yes
  //## end ClDMSChannelHandler%3A8A690E007C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDMSChannelHandler%982251389
      ClDMSChannelHandler (ClChannelManager *ai_poChannelManager);

    //## Destructor (specified)
      //## Operation: ~ClDMSChannelHandler%982251398
      ~ClDMSChannelHandler ();


    //## Other Operations (specified)
      //## Operation: LoadChannel%982251390
      m4return_t LoadChannel (const string &ai_sNameChannel, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess);

      //## Operation: UnLoadChannel%982251391
      m4return_t UnLoadChannel (ClAccess *&ao_poAccess, ClChannel *&ao_poChannel);

      //## Operation: GetBuiltChannel%984388359
      m4return_t GetBuiltChannel (const string &ai_sChannelName, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess);

	  m4return_t GetNodeHandle (ClNode *ai_poNode, const string &ai_sNodeName, m4uint32_t &ao_uiNodeHandle);
	  m4return_t GetItemHandle (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiItemHandle);

      //## Operation: GetItemPrecScale%990436214
      m4return_t GetItemPrecScale (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiItemPrec, m4uint8_t &ao_uiItemScale);

      //## Operation: GetItemCppType%990436215
      m4uint8_t GetItemCppType (ClNode *ai_poNode, const string &ai_sItemName, const string &ai_sParentName);

      //## Operation: GetItemM4Type%990436216
      m4return_t GetItemM4Type (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint8_t &ao_uiItemM4Type);

      //## Operation: GetChannelByHandle%1116582877
      m4return_t GetChannelByHandle (m4int32_t ai_iChannelHandle, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess);

      //## Operation: GetChannelName%1116582878
      m4return_t GetChannelName (ClChannel *  ai_poChannel, string &aio_sChannelName);

      //## Operation: GetItemScope%1123661606
      m4return_t GetItemScope (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint8_t &ao_uiItemScope);

	  m4return_t ExecuteMethod(ClNode * ai_poNode, const string & ai_sNodeName, const string & ai_sMethodName, m4int32_t ai_ArgValue, m4return_t & ao_iResult);

	  m4return_t GetItemAux (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiAuxItemHandle, string& ao_sAuxItemId);

    // Additional Public Declarations
      //## begin ClDMSChannelHandler%3A8A690E007C.public preserve=yes
      //## end ClDMSChannelHandler%3A8A690E007C.public

  protected:
    // Additional Protected Declarations
      //## begin ClDMSChannelHandler%3A8A690E007C.protected preserve=yes
      //## end ClDMSChannelHandler%3A8A690E007C.protected

  private:
    // Data Members for Associations

      //## Association: M4DMSXml::<unnamed>%3A8A69860273
      //## Role: ClDMSChannelHandler::m_poChannelManager%3A8A6987006C
      //## begin ClDMSChannelHandler::m_poChannelManager%3A8A6987006C.role preserve=no  private: ClChannelManager { -> RFHAN}
      ClChannelManager *m_poChannelManager;
      //## end ClDMSChannelHandler::m_poChannelManager%3A8A6987006C.role

    // Additional Private Declarations
      //## begin ClDMSChannelHandler%3A8A690E007C.private preserve=yes
      //## end ClDMSChannelHandler%3A8A690E007C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDMSChannelHandler%3A8A690E007C.implementation preserve=yes
      //## end ClDMSChannelHandler%3A8A690E007C.implementation

};

//## begin ClDMSChannelHandler%3A8A690E007C.postscript preserve=yes
//## end ClDMSChannelHandler%3A8A690E007C.postscript

// Class ClDMSChannelHandler 

//## begin module%3A8A6FEA0280.epilog preserve=yes
//## end module%3A8A6FEA0280.epilog


#endif
