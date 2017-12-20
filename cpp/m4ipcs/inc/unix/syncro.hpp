//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             semaphor.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:6/27/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#if !defined (_SYNCRO_HPP_)
#define _SYNCRO_HPP_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <pthread.h> 

#include "m4types.hpp"
#include "m4define.hpp"


#define M4_DEFAULT_FILE_PERMS 0666
#define nsems 1
#define u_short unsigned short
#define M4_INVALID_SEM_KEY -37
#define u_long unsigned long

typedef union {
	int val;
	struct semid_ds *buf;
	u_short *array;
} semun;
class ClSynObject
{
   public:
		
	ClSynObject ( m4pchar_t ai_szName = NULL ) { ; };					            virtual  ~ClSynObject ( void ){ ; }; 
	virtual m4return_t  Lock ( void )=0; 
	virtual m4return_t  Unlock ( void )=0 ; 
										}; 
class ClSemaphore :public ClSynObject{
public:
  enum {
     
   M4_OPEN   = 0
  };

  // = Initialization and termination methods.
  //ClSemaphore (void);
  ClSemaphore (m4int16_t  initial_value = 1,m4int16_t  max_value=1, m4pchar_t name=NULL);

  ~ClSemaphore (void);

   m4int16_t  Open ( m4int16_t  initial_value = 1,m4int16_t  max_value=1,m4pchar_t ai_name=NULL);
  // Open or create an array of SV_Semaphores.  We return 0 if all is
  // OK, else -1.


  // = Semaphore acquire and release methods.

   m4return_t  Lock (void) ;
  // Acquire the semaphore.

   m4return_t  Unlock (void) ;
  // Release the semaphore.

  m4bool_t IsSemCreate(void);

private:
  static const int BIGCOUNT_;
  static sembuf op_lock_[2];
  static sembuf op_endcreate_[2];
  static sembuf op_open_[1];
  static sembuf op_close_[3];
  static sembuf op_unlock_[1];

   int  close (void);
// Close an 4_SV_Semaphore. Unlike the <remove> method, this
// method is for a process to call before it exits, when it is done
// with the 4_SV_Semaphore. We "decrement" the counter of
// processes using the 4_SV_Semaphore, and if this was the last
// one, we can remove the 4_SV_Semaphore.


  // = Semaphore operation methods.
   int  op (int val, u_short n = 0, int flags = 0) const;
   int op (sembuf op_vec[], u_short n) const;

  // = Semaphore control methods.
   int control (int cmd, semun arg, u_short n = 0) const;
   int control (int cmd, int value = 0, u_short n = 0) const;

   int remove (void);
   int init ( key_t k = M4_INVALID_SEM_KEY, int i = -1);
   key_t name_2_key (m4pchar_t name);

   key_t key_;
   int internal_id_;
   int m_maxval;
   m4bool_t m_create;
};


class ClMutex : public ClSynObject
{  
   protected:
        pthread_mutex_t m_hMutexHandle;
	//ADD FJSM 990408 Para controlar que no se hace un lock,... antes de init
	m4bool_t	m_bInitialized; 
   
	public:  
		ClMutex( void );  
		ClMutex(m4bool_t);
		~ClMutex ( void );
        m4return_t  Init(void);
		m4return_t Lock(void);
		m4return_t Unlock(void);
        pthread_mutex_t *Handle()
		{
			//MOD FJSM 990408 Controlar llamadas sin inicializar Mutex
			M4_ASSERT(m_bInitialized == M4_TRUE);
			if (m_bInitialized == M4_TRUE)
				return &m_hMutexHandle;
			else
				return NULL;
		};
};  
  
 
  
class ClSemaphoreForWaiting   
{  
  
	private:  
  
		ClSemaphore *m_smpUsing;  
		ClSemaphore *m_smpSincro;  
		m4int16_t  m_iNumTotalForWaiting;  
		m4int16_t m_iEnded;  
  
	public:  
  
		ClSemaphoreForWaiting ( m4int16_t ai_iForWaiting );  
		ClSemaphoreForWaiting ( void );  
		~ClSemaphoreForWaiting ( void );  
		m4int16_t  SetNumSemaphoreForWaiting ( m4int16_t  ai_iForSemaphoreWaiting );  
		m4int16_t  Init( void  );  
		m4int16_t  Lock ();  
		m4int16_t  Unlock ();  
  
  
};  

#endif 


