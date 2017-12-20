//## begin module%377C88560206.cm preserve=no
//## end module%377C88560206.cm

//## begin module%377C88560206.cp preserve=no
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
//## end module%377C88560206.cp

//## Module: dbgresou%377C88560206; Package body
//## Subsystem: M4Log::src%377C88560044
//	d:\compon\m4log\version\src
//## Source file: d:\compon\m4log\version\src\dbgresou.cpp

//## begin module%377C88560206.additionalIncludes preserve=no
//## end module%377C88560206.additionalIncludes

//## begin module%377C88560206.includes preserve=yes
//## end module%377C88560206.includes

// dbgresou
#include <dbgresou.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// m4log
#include <m4log.hpp>
#include <logsys.hpp>


//## begin module%377C88560206.declarations preserve=no
//## end module%377C88560206.declarations

//## begin module%377C88560206.additionalDeclarations preserve=yes
//## end module%377C88560206.additionalDeclarations


// Class ClStaticResourceList 




ClStaticResourceList::ClStaticResourceList ()
  //## begin ClStaticResourceList::ClStaticResourceList%-1021373867.hasinit preserve=no
  //## end ClStaticResourceList::ClStaticResourceList%-1021373867.hasinit
  //## begin ClStaticResourceList::ClStaticResourceList%-1021373867.initialization preserve=yes
  //## end ClStaticResourceList::ClStaticResourceList%-1021373867.initialization
{
  //## begin ClStaticResourceList::ClStaticResourceList%-1021373867.body preserve=yes
  //## end ClStaticResourceList::ClStaticResourceList%-1021373867.body
}


ClStaticResourceList::~ClStaticResourceList ()
{
  //## begin ClStaticResourceList::~ClStaticResourceList%-2107676040.body preserve=yes

	DeleteAllResources();

  //## end ClStaticResourceList::~ClStaticResourceList%-2107676040.body
}



//## Other Operations (implementation)
m4return_t ClStaticResourceList::GetLevelInfo (m4int32_t icode, m4uint32_t& ilevelInfo)
{
  //## begin ClStaticResourceList::GetLevelInfo%455248537.body preserve=yes

	RESOURCEMAP::iterator	iter;
	ClDebugResource	*presource;

	
	if ( ( iter = m_resources.find( icode ) ) == m_resources.end() )
	{
		
		ilevelInfo = 0;
		return M4_ERROR;
	}
	
	presource = (* iter).second;

	ilevelInfo = presource -> GetLevelInfo();
	return M4_SUCCESS;

  //## end ClStaticResourceList::GetLevelInfo%455248537.body
}

m4return_t ClStaticResourceList::AddResource (m4int32_t icode, m4uint32_t ilevelInfo)
{
  //## begin ClStaticResourceList::AddResource%-1463548443.body preserve=yes

	//Solo añadimos si no esta
	RESOURCEMAP::iterator itResourceMap;
	itResourceMap = m_resources.find( icode );
	if (  itResourceMap != m_resources.end() ) {
		return M4_ERROR;
	}

	ClDebugResource	*presource;
	presource = new ClDebugResource( ilevelInfo );
	m_resources.insert( RESOURCEMAP::value_type( icode, presource ) );

	return M4_SUCCESS;	

  //## end ClStaticResourceList::AddResource%-1463548443.body
}

m4return_t ClStaticResourceList::UpResource (m4int32_t icode, m4uint32_t ilevelInfo)
{
  //## begin ClStaticResourceList::UpdateResource%-115869996.body preserve=yes

	//Si esta actualizamos el ilevelInfo, y si no esta, lo añadimos.

	RESOURCEMAP::iterator itResourceMap;
	itResourceMap = m_resources.find( icode );
	if (  itResourceMap != m_resources.end() ) {
		//Significa que ya esta en el mapa, lo unico que hacemos es
		//actualizar el ilevelInfo....
		ClDebugResource* oDebugResource = (*itResourceMap).second;
		oDebugResource -> SetLevelInfo(ilevelInfo);
		return M4_WARNING;
	}

	ClDebugResource	*presource;
	presource = new ClDebugResource( ilevelInfo );
	m_resources.insert( RESOURCEMAP::value_type( icode, presource ) );

	return M4_SUCCESS;	

  //## end ClStaticResourceList::UpdateResource%-115869996.body
}

