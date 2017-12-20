//## begin module%37E7B49D0238.cm preserve=no
//## end module%37E7B49D0238.cm

//## begin module%37E7B49D0238.cp preserve=no
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
//## end module%37E7B49D0238.cp

//## Module: clmemorylevelss%37E7B49D0238; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clmemorylevelss.cpp

//## begin module%37E7B49D0238.additionalIncludes preserve=no
//## end module%37E7B49D0238.additionalIncludes

//## begin module%37E7B49D0238.includes preserve=yes
#include <ssnames.hpp>
#include <cloblconfiguration.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
//## end module%37E7B49D0238.includes

// clmemorylevelss
#include <clmemorylevelss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
#include <math.h>
//## begin module%37E7B49D0238.declarations preserve=no
//## end module%37E7B49D0238.declarations

//## begin module%37E7B49D0238.additionalDeclarations preserve=yes
#define	MAX_PERCENT_LOW_GREEN_MEMORY		0
#define	MIN_PERCENT_LOW_GREEN_MEMORY		0
#define	MAX_PERCENT_LOW_YELLOW_MEMORY		96
#define	MIN_PERCENT_LOW_YELLOW_MEMORY		20
#define	MAX_PERCENT_LOW_RED_MEMORY			98
#define	MIN_PERCENT_LOW_RED_MEMORY			70

#define	MAX_PERCENT_HIGH_GREEN_MEMORY		97
#define	MIN_PERCENT_HIGH_GREEN_MEMORY		20
#define	MAX_PERCENT_HIGH_YELLOW_MEMORY		99
#define	MIN_PERCENT_HIGH_YELLOW_MEMORY		30
#define	MAX_PERCENT_HIGH_RED_MEMORY			0
#define	MIN_PERCENT_HIGH_RED_MEMORY			0

#define MEM_INTERVAL_OVERLAPPED					5
//## end module%37E7B49D0238.additionalDeclarations


// Class ClMemoryLevelSS 






ClMemoryLevelSS::ClMemoryLevelSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClMemoryLevelSS::ClMemoryLevelSS%937897778.hasinit preserve=no
  //## end ClMemoryLevelSS::ClMemoryLevelSS%937897778.hasinit
  //## begin ClMemoryLevelSS::ClMemoryLevelSS%937897778.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguratoin, ai_poTables, ai_poNavigator )
  //## end ClMemoryLevelSS::ClMemoryLevelSS%937897778.initialization
{
  //## begin ClMemoryLevelSS::ClMemoryLevelSS%937897778.body preserve=yes
		m_iLowAbsMemory = 0 ;
		m_iHighAbsMemory = 0 ;
		m_iLowPercentMemory = 0 ;
		m_iHighPercentMemory = 0 ;
  //## end ClMemoryLevelSS::ClMemoryLevelSS%937897778.body
}


ClMemoryLevelSS::~ClMemoryLevelSS ()
{
  //## begin ClMemoryLevelSS::~ClMemoryLevelSS%937897779.body preserve=yes
  //## end ClMemoryLevelSS::~ClMemoryLevelSS%937897779.body
}



//## Other Operations (implementation)
m4return_t ClMemoryLevelSS::Initialize ()
{
  //## begin ClMemoryLevelSS::Initialize%937897780.body preserve=yes
	m4double_t dAux;
	m4char_t szColor [20];
	size_t iLen ;
	
	if ( M4_SUCCESS == GetConfigNumericValue ( "ABS_LOW_MEMORY" , dAux ) )
	{
		m_iLowAbsMemory = (m4uint32_t)dAux ;
	}
	else
	{
		SetConfigNumericProperty ( "ABS_LOW_MEMORY" , 0 ) ;
	}

	if ( M4_SUCCESS == GetConfigNumericValue ( "ABS_HIGH_MEMORY" , dAux ) )
	{
		m_iHighAbsMemory = (m4uint32_t)dAux ;
	} 
	else
	{
		SetConfigNumericProperty ( "ABS_HIGH_MEMORY" , 0 ) ;
	}

	if ( M4_SUCCESS == GetConfigNumericValue ( "PERCENT_LOW_MEMORY" , dAux ) )
	{
		m_iLowPercentMemory = (m4uint32_t)dAux ;
	}
	else
	{
		SetConfigNumericProperty ( "PERCENT_LOW_MEMORY" , 0 ) ;
	}

	if ( M4_SUCCESS == GetConfigNumericValue ( "PERCENT_HIGH_MEMORY" , dAux ) )
	{
		m_iHighPercentMemory = (m4uint32_t)dAux ;
	}
	else
	{
		SetConfigNumericProperty( "PERCENT_HIGH_MEMORY" , 0 ) ;
	}

	if ( M4_ERROR == GetConfigStringValue ( COLOR , szColor , 20 , iLen ) )
	{
		return M4_ERROR ;
	}

	switch ( m_poTables -> GetId ( szColor ) )
	{
	case 0 :
		m_iColor = MEMORY_LEVEL_GREEN ;

		break ;

	case 1 :
		m_iColor = MEMORY_LEVEL_YELLOW ;

		break;

	case 2:
		m_iColor = MEMORY_LEVEL_RED ;

	}

	CheckInitialConfiguration();

	return M4_SUCCESS;
  //## end ClMemoryLevelSS::Initialize%937897780.body
}

