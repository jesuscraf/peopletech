
#ifndef __M4SOTIME_HPP__
#define __M4SOTIME_HPP__

#include <time.h>
#include <sys/types.h>



static long ElapsedMilliSeconds( const struct timeval &t2, const struct timeval &t1 )
{
    return( ( t2.tv_sec - t1.tv_sec ) * 1000 + ( t2.tv_usec - t1.tv_usec ) / 1000 ) ;
}


class ClM4Clock{
public:
   ClM4Clock()
   {
      Start();
   }
   
   ~ClM4Clock()
   {
   }
   
   void Reset(void)
   {
       m_stTimeval.tv_sec = 0 ;
       m_stTimeval.tv_usec = 0 ;
   }
   
   void Start(void)
   {
        gettimeofday( &m_stTimeval, NULL ) ;
   }
   
   long GetDifference(void) const
   {

        struct timeval stTimeval ;

        gettimeofday( &stTimeval, NULL ) ;
        return ( ElapsedMilliSeconds( stTimeval, m_stTimeval ) );
   }
   
   friend  long operator - (const ClM4Clock &r2, const ClM4Clock &r1)
   {
      return( ElapsedMilliSeconds( r2.m_stTimeval, r1.m_stTimeval ) );
   }

private:
   struct timeval    m_stTimeval ;
};

#endif