m4return_t ClStaticResourceList::DeleteResource (m4int32_t icode)
{
  //## begin ClStaticResourceList::DeleteResource%411999185.body preserve=yes

	RESOURCEMAP::iterator	iter;
	ClDebugResource	*presource;

	if ( m_resources.find( icode ) == m_resources.end() )
	{
		return M4_ERROR;
	}
	
	presource = (* iter).second;

	m_resources.erase( iter );

	if ( presource )
		delete presource;

	return M4_SUCCESS;

  //## end ClStaticResourceList::DeleteResource%411999185.body
}

m4return_t ClStaticResourceList::DeleteAllResources ()
{
  //## begin ClStaticResourceList::DeleteAllResources%-430054.body preserve=yes

	RESOURCEMAP::iterator	iter, iter1;
	ClDebugResource	*presource;

	iter = m_resources.begin();

	while ( iter != m_resources.end() )
	{
		iter1 = iter;
		presource = (* iter1).second;
		if ( presource )
			delete presource;
		m_resources.erase( iter1 );
		iter = m_resources.begin();

	}

	
	return M4_SUCCESS;	

  //## end ClStaticResourceList::DeleteAllResources%-430054.body
}

m4return_t ClStaticResourceList::AddResourceList (const ClStaticResourceList& ai_ResourceList)
{
  //## begin ClStaticResourceList::AddResourceList%1574313136.body preserve=yes


	RESOURCEMAP *aiMap;
	RESOURCEMAP::iterator it_aiMap;
	ClStaticResourceList *TempResourceList = (ClStaticResourceList *)&ai_ResourceList;
	aiMap = TempResourceList->GetResources();

	it_aiMap = aiMap->begin();
	m4int32_t   icode, ilevelInfo;
	ClDebugResource* oDebugResource;
	while (it_aiMap != aiMap->end()) {
		icode = (*it_aiMap).first;
		oDebugResource = (*it_aiMap).second;
		ilevelInfo = oDebugResource -> GetLevelInfo();
		//Añadimos o modificamos los que existen...
		UpResource(icode, ilevelInfo );
		it_aiMap++;
	}
	return M4_SUCCESS;

  //## end ClStaticResourceList::AddResourceList%1574313136.body
}

ClStaticResourceList& ClStaticResourceList::operator += (const ClStaticResourceList& ai_ResourceList)
{
  //## begin ClStaticResourceList::operator +=%-1307957918.body preserve=yes

	AddResourceList(ai_ResourceList);
	return *this;

  //## end ClStaticResourceList::operator +=%-1307957918.body
}

// Additional Declarations
  //## begin ClStaticResourceList%377C883E0360.declarations preserve=yes
  //## end ClStaticResourceList%377C883E0360.declarations

// Class ClStaticModuleList 










ClStaticModuleList::ClStaticModuleList ()
  //## begin ClStaticModuleList::ClStaticModuleList%219287769.hasinit preserve=no
  //## end ClStaticModuleList::ClStaticModuleList%219287769.hasinit
  //## begin ClStaticModuleList::ClStaticModuleList%219287769.initialization preserve=yes
  //## end ClStaticModuleList::ClStaticModuleList%219287769.initialization
{
  //## begin ClStaticModuleList::ClStaticModuleList%219287769.body preserve=yes

	
	m_Size = 0;
	m_ModuleList = NULL;
	m_FlagsLevels = NULL;
	m_AddIterator = 0;

	m_pDefaultCodeExceptions = new ClStaticResourceList();
	m_patternsMap = new ClPatternsMap();


  //## end ClStaticModuleList::ClStaticModuleList%219287769.body
}

ClStaticModuleList::ClStaticModuleList (m4uint32_t ai_Modules)
  //## begin ClStaticModuleList::ClStaticModuleList%1772360769.hasinit preserve=no
  //## end ClStaticModuleList::ClStaticModuleList%1772360769.hasinit
  //## begin ClStaticModuleList::ClStaticModuleList%1772360769.initialization preserve=yes
  //## end ClStaticModuleList::ClStaticModuleList%1772360769.initialization
{
  //## begin ClStaticModuleList::ClStaticModuleList%1772360769.body preserve=yes

	
	m_pDefaultCodeExceptions = new ClStaticResourceList();
	m_patternsMap = new ClPatternsMap();

  //## end ClStaticModuleList::ClStaticModuleList%1772360769.body
}


