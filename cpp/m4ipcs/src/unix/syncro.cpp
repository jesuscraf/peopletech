//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo> 
// File:             sincro.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:6/28/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "syncro.hpp"
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>


// initial value of process 
const int  ClSemaphore::BIGCOUNT_ = 10000;




//-------------------------------------------------------------------

//*********************ClSemaphore***********************************

//-------------------------------------------------------------------

//-------------------------------------------------------------------
//******************op_lock_[2]**************************************
//-------------------------------------------------------------------

// Define the Semaphore operation arrays for the semop() calls. 
sembuf ClSemaphore::op_lock_[2] = 
{
  {0, 0, 0},			// Wait for [0] (lock) to equal 0 
  {0, 1, IPC_NOWAIT},		// then increment [0] to 1 - this locks it. 
                                // UNDO to release the lock if processes exit 
				// before explicitly unlocking. 
};


//------------------------------------------------------------------------
//*********************op_endcreate_**************************************
//------------------------------------------------------------------------

sembuf ClSemaphore::op_endcreate_[2] = 
{
  {1, -1, IPC_NOWAIT},		// Decrement [1] (proc counter) with undo on 
                                // exit, UNDO to adjust proc counter if 
				// process exits before explicitly calling close() 
  {0, -1, IPC_NOWAIT},		// the decrement [0] (lock) back to 0 
};


//---------------------------------------------------------------------------
//**********************op_open*******************************************
//--------------------------------------------------------------------------

sembuf ClSemaphore::op_open_[1] = 
{
  {1, -1, IPC_NOWAIT},		// Decrement [1] (proc counter) with undo on 
                                // exit. 
};

//--------------------------------------------------------------------------
//*****************************op_close_**********************************
//-------------------------------------------------------------------------

sembuf ClSemaphore::op_close_[3] = 
{
  {0, 0, 0},			// Wait for [0] (lock) to equal 0 
  {0, 1, IPC_NOWAIT},		// then increment [0] to 1 - this lock it 
  {1, 1, IPC_NOWAIT},		// then increment [1] (proc counter) 
};
//------------------------------------------------------------------------
//***************************op_unlock************************************
//------------------------------------------------------------------------

sembuf ClSemaphore::op_unlock_[1] = 
{
  {0, -1, IPC_NOWAIT},		// Decrement [0] (lock) back to 0 
};


//-------------------------------------------------------------------------
//*****************************Open***************************************
//-----------------------------------------------------------------------

// Open or create an array of SV_Semaphores.  We return 0 if all is OK, else -1. 
m4int16_t ClSemaphore::Open (m4int16_t  initial_value,m4int16_t   max_value,m4pchar_t ai_name) 
{
  key_t key;

  //En solaris IPC_PRIVATE es 0.
  //En aix IPC_PRIVATE es -1.
  //Al comparar key con -1 NO es un error pq en Aix es factible.

  if (ai_name == NULL) {    
    key = IPC_PRIVATE;
  }  
  else {
	key = name_2_key( ai_name );
    if (key == -1) {
	    return M4_ERROR;
	}
  }

  int  create = IPC_CREAT;
 
  this->m_maxval = max_value;
  this->key_ = key;
  
  if (create == IPC_CREAT)
    {
      int result;

      do
	{
	  this->internal_id_ = semget 
	    (this->key_, 2 + nsems, M4_DEFAULT_FILE_PERMS | IPC_CREAT);

	  if (this->internal_id_ == -1)
	    return M4_ERROR; // permission problem or tables full 


	  if (errno==EINVAL)
	    return M4_ERROR;
      
	  result = semop (this->internal_id_, 
				  &ClSemaphore::op_lock_[0], 
				  2);
	}
      while (result == -1 && (errno == EINVAL || errno == EIDRM));

      if (result ==-1)
	return M4_ERROR;

      int  semval = control (GETVAL, 0, 1);
  
      if (semval == -1)
	
	return this->init ();
      
      else if (semval == 0)
	{
	  // We should initialize by doing a SETALL, but that would
	  // clear the adjust value that we set when we locked the
	  // ACE_SV_Semaphore above. Instead we do system calls to
	  // initialize [1], as well as all the nsems SV_Semaphores.
	        
	  if (control (SETVAL, ClSemaphore::BIGCOUNT_, 1) == -1)
	    
	    return M4_ERROR;
	  
	  else
	    
	    for (int i = 0; i < nsems; i++){
	      if (this->control (SETVAL, initial_value, i+2) == -1)
				return M4_ERROR;
	
	    }
	       
	}	      
  
      // Decrement the process counter and then release the lock. 
      return semop (this->internal_id_, 
			    &op_endcreate_[0], 
			    2);
    }
  else 
    {
      this->internal_id_ = semget (this->key_, 2 + nsems, 0);
      if (this->internal_id_ == -1)
	return M4_ERROR;			// doesn't exist or tables full 
  
      // Decrement the process counter. We don't need a lock to do this. 
      if (semop (this->internal_id_, &op_open_[0], 1) < 0)
	
	return this->init ();
      
      return M4_SUCCESS;
    }
}


