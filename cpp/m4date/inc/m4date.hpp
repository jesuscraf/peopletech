//## begin module%3779DEF60046.cm preserve=no
//## end module%3779DEF60046.cm

//## begin module%3779DEF60046.cp preserve=no
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
//## end module%3779DEF60046.cp

//## Module: m4date%3779DEF60046; Package specification
//## Subsystem: M4Date::inc%3779DEF502DA
//	d:\compon\m4date\version\inc
//## Source file: d:\compon\m4date\version\inc\m4date.hpp

#ifndef __M4DATE_H_
#define __M4DATE_H_ 1

//## begin module%3779DEF60046.additionalIncludes preserve=no
//## end module%3779DEF60046.additionalIncludes

//## begin module%3779DEF60046.includes preserve=yes

#include "m4stl.hpp"
#include "m4string.hpp"
#include "cldates.hpp"
#include "m4date_dll.hpp"

#ifdef _UNIX

/*
 * JRM, 02/Dec/1998
 * Cambio m4sleep, debido a que:
 *
 *  Los defines de funciones son peligrosos; es mejor crear una función que encapsule
 * las diferencias entre distintos s.o.
 *
 *  En el s.o. Solaris, usar sleep en  entorno multithread provoca errores. Por ello, se
 * aprovecha el timeout que proporciona la función select, que si es MT safe.
 */
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

#ifdef _WINDOWS
#include <windows.h>
#include <time.h>
//## end module%3779DEF60046.includes

// m4types
#include <m4types.hpp>
// wchar
#include <wchar.h>
// winsock2
#include <winsock.h>
//## begin module%3779DEF60046.declarations preserve=no
//## end module%3779DEF60046.declarations

//## begin module%3779DEF60046.additionalDeclarations preserve=yes
#endif

void M4_DECL_M4DATE m4sleep(int);
void M4_DECL_M4DATE m4msleep(int);
void M4_DECL_M4DATE m4usleep(int);





//==============================================================================
//
//   Include files from the operating system.
//
//==============================================================================

//------------------------------------------------------------------------------
// This include is for the declaration of...
//------------------------------------------------------------------------------

//==============================================================================
//
//   Include files from the project.
//
//==============================================================================


//==============================================================================
//
//   Defines from the project.
//
//==============================================================================



//==============================================================================
//
//   Definición clase M4Date,  que es la que accede a la BD
//
//  NOTA: los dias son del 1 al 28.29.30.31  según el año
//        los meses son del 1-Enero al 12-Diciembre
//==============================================================================



//## end module%3779DEF60046.additionalDeclarations


//## Class: m4dates_t%3779DEFA027D
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef long m4dates_t;

//## begin m4time_t%3779DEFB009B.preface preserve=yes
//## end m4time_t%3779DEFB009B.preface

//## Class: m4time_t%3779DEFB009B
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef double m4time_t;

//## begin m4time_t%3779DEFB009B.postscript preserve=yes
//## end m4time_t%3779DEFB009B.postscript



//## begin M4ClDate%3779DEF60097.preface preserve=yes
//## end M4ClDate%3779DEF60097.preface

//## Class: M4ClDate%3779DEF60097
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3779DEFA02E4;m4return_t { -> }
//## Uses: <unnamed>%3779DEFA02F7;m4seconds_t { -> }
//## Uses: <unnamed>%3779DEFA0312;m4bool_t { -> }
//## Uses: <unnamed>%3779DEFA031F;m4pchar_t { -> }
//## Uses: <unnamed>%3779DEFA033C;M4ClString { -> }
//## Uses: <unnamed>%3779DEFA03D1;time_t { -> }
//## Uses: <unnamed>%3779DEFA03DC;tm { -> }

class M4_DECL_M4DATE  M4ClDate 
{
  //## begin M4ClDate%3779DEF60097.initialDeclarations preserve=yes
  //## end M4ClDate%3779DEF60097.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClDate%1545979191; C++
      //	////////////  constructores y destructores
      M4ClDate ()
        //## begin M4ClDate::M4ClDate%1545979191.hasinit preserve=no
        //## end M4ClDate::M4ClDate%1545979191.hasinit
        //## begin M4ClDate::M4ClDate%1545979191.initialization preserve=yes
        //## end M4ClDate::M4ClDate%1545979191.initialization
      {
        //## begin M4ClDate::M4ClDate%1545979191.body preserve=yes
 reset();
        //## end M4ClDate::M4ClDate%1545979191.body
      }

      //## Operation: M4ClDate%1125775565; C++
      M4ClDate (const M4ClDate& ai_Date)
        //## begin M4ClDate::M4ClDate%1125775565.hasinit preserve=no
        //## end M4ClDate::M4ClDate%1125775565.hasinit
        //## begin M4ClDate::M4ClDate%1125775565.initialization preserve=yes
        //## end M4ClDate::M4ClDate%1125775565.initialization
      {
        //## begin M4ClDate::M4ClDate%1125775565.body preserve=yes
if ( set (ai_Date) == M4_ERROR) reset();
        //## end M4ClDate::M4ClDate%1125775565.body
      }

      //## Operation: M4ClDate%1496470062; C++
      M4ClDate (const m4pchar_t ai_pchar)
        //## begin M4ClDate::M4ClDate%1496470062.hasinit preserve=no
        //## end M4ClDate::M4ClDate%1496470062.hasinit
        //## begin M4ClDate::M4ClDate%1496470062.initialization preserve=yes
        //## end M4ClDate::M4ClDate%1496470062.initialization
      {
        //## begin M4ClDate::M4ClDate%1496470062.body preserve=yes
 if (set(ai_pchar) == M4_ERROR) reset(); 
        //## end M4ClDate::M4ClDate%1496470062.body
      }

      //## Operation: M4ClDate%128703627; C++
      M4ClDate (const M4ClString& ai_mschar)
        //## begin M4ClDate::M4ClDate%128703627.hasinit preserve=no
        //## end M4ClDate::M4ClDate%128703627.hasinit
        //## begin M4ClDate::M4ClDate%128703627.initialization preserve=yes
        //## end M4ClDate::M4ClDate%128703627.initialization
      {
        //## begin M4ClDate::M4ClDate%128703627.body preserve=yes
 if (set(ai_mschar) == M4_ERROR) reset();
        //## end M4ClDate::M4ClDate%128703627.body
      }

      //## Operation: M4ClDate%-702983880; C++
      M4ClDate (const m4dates_t& ai_lJul)
        //## begin M4ClDate::M4ClDate%-702983880.hasinit preserve=no
        //## end M4ClDate::M4ClDate%-702983880.hasinit
        //## begin M4ClDate::M4ClDate%-702983880.initialization preserve=yes
        //## end M4ClDate::M4ClDate%-702983880.initialization
      {
        //## begin M4ClDate::M4ClDate%-702983880.body preserve=yes
  if (set(ai_lJul) == M4_ERROR) reset(); 
        //## end M4ClDate::M4ClDate%-702983880.body
      }

