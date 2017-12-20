//## begin module%358FEA98008C.cm preserve=no
//## end module%358FEA98008C.cm

//## begin module%358FEA98008C.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%358FEA98008C.cp

//## Module: Clienttag%358FEA98008C; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\version\src\clienttag.cpp

//## begin module%358FEA98008C.additionalIncludes preserve=no
//## end module%358FEA98008C.additionalIncludes

//## begin module%358FEA98008C.includes preserve=yes
#include <m4types.hpp>
#include <m4trace.hpp>
#include <m4srvres.hpp>
#include <m4log.hpp>
#include <basiclog.hpp>
#include <m4srvtrace.h>
#include <m4servervt.hpp>
#include <clienttag.hpp>
#include <datastorages.hpp>

#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%358FEA98008C.includes

//## begin module%358FEA98008C.declarations preserve=no
//## end module%358FEA98008C.declarations

//## begin module%358FEA98008C.additionalDeclarations preserve=yes
//## end module%358FEA98008C.additionalDeclarations


// Class ClTagList 





ClTagList::ClTagList ()
  //## begin ClTagList::ClTagList%897932191.hasinit preserve=no
      : m_uMapMaxSize(0), m_uMapSize(0)
  //## end ClTagList::ClTagList%897932191.hasinit
  //## begin ClTagList::ClTagList%897932191.initialization preserve=yes
  //## end ClTagList::ClTagList%897932191.initialization
{
  //## begin ClTagList::ClTagList%897932191.body preserve=yes
	m_uMapMaxSize = (m4uint32_t)m_oTagMap.max_size();

	m_uMapSize =0;
	m_TagMapIterator=m_oTagMap.end();
  //## end ClTagList::ClTagList%897932191.body
}


ClTagList::~ClTagList ()
{
  //## begin ClTagList::~ClTagList%897932190.body preserve=yes

	Clear();
	m_uMapMaxSize = 0;
	m_uMapSize = 0;

  //## end ClTagList::~ClTagList%897932190.body
}



//## Other Operations (implementation)
m4return_t ClTagList::SetTag (m4pchar_t ai_pcTag)
{
  //## begin ClTagList::SetTag%897932192.body preserve=yes
	Tag_Map::iterator eliterador;

#ifndef _aCC
	pair< m4pchar_t , m4ServerVT> parIn;
	pair<map < m4pchar_t , m4ServerVT , LessString>::iterator , bool> parRet;
	
#endif

	if (m_uMapSize >= m_uMapMaxSize) 
	{
		M4_SRV_ERROR_N(M4_ERR_TAG_ERROR,"MaxSize of ClTagList reach");
		return M4_ERROR;
	}
	
	eliterador=m_oTagMap.find(ai_pcTag);
	if (eliterador != m_oTagMap.end()) 
	{
		M4_SRV_DEBUG( "Tag %0:s not found", ai_pcTag);
		return M4_ERROR;
	}
	


;

	m4pchar_t 	tagname=new m4char_t[strlen(ai_pcTag)+1];
	
	strcpy(tagname,ai_pcTag);

	m4ServerVT m_oVT;

	m_oVT.SetNull();
	

#ifndef _aCC	
	parIn.first=tagname;
	parIn.second=m_oVT;

	parRet=m_oTagMap.insert(parIn);

	if (parRet.second)
	{
		m_uMapSize++;
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Error inserting %0:s",ai_pcTag);
		return M4_ERROR;
	}

#else
	if (m_oTagMap.insert(Tag_Map::value_type(tagname,m_oVT)).second)
		return M4_SUCCESS;
	else 
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Error inserting %0:s",ai_pcTag);
		return M4_ERROR;
	}
#endif
  //## end ClTagList::SetTag%897932192.body
}

m4return_t ClTagList::GetArg (m4pchar_t ai_pcTag, m4ServerVT &ao_vArg)
{
  //## begin ClTagList::GetArg%897932193.body preserve=yes
	Tag_Map::iterator eliterador;
	eliterador=m_oTagMap.find(ai_pcTag);

	if (eliterador != m_oTagMap.end())
	{
		ao_vArg=(* eliterador).second;
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_DEBUG( "Tag %0:s not found", ai_pcTag);
		return M4_ERROR;
	}
  //## end ClTagList::GetArg%897932193.body
}