m4return_t ClMemoryLevelSS::IsMachingLevel (m4uint32_t ai_iAbsMemory, m4uint32_t ai_iPercentMemory, eMemoryPolicy ai_iPolicy)
{
  //## begin ClMemoryLevelSS::IsMachingLevel%937897781.body preserve=yes
	m4return_t iRet ;
	
	switch ( ai_iPolicy )
	{
	case PERCENT_MEMORY :
	case ABS_MEMORY:

		if ( ( ( m_iHighPercentMemory ) && ( ai_iPercentMemory >= m_iHighPercentMemory ) ) ||
			( (m_iHighAbsMemory ) && ( ai_iAbsMemory >= m_iHighAbsMemory ) ) )
		{
				iRet = M4_ABOVE_LEVEL ;
		}
		else
		{
			if ( ( ( ai_iPercentMemory >= m_iLowPercentMemory ) && ( (! m_iHighPercentMemory ) || (  ai_iPercentMemory < m_iHighPercentMemory ) ) ) ||
			 ( ( ai_iAbsMemory >= m_iLowAbsMemory ) && ( (! m_iHighAbsMemory ) || (  ai_iAbsMemory < m_iHighAbsMemory ) ) ) )
			{
				iRet = M4_MATCH_LEVEL ;
			}
			else
			{
				iRet = M4_BELLOW_LEVEL ;
			}
		}

/*
		if ( ai_iAbsMemory < m_iLowAbsMemory )
		{
			iRet = M4_BELLOW_LEVEL ;
		}else
		{
			if ( ( m_iHighAbsMemory ) && ( ai_iAbsMemory > m_iHighAbsMemory ) )
			{
				iRet = M4_ABOVE_LEVEL ;
			}else
			{
				iRet = M4_MATCH_LEVEL ;
			}
		}
	
		if ( iRet != M4_MATCH_LEVEL )
		{
			if ( ai_iPercentMemory < m_iLowPercentMemory )
			{
				iRet = M4_BELLOW_LEVEL ;
			}else
				if ( ( m_iHighPercentMemory ) && ( ai_iPercentMemory > m_iHighPercentMemory ) )
				{
					iRet = M4_ABOVE_LEVEL ;
				}else
				{
					iRet = M4_MATCH_LEVEL ;
				}
		}
*/
	}

	return iRet ;
  //## end ClMemoryLevelSS::IsMachingLevel%937897781.body
}

m4return_t ClMemoryLevelSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClMemoryLevelSS::ExecuteCommand%938162700.body preserve=yes
	ClAdminCommandTag *			newParam;
	m4uint32_t					iPribateBytes ;
	m4uint32_t					iPercentMemory ;
	m4uint32_t					iLow ;
	m4uint32_t					iHigh ;
	eMemoryPolicy				policy ;
	m4char_t					szRet [10] ;
	m4char_t					szLevel [10] ;
	m4char_t					szLow [10] ;
	m4char_t					szHigh [10] ;
	m4return_t					iRet;
	eStatus						eState ;
	ClAdminTagList::iterator	it;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;


	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);

	switch ( ai_cCommand )
	{
	case IS_MATCHING_LEVEL :

		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
				// converts the value
		iPribateBytes = atol((*it)->m_tagValue);

		it ++;

		iPercentMemory =atol((*it)->m_tagValue);

		it ++;

		policy = (eMemoryPolicy ) atol((*it)->m_tagValue);

		iRet = IsMachingLevel ( iPribateBytes , iPercentMemory , policy ) ;

		sprintf (szRet , "%d" , iRet ) ;

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "IS_MATCHING_LEVEL", adminLong, szRet);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);

		break ;

	case GET_LEVEL :

		sprintf (szRet , "%d" , m_iColor ) ;

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "IS_MATCHING_LEVEL", adminLong, szRet);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
		break ;
	case GET_LEVEL_CONFIG :
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		policy = (eMemoryPolicy ) atol((*it)->m_tagValue);

		sprintf (szLevel , "%d" , m_iColor ) ;

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "LEVEL", adminLong, szLevel);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
		switch ( policy )
		{
		case ABS_MEMORY :
			sprintf ( szLow , "%d" , m_iLowAbsMemory ) ;
			sprintf ( szHigh , "%d" , m_iHighAbsMemory ) ;
			break ;
		case PERCENT_MEMORY :
			sprintf ( szLow , "%d" , m_iLowPercentMemory ) ;
			sprintf ( szHigh , "%d" , m_iHighPercentMemory ) ;
			break ;
		}

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "LOW_VALUE", adminLong, szLow);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "HIGH_VALUE", adminLong, szHigh);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
		break ;
	case CHANGE_LEVEL:
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}

		policy = (eMemoryPolicy ) atol((*it)->m_tagValue);
		 
		it ++ ;

		iLow = atol((*it)->m_tagValue);

		it ++;

		iHigh =atol((*it)->m_tagValue);

		if ( M4_ERROR == _ChangeValue ( policy , iLow , iHigh ) )
			return M4_ERROR ;

		break ;
	}

	return M4_SUCCESS;

  //## end ClMemoryLevelSS::ExecuteCommand%938162700.body
}

