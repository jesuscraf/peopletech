#ifndef _CLGETTSAP_HPP
#define _CLGETTSAP_HPP

#include "m4date.hpp"
#include "m4thread.hpp"
#include "m4win.hpp"
#include "tsap.hpp"

#ifdef _UNIX
#include <unistd.h>
//#define m4sleep( sec ) 		( sleep( sec ) )

// To use select
#include <sys/time.h>
#include <sys/types.h>

#endif

class ClGetTSAP : public M4Thread 
{

		ClControlWriteTSAP	*m_poTSAP  ;
		int		m_iPort;

	public:

		ClGetTSAP ( m4int16_t iPort )
		{	
			m_poTSAP = NULL ;

			m_iPort = iPort;
				
		};

//	DEL	EMN	981030		~ClGetTSAP ( ){;}

		void Run(void)
		{			
#ifdef _UNIX
			 struct timeval timeout ={1, 0};
#endif			

			m_poTSAP = new ClControlWriteTSAP() ;

#ifndef _UNIX
			m4sleep (1) ;
#endif

#ifdef _UNIX
			(void) select(2, NULL, NULL, NULL, &timeout);
#endif			

			m_poTSAP->Init("localhost", m_iPort) ;
		}

		ClControlWriteTSAP * GetTSAP ( ) 
		{
			return m_poTSAP ;
		};
};

#endif
