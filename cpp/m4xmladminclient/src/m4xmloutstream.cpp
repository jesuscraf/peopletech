//## begin module%3958D873023C.cm preserve=no
//## end module%3958D873023C.cm

//## begin module%3958D873023C.cp preserve=no
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
//## end module%3958D873023C.cp

//## Module: M4XmlOutStream%3958D873023C; Package body
//## Subsystem: M4XMLAdminClient::src%39084D03030B
//## Source file: D:\INTEGRATION\m4xmladminclient\src\m4xmloutstream.cpp

//## begin module%3958D873023C.additionalIncludes preserve=no
//## end module%3958D873023C.additionalIncludes

//## begin module%3958D873023C.includes preserve=yes
//## end module%3958D873023C.includes

// xmlstreamgenerator
#include <xmlstreamgenerator.hpp>
// M4XmlOutStream
#include <m4xmloutstream.hpp>
//## begin module%3958D873023C.declarations preserve=no
//## end module%3958D873023C.declarations

//## begin module%3958D873023C.additionalDeclarations preserve=yes
#include <m4cryptc.hpp>
//## end module%3958D873023C.additionalDeclarations


// Class ClXMLOutStream 

ClXMLOutStream::ClXMLOutStream (m4string_t ai_strQuote)
  //## begin ClXMLOutStream::ClXMLOutStream%962275920.hasinit preserve=no
  //## end ClXMLOutStream::ClXMLOutStream%962275920.hasinit
  //## begin ClXMLOutStream::ClXMLOutStream%962275920.initialization preserve=yes
  :M4XMLStreamGenerator(ai_strQuote)
  //## end ClXMLOutStream::ClXMLOutStream%962275920.initialization
{
  //## begin ClXMLOutStream::ClXMLOutStream%962275920.body preserve=yes
  //## end ClXMLOutStream::ClXMLOutStream%962275920.body
}


ClXMLOutStream::~ClXMLOutStream ()
{
  //## begin ClXMLOutStream::~ClXMLOutStream%962275921.body preserve=yes
  //## end ClXMLOutStream::~ClXMLOutStream%962275921.body
}



//## Other Operations (implementation)
m4return_t ClXMLOutStream::ConfigurationsCommand (m4string_t ai_strUser, m4string_t ai_strPassword)
{
  //## begin ClXMLOutStream::ConfigurationsCommand%962124054.body preserve=yes
	m4return_t		iRet = M4_ERROR;
	m4string_t		sToken;
	
	MakeXMLDeclaration();

	if (M4_SUCCESS != (iRet = InsertComment("@version")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT,ERRORLOG,"Error making XML answer.\nFailure inserting the XML comment \"%s\".", "@version");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT, "@version");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = MakeElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "srvmonitor");

		return M4_ERROR;
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "command");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("name", "configurationsquery")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "name");
		
		return M4_ERROR;
	}

	EndElement();

	// Construir el token.
	iRet = BuildToken(ai_strUser, ai_strPassword, sToken);
	if (iRet != M4_SUCCESS)
	{
		// m4log error message
		// BL_ERRORF(M4_ERR_XMLADMCLNT_BUILDTOKEN, ai_strUser);

		return M4_ERROR;
	}
	
	if (M4_SUCCESS != (iRet = MakeElement("params")))
	{
		// m4log error message
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "params");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("token", sToken)))
	{
		// m4log error message
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "token");
		
		return M4_ERROR;
	}

	CloseSingleClosedElement();

	if (M4_SUCCESS != (iRet = CloseElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "command");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = CloseElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "srvmonitor");
		
		return M4_ERROR;	
	}

	return M4_SUCCESS;

  //## end ClXMLOutStream::ConfigurationsCommand%962124054.body
}

