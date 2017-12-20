//## begin module%3911AA290316.cm preserve=no
//## end module%3911AA290316.cm

//## begin module%3911AA290316.cp preserve=no
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
//## end module%3911AA290316.cp

//## Module: diskpers%3911AA290316; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\diskpers.cpp

//## begin module%3911AA290316.additionalIncludes preserve=no
//## end module%3911AA290316.additionalIncludes

//## begin module%3911AA290316.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                diskpers.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================


#include <stdio.h>

#include "cldefine.hpp"
#include "m4win2ux.hpp"
#include "diskpers.hpp"
#include "clfileiod.hpp"
#include "serres.hpp"
#include "basiclog.hpp"
#include "m4unicode.hpp"


//## end module%3911AA290316.includes

// clprsstb
#include <clprsstb.hpp>
//## begin module%3911AA290316.declarations preserve=no
//## end module%3911AA290316.declarations

//## begin module%3911AA290316.additionalDeclarations preserve=yes
//## end module%3911AA290316.additionalDeclarations


static	m4pchar_t	_Normalize( m4pchar_t ai_pcInput )
{

	size_t		iOldLength = 0 ;
	size_t		iNewLength = 0 ;
	m4pchar_t	pcResult = NULL ;
	m4pchar_t	pcValue = NULL ;
	m4pchar_t	pcRead = NULL ;
	m4pchar_t	pcWrite = NULL ;
	m4char_t	acValue[ 3 + 1 ] ;

	static m4char_t	s_ackey[] = { '<', '>', ':', '"', '\\', '/', '|', '?', '*', '%', '#', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 } ;


	if( ai_pcInput == NULL )
	{
		return( ai_pcInput ) ;
	}

	pcValue = strpbrk( ai_pcInput, s_ackey ) ;

	iOldLength = strlen( ai_pcInput ) ;
	iNewLength = iOldLength ;

	while( pcValue != NULL )
	{
		iNewLength += 2 ;
		pcValue = strpbrk( pcValue + 1, s_ackey ) ;
	}

	if( iNewLength == iOldLength )
	{
		return( ai_pcInput ) ;
	}

	pcResult = new m4char_t[ iNewLength + 1 ] ;

	if( pcResult == NULL )
	{
		return( ai_pcInput ) ;
	}

	pcRead = ai_pcInput ;
	pcWrite = pcResult ;

	pcValue = strpbrk( ai_pcInput, s_ackey ) ;

	while( pcValue != NULL )
	{
		memcpy( pcWrite, pcRead, pcValue - pcRead ) ;
		pcWrite += (pcValue - pcRead) ;

		sprintf( acValue, "#%02X", *pcValue ) ;
		memcpy( pcWrite, acValue, 3 ) ;
		pcWrite += 3 ;

		pcRead = pcValue + 1 ;

		pcValue = strpbrk( pcValue + 1, s_ackey ) ;
	}

	memcpy( pcWrite, pcRead, ai_pcInput + iOldLength + 1 - pcRead ) ;

	return( pcResult ) ;
}

// Class ClDiskPersistor 

ClDiskPersistor::ClDiskPersistor (void )
  //## begin ClDiskPersistor::ClDiskPersistor%2024729955.hasinit preserve=no
  //## end ClDiskPersistor::ClDiskPersistor%2024729955.hasinit
  //## begin ClDiskPersistor::ClDiskPersistor%2024729955.initialization preserve=yes
  //## end ClDiskPersistor::ClDiskPersistor%2024729955.initialization
{
  //## begin ClDiskPersistor::ClDiskPersistor%2024729955.body preserve=yes
  //## end ClDiskPersistor::ClDiskPersistor%2024729955.body
}


ClDiskPersistor::~ClDiskPersistor (void )
{
  //## begin ClDiskPersistor::~ClDiskPersistor%546693214.body preserve=yes
  //## end ClDiskPersistor::~ClDiskPersistor%546693214.body
}



//## Other Operations (implementation)
m4return_t ClDiskPersistor::Persist (ClPersistiableObject& ai_oPersistiableObj)
{
  //## begin ClDiskPersistor::Persist%-2109917672.body preserve=yes
	m4return_t ret;
	ClFileIODriver IOD;

	ret = InitIODFromObject(ai_oPersistiableObj, IOD, GEN_IO_MODE_WRITE);

	if (ret != M4_ERROR)
	{
		ret = ai_oPersistiableObj.Serialize(IOD);

		IOD.End(M4_TRUE);
	}	

	
	return ret;
  //## end ClDiskPersistor::Persist%-2109917672.body
}