// Close a M4_Semaphore.  Unlike the remove above, this function
// is for a process to call before it exits, when it is done with the
// Semaphore.  We "decrement" the counter of processes using
// the Semaphore, and if this was the last one, we can remove
// the Semaphore.

//--------------------------------------------------------------------------
//************************Close*******************************************
//------------------------------------------------------------------------

 
int ClSemaphore::close (void)
{
  int  semval;
  
  if (this->key_ <= (key_t) - 1 || this->internal_id_ == -1)
    return M4_ERROR;
  
  // The following semop() first gets a lock on the Semaphore,
  // then increments [1] - the process number.
  
  if (semop (this->internal_id_, 
		     &op_close_[0], 3) == -1)
    return M4_ERROR;
  
  // Now that we have a lock, read the value of the process counter to
  // see if this is the last reference to the Semaphore. There
  // is a race condition here - see the comments in create ().
  
  if ((semval = control (GETVAL, 0, 1)) == -1)
    return M4_ERROR;
  
  if (semval > ClSemaphore::BIGCOUNT_)
    return M4_ERROR;
  else if (semval == ClSemaphore::BIGCOUNT_)
    return this->remove ();
  else
    {

      int result = semop (this->internal_id_, 
				  &op_unlock_[0], 1);
      this->init ();
      return result;
    }
}

//-----------------------------------------------------------------------------------
//******************************Constructor del Semaphore*****************************
//-----------------------------------------------------------------------------------

ClSemaphore::ClSemaphore (m4int16_t  initial_value,m4int16_t  max_value ,m4pchar_t name)
{

  m_create=M4_TRUE;

  if (initial_value<0)

		m_create=M4_FALSE;

  if (max_value<=0)

		m_create=M4_FALSE;

  if (this->Open (initial_value, max_value,name) == -1)
    {
      m_create=M4_FALSE;
    }
   
  
}

//---------------------------------------------------------------------------
//**************************Destructor del Semaforo**************************
//-------------------------------------------------------------------------

ClSemaphore::~ClSemaphore (void)
{
  if (this->internal_id_ >= 0)
    this->close ();
}

//-----------------------------------------------------------------------
//************************Constructor del Semaforo**********************
//-----------------------------------------------------------------------
//ClSemaphore::ClSemaphore (void)
//{
// this->init ();
//}


int  ClSemaphore::remove (void)  {
  int result = this->control( IPC_RMID );
  this->init ();
  return result;
}

int  ClSemaphore::init ( key_t k, int  i)  {
	this->key_ = k;
	this->internal_id_ = i;
	return 0;
}

//----------------------------------------------------------------
//********************Funciones para controlar el Semaforo********
//----------------------------------------------------------------


int  ClSemaphore::control( int  cmd, semun arg, u_short n) const {
	return (internal_id_ == -1) ? -1 : semctl( internal_id_, n, cmd, arg );
}


