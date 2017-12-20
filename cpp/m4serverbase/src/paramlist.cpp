//## begin module%3904571202F8.cm preserve=no
//## end module%3904571202F8.cm

//## begin module%3904571202F8.cp preserve=no
//	=============================================================================
//	=
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
//## end module%3904571202F8.cp

//## Module: ParamList%3904571202F8; Package body
//## Subsystem: M4ServerBase::ParamList::src%390457380072
//## Source file: e:\mybuild\m4serverbase\src\paramlist.cpp

//## begin module%3904571202F8.additionalIncludes preserve=no
//## end module%3904571202F8.additionalIncludes

//## begin module%3904571202F8.includes preserve=yes
#include <m4trace.hpp>
#include <basiclog.hpp>
#include <m4srvtrace.h>
#include <paramlist.hpp>
#include <m4servervt.hpp>
#include <clienttag.hpp>
#include <garbageableinterface.hpp>

#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%3904571202F8.includes

//## begin module%3904571202F8.declarations preserve=no
//## end module%3904571202F8.declarations

//## begin module%3904571202F8.additionalDeclarations preserve=yes
m4char_t M4ParamListFieldSeparator=1;
m4char_t M4ParamListTagSeparator=2;

#ifdef _AIX
#define DENOMINADOR 4294967296LL
#else
#define DENOMINADOR 4294967296
#endif

//## end module%3904571202F8.additionalDeclarations


// Class ClParamList 




ClParamList::ClParamList ()
  //## begin ClParamList::ClParamList%956586087.hasinit preserve=no
  //## end ClParamList::ClParamList%956586087.hasinit
  //## begin ClParamList::ClParamList%956586087.initialization preserve=yes
  : m_iState ( PARAM_AVAILABLE )
  //## end ClParamList::ClParamList%956586087.initialization
{
  //## begin ClParamList::ClParamList%956586087.body preserve=yes
  //## end ClParamList::ClParamList%956586087.body
}


ClParamList::~ClParamList ()
{
  //## begin ClParamList::~ClParamList%956586088.body preserve=yes
	if(!m_ParamMap.empty())
	{
		Param_map::iterator theIte=m_ParamMap.begin();
		while(theIte!=m_ParamMap.end())
		{
			m4pchar_t thename=(*theIte).first;
			delete thename;
			delete (*theIte).second;
			m_ParamMap.erase(theIte);
			theIte=m_ParamMap.begin();
		}
	}
  //## end ClParamList::~ClParamList%956586088.body
}



//## Other Operations (implementation)
m4return_t ClParamList::SetPointer (m4pchar_t ai_szParamName, const void *ai_Pointer)
{
  //## begin ClParamList::SetPointer%956586089.body preserve=yes
	m4ServerVT *poVT = new m4ServerVT((const char*)ai_Pointer, M4_FALSE /* No queremos copia */);
	
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;

		return M4_ERROR;
	}
	else
		return M4_SUCCESS;

  //## end ClParamList::SetPointer%956586089.body
}

m4return_t ClParamList::SetString (m4pcchar_t ai_szParamName, m4pcchar_t ai_pcString)
{
  //## begin ClParamList::SetString%956586090.body preserve=yes
	m4ServerVT *poVT=new m4ServerVT((const char *)ai_pcString,M4_TRUE /* Queremos copia */);
	
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;

		return M4_ERROR;
	}
	else
		return M4_SUCCESS;
  //## end ClParamList::SetString%956586090.body
}

m4return_t ClParamList::SetBinaryChunk (m4pcchar_t ai_szParamName, m4pchar_t ai_pcChunk, m4uint_t ai_uiChunkSize)
{
  //## begin ClParamList::SetBinaryChunk%956586091.body preserve=yes
	return M4_ERROR;
  //## end ClParamList::SetBinaryChunk%956586091.body
}

m4return_t ClParamList::GetPointer (m4pcchar_t ai_szParamName, void * &ao_Pointer)
{
  //## begin ClParamList::GetPointer%956586092.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName, poVT);
	if(M4_ERROR==iRet)
	{
		return M4_ERROR;
	}
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_POINTER)
		{
			M4_SRV_ERROR(0, "GetPointer. Invalid Type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName );
			return M4_ERROR;
		}
		else
		{
			ao_Pointer=(void *)poVT->Data.PointerChar;
			return M4_SUCCESS;
		}
	}
  //## end ClParamList::GetPointer%956586092.body
}

