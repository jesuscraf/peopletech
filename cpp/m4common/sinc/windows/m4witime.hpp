
#ifndef __M4WITIME_HPP__
#define __M4WITIME_HPP__

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
       m_lTimeMileSeconds = 0 ;
   }
   
   void Start(void)
   {
       m_lTimeMileSeconds=GetTickCount();
   }
   
   long GetDifference(void) const
   {
       return GetTickCount()-m_lTimeMileSeconds;
   }
   
   friend  long operator - (const ClM4Clock &r2, const ClM4Clock &r1)
   {
      return( r2.m_lTimeMileSeconds - r1.m_lTimeMileSeconds );
   }

private:
   long m_lTimeMileSeconds;   
};

#endif

