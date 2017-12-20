
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:              m4condition.cpp
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
//
//=======================================================================


#include "m4condition.hpp"


ClCondition::ClCondition()
{
	m_create = M4_FALSE;
	m_terminate = M4_FALSE;
}

m4return_t	ClCondition::Init()
{
	if (pthread_mutex_init(&m_hHandleMutexCond, NULL) == -1)
		return M4_ERROR;

	if (pthread_cond_init(&m_hHandleCondVar , NULL) == -1)
	  return M4_ERROR;

	m_create = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t	ClCondition::Reset()
{
	// Vacia
	return M4_SUCCESS;
}

m4int16_t ClCondition::Wait(m4int32_t millisecs)
{
	m4uint16_t ret;

	if (m_create == M4_TRUE)
	{

	   pthread_mutex_lock(&m_hHandleMutexCond);
	
	   if (millisecs == -1)
	   {
		  pthread_cond_wait(&m_hHandleCondVar, 
							&m_hHandleMutexCond);

		   ret = M4_SUCCESS;
		
	   }
	   else
	   {
			struct timespec to;
				
			to.tv_sec = time(NULL) + 
						(int)(millisecs / 1000);
			to.tv_nsec = millisecs % 1000;

			switch (pthread_cond_timedwait(&m_hHandleCondVar, 
											&m_hHandleMutexCond, 
											&to))
			{
				case 0:
					ret = M4_COND_SIGNALED;
				break;

				case ETIME:
				case ETIMEDOUT:
						ret = M4_COND_TIMEOUT;
				break;
		                
				default:
						ret = M4_COND_ERROR;
				break;
		  }
	   }
	

		pthread_mutex_unlock(&m_hHandleMutexCond);

		return ret;
	}

	return M4_COND_ERROR;

}


m4return_t	ClCondition::Signal(void)
{

	if (m_create == M4_TRUE)
	{
	   if (pthread_cond_broadcast(&m_hHandleCondVar) != 0)
		  return M4_ERROR;

	   return M4_SUCCESS;
    }

	return M4_ERROR;
}

m4return_t ClCondition::Close()
{
	if (m_create == M4_TRUE)
	{
	   if (pthread_cond_destroy(&m_hHandleCondVar) == -1)
		 return M4_ERROR;

	   if (pthread_mutex_destroy(&m_hHandleMutexCond) == -1)
		   return M4_ERROR;

	   m_terminate = M4_TRUE;

	   return M4_SUCCESS;
	}

	return M4_ERROR;

}

ClCondition::~ClCondition()
{
	if (m_terminate==M4_FALSE)
	{
		pthread_cond_destroy(&m_hHandleCondVar);
		pthread_mutex_destroy(&m_hHandleMutexCond);
	}       
}