m4return_t ClParamList::GetString (m4pcchar_t ai_szParamName, m4pchar_t &ao_pcString)
{
  //## begin ClParamList::GetString%956586093.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName, poVT);
	if(M4_ERROR==iRet)
		return M4_ERROR;
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_STRING)
		{
			M4_SRV_ERROR(0, "GetString. Invalid Type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName );
			return M4_ERROR;
		}
		else
		{
			ao_pcString=poVT->Data.PointerChar;
			return M4_SUCCESS;
		}
	}
	
  //## end ClParamList::GetString%956586093.body
}

m4return_t ClParamList::GetBinaryChunk (m4pchar_t ai_szParamName, m4pchar_t &ao_pcChunk, m4uint_t &ao_uiChunkSize)
{
  //## begin ClParamList::GetBinaryChunk%956586094.body preserve=yes
	return M4_ERROR;
  //## end ClParamList::GetBinaryChunk%956586094.body
}

m4return_t ClParamList::SetDouble (m4pcchar_t ai_szParamName, const m4double_t ai_dValue)
{
  //## begin ClParamList::SetDouble%956586095.body preserve=yes
	m4ServerVT *poVT=new m4ServerVT((m4double_t)ai_dValue);
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;
		return M4_ERROR;
	}
	else
		return M4_SUCCESS;
  //## end ClParamList::SetDouble%956586095.body
}

m4return_t ClParamList::GetDouble (m4pchar_t ai_szParamName, m4double_t &ao_dValue)
{
  //## begin ClParamList::GetDouble%956586096.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName,poVT);
	if(M4_ERROR==iRet)
		return M4_ERROR;
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_DOUBLE)
		{
			M4_SRV_ERROR(0, "GetDouble. Invalid Type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName );
			return M4_ERROR;
		}
		else
		{
			ao_dValue=poVT->Data.DoubleData;
			return M4_SUCCESS;
		}
	}
  //## end ClParamList::GetDouble%956586096.body
}

m4return_t ClParamList::Set (m4pcchar_t ai_szParamName, m4ServerVT *ai_poServerVT)
{
  //## begin ClParamList::Set%956586097.body preserve=yes
	if(!ai_szParamName)
	{
		M4_SRV_ERROR(0, "Invalid ParamName %0:s ", ai_szParamName );
		return M4_ERROR;
	}
	Param_map::iterator ite=m_ParamMap.find((char *)ai_szParamName);
	if(ite==m_ParamMap.end())
	{
		m4char_t *name=new char[strlen(ai_szParamName)+1];
		strcpy(name,ai_szParamName);
		m_ParamMap[name]=ai_poServerVT;
	}
	else
	{
		M4_SRV_ERROR(0, "Param %0:s already exists", ai_szParamName );

		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClParamList::Set%956586097.body
}

m4return_t ClParamList::Get (m4pcchar_t ai_szParamName, m4ServerVT *&ao_poServerVT)
{
  //## begin ClParamList::Get%956586098.body preserve=yes
	if(!ai_szParamName)
	{
		M4_SRV_ERROR(0, "Invalid ParamName %0:s", ai_szParamName );
		return M4_ERROR;
	}
	Param_map::iterator theIte;
	theIte=m_ParamMap.find((char *)ai_szParamName);
	if(theIte==m_ParamMap.end())
	{
		M4_SRV_DEBUG(  "Param  %0:s not found",  ai_szParamName  );
		return M4_ERROR;
	}

	ao_poServerVT=(*theIte).second;
	return M4_SUCCESS;
	
  //## end ClParamList::Get%956586098.body
}

m4int_t ClParamList::GetNumberParams ()
{
  //## begin ClParamList::GetNumberParams%956586099.body preserve=yes
	return m_ParamMap.size();
  //## end ClParamList::GetNumberParams%956586099.body
}

m4return_t ClParamList::DeleteParam (m4pchar_t ai_szParamName)
{
  //## begin ClParamList::DeleteParam%956592445.body preserve=yes
	if(!ai_szParamName)
	{
		M4_SRV_ERROR(0, "Invalid ParamName %0:s", ai_szParamName );
		return M4_ERROR;
	}
	Param_map::iterator theIte;
	theIte=m_ParamMap.find(ai_szParamName);
	if(theIte==m_ParamMap.end())
	{
		M4_SRV_DEBUG(  "Can not to delete unknown param %0:s ", ai_szParamName );
		return M4_ERROR;
	}

	delete (*theIte).first;
	delete (*theIte).second;
	m_ParamMap.erase(theIte);
	
	return M4_SUCCESS;
  //## end ClParamList::DeleteParam%956592445.body
}

m4return_t ClParamList::SetInt (m4pcchar_t ai_szParamName, const m4int_t ai_iValue)
{
  //## begin ClParamList::SetInt%956649480.body preserve=yes
	m4ServerVT *poVT=new m4ServerVT((m4int_t)ai_iValue);
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;
		return M4_ERROR;
	}
	else
		return M4_SUCCESS;
  //## end ClParamList::SetInt%956649480.body
}