    //## Destructor (specified)
      //## Operation: ~M4ClDate%-1669786668; C++
      ~M4ClDate ()
      {
        //## begin M4ClDate::~M4ClDate%-1669786668.body preserve=yes
 reset(); 
        //## end M4ClDate::~M4ClDate%-1669786668.body
      }


    //## Other Operations (specified)
      //## Operation: reset%2129287049; C++
      //	////////////  conversores de tipo
      //	pone el dia 0 juliano
      m4return_t reset ()
      {
        //## begin M4ClDate::reset%2129287049.body preserve=yes
 m_numdays = 0; return M4_SUCCESS; 
        //## end M4ClDate::reset%2129287049.body
      }

      //## Operation: set%-1113880272; C++
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const M4ClDate& ai_Date)
      {
        //## begin M4ClDate::set%-1113880272.body preserve=yes
 m_numdays = ai_Date.m_numdays; return M4_SUCCESS; 
        //## end M4ClDate::set%-1113880272.body
      }

      //## Operation: set%1187313380; C++
      //	////////////  conversores de tipo
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const m4pchar_t ai_pcDate);

      //## Operation: set%-1461480372; C++
      m4return_t set (const M4ClString& ai_pcDate);

      //## Operation: set%666121618; C++
      m4return_t set (const m4int32_t& year, const m4int32_t& month, const m4int32_t& day);

      //## Operation: set%50083519; C++
      m4return_t set (const m4dates_t& ai_time)
      {
        //## begin M4ClDate::set%50083519.body preserve=yes
m_numdays = ai_time; return M4_SUCCESS;
        //## end M4ClDate::set%50083519.body
      }

      //## Operation: now%-1344834786; C++
      m4return_t now ();

      //## Operation: operator =%-1742363750; C++
      //	pone la fecha actual
      //	////////////  asignaciones
      const M4ClDate& operator = (const M4ClDate& ai_Date)
      {
        //## begin M4ClDate::operator =%-1742363750.body preserve=yes
 set(ai_Date); return *this;
        //## end M4ClDate::operator =%-1742363750.body
      }

      //## Operation: operator =%77079784; C++
      const M4ClDate& operator = (const m4dates_t& ai_Jul)
      {
        //## begin M4ClDate::operator =%77079784.body preserve=yes
 set(ai_Jul); return *this;
        //## end M4ClDate::operator =%77079784.body
      }

      //## Operation: operator =%606708107; C++
      const M4ClDate& operator = (const m4pchar_t ai_char)
      {
        //## begin M4ClDate::operator =%606708107.body preserve=yes
set (ai_char); return *this;
        //## end M4ClDate::operator =%606708107.body
      }

      //## Operation: operator =%-1102831308; C++
      const M4ClDate& operator = (const M4ClString& ai_char)
      {
        //## begin M4ClDate::operator =%-1102831308.body preserve=yes
set (ai_char); return *this;
        //## end M4ClDate::operator =%-1102831308.body
      }

      //## Operation: get%619093995; C++
      m4return_t get (M4ClString& ai_pcDate);

      //## Operation: get%651866307; C++
      m4return_t get (m4int32_t& a_year, m4int32_t& a_month, m4int32_t& a_day);

      //## Operation: get%1844109097; C++
      //	la cantidad de segundos
      m4return_t get (m4dates_t& ao_time)
      {
        //## begin M4ClDate::get%1844109097.body preserve=yes
 ao_time = m_numdays;return M4_SUCCESS;
        //## end M4ClDate::get%1844109097.body
      }

      //## Operation: get_dayofWeek%-1486394387; C++
      //	1-Lunes, 2-Martes, 3-Miercoles, .., 7-Domingo
      m4return_t get_dayofWeek (m4int32_t& ao_wday);

      //## Operation: operator <%-274135588; C++
      //	///////////////  comparacion
      m4bool_t operator < (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator <%-274135588.body preserve=yes
 return M4_BOOL(m_numdays < ai_hora1.m_numdays) ;
        //## end M4ClDate::operator <%-274135588.body
      }

      //## Operation: operator >%240674692; C++
      m4bool_t operator > (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator >%240674692.body preserve=yes
 return M4_BOOL(m_numdays > ai_hora1.m_numdays) ;
        //## end M4ClDate::operator >%240674692.body
      }

      //## Operation: operator ==%-173212813; C++
      m4bool_t operator == (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator ==%-173212813.body preserve=yes
 return M4_BOOL(m_numdays == ai_hora1.m_numdays) ;
        //## end M4ClDate::operator ==%-173212813.body
      }

      //## Operation: operator <=%478752064; C++
      m4bool_t operator <= (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator <=%478752064.body preserve=yes
 return M4_BOOL(m_numdays <= ai_hora1.m_numdays) ;
        //## end M4ClDate::operator <=%478752064.body
      }

      //## Operation: operator >=%1266111089; C++
      m4bool_t operator >= (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator >=%1266111089.body preserve=yes
 return M4_BOOL(m_numdays >= ai_hora1.m_numdays) ;
        //## end M4ClDate::operator >=%1266111089.body
      }

      //## Operation: operator !=%1858288662; C++
      m4bool_t operator != (const M4ClDate& ai_hora1)
      {
        //## begin M4ClDate::operator !=%1858288662.body preserve=yes
 return M4_BOOL(m_numdays != ai_hora1.m_numdays) ;
        //## end M4ClDate::operator !=%1858288662.body
      }

      //## Operation: operator +%1105840763; C++
      //	//////////////// operaciones
      m4dates_t operator + (const m4dates_t& ai_hora1);

      //## Operation: operator -%1634427330; C++
      m4dates_t operator - (const m4dates_t& ai_hora1);

      //## Operation: operator +=%-823052456; C++
      const m4dates_t operator += (const m4dates_t& ai_hora1)
      {
        //## begin M4ClDate::operator +=%-823052456.body preserve=yes
add_days( ai_hora1 ); return m_numdays;
        //## end M4ClDate::operator +=%-823052456.body
      }

      //## Operation: add_days%-1413563437; C++
      m4return_t add_days (const m4int32_t& ai_num_days)
      {
        //## begin M4ClDate::add_days%-1413563437.body preserve=yes
 m_numdays += ai_num_days; return M4_SUCCESS;
        //## end M4ClDate::add_days%-1413563437.body
      }

      //## Operation: operator -=%543455069; C++
      const m4dates_t operator -= (const m4dates_t& ai_hora1)
      {
        //## begin M4ClDate::operator -=%543455069.body preserve=yes
add_days( -ai_hora1 ); return m_numdays;
        //## end M4ClDate::operator -=%543455069.body
      }

      //## Operation: add_months%30437418; C++
      m4return_t add_months (const m4int32_t& ai_num_months);

      //## Operation: add_years%-1132219383; C++
      m4return_t add_years (const m4int32_t& ai_num_years);

      //## Operation: sub_days%-567136918; C++
      m4return_t sub_days (const m4int32_t& ai_num_days)
      {
        //## begin M4ClDate::sub_days%-567136918.body preserve=yes
 return add_days( -ai_num_days); 
        //## end M4ClDate::sub_days%-567136918.body
      }

      //## Operation: sub_months%647603905; C++
      m4return_t sub_months (const m4int32_t& ai_num_months)
      {
        //## begin M4ClDate::sub_months%647603905.body preserve=yes
 return add_months( -ai_num_months); 
        //## end M4ClDate::sub_months%647603905.body
      }

      //## Operation: sub_years%-436225140; C++
      m4return_t sub_years (const m4int32_t& ai_num_years)
      {
        //## begin M4ClDate::sub_years%-436225140.body preserve=yes
 return add_years( -ai_num_years); 
        //## end M4ClDate::sub_years%-436225140.body
      }

      //## Operation: operator m4dates_t%-1879220757; C++
      //	//////////////// conversiones
      operator m4dates_t ()
      {
        //## begin M4ClDate::operator m4dates_t%-1879220757.body preserve=yes
 return m_numdays; 
        //## end M4ClDate::operator m4dates_t%-1879220757.body
      }

      //## Operation: operator M4ClString%-831920135; C++
