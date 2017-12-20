//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         m4zlib
// File:                m4zlib.h
// Project:        Interfaz C++ para Zlib		
// Author:         Meta Software M.S. , S.A
// Date:            22/04/97
// Language:        C++  
// Operating System: WINDOWS
//
//
// Definition:
//
//    Interfaz C++ de la librería pública zLib de compresión
//
//
//==============================================================================

#if !defined( _M4ZLIB_H )
#define _M4ZLIB_H

#define M4ZLIB_OK 0
#define M4ZLIB_ERROR  -1
#define M4ZLIB_INVALID_INPUT_FILE -7
#define M4ZLIB_ERROR_OPEN_OUTPUT_FILE -8
#define Z_USER_ABORT (-7)
// More Error in zlib.h

#define Z_BLOCK_SIZE 4096

#include <stdio.h>
#include "zlib.h"


class M4Zlib : public z_stream
{
    public :
        M4Zlib();
		~M4Zlib();
        int Compress(int level = 6 );
        int CompressFile( const char *input,
                      const char *output,
                      int level = 6 );
        int Decompress();
        int DecompressFile( const char *input,
                        const char *output );
        int SetBufferIn(const unsigned char *ai_data,long ai_size);
		const char* GetBufferOut()const;
		long GetBufferOutSize() const;
		void CleanBuffers();
		void Set_Abort_Flag( int i ){ m_AbortFlag = i; }
//
// Uso interno
//
    protected :
        int FlushOutput();
        virtual void status( char *message ){};
        int percent();
        int load_input();
        int flush_output();
   
        
//
// The remaining data members are used internally/
//
    protected :
        long length;
        int err;
		unsigned char *BufferIn;
		unsigned char *BufferOut;
		long BufferInSize;
		long BufferOutSize;
        virtual void progress( int percent ){};
        int m_AbortFlag;
        FILE *fin;
        FILE *fout;
        enum { input_length = 4096 };
        unsigned char input_buffer[ input_length ];
        enum { output_length = 4096 };
        unsigned char output_buffer[ output_length ];
};

#endif 

/* si queremos grabar el resultado a ficheros .gz, para
que los pueda descomprimir o nosotros podamos descomprimir
estos ficheros hay que tener en cuenta lo siguiente:
Cabecera:

  0x1F  -Magic number
  Ox8b	- "      "
  8		- Z_DEFLATED
  0		-flags
  0		- time
  0		- "
  0		- "
  0		- "
  0		- xflags
  OS_CODE	-0 =MS-DOS
			-6 =OS/2
			-0x0B=WIN32
			-0x03=UNIX
			-0x02=VAX
			-0x01=AMIGA
			-0x07=MAC OS
			-0x0A=PRIME

Al final del fichero hay 2 bytes que corresponden 
a la longitud en formato LSB (ver gzio.h)


*/