m4return_t ClParamList::GetInt (m4pcchar_t ai_szParamName, m4int_t &ao_iValue)
{
  //## begin ClParamList::GetInt%956649481.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName,poVT);
	if(M4_ERROR==iRet)
		return M4_ERROR;
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_INT)
		{
			M4_SRV_ERROR(0, "GetInt. Invalid type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName  );
			return M4_ERROR;
		}
		else
		{
			ao_iValue=poVT->Data.IntData;
			return M4_SUCCESS;
		}
	}
  //## end ClParamList::GetInt%956649481.body
}

m4return_t ClParamList::SetM4Return (m4pcchar_t ai_szParamName, m4return_t ai_retValue)
{
  //## begin ClParamList::SetM4Return%958463329.body preserve=yes
	m4ServerVT *poVT=new m4ServerVT(ai_retValue);
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;
		return M4_ERROR;
	}
	else
		return M4_SUCCESS;
  //## end ClParamList::SetM4Return%958463329.body
}

m4return_t ClParamList::GetM4Return (m4pcchar_t ai_szParamName, m4return_t &ao_retValue)
{
  //## begin ClParamList::GetM4Return%958463330.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName,poVT);
	if(M4_ERROR==iRet)
		return M4_ERROR;
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_M4RETURN)
		{
			M4_SRV_ERROR(0, "GetM4Return. Invalid type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName  );
			return M4_ERROR;
		}
		else
		{
			ao_retValue=(m4return_t)poVT->Data.IntData;
			return M4_SUCCESS;
		}
	}
  //## end ClParamList::GetM4Return%958463330.body
}

m4return_t ClParamList::Dump ()
{
  //## begin ClParamList::Dump%958548779.body preserve=yes
	Param_map::iterator theIte;
	theIte=m_ParamMap.begin();
	M4_SRV_DEBUG_BLOCK_N( Blk, "ParamList::Dump" ) ;  
	m4string_t Aux ;
	while(theIte!=m_ParamMap.end())
	{
		(*((*theIte).second)).TraceSVT(Aux) ;
		M4_SRV_DEBUG( "%0:s::%1:s", (*theIte).first << Aux.c_str() ) ;
		theIte++;
	}
	return M4_SUCCESS;
  //## end ClParamList::Dump%958548779.body
}

m4return_t ClParamList::Recycle ()
{
  //## begin ClParamList::Recycle%968315705.body preserve=yes
	if ( CanRecycle () )
	{
		Clear () ;
		return M4_SUCCESS ;
	}

	return M4_ERROR ;
  //## end ClParamList::Recycle%968315705.body
}

m4bool_t ClParamList::CanRecycle ()
{
  //## begin ClParamList::CanRecycle%968315706.body preserve=yes
	return (m_iState == PARAM_AVAILABLE)?M4_TRUE:M4_FALSE ; 
  //## end ClParamList::CanRecycle%968315706.body
}

m4return_t ClParamList::Destroy ()
{
  //## begin ClParamList::Destroy%968315707.body preserve=yes
	Clear () ;

	delete this ;

	return M4_SUCCESS ;
  //## end ClParamList::Destroy%968315707.body
}

m4bool_t ClParamList::CanDestroy ()
{
  //## begin ClParamList::CanDestroy%968315708.body preserve=yes
	return CanRecycle () ;
  //## end ClParamList::CanDestroy%968315708.body
}

void ClParamList::Clear ()
{
  //## begin ClParamList::Clear%968315709.body preserve=yes
	Param_map::iterator theIte;
	
	while((theIte=m_ParamMap.begin() )!=m_ParamMap.end() )
	{
		delete (*theIte).first;
		delete (*theIte).second;
		m_ParamMap.erase(theIte);
	}

  //## end ClParamList::Clear%968315709.body
}