ClStaticModuleList::~ClStaticModuleList ()
{
  //## begin ClStaticModuleList::~ClStaticModuleList%-1535370729.body preserve=yes


	MODULEMAP::iterator	iter, iter1;
	ClModuleInfo	*pModule;

	iter = m_ModuleMap.begin();

	while ( iter !=  m_ModuleMap.end() )
	{
		iter1 = iter;
		pModule = (* iter1).second;
		if ( pModule )
			delete pModule;
		m_ModuleMap.erase( iter1 );
		iter = m_ModuleMap.begin();

	}

	delete m_pDefaultCodeExceptions;
	delete m_patternsMap;
  //## end ClStaticModuleList::~ClStaticModuleList%-1535370729.body
}



//## Other Operations (implementation)
m4return_t ClStaticModuleList::GetLevelInfo (const ClLogSystem *ai_pcoLogSystem, m4int32_t ai_Sever, m4int32_t ai_code, m4uint32_t& ilevelInfo)
{
  //## begin ClStaticModuleList::GetLevelInfo%-489667911.body preserve=yes


	// De momento busqueda de orden lineal.
	// buscamos modulo por modulo si el codigo del error
	// esta entre InitCode y FinalCode => buscamos si existe excepcion para ese codigo
	// si existe devolvemos ese nivel, sino, se devuelve el nivel asociado a la severidad
	// del error para ese modulo en concreto.


	ClOutputPattern *pOutputPattern;

	ilevelInfo = 0;
	m4int32_t auxIndex = 0;
	m4uint32_t iOverwriteMask = M4LOG_OVER_ALL ;
	MODULEMAP::iterator	iter;
	ClModuleInfo	*pauxModule;

	iter = m_ModuleMap.begin();

	while ( iter !=  m_ModuleMap.end() )
	{
		pauxModule = (* iter).second;
		
		if (pauxModule == NULL ) {

				ilevelInfo = 0;
				iter++;
				continue;
			}

		if (  pauxModule -> IsInModule( ai_code ) == M4_TRUE ) {

				
			// Miramos si esta desactivado el modulo.
			if (pauxModule -> GetModuleState() == M4_FALSE) {
					
				// No se vuelca a los Storages.
				ilevelInfo = 0;
				return M4_SUCCESS;
			}

				
			// Buscamos la excepcion.
			ilevelInfo = pauxModule -> FindCodeException ( ai_code );
			if (ilevelInfo != -1 ) {
			//	ilevelInfo = m_FlagsLevels[(ilevelInfo- 1)];
				return M4_SUCCESS;
			}
				
			// Si no es excepcion, miramos la severidad asociada al modulo

			pOutputPattern = pauxModule -> GetOutputPattern();

			if (pOutputPattern==NULL) {
				iter++;
				continue;
			}

			/* Bug 0108256
			Se tiene en cuenta lo que diga el nivel del logsys
			*/
			if( ai_pcoLogSystem != NULL )
			{
				if( strcmp( pOutputPattern->GetOutputPatternName().c_str(), "defaultPattern" ) == 0 )
				{
					iOverwriteMask = ai_pcoLogSystem->GetOverwriteMask() ;
				}
			}

			ilevelInfo = 0;

			switch ( ai_Sever )
			{
					
				case WARNINGLOG:
					auxIndex = pOutputPattern -> GetWarningLevel();
					if (auxIndex)
					{
						if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_WARNING ) != 0 )
						{
							ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
						}
					}
					break;

				case DEBUGINFOLOG:
					auxIndex = pOutputPattern ->  GetDebugLevel();
					if (auxIndex)
					{
						if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_DEBUG ) != 0 )
						{
							ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
						}
					}
					break;

				case ERRORLOG:
					auxIndex = pOutputPattern -> GetErrorLevel();
					if (auxIndex)
					{
						if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_ERROR ) != 0 )
						{
							ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
						}
					}
					break;

				case TRACEINFOLOG:
					auxIndex = pOutputPattern -> GetTraceLevel();
					if (auxIndex)
					{
						if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_TRACE ) != 0 )
						{
							ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
						}
					}
					break;

				default:
					return M4_ERROR;
			}

			return M4_SUCCESS;
		}

		iter++;

	}

	// Si no pert. a nigun modulo...

	// Si es una excepcion
	if ((m_pDefaultCodeExceptions -> GetLevelInfo( ai_code, ilevelInfo )) == M4_SUCCESS)
		return M4_SUCCESS;
	
	//si no lo es

	/* Bug 0108256
	Se tiene en cuenta lo que diga el nivel del logsys
	*/
	if( ai_pcoLogSystem != NULL )
	{
		iOverwriteMask = ai_pcoLogSystem->GetOverwriteMask() ;
	}

	ilevelInfo = 0;

	switch ( ai_Sever )
		{
				

		case WARNINGLOG:
			auxIndex = getOutputPatternMap() -> getOutputPatternByName("defaultPattern") -> GetWarningLevel();
			if (auxIndex)
			{
				if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_WARNING ) != 0 )
				{
					ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
				}
			}
			break;

		case DEBUGINFOLOG:
			auxIndex = getOutputPatternMap() -> getOutputPatternByName("defaultPattern") -> GetDebugLevel();
			if (auxIndex)
			{
				if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_DEBUG ) != 0 )
				{
					ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
				}
			}
			break;

		case ERRORLOG:
			auxIndex = getOutputPatternMap() -> getOutputPatternByName("defaultPattern") -> GetErrorLevel();
			if (auxIndex)
			{
				if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_ERROR ) != 0 )
				{
					ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
				}
			}
			break;

		case TRACEINFOLOG:
			auxIndex = getOutputPatternMap() -> getOutputPatternByName("defaultPattern") -> GetTraceLevel();
			if (auxIndex)
			{
				if( ( iOverwriteMask & M4LOG_OVER_DEFAULT_TRACE ) != 0 )
				{
					ilevelInfo = (m4uint32_t) m_FlagsLevels[ auxIndex - 1];
				}
			}
			break;
		
		default:
			return M4_ERROR;
	}
	return M4_SUCCESS;

  //## end ClStaticModuleList::GetLevelInfo%-489667911.body
}