m4return_t ClTagList::DelTag (m4pchar_t ai_pcTag)
{
  //## begin ClTagList::DelTag%898005170.body preserve=yes

	m4int8_t iret;
	StartIteration();
	iret=(m4int8_t)m_oTagMap.erase(ai_pcTag);

	if (iret)
	{
		m_uMapSize--;
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Error deleting Tag: %0:s",ai_pcTag);
		return M4_ERROR;
	}

  //## end ClTagList::DelTag%898005170.body
}

void ClTagList::Clear ()
{
  //## begin ClTagList::Clear%898005171.body preserve=yes
	Tag_Map::iterator eliterador;
	m4ServerVT oVT1;
	
	M4_SRV_DEBUG_BLOCK_N( Blk, "ClTagList::Clear()" )  ;	
	eliterador=m_oTagMap.begin();

	while (eliterador!=m_oTagMap.end())
	{
		delete (*eliterador).first;
		(*eliterador).second.FreeData();
		m_oTagMap.erase(eliterador);
		eliterador=m_oTagMap.begin();
	}	
	m_uMapSize=0;
  //## end ClTagList::Clear%898005171.body
}

m4return_t ClTagList::SetArg (m4pchar_t ai_pcTag, m4ServerVT ai_vArg)
{
  //## begin ClTagList::SetArg%898080527.body preserve=yes

	if(!ai_pcTag)
	{
		M4_SRV_ERROR_N(M4_ERR_TAG_ERROR, "NULL Tag");
		return M4_ERROR;
	}
	
	Tag_Map::iterator eliterador;

	eliterador=m_oTagMap.find(ai_pcTag);
	if (eliterador == m_oTagMap.end()) // Es la primera vez que lo ponemos
	{
		m4pchar_t tagname;
		m4int32_t tagsize;
		
		tagsize=strlen(ai_pcTag);
		tagname=new m4char_t[tagsize+1];
		
		strcpy(tagname,ai_pcTag);
	
		m4ServerVT oVT(ai_vArg);

#ifndef _aCC
		pair< m4pchar_t , m4ServerVT> parIn;
		pair<map < m4pchar_t , m4ServerVT ,LessString >::iterator , bool > parRet;
		parIn.first=tagname;
		parIn.second=oVT;
		parRet=m_oTagMap.insert(parIn);

		if (parRet.second)
		{
			m_uMapSize++;
			return M4_SUCCESS;
		}
		else
		{
			M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Error inserting %0:s",ai_pcTag);
			return M4_ERROR;
		}
#else
		if (m_oTagMap.insert(Tag_Map::value_type(tagname,oVT)).second)
		{
			m_uMapSize++;
			return M4_SUCCESS;
		}
		else
		{
			M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Error inserting %0:s",ai_pcTag);
			return M4_ERROR;
		}
#endif
	}
	else
	{
		m4ServerVT oVT(ai_vArg);
		(*eliterador).second.FreeData();
		m_oTagMap[ai_pcTag]=oVT;
		return M4_SUCCESS;
	}

  //## end ClTagList::SetArg%898080527.body
}

m4return_t ClTagList::SetArg (m4pchar_t ai_pcString)
{
  //## begin ClTagList::SetArg%911836552.body preserve=yes

	if(!ai_pcString)
	{
		M4_SRV_ERROR_N(M4_ERR_TAG_ERROR,"NULL Tag");
		return M4_ERROR;
	}
	m4char_t pcTagName[256];
	m4ServerVT oVT1;
	if(Str2TagArg(ai_pcString,pcTagName,256,oVT1)==M4_ERROR)
		return M4_ERROR;
	if(SetArg(pcTagName,oVT1)==M4_ERROR)
		return M4_ERROR;
	return M4_SUCCESS;		
  //## end ClTagList::SetArg%911836552.body
}