m4return_t ClXMLOutStream::StartServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort)
{
  //## begin ClXMLOutStream::StartServerCommand%962124055.body preserve=yes

	m4uint32_t	len;
	m4return_t	iRet = M4_ERROR;
	m4string_t	sToken;

	if ( 0 >= ( len = ai_strHost.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Host");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strConfiguration.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Server Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Server Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strUser.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "User Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "User Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strPassword.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Password");

		return M4_ERROR;
	}

	MakeXMLDeclaration();

	if (M4_SUCCESS != (iRet = InsertComment("@version")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT,ERRORLOG,"Error making XML answer.\nFailure inserting the XML comment \"%s\".", "@version");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT, "@version");
		
		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = MakeElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "srvmonitor");

		return M4_ERROR;
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "command");

		return M4_ERROR;
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("name", "startserver")))
	{

		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "name");

		return M4_ERROR;
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("params")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "params");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "params");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("host", ai_strHost)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "host");
		
		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("configuration", ai_strConfiguration)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "configuration");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "configuration");

		return M4_ERROR;
	}

	if ( 0 < ( len = ai_strBasePort.size() ) ) {

		if (M4_SUCCESS != (iRet = AddElementAttribute("baseport", ai_strBasePort)))
		{
			// m4log error message
			//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "baseport");
			BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "baseport");
			
			return M4_ERROR;		
		}
	}
	 	
	if (M4_SUCCESS != (iRet = AddElementAttribute("user", ai_strUser)))
	{

		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "user");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "user");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("password", ai_strPassword)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "password");
		
		return M4_ERROR;
	}

	// Construir el token.
	iRet = BuildToken(ai_strUser, ai_strPassword, sToken);
	if (iRet != M4_SUCCESS)
	{
		// m4log error message
		// BL_ERRORF(M4_ERR_XMLADMCLNT_BUILDTOKEN, ai_strUser);

		return M4_ERROR;
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("token", sToken)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "token");
		
		return M4_ERROR;
	}

	CloseSingleClosedElement();

	if (M4_SUCCESS != (iRet = CloseElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "command");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = CloseElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "srvmonitor");
		
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClXMLOutStream::StartServerCommand%962124055.body
}

m4return_t ClXMLOutStream::StopServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort)
{
  //## begin ClXMLOutStream::StopServerCommand%962124056.body preserve=yes

	m4uint32_t	len;
	m4return_t	iRet = M4_ERROR;
	m4string_t	sToken;

	if ( 0 >= ( len = ai_strHost.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Host");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strConfiguration.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Server Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Server Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strUser.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "User Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "User Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strPassword.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Password");

		return M4_ERROR;
	}

	MakeXMLDeclaration();

	if (M4_SUCCESS != (iRet = InsertComment("@version")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT,ERRORLOG,"Error making XML answer.\nFailure inserting the XML comment \"%s\".", "@version");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT, "@version");

		return M4_ERROR;	
	}


	if (M4_SUCCESS != (iRet = MakeElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "srvmonitor");

		return M4_ERROR;	
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "command");

		return M4_ERROR;	
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("name", "stopserver")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "name");

		return M4_ERROR;		
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("params")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "params");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "params");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("host", ai_strHost)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "host");

		return M4_ERROR;	
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("configuration", ai_strConfiguration)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "configuration");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "configuration");

		return M4_ERROR;	
	}

	if ( 0 < ( len = ai_strBasePort.size() ) )
	{
		if (M4_SUCCESS != (iRet = AddElementAttribute("baseport", ai_strBasePort)))
		{
			// m4log error message
			//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "baseport");
			BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "baseport");

			return M4_ERROR;	
		}
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("user", ai_strUser)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "user");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "user");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("password", ai_strPassword)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "password");

		return M4_ERROR;	
	}

	// Construir el token.
	iRet = BuildToken(ai_strUser, ai_strPassword, sToken);
	if (iRet != M4_SUCCESS)
	{
		// m4log error message
		// BL_ERRORF(M4_ERR_XMLADMCLNT_BUILDTOKEN, ai_strUser);

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("token", sToken)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "token");

		return M4_ERROR;	
	}

	CloseSingleClosedElement();


	if (M4_SUCCESS != (iRet = CloseElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "command");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = CloseElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "srvmonitor");

		return M4_ERROR;	
	}

	return M4_SUCCESS;

  //## end ClXMLOutStream::StopServerCommand%962124056.body
}