ClStaticModuleList& ClStaticModuleList::operator = (const ClStaticModuleList& )
{
  //## begin ClStaticModuleList::operator =%-1144838330.body preserve=yes
  	
	return *this;  
  //## end ClStaticModuleList::operator =%-1144838330.body
}

m4uint32_t ClStaticModuleList::GetModuleListSize ()
{
  //## begin ClStaticModuleList::GetModuleListSize%-328154895.body preserve=yes
				return 0;
  //## end ClStaticModuleList::GetModuleListSize%-328154895.body
}

m4return_t ClStaticModuleList::SetFlagsLevels (m4uint32_t* ai_FlagsLevels)
{
  //## begin ClStaticModuleList::SetFlagsLevels%-1436605533.body preserve=yes


	if (m_FlagsLevels != NULL) return M4_ERROR;
	
	m_FlagsLevels = ai_FlagsLevels;
	return M4_SUCCESS;


  //## end ClStaticModuleList::SetFlagsLevels%-1436605533.body
}

m4return_t ClStaticModuleList::AddModule (ClModuleInfo* ai_Module)
{
  //## begin ClStaticModuleList::AddModule%-663784900.body preserve=yes


	ClModuleInfo	*pModule;
	MODULEMAP::iterator	iter;

	if (!ai_Module)
		return M4_ERROR;
	
	if ( (iter = m_ModuleMap.find( ai_Module -> GetModuleName ().c_str() )) != m_ModuleMap.end() )
	{
		
		pModule = (* iter).second;

		//Sobrecargamos el operador += para que
		//las excepciones se añadan
		*pModule += *ai_Module;

		return M4_WARNING;
	}
	
	m_ModuleMap.insert( MODULEMAP::value_type( ai_Module -> GetModuleName ().c_str(), ai_Module  ) );
	
	return M4_SUCCESS;	


  //## end ClStaticModuleList::AddModule%-663784900.body
}