m4return_t ClTagList::SetArg (m4pchar_t ai_pcTag, m4pchar_t ai_pcString)
{
  //## begin ClTagList::SetArg%911836553.body preserve=yes
	if ((ai_pcTag==NULL) ||  !(ai_pcTag[0]) ||  (ai_pcString==NULL) ||  !(ai_pcString[0]))
	{
		M4_SRV_ERROR( 0, "Invalid Tag %0:s = Value %1:s", ai_pcTag << ai_pcString ); 
		return M4_ERROR;
	}

//OSCAR
//mal hecho - Si se pasa en constructor se guarda como pointer, no como string
//	m4ServerVT oArg (ai_pcString) ;

//Así si:
	m4ServerVT oArg;

	oArg.SetString (ai_pcString) ;
//END OSCAR
	return (SetArg(ai_pcTag,oArg));

  //## end ClTagList::SetArg%911836553.body
}

m4return_t ClTagList::SetArg (m4pchar_t ai_pcTag, m4double_t ai_dNumber)
{
  //## begin ClTagList::SetArg%911836554.body preserve=yes
	m4ServerVT oArg(ai_dNumber);

	return (SetArg(ai_pcTag,oArg)); 

  //## end ClTagList::SetArg%911836554.body
}

m4return_t ClTagList::GetArg (m4pchar_t ai_pcTag, m4pchar_t &ao_pcArg, m4uint32_t ai_iSizeBuffer)
{
  //## begin ClTagList::GetArg%912423223.body preserve=yes

	if ((ao_pcArg==NULL) || !(ai_iSizeBuffer))
	{
		M4_SRV_ERROR( 0, "Invalid Tag %0:s", ai_pcTag  );
		return M4_ERROR;
	}

	m4ServerVT oVT1;
	m4return_t iRet;

	iRet=GetArg(ai_pcTag,oVT1);
	if(M4_ERROR==iRet)
	{
		M4_SRV_DEBUG( "Tag %0:s not found", ai_pcTag);
		return M4_ERROR;
	}
// Cambio el comportamiento. Solo devuelvo el valor
//	return TagArg2Str(ai_pcTag,oVT1,ao_pcArg,ai_iSizeBuffer);
	if((oVT1.GetType()==M4_SVT_TYPE_STRING) || (oVT1.GetType()==M4_SVT_TYPE_STRING_VAR) )
	{
		m4pchar_t thepointer=oVT1.Data.PointerChar;
		if(thepointer)
		{
			if(strlen(thepointer)>ai_iSizeBuffer)
			{
				M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTag);
				return M4_ERROR;
			}
			else
			{
				strcpy(ao_pcArg,thepointer);
				return M4_SUCCESS;
			}
		}
		else
		{
			M4_SRV_ERROR(M4_ERR_TAG_ERROR,"Null Tag:%0:s",ai_pcTag);
			return M4_ERROR;
		}
	}
	else
	{
//		M4_SRV_ERROR(M4_ERR_INCORRECT_TAG_TYPE, "Diferent type in tag %0:s",ai_pcTag);
		return M4_ERROR;
	}

  //## end ClTagList::GetArg%912423223.body
}

m4return_t ClTagList::GetNextTag (m4pchar_t ao_pcTagName, m4uint32_t ai_iBufferSize, m4ServerVT& ao_ovArg)
{
  //## begin ClTagList::GetNextTag%913214647.body preserve=yes
	if (m_TagMapIterator==m_oTagMap.end())
	{
		// No es un error, solo que hemos llegado al final	
		M4_SRV_DEBUG_N( "End of taglist reach");
		return M4_ERROR;
	}

	if(ai_iBufferSize>strlen((*m_TagMapIterator).first))   
	{
		strcpy(ao_pcTagName,(*m_TagMapIterator).first);
		ao_ovArg=(*m_TagMapIterator).second;
		m_TagMapIterator++;
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ao_pcTagName);
		return M4_ERROR;
	}

  //## end ClTagList::GetNextTag%913214647.body
}