m4return_t ClDiskPersistor::DePersist (ClPersistiableObject& ai_oPersistiableObj)
{
  //## begin ClDiskPersistor::DePersist%-788918971.body preserve=yes
	m4return_t ret;
	ClFileIODriver IOD;

	ret = InitIODFromObject(ai_oPersistiableObj, IOD, GEN_IO_MODE_READ);

	if (ret != M4_ERROR)
	{
		ret = ai_oPersistiableObj.DeSerialize(IOD);
		IOD.End(M4_TRUE);

	}	
	
	return ret;
  //## end ClDiskPersistor::DePersist%-788918971.body
}

m4return_t ClDiskPersistor::Delete (ClPersistiableObject& ai_oPersistiableObj)
{
  //## begin ClDiskPersistor::Delete%119342329.body preserve=yes
	m4return_t ret;
	m4bool_t bMustExist;
	m4pchar_t pcFile = GetFileNameFromObject(ai_oPersistiableObj, bMustExist);

	if (!pcFile)
		return M4_ERROR;

	if ( remove(pcFile) == -1 )
		ret = M4_ERROR;
	else
		ret = M4_SUCCESS;

	delete [] pcFile;

	return ret;
  //## end ClDiskPersistor::Delete%119342329.body
}

m4return_t ClDiskPersistor::InitIODFromObject (ClPersistiableObject& ai_oPersistiableObj, ClFileIODriver& ai_IOD, m4uint32_t ai_iMode)
{
  //## begin ClDiskPersistor::InitIODFromObject%1113201133.body preserve=yes
	m4return_t ret;
	m4bool_t bMustExist;

	m4pchar_t pcFile = GetFileNameFromObject(ai_oPersistiableObj, bMustExist);

	if (!pcFile)
		return M4_ERROR;

	//ret = ai_IOD.Init(ai_iMode, pcFile, (ai_iMode == GEN_IO_MODE_READ)? (M4_TRUE && bMustExist): M4_FALSE);
	ret = ai_IOD.Init(ai_iMode, pcFile, M4_FALSE);

	if ((ret == M4_ERROR) && (ai_iMode == GEN_IO_MODE_WRITE))
	{
		ret = MkDirFromObject(ai_oPersistiableObj);

		/* Bug 0114783
		Si no se puede crear el directorio ya no se sigue con el fichero
		*/
		if( ret == M4_SUCCESS )
		{
			ret = ai_IOD.Init(ai_iMode, pcFile, M4_TRUE);
		}
	}

	delete [] pcFile;

	return ret;
  //## end ClDiskPersistor::InitIODFromObject%1113201133.body
}

m4pchar_t ClDiskPersistor::GetFileNameFromObject (ClPersistiableObject& ai_oPersistiableObj, m4bool_t& ao_bMustExist)
{
  //## begin ClDiskPersistor::GetFileNameFromObject%-526635167.body preserve=yes
	size_t		iSize = 0;
	m4pchar_t	pcFile, pcObjectType, pcEnvId, pcTypeId, pcNormalizedTypeId, pcUniqueKey, pcExt;

	ao_bMustExist = M4_TRUE;

	if (!ai_oPersistiableObj.GetObjId())
	{
		//g_oChLog<<BeginError(M4_SERIAL_NO_PERSISTIABLE_OBJECT_ID)<<EndLog;			
		SETCODE(M4_SERIAL_NO_PERSISTIABLE_OBJECT_ID, ERRORLOG);
		return 0;
	}

	eObjectType oObjectType = ai_oPersistiableObj.GetObjId()->GetType ();

	switch (oObjectType)
	{
		case CacheObjectType:
			pcObjectType = "Cache";
			pcExt = ".cac";
			ao_bMustExist = M4_FALSE;
			break;

		case MetaChannelObjectType:
			pcObjectType = "MCR";
			pcExt = ".mcr";
			break;

		case MetaSecurityObjectType:
			pcObjectType = "SCR";
			pcExt = ".scr";
			break;

		case PresentationObjectType:
			pcObjectType = "PRS";
			pcExt = ".prs";
			break;

		case MapObjectType:
			pcObjectType = "MAP";
			pcExt = ".map";
			break;

		case ChannelDataObjectType:
			pcObjectType = "CHANNEL";
			pcExt = ".chn";
			break;

		case ObjDataObjectType:
			pcObjectType = "DELTA";
			pcExt = ".chn";
			break;

		case CacheDirectoryObjectType:
			pcObjectType = "Cache";
			pcExt = ".ver";
			ao_bMustExist = M4_FALSE;
			break;
		
		case ExecutorStateObjectType:
			pcObjectType = "m4state";
			pcExt = ".est";	// eXECUTOR stATE = est
			ao_bMustExist = M4_FALSE;
			break;

		default:
			pcObjectType = "Undefined";
			break;
	}

	pcEnvId = ai_oPersistiableObj.GetObjId()->GetEnvironmentId();

    pcTypeId = ai_oPersistiableObj.GetObjId()->GetTypedId();

    pcUniqueKey = ai_oPersistiableObj.GetObjId()->GetUniqueKey();

	if (!pcObjectType)
		pcObjectType = "";

	iSize += strlen(pcObjectType);

	if (!pcExt)
		pcExt = "";

	iSize += strlen(pcExt);

	if (!pcEnvId)
	{
		pcEnvId = "c:\\temp";
		M4_ASSERT(0);
	}
	
	iSize += strlen(pcEnvId);

	if (!pcTypeId)
		pcTypeId = "";

	/* Bug 0279621
	Se escapan los caracteres especiales
	*/
	pcNormalizedTypeId = _Normalize(pcTypeId);

	iSize += strlen(pcNormalizedTypeId);

	//if (!pcUniqueKey)
	//Por ahora no ponemos UniqueKey
	pcUniqueKey = "";
	
	iSize += strlen(pcUniqueKey);

	pcFile = new m4char_t [iSize + 10];

	sprintf(pcFile,"%s%s%s%s%s%s%s", pcEnvId, M4_BACK_SLASH, pcObjectType, M4_BACK_SLASH, pcNormalizedTypeId, pcUniqueKey, pcExt);

	if (pcNormalizedTypeId != pcTypeId)
	{
		delete(pcNormalizedTypeId);
	}

	return pcFile;
  //## end ClDiskPersistor::GetFileNameFromObject%-526635167.body
}