m4return_t ClMemoryLevelSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClMemoryLevelSS::GetCommandParams%938162701.body preserve=yes
	ClAdminCommandTag *newParam;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	m4return_t retVal=M4_ERROR;

	//-----------------------------------------------------------------------
	// check if the command is a base class command.
	//-----------------------------------------------------------------------
	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	
	//-----------------------------------------------------------------------
	// build the parameters list.
	//-----------------------------------------------------------------------
	
	switch (ai_cCommand)
	{
    case IS_MATCHING_LEVEL:
		// There is not parameters for this command.
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "ABS_PROCESS_MEMORY", adminLong, "0");
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "PERCENT_FREE_MEMORY", adminLong, "1");
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "MEMORY_POLICY", adminLong, "2");
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		retVal=M4_SUCCESS;
		break;
		

	default:
		M4_ASSERT("Request command not recognized" == NULL);
		break;		
	}

	return retVal;
  //## end ClMemoryLevelSS::GetCommandParams%938162701.body
}

m4return_t ClMemoryLevelSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClMemoryLevelSS::GetCommandsId%938162702.body preserve=yes
//	ClAdminTag *newCommand;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;
/* Estos comandos son internos y no se exportan a la herramienta
	OSCAR
	newCommand = ClAdminTag::GetNewAdminTag(IS_MATCHING_LEVEL, "CHECK_MEMORY_LEVEL", M4_TRUE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_FALSE;
	aio_oCommandList.push_back(newCommand);
*/
	
	return  M4_SUCCESS;
  //## end ClMemoryLevelSS::GetCommandsId%938162702.body
}

