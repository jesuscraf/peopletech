

#ifndef _SYNCROGENERAL_HPP_
#define _SYNCROGENERAL_HPP_

#ifdef _UNIX
  #include "unix/syncro.hpp"
#elif defined (WINDOWS) || defined (_WINDOWS)
  #include "windows\syncro.hpp"
#else
# error hay que definir una plataforma
#endif

#endif



