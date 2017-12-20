//## begin module%3742D023004C.cm preserve=no
//## end module%3742D023004C.cm

//## begin module%3742D023004C.cp preserve=no
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
//## end module%3742D023004C.cp

//## Module: syncro%3742D023004C; Package body
//## Subsystem: M4Ipcs::src::win32%3742D0230024
//	d:\fuentesservidor\m4ipcs\version\src\win32
//## Source file: D:\eduardoma\m4ipcs\src\win32\syncro.cpp

//## begin module%3742D023004C.additionalIncludes preserve=no
//## end module%3742D023004C.additionalIncludes

//## begin module%3742D023004C.includes preserve=yes
#include <stdio.h>
//## end module%3742D023004C.includes

// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
//## begin module%3742D023004C.declarations preserve=no
//## end module%3742D023004C.declarations

//## begin module%3742D023004C.additionalDeclarations preserve=yes
//## end module%3742D023004C.additionalDeclarations


// Class ClMutex 




ClMutex::ClMutex (void )
  //## begin ClMutex::ClMutex%-1395052867.hasinit preserve=no
  //## end ClMutex::ClMutex%-1395052867.hasinit
  //## begin ClMutex::ClMutex%-1395052867.initialization preserve=yes
  //## end ClMutex::ClMutex%-1395052867.initialization
{
  //## begin ClMutex::ClMutex%-1395052867.body preserve=yes

		m_hMutexHandle = NULL ;

  //## end ClMutex::ClMutex%-1395052867.body
}

ClMutex::ClMutex (m4bool_t ao_bInit)
  //## begin ClMutex::ClMutex%909182102.hasinit preserve=no
  //## end ClMutex::ClMutex%909182102.hasinit
  //## begin ClMutex::ClMutex%909182102.initialization preserve=yes
  //## end ClMutex::ClMutex%909182102.initialization
{
  //## begin ClMutex::ClMutex%909182102.body preserve=yes

		m_hMutexHandle = NULL ;
		if (ao_bInit == M4_TRUE)
			Init();

  //## end ClMutex::ClMutex%909182102.body
}


ClMutex::~ClMutex (void )
{
  //## begin ClMutex::~ClMutex%-429500371.body preserve=yes
	if (m_hMutexHandle != NULL)
	{
		CloseHandle(m_hMutexHandle);
	}

  //## end ClMutex::~ClMutex%-429500371.body
}



//## Other Operations (implementation)
m4return_t ClMutex::Lock (void )
{
  //## begin ClMutex::Lock%-1740955238.body preserve=yes

	if ((m_hMutexHandle == NULL) || (WaitForSingleObject(m_hMutexHandle, INFINITE) == WAIT_FAILED))
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClMutex::Lock%-1740955238.body
}

m4return_t ClMutex::Unlock (void )
{
  //## begin ClMutex::Unlock%718283932.body preserve=yes

	if ((m_hMutexHandle == NULL) || (ReleaseMutex(m_hMutexHandle)==0))
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClMutex::Unlock%718283932.body
}

m4return_t ClMutex::Init (void )
{
  //## begin ClMutex::Init%1902890713.body preserve=yes
	//MOD FJSM 990408 Si ya esta inicializado no lo volvemos a inicializar
	if (m_hMutexHandle != NULL)
//		if (!CloseHandle(m_hMutexHandle))
//			return M4_FALSE;
		return M4_SUCCESS;

	SECURITY_ATTRIBUTES	atributos;
	atributos.nLength = sizeof(SECURITY_ATTRIBUTES);
	atributos.lpSecurityDescriptor = NULL;
	atributos.bInheritHandle = true;

	m_hMutexHandle = CreateMutex(&atributos, false, NULL);
	if ( m_hMutexHandle == NULL)
		return M4_ERROR;
	
	return M4_SUCCESS;
  //## end ClMutex::Init%1902890713.body
}

// Additional Declarations
  //## begin ClMutex%3742D01F0263.declarations preserve=yes
  //## end ClMutex%3742D01F0263.declarations

// Class ClSemaphore 