ClModuleInfo* ClStaticModuleList::GetModuleByName (m4pchar_t ai_ModuleName)
{
  //## begin ClStaticModuleList::GetModuleByName%-39832169.body preserve=yes



	MODULEMAP::iterator	iter;

	if ( (iter = m_ModuleMap.find( ai_ModuleName )) != m_ModuleMap.end() )
	{
		return (*iter).second;
	}

	return NULL;

	
/*
	m4uint32_t		iter = 0;
	ClModuleInfo	*pauxModule = NULL;

	while (iter<m_Size) {

		pauxModule = m_ModuleList[iter];

		if (pauxModule == NULL ) {
			
			return NULL;
		}

		if (!strcmp((pauxModule -> GetModuleName()), ai_ModuleName))
			return pauxModule;

		iter++;

	}

	return NULL;
*/


  //## end ClStaticModuleList::GetModuleByName%-39832169.body
}

// Additional Declarations
  //## begin ClStaticModuleList%377C883E0393.declarations preserve=yes
  //## end ClStaticModuleList%377C883E0393.declarations

// Class ClPatternsMap 



ClPatternsMap::ClPatternsMap ()
  //## begin ClPatternsMap::ClPatternsMap%-430534877.hasinit preserve=no
  //## end ClPatternsMap::ClPatternsMap%-430534877.hasinit
  //## begin ClPatternsMap::ClPatternsMap%-430534877.initialization preserve=yes
  //## end ClPatternsMap::ClPatternsMap%-430534877.initialization
{
  //## begin ClPatternsMap::ClPatternsMap%-430534877.body preserve=yes
  //## end ClPatternsMap::ClPatternsMap%-430534877.body
}


ClPatternsMap::~ClPatternsMap ()
{
  //## begin ClPatternsMap::~ClPatternsMap%-1564202931.body preserve=yes


	PATTERNSMAP::iterator	iter, iter1;
	ClOutputPattern	*pPattern;

	iter = m_patternsMap.begin();

	while ( iter !=  m_patternsMap.end() )
	{
		iter1 = iter;
		pPattern = (* iter1).second;
		if ( pPattern )
			delete pPattern;
		m_patternsMap.erase( iter1 );
		iter = m_patternsMap.begin();

	}

  //## end ClPatternsMap::~ClPatternsMap%-1564202931.body
}



//## Other Operations (implementation)
m4return_t ClPatternsMap::addOutputPattern (ClOutputPattern* ai_pattern)
{
  //## begin ClPatternsMap::addOutputPattern%1079849859.body preserve=yes

	
	
	ClOutputPattern	*pPattern;
//	M4ClString		key;
	PATTERNSMAP::iterator	iter;

	if (!ai_pattern) return M4_ERROR;
	
//	key = ai_pattern -> GetOutputPatternName ();

	if ( (iter = m_patternsMap.find( ai_pattern -> GetOutputPatternName ().c_str() )) != m_patternsMap.end() )
	{
		
		pPattern = (* iter).second;
		
		pPattern -> SetErrorLevel ( ai_pattern -> GetErrorLevel() ) ;
		pPattern -> SetWarningLevel ( ai_pattern -> GetWarningLevel() ) ;
		pPattern -> SetDebugLevel ( ai_pattern -> GetDebugLevel() ) ;
		pPattern -> SetTraceLevel ( ai_pattern -> GetTraceLevel() ) ;
	//	m_patternsMap.erase(iter);		
	//	(* iter).second = ai_pattern;
		return M4_WARNING;
	}
	
	m_patternsMap.insert( PATTERNSMAP::value_type( ai_pattern -> GetOutputPatternName ().c_str(), ai_pattern  ) );
	
	return M4_SUCCESS;	

  //## end ClPatternsMap::addOutputPattern%1079849859.body
}

ClOutputPattern* ClPatternsMap::getOutputPatternByName (const M4ClString& ai_patternName)
{
  //## begin ClPatternsMap::getOutputPatternByName%783728752.body preserve=yes


	PATTERNSMAP::iterator	iter;

	if ( (iter = m_patternsMap.find( ai_patternName.c_str() )) != m_patternsMap.end() )
	{
		return (*iter).second;
	}

	return NULL;

  //## end ClPatternsMap::getOutputPatternByName%783728752.body
}

