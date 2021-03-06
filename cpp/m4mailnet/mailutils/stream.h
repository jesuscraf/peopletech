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

#ifndef _MAILUTILS_STREAM_H
# define _MAILUTILS_STREAM_H

#include <stdio.h>

#include <mailutils/types.h>

typedef int (*_open) __PMT ((stream_t));
typedef void (*_destroy) __PMT ((stream_t));
typedef int (*_close) __PMT ((stream_t));
typedef int (*_get_fd) __PMT ((stream_t, int *));
typedef int (*_read) __PMT ((stream_t, char *, size_t, off_t, size_t *));
typedef int (*_readline) __PMT ((stream_t, char *, size_t, off_t, size_t *));
typedef int (*_size) __PMT ((stream_t, off_t *));
typedef int (*_truncate) __PMT ((stream_t, off_t));
typedef int (*_write) __PMT ((stream_t, const char *, size_t, off_t, size_t *));
typedef int (*_flush) __PMT ((stream_t));
typedef int (*fp) (stream_t, char **);


#ifdef __cplusplus
extern "C" { /*}*/
#endif

#define MU_STREAM_READ	      0x00000001
#define MU_STREAM_WRITE	      0x00000002
#define MU_STREAM_RDWR        0x00000004
#define MU_STREAM_APPEND      0x00000008
#define MU_STREAM_CREAT	      0x00000010
#define MU_STREAM_NONBLOCK    0x00000020
/* Stream will be destroyed on stream_destroy without checking the owner. */
#define MU_STREAM_NO_CHECK    0x00000040
#define MU_STREAM_SEEKABLE    0x00000080
#define MU_STREAM_NO_CLOSE    0x00000100
#define MU_STREAM_ALLOW_LINKS 0x00000200
  
/* Functions useful to users of the pre-defined stream types. */

extern int file_stream_create    __P ((stream_t *stream, const char* filename, int flags));
extern int tcp_stream_create     __P ((stream_t *stream, const char* host, int port, int flags));
extern int mapfile_stream_create __P ((stream_t *stream, const char* filename, int flags));
extern int memory_stream_create  __P ((stream_t *stream, const char* filename, int flags));
extern int encoder_stream_create __P ((stream_t *stream, stream_t iostream, const char *encoding));
extern int decoder_stream_create __P ((stream_t *stream, stream_t iostream, const char *encoding));
extern int stdio_stream_create   __P ((stream_t *stream, FILE* stdio, int flags));
extern int prog_stream_create    __P ((stream_t *stream, char *progname, int flags));
  
extern void stream_destroy    __P ((stream_t *, void *owner));

extern int stream_open        __P ((stream_t));
extern int stream_close       __P ((stream_t));
extern int stream_is_seekable __P ((stream_t));
extern int stream_get_fd      __P ((stream_t , int *));
extern int stream_read        __P ((stream_t, char *, size_t, off_t, size_t *));
extern int stream_readline    __P ((stream_t, char *, size_t, off_t, size_t *));
extern int stream_size        __P ((stream_t, off_t *));
extern int stream_truncate    __P ((stream_t, off_t));
extern int stream_write       __P ((stream_t, const char *, size_t, off_t, size_t *));
extern int stream_setbufsiz   __P ((stream_t stream, size_t size));
extern int stream_flush       __P ((stream_t));


/* Functions useful to implementors of new stream types. */

extern int stream_create       __P ((stream_t *stream, int flags, void* owner));

extern void* stream_get_owner  __P ((stream_t));
extern void stream_set_owner   __P ((stream_t, void* owner));

extern int stream_get_flags    __P ((stream_t, int *pflags));
extern int stream_set_flags    __P ((stream_t, int flags));

extern int stream_get_property __P ((stream_t, property_t *));
extern int stream_set_property __P ((stream_t, property_t, void *));

#define MU_STREAM_STATE_OPEN  1
#define MU_STREAM_STATE_READ  2
#define MU_STREAM_STATE_WRITE 4
#define MU_STREAM_STATE_CLOSE 8

extern int stream_get_state    __P ((stream_t, int *pstate));

extern int stream_set_destroy  __P ((stream_t, _destroy, void *owner));

extern int stream_set_open     __P ((stream_t, _open, void *owner));

extern int stream_set_close    __P ((stream_t, _close, void *owner));

extern int stream_set_fd       __P ((stream_t, _get_fd, void *owner));

extern int stream_set_read     __P ((stream_t, _read, void *owner));

extern int stream_set_readline __P ((stream_t, _readline, void *owner));

extern int stream_set_size     __P ((stream_t, _size, void *owner));

extern int stream_set_truncate __P ((stream_t, _truncate, void *owner));

extern int stream_set_write    __P ((stream_t, _write, void *owner));

extern int stream_set_flush    __P ((stream_t, _flush, void *owner));

extern int stream_set_strerror __P ((stream_t stream, fp, void *owner));
  
extern int stream_sequential_read __P((stream_t stream, char *buf, size_t size, size_t *nbytes));
  
extern int stream_sequential_readline __P((stream_t stream, char *buf, size_t size, size_t *nbytes));
  
extern int stream_sequential_write __P((stream_t stream, char *buf, size_t size));

extern int stream_seek __P((stream_t stream, off_t off, int whence));
  
extern int stream_strerror __P((stream_t stream, char **p));
  
#ifdef __cplusplus
}
#endif


#ifdef _WINDOWS
	int strcasecmp(const char *s1, const char *s2);
#endif


#endif /* _MAILUTILS_STREAM_H */