void ClMemoryLevelSS::CheckInitialConfiguration ()
{
  //## begin ClMemoryLevelSS::CheckInitialConfiguration%941621153.body preserve=yes
	static m4uint32_t iAbsLowYellow = 0 ;
	static m4uint32_t iAbsLowRed = 0 ;
	static m4uint32_t iAbsHighGreen = 0 ;
	static m4uint32_t iAbsHighYellow = 0 ;
	static m4uint32_t iPercentLowYellow = 0 ;
	static m4uint32_t iPercentLowRed = 0 ;
	static m4uint32_t iPercentHighGreen = 0 ;
	static m4uint32_t iPercentHighYellow = 0 ;
	m4char_t szStringId [ OBL_PROPVALUE_MAX_LEN  ];
	m4size_t len ;
	ClAbsoluteMemoryLimits&	oAML = ClAbsoluteMemoryLimits::getInstance();


	GetConfigStringValue ( "STRINGID" , szStringId , OBL_PROPVALUE_MAX_LEN , len ) ;
	switch ( m_iColor )
	{
	case MEMORY_LEVEL_GREEN :
//----------------------Green Low absolute memory-----------------------------
		 m_iLowAbsMemory = oAML[ MAX_ABS_LOW_GREEN_MEMORY ] ;

//----------------------Green high absolute memory----------------------------
		if ( m_iHighAbsMemory > oAML[ MAX_ABS_HIGH_GREEN_MEMORY ] )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighAbsMemory , oAML[ MAX_ABS_HIGH_GREEN_MEMORY ] ) ;
			 m_iHighAbsMemory = oAML[ MAX_ABS_HIGH_GREEN_MEMORY ] ;
		}
		else
		{
			if ( m_iHighAbsMemory  < oAML[ MIN_ABS_HIGH_GREEN_MEMORY ] )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighAbsMemory , oAML[ MIN_ABS_HIGH_GREEN_MEMORY ] ) ;
				m_iHighAbsMemory = oAML[ MIN_ABS_HIGH_GREEN_MEMORY ] ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iAbsLowYellow )
		{
			if ( m_iHighAbsMemory <= iAbsLowYellow )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY in %s. %d is not a valid value (Not overlapped levels: GREEN / YELLOW ), set to %d" , szStringId , m_iHighAbsMemory , iAbsLowYellow + MEM_INTERVAL_OVERLAPPED  ) ;

				m_iHighAbsMemory = iAbsLowYellow + MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iAbsHighGreen = m_iHighAbsMemory ;

//---------------------Green Low percent memory--------------------------------

		 m_iLowPercentMemory = MAX_PERCENT_LOW_GREEN_MEMORY ;

//---------------------Green high percent memory------------------------------

		if ( m_iHighPercentMemory > MAX_PERCENT_HIGH_GREEN_MEMORY )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighPercentMemory , MAX_PERCENT_HIGH_GREEN_MEMORY  ) ;
			 m_iHighPercentMemory = MAX_PERCENT_HIGH_GREEN_MEMORY ;
		}
		else
		{
			if ( m_iHighPercentMemory < MIN_PERCENT_HIGH_GREEN_MEMORY )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighPercentMemory , MIN_PERCENT_HIGH_GREEN_MEMORY   ) ;
				m_iHighPercentMemory = MIN_PERCENT_HIGH_GREEN_MEMORY ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iPercentLowYellow )
		{
			if ( m_iHighPercentMemory <= iPercentLowYellow )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value (Not overlapped levels: GREEN / YELLOW ), set to %d" , szStringId , m_iHighPercentMemory , iPercentLowYellow + MEM_INTERVAL_OVERLAPPED ) ;
				m_iHighPercentMemory = iPercentLowYellow + MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iPercentHighGreen = m_iHighPercentMemory  ;

		break ;

	case MEMORY_LEVEL_YELLOW :

//---------------------Yellow Low Abs memory------------------------------

		if ( m_iLowAbsMemory > oAML[ MAX_ABS_LOW_YELLOW_MEMORY ] )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowAbsMemory , oAML[ MAX_ABS_LOW_YELLOW_MEMORY ] ) ;
			 m_iLowAbsMemory = oAML[ MAX_ABS_LOW_YELLOW_MEMORY ] ;
		}
		else
		{
			if ( m_iLowAbsMemory < oAML[ MIN_ABS_LOW_YELLOW_MEMORY ] )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowAbsMemory , oAML[ MIN_ABS_LOW_YELLOW_MEMORY ] ) ;
				m_iLowAbsMemory = oAML[ MIN_ABS_LOW_YELLOW_MEMORY ] ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iAbsHighGreen )
		{
			if ( m_iLowAbsMemory >= iAbsHighGreen)
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value (Not overlapped levels: GREEN / YELLOW ), set to %d" , szStringId , m_iLowAbsMemory , iAbsHighGreen - MEM_INTERVAL_OVERLAPPED ) ;
				m_iLowAbsMemory = iAbsHighGreen - MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iAbsLowYellow = m_iLowAbsMemory ;

//------------------Yellow high absolute memory-----------------------------

		if ( m_iHighAbsMemory > oAML[ MAX_ABS_HIGH_YELLOW_MEMORY ] )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY  in %s. %d is not a valid value, set to %d" , szStringId , m_iHighAbsMemory , oAML[ MAX_ABS_HIGH_YELLOW_MEMORY ] ) ;
			 m_iHighAbsMemory = oAML[ MAX_ABS_HIGH_YELLOW_MEMORY ] ;
		}
		else
		{
			if ( m_iHighAbsMemory  < oAML[ MIN_ABS_HIGH_YELLOW_MEMORY ] )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY  in %s. %d is not a valid value, set to %d" , szStringId , m_iHighAbsMemory , oAML[ MIN_ABS_HIGH_YELLOW_MEMORY ] ) ;
				m_iHighAbsMemory = oAML[ MIN_ABS_HIGH_YELLOW_MEMORY ] ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iAbsLowRed )
		{
			if ( m_iHighAbsMemory <= iAbsLowRed )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_HIGH_MEMORY in %s. %d is not a valid value (Not overlapped levels: YELLOW / RED ), set to %d" , szStringId , m_iHighAbsMemory , iAbsLowRed + MEM_INTERVAL_OVERLAPPED ) ;
				m_iHighAbsMemory = iAbsLowRed + MEM_INTERVAL_OVERLAPPED ;
			}
		}


		iAbsHighYellow = m_iHighAbsMemory ;