void ClPatternsMap::Dump ()
{
  //## begin ClPatternsMap::Dump%2057842898.body preserve=yes


	PATTERNSMAP::iterator	iter, iter1;
	ClOutputPattern	*pPattern;

	iter = m_patternsMap.begin();

	while ( iter !=  m_patternsMap.end() )
	{
		pPattern = (* iter).second;
		printf("\n-> %s",pPattern -> GetOutputPatternName().c_str());
		iter++;
	}

  //## end ClPatternsMap::Dump%2057842898.body
}

// Additional Declarations
  //## begin ClPatternsMap%377C883F0142.declarations preserve=yes
  //## end ClPatternsMap%377C883F0142.declarations

// Class ClModuleInfo 







ClModuleInfo::ClModuleInfo ()
  //## begin ClModuleInfo::ClModuleInfo%-945366519.hasinit preserve=no
  //## end ClModuleInfo::ClModuleInfo%-945366519.hasinit
  //## begin ClModuleInfo::ClModuleInfo%-945366519.initialization preserve=yes
  //## end ClModuleInfo::ClModuleInfo%-945366519.initialization
{
  //## begin ClModuleInfo::ClModuleInfo%-945366519.body preserve=yes


	m_sModuleName = "No Name module";
	m_FinalCode = 0;
	m_ModuleState = M4_TRUE;

	m_pCodeExceptions = NULL;

  //## end ClModuleInfo::ClModuleInfo%-945366519.body
}


ClModuleInfo::~ClModuleInfo ()
{
  //## begin ClModuleInfo::~ClModuleInfo%-1381154582.body preserve=yes


	if (m_pCodeExceptions != NULL)
    {
		m_pCodeExceptions -> DeleteAllResources();
        delete m_pCodeExceptions;
    }

  //## end ClModuleInfo::~ClModuleInfo%-1381154582.body
}



//## Other Operations (implementation)
ClModuleInfo& ClModuleInfo::operator += (const ClModuleInfo& ai_ModuleInfo)
{
  //## begin ClModuleInfo::operator +=%-929270123.body preserve=yes

	//El nombre no lo igualamos, porque los mapas de modulos se
	//clasifican por él.
	m_InitCode = ai_ModuleInfo.m_InitCode;
	m_FinalCode = ai_ModuleInfo.m_FinalCode;
	m_ModuleState = ai_ModuleInfo.m_ModuleState;

	if (m_pCodeExceptions == NULL)
		m_pCodeExceptions = new ClStaticResourceList();
	(*m_pCodeExceptions)+=*(ai_ModuleInfo.m_pCodeExceptions);

	//Igualamos punteros, porque se corresponden a la dirección de
	//los elementos de una lista, donde se guardan los pattern.
	m_pattern = ai_ModuleInfo.m_pattern;

	return *this;

  //## end ClModuleInfo::operator +=%-929270123.body
}

ClStaticResourceList* ClModuleInfo::GetCodeExceptions ()
{
  //## begin ClModuleInfo::GetCodeExceptions%2017224066.body preserve=yes


	return m_pCodeExceptions;

  //## end ClModuleInfo::GetCodeExceptions%2017224066.body
}

ClOutputPattern* ClModuleInfo::GetOutputPattern ()
{
  //## begin ClModuleInfo::GetOutputPattern%-597407052.body preserve=yes

	return m_pattern;

  //## end ClModuleInfo::GetOutputPattern%-597407052.body
}

m4return_t ClModuleInfo::AddCodeException (m4int32_t ai_code, m4uint32_t ai_level)
{
  //## begin ClModuleInfo::AddCodeException%-1020459840.body preserve=yes


	if (m_pCodeExceptions == NULL) m_pCodeExceptions = new ClStaticResourceList();
	
	return m_pCodeExceptions -> AddResource( ai_code, ai_level );

  //## end ClModuleInfo::AddCodeException%-1020459840.body
}