m4return_t ClParamList::GetInt64 (m4pcchar_t ai_szParamName, m4int64_t &ao_i64Value)
{
  //## begin ClParamList::GetInt64%972896441.body preserve=yes
	m4ServerVT *poVT;
	m4return_t iRet=Get(ai_szParamName,poVT);
	if(M4_ERROR==iRet)
		return M4_ERROR;
	else
	{
		if(poVT->GetType()!=M4_SVT_TYPE_INT64)
		{
			M4_SRV_ERROR(0, "GetInt64. Invalid type %0:s in Param %1:s", 
				poVT->GetType() << ai_szParamName  );
			return M4_ERROR;
		}
		else
		{
			ao_i64Value=poVT->Data.Int64Data;
			return M4_SUCCESS;
		}
	}
  //## end ClParamList::GetInt64%972896441.body
}

m4return_t ClParamList::SetInt64 (m4pcchar_t ai_szParamName, const m4int64_t ai_i64Value)
{
  //## begin ClParamList::SetInt64%972896442.body preserve=yes
	m4ServerVT *poVT=new m4ServerVT((m4int64_t)ai_i64Value);
	if(M4_ERROR==Set(ai_szParamName,poVT))
	{
		delete poVT;
		return M4_ERROR;
	}
	else
		return M4_SUCCESS;
  //## end ClParamList::SetInt64%972896442.body
}

m4return_t ClParamList::Serialize (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClParamList::Serialize%972896443.body preserve=yes
	Param_map::iterator theIte;
	theIte=m_ParamMap.begin();
	m4pchar_t pcTagName;
	m4ServerVT *poSVT;
	m4char_t vtType;
	m4int_t iUsedBytes=0;
	m4int_t iTagSize;
	m4pchar_t pcPointer=ai_pcBuffer;
	m4char_t auxBuffer[256];
//	Usaremos el formato TAG_NAME:Type:Value;  con los separadores 
//	M4ParamListFieldSeparator
//	M4ParamListTagSeparator

	while(theIte!=m_ParamMap.end())
	{
		pcTagName=(*theIte).first;
		poSVT=(*theIte).second;
		vtType=poSVT->GetType ();
		iTagSize=strlen(pcTagName)+4;  // Los 2 separadores de campo, el tipo y el separador de tag
		if(iTagSize+iUsedBytes>ai_iBufferSize)
		{
			M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
			return M4_ERROR;
		}
		sprintf(pcPointer,"%s%c%c%c",pcTagName,M4ParamListFieldSeparator,vtType,M4ParamListFieldSeparator);
		iUsedBytes+=iTagSize;
		pcPointer+=iTagSize-1; // por el separador de tag que aun no hemos añadido pero si contado
		switch(vtType)
		{
		case M4_SVT_TYPE_POINTER:
		case M4_SVT_TYPE_NULL:
			break;
		case M4_SVT_TYPE_DOUBLE:
			{
				m4int_t iNumberSize=sprintf(auxBuffer,"%f",poSVT->Data.DoubleData);
				if(iUsedBytes+iNumberSize>ai_iBufferSize)   
				{
					M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
					return M4_ERROR;
				}
				strcat(pcPointer,auxBuffer);
				iUsedBytes+=iNumberSize;
				pcPointer+=iNumberSize;
			}
			break;
		case M4_SVT_TYPE_INT:
			{
				m4int_t iNumberSize=sprintf(auxBuffer,"%i",poSVT->Data.IntData);
				if(iUsedBytes+iNumberSize>ai_iBufferSize)   
				{
					M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
					return M4_ERROR;
				}
				strcat(pcPointer,auxBuffer);
				iUsedBytes+=iNumberSize;
				pcPointer+=iNumberSize;
			}
			break;
		case M4_SVT_TYPE_INT64:
			{
				m4int_t iNumberSize=sprintf(auxBuffer,"%i",poSVT->Data.Int64Data/DENOMINADOR);
				auxBuffer[iNumberSize]='@';
				iNumberSize++;
				iNumberSize+=sprintf(auxBuffer+iNumberSize,"%i",poSVT->Data.Int64Data%DENOMINADOR);
				if(iUsedBytes+iNumberSize>ai_iBufferSize)   
				{
					M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
					return M4_ERROR;
				}
				strcat(pcPointer,auxBuffer);
				iUsedBytes+=iNumberSize;
				pcPointer+=iNumberSize;
			}
			break;
		case M4_SVT_TYPE_STRING_VAR:
		case M4_SVT_TYPE_STRING:
			{
				m4int_t valuesize=strlen(poSVT->Data.PointerChar);
				if(iUsedBytes+valuesize>ai_iBufferSize)
				{
					M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
					return M4_ERROR;
				}
				iUsedBytes+=valuesize;
				sprintf(pcPointer,"%s",poSVT->Data.PointerChar);
				pcPointer+=valuesize;
			}
			break;
		case M4_SVT_TYPE_M4RETURN:
			if(iUsedBytes+1>ai_iBufferSize)
			{
				M4_SRV_ERROR_N( 0, "Serialize(). Not enough space" );
				return M4_ERROR;
			}
			sprintf(pcPointer,"%c",(char )(poSVT->Data.IntData));
			pcPointer++;
			iUsedBytes++;
			break;
		default:
			M4_SRV_ERROR( 0, "Serialize. Unkown Type %0:s",  vtType );
			return M4_ERROR;
			break;
		}
		sprintf(pcPointer,"%c",M4ParamListTagSeparator);
		iUsedBytes++;
		pcPointer++;
		theIte++;
	}
	return M4_SUCCESS;

  //## end ClParamList::Serialize%972896443.body
}