m4int32_t ClTagList::GetSize ()
{
  //## begin ClTagList::GetSize%913214648.body preserve=yes
	return m_uMapSize;
  //## end ClTagList::GetSize%913214648.body
}

m4return_t ClTagList::StartIteration ()
{
  //## begin ClTagList::StartIteration%913214649.body preserve=yes
	m_TagMapIterator=m_oTagMap.begin();
	if(m_TagMapIterator!=m_oTagMap.end())
		return M4_SUCCESS;
	else
	{
		M4_SRV_DEBUG_N( "Empty Taglist");
		return M4_ERROR;
	}
  //## end ClTagList::StartIteration%913214649.body
}

m4return_t ClTagList::GetNextTag (m4pchar_t ao_pcTagArg, m4uint32_t ai_uiBufferSize)
{
  //## begin ClTagList::GetNextTag%913214650.body preserve=yes
	if (m_TagMapIterator==m_oTagMap.end())
	{
		//No es un error, solo que ya estamos en el final
		M4_SRV_DEBUG_N( "End of taglist reach");
		return M4_ERROR;
	}
	if(TagArg2Str((*m_TagMapIterator).first,(*m_TagMapIterator).second,ao_pcTagArg,ai_uiBufferSize)==M4_ERROR)
		return M4_ERROR;
	m_TagMapIterator++;
	return M4_SUCCESS;
  //## end ClTagList::GetNextTag%913214650.body
}

m4return_t ClTagList::TagArg2Str (m4pchar_t ai_pcTagName, m4ServerVT ai_oVariant, m4pchar_t ao_pcBuffer, m4uint32_t ai_uiBufferSize)
{
  //## begin ClTagList::TagArg2Str%913214651.body preserve=yes

	m4uint32_t iSize=0,iSizeAux=0;
	m4pchar_t pcBuffer=ao_pcBuffer;
	m4pchar_t pcBufferAux=NULL;

// COPIAMOS EL NOMBRE
	if(ai_oVariant.GetType()==M4_SVT_TYPE_DOUBLE)
	{
		if(iSize>=ai_uiBufferSize)
		{
			M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTagName);
			return M4_ERROR;
		}
		iSize=3;
		strcpy(pcBuffer,":D:");
		pcBuffer+=3;
	}
	iSize+=iSizeAux=strlen(ai_pcTagName);
	if(iSize>=ai_uiBufferSize)
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTagName);
		return M4_ERROR;
	}

	strcpy(pcBuffer,ai_pcTagName);
	pcBuffer+=iSizeAux;
// COPIAMOS EL =
	iSize++;	
	if(iSize>=ai_uiBufferSize)
	{
		M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTagName);
		return M4_ERROR;
	}
	strcpy(pcBuffer,"=");
	pcBuffer++;
// COPIAMOS EL VALOR
	switch(ai_oVariant.GetType())
	{
	case M4_SVT_TYPE_STRING:
	case M4_SVT_TYPE_STRING_VAR:
		if(ai_oVariant.Data.PointerChar)
		{
			iSize+=strlen(ai_oVariant.Data.PointerChar);
			if(iSize>=ai_uiBufferSize)
			{
				M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTagName);
				return M4_ERROR;
			}
			strcpy(pcBuffer,ai_oVariant.Data.PointerChar);
		}
		break;
	case M4_SVT_TYPE_DOUBLE:
		pcBufferAux=new m4char_t[256];
		sprintf(pcBufferAux,"%f",ai_oVariant.Data.DoubleData);
		iSize+=strlen(pcBufferAux);
		if(iSize>=ai_uiBufferSize)
		{
			M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Not enough size in buffer: %0:s",ai_pcTagName);
			return M4_ERROR;
		}
		strcpy(pcBuffer,pcBufferAux);
		delete pcBufferAux;
		break;
	case M4_SVT_TYPE_POINTER:
	case M4_SVT_TYPE_NULL:
		break;
	default:
		M4_SRV_ERROR(0, "Invalid Data Type %0:s",  ai_oVariant.GetType() );
		return M4_WARNING;
		break;
	}
