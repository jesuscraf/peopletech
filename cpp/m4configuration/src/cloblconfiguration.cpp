//## begin module%3493E7720009.cm preserve=no
//## end module%3493E7720009.cm

//## begin module%3493E7720009.cp preserve=no
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
//## end module%3493E7720009.cp

//## Module: ClOblConfiguration%3493E7720009; Package body
//## Subsystem: M4Configuration::src%379DD22F012D
//## Source file: F:\integration\M4Configuration\src\cloblconfiguration.cpp

//## begin module%3493E7720009.additionalIncludes preserve=no
//## end module%3493E7720009.additionalIncludes

//## begin module%3493E7720009.includes preserve=yes
#include <m4win.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module%3493E7720009.includes

// syncro
#include <syncro.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
// Directory
#include <directory.hpp>
//## begin module%3493E7720009.declarations preserve=no
//## end module%3493E7720009.declarations


// Class ClOblConfiguration 

//## begin ClOblConfiguration::m_oMutex%367E16400279.role preserve=no  public: static ClMutex { -> VHAN}
ClMutex ClOblConfiguration::m_oMutex;
//## end ClOblConfiguration::m_oMutex%367E16400279.role

ClOblConfiguration::ClOblConfiguration (UINT ai_hHandle)
  //## begin ClOblConfiguration::ClOblConfiguration%884203093.hasinit preserve=no
  //## end ClOblConfiguration::ClOblConfiguration%884203093.hasinit
  //## begin ClOblConfiguration::ClOblConfiguration%884203093.initialization preserve=yes
  :ClExObl ( ai_hHandle )
  //## end ClOblConfiguration::ClOblConfiguration%884203093.initialization
{
  //## begin ClOblConfiguration::ClOblConfiguration%884203093.body preserve=yes
  //## end ClOblConfiguration::ClOblConfiguration%884203093.body
}

ClOblConfiguration::ClOblConfiguration ()
  //## begin ClOblConfiguration::ClOblConfiguration%884203094.hasinit preserve=no
  //## end ClOblConfiguration::ClOblConfiguration%884203094.hasinit
  //## begin ClOblConfiguration::ClOblConfiguration%884203094.initialization preserve=yes
  //## end ClOblConfiguration::ClOblConfiguration%884203094.initialization
{
  //## begin ClOblConfiguration::ClOblConfiguration%884203094.body preserve=yes
  //## end ClOblConfiguration::ClOblConfiguration%884203094.body
}

ClOblConfiguration::ClOblConfiguration (m4pchar_t ai_szOblString)
  //## begin ClOblConfiguration::ClOblConfiguration%913835326.hasinit preserve=no
  //## end ClOblConfiguration::ClOblConfiguration%913835326.hasinit
  //## begin ClOblConfiguration::ClOblConfiguration%913835326.initialization preserve=yes
  //## end ClOblConfiguration::ClOblConfiguration%913835326.initialization
{
  //## begin ClOblConfiguration::ClOblConfiguration%913835326.body preserve=yes

	M4AOblGeneraObl ( GetHandle ( ) , ai_szOblString ) ;

  //## end ClOblConfiguration::ClOblConfiguration%913835326.body
}


ClOblConfiguration::~ClOblConfiguration ()
{
  //## begin ClOblConfiguration::~ClOblConfiguration%903009694.body preserve=yes
		m4return_t iRet ;
		UINT handle =  GetHandle ( ) ;

		iRet = M4AOblFinaliza ( handle ) ;
  //## end ClOblConfiguration::~ClOblConfiguration%903009694.body
}



//## Other Operations (implementation)
int ClOblConfiguration::GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClOblConfiguration::GetInstance%882264738.body preserve=yes
	return ClExObl::GetInstance ( ao_pszString, ai_iStringSize, ao_riOutput) ;
  //## end ClOblConfiguration::GetInstance%882264738.body
}

int ClOblConfiguration::GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClOblConfiguration::GetNumericValue%882264739.body preserve=yes
	return ClExObl::GetNumericValue (ai_pszPath, ai_pszName, ao_rdNumber) ;
  //## end ClOblConfiguration::GetNumericValue%882264739.body
}

int ClOblConfiguration::GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClOblConfiguration::GetNumericValue%882264740.body preserve=yes
	return ClExObl::GetNumericValue (ai_hBlockHandle, ai_pszName, ao_rdNumber) ;
  //## end ClOblConfiguration::GetNumericValue%882264740.body
}

int ClOblConfiguration::GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClOblConfiguration::GetStringValue%882264741.body preserve=yes
	return ClExObl::GetStringValue (ai_pszPath, ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength) ;
  //## end ClOblConfiguration::GetStringValue%882264741.body
}

int ClOblConfiguration::GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClOblConfiguration::GetStringValue%882264742.body preserve=yes
	return ClExObl::GetStringValue (ai_hBlockHandle, ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength) ;
  //## end ClOblConfiguration::GetStringValue%882264742.body
}

int ClOblConfiguration::GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClOblConfiguration::GetDateValue%882264743.body preserve=yes
	return ClExObl::GetDateValue ( ai_pszPath, ai_pszName, ao_rdDate) ;
  //## end ClOblConfiguration::GetDateValue%882264743.body
}

