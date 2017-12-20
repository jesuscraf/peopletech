//## begin module%34E2EAB90183.cm preserve=no
//## end module%34E2EAB90183.cm

//## begin module%34E2EAB90183.cp preserve=no
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
//## end module%34E2EAB90183.cp

//## Module: ExeForEcho%34E2EAB90183; Package body
//## Subsystem: M4ExeFactory::src%37D781A303AA
//## Source file: d:\eduardoma\M4ExeFactory\version\src\exeforecho.cpp

//## begin module%34E2EAB90183.additionalIncludes preserve=no
//## end module%34E2EAB90183.additionalIncludes

//## begin module%34E2EAB90183.includes preserve=yes
#include <m4srvres.hpp>
//	Rose 98 Body Include Dependences
#include <m4log.hpp>
#include <datastorages.hpp>

//## end module%34E2EAB90183.includes

// ExeForEcho
#include <exeforecho.hpp>
// Executive
#include <executive.hpp>
//## begin module%34E2EAB90183.declarations preserve=no
//## end module%34E2EAB90183.declarations

//## begin module%34E2EAB90183.additionalDeclarations preserve=yes
//## end module%34E2EAB90183.additionalDeclarations


// Class ClExeForEcho 

ClExeForEcho::ClExeForEcho (ClEngine *ai_pEngine)
  //## begin ClExeForEcho::ClExeForEcho%887109594.hasinit preserve=no
  //## end ClExeForEcho::ClExeForEcho%887109594.hasinit
  //## begin ClExeForEcho::ClExeForEcho%887109594.initialization preserve=yes
  :ClExecutive(ai_pEngine)
  //## end ClExeForEcho::ClExeForEcho%887109594.initialization
{
  //## begin ClExeForEcho::ClExeForEcho%887109594.body preserve=yes
  //## end ClExeForEcho::ClExeForEcho%887109594.body
}


ClExeForEcho::~ClExeForEcho ()
{
  //## begin ClExeForEcho::~ClExeForEcho%887109595.body preserve=yes
  //## end ClExeForEcho::~ClExeForEcho%887109595.body
}



//## Other Operations (implementation)
m4return_t  ClExeForEcho::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExeForEcho::StartUp%887109596.body preserve=yes
	return M4_SUCCESS;
  //## end ClExeForEcho::StartUp%887109596.body
}

m4return_t  ClExeForEcho::ShutDown ()
{
  //## begin ClExeForEcho::ShutDown%887109597.body preserve=yes
	return M4_SUCCESS;
  //## end ClExeForEcho::ShutDown%887109597.body
}

m4return_t ClExeForEcho::Reset ()
{
  //## begin ClExeForEcho::Reset%887109598.body preserve=yes
	return M4_SUCCESS;
  //## end ClExeForEcho::Reset%887109598.body
}

m4return_t ClExeForEcho::Execute (ClImage &aio_pImage)
{
  //## begin ClExeForEcho::Execute%887109599.body preserve=yes
	ClEngineArgs			*poEngineArgs;
	M4DataStorage			*poInputDataStorage,*poOutputDataStorage;
	ClDataUnitInterface     *poDataUnit;
	m4uint32_t				iTotalSize = 0;
	m4char_t				szTotalSize [20] ;
	m4string_t				msg ;

	aio_pImage.GetArgs(poEngineArgs);

	if (poEngineArgs==NULL)
	{
		return M4_ERROR;
	}

	if (poEngineArgs->GetInputData(poInputDataStorage)!= M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	if (poEngineArgs->GetOutputData(poOutputDataStorage)!= M4_SUCCESS)
	{
		return M4_ERROR;
	}

//	SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "EXE4ECHO: Executing echo") ;

	poInputDataStorage->InitializeIteration();//Esto es para empezar a leer 
												   //del data storage de entrada
			m4uint32_t size=0;
			m4int8_t *buffer;
			m4pchar_t buffersal;
			m4return_t flags=M4_SUCCESS;

		while (poInputDataStorage->GetNext(poDataUnit)==M4_SUCCESS && flags==M4_SUCCESS)//Mientras no devuelva la 
															//función un M4_ERROR hay data storages que leer

		{
//	ADD	EMN	980921
			m4uint16_t		uiType = poDataUnit->GetType();
			switch(uiType)
			{
			case M4_DATA_UNIT:

				ClDataUnit		*poMemDataUnit;

				poMemDataUnit = (ClDataUnit *)poDataUnit;
				if ((size=poMemDataUnit->GetSize())==0)//Obtenemos la talla de la siguiente unidad
				{
					flags=M4_ERROR;
				}
				else
				{
				   if ((buffer=poMemDataUnit->GetBuffer())==NULL)//Obtenemos la unit de entrada
				   {
					   flags=M4_ERROR;
				   }
				   else
				   {

					 if (poOutputDataStorage->GetNewDataUnit(poMemDataUnit)==M4_ERROR)//Obtenemos la unidada de salida
					 {
						flags=M4_ERROR;
					 }
					else
					{
					  poMemDataUnit->SetSize(size);//Seleccionamos la talla del buffer de salida
					
						if ((buffersal=poMemDataUnit->GetBuffer())==NULL)
						{
							flags=M4_ERROR;
						}
						else
						{
							iTotalSize += size ;
						   if (memcpy(buffersal,buffer,size)==NULL)//Con esto copiamos la unidad de datos
											  //de entrada en la de salida
						   {
							   flags=M4_ERROR;
						   }
						}
					}
				   }
				}

				sprintf ( szTotalSize , "%d" , iTotalSize ) ;
				msg = "\tTotal Size: " ;
				msg += szTotalSize ;
				AddTraceMessage (msg) ;

				break;
			case M4_FILE_UNIT:
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;

				poFileUnit = (ClFileUnit *)poDataUnit;
				pszFilename = poFileUnit->GetFileName();

				if (poOutputDataStorage->GetNewFileUnit(poFileUnit)==M4_ERROR)//Obtenemos la unidada de salida
				{
					flags=M4_ERROR;
				}
				else
				{
					msg = "File unit: " ;
					msg += pszFilename ;
					AddTraceMessage ( msg) ;
					poFileUnit->SetFile(pszFilename);
				}
				break;

			default:
					flags=M4_ERROR;
			}
				
		}

	return M4_SUCCESS;
  //## end ClExeForEcho::Execute%887109599.body
}

m4return_t ClExeForEcho::DumpLevel (const m4int16_t ai_iLevel)
{
  //## begin ClExeForEcho::DumpLevel%921077500.body preserve=yes
	return M4_SUCCESS;
  //## end ClExeForEcho::DumpLevel%921077500.body
}

// Additional Declarations
  //## begin ClExeForEcho%34DF52F30339.declarations preserve=yes
  //## end ClExeForEcho%34DF52F30339.declarations

//## begin module%34E2EAB90183.epilog preserve=yes
//## end module%34E2EAB90183.epilog
