//## begin module%3912A6BD0365.cm preserve=no
//## end module%3912A6BD0365.cm

//## begin module%3912A6BD0365.cp preserve=no
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
//## end module%3912A6BD0365.cp

//## Module: m4glbsea%3912A6BD0365; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\m4glbsea.cpp

//## begin module%3912A6BD0365.additionalIncludes preserve=no
//## end module%3912A6BD0365.additionalIncludes

//## begin module%3912A6BD0365.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                m4glbsea.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las funciones globales de búsqueda
//
//
//
//==============================================================================



#include "m4objglb.hpp"
#include <stdio.h>
//## end module%3912A6BD0365.includes

//## begin module%3912A6BD0365.declarations preserve=no
//## end module%3912A6BD0365.declarations

//## begin module%3912A6BD0365.additionalDeclarations preserve=yes
//## end module%3912A6BD0365.additionalDeclarations


//## begin module%3912A6BD0365.epilog preserve=yes


m4pvoid_t	M4VMBSearch
			(
				const void *key,
				const void *base,
				size_t num,
				size_t width,
				int (*compare)(const void *, const void *)
			)
{
        char *lo = (char *)base;
        char *hi = (char *)base + (num - 1) * width;
        char *mid;
        unsigned int half;
        int result;

        while (lo <= hi)
                if (half = num / 2)
                {
                        mid = lo + (num & 1 ? half : (half - 1)) * width;
                        if (!(result = (*compare)(key,mid)))
                                return(mid);
                        else if (result < 0)
                        {
                                hi = mid - width;
                                num = num & 1 ? half : half-1;
                        }
                        else    {
                                lo = mid + width;
                                num = half;
                        }
                }
                else if (num)
                        return((*compare)(key,lo) ? NULL : lo);
                else
                        break;

        return(NULL);
}




//## end module%3912A6BD0365.epilog