//------------------------Low percent memory------------------------------------

		if ( m_iLowPercentMemory > MAX_PERCENT_LOW_YELLOW_MEMORY)
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowPercentMemory , MAX_PERCENT_LOW_YELLOW_MEMORY ) ;
			 m_iLowPercentMemory = MAX_PERCENT_LOW_YELLOW_MEMORY ;
		}
		else
		{
			if ( m_iLowPercentMemory < MIN_PERCENT_LOW_YELLOW_MEMORY )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowPercentMemory , MIN_PERCENT_LOW_YELLOW_MEMORY ) ;
				m_iLowPercentMemory = MIN_PERCENT_LOW_YELLOW_MEMORY ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iPercentHighGreen )
		{
			if ( m_iLowPercentMemory >= iPercentHighGreen)
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value (Not overlapped levels: GREEN / YELLOW  ), set to %d" , szStringId , m_iLowPercentMemory , iPercentHighGreen - MEM_INTERVAL_OVERLAPPED ) ;
				m_iLowPercentMemory = iPercentHighGreen - MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iPercentLowYellow = m_iLowPercentMemory ;

//-------------------------high percent memory----------------------------------

		if ( m_iHighPercentMemory > MAX_PERCENT_HIGH_YELLOW_MEMORY )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighPercentMemory , MAX_PERCENT_HIGH_YELLOW_MEMORY ) ;
			 m_iHighPercentMemory = MAX_PERCENT_HIGH_YELLOW_MEMORY ;
		}
		else
		{
			if ( m_iHighPercentMemory < MIN_PERCENT_HIGH_YELLOW_MEMORY )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iHighPercentMemory , MIN_PERCENT_HIGH_YELLOW_MEMORY ) ;
				m_iHighPercentMemory = MIN_PERCENT_HIGH_YELLOW_MEMORY ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iPercentLowRed )
		{
			if ( m_iHighPercentMemory <= iPercentLowRed)
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_HIGH_MEMORY in %s. %d is not a valid value (Not overlapped levels: YELLOW / RED ), set to %d" , szStringId , m_iHighPercentMemory , iPercentLowRed + MEM_INTERVAL_OVERLAPPED ) ;
				m_iHighPercentMemory = iPercentLowRed + MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iPercentHighYellow = m_iHighPercentMemory ;

		break ;

	case MEMORY_LEVEL_RED:

//-------------------------Red Low Abs memory----------------------------------

		if ( m_iLowAbsMemory > oAML[ MAX_ABS_LOW_RED_MEMORY ] )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowAbsMemory , oAML[ MAX_ABS_LOW_RED_MEMORY ] ) ;
			 m_iLowAbsMemory = oAML[ MAX_ABS_LOW_RED_MEMORY ] ;
		}
		else
		{
			if ( m_iLowAbsMemory < oAML[ MIN_ABS_LOW_RED_MEMORY ] )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowAbsMemory , oAML[ MIN_ABS_LOW_RED_MEMORY ] ) ;
				m_iLowAbsMemory = oAML[ MIN_ABS_LOW_RED_MEMORY ] ;
			}
		}


		//´Si no hay solapamiento de niveles debe haberlo
		if ( iAbsHighYellow )
		{
			if ( m_iLowAbsMemory >= iAbsHighYellow)
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property ABS_LOW_MEMORY in %s. %d is not a valid value (Not overlapped levels: YELLOW / RED ), set to %d" , szStringId , m_iLowAbsMemory , iAbsHighYellow - MEM_INTERVAL_OVERLAPPED ) ;
				m_iLowAbsMemory = iAbsHighYellow - MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iAbsLowRed = m_iLowAbsMemory  ;

//--------------------------Red high absolute memory-----------------------------

		 m_iHighAbsMemory = oAML[ MAX_ABS_HIGH_RED_MEMORY ] ;

//--------------------------Red Low percent memory------------------------------

		if ( m_iLowPercentMemory > MAX_PERCENT_LOW_RED_MEMORY)
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowPercentMemory , MAX_PERCENT_LOW_RED_MEMORY ) ;
			 m_iLowPercentMemory = MAX_PERCENT_LOW_RED_MEMORY ;
		}
		else
		{
			if ( m_iLowPercentMemory < MIN_PERCENT_LOW_RED_MEMORY )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value, set to %d" , szStringId , m_iLowPercentMemory , MIN_PERCENT_LOW_RED_MEMORY ) ;
				m_iLowPercentMemory = MIN_PERCENT_LOW_RED_MEMORY ;
			}
		}

		//´Si no hay solapamiento de niveles debe haberlo
		if ( iPercentHighYellow )
		{
			if ( m_iLowPercentMemory >= iPercentHighYellow )
			{
				SETCODEF ( M4_SRV_SS_PROPERTY_VALUE_ERROR , WARNINGLOG , "Property PERCENT_LOW_MEMORY in %s. %d is not a valid value (Not overlapped levels: YELLOW / RED ), set to %d" , szStringId , m_iLowPercentMemory , iPercentHighYellow - MEM_INTERVAL_OVERLAPPED ) ;
				m_iLowPercentMemory = iPercentHighYellow - MEM_INTERVAL_OVERLAPPED ;
			}
		}

		iPercentLowRed = m_iLowPercentMemory ;

