//## begin module%3F5314260279.cm preserve=no
//## end module%3F5314260279.cm

//## begin module%3F5314260279.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%3F5314260279.cp

//## Module: clrollingfile%3F5314260279; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: C:\m4server\m4serverbase\inc\rollingfile.hpp

#ifndef clrollingfile_h
#define clrollingfile_h 1

//## begin module%3F5314260279.additionalIncludes preserve=no
//## end module%3F5314260279.additionalIncludes

//## begin module%3F5314260279.includes preserve=yes
#include <m4serverbase_dll.hpp>
#include "m4stl.hpp"
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>
//## end module%3F5314260279.includes

//## begin module%3F5314260279.declarations preserve=no
//## end module%3F5314260279.declarations

//## begin module%3F5314260279.additionalDeclarations preserve=yes
//## end module%3F5314260279.additionalDeclarations


//## begin ClRollingFile%3F53183A039D.preface preserve=yes
//## end ClRollingFile%3F53183A039D.preface

//## Class: ClRollingFile%3F53183A039D
//	This class will be in charge of printing messages to a given file. The user
//	can specify a number of maximum log files and the maximum size of each one.
//	This class will check that each file do not exceed the maximum size and it
//	will do a backup file if the file reaches the maximum size.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3F531D710234;ClMutex { -> }
//## Uses: <unnamed>%3F531D7303BD;M4Thread { -> }
//## Uses: <unnamed>%3F5342CD0313;ClPrecisionTicker { -> }
//## Uses: <unnamed>%3F5342CF018F;M4ClTimeStamp { -> }

class M4_DECL_M4SERVERBASE ClRollingFile 
{
  //## begin ClRollingFile%3F53183A039D.initialDeclarations preserve=yes
  //## end ClRollingFile%3F53183A039D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRollingFile%1062412773
      //	Default class constructor.
      ClRollingFile ();

    //## Destructor (specified)
      //## Operation: ~ClRollingFile%1062412775
      ~ClRollingFile ();


    //## Other Operations (specified)
      //## Operation: Init%1128499375
      //	/* Bug 0090231
      //		  La inicialización no puede hacerse en el constructor
      //	 */
      m4return_t Init (const m4string_t &ai_sFilePath, m4uint32_t ai_iMaxFileNum, m4uint32_t ai_iMaxFileSize);

      //## Operation: SetFile%1062412778
      //	Set log file path. It will also do a backup for all files that matches
      //	filename*.fileextension and for the current log file.
      m4return_t SetFile (const m4string_t &ai_sFilePath);

      //## Operation: SetHeader%1062412779
      //	Set file header and print it to file.
      m4return_t SetHeader (const m4string_t &ai_sHeader);

      //## Operation: SetMaxFileNum%1062412780
      //	Set maximum number of files.
      void SetMaxFileNum (m4uint32_t ai_iMaxFileNum);

      //## Operation: SetMaxFileSize%1062412781
      //	Set maximum size for each file.
      void SetMaxFileSize (m4uint32_t ai_iMaxFileSize);

      //## Operation: SetFilePath%1063623438
      //	Set file path.
      m4return_t SetFilePath (const m4string_t &ai_sFileName);

      //## Operation: GetMaxFileNum%1063354420
      //	Returns maximum number of files.
      m4uint32_t GetMaxFileNum ();

      //## Operation: GetMaxFileSize%1063354421
      //	Returns maximum file size.
      m4uint32_t GetMaxFileSize ();

      //## Operation: GetFilePath%1063623439
      //	Returns file path.
      m4string_t GetFilePath ();

      //## Operation: PrintLn%1062412783
      //	Print message to file.
      m4return_t PrintLn (const m4string_t &ai_sMessage);

      //## Operation: PrintStr%1128499373
      //	Print string to file. No Backup and no end of line is spedified.
      //
      //	Use example: you can print strings using PrintStr(), but when you finish a
      //	message (or line), you have to call PrintLn("") with no message when you
      //	want to end the line. This metod will also check if you have reached maximum
      //	file size and it will do a backup. It will also print a carriage return.
      m4return_t PrintStr (const m4string_t &ai_sStr);

      //## Operation: EnableTickCount%1062412787
      //	Enable to print the tick count
      void EnableTickCount ();