/*      operator M4ClString ()
      {
        //## begin M4ClDate::operator M4ClString%-831920135.body preserve=yes
 M4ClString sTemp; get(sTemp); return sTemp; 
        //## end M4ClDate::operator M4ClString%-831920135.body
      }
*/
      //## Operation: LocalTime%-402065951; C++
      static void LocalTime (time_t ai_time, tm& ao_tm);

      //## Operation: GmTime%-724220446; C++
      static void GmTime (time_t ai_time, tm& ao_tm);

    // Additional Public Declarations
      //## begin M4ClDate%3779DEF60097.public preserve=yes
      static void AscTime (struct tm& ai_time, m4pchar_t ao_buffer);
      //## end M4ClDate%3779DEF60097.public

  protected:

    //## Other Operations (specified)
      //## Operation: diamesano%-1697966372; C++
      //	// FUNCIONES PRIVADAS _____
      //	de un numero de dias da el dia el mes y año
      m4return_t diamesano (m4dates_t dias, m4int32_t* dia, m4int32_t* mes, m4int32_t* ano);

      //## Operation: is_ok%-785046232; C++
      //	indica si la cifra dia mes año es valida
      m4bool_t is_ok (m4int32_t year, m4int32_t month, m4int32_t day);

      //## Operation: numdias%1473450782; C++
      //	da el numerod e dias de la fecha dia, mes , año
      //	=============================================================================
      //	=
      //
      //	Defines from the project.
      //
      //	=============================================================================
      //	=
      //	////////////////////////   DEFINES
      //	=============================================================================
      //	=
      //
      //	Definición  de las funciones de Julianos
      //
      //	=============================================================================
      //	=
      //	**********************************************
      //	***************** M4ClDate_diamesano ******************
      //	**********************************************
      //	m4int32_terna calcula dia, mes a¤o a partir del n
      //	de d¡as desde 31-12-0000
      //	**********************************************
      m4dates_t numdias (m4int32_t dia, m4int32_t mes, m4int32_t anno);

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%3779DEFA029B
      //## Role: M4ClDate::m_numdays%3779DEFA02CD
      //## begin M4ClDate::m_numdays%3779DEFA02CD.role preserve=no  protected: m4dates_t {1 -> 1VHAPN}
      m4dates_t m_numdays;
      //## end M4ClDate::m_numdays%3779DEFA02CD.role

    // Additional Protected Declarations
      //## begin M4ClDate%3779DEF60097.protected preserve=yes
      //## end M4ClDate%3779DEF60097.protected

  private:
    // Additional Private Declarations
      //## begin M4ClDate%3779DEF60097.private preserve=yes
      //## end M4ClDate%3779DEF60097.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClDate%3779DEF60097.implementation preserve=yes
      //## end M4ClDate%3779DEF60097.implementation

};

//## begin M4ClDate%3779DEF60097.postscript preserve=yes

//==============================================================================
//
//   Definición clase M4ClHour2Second,  que es la que accede a la BD
//
//==============================================================================

typedef m4int32_t m4seconds_t;

//## end M4ClDate%3779DEF60097.postscript

//## begin M4ClHour2Second%3779DEFB001B.preface preserve=yes
//## end M4ClHour2Second%3779DEFB001B.preface

//## Class: M4ClHour2Second%3779DEFB001B
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3779DEFB001C;m4return_t { -> }
//## Uses: <unnamed>%3779DEFB0025;m4pchar_t { -> }
//## Uses: <unnamed>%3779DEFB0028;M4ClString { -> }
//## Uses: <unnamed>%3779DEFB0050;m4bool_t { -> }
//## Uses: <unnamed>%3779DEFB0072;m4seconds_t { -> }
//## Uses: <unnamed>%377A04060376;M4ClDate { -> }

class M4_DECL_M4DATE  M4ClHour2Second 
{
  //## begin M4ClHour2Second%3779DEFB001B.initialDeclarations preserve=yes
  //## end M4ClHour2Second%3779DEFB001B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClHour2Second%-1560329228; C++
      //	////////////  constructores y destructores
      M4ClHour2Second ()
        //## begin M4ClHour2Second::M4ClHour2Second%-1560329228.hasinit preserve=no
        //## end M4ClHour2Second::M4ClHour2Second%-1560329228.hasinit
        //## begin M4ClHour2Second::M4ClHour2Second%-1560329228.initialization preserve=yes
        //## end M4ClHour2Second::M4ClHour2Second%-1560329228.initialization
      {
        //## begin M4ClHour2Second::M4ClHour2Second%-1560329228.body preserve=yes
 reset();
        //## end M4ClHour2Second::M4ClHour2Second%-1560329228.body
      }

      //## Operation: M4ClHour2Second%-374535670; C++
      M4ClHour2Second (const M4ClHour2Second& ai_Hour)
        //## begin M4ClHour2Second::M4ClHour2Second%-374535670.hasinit preserve=no
        //## end M4ClHour2Second::M4ClHour2Second%-374535670.hasinit
        //## begin M4ClHour2Second::M4ClHour2Second%-374535670.initialization preserve=yes
        //## end M4ClHour2Second::M4ClHour2Second%-374535670.initialization
      {
        //## begin M4ClHour2Second::M4ClHour2Second%-374535670.body preserve=yes
if( set( ai_Hour) == M4_ERROR) reset(); 
        //## end M4ClHour2Second::M4ClHour2Second%-374535670.body
      }

      //## Operation: M4ClHour2Second%997005845; C++
      M4ClHour2Second (const m4pchar_t ai_pchar)
        //## begin M4ClHour2Second::M4ClHour2Second%997005845.hasinit preserve=no
        //## end M4ClHour2Second::M4ClHour2Second%997005845.hasinit
        //## begin M4ClHour2Second::M4ClHour2Second%997005845.initialization preserve=yes
        //## end M4ClHour2Second::M4ClHour2Second%997005845.initialization
      {
        //## begin M4ClHour2Second::M4ClHour2Second%997005845.body preserve=yes
  if (set(ai_pchar) == M4_ERROR) reset(); 
        //## end M4ClHour2Second::M4ClHour2Second%997005845.body
      }

