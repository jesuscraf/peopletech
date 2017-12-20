//## begin module%3911AA2503C4.cm preserve=no
//## end module%3911AA2503C4.cm

//## begin module%3911AA2503C4.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%3911AA2503C4.cp

//## Module: clfileiod%3911AA2503C4; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\clfileiod.hpp

#ifndef clfileiod_h
#define clfileiod_h 1

//## begin module%3911AA2503C4.additionalIncludes preserve=no
//## end module%3911AA2503C4.additionalIncludes

//## begin module%3911AA2503C4.includes preserve=yes

#include "m4serial_dll.hpp"

#include "stdio.h"

#include "clgeniod.hpp"

#include "cltadof.hpp"

#include "m4define.hpp"

#define ABNORMAL_ENDIAN (M4_ARCH!=M4_ARCH_INTEL) // Normal is Intel
// #pragma error ??


//## end module%3911AA2503C4.includes

// m4types
#include <m4types.hpp>
//## begin module%3911AA2503C4.declarations preserve=no
//## end module%3911AA2503C4.declarations

//## begin module%3911AA2503C4.additionalDeclarations preserve=yes
//## end module%3911AA2503C4.additionalDeclarations


//## begin ClFileIODriver%3911AA2503CE.preface preserve=yes
//## end ClFileIODriver%3911AA2503CE.preface