int ClOblConfiguration::GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClOblConfiguration::GetDateValue%882264744.body preserve=yes
	return ClExObl::GetDateValue (ai_hBlockHandle, ai_pszName, ao_rdDate) ;
  //## end ClOblConfiguration::GetDateValue%882264744.body
}

int ClOblConfiguration::SetNumericValue (char* a_pszPath, char* a_pszName, double a_dNumber)
{
  //## begin ClOblConfiguration::SetNumericValue%882264745.body preserve=yes
	return ClExObl::SetNumericValue (a_pszPath, a_pszName, a_dNumber) ;
  //## end ClOblConfiguration::SetNumericValue%882264745.body
}

int ClOblConfiguration::SetNumericValue (UINT ai_hBlockHandle, char* a_pszName, double a_dNumber)
{
  //## begin ClOblConfiguration::SetNumericValue%882264746.body preserve=yes
	return ClExObl::SetNumericValue (ai_hBlockHandle, a_pszName, a_dNumber) ;
  //## end ClOblConfiguration::SetNumericValue%882264746.body
}

int ClOblConfiguration::SetStringValue (char* a_pszPath, char* a_pszName, char* a_pszString)
{
  //## begin ClOblConfiguration::SetStringValue%882264747.body preserve=yes
	return ClExObl::SetStringValue (a_pszPath, a_pszName, a_pszString) ;
  //## end ClOblConfiguration::SetStringValue%882264747.body
}

int ClOblConfiguration::SetStringValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszString)
{
  //## begin ClOblConfiguration::SetStringValue%882264748.body preserve=yes
 return ClExObl::SetStringValue (ai_hBlockHandle, a_pszName, a_pszString) ;
  //## end ClOblConfiguration::SetStringValue%882264748.body
}

int ClOblConfiguration::SetDateValue (char* a_pszPath, char* a_pszName, char* a_pszDate)
{
  //## begin ClOblConfiguration::SetDateValue%882264749.body preserve=yes
	return ClExObl::SetDateValue (a_pszPath, a_pszName, a_pszDate) ;
  //## end ClOblConfiguration::SetDateValue%882264749.body
}

int ClOblConfiguration::SetDateValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate)
{
  //## begin ClOblConfiguration::SetDateValue%882264750.body preserve=yes
	return ClExObl::SetDateValue (ai_hBlockHandle, a_pszName, a_pszDate) ;
  //## end ClOblConfiguration::SetDateValue%882264750.body
}

int ClOblConfiguration::AddNumericProperty (char* a_pszPath, char* a_pszName, double a_dNumber)
{
  //## begin ClOblConfiguration::AddNumericProperty%882264751.body preserve=yes
	return ClExObl::AddNumericProperty (a_pszPath, a_pszName, a_dNumber) ;
  //## end ClOblConfiguration::AddNumericProperty%882264751.body
}

int ClOblConfiguration::AddNumericProperty (UINT a_hBlockHandle, char* a_pszName, double a_dNumber)
{
  //## begin ClOblConfiguration::AddNumericProperty%882264752.body preserve=yes
	return ClExObl::AddNumericProperty (a_hBlockHandle, a_pszName, a_dNumber) ;
  //## end ClOblConfiguration::AddNumericProperty%882264752.body
}

int ClOblConfiguration::AddStringProperty (char* a_pszPath, char* a_pszName, char* a_pszString)
{
  //## begin ClOblConfiguration::AddStringProperty%882264753.body preserve=yes
	return ClExObl::AddStringProperty (a_pszPath, a_pszName, a_pszString) ;
  //## end ClOblConfiguration::AddStringProperty%882264753.body
}

int ClOblConfiguration::AddStringProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszString)
{
  //## begin ClOblConfiguration::AddStringProperty%882264754.body preserve=yes
	return ClExObl::AddStringProperty (a_hBlockHandle, a_pszName, a_pszString) ;
  //## end ClOblConfiguration::AddStringProperty%882264754.body
}

int ClOblConfiguration::AddDateProperty (char* a_pszPath, char* a_pszName, char* a_pszDate)
{
  //## begin ClOblConfiguration::AddDateProperty%882264755.body preserve=yes
	return ClExObl::AddDateProperty (a_pszPath, a_pszName, a_pszDate) ;
  //## end ClOblConfiguration::AddDateProperty%882264755.body
}

int ClOblConfiguration::AddDateProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszDate)
{
  //## begin ClOblConfiguration::AddDateProperty%882264756.body preserve=yes
	return ClExObl::AddDateProperty (a_hBlockHandle, a_pszName, a_pszDate) ;
  //## end ClOblConfiguration::AddDateProperty%882264756.body
}

int ClOblConfiguration::AddBlock (char* a_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle)
{
  //## begin ClOblConfiguration::AddBlock%882264757.body preserve=yes
	return ClExObl::AddBlock (a_pszPath, pszClass, pszInstance, ao_rhHandle) ;
  //## end ClOblConfiguration::AddBlock%882264757.body
}