//--------------------------red  high percent memory-----------------------------

		 m_iHighPercentMemory = MAX_PERCENT_HIGH_RED_MEMORY ;

		iAbsLowYellow = 0 ;
		iAbsLowRed = 0 ;
		iAbsHighGreen = 0 ;
		iAbsHighYellow = 0 ;
		iPercentLowYellow = 0 ;
		iPercentLowRed = 0 ;
		iPercentHighGreen = 0 ;
		iPercentHighYellow = 0 ;

		break ;
	}

	SetConfigNumericProperty ( "PERCENT_LOW_MEMORY" , m_iLowPercentMemory ) ;
	SetConfigNumericProperty ( "PERCENT_HIGH_MEMORY" , m_iHighPercentMemory ) ;
	SetConfigNumericProperty ( "ABS_LOW_MEMORY" , m_iLowAbsMemory  ) ;
	SetConfigNumericProperty ( "ABS_HIGH_MEMORY" ,  m_iHighAbsMemory  ) ;

  //## end ClMemoryLevelSS::CheckInitialConfiguration%941621153.body
}

m4return_t ClMemoryLevelSS::GetSubsystemAttribsId (M4DataStorage *ao_poDataStorage)
{
  //## begin ClMemoryLevelSS::GetSubsystemAttribsId%942768436.body preserve=yes
	m4uint32_t aiIds [] = {ID_TYPE , 
		ID_STRINGID , 
		ID_START_DATE ,
		ID_STATE ,
		ID_COLOR ,
		ID_ABS_HIGH_MEMORY ,
		ID_ABS_LOW_MEMORY ,
		ID_PERCENT_HIGH_MEMORY ,
		ID_PERCENT_LOW_MEMORY };

	m4pchar_t aszProps []= {
		"TYPE" , 
		"STRINGID" , 
		"START_DATE",
		"STATE" ,
		"COLOR" ,
		"ABS_HIGH_MEMORY" ,
		"ABS_LOW_MEMORY" ,
		"PERCENT_HIGH_MEMORY" ,
		"PERCENT_LOW_MEMORY" };

//	double iAttribs  ;
	m4return_t iRet ;
	m4uint16_t i , iType ;
	m4char_t iScale ;
//	m4char_t szPath [MAXNAMEPROP] ;
//	m4char_t szProperty [ MAXNAMEPROP ];
	m4char_t szValue [MAXNAMEPROP] ;
	m4char_t szLiteral [MAXNAMEPROP];
	m4char_t szAttribute [20] ;
	m4pchar_t pLiteral ;
	m4pchar_t szIds ;
	m4char_t szId [10] ;
	m4char_t szScale [10] ;
	m4char_t szType [10] ;
	m4uint32_t iSize = 0;
	ClDataUnit * poDataUnit ;
	size_t iLen ;
	m4char_t szSubsytemType [100] ;
	m4bool_t bAdd;

	
	m_oSyncBellowLevel.EnterReader ();			


	poDataUnit = new ClDataUnit ;

	poDataUnit -> SetSize ( MAXNAMEPROP ) ;

	szIds = poDataUnit -> GetBuffer ( ) ;

	szIds [0] = 0;

	GetConfigStringValue ( "TYPE" , szSubsytemType , 100 , iLen) ;

	m4char_t accessOblPath[256];
	sprintf (accessOblPath, "M4ADMINISTRATOR.ACCESS-PERMISSIONS.%s.ATTRIBUTES", szSubsytemType );

	for ( i = 0 ; i < TOTAL_ATTRIBS ; i++ )
	{
		bAdd = M4_TRUE ;
		switch ( i )
		{
		case ID_ABS_LOW_MEMORY :
			if ( (i==ID_ABS_LOW_MEMORY) && (m_iColor == MEMORY_LEVEL_GREEN ))
			break ;
		case ID_ABS_HIGH_MEMORY :
			if ( (i==ID_ABS_HIGH_MEMORY ) && (m_iColor == MEMORY_LEVEL_RED ))
				break ;
		case ID_PERCENT_LOW_MEMORY :
			if ( (i==ID_PERCENT_LOW_MEMORY ) && (m_iColor == MEMORY_LEVEL_GREEN ))
				break ;
		case ID_PERCENT_HIGH_MEMORY :
			if ( (i==ID_PERCENT_HIGH_MEMORY ) && (m_iColor == MEMORY_LEVEL_RED ))
				break ;
		case ID_TYPE : 
		case ID_STRINGID :
		case ID_START_DATE :

			sprintf ( szId , "%d," , i ) ;

			sprintf ( szAttribute , "%s%d" , ATTRIBUTE , i ) ;

			//------------
			// get in the properties.obl the attribute access-permisions.
			iRet = m_poTables->m_oBook->GetStringValue (accessOblPath, szAttribute, szValue, 100, iLen);

			// check if the command is enable.
			if ((iRet==M4_SUCCESS) && (!strcmp(szValue, "NOT-ACCESSIBLE")))
				continue;
			//------------


			iRet = m_poTables -> GetAttrib ( aszProps[i] , iType , iScale , pLiteral ) ;

			if ( iRet == M4_ERROR )
			{
				if (GetName())
					SETCODEF ( M4_SRV_SS_ATTRIBUTES_ERROR , ERRORLOG , "No attributes (Type or Scale) for attribute %s in subsystem %s" , szValue , GetName () ) ;

				m_oSyncBellowLevel.LeaveReader ();			
				return M4_ERROR ;
			}
		//si la propiedad solicitada es 
			if ( ! strcmp ( aszProps[i] , M4_SUBSYSTEM_TYPE ) )
			{
				if ( GetConfigStringValue ( M4_SUBSYSTEM_TYPE , szLiteral , 100 , iLen ) == M4_ERROR )
				{
					m_oSyncBellowLevel.LeaveReader ();			
					return M4_ERROR ;
				}
				pLiteral = szLiteral ;
			}

			sprintf ( szScale , ",%d;" , iScale ) ;

			sprintf ( szType , "%d," , iType ) ;

			iSize = strlen ( szId ) + strlen (szType ) + strlen ( pLiteral ) + strlen ( szScale ) + 1 ;
		
			if ( iSize + strlen ( szIds ) > poDataUnit -> GetSize ( ) )
			{
			
				poDataUnit -> SetSize ( poDataUnit -> GetSize ( ) + MAXNAMEPROP ) ;

				szIds = poDataUnit -> GetBuffer ( ) ;
			}

			strcat ( szIds , szId ) ;

			strcat ( szIds , szType ) ;

			strcat ( szIds , pLiteral ) ;

			strcat ( szIds , szScale ) ;

			}
		}
	
	ao_poDataStorage -> Add ( poDataUnit ) ;

	m_oSyncBellowLevel.LeaveReader ();			

	return  M4_SUCCESS; 
  //## end ClMemoryLevelSS::GetSubsystemAttribsId%942768436.body
}

