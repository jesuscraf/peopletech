//## begin module%38102BD3030B.cm preserve=no
//## end module%38102BD3030B.cm

//## begin module%38102BD3030B.cp preserve=no
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
//## end module%38102BD3030B.cp

//## Module: ClJSChannelHandler30%38102BD3030B; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: E:\maite_work\m4jsexejob\version\src\cljschannelhandler30.cpp

//## begin module%38102BD3030B.additionalIncludes preserve=no
//## end module%38102BD3030B.additionalIncludes

//## begin module%38102BD3030B.includes preserve=yes
#include <m4jsres.hpp>

//Channel
#include <channel.hpp>
//access
#include <access.hpp>
//## end module%38102BD3030B.includes

// ClJSChannelHandler30
#include <cljschannelhandler30.hpp>
// ClJSChannelHandler
#include <cljschannelhandler.hpp>
//## begin module%38102BD3030B.declarations preserve=no
//## end module%38102BD3030B.declarations

//## begin module%38102BD3030B.additionalDeclarations preserve=yes
//## end module%38102BD3030B.additionalDeclarations


// Class ClJSChannelHandler30 

ClJSChannelHandler30::ClJSChannelHandler30 ()
  //## begin ClJSChannelHandler30::ClJSChannelHandler30%940584197.hasinit preserve=no
  //## end ClJSChannelHandler30::ClJSChannelHandler30%940584197.hasinit
  //## begin ClJSChannelHandler30::ClJSChannelHandler30%940584197.initialization preserve=yes
  :ClJSChannelHandler()
  //## end ClJSChannelHandler30::ClJSChannelHandler30%940584197.initialization
{
  //## begin ClJSChannelHandler30::ClJSChannelHandler30%940584197.body preserve=yes
  //## end ClJSChannelHandler30::ClJSChannelHandler30%940584197.body
}



//## Other Operations (implementation)
m4return_t ClJSChannelHandler30::BuildChannel (m4pcchar_t ai_pccChannel, ClChannel *&ao_poChannel, ClAccess *&ao_poAccess)
{
  //## begin ClJSChannelHandler30::BuildChannel%940584198.body preserve=yes


	//Creamos el puntero al canal
	m4return_t ret=m_poChannelManager->CreateChannel(ao_poChannel);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_CREATE_META4OBJECT,ERRORLOG,"#*S1*#%s",ai_pccChannel);

		ao_poChannel=NULL;

		return M4_ERROR;
	}

	if (NULL == ao_poChannel)
	{
		SETCODEF(M4_ERROR_CREATE_META4OBJECT,ERRORLOG,"#*S1*#%s",ai_pccChannel);

		return M4_ERROR;
	}

	//Construimos el canal
	ret=ao_poChannel->BuildFromId(ai_pccChannel);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_BUILD_CHANNEL_ERROR,ERRORLOG,"#*S1*#%s",ai_pccChannel);

		ao_poChannel->Destroy();

		ao_poChannel=NULL;

		return M4_ERROR;
	}

	//Creamos el acceso al canal
	ao_poAccess=ao_poChannel->CreateAccess();

	if (NULL == ao_poAccess)
	{
		SETCODEF(M4_ERROR_CREATE_ACCESS,ERRORLOG,"#*S1*#%s",ai_pccChannel);

		ao_poChannel->Destroy();

		ao_poChannel=NULL;

		return M4_ERROR;
	}

	return M4_SUCCESS;


  //## end ClJSChannelHandler30::BuildChannel%940584198.body
}

// Additional Declarations
  //## begin ClJSChannelHandler30%38102A23015B.declarations preserve=yes
  //## end ClJSChannelHandler30%38102A23015B.declarations

//## begin module%38102BD3030B.epilog preserve=yes
//## end module%38102BD3030B.epilog