int ClOblConfiguration::WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut)
{
  //## begin ClOblConfiguration::WriteOut%882264758.body preserve=yes
	return ClExObl::WriteOut (ao_szTextOut, ai_iLenBuf, ao_iLenOut) ;
  //## end ClOblConfiguration::WriteOut%882264758.body
}

void ClOblConfiguration::GetConfigvariables ()
{
  //## begin ClOblConfiguration::GetConfigvariables%882264759.body preserve=yes
  //## end ClOblConfiguration::GetConfigvariables%882264759.body
}

int ClOblConfiguration::GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength)
{
  //## begin ClOblConfiguration::GetAllMatchingBlocks%882350088.body preserve=yes
	return ClExObl::GetAllMatchingBlocks ( ai_pszPath , ao_auiBlockHandles , ai_iHandleSize , ao_riHandleLength ) ;
  //## end ClOblConfiguration::GetAllMatchingBlocks%882350088.body
}

int ClOblConfiguration::GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq)
{
  //## begin ClOblConfiguration::GetPathBlq%882350090.body preserve=yes
	return ClExObl::GetPathBlq ( ai_hBlq , ao_szPathBlq) ;
  //## end ClOblConfiguration::GetPathBlq%882350090.body
}

ClConfiguration * ClOblConfiguration::GetBloqConfiguration (m4uint32_t ai_hBlq)
{
  //## begin ClOblConfiguration::GetBloqConfiguration%884203091.body preserve=yes
	ClConfiguration * poConfiguration;
	UINT iRet , hObl , hNew , iLen;
	m4char_t szObl [20240] = "VERSION ";


	

	hObl = GetHandle ( ) ;

	iRet = M4AOblGetVersion ( hObl , & szObl [8] , 10240 , &iLen ) ;

	if ( iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "No versión for Obl Generating new one from a block" ) ;

		return NULL ;
	}

	strcat ( szObl , "\n" ) ;

	iRet = M4AOblWriteBloque ( hObl , ai_hBlq , & szObl [ strlen ( szObl ) ] , 10240 , &iLen ) ;

	if ( iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Block found Generating new obl file from a block" ) ;

		return NULL ;
	}
	//sprintf ( szObl , "VERSION %s\n%s" , szVersion , szBloque ) ;

	iRet = M4AOblInicializa ( "" , "" , "" , "" , &hNew ) ;

	iRet = M4AOblGeneraObl ( hNew , szObl ) ;

	if ( iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Error Generating new obl file from a block string" ) ;

		return NULL ;
	}

	poConfiguration = new ClOblConfiguration ( hNew ) ;


	return poConfiguration ;

  //## end ClOblConfiguration::GetBloqConfiguration%884203091.body
}

m4return_t ClOblConfiguration::GetType (m4pchar_t ao_szType, m4uint32_t ai_iLenBuffer)
{
  //## begin ClOblConfiguration::GetType%884255523.body preserve=yes
	size_t	iLength;
	m4return_t iRet ;

	
	iRet = ClExObl::GetStringValue ( "" , "TYPE" , ao_szType , 100 , iLength ) ;

	if ( iRet != M4_SUCCESS )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Configuration whithout TYPE property" ) ;

		return M4_ERROR ;
	}



return ( M4_SUCCESS ) ;

  //## end ClOblConfiguration::GetType%884255523.body
}

int ClOblConfiguration::Init (const char *ai_szFile, size_t ai_iReadSize, size_t ai_iWriteSize)
{
  //## begin ClOblConfiguration::Init%916218709.body preserve=yes
	return ClExObl::Init (ai_szFile, ai_iReadSize, ai_iWriteSize ) ;
  //## end ClOblConfiguration::Init%916218709.body
}

m4return_t ClOblConfiguration::GetFirstProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &ao_iHndProp, void * & ao_iHndMatch)
{
  //## begin ClOblConfiguration::GetFirstProp%917598968.body preserve=yes
	return M4_SUCCESS;
  //## end ClOblConfiguration::GetFirstProp%917598968.body
}

m4return_t ClOblConfiguration::GetNextProp (m4pchar_t ai_szMatchProp, m4pchar_t ao_szProperty, m4uint32_t ai_iTamMaxProp, m4uint32_t &ao_iTamProp, m4pchar_t ao_szValue, m4uint32_t ai_iTamMaxValue, m4uint32_t &ao_iTamValu, m4uint32_t &aio_iHndProp, void * & aio_iHndMatch)
{
  //## begin ClOblConfiguration::GetNextProp%917598971.body preserve=yes
	return M4_SUCCESS ;
  //## end ClOblConfiguration::GetNextProp%917598971.body
}

int ClOblConfiguration::GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClOblConfiguration::GetClass%959065913.body preserve=yes
      return ClExObl::GetClass (ao_pszString, ai_iStringSize, ao_riOutput);
  //## end ClOblConfiguration::GetClass%959065913.body
}

// Additional Declarations
  //## begin ClOblConfiguration%348E5D8C0173.declarations preserve=yes
  //## end ClOblConfiguration%348E5D8C0173.declarations

//## begin module%3493E7720009.epilog preserve=yes
//## end module%3493E7720009.epilog