m4uint32_t ClModuleInfo::FindCodeException (m4uint32_t ai_Code)
{
  //## begin ClModuleInfo::FindCodeException%929810155.body preserve=yes


	m4uint32_t	iret = 0;
	m4return_t	auxret = M4_ERROR;

	if (m_pCodeExceptions == NULL) return (m4uint32_t)-1;
	auxret = m_pCodeExceptions -> GetLevelInfo( ai_Code, iret );
	if (auxret == M4_ERROR ) iret = (m4uint32_t)-1;

	return iret;

  //## end ClModuleInfo::FindCodeException%929810155.body
}

m4bool_t ClModuleInfo::IsInModule (m4uint32_t ai_Code)
{
  //## begin ClModuleInfo::IsInModule%-1289820936.body preserve=yes

   return ((ai_Code >= m_InitCode)&&(ai_Code <= m_FinalCode)?M4_TRUE:M4_FALSE);

  //## end ClModuleInfo::IsInModule%-1289820936.body
}

ClModuleInfo& ClModuleInfo::operator = (const ClModuleInfo& ai_ModuleInfo)
{
  //## begin ClModuleInfo::operator =%-192709965.body preserve=yes

	//El nombre no lo igualamos, porque los mapas de modulos se
	//clasifican por él.
	m_InitCode = ai_ModuleInfo.m_InitCode;
	m_FinalCode = ai_ModuleInfo.m_FinalCode;
	m_ModuleState = ai_ModuleInfo.m_ModuleState;

	if (m_pCodeExceptions == NULL)
		m_pCodeExceptions = new ClStaticResourceList();
	else
		m_pCodeExceptions -> DeleteAllResources();
	(*m_pCodeExceptions)+=*(ai_ModuleInfo.m_pCodeExceptions);

	//Igualamos punteros, porque se corresponden a la dirección de
	//los elementos de una lista, donde se guardan los pattern.
	m_pattern = ai_ModuleInfo.m_pattern;

	return *this;

  //## end ClModuleInfo::operator =%-192709965.body
}

// Additional Declarations
  //## begin ClModuleInfo%377C883F0164.declarations preserve=yes
  //## end ClModuleInfo%377C883F0164.declarations

// Class ClDebugResource 


ClDebugResource::ClDebugResource ()
  //## begin ClDebugResource::ClDebugResource%806095081.hasinit preserve=no
  //## end ClDebugResource::ClDebugResource%806095081.hasinit
  //## begin ClDebugResource::ClDebugResource%806095081.initialization preserve=yes
  //## end ClDebugResource::ClDebugResource%806095081.initialization
{
  //## begin ClDebugResource::ClDebugResource%806095081.body preserve=yes

	m_ilevelInfo = 0;

  //## end ClDebugResource::ClDebugResource%806095081.body
}

ClDebugResource::ClDebugResource (m4uint32_t ilevelInfo)
  //## begin ClDebugResource::ClDebugResource%1744602673.hasinit preserve=no
  //## end ClDebugResource::ClDebugResource%1744602673.hasinit
  //## begin ClDebugResource::ClDebugResource%1744602673.initialization preserve=yes
  //## end ClDebugResource::ClDebugResource%1744602673.initialization
{
  //## begin ClDebugResource::ClDebugResource%1744602673.body preserve=yes

	m_ilevelInfo = ilevelInfo;

  //## end ClDebugResource::ClDebugResource%1744602673.body
}


ClDebugResource::~ClDebugResource ()
{
  //## begin ClDebugResource::~ClDebugResource%192280011.body preserve=yes
  //## end ClDebugResource::~ClDebugResource%192280011.body
}



//## Other Operations (implementation)
m4uint32_t ClDebugResource::GetLevelInfo ()
{
  //## begin ClDebugResource::GetLevelInfo%119451585.body preserve=yes

	return m_ilevelInfo;

  //## end ClDebugResource::GetLevelInfo%119451585.body
}

m4return_t ClDebugResource::SetLevelInfo (m4uint32_t ilevelInfo)
{
  //## begin ClDebugResource::SetLevelInfo%235718619.body preserve=yes

	m_ilevelInfo = ilevelInfo;
	return M4_SUCCESS;

  //## end ClDebugResource::SetLevelInfo%235718619.body
}

// Additional Declarations
  //## begin ClDebugResource%377C8840007F.declarations preserve=yes
  //## end ClDebugResource%377C8840007F.declarations

//## begin module%377C88560206.epilog preserve=yes
//## end module%377C88560206.epilog