      //## Operation: M4ClHour2Second%-332865994; C++
      M4ClHour2Second (const M4ClString& ai_shar)
        //## begin M4ClHour2Second::M4ClHour2Second%-332865994.hasinit preserve=no
        //## end M4ClHour2Second::M4ClHour2Second%-332865994.hasinit
        //## begin M4ClHour2Second::M4ClHour2Second%-332865994.initialization preserve=yes
        //## end M4ClHour2Second::M4ClHour2Second%-332865994.initialization
      {
        //## begin M4ClHour2Second::M4ClHour2Second%-332865994.body preserve=yes
 if (set(ai_shar)== M4_ERROR) reset(); 
        //## end M4ClHour2Second::M4ClHour2Second%-332865994.body
      }

      //## Operation: M4ClHour2Second%-1127292345; C++
      M4ClHour2Second (const m4seconds_t& ai_lJul)
        //## begin M4ClHour2Second::M4ClHour2Second%-1127292345.hasinit preserve=no
        //## end M4ClHour2Second::M4ClHour2Second%-1127292345.hasinit
        //## begin M4ClHour2Second::M4ClHour2Second%-1127292345.initialization preserve=yes
        //## end M4ClHour2Second::M4ClHour2Second%-1127292345.initialization
      {
        //## begin M4ClHour2Second::M4ClHour2Second%-1127292345.body preserve=yes
  if (set(ai_lJul) == M4_ERROR) reset(); 
        //## end M4ClHour2Second::M4ClHour2Second%-1127292345.body
      }

    //## Destructor (specified)
      //## Operation: ~M4ClHour2Second%614282393; C++
      ~M4ClHour2Second ()
      {
        //## begin M4ClHour2Second::~M4ClHour2Second%614282393.body preserve=yes
 reset(); 
        //## end M4ClHour2Second::~M4ClHour2Second%614282393.body
      }


    //## Other Operations (specified)
      //## Operation: reset%-756254878; C++
      //	////////////  conversores de tipo
      m4return_t reset ()
      {
        //## begin M4ClHour2Second::reset%-756254878.body preserve=yes
m_numseconds = 0; return M4_SUCCESS;
        //## end M4ClHour2Second::reset%-756254878.body
      }

