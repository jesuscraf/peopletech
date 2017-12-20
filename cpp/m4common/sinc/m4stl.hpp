//==============================================================================  
//  
// (c) Copyright  1991-1997 Meta Software M.S., S.A  
// All rights reserved.  
//  
// Module:           <modulo>  
// File:             m4stl.hpp  
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


#ifndef _M4STL_HPP_
#define _M4STL_HPP_

#include "m4types.hpp"

#ifdef _WINDOWS
	#define M4_USE_NATIVE_STL		1
#else
	#define M4_USE_NATIVE_STL		0
#endif


#if M4_USE_NATIVE_STL == 1

#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <queue>
#include <set>
#include <ostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <strstream>
#include <vector>
using namespace std;

// Final de cadena de un string
#define M4_STR_NPOS					string::npos
// Modo de apertura de fichero no_create
#define M4_IOS_OPEN_MODE_NO_CREATE	ios::_Nocreate

#else

#include <ospace/std/algorithm>
#include <ospace/std/deque>
#include <ospace/std/fstream> 
#include <ospace/std/functional>
#include <ospace/std/iomanip>
#include <ospace/std/iostream>
#include <ospace/std/istream>
#include <ospace/std/list>
#include <ospace/std/map>
#include <ospace/std/memory>
#include <ospace/std/ostream>
#include <ospace/std/sstream>
#include <ospace/std/queue>
#include <ospace/std/set>
#include <ospace/std/stack>
#include <ospace/std/stdexcept>
#include <ospace/std/string>
#include <ospace/std/vector>

// Final de cadena de un string
#define M4_STR_NPOS					os_npos
// Modo de apertura de fichero no_create
#define M4_IOS_OPEN_MODE_NO_CREATE	ios::nocreate	

#endif

/// las clases de STL se pueden poner debajo sin probelmas.
/// no incluyais <iostream.h>, ni pragma, ni using ya esta incluida

#endif  // _M4STL_HPP_