m4return_t ClParamList::Deserialize (m4pchar_t ai_pcBuffer, m4int_t ai_iBufferSize)
{
  //## begin ClParamList::Deserialize%972896444.body preserve=yes
	if((NULL==ai_pcBuffer) || (0==ai_iBufferSize))
	{
		M4_SRV_ERROR( 0, "Deserialize. Invalid arguments. Buffer %0:s, Size %1:s", 
			ai_pcBuffer << ai_iBufferSize );
		return M4_ERROR;
	}
	
	m4pchar_t  pcPointer=ai_pcBuffer;
	m4pchar_t  pcEndTag;
	m4pchar_t  pcEndTagName;
	m4pchar_t  pcTagValue;
	m4char_t cTagType;
	m4bool_t bMoreTags=M4_TRUE;
	m4int_t iTagSize;
	m4return_t iRet;
	while(NULL!=(pcEndTag=strchr(pcPointer,M4ParamListTagSeparator)) )
	{
		iTagSize=pcEndTag-pcPointer;
		*pcEndTag='\0';
		pcEndTagName=strchr(pcPointer,M4ParamListFieldSeparator);
		if(NULL==pcEndTagName)
		{
			*pcEndTag=M4ParamListTagSeparator;
			M4_SRV_ERROR_N( 0, "Deserialize. Invalid format" ) ;
			return M4_ERROR;
		}
		*pcEndTagName='\0';
		cTagType=*(pcEndTagName+1);
		pcTagValue=pcEndTagName+3;
		switch(cTagType)
		{
		case M4_SVT_TYPE_POINTER:
		case M4_SVT_TYPE_NULL:
			break;
		case M4_SVT_TYPE_DOUBLE:
			{
				m4double_t dValue;
				sscanf(pcTagValue,"%f",&dValue);
				iRet=SetDouble(pcPointer,dValue);
			}
			break;
		case M4_SVT_TYPE_INT:
			{
				m4int_t iValue;
				sscanf(pcTagValue,"%i",&iValue);
				iRet=SetInt(pcPointer,iValue);
			}
			break;
		case M4_SVT_TYPE_INT64:
			{
				m4int_t iValue1;
				m4int_t iValue2;
				sscanf(pcTagValue,"%i@%i",&iValue1,&iValue2);
				m4int64_t i64Value=iValue1;
				i64Value*=DENOMINADOR;
				i64Value+=iValue2;
				iRet=SetInt64(pcPointer,i64Value);
			}
			break;
		case M4_SVT_TYPE_STRING_VAR:
		case M4_SVT_TYPE_STRING:
			iRet=SetString(pcPointer,pcTagValue);
			break;
		case M4_SVT_TYPE_M4RETURN:
			iRet=SetM4Return(pcPointer,*pcTagValue);
			break;
		default:
			M4_SRV_ERROR( 0, "Deserialize. Unkown Type %0:s", cTagType ) ;
			return M4_ERROR;
			break;
		}

		*pcEndTagName=M4ParamListFieldSeparator;
		*pcEndTag=M4ParamListTagSeparator;
		pcPointer=pcEndTag+1;
	}
	return M4_SUCCESS;
  //## end ClParamList::Deserialize%972896444.body
}

// Additional Declarations
  //## begin ClParamList%39044EBE03CF.declarations preserve=yes
  //## end ClParamList%39044EBE03CF.declarations

//## begin module%3904571202F8.epilog preserve=yes
//## end module%3904571202F8.epilog