m4return_t ClXMLOutStream::RemoveServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort)
{
	m4uint32_t	len;
	m4return_t	iRet = M4_ERROR;
	m4string_t	sToken;

	if ( 0 >= ( len = ai_strHost.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Host");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strConfiguration.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Server Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Server Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strUser.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "User Name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "User Name");

		return M4_ERROR;
	}

	if ( 0 >= ( len = ai_strPassword.size() ) )
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM,ERRORLOG,"Error making XML answer.\nParameter \"%s\" incorrect or not provided", "Password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM, "Password");

		return M4_ERROR;
	}

	MakeXMLDeclaration();

	if (M4_SUCCESS != (iRet = InsertComment("@version")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT,ERRORLOG,"Error making XML answer.\nFailure inserting the XML comment \"%s\".", "@version");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT, "@version");

		return M4_ERROR;	
	}


	if (M4_SUCCESS != (iRet = MakeElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "srvmonitor");

		return M4_ERROR;	
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "command");

		return M4_ERROR;	
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("name", "removeserver")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "name");

		return M4_ERROR;		
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("params")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "params");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "params");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("host", ai_strHost)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "host");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "host");

		return M4_ERROR;	
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("configuration", ai_strConfiguration)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "configuration");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "configuration");

		return M4_ERROR;	
	}

	if ( 0 < ( len = ai_strBasePort.size() ) )
	{
		if (M4_SUCCESS != (iRet = AddElementAttribute("baseport", ai_strBasePort)))
		{
			// m4log error message
			//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "baseport");
			BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "baseport");

			return M4_ERROR;	
		}
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("user", ai_strUser)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "user");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "user");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("password", ai_strPassword)))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "password");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "password");

		return M4_ERROR;	
	}

	// Construir el token.
	iRet = BuildToken(ai_strUser, ai_strPassword, sToken);
	if (iRet != M4_SUCCESS)
	{
		// m4log error message
		// BL_ERRORF(M4_ERR_XMLADMCLNT_BUILDTOKEN, ai_strUser);

		return M4_ERROR;
	}
	
	if (M4_SUCCESS != (iRet = AddElementAttribute("token", sToken)))
	{
		// m4log error message
		(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "token");

		return M4_ERROR;	
	}

	CloseSingleClosedElement();


	if (M4_SUCCESS != (iRet = CloseElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "command");

		return M4_ERROR;	
	}
	
	if (M4_SUCCESS != (iRet = CloseElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "srvmonitor");

		return M4_ERROR;	
	}

	return M4_SUCCESS;
}

m4return_t ClXMLOutStream::StopDispatcherCommand (m4string_t ai_strUser, m4string_t ai_strPassword)
{
  //## begin ClXMLOutStream::StopDispatcherCommand%974134062.body preserve=yes


	m4return_t	iRet = M4_ERROR;
	m4string_t	sToken;

	MakeXMLDeclaration();

	if (M4_SUCCESS != (iRet = InsertComment("@version")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT,ERRORLOG,"Error making XML answer.\nFailure inserting the XML comment \"%s\".", "@version");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOINSERTCOMMENT, "@version");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = MakeElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "srvmonitor");

		return M4_ERROR;
	}

	EndElement();

	if (M4_SUCCESS != (iRet = MakeElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT,ERRORLOG,"Error making XML answer.\nFailure making the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "command");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("name", "stopdpch")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE,ERRORLOG,"Error making XML answer.\nFailure adding the XML element attribute\"%s\".", "name");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "name");
		
		return M4_ERROR;
	}

	EndElement();

	// Construir el token.
	iRet = BuildToken(ai_strUser, ai_strPassword, sToken);
	if (iRet != M4_SUCCESS)
	{
		// m4log error message
		// BL_ERRORF(M4_ERR_XMLADMCLNT_BUILDTOKEN, ai_strUser);

		return M4_ERROR;
	}
	
	if (M4_SUCCESS != (iRet = MakeElement("params")))
	{
		// m4log error message
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOMAKEELEMENT, "params");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = AddElementAttribute("token", sToken)))
	{
		// m4log error message
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE, "token");
		
		return M4_ERROR;
	}

	CloseSingleClosedElement();

	if (M4_SUCCESS != (iRet = CloseElement("command")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "command");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "command");

		return M4_ERROR;
	}

	if (M4_SUCCESS != (iRet = CloseElement("srvmonitor")))
	{
		// m4log error message
		//SETCODEF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT,ERRORLOG,"Error making XML answer.\nFailure closing the XML element \"%s\".", "srvmonitor");
		BL_ERRORF(M4_ERR_XMLADMCLNT_NOCLOSEELEMENT, "srvmonitor");
		
		return M4_ERROR;	
	}

	return M4_SUCCESS;

  //## end ClXMLOutStream::StopDispatcherCommand%974134062.body
}

m4return_t ClXMLOutStream::BuildToken(m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t & ao_strToken)
{
	m4return_t	iRet = M4_SUCCESS;
	m4char_t	acString[2 * M4_MAX_LEN_ONE_WAY + 1];
	m4int_t		iOutLength = -1;

	// Inicializaciones
	ao_strToken = "";
	*acString = '\0';
	
	// Construir el token usuario:password (cifrada oneway).
	iRet =  EncryptOneWaySHA(ai_strPassword.c_str(), strlen(ai_strPassword.c_str()) * sizeof(m4char_t), acString, 2 * M4_MAX_LEN_ONE_WAY, iOutLength) ;
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	ao_strToken = ai_strUser + ":" + acString;
	return M4_SUCCESS;
}
// Additional Declarations
  //## begin ClXMLOutStream%3958D4EB00A2.declarations preserve=yes
  //## end ClXMLOutStream%3958D4EB00A2.declarations

//## begin module%3958D873023C.epilog preserve=yes
//## end module%3958D873023C.epilog

