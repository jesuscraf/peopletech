//## begin module%392958EE01A8.cm preserve=no
//## end module%392958EE01A8.cm

//## begin module%392958EE01A8.cp preserve=no
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
//## end module%392958EE01A8.cp

//## Module: loadrule%392958EE01A8; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\loadrule.cpp

//## begin module%392958EE01A8.additionalIncludes preserve=no
//## end module%392958EE01A8.additionalIncludes

//## begin module%392958EE01A8.includes preserve=yes
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module%392958EE01A8.includes

// clssinterfaz
#include <clssinterfaz.hpp>
// loadrule
#include <loadrule.hpp>
//## begin module%392958EE01A8.declarations preserve=no
//## end module%392958EE01A8.declarations

//## begin module%392958EE01A8.additionalDeclarations preserve=yes
//## end module%392958EE01A8.additionalDeclarations


// Class ClLoadRule 





ClLoadRule::ClLoadRule (m4pcchar_t ai_szCommandName)
  //## begin ClLoadRule::ClLoadRule%960220697.hasinit preserve=no
  //## end ClLoadRule::ClLoadRule%960220697.hasinit
  //## begin ClLoadRule::ClLoadRule%960220697.initialization preserve=yes
  //## end ClLoadRule::ClLoadRule%960220697.initialization
{
  //## begin ClLoadRule::ClLoadRule%960220697.body preserve=yes
	m_szCommandName = new m4char_t [ strlen ( ai_szCommandName ) +1 ] ;

	strcpy ( m_szCommandName , ai_szCommandName ) ;
  //## end ClLoadRule::ClLoadRule%960220697.body
}



//## Other Operations (implementation)
m4return_t ClLoadRule::GetSubsystemLoad (ClParamList &ao_oFinalParamList)
{
  //## begin ClLoadRule::GetSubsystemLoad%960220698.body preserve=yes
	ClAdminTagList oParamList,oResultList;
	ClAdminTagList::iterator Iterator ;
	ParamNameList::iterator ParamNameIterator ;
	m4double_t dArg = 0 ;
	m4char_t szId [20] ;


	sprintf ( szId , "%ld" , (m4uint32_t) m_poSubsystem -> GetId () ) ;

	if ( M4_ERROR == m_poSubsystem -> ExecuteCommand (szId , m_iCommandId , oParamList , oResultList ) )
	{
		return M4_ERROR ;
	}

	for ( Iterator = oResultList.begin() ; Iterator != oResultList.end() ; Iterator ++ )
	{
		for ( ParamNameIterator = m_oParamNameList.begin () ;
			ParamNameIterator != m_oParamNameList.end () ;
			ParamNameIterator ++)
			{
				if ( ! stricmp ( (*Iterator)-> m_tagStringId , (* ParamNameIterator ).c_str() ) )
				{
					ao_oFinalParamList.SetDouble ( (* ParamNameIterator ).c_str() , atof ( (*Iterator)-> m_tagValue ) ) ;
				}
			}
	}
			

    // free local list.
    for ( Iterator = oParamList.begin() ; Iterator != oParamList.end() ; Iterator ++ )
	{
        ClAdminTag *poTag = *Iterator;
        if (poTag != NULL)
        {
            delete poTag;
        }       
    }
    for ( Iterator = oResultList.begin() ; Iterator != oResultList.end() ; Iterator ++ )
	{
        ClAdminTag *poTag = *Iterator;
        if (poTag != NULL)
        {
            delete poTag;
        }       
    }

    return M4_SUCCESS ;
  //## end ClLoadRule::GetSubsystemLoad%960220698.body
}

m4return_t ClLoadRule::Initialize (ClNavigatorFirewall *ai_poNavigator, m4pchar_t ai_szSubsystemName)
{
  //## begin ClLoadRule::Initialize%960220699.body preserve=yes
	ClAdminTagList oCommandList ;
	ClAdminTag * poTag ;
	m4uint32_t iCommands;
	m4bool_t bCommand = M4_FALSE ;
	ClAdminTagList::iterator Iterator ;
	m4char_t szId [20] ;

	//Busco el subsystema
	m_poSubsystem = (ClSSInterfaz*) ai_poNavigator ->GetSubsystemByName ( ai_szSubsystemName ) ;

	if ( ! m_poSubsystem ) 
	{
		return M4_ERROR ; 
	}

	sprintf ( szId , "%ld" , (m4uint32_t) m_poSubsystem -> GetId () ) ;

	//pido la lista de comandos
	if ( M4_ERROR == m_poSubsystem -> GetCommandsId ( szId , oCommandList ) )
	{
		return M4_ERROR ;
	}

	iCommands = oCommandList.size() ;

	//Busco en la lista de comandos el que me interesa que es "ESTIAMTE_LOAD"
	for ( Iterator = oCommandList.begin() ; Iterator != oCommandList.end() ; Iterator ++)
	{
		poTag = (*Iterator) ;
		if (! strcmp  ( poTag -> m_tagStringId , m_szCommandName ) )
		{
			bCommand = M4_TRUE ;
			break ;
		}
	}

	if ( bCommand )
	{
		m_iCommandId = (*Iterator)->m_tagId  ;

		bCommand = M4_FALSE ;
	}
	else
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Command %s not found in %s subsystem" , m_szCommandName, ai_szSubsystemName ) ;

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLoadRule::Initialize%960220699.body
}

m4return_t ClLoadRule::SetParamName (m4pcchar_t ai_szParamName)
{
  //## begin ClLoadRule::SetParamName%960220700.body preserve=yes
	m_oParamNameList.push_back ( ai_szParamName ) ;
	return M4_SUCCESS ;
  //## end ClLoadRule::SetParamName%960220700.body
}

// Additional Declarations
  //## begin ClLoadRule%393BCB1400A4.declarations preserve=yes
ClLoadRule::~ClLoadRule()
{
    if (m_szCommandName != NULL)
    {
        delete m_szCommandName;
    }
}
  //## end ClLoadRule%393BCB1400A4.declarations

//## begin module%392958EE01A8.epilog preserve=yes
//## end module%392958EE01A8.epilog