ClSemaphore::ClSemaphore (m4int16_t ai_lInitialCount, m4int16_t ai_lMaxCount, m4pchar_t ai_pstrName)
  //## begin ClSemaphore::ClSemaphore%-1086119639.hasinit preserve=no
  //## end ClSemaphore::ClSemaphore%-1086119639.hasinit
  //## begin ClSemaphore::ClSemaphore%-1086119639.initialization preserve=yes
  //## end ClSemaphore::ClSemaphore%-1086119639.initialization
{
  //## begin ClSemaphore::ClSemaphore%-1086119639.body preserve=yes

	
	//  ------------------------------------------------------------------------------------
	//  Si el nombre es distinto de nulo puede que ya esté creado con lo cual lo intento
	//  abrir. En caso que ya esté creado dará un error
	//  ------------------------------------------------------------------------------------
	//if ( ai_pstrName != NULL )
	//{
		m_create=M4_TRUE;
		
		m_hHandle=HANDLE_NULL;//inicializamos el handle pore si tenía un valor previo
		
		if (ai_lInitialCount<0)//si el semaforo se inicializa a un numero
							   //negativo no se puede crear y poenemos el flag a false					
			m_create=M4_FALSE;

		if (ai_lMaxCount <= 0)

			m_create=M4_FALSE;
		

		 m4int16_t ret=Open (  ai_lInitialCount ,  ai_lMaxCount ,  ai_pstrName );

		 if (ret==M4_ERROR)//si no puede crearse bien devuelve false en m_create

			m_create=M4_FALSE;
	//}
	
	

	//m_hHandle = hHandleSemaphore ;

  //## end ClSemaphore::ClSemaphore%-1086119639.body
}


ClSemaphore::~ClSemaphore (void )
{
  //## begin ClSemaphore::~ClSemaphore%-771045020.body preserve=yes
	if ( m_hHandle != HANDLE_NULL )
	{
		CloseHandle ( m_hHandle );

		
		
	}
	
  //## end ClSemaphore::~ClSemaphore%-771045020.body
}