m4return_t ClDiskPersistor::MkDirFromObject (ClPersistiableObject& ai_oPersistiableObj)
{
  //## begin ClDiskPersistor::MkDirFromObject%1585648303.body preserve=yes
	m4return_t	iResult = 0;
	size_t		iSize = 0;
	m4pchar_t	pcDir, pcObjectType, pcEnvId;

	if (!ai_oPersistiableObj.GetObjId())
	{
		SETCODE(M4_SERIAL_NO_PERSISTIABLE_OBJECT_ID, ERRORLOG);
		return( M4_ERROR );
	}

	eObjectType oObjectType = ai_oPersistiableObj.GetObjId()->GetType ();

	switch (oObjectType)
	{
		case CacheObjectType:
			pcObjectType = "Cache";
			break;

		case MetaChannelObjectType:
			pcObjectType = "MCR";
			break;

		case MetaSecurityObjectType:
			pcObjectType = "SCR";
			break;

		case PresentationObjectType:
			pcObjectType = "PRS";
			break;

		case MapObjectType:
			pcObjectType = "MAP";
			break;

		case ChannelDataObjectType:
			pcObjectType = "CHANNEL";
			break;

		case ObjDataObjectType:
			pcObjectType = "DELTA";
			break;

		case CacheDirectoryObjectType:
			pcObjectType = "Cache";
			break;

		case ExecutorStateObjectType:
			pcObjectType = "m4state";
			break;

		default:
			pcObjectType = "Undefined";
			break;
	}

	pcEnvId = ai_oPersistiableObj.GetObjId()->GetEnvironmentId();

	if (!pcObjectType)
		pcObjectType = "";

	iSize += strlen(pcObjectType);

	if (!pcEnvId)
	{
		SETCODE(M4_SERIAL_NO_PERSISTIABLE_OBJECT_ID, ERRORLOG);
		return( M4_ERROR );
	}
	
	/* Bug 0114783
	Si no se puede crear el directorio ya no se sigue con el subdirectorio
	*/
	iResult = M4MakeDirectory(pcEnvId);

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iSize += strlen(pcEnvId);

	pcDir = new m4char_t [iSize + 10];
	sprintf(pcDir,"%s%s%s", pcEnvId, M4_BACK_SLASH, pcObjectType);
	iResult = M4MakeDirectory(pcDir);	// Crear directorio
	delete [] pcDir;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
  //## end ClDiskPersistor::MkDirFromObject%1585648303.body
}

// Additional Declarations
  //## begin ClDiskPersistor%3911AA280350.declarations preserve=yes
  //## end ClDiskPersistor%3911AA280350.declarations

//## begin module%3911AA290316.epilog preserve=yes
//## end module%3911AA290316.epilog