return M4_SUCCESS;
  //## end ClTagList::TagArg2Str%913214651.body
}

m4return_t ClTagList::TagArg2Str (Tag_Map::iterator ai_iterator, m4pchar_t ao_pcBuffer, m4uint32_t ai_uiBufferSize)
{
  //## begin ClTagList::TagArg2Str%913214652.body preserve=yes
	if (ai_iterator==m_oTagMap.end())
	{
		M4_SRV_DEBUG_N( "Tag not found" );
		return M4_ERROR;
	}
	return TagArg2Str((*ai_iterator).first,(*ai_iterator).second,ao_pcBuffer,ai_uiBufferSize);

  //## end ClTagList::TagArg2Str%913214652.body
}

m4return_t ClTagList::Str2TagArg (m4pchar_t ai_pcBuffer, m4pchar_t ao_pcTagName, m4uint32_t ai_uiTagNameSize, m4ServerVT &ao_oVariant)
{
  //## begin ClTagList::Str2TagArg%913214653.body preserve=yes

	m4pchar_t pcBufferIgual=NULL;
	m4pchar_t pcBuffer2Points=NULL;
	m4pchar_t pcBufferTagName=new m4char_t[strlen(ai_pcBuffer)+1];
	m4pchar_t pcBuffer2Deallocate=pcBufferTagName;

	m4uint32_t uiSize=0,uiTotalSize=0;
	m4char_t cType=0;

	uiTotalSize=strlen(ai_pcBuffer);
	strcpy(pcBufferTagName,ai_pcBuffer);

	if((pcBufferIgual=strchr(pcBufferTagName,'='))==NULL) 
	{
		delete [] pcBuffer2Deallocate;
		M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Wrong format in tag %0:s",ai_pcBuffer);
		return 	M4_ERROR; // No encuentra el igual
	}
	
	*pcBufferIgual=0;
	uiSize=strlen(pcBufferTagName);
	if(ai_uiTagNameSize<uiSize)
	{
		delete [] pcBuffer2Deallocate;
		M4_SRV_ERROR_N(M4_ERR_TAG_ERROR, "Not enough buffer");
		return M4_ERROR;
	}
	if(strchr(pcBufferTagName,':')!=NULL)
	{
		cType=pcBufferTagName[1];
		pcBufferTagName+=3;
	}
	strcpy(ao_pcTagName,pcBufferTagName);

	if((uiSize+1)<uiTotalSize)
	{	// tiene Arg
		pcBufferIgual++; // Ahora apunta al comienzo del arg
		
// En el caso de que la tag contenga un double nos lo pasaran con ":D:" en el principio
// Dejamos el switch por si establecemos mas tipos
		if(!cType) 
		{	// Es una cadena
			ao_oVariant.SetString(pcBufferIgual);
			delete [] pcBuffer2Deallocate;
			return M4_SUCCESS;
		}
		else
		{
			switch(cType)
			{
			case 'D':
				ao_oVariant.SetDouble(atof(pcBufferIgual));
				delete [] pcBuffer2Deallocate;
				return M4_SUCCESS;
				break;
			default:
				delete [] pcBuffer2Deallocate;
				M4_SRV_ERROR(M4_ERR_TAG_ERROR, "Wrong TYPE in tag %0:s",ai_pcBuffer);
				return M4_ERROR;
			break;
			}
		}
	}
	else
	{	// no tiene Arg
		ao_oVariant.SetNull();
		delete [] pcBuffer2Deallocate;
		return M4_SUCCESS;
	}
  //## end ClTagList::Str2TagArg%913214653.body
}

