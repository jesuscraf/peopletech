//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         m4zlib
// File:                m4zlib.cpp
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


#if defined( _WINDOWS )
	#include <windows.h>
#endif
#include <memory.h>
#include "m4zlib.hpp"



//
// The constructor initializes a couple of members
// of the z_stream class.  See the Zlib documentation
// for details on what those members do
//
M4Zlib::M4Zlib()
{
    zalloc = 0;  //z_stream member
    zfree = 0;   //z_stream member
    opaque = 0;  //z_stream member
//
// I initialize thse members just for tidiness.
//
    fin = 0;
    fout = 0;
//
// I initialize thse members just for tidiness.
//
	BufferIn=NULL;
	BufferOut=new unsigned char[2];
	BufferInSize=0;
	BufferOutSize=2;//Z_BLOCK_SIZE;
}

M4Zlib::~M4Zlib()
{
	CleanBuffers();
}

//
// Compress comprime el BufferIn y lo guarda en BufferOut
int M4Zlib::Compress(int level )
{
    err = Z_OK;
    avail_in = BufferInSize;
    avail_out = BufferOutSize;
	next_in=BufferIn;
    next_out = BufferOut;


    length = BufferInSize;

    deflateInit( this, level );
    
	for ( ; ; ) {
        err = deflate( this, Z_NO_FLUSH );
        FlushOutput();
        if ( err != Z_OK )
            break;
        
    }
    for ( ; ; ) {
        err = deflate( this, Z_FINISH );
        if ( !FlushOutput() )
            break;
        if ( err != Z_OK )
            break;
    }
    
    deflateEnd( this );
	if ( err != Z_OK && err != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        err = Z_OK;
    }
    return err;
}

//
// decompress has to do most of the same chores as compress().
// The only major difference it has is the absence of the level
// parameter.  The level isn't needed when decompressing data
// using the deflate algorithm.
//
int M4Zlib::Decompress(void)
{
    err = Z_OK;
    
	avail_in = BufferInSize;
    avail_out = BufferOutSize;
	next_in=BufferIn;
    next_out = BufferOut;
    length = BufferInSize;
	
	inflateInit( this );

    for ( ; ; ) {
        err = inflate( this, Z_NO_FLUSH );
        FlushOutput();
        if ( err != Z_OK )
            break;
     
    }
    for ( ; ; ) {
        err = inflate( this, Z_FINISH );
        if ( !FlushOutput() )
            break;
        if ( err != Z_OK )
            break;
    }
    inflateEnd( this );
    if ( err != Z_OK && err != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        err = Z_OK;
    }
    return err;
}

//
//  Every time Zlib filsl the output buffer with data,
//  this function gets called.  Its job is to write
//  that data out to the output file, then update
//  the z_stream member avail_out to indicate that more
//  space is now available.  I churn the Windows message
//  loop to ensure that the process can be aborted by a
//  button press or other Windows event.
//
int M4Zlib::FlushOutput()
{
    long tamAnterior=BufferOutSize;
	if (avail_out==0){
		unsigned char *temp=new unsigned char[BufferOutSize+Z_BLOCK_SIZE];
		if (temp==NULL){
			err=Z_ERRNO;
			return 0;
		}
		//copiamos a temp lo que habia en BufferOut
		memcpy(temp,BufferOut,tamAnterior);
		//nos cargamos el viejo
		delete []BufferOut;
		//ponemos en nuevo, y el tamaño
		BufferOut=temp;
		BufferOutSize+=Z_BLOCK_SIZE;
		//actualizamos next_out y avail_out
		next_out=BufferOut+tamAnterior;
		avail_out=Z_BLOCK_SIZE;
	}
	return tamAnterior;
	
}

int M4Zlib::SetBufferIn(const unsigned char *ai_data,long ai_size){
	if (BufferIn!=NULL) delete [] BufferIn;
	BufferIn=new unsigned char [ai_size];
	if (BufferIn==NULL) return 0;
	BufferInSize=ai_size;
	memcpy (BufferIn,ai_data,ai_size);
	return ai_size;
}

const char* M4Zlib::GetBufferOut() const
{
	return (const char *)BufferOut;
}

long M4Zlib::GetBufferOutSize(void) const
{
	return total_out;	
}

void M4Zlib::CleanBuffers()
{
	if (BufferIn!=NULL) delete [] BufferIn;
	if (BufferOut!=NULL) delete [] BufferOut;
	BufferInSize=BufferOutSize=0;
	BufferIn=BufferOut=NULL;
}