//## Class: ClFileIODriver%3911AA2503CE
//	#pragma error ??
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClFileIODriver : public ClGenericIODriver  //## Inherits: <unnamed>%3911AA260260
{
  //## begin ClFileIODriver%3911AA2503CE.initialDeclarations preserve=yes
public:

	  // --- Error support ---
	  enum FileErrors
	  { FILE_ERR_OPEN_FILE=GEN_ERR_N_ERRORS, FILE_ERR_WRITE_FILE,
	    FILE_ERR_READ_FILE, FILE_ERR_SEEK_FILE, FILE_ERR_N_ERRORS };
  //## end ClFileIODriver%3911AA2503CE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClFileIODriver%-409652940; C++
      ClFileIODriver (void );

      //## Operation: ClFileIODriver%1006753913; C++
      //	-- {AddDecl: 305} region.unprotectedFunction [937..1178]
      ClFileIODriver (m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog = M4_TRUE);

    //## Destructor (specified)
      //## Operation: ~ClFileIODriver%-420270528; C++
      //	-- {AddDecl: 308} region.unprotectedFunction [3693..3753]
      virtual ~ClFileIODriver (void );


    //## Other Operations (specified)
      //## Operation: Init%1969669876; C++
      //	-- {AddDecl: 306} region.unprotectedFunction [1179..3418]
      m4return_t Init (m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog = M4_TRUE);

      virtual void	Reset( void ) ;

      //## Operation: File%758901826; C++
      //	-- {AddDecl: 98} module.vulnerableDeclarations [1349..1526]
      //	-- {AddDecl: 309} region.unprotectedFunction [3754..3811]
      FILE* File (void );

      //## Operation: ResetWriteError%-1965503359; C++
      //	-- {AddDecl: 310} region.unprotectedFunction [3812..4091]
      //	////////////////////////////   ERRORS   //////////////////////////////
      void ResetWriteError (void );

      //## Operation: ResetReadError%2133215742; C++
      //	Write
      //	-- {AddDecl: 311} region.unprotectedFunction [4092..4296]
      void ResetReadError (void );

      //## Operation: Write%-1016503413; C++
      m4return_t Write (m4uint32_t ai_iNumber);

      //## Operation: Write%715853397; C++
      //	-- {AddDecl: 314} region.unprotectedFunction [4896..5014]
      m4return_t Write (m4int32_t ai_iNumber);

      //## Operation: Write%-1528002629; C++
      //	-- {AddDecl: 315} region.unprotectedFunction [5015..5134]
      m4return_t Write (m4uint16_t ai_iNumber);

      //## Operation: Write%-656974324; C++
      //	-- {AddDecl: 316} region.unprotectedFunction [5135..5253]
      m4return_t Write (m4int16_t ai_iNumber);

      //## Operation: Write%2041861160; C++
      //	-- {AddDecl: 317} region.unprotectedFunction [5254..5368]
      m4return_t Write (m4uint8_t ai_cChar);

      //## Operation: Write%2093728905; C++
      //	-- {AddDecl: 318} region.unprotectedFunction [5369..5482]
      m4return_t Write (m4int8_t ai_cChar);

      //## Operation: Write%767697442; C++
      //	-- {AddDecl: 319} region.unprotectedFunction [5483..5601]
      m4return_t Write (m4float_t ai_fNumber);

      //## Operation: Write%-1997159106; C++
      //	-- {AddDecl: 320} region.unprotectedFunction [5602..5721]
      m4return_t Write (m4double_t ai_dNumber);

      //## Operation: Write%444285371; C++
      //	-- {AddDecl: 321} region.unprotectedFunction [5722..5852]
      m4return_t Write (m4bool_t ai_iBoolean);

      //## Operation: Write%264418933; C++
      //	-- {AddDecl: 322} region.unprotectedFunction [5853..6817]
      m4return_t Write (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Write%-607996546; C++
      //	-- {AddDecl: 324} region.unprotectedFunction [6991..7428]
      m4return_t Write (void* ai_pPtr, m4uint32_t ai_iSize);

      //## Operation: Write%2124946526; C++
      //	-- {AddDecl: 325} region.unprotectedFunction [7429..7611]
      m4return_t Write (m4pchar_t ai_pString);

      //## Operation: Write%529186606; C++
      //	-- {AddDecl: 326} region.unprotectedFunction [7612..7723]
      m4return_t Write (m4pcchar_t ai_pConstString);

      //## Operation: WritePrivate%933780840; C++
      //	-- {AddDecl: 323} region.unprotectedFunction [6818..6990]
      m4return_t WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: WriteFromFile%-592305341; C++
      //	Read
      //	-- {AddDecl: 327} region.unprotectedFunction [7724..9598]
      m4return_t WriteFromFile (m4pcchar_t ai_pFileName);

      //## Operation: Read%52170989; C++
      m4return_t Read (m4uint32_t& ai_iNumber);

      //## Operation: Read%131008638; C++
      //	-- {AddDecl: 330} region.unprotectedFunction [10189..10290]
      m4return_t Read (m4int32_t& ai_iNumber);

      //## Operation: Read%-10040338; C++
      //	-- {AddDecl: 331} region.unprotectedFunction [10291..10393]
      m4return_t Read (m4uint16_t& ai_iNumber);

      //## Operation: Read%-1033068938; C++
      //	-- {AddDecl: 332} region.unprotectedFunction [10394..10495]
      m4return_t Read (m4int16_t& ai_iNumber);

      //## Operation: Read%-132024382; C++
      //	-- {AddDecl: 333} region.unprotectedFunction [10496..10593]
      m4return_t Read (m4uint8_t& ai_cChar);

      //## Operation: Read%-487451590; C++
      //	-- {AddDecl: 334} region.unprotectedFunction [10594..10690]
      m4return_t Read (m4int8_t& ai_cChar);

      //## Operation: Read%-696517996; C++
      //	-- {AddDecl: 335} region.unprotectedFunction [10691..10792]
      m4return_t Read (m4float_t& ai_fNumber);

      //## Operation: Read%-691472444; C++
      //	-- {AddDecl: 336} region.unprotectedFunction [10793..10895]
      m4return_t Read (m4double_t& ai_dNumber);

      //## Operation: Read%307694310; C++
      //	-- {AddDecl: 337} region.unprotectedFunction [10896..11131]
      m4return_t Read (m4bool_t& ai_iBoolean);

      //## Operation: Read%1983193096; C++
      //	-- {AddDecl: 338} region.unprotectedFunction [11132..11958]
      m4return_t Read (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Read%1278894514; C++
      //	-- {AddDecl: 345} region.unprotectedFunction [13400..13878]
      m4return_t Read (void* ai_pPtr, m4uint32_t& ao_iSize);

      //## Operation: Read%632421546; C++
      //	-- {AddDecl: 348} region.unprotectedFunction [14506..14892]
      m4return_t Read (m4pchar_t ai_pString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: Read%-1758939566; C++
      //	-- {AddDecl: 349} region.unprotectedFunction [14893..15113]
      m4return_t Read (m4pcchar_t ai_pConstString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: ReadPrivate%1839577441; C++
      //	-- {AddDecl: 339} region.unprotectedFunction [11959..12132]
      m4return_t ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: ReadSize%-1216882674; C++
      //	-- {AddDecl: 346} region.unprotectedFunction [13879..14048]
      m4return_t ReadSize (m4uint32_t& ao_iSize);

      //## Operation: ReadBuffer%-1973378766; C++
      //	-- {AddDecl: 347} region.unprotectedFunction [14049..14505]
      m4return_t ReadBuffer (void* ai_pPtr);

      //## Operation: ReadToFile%-102116644; C++
      //	Others
      //	-- {AddDecl: 350} region.unprotectedFunction [15114..16794]
      m4return_t ReadToFile (m4pchar_t ao_pFileName);

      //## Operation: Close%-1601601788; C++
      m4return_t Close (void );

      //## Operation: Flush%1195410010; C++
      //	-- {AddDecl: 352} region.unprotectedFunction [16888..16984]
      m4bool_t Flush (void );

      //## Operation: Eof%1937909443; C++
      //	-- {AddDecl: 351} region.unprotectedFunction [16795..16887]
      m4bool_t Eof (void );

      //## Operation: StoreAddress%-1340936799; C++
      //	-- {AddDecl: 341} region.unprotectedFunction [12213..12692]
      m4return_t StoreAddress (void* ai_pPtr);

      //## Operation: StoreAddress%2032249160; C++
      //	-- {AddDecl: 343} region.unprotectedFunction [12809..13197]
      m4return_t StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: StoreAddressPrivate%665104906; C++
      //	-- {AddDecl: 342} region.unprotectedFunction [12693..12808]
      m4return_t StoreAddressPrivate (void* ai_pPtr);

      //## Operation: StoreAddressPrivate%-744541088; C++
      //	-- {AddDecl: 344} region.unprotectedFunction [13198..13399]
      m4return_t StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: GetActualOffset%474850445; C++
      //	-- {AddDecl: 340} region.unprotectedFunction [12133..12212]
      m4uint32_t GetActualOffset (void );

      //## Operation: End%2087747482; C++
      //	-- {AddDecl: 307} region.unprotectedFunction [3419..3692]
      void End (m4bool_t ai_iCloseFile);

      //## Operation: ResetPrivateInfo%32970469; C++
      //	-- {AddDecl: 100} region.unprotectedFunction [4039..5698]
      //	-- {AddDecl: 353} region.unprotectedFunction [16985..17061]
      m4return_t ResetPrivateInfo (void );

      virtual	size_t	FRead (       void* ai_pvPointer,  size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile ) ;
      virtual	size_t	FWrite( const void* ai_cpvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile ) ;

      //## Operation: _Translate%1414679500; C++
      static void _Translate (m4char_t* ao_pBuffer, m4int32_t ai_iSize)
      {
        //## begin ClFileIODriver::_Translate%1414679500.body preserve=yes
         #if ABNORMAL_ENDIAN==1 // Assuming that the natural plataform is NT ! ! !
         m4uint8_t uiAux1,
                   uiAux2,
                   uiAux3,
                   uiAux4;

         switch (ai_iSize)
         {
            case 1 :
               break;

            case 2 :
               uiAux1=ao_pBuffer[0];
               ao_pBuffer[0]=ao_pBuffer[1];
               ao_pBuffer[1]=uiAux1;
               break;

            case 4 :
               uiAux1=ao_pBuffer[0];
               uiAux2=ao_pBuffer[1];
               ao_pBuffer[0]=ao_pBuffer[3];
               ao_pBuffer[1]=ao_pBuffer[2];
               ao_pBuffer[2]=uiAux2;
               ao_pBuffer[3]=uiAux1;
               break;

            case 8 :
               uiAux1=ao_pBuffer[0];
               uiAux2=ao_pBuffer[1];
               uiAux3=ao_pBuffer[2];
               uiAux4=ao_pBuffer[3];
               ao_pBuffer[0]=ao_pBuffer[7];
               ao_pBuffer[1]=ao_pBuffer[6];
               ao_pBuffer[2]=ao_pBuffer[5];
               ao_pBuffer[3]=ao_pBuffer[4];
               ao_pBuffer[4]=uiAux4;
               ao_pBuffer[5]=uiAux3;
               ao_pBuffer[6]=uiAux2;
               ao_pBuffer[7]=uiAux1;
               break;

            default : // Hay algun caso...
               for (m4int32_t i=0; i<(ai_iSize>>1); i++)
               {
                  uiAux1=ao_pBuffer[i];
                  ao_pBuffer[i]=ao_pBuffer[(ai_iSize-1)-i];
                  ao_pBuffer[(ai_iSize-1)-i]=uiAux1;
               }
               break;
         }
         #endif
      
        //## end ClFileIODriver::_Translate%1414679500.body
      }

    // Additional Public Declarations
      //## begin ClFileIODriver%3911AA2503CE.public preserve=yes
      //## end ClFileIODriver%3911AA2503CE.public

  protected:
    // Additional Protected Declarations
      //## begin ClFileIODriver%3911AA2503CE.protected preserve=yes
      //## end ClFileIODriver%3911AA2503CE.protected

  protected:

    //## Other Operations (specified)


    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA260253
      //## Role: ClFileIODriver::m_oTAO%3911AA260255
      //## begin ClFileIODriver::m_oTAO%3911AA260255.role preserve=no  private: ClTableAddressOffset {1 -> 0..1RHAPN}
      ClTableAddressOffset *m_oTAO;
      //## end ClFileIODriver::m_oTAO%3911AA260255.role

      //## Association: M4Serial::<unnamed>%3911AA2600B5
      //## Role: ClFileIODriver::m_iMode%3911AA2600B7
      //## begin ClFileIODriver::m_iMode%3911AA2600B7.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iMode;
      //## end ClFileIODriver::m_iMode%3911AA2600B7.role

      //## Association: M4Serial::<unnamed>%3911AA2600B8
      //## Role: ClFileIODriver::m_iLastSize%3911AA2600BA
      //## begin ClFileIODriver::m_iLastSize%3911AA2600BA.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iLastSize;
      //## end ClFileIODriver::m_iLastSize%3911AA2600BA.role

      //## Association: M4Serial::<unnamed>%3911AA2600BB
      //## Role: ClFileIODriver::m_pFile%3911AA2600BD
      //## begin ClFileIODriver::m_pFile%3911AA2600BD.role preserve=no  private: FILE {1 -> 0..1RHAPN}
      FILE *m_pFile;
      //## end ClFileIODriver::m_pFile%3911AA2600BD.role

      //## Association: M4Serial::<unnamed>%3911AA26025D
      //## Role: ClFileIODriver::m_pcFileName%3911AA26025F
      //## begin ClFileIODriver::m_pcFileName%3911AA26025F.role preserve=no  private: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_pcFileName;
      //## end ClFileIODriver::m_pcFileName%3911AA26025F.role

    // Additional Private Declarations
      //## begin ClFileIODriver%3911AA2503CE.private preserve=yes
      //## end ClFileIODriver%3911AA2503CE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileIODriver%3911AA2503CE.implementation preserve=yes
      //## end ClFileIODriver%3911AA2503CE.implementation

};

//## begin ClFileIODriver%3911AA2503CE.postscript preserve=yes
//## end ClFileIODriver%3911AA2503CE.postscript

//## begin module%3911AA2503C4.epilog preserve=yes
//## end module%3911AA2503C4.epilog



/*
Esta clase implementa un driver de fichero que permite serializar y deserializar
en cualquier parte del fichero y no sólo serializar al final y deserializar al principio.
Para conseguirlo mantiene en el fichero una estructura de bloques enlazados, de modo
que cuando se va a leer o escribir en un bloque se comprueba si lo que se quiere leer
o escribir cabe en el bloque actual. Si es así lee o escribe en el bloque actual.
Si no es así lee o escribe parte del bloque actual, avanza al siguiente bloque y lee
o escribe el resto de este nuevo bloque, y así hasta que se acaba de leer o escribir
el tamaño pedido.
La estructura de los bloques es la siguiente:

	sizeof( long ) bytes con el tamaño útil del bloque (tamaño físico menos los bytes de control)
	Tantos bytes de contenido como indiquen los bytes anteriores
	sizeof( long ) bytes con el comienzo del siguiente bloque enlazado o -1 si no hay más bloques

En el comienzo del fichero siempre se escribe 1 byte auxiliar. Esto se explica en
los comentarios de las funciones.

Para manejar este driver siempre hay que seguir la secuencia de

	Comienzo de lectura o escritura
	Lectura o escritura de los datos
	Final de la lectura escritura

En el comienzo de la lectura o escritura el usuario debe pasar el comienzo del
bloque en el que se quiere trabajar y se realiza el posicionamiento inicial
En el final de la escritura se devuelve el valor donde comienza el bloque
para que el usuario pueda guardarlo si es un bloque nuevo. La función de final
de lectura no hace nada más que resetear variable, pero se implementa por simetría.

Para poder utilizar toda la funcionalidad del driver de fichero original se hereda
de él y se han definido las funciones FRead y FWrite que son virtuales y implementan
la gestíon de la lectura escritura entre bloques. En la clase padre esas funciones
se han mapeado directamente a fread y fwrite y se ha cambiado el uso de éstas por
el uso de las otras. Ha habido que hacer los miembros del padre protected para
poder acceder a ellos. Ha habido que poner virtual el destructor del padre.
También se ha implementado la función Reset que limpia la tabla de direcciones.

La clase mantiene dos contadores. El tamaño que queda por leer o escribir en el
bloque actual o -1 (realmente un número negativo) si estamos escribiendo al final
del fichero. Y el comienzo del bloque actual. En modo de lectura el comienzo
del bloque no se utiliza.
*/
class M4_DECL_M4SERIAL ClRamdomFileIODriver : public ClFileIODriver
{

public:

		ClRamdomFileIODriver( void ) ;
		ClRamdomFileIODriver( m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog = M4_TRUE ) ;

	virtual ~ClRamdomFileIODriver( void ) ;


	virtual void	Reset( void ) ;

	m4return_t		StartReading( long ai_lPosition ) ;
	virtual	size_t	FRead( void* ai_pvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile ) ;
	m4return_t		EndReading( void ) ;

	m4return_t		StartWritting( long ai_lPosition ) ;
	virtual	size_t	FWrite( const void* ai_cpvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile ) ;
	m4return_t		EndWritting( long& ao_rlPosition ) ;

protected:

	m4return_t	_Start( long ai_lPosition, m4bool_t ai_bRead ) ;
	size_t		_FReadWrite( void* ai_pvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile, m4bool_t ai_bRead ) ;
	m4return_t	_End( long& ao_rlPosition, m4bool_t ai_bRead ) ;


	long	m_lFreeSize ;
	long	m_lChunkStart ;

} ;



#endif