m4return_t ClTagList::TagList2DataStorage (M4DataStorage *&ao_DataStorage)
{
  //## begin ClTagList::TagList2DataStorage%913291087.body preserve=yes
	m4return_t iRet;
	ClDataUnit *pDU;
	const m4int32_t iSize = 4096;
	m4char_t pcBuffer[iSize];
	m4ServerVT oVT;
	iRet=StartIteration();
	if(iRet==M4_ERROR)
	{
		M4_SRV_ERROR_N(0, "Error StartingIteration in TagList");
		return M4_ERROR;
	}
	while(GetNextTag(pcBuffer,iSize)!=M4_ERROR)
	{
		ao_DataStorage->GetNewDataUnit(pDU);
		m4uint32_t aux_size=strlen(pcBuffer);
		pDU->SetSize(aux_size+1);
		strcpy(pDU->GetBuffer(),pcBuffer);
	}
	return M4_SUCCESS;
  //## end ClTagList::TagList2DataStorage%913291087.body
}

m4return_t ClTagList::DataStorage2TagList (M4DataStorage *ai_DataStorage)
{
  //## begin ClTagList::DataStorage2TagList%913291088.body preserve=yes
	ClDataUnitInterface *pDUI;
	m4pchar_t pcBuffer;
	ai_DataStorage->InitializeIteration();
	while(ai_DataStorage->GetNext(pDUI)==M4_SUCCESS)
	{
		if(pDUI->GetType() == M4_FILE_UNIT) {
			M4_SRV_ERROR_N(0, "FILE_UNIT not soported" );
		}
		else
		{
			pcBuffer=pDUI->GetBuffer();
			if (M4_SUCCESS!=SetArg(pcBuffer)) {
				M4_SRV_WARNING( M4_WAR_OPENSES_INVTAG, "Error setting tag:%0:s", pcBuffer);
			} ;
		}
	}

	return M4_SUCCESS;
  //## end ClTagList::DataStorage2TagList%913291088.body
}

m4return_t ClTagList::Dump ()
{
  //## begin ClTagList::Dump%913717804.body preserve=yes
	m4char_t pcTagName[256];
	m4ServerVT oVT;
	m4string_t Aux ;

	M4_SRV_DEBUG( "%0:s Tags.",  GetSize() ) ;
	if(StartIteration()==M4_ERROR)
		return M4_ERROR;
	while(GetNextTag(pcTagName,256,oVT)!=M4_ERROR) {
		oVT.TraceSVT(Aux) ;
		M4_SRV_DEBUG( "Name: %0:s  Value %1:s", pcTagName << Aux.c_str() ) ;
	} ;
	return M4_SUCCESS;
  //## end ClTagList::Dump%913717804.body
}

m4return_t ClTagList::GetArg (m4pchar_t ai_pcTag, m4double_t &ao_dNumber)
{
  //## begin ClTagList::GetArg%920306529.body preserve=yes
	Tag_Map::iterator eliterador;
	eliterador=m_oTagMap.find(ai_pcTag);
	m4ServerVT oVT;

	if (eliterador != m_oTagMap.end())
	{
		oVT=(* eliterador).second;
		if(oVT.GetType()!=M4_SVT_TYPE_DOUBLE)
		{
//			M4_SRV_ERROR(M4_ERR_INCORRECT_TAG_TYPE, "Diferent type in tag %0:s",ai_pcTag);
			return M4_ERROR;
		}
		else
		{
			ao_dNumber=oVT.Data.DoubleData;
			return M4_SUCCESS;
		}
	}
	else
	{
		M4_SRV_DEBUG( "Tag %0:s not found.", ai_pcTag );
		return M4_ERROR;
	}
  //## end ClTagList::GetArg%920306529.body
}

// Additional Declarations
  //## begin ClTagList%358553B303A7.declarations preserve=yes
  //## end ClTagList%358553B303A7.declarations

// Class LessString 


//## Other Operations (implementation)
bool LessString::operator () (m4pchar_t ai_pcStr1, m4pchar_t ai_pcStr2) const
{
  //## begin LessString::operator()%898625521.body preserve=yes
		return((strcmp(ai_pcStr1, ai_pcStr2) < 0) ? true : false);
  //## end LessString::operator()%898625521.body
}

// Additional Declarations
  //## begin LessString%358FCD7C0249.declarations preserve=yes
  //## end LessString%358FCD7C0249.declarations

//## begin module%358FEA98008C.epilog preserve=yes
//## end module%358FEA98008C.epilog