// From compress de ZlibEngine
// compress() is the public function used to compress
// a single file.  It has to take care of opening the
// input and output files and setting up the buffers for
// Zlib.  It then calls deflate() repeatedly until all
// input and output processing has been done, and finally
// closes the files and cleans up the Zlib structures.
//
int M4Zlib::CompressFile( const char *input,const char *output,int level)
{
   err = Z_OK;
    avail_in = 0;
    avail_out = output_length;
    next_out = output_buffer;
    m_AbortFlag = 0;

    if(NULL==(fin  = fopen( input, "rb" )))
	{
		return M4ZLIB_INVALID_INPUT_FILE;
	}
	if(NULL==(fout = fopen( output, "wb" )))
	{
		fclose(fin);
		return M4ZLIB_ERROR_OPEN_OUTPUT_FILE;
	}
    length = 0; //filelength( fileno( fin ) );
    deflateInit( this, level );
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        if ( !load_input() )
            break;
        err = deflate( this, Z_NO_FLUSH );
        flush_output();
        if ( err != Z_OK )
            break;
        progress( percent() );
    }
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        err = deflate( this, Z_FINISH );
        if ( !flush_output() )
            break;
        if ( err != Z_OK )
            break;
    }
    progress( percent() );
    deflateEnd( this );
    if ( m_AbortFlag )
        status( "User Abort" );
    else if ( err != Z_OK && err != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        err = Z_OK;
    }
    fclose( fin );
    fclose( fout );
    fin = 0;
    fout = 0;
    if ( m_AbortFlag )
        return Z_USER_ABORT;
    else
        return err;
}

//
// decompress has to do most of the same chores as compress().
// The only major difference it has is the absence of the level
// parameter.  The level isn't needed when decompressing data
// using the deflate algorithm.
//
int M4Zlib::DecompressFile( const char *input, const char *output )
{
    err = Z_OK;
    avail_in = 0;
    avail_out = output_length;
    next_out = output_buffer;
    m_AbortFlag = 0;

    if(NULL==(fin  = fopen( input, "rb" )))
	{
		return M4ZLIB_INVALID_INPUT_FILE;
	}

	if(NULL==(fout = fopen( output, "wb" )))
	{
		fclose(fin);
		return M4ZLIB_ERROR_OPEN_OUTPUT_FILE;
	}
    length = 0;//filelength( fileno( fin ) );
    inflateInit( this );
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        if ( !load_input() )
            break;
        err = inflate( this, Z_NO_FLUSH );
        flush_output();
        if ( err != Z_OK )
            break;
        progress( percent() );
    }
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        err = inflate( this, Z_FINISH );
        if ( !flush_output() )
            break;
        if ( err != Z_OK )
            break;
    }
    progress( percent() );
    inflateEnd( this );
    if ( m_AbortFlag )
        status( "User Abort" );
    else if ( err != Z_OK && err != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        err = Z_OK;
    }
    if ( fin )
        fclose( fin );
    fin = 0;
    if ( fout )
        fclose( fout );
    fout = 0;
    if ( m_AbortFlag )
        return Z_USER_ABORT;
    else
        return err;
}

//
//  This function is called so as to provide the progress()
//  virtual function with a reasonable figure to indicate
//  how much processing has been done.  Note that the length
//  member is initialized when the input file is opened.
//
int M4Zlib::percent()
{
    if ( length == 0 )
        return 100;
    else if ( length > 10000000L )
        return ( total_in / ( length / 100 ) );
    else
        return ( total_in * 100 / length );
}

//
//  Every time Zlib consumes all of the data in the
//  input buffer, this function gets called to reload.
//  The avail_in member is part of z_stream, and is
//  used to keep track of how much input is available.
//  I churn the Windows message loop to ensure that
//  the process can be aborted by a button press or
//  other Windows event.
//
int M4Zlib::load_input()
{
#if defined( _WINDOWS )
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
#endif
    if ( avail_in == 0 ) {
        next_in = input_buffer;
        avail_in = (unsigned int) fread( input_buffer, 1, input_length, fin );
    }
    return avail_in;
}

//
//  Every time Zlib filsl the output buffer with data,
//  this function gets called.  Its job is to write
//  that data out to the output file, then update
//  the z_stream member avail_out to indicate that more
//  space is now available.  I churn the Windows message
//  loop to ensure that the process can be aborted by a
//  button press or other Windows event.
//
int M4Zlib::flush_output()
{
#if defined( _WINDOWS )
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
#endif
    unsigned int count = output_length - avail_out;
    if ( count ) {
        if ( fwrite( output_buffer, 1, count, fout ) != count ) {
            err = Z_ERRNO;
            return 0;
        }
        next_out = output_buffer;
        avail_out = output_length;
    }
    return count;
}