//## Other Operations (implementation)
m4return_t ClSemaphore::Lock (void )
{
  //## begin ClSemaphore::Lock%1224834127.body preserve=yes
    if (WaitForSingleObject ( m_hHandle , INFINITE )==WAIT_FAILED)
	{
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
  //## end ClSemaphore::Lock%1224834127.body
}

m4return_t ClSemaphore::Unlock (void )
{
  //## begin ClSemaphore::Unlock%-641722682.body preserve=yes


		if (ReleaseSemaphore ( m_hHandle , 1 , NULL)==0)
		{
			return M4_ERROR;
		}

		return M4_SUCCESS;
  //## end ClSemaphore::Unlock%-641722682.body
}

m4int16_t ClSemaphore::Open (m4int16_t ai_lInitialCount, m4int16_t ai_lMaxCount, m4pchar_t ai_strName)
{
  //## begin ClSemaphore::Open%-1129495857.body preserve=yes
	if ( m_hHandle != NULL )
	{
		CloseHandle ( m_hHandle );

		m_hHandle = NULL ;
	}
	
	m_hHandle = OpenSemaphore ( SEMAPHORE_ALL_ACCESS , TRUE , ai_strName );

	if (m_hHandle==NULL)
	
		m_hHandle = CreateSemaphore (  NULL , ai_lInitialCount , ai_lMaxCount , ai_strName );

	if ( m_hHandle == NULL )
	{
			m4char_t szBuffer [ 255 ];

		if ( ai_strName != NULL )
		{
			sprintf ( szBuffer , "Semphore Name = %s" , ai_strName );
		}
		else
		{
			sprintf ( szBuffer , "Semaphore Name is NULL" );
		}


	    return M4_ERROR;

	}

	return M4_SUCCESS;

  //## end ClSemaphore::Open%-1129495857.body
}

m4bool_t ClSemaphore::IsSemCreate (void )
{
  //## begin ClSemaphore::IsSemCreate%-645605075.body preserve=yes

	return m_create;
  //## end ClSemaphore::IsSemCreate%-645605075.body
}

// Additional Declarations
  //## begin ClSemaphore%3742D01F027D.declarations preserve=yes
  //## end ClSemaphore%3742D01F027D.declarations

// Class ClSemaphoreForWaiting 





ClSemaphoreForWaiting::ClSemaphoreForWaiting (m4int16_t ai_iForSemaphoreWaiting)
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.hasinit preserve=no
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.hasinit
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.initialization preserve=yes
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.initialization
{
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.body preserve=yes
	m_smpUsing = NULL;

	m_smpSincro = NULL;

	m_iNumTotalForWaiting = ai_iForSemaphoreWaiting ;
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%1423602053.body
}

ClSemaphoreForWaiting::ClSemaphoreForWaiting (void )
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.hasinit preserve=no
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.hasinit
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.initialization preserve=yes
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.initialization
{
  //## begin ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.body preserve=yes
	m_smpUsing = NULL;
	m_smpSincro = NULL;
  //## end ClSemaphoreForWaiting::ClSemaphoreForWaiting%-1520535565.body
}


ClSemaphoreForWaiting::~ClSemaphoreForWaiting (void )
{
  //## begin ClSemaphoreForWaiting::~ClSemaphoreForWaiting%827792698.body preserve=yes
	if ( m_smpUsing != NULL )
	{
		delete m_smpUsing;
	}
	if ( m_smpSincro != NULL )
	{
		delete m_smpSincro;
	}

	m_iEnded = 0;

	m_iNumTotalForWaiting = 0 ;
  //## end ClSemaphoreForWaiting::~ClSemaphoreForWaiting%827792698.body
}



//## Other Operations (implementation)
m4int16_t ClSemaphoreForWaiting::Lock ()
{
  //## begin ClSemaphoreForWaiting::Lock%-435247507.body preserve=yes

	m_smpUsing -> Unlock ();
	
	m_smpSincro -> Lock ();

	return M4_SUCCESS;

  //## end ClSemaphoreForWaiting::Lock%-435247507.body
}

m4int16_t ClSemaphoreForWaiting::Unlock ()
{
  //## begin ClSemaphoreForWaiting::Unlock%-371131849.body preserve=yes
	m_smpUsing -> Lock ();
		
	m_iEnded ++;

	if ( m_iEnded == m_iNumTotalForWaiting )
	{
		m_smpSincro -> Unlock ( );
	}

	m_smpUsing -> Unlock ();

	return M4_SUCCESS;
  //## end ClSemaphoreForWaiting::Unlock%-371131849.body
}

m4int16_t ClSemaphoreForWaiting::Init (void )
{
  //## begin ClSemaphoreForWaiting::Init%586533056.body preserve=yes
		m_smpUsing = new ClSemaphore ( 0 , 1 );

		//m_smpUsing -> Lock ();
		
		m_smpSincro = new ClSemaphore ( 0 , 1 );

		//m_smpSincro -> Lock ();
		
		m_iEnded = 0 ;
		
		

		return M4_SUCCESS;
  //## end ClSemaphoreForWaiting::Init%586533056.body
}

m4int16_t ClSemaphoreForWaiting::SetNumSemaphoreForWaiting (m4int16_t ai_iForSemaphoreWaiting)
{
  //## begin ClSemaphoreForWaiting::SetNumSemaphoreForWaiting%-332891486.body preserve=yes
	m_iNumTotalForWaiting = ai_iForSemaphoreWaiting ;

	return M4_SUCCESS;
  //## end ClSemaphoreForWaiting::SetNumSemaphoreForWaiting%-332891486.body
}

// Additional Declarations
  //## begin ClSemaphoreForWaiting%3742D022031B.declarations preserve=yes
  //## end ClSemaphoreForWaiting%3742D022031B.declarations

// Class ClSemBlock 


ClSemBlock::ClSemBlock (ClSemaphore* a_poSem)
  //## begin ClSemBlock::ClSemBlock%-826340450.hasinit preserve=no
  //## end ClSemBlock::ClSemBlock%-826340450.hasinit
  //## begin ClSemBlock::ClSemBlock%-826340450.initialization preserve=yes
:	m_poSem(a_poSem)
  //## end ClSemBlock::ClSemBlock%-826340450.initialization
{
  //## begin ClSemBlock::ClSemBlock%-826340450.body preserve=yes
	m_poSem->Lock();
  //## end ClSemBlock::ClSemBlock%-826340450.body
}


ClSemBlock::~ClSemBlock ()
{
  //## begin ClSemBlock::~ClSemBlock%528873236.body preserve=yes
	m_poSem->Unlock();
  //## end ClSemBlock::~ClSemBlock%528873236.body
}


// Additional Declarations
  //## begin ClSemBlock%3742D022034E.declarations preserve=yes
  //## end ClSemBlock%3742D022034E.declarations

// Class ClMutBlock2 


ClMutBlock2::ClMutBlock2 (ClMutex* a_poMut)
  //## begin ClMutBlock2::ClMutBlock2%1908518068.hasinit preserve=no
  //## end ClMutBlock2::ClMutBlock2%1908518068.hasinit
  //## begin ClMutBlock2::ClMutBlock2%1908518068.initialization preserve=yes
  //## end ClMutBlock2::ClMutBlock2%1908518068.initialization
{
  //## begin ClMutBlock2::ClMutBlock2%1908518068.body preserve=yes
	m_poMut = a_poMut;

	/* Comprobamos que nos pasan un mutex. Es util por ejemplo en LDB ya que el comportamiento
	en cliente y server es diferente al no necesitar mutex en cliente*/
	if (m_poMut)  //ADD FJSM 23/03/2000
	{ 
		m_poMut->Lock();
	}
  //## end ClMutBlock2::ClMutBlock2%1908518068.body
}


ClMutBlock2::~ClMutBlock2 ()
{
  //## begin ClMutBlock2::~ClMutBlock2%1253030407.body preserve=yes
	if (m_poMut) //ADD FJSM 23/03/2000
	{
		m_poMut->Unlock();
	}
  //## end ClMutBlock2::~ClMutBlock2%1253030407.body
}


// Additional Declarations
  //## begin ClMutBlock2%3742D022035D.declarations preserve=yes
  //## end ClMutBlock2%3742D022035D.declarations

//## begin module%3742D023004C.epilog preserve=yes
//## end module%3742D023004C.epilog