      //## Operation: EnableThreadId%1062412788
      //	Enable to print the thread id.
      void EnableThreadId ();

	  FILE* GetFile( void ) const ;

    // Additional Public Declarations
      //## begin ClRollingFile%3F53183A039D.public preserve=yes
      //## end ClRollingFile%3F53183A039D.public

  protected:
    // Additional Protected Declarations
      //## begin ClRollingFile%3F53183A039D.protected preserve=yes
      //## end ClRollingFile%3F53183A039D.protected

  private:

    //## Other Operations (specified)
      //## Operation: PrintHeader%1062412782
      //	Print file header.
      m4return_t PrintHeader ();

      //## Operation: PrintInfo%1128499374
      //	Generic message print. Parameters:
      //
      //	  ai_bDoBackUp: specifies if you have to make a file   backup (only if
      //	maximum size is reached).
      //	  ai_bEndOfLine: specifies if you want to print and end of   line after the
      //	message.
      m4return_t PrintInfo (const m4string_t &ai_sInfo, m4bool_t ai_bDoBackUp, m4bool_t ai_bEndOfLine);

      //## Operation: DoBackup%1062412784
      //	Do a backup of all files. It will look for all files that matches file_name
      //	+ index + file_extension, where index < max number of files.
      m4return_t DoBackup ();

      //## Operation: GetFileSize%1062412785
      //	Get file size.
      m4uint32_t GetFileSize ();

      //## Operation: SplitFileName%1062412786
      //	Splits a file path into file extension and the file path + file name without
      //	extension.
      void SplitFileName (m4string_t &aio_FilePathWithoutExtension, m4string_t &aio_FileExtension);

    // Data Members for Class Attributes

      //## Attribute: m_iMaxFileNum%3F531D8C002A
      //	Maximum number of files.
      //## begin ClRollingFile::m_iMaxFileNum%3F531D8C002A.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iMaxFileNum;
      //## end ClRollingFile::m_iMaxFileNum%3F531D8C002A.attr

      //## Attribute: m_iMaxFileSize%3F531D9F01E0
      //	Maximum size for each file.
      //## begin ClRollingFile::m_iMaxFileSize%3F531D9F01E0.attr preserve=no  private: m4int32_t {UA} 
      m4int32_t m_iMaxFileSize;
      //## end ClRollingFile::m_iMaxFileSize%3F531D9F01E0.attr

      //## Attribute: m_sFilePath%3F531DB2009D
      //	File path.
      //## begin ClRollingFile::m_sFilePath%3F531DB2009D.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_sFilePath;
      //## end ClRollingFile::m_sFilePath%3F531DB2009D.attr

      //## Attribute: m_sHeader%3F531DC4039C
      //	File header.
      //## begin ClRollingFile::m_sHeader%3F531DC4039C.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_sHeader;
      //## end ClRollingFile::m_sHeader%3F531DC4039C.attr

      //## Attribute: m_oMutex%3F531DD5036E
      //## begin ClRollingFile::m_oMutex%3F531DD5036E.attr preserve=no  private: ClMutex {UA} 
      ClMutex m_oMutex;
      //## end ClRollingFile::m_oMutex%3F531DD5036E.attr

      //## Attribute: m_bTickCount%3F531DE202FF
      //## begin ClRollingFile::m_bTickCount%3F531DE202FF.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bTickCount;
      //## end ClRollingFile::m_bTickCount%3F531DE202FF.attr

      //## Attribute: m_bThreadId%3F531DEE0202
      //## begin ClRollingFile::m_bThreadId%3F531DEE0202.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bThreadId;
      //## end ClRollingFile::m_bThreadId%3F531DEE0202.attr

	  FILE* m_pfFile ;

    // Additional Private Declarations
      //## begin ClRollingFile%3F53183A039D.private preserve=yes
      //## end ClRollingFile%3F53183A039D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRollingFile%3F53183A039D.implementation preserve=yes
      //## end ClRollingFile%3F53183A039D.implementation

};

//## begin ClRollingFile%3F53183A039D.postscript preserve=yes
//## end ClRollingFile%3F53183A039D.postscript

// Class ClRollingFile 

//## begin module%3F5314260279.epilog preserve=yes
//## end module%3F5314260279.epilog


#endif