// Additional Declarations
  //## begin ClMemoryLevelSS%37E7B51C0303.declarations preserve=yes
m4return_t   ClMemoryLevelSS::_ChangeValue ( eMemoryPolicy policy , m4uint32_t iLow , m4uint32_t iHigh ) 
{
	switch ( policy )
	{
	case ABS_MEMORY :
		m_iLowAbsMemory  = iLow ;
		m_iHighAbsMemory  = iHigh ;
		break ;
	case PERCENT_MEMORY :
		m_iLowPercentMemory  = iLow ;
		m_iHighPercentMemory  = iHigh ;
		break ;
	}
  
	CheckInitialConfiguration ( ) ;

	return M4_SUCCESS ;
}
  //## end ClMemoryLevelSS%37E7B51C0303.declarations

//## begin module%37E7B49D0238.epilog preserve=yes
//## end module%37E7B49D0238.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClMemoryLevelSS::GetSubsystemAttribValue%942768435.body preserve=no
	size_t iLen ;
	double dValue ;
	m4pchar_t szValue ;
	m4char_t szName [OBL_PROPVALUE_MAX_LEN] ;
	m4char_t szPath [OBL_PROPVALUE_MAX_LEN]  ;
	m4char_t szAttribute [20] ;
	m4return_t iRet = M4_SUCCESS;
	eStatus eState;

	
	GetState ( eState ) ;
	if ( eState == SS_NOT_ACCESIBLE )
		return NULL ;


	m_oSyncBellowLevel.EnterReader ();

	ao_poDataStorage -> GetNewBuffer ( szValue , OBL_PROPVALUE_MAX_LEN ) ;

		switch ( ai_iAttribId  )
		{
		case ID_TYPE :
			strcpy ( szName , "TYPE" ) ;
			break ;
		case ID_STRINGID :
			strcpy ( szName , "STRINGID" ) ;
			break ;
		case ID_START_DATE :
			strcpy ( szName , "START_DATE" ) ;
			break ;
		case ID_STATE :
			strcpy ( szName , "STATE" ) ;
			break ;
		case ID_ABS_LOW_MEMORY :
			strcpy ( szName , "ABS_LOW_MEMORY" ) ;
			break ;
		case ID_ABS_HIGH_MEMORY :
			strcpy ( szName , "ABS_LOW_MEMORY" ) ;
			break ;
		case ID_PERCENT_LOW_MEMORY :
			strcpy ( szName , "ABS_LOW_MEMORY" ) ;
			break ;
		case ID_PERCENT_HIGH_MEMORY :
			strcpy ( szName , "ABS_LOW_MEMORY" ) ;
			break ;
		};

		if ( m_poConfiguration -> GetStringValue ( "" , szName , szValue , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
		{
			iRet = m_poConfiguration -> GetNumericValue ( "" , szName , dValue ) ;
			if ( iRet == M4_ERROR )
			{
				m_oSyncBellowLevel.LeaveReader ();			
				return M4_ERROR ;
			}
			sprintf ( szValue , "%d" , (m4uint32_t) dValue ) ;
		}

//	}

	//----------------------------------------------------------
	// Check for the syntax of the data in this attribute.
	// Some values conflicts with the VB client administrator 
	// protocol
	//----------------------------------------------------------
	m4pchar_t pcBuffer = szValue;
	for (m4uint32_t i=0; i<strlen(pcBuffer); i++)
	{
		if ((pcBuffer[i] == ':') ||
			(pcBuffer[i] == ',') ||
			(pcBuffer[i] == ';'))
		{
			pcBuffer[i] = '\'';
		}
	}
	
	ao_poDataStorage -> Add ( szValue , strlen ( szValue ) +1 , M4_TRUE ) ;

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
//## end ClMemoryLevelSS::GetSubsystemAttribValue%942768435.body

#endif



ClAbsoluteMemoryLimits::ClAbsoluteMemoryLimits()
{
	m_pvMemoryLimits = NULL;

	Initialize();
}

ClAbsoluteMemoryLimits::~ClAbsoluteMemoryLimits()
{
	if( m_pvMemoryLimits != NULL )
	{
		m_pvMemoryLimits->clear();
		delete m_pvMemoryLimits;
	}
}

ClAbsoluteMemoryLimits& ClAbsoluteMemoryLimits::getInstance()
{
	static ClAbsoluteMemoryLimits	oInstance;
    
	return oInstance;
}

int ClAbsoluteMemoryLimits::operator[]( int ai_iWhichLimit )
{
	return( m_pvMemoryLimits->at( ai_iWhichLimit ) );
}

m4return_t	ClAbsoluteMemoryLimits::Initialize()
{
	m_pvMemoryLimits = new vector< int >();
	m_pvMemoryLimits->reserve( ENUM_NUM_ITEMS );

#ifdef _WINDOWS

	// Sets limits based on max process limits.
	MEMORYSTATUS	stStatus;
	double			dMaxProcSzMB;

	stStatus.dwLength = sizeof (stStatus) ;

	GlobalMemoryStatus( &stStatus ) ;
	dMaxProcSzMB = stStatus.dwTotalVirtual / (1024 * 1024) ;

	
	m_pvMemoryLimits->push_back( 0 );										// MAX_ABS_LOW_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MIN_ABS_LOW_GREEN_MEMORY
	m_pvMemoryLimits->push_back( getCeiledPercent( 58, dMaxProcSzMB ) );	// MAX_ABS_LOW_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 30 );										// MIN_ABS_LOW_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( dMaxProcSzMB );							// MAX_ABS_LOW_RED_MEMORY
	m_pvMemoryLimits->push_back( 50 );										// MIN_ABS_LOW_RED_MEMORY

	m_pvMemoryLimits->push_back( getCeiledPercent( 78, dMaxProcSzMB  ) );	// MAX_ABS_HIGH_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 50 );										// MIN_ABS_HIGH_GREEN_MEMORY
	m_pvMemoryLimits->push_back( dMaxProcSzMB );							// MAX_ABS_HIGH_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 100 );										// MIN_ABS_HIGH_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MAX_ABS_HIGH_RED_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MIN_ABS_HIGH_RED_MEMORY

#else
	
	// Sets classic limits.
	m_pvMemoryLimits->push_back( 0 );										// MAX_ABS_LOW_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MIN_ABS_LOW_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 1200 );									// MAX_ABS_LOW_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 30 );										// MIN_ABS_LOW_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 2048 );									// MAX_ABS_LOW_RED_MEMORY
	m_pvMemoryLimits->push_back( 50 );										// MIN_ABS_LOW_RED_MEMORY

	m_pvMemoryLimits->push_back( 1600 );									// MAX_ABS_HIGH_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 50 );										// MIN_ABS_HIGH_GREEN_MEMORY
	m_pvMemoryLimits->push_back( 2048 );									// MAX_ABS_HIGH_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 100 );										// MIN_ABS_HIGH_YELLOW_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MAX_ABS_HIGH_RED_MEMORY
	m_pvMemoryLimits->push_back( 0 );										// MIN_ABS_HIGH_RED_MEMORY

#endif


	return M4_SUCCESS;
}

int	ClAbsoluteMemoryLimits::getCeiledPercent( int ai_iPercent, int ai_iMaxValue )
{
	int		iValue;

	iValue = ai_iPercent * ( ai_iMaxValue / 100 );
	iValue = ceil( ( double )iValue / 100 ) * 100;


	return iValue;
}