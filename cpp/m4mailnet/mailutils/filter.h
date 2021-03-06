/* GNU Mailutils -- a suite of utilities for electronic mail
   Copyright (C) 1999, 2000, 2001 Free Software Foundation, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA  */

#ifndef _MAILUTILS_FILTER_H
#define _MAILUTILS_FILTER_H

#include <mailutils/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Type.  */
#define MU_FILTER_DECODE 0
#define MU_FILTER_ENCODE 1

/* Direction.  */
#define MU_FILTER_READ  MU_STREAM_READ
#define MU_FILTER_WRITE MU_STREAM_WRITE
#define MU_FILTER_RDWR  MU_STREAM_RDWR

/*
 * In Sun CC 5.5, we have an error when are calling to _get_filter
 *"../mailutils/filter.cpp", line 187: 
 *   Error: Formal argument _filter of type extern "C" int(*)(_filter*)* is being passed int(*)(_filter*)*.
 */

typedef int (*FILTER_CB)(filter_t);

struct _filter_record
{
  const char *name;
  int  (*_filter)     __PMT ((filter_t));
  void *data;

  /* Stub function return the fields.  */
  int (*_is_filter)  __PMT ((filter_record_t, const char *));
  int (*_get_filter) __PMT ((filter_record_t, FILTER_CB *));
};


extern int filter_create   __P ((stream_t *, stream_t, const char*, int, int));
extern int filter_get_list __P ((list_t *));

/* List of defaults.  */
extern filter_record_t rfc822_filter;
extern filter_record_t qp_filter; /* quoted-printable.  */
extern filter_record_t base64_filter;
extern filter_record_t binary_filter;
extern filter_record_t bit8_filter;
extern filter_record_t bit7_filter;

#ifdef __cplusplus
}
#endif

#endif /* _MAILUTILS_FILTER_H */
