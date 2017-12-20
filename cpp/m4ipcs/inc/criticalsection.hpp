
#ifndef _CRITICALSECTIONGENERAL_HPP_
#define _CRITICALSECTIONGENERAL_HPP_

#ifdef _UNIX
  //class ClMutex;
  #include "syncro.hpp"
  typedef	ClMutex ClCriticalSection;
#elif defined (WINDOWS) || defined (_WINDOWS)
  #include "windows\criticalsection.hpp"
#else
  error hay que definir una plataforma
#endif

#endif	//_CRITICALSECTIONGENERAL_HPP_