      //## Operation: set%-27414694; C++
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const M4ClHour2Second& ai_Date);

      //## Operation: set%-557392168; C++
      //	////////////  conversores de tipo
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const m4pchar_t ai_pcDate);

      //## Operation: set%-101020067; C++
      m4return_t set (const M4ClString& ai_pcDate);

      //## Operation: set%-382550520; C++
      m4return_t set (const m4int32_t ai_hour, const m4int32_t ai_min, const m4int32_t ai_sec);

      //## Operation: set%1231272942; C++
      m4return_t set (const m4seconds_t ai_time);

      //## Operation: now%1014404966; C++
      //	pone a 00:00:00
      m4return_t now ();

      //## Operation: operator =%-748785989; C++
      //	coge la hora del sistema
      //	////////////  asignaciones
      const M4ClHour2Second& operator = (const M4ClHour2Second& ai_Date)
      {
        //## begin M4ClHour2Second::operator =%-748785989.body preserve=yes
 set(ai_Date); return *this;
        //## end M4ClHour2Second::operator =%-748785989.body
      }

      //## Operation: operator =%1431116878; C++
      const M4ClHour2Second& operator = (const m4seconds_t& ai_Jul)
      {
        //## begin M4ClHour2Second::operator =%1431116878.body preserve=yes
 set(ai_Jul); return *this;
        //## end M4ClHour2Second::operator =%1431116878.body
      }

      //## Operation: operator =%-1870622341; C++
      const M4ClHour2Second& operator = (const m4pchar_t ai_char)
      {
        //## begin M4ClHour2Second::operator =%-1870622341.body preserve=yes
set (ai_char); return *this;
        //## end M4ClHour2Second::operator =%-1870622341.body
      }

      //## Operation: operator =%551163272; C++
      const M4ClHour2Second& operator = (const M4ClString& ai_char)
      {
        //## begin M4ClHour2Second::operator =%551163272.body preserve=yes
set (ai_char); return *this;
        //## end M4ClHour2Second::operator =%551163272.body
      }

      //## Operation: get%-1043751093; C++
      m4return_t get (M4ClString& ai_pcDate);

      //## Operation: get%1198240775; C++
      m4return_t get (m4int32_t& ao_ihour, m4int32_t& ao_imin, m4int32_t& ao_isec);

      //## Operation: get%1239582698; C++
      //	la cantidad de segundos
      m4return_t get (m4seconds_t& ao_time);

      //## Operation: operator <%-1119428101; C++
      //	///////////////  comparacion
      m4bool_t operator < (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator <%-1119428101.body preserve=yes
return M4_BOOL(m_numseconds < ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator <%-1119428101.body
      }

      //## Operation: operator >%536972909; C++
      m4bool_t operator > (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator >%536972909.body preserve=yes
return M4_BOOL(m_numseconds > ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator >%536972909.body
      }

      //## Operation: operator ==%1104870725; C++
      m4bool_t operator == (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator ==%1104870725.body preserve=yes
return M4_BOOL(m_numseconds == ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator ==%1104870725.body
      }

      //## Operation: operator <=%-1673781336; C++
      m4bool_t operator <= (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator <=%-1673781336.body preserve=yes
return M4_BOOL(m_numseconds <= ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator <=%-1673781336.body
      }

      //## Operation: operator >=%1010835517; C++
      m4bool_t operator >= (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator >=%1010835517.body preserve=yes
return M4_BOOL(m_numseconds >= ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator >=%1010835517.body
      }

      //## Operation: operator !=%-1006497259; C++
      m4bool_t operator != (const M4ClHour2Second& ai_hora1)
      {
        //## begin M4ClHour2Second::operator !=%-1006497259.body preserve=yes
return M4_BOOL(m_numseconds != ai_hora1.m_numseconds) ;
        //## end M4ClHour2Second::operator !=%-1006497259.body
      }

      //## Operation: operator +%-463649005; C++
      //	///////////////  operaciones
      //	no hay ningún ajuste de días
      m4seconds_t operator + (const m4seconds_t& ai_hora1);

      //## Operation: operator -%-1906980919; C++
      m4seconds_t operator - (const m4seconds_t& ai_hora1);

      //## Operation: operator +=%299288208; C++
      const m4seconds_t operator += (const m4seconds_t& ai_hora1)
      {
        //## begin M4ClHour2Second::operator +=%299288208.body preserve=yes
add_seconds( ai_hora1 ); return m_numseconds;
        //## end M4ClHour2Second::operator +=%299288208.body
      }

      //## Operation: operator -=%1830252586; C++
      const m4seconds_t operator -= (const m4seconds_t& ai_hora1)
      {
        //## begin M4ClHour2Second::operator -=%1830252586.body preserve=yes
add_seconds( -ai_hora1 ); return m_numseconds;
        //## end M4ClHour2Second::operator -=%1830252586.body
      }

      //## Operation: operator m4seconds_t%1394574244; C++
      //	//////////////// conversiones
      operator m4seconds_t ()
      {
        //## begin M4ClHour2Second::operator m4seconds_t%1394574244.body preserve=yes
 return m_numseconds; 
        //## end M4ClHour2Second::operator m4seconds_t%1394574244.body
      }

      //## Operation: operator M4ClString%1686687818; C++
      operator M4ClString ()
      {
        //## begin M4ClHour2Second::operator M4ClString%1686687818.body preserve=yes
 M4ClString sTemp;	get( sTemp); return sTemp; 
        //## end M4ClHour2Second::operator M4ClString%1686687818.body
      }

      //## Operation: add_seconds%-1496025804; C++
      //	/  devuelve un numero positivo si sobran días
      //	/  siempre es una hora valida
      m4return_t add_seconds (const m4int32_t& ai_num_seconds);

      //## Operation: add_minutes%1736473490; C++
      m4return_t add_minutes (const m4int32_t& ai_num_minutes);

      //## Operation: add_hours%1607782103; C++
      m4return_t add_hours (const m4int32_t& ai_num_hours);

      //## Operation: sub_seconds%-1937470579; C++
      m4return_t sub_seconds (const m4int32_t& ai_num_seconds)
      {
        //## begin M4ClHour2Second::sub_seconds%-1937470579.body preserve=yes
 return add_seconds( -ai_num_seconds);
        //## end M4ClHour2Second::sub_seconds%-1937470579.body
      }

      //## Operation: sub_minutes%-472127370; C++
      m4return_t sub_minutes (const m4int32_t& ai_num_minutes)
      {
        //## begin M4ClHour2Second::sub_minutes%-472127370.body preserve=yes
 return add_minutes( -ai_num_minutes);
        //## end M4ClHour2Second::sub_minutes%-472127370.body
      }

      //## Operation: sub_hours%838555582; C++
      m4return_t sub_hours (const m4int32_t& ai_num_hours)
      {
        //## begin M4ClHour2Second::sub_hours%838555582.body preserve=yes
 return add_hours( -ai_num_hours);
        //## end M4ClHour2Second::sub_hours%838555582.body
      }

    // Additional Public Declarations
      //## begin M4ClHour2Second%3779DEFB001B.public preserve=yes
      //## end M4ClHour2Second%3779DEFB001B.public

  protected:

    //## Other Operations (specified)
      //## Operation: is_ok%-307678948; C++
      m4return_t is_ok (m4int32_t hour, m4int32_t min, m4int32_t sec);

      //## Operation: horaminseg%-1613651857; C++
      //	/ numero de  dias desde 1970
      //	/ devuelve el numero de segundos de un Hor2Second
      m4return_t horaminseg (m4seconds_t ai_numsec, m4int32_t* sec, m4int32_t* min, m4int32_t* hour);

      //## Operation: numseconds%1264425121; C++
      m4seconds_t numseconds (m4int32_t sec, m4int32_t min, m4int32_t hour);

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%3779DEFB006F
      //## Role: M4ClHour2Second::m_numseconds%3779DEFB0071
      //## begin M4ClHour2Second::m_numseconds%3779DEFB0071.role preserve=no  protected: m4seconds_t {1 -> 1VHAPN}
      m4seconds_t m_numseconds;
      //## end M4ClHour2Second::m_numseconds%3779DEFB0071.role

    // Additional Protected Declarations
      //## begin M4ClHour2Second%3779DEFB001B.protected preserve=yes
      //## end M4ClHour2Second%3779DEFB001B.protected

  private:
    // Additional Private Declarations
      //## begin M4ClHour2Second%3779DEFB001B.private preserve=yes
      //## end M4ClHour2Second%3779DEFB001B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClHour2Second%3779DEFB001B.implementation preserve=yes
      //## end M4ClHour2Second%3779DEFB001B.implementation

};

//## begin M4ClHour2Second%3779DEFB001B.postscript preserve=yes

//==============================================================================
//
//   Definición clase M4ClTimeStamp,  que es la que accede a la BD
//
//==============================================================================

// lleva una cantidad de tiempo de segundos y de horas
// no cumplen ninguna condicion esa cantidad de segundos y horas
typedef double m4time_t;


//## end M4ClHour2Second%3779DEFB001B.postscript

//## begin M4ClTimeStamp%3779DEFB0088.preface preserve=yes
//## end M4ClTimeStamp%3779DEFB0088.preface

//## Class: M4ClTimeStamp%3779DEFB0088
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3779DEFB0089;m4return_t { -> }
//## Uses: <unnamed>%3779DEFB008A;m4seconds_t { -> }
//## Uses: <unnamed>%3779DEFB0095;m4pchar_t { -> }
//## Uses: <unnamed>%3779DEFB0098;M4ClString { -> }
//## Uses: <unnamed>%3779DEFB009C;m4time_t { -> }
//## Uses: <unnamed>%3779DEFB00C4;m4bool_t { -> }
//## Uses: <unnamed>%3779DEFB00F1;protected: m4dates_t { -> }

class M4_DECL_M4DATE  M4ClTimeStamp : public M4ClDate, //## Inherits: <unnamed>%3779DEFB00FB
    	public M4ClHour2Second  //## Inherits: <unnamed>%3779DEFB00FC
{
  //## begin M4ClTimeStamp%3779DEFB0088.initialDeclarations preserve=yes
  //## end M4ClTimeStamp%3779DEFB0088.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClTimeStamp%1774825004; C++
      //	////////////  constructores y destructores
      M4ClTimeStamp ()
        //## begin M4ClTimeStamp::M4ClTimeStamp%1774825004.hasinit preserve=no
        //## end M4ClTimeStamp::M4ClTimeStamp%1774825004.hasinit
        //## begin M4ClTimeStamp::M4ClTimeStamp%1774825004.initialization preserve=yes
        //## end M4ClTimeStamp::M4ClTimeStamp%1774825004.initialization
      {
        //## begin M4ClTimeStamp::M4ClTimeStamp%1774825004.body preserve=yes
 reset();
        //## end M4ClTimeStamp::M4ClTimeStamp%1774825004.body
      }

      //## Operation: M4ClTimeStamp%1026520756; C++
      M4ClTimeStamp (const M4ClTimeStamp& ai_TS)
        //## begin M4ClTimeStamp::M4ClTimeStamp%1026520756.hasinit preserve=no
        //## end M4ClTimeStamp::M4ClTimeStamp%1026520756.hasinit
        //## begin M4ClTimeStamp::M4ClTimeStamp%1026520756.initialization preserve=yes
        //## end M4ClTimeStamp::M4ClTimeStamp%1026520756.initialization
      {
        //## begin M4ClTimeStamp::M4ClTimeStamp%1026520756.body preserve=yes
 if (set( ai_TS) == M4_ERROR) reset(); 
        //## end M4ClTimeStamp::M4ClTimeStamp%1026520756.body
      }

      //## Operation: M4ClTimeStamp%128873400; C++
      M4ClTimeStamp (const m4pchar_t ai_pchar)
        //## begin M4ClTimeStamp::M4ClTimeStamp%128873400.hasinit preserve=no
        //## end M4ClTimeStamp::M4ClTimeStamp%128873400.hasinit
        //## begin M4ClTimeStamp::M4ClTimeStamp%128873400.initialization preserve=yes
        //## end M4ClTimeStamp::M4ClTimeStamp%128873400.initialization
      {
        //## begin M4ClTimeStamp::M4ClTimeStamp%128873400.body preserve=yes
 if (set(ai_pchar) == M4_ERROR) reset(); 
        //## end M4ClTimeStamp::M4ClTimeStamp%128873400.body
      }

      //## Operation: M4ClTimeStamp%1697046988; C++
      M4ClTimeStamp (const M4ClString& ai_pchar)
        //## begin M4ClTimeStamp::M4ClTimeStamp%1697046988.hasinit preserve=no
        //## end M4ClTimeStamp::M4ClTimeStamp%1697046988.hasinit
        //## begin M4ClTimeStamp::M4ClTimeStamp%1697046988.initialization preserve=yes
        //## end M4ClTimeStamp::M4ClTimeStamp%1697046988.initialization
      {
        //## begin M4ClTimeStamp::M4ClTimeStamp%1697046988.body preserve=yes
 if (set(ai_pchar) == M4_ERROR) reset(); 
        //## end M4ClTimeStamp::M4ClTimeStamp%1697046988.body
      }

      //## Operation: M4ClTimeStamp%-92911113; C++
      M4ClTimeStamp (const m4time_t& ai_lJul)
        //## begin M4ClTimeStamp::M4ClTimeStamp%-92911113.hasinit preserve=no
        //## end M4ClTimeStamp::M4ClTimeStamp%-92911113.hasinit
        //## begin M4ClTimeStamp::M4ClTimeStamp%-92911113.initialization preserve=yes
        //## end M4ClTimeStamp::M4ClTimeStamp%-92911113.initialization
      {
        //## begin M4ClTimeStamp::M4ClTimeStamp%-92911113.body preserve=yes
  if (set(ai_lJul) == M4_ERROR) reset(); 
        //## end M4ClTimeStamp::M4ClTimeStamp%-92911113.body
      }

    //## Destructor (specified)
      //## Operation: ~M4ClTimeStamp%-1814475445; C++
      ~M4ClTimeStamp ()
      {
        //## begin M4ClTimeStamp::~M4ClTimeStamp%-1814475445.body preserve=yes
 reset(); 
        //## end M4ClTimeStamp::~M4ClTimeStamp%-1814475445.body
      }


    //## Other Operations (specified)
      //## Operation: reset%-797734277; C++
      //	////////////  conversores de tipo
      m4return_t reset ()
      {
        //## begin M4ClTimeStamp::reset%-797734277.body preserve=yes
 M4ClDate::reset(); return M4ClHour2Second::reset();
        //## end M4ClTimeStamp::reset%-797734277.body
      }

      //## Operation: set%1618862349; C++
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const M4ClTimeStamp& ai_TS);

      //## Operation: set%1675089131; C++
      //	////////////  operaciones especificas  &}  puntero al objeto
      m4return_t set (const m4pchar_t ai_pcTS);

      //## Operation: set%-818293914; C++
      m4return_t set (const M4ClString& ai_pcTS);

      //## Operation: set%889756185; C++
      m4return_t set (const m4int32_t& ai_iyear, const m4int32_t& ai_imon, const m4int32_t& ai_iday, const m4int32_t& ai_ihour, const m4int32_t& ai_imin, const m4int32_t& ai_isec);

      //## Operation: set%914940060; C++
      m4return_t set (const m4time_t& ai_TS);

      //## Operation: now%547386774; C++
      m4return_t now ()
      {
        //## begin M4ClTimeStamp::now%547386774.body preserve=yes
 M4ClDate::now(); return M4ClHour2Second::now(); 
        //## end M4ClTimeStamp::now%547386774.body
      }

      //## Operation: operator =%-1828990391; C++
      //	////////////  asignaciones
      const M4ClTimeStamp& operator = (const M4ClTimeStamp& ai_Date)
      {
        //## begin M4ClTimeStamp::operator =%-1828990391.body preserve=yes
 set(ai_Date); return *this;
        //## end M4ClTimeStamp::operator =%-1828990391.body
      }

      //## Operation: operator =%773113626; C++
      const M4ClTimeStamp& operator = (const m4time_t& ai_Jul)
      {
        //## begin M4ClTimeStamp::operator =%773113626.body preserve=yes
 set(ai_Jul); return *this;
        //## end M4ClTimeStamp::operator =%773113626.body
      }

      //## Operation: operator =%-157667127; C++
      const M4ClTimeStamp& operator = (const m4pchar_t& ai_char)
      {
        //## begin M4ClTimeStamp::operator =%-157667127.body preserve=yes
set (ai_char); return *this;
        //## end M4ClTimeStamp::operator =%-157667127.body
      }

      //## Operation: operator =%-896585841; C++
      const M4ClTimeStamp& operator = (const M4ClString& ai_char)
      {
        //## begin M4ClTimeStamp::operator =%-896585841.body preserve=yes
set (ai_char); return *this;
        //## end M4ClTimeStamp::operator =%-896585841.body
      }

      //## Operation: get%-1093929978; C++
      m4return_t get (M4ClString& ao_pcTS);

      //## Operation: get%1430722701; C++
      m4return_t get (m4int32_t& ai_iyear, m4int32_t& ai_imon, m4int32_t& ai_iday, m4int32_t& ai_ihour, m4int32_t& ai_imin, m4int32_t& ai_isec);

      //## Operation: get%-1980885053; C++
      //	la cantidad de segundos
      m4return_t get (m4time_t& ao_TS);

      //## Operation: operator <%-760351685; C++
      //	///////////////  comparacion
      //	///////////////  comparacion
      m4bool_t operator < (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator >%-442288218; C++
      m4bool_t operator > (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator ==%-83595235; C++
      m4bool_t operator == (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator <=%2068151005; C++
      m4bool_t operator <= (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator >=%497264708; C++
      m4bool_t operator >= (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator !=%-343800387; C++
      m4bool_t operator != (const M4ClTimeStamp& ai_hora1);

      //## Operation: operator +%-1064328848; C++
      //	///////////////  operaciones
      m4time_t operator + (const m4time_t& ai_hora1);

      //## Operation: operator -%842843240; C++
      m4time_t operator - (const m4time_t& ai_hora1);

      //## Operation: operator +=%-1771464033; C++
      const m4time_t operator += (const m4time_t& ai_hora1);

      //## Operation: operator -=%1894276242; C++
      const m4time_t operator -= (const m4time_t& ai_hora1);

      //## Operation: operator M4ClString%854809655; C++
      operator M4ClString ()
      {
        //## begin M4ClTimeStamp::operator M4ClString%854809655.body preserve=yes
 M4ClString sTemp; get(sTemp); return sTemp; 
        //## end M4ClTimeStamp::operator M4ClString%854809655.body
      }

      //## Operation: add_seconds%325856352; C++
      m4return_t add_seconds (const m4int32_t& ai_num_seconds);

      //## Operation: add_minutes%622123393; C++
      m4return_t add_minutes (const m4int32_t& ai_num_minutes);

      //## Operation: add_hours%-600163017; C++
      m4return_t add_hours (const m4int32_t& ai_num_hours);

      //## Operation: sub_seconds%652988665; C++
      m4return_t sub_seconds (const m4int32_t& ai_num_seconds)
      {
        //## begin M4ClTimeStamp::sub_seconds%652988665.body preserve=yes
 return add_seconds( -ai_num_seconds); 
        //## end M4ClTimeStamp::sub_seconds%652988665.body
      }

      //## Operation: sub_minutes%1619221697; C++
      m4return_t sub_minutes (const m4int32_t& ai_num_minutes)
      {
        //## begin M4ClTimeStamp::sub_minutes%1619221697.body preserve=yes
 return add_minutes( -ai_num_minutes); 
        //## end M4ClTimeStamp::sub_minutes%1619221697.body
      }

      //## Operation: sub_hours%-769663738; C++
      m4return_t sub_hours (const m4int32_t& ai_num_hours)
      {
        //## begin M4ClTimeStamp::sub_hours%-769663738.body preserve=yes
 return add_hours( -ai_num_hours); 
        //## end M4ClTimeStamp::sub_hours%-769663738.body
      }

      //## Operation: operator m4time_t%1009411124; C++
      operator m4time_t ()
      {
        //## begin M4ClTimeStamp::operator m4time_t%1009411124.body preserve=yes
 m4time_t Tempellap;
		      daysec_timellap( m_numdays, m_numseconds, Tempellap);
		      return Tempellap; 	
        //## end M4ClTimeStamp::operator m4time_t%1009411124.body
      }

    // Additional Public Declarations
      //## begin M4ClTimeStamp%3779DEFB0088.public preserve=yes
      //## end M4ClTimeStamp%3779DEFB0088.public

  protected:

    //## Other Operations (specified)
      //## Operation: timellap_daysec%-304128742; C++
      //	=============================================================================
      //	=
      //
      //	Definición clase M4ClTimeStamp,  que es la que accede a la BD
      //
      //	=============================================================================
      //	=
      //	obtiene de un double los dias y los segundos
      m4return_t timellap_daysec (const m4time_t& ao_timellap, m4dates_t& ai_days, m4seconds_t& ai_seconds);

      //## Operation: daysec_timellap%1335809157; C++
      //	obtiene de unos dias y segundo un double
      m4return_t daysec_timellap (const m4dates_t& ai_days, const m4seconds_t& ai_seconds, m4time_t& ao_timellap);

    // Additional Protected Declarations
      //## begin M4ClTimeStamp%3779DEFB0088.protected preserve=yes
      //## end M4ClTimeStamp%3779DEFB0088.protected

  private:
    // Additional Private Declarations
      //## begin M4ClTimeStamp%3779DEFB0088.private preserve=yes
      //## end M4ClTimeStamp%3779DEFB0088.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClTimeStamp%3779DEFB0088.implementation preserve=yes
      //## end M4ClTimeStamp%3779DEFB0088.implementation

};

//## begin M4ClTimeStamp%3779DEFB0088.postscript preserve=yes
//## end M4ClTimeStamp%3779DEFB0088.postscript

//## begin M4ClCrono%3779DEFB00FD.preface preserve=yes
//## end M4ClCrono%3779DEFB00FD.preface

//## Class: M4ClCrono%3779DEFB00FD
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3779DEFB02F9;m4double_t { -> }
//## Uses: <unnamed>%377A00B203CB;M4ClTimeStamp { -> }

class M4_DECL_M4DATE  M4ClCrono 
{
  //## begin M4ClCrono%3779DEFB00FD.initialDeclarations preserve=yes
  //## end M4ClCrono%3779DEFB00FD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClCrono%-457563016; C++
      //	----------------------------------------------------------
      //	*******************M4Clcrono*******************************
      //	-----------------------------------------------------------
      //	---------------------------------------------------------
      //	*****************Constructores M4Clcrono********************
      //	------------------------------------------------------
      M4ClCrono (m4bool_t ai_bSelectUn = M4_FALSE, m4bool_t ai_bSelectModMed = M4_FALSE);

      //## Operation: M4ClCrono%544929149; C++
      //	// se ponen a false los dos parámetros, se usa el tipo m4int8_t para evitar
      //	problemas.
      M4ClCrono (M4ClCrono& ai_oCrono);

    //## Destructor (specified)
      //## Operation: ~M4ClCrono%-195274200; C++
      ~M4ClCrono ()
      {
        //## begin M4ClCrono::~M4ClCrono%-195274200.body preserve=yes
 ; 
        //## end M4ClCrono::~M4ClCrono%-195274200.body
      }


    //## Other Operations (specified)
      //## Operation: Start%392368292; C++
      //	---------------------------------------------------------
      //	********************Start******************************
      //	-------------------------------------------------------
      m4double_t Start ();

      //## Operation: Pause%38379244; C++
      //	Función que permite que se inicialice
      //	el contador con el tiempo en ese momento
      //	----------------------------------------------------------
      //	**********************Pause*******************************
      //	----------------------------------------------------------
      m4double_t Pause ();

      //## Operation: Stop%-2140867229; C++
      //	Permite conocer el tiempo en el
      //	momento que se la llama
      m4double_t Stop ();

      //## Operation: Reset%-968831580; C++
      //	Para el cronómetro
      //	------------------------------------------------------------
      //	******************Reset*************************************
      //	------------------------------------------------------------
      m4double_t Reset ();

    // Additional Public Declarations
      //## begin M4ClCrono%3779DEFB00FD.public preserve=yes
      //## end M4ClCrono%3779DEFB00FD.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClCrono%3779DEFB00FD.protected preserve=yes
      //## end M4ClCrono%3779DEFB00FD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lStartTimeP%3779DEFB02EC
      //## begin M4ClCrono::m_lStartTimeP%3779DEFB02EC.attr preserve=no  private: long {VA} 
      long m_lStartTimeP;
      //## end M4ClCrono::m_lStartTimeP%3779DEFB02EC.attr

      //## Attribute: m_lExtraTimeP%3779DEFB02F0
      //## begin M4ClCrono::m_lExtraTimeP%3779DEFB02F0.attr preserve=no  private: long {VA} 
      long m_lExtraTimeP;
      //## end M4ClCrono::m_lExtraTimeP%3779DEFB02F0.attr

    // Data Members for Associations

      //## Association: M4Date::<unnamed>%3779DEFB0263
      //## Role: M4ClCrono::m_bSelectUn%3779DEFB0265
      //## begin M4ClCrono::m_bSelectUn%3779DEFB0265.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bSelectUn;
      //## end M4ClCrono::m_bSelectUn%3779DEFB0265.role

      //## Association: M4Date::<unnamed>%3779DEFB0266
      //## Role: M4ClCrono::m_bSelectModMed%3779DEFB0268
      //## begin M4ClCrono::m_bSelectModMed%3779DEFB0268.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bSelectModMed;
      //## end M4ClCrono::m_bSelectModMed%3779DEFB0268.role

      //## Association: M4Date::<unnamed>%3779DEFB02E2
      //## Role: M4ClCrono::m_stStartTimeR%3779DEFB02E4
      //## begin M4ClCrono::m_stStartTimeR%3779DEFB02E4.role preserve=no  private: timeval {1 -> 1VHAPN}
      timeval m_stStartTimeR;
      //## end M4ClCrono::m_stStartTimeR%3779DEFB02E4.role

      //## Association: M4Date::<unnamed>%3779DEFB02ED
      //## Role: M4ClCrono::m_stExtraTimeR%3779DEFB02EF
      //## begin M4ClCrono::m_stExtraTimeR%3779DEFB02EF.role preserve=no  private: timeval {1 -> 1VHAPN}
      timeval m_stExtraTimeR;
      //## end M4ClCrono::m_stExtraTimeR%3779DEFB02EF.role

      //## Association: M4Date::<unnamed>%3779DEFB02F2
      //## Role: M4ClCrono::m_start%3779DEFB02F4
      //## begin M4ClCrono::m_start%3779DEFB02F4.role preserve=no  private: m4double_t {1 -> 1VHAPN}
      m4double_t m_start;
      //## end M4ClCrono::m_start%3779DEFB02F4.role

      //## Association: M4Date::<unnamed>%3779DEFB02F6
      //## Role: M4ClCrono::m_extra%3779DEFB02F8
      //## begin M4ClCrono::m_extra%3779DEFB02F8.role preserve=no  private: m4double_t {1 -> 1VHAPN}
      m4double_t m_extra;
      //## end M4ClCrono::m_extra%3779DEFB02F8.role

    // Additional Private Declarations
      //## begin M4ClCrono%3779DEFB00FD.private preserve=yes
      //## end M4ClCrono%3779DEFB00FD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClCrono%3779DEFB00FD.implementation preserve=yes
      //## end M4ClCrono%3779DEFB00FD.implementation

};

//## begin M4ClCrono%3779DEFB00FD.postscript preserve=yes
//## end M4ClCrono%3779DEFB00FD.postscript

//## begin M4ClCronoStatistic%3779DEFB02FE.preface preserve=yes
//## end M4ClCronoStatistic%3779DEFB02FE.preface

//## Class: M4ClCronoStatistic%3779DEFB02FE
//## Category: M4Date%3779DEF60096
//## Subsystem: M4Date::inc%3779DEF502DA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3779DEFB031E;m4bool_t { -> }
//## Uses: <unnamed>%3779DEFB0330;m4double_t { -> }
//## Uses: <unnamed>%3779DEFB0333;M4ClString { -> }

class M4_DECL_M4DATE  M4ClCronoStatistic : public M4ClCrono  //## Inherits: <unnamed>%3779DEFB0336
{
  //## begin M4ClCronoStatistic%3779DEFB02FE.initialDeclarations preserve=yes
  //## end M4ClCronoStatistic%3779DEFB02FE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4ClCronoStatistic%1999631873; C++
      //	----------------------------------------------------------
      //	****************M4ClCronoStatistic*************************
      //	------------------------------------------------------------
      //	-------------------------------------------------------
      //	**************Constructor M4Clcronostatistic***********
      //	-------------------------------------------------------
      M4ClCronoStatistic (m4bool_t ai_bSelectUn = M4_FALSE, m4bool_t ai_bSelectModMed = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~M4ClCronoStatistic%-1141200579; C++
      //	----------------------------------------------------------
      //	****************Destructor ClCrnoStatistic****************
      //	-----------------------------------------------------------
      ~M4ClCronoStatistic ();


    //## Other Operations (specified)
      //## Operation: Start%-659517363; C++
      //	-------------------------------------------------------
      //	**********************Start****************************
      //	-------------------------------------------------------
      void Start ();

      //## Operation: Pause%1175962161; C++
      //	-----------------------------------------------------------
      //	**********************Pause********************************
      //	------------------------------------------------------------
      m4double_t Pause ();

      //## Operation: Stop%-2113383620; C++
      //	-----------------------------------------------------------
      //	***********************Stop**********************************
      //	-------------------------------------------------------------
      m4double_t Stop ();

      //## Operation: GetAverage%-204437271; C++
      //	-------------------------------------------------------------
      //	*******************GetAverage*********************************
      //	--------------------------------------------------------------
      m4double_t GetAverage ();

      //## Operation: GetTotal%-918977411; C++
      //	Calcula los tiempos medios
      //	-----------------------------------------------------------
      //	********************GetTotal*********************************
      //	------------------------------------------------------------
      m4double_t GetTotal ();

      //## Operation: GetLastStartTime%783865178; C++
      //	Devuelve el total de los tiempos
      //	------------------------------------------------------------
      //	********************GetLastStartTime**************************
      //	-----------------------------------------------------------
      m4double_t GetLastStartTime (M4ClString& lst);

    // Additional Public Declarations
      //## begin M4ClCronoStatistic%3779DEFB02FE.public preserve=yes
      //## end M4ClCronoStatistic%3779DEFB02FE.public

  protected:
    // Additional Protected Declarations
      //## begin M4ClCronoStatistic%3779DEFB02FE.protected preserve=yes
      //## end M4ClCronoStatistic%3779DEFB02FE.protected

  private:
    // Data Members for Associations

      //## Association: M4Date::<unnamed>%3779DEFB0327
      //## Role: M4ClCronoStatistic::m_ipulsaciones%3779DEFB0329
      //## begin M4ClCronoStatistic::m_ipulsaciones%3779DEFB0329.role preserve=no  private: m4seconds_t {1 -> 1VHAPN}
      m4seconds_t m_ipulsaciones;
      //## end M4ClCronoStatistic::m_ipulsaciones%3779DEFB0329.role

      //## Association: M4Date::<unnamed>%3779DEFB032A
      //## Role: M4ClCronoStatistic::m_dttotal%3779DEFB032C
      //## begin M4ClCronoStatistic::m_dttotal%3779DEFB032C.role preserve=no  private: m4double_t {1 -> 1VHAPN}
      m4double_t m_dttotal;
      //## end M4ClCronoStatistic::m_dttotal%3779DEFB032C.role

      //## Association: M4Date::<unnamed>%3779DEFB032D
      //## Role: M4ClCronoStatistic::m_lastStart%3779DEFB032F
      //## begin M4ClCronoStatistic::m_lastStart%3779DEFB032F.role preserve=no  private: M4ClTimeStamp {1 -> 1VHAPN}
      M4ClTimeStamp m_lastStart;
      //## end M4ClCronoStatistic::m_lastStart%3779DEFB032F.role

    // Additional Private Declarations
      //## begin M4ClCronoStatistic%3779DEFB02FE.private preserve=yes
      //## end M4ClCronoStatistic%3779DEFB02FE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4ClCronoStatistic%3779DEFB02FE.implementation preserve=yes
      //## end M4ClCronoStatistic%3779DEFB02FE.implementation

};

//## begin M4ClCronoStatistic%3779DEFB02FE.postscript preserve=yes
//## end M4ClCronoStatistic%3779DEFB02FE.postscript

//## begin module%3779DEF60046.epilog preserve=yes
//## end module%3779DEF60046.epilog


#endif