int  ClSemaphore::control( int  cmd, int  value, u_short n) const {
	if (this->internal_id_ == -1 )
		return -1;
    else {
		semun semctl_arg;
		semctl_arg.val = value ;
		return semctl( internal_id_,n,cmd,semctl_arg );
	}
}


//------------------------------------------------------------------
//**************Funciones para contrlar el semaforo****************
//-----------------------------------------------------------------


int  ClSemaphore::op( int  val, u_short n, int  flags ) const {
	sembuf op_op;
	op_op.sem_num = n+2;
	op_op.sem_flg = flags;

	if (internal_id_ == -1)
		return M4_ERROR;
	else if ((op_op.sem_op = val) == 0 )
		return M4_ERROR;
	else
	{
		if ( semop( internal_id_, &op_op, 1 )== -1)
		{
			return M4_ERROR;
		}
		
		return M4_SUCCESS;
	}
}


int  ClSemaphore::op( sembuf op_vec[], u_short n) const {
	if  (internal_id_ == 1 ) 
	{
		return M4_ERROR;
	}

	if (semop( internal_id_, op_vec, n+2 )==-1)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
//-----------------------------------------------------------------
//******************** IsSemCreate**********************************
//------------------------------------------------------------------

m4bool_t ClSemaphore::IsSemCreate(void)
{
	return m_create;
}

//-------------------------------------------------------------------
//************************Lock***************************************
//------------------------------------------------------------------


m4return_t   ClSemaphore::Lock (void)  {
	 
		return op( -1,0,0 );
}

//---------------------------------------------------------------------
//***********************Unlock***************************************
//--------------------------------------------------------------------


m4return_t  ClSemaphore::Unlock (void) {
	int  res, r_op;
	res = semop( internal_id_, &op_lock_[0],2 );
	if (res == -1 )
		return M4_ERROR;
	int  semval = control( GETVAL,0,2 );
	if ( semval >= m_maxval )
		r_op = 0;
	else
		r_op = op( 1,0,0 );
	res = semop( internal_id_, &op_unlock_[0],1 );
	if (res == -1 )
		return M4_ERROR;
	return M4_SUCCESS ;
}

//-----------------------------------------------------------------
//***************Funciones de conversion**************************
//-----------------------------------------------------------------

m4int32_t  power(m4int16_t  exp , m4int16_t base)
{
  m4int32_t powerval;
  if (exp==0)
	  return 1;
	else{
          powerval=long(pow(double(base),double(exp)));
          return powerval;
	}
}



key_t ClSemaphore::name_2_key ( m4pchar_t name )     
{
 
       m4int16_t leng;
       m4char_t letter;
       m4int16_t val;
       m4int32_t sum=0;
       m4int16_t ret;
       m4int16_t i;
       m4int16_t cont=0;
									   
   	leng = strlen(name);
	
	if (leng<=5)
	    
	  for (i=leng-1;i>=0;i--){
    	  	letter=toupper(name[i]);
            val=int(letter);		   						                                if (val>=48 && val<=57)							                                       val=(val-48)+29;							                                if (val>=65 && val<=90)
		           val-=65;					
            sum +=power(cont,38)*val;			
       	    cont++;																			   }
	  else 
		 sum=-1;

	 return (key_t) sum;

      
}


// ---------------------------------------------------------------------------  

// ***************	Mutex  ***********************************************  

// ---------------------------------------------------------------------------  

  
ClMutex::ClMutex(void)
{
	m_bInitialized = M4_FALSE;
}
  
ClMutex::ClMutex( m4bool_t ao_bInit)
{		
	m_bInitialized = M4_FALSE;
	if (ao_bInit == M4_TRUE)
		Init();
}
  
ClMutex::~ClMutex( void )    
{
	if (m_bInitialized==M4_TRUE)	
		pthread_mutex_destroy(&m_hMutexHandle);
}  

m4return_t ClMutex::Init(void)
{
	//MOD FJSM 990408 Si ya esta inicializado no lo volvemos a inicializar
	if (m_bInitialized==M4_TRUE)
		return M4_SUCCESS;
	if (pthread_mutex_init(&m_hMutexHandle, NULL) == 0) {
		m_bInitialized = M4_TRUE;
		return M4_SUCCESS;
	}
	
	return M4_ERROR;
}

m4return_t ClMutex::Lock(void)
{
	m4return_t ret = M4_SUCCESS;
	if (m_bInitialized==M4_TRUE)
	{
		if (pthread_mutex_lock(&m_hMutexHandle) < 0 )
			ret = M4_ERROR;
	}
	else
		ret = M4_ERROR;	

	return ret;
}


m4return_t ClMutex::Unlock(void)
{
	m4return_t ret = M4_SUCCESS;
	if (m_bInitialized==M4_TRUE)
	{
		if (pthread_mutex_unlock(&m_hMutexHandle) < 0)
			ret = M4_ERROR;
	}
	else
		ret = M4_ERROR;	

	return ret;
}
		
// ----------------------------------------------------------------------------  
// ***************	Constructores de ClSemaporeForWaiting		***************  
// ---------------------------------------------------------------------------  
  
  
ClSemaphoreForWaiting::ClSemaphoreForWaiting ( m4int16_t  ai_iForSemaphoreWaiting )  
{  
	m_smpUsing = NULL;  
  
	m_smpSincro = NULL;  
  
	m_iNumTotalForWaiting = ai_iForSemaphoreWaiting ;  
}  
  
  
  
ClSemaphoreForWaiting::ClSemaphoreForWaiting ( void )  
{  
	m_smpUsing = NULL;  
	m_smpSincro = NULL;  
}  
  
// ----------------------------------------------------------------------------  
// ***************	Destructor de ClSemaphoreForWaiting		***************  
// ----------------------------------------------------------------------------  
ClSemaphoreForWaiting::~ClSemaphoreForWaiting ( void )  
{  
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
}  
  
  
// ----------------------------------------------------------------------------  
// ***************	Init										***************  
// ----------------------------------------------------------------------------  
m4int16_t  ClSemaphoreForWaiting::Init( void  )  
{  
		m_smpUsing = new ClSemaphore ( 0 , 1 );  
  
		//m_smpUsing -> Lock ();  
		  
		m_smpSincro = new ClSemaphore ( 0 , 1 );  
  
		//m_smpSincro -> Lock ();  
		  
		m_iEnded = 0 ;  
		  
		  
  
		return M4_SUCCESS;  
}  
  
  
  
// ----------------------------------------------------------------------------  
// ***************	SetNumSemaphoreForWaiting					***************  
// ----------------------------------------------------------------------------  
m4int16_t ClSemaphoreForWaiting::SetNumSemaphoreForWaiting ( m4int16_t ai_iForSemaphoreWaiting )  
{  
	m_iNumTotalForWaiting = ai_iForSemaphoreWaiting ;  
  
	return M4_SUCCESS;  
};  
  
// ----------------------------------------------------------------------------  
// ***************	Lock											***************  
// ----------------------------------------------------------------------------  
m4int16_t ClSemaphoreForWaiting::Lock ()  
{  
  
	m_smpUsing -> Unlock ();  
	  
	m_smpSincro -> Lock ();  
  
	return M4_SUCCESS;  
  
}  
  
  
// ----------------------------------------------------------------------------  
// ***************	Unlock										***************  
// ----------------------------------------------------------------------------  
m4int16_t  ClSemaphoreForWaiting::Unlock ()  
{  
	m_smpUsing -> Lock ();  
		  
	m_iEnded ++;  
  
	if ( m_iEnded == m_iNumTotalForWaiting )  
	{  
		m_smpSincro -> Unlock ( );  
	}  
  
	m_smpUsing -> Unlock ();  
  
	return M4_SUCCESS;  
};

