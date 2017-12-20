//## begin module%3912A1F501C1.cm preserve=no
//## end module%3912A1F501C1.cm

//## begin module%3912A1F501C1.cp preserve=no
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
//## end module%3912A1F501C1.cp

//## Module: m4regimpl%3912A1F501C1; Package specification
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Source file: b:\m4objreg\inc\m4regimpl.hpp

#ifndef __M4REGIMPL_HPP__
#define __M4REGIMPL_HPP__ 1

//## begin module%3912A1F501C1.additionalIncludes preserve=no
//## end module%3912A1F501C1.additionalIncludes

//## begin module%3912A1F501C1.includes preserve=yes

#include <stdio.h>
#include "m4objreg_dll.hpp"
#include "m4regdef.hpp"		// Defines
#include "m4types.hpp"
#include "m4cryptc.hpp"

/*
	This is the "final" relation between classes :


					ClVMBaseEnv
					  /	    \
			 		 /		  \	
			  		/	        \
	Final:	ClM4ObjRegServer	ClM4ObjRegClient
									|
									|
								ClM4ObjRegSM
*/


// ============= clase base con propiedades comunes de registry ======================
//class  M4_DECL_M4OBJREG ClM4ObjRegBase : public ClVMBaseEnv
//## end module%3912A1F501C1.includes

//## begin module%3912A1F501C1.declarations preserve=no
//## end module%3912A1F501C1.declarations

//## begin module%3912A1F501C1.additionalDeclarations preserve=yes
//## end module%3912A1F501C1.additionalDeclarations


//## begin ClVMBaseEnv%3912A1F501F4.preface preserve=yes
//## end ClVMBaseEnv%3912A1F501F4.preface

//## Class: ClVMBaseEnv%3912A1F501F4; Abstract
//	This is the "final" relation between classes :
//
//
//	ClVMBaseEnv
//	/	    			 		 /		  \
//	/	        	Final:	ClM4ObjRegServer	ClM4ObjRegClient
//	|
//	|
//	ClM4ObjRegSM
//
//	============= clase base con propiedades comunes de registry
//	======================
//	class  M4_DECL_M4OBJREG ClM4ObjRegBase : public ClVMBaseEnv
//## Category: m4objreg%3912A1F501F3
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJREG ClVMBaseEnv 
{
  //## begin ClVMBaseEnv%3912A1F501F4.initialDeclarations preserve=yes
  //## end ClVMBaseEnv%3912A1F501F4.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClVMBaseEnv%-298358039; C++
      //	-- {AddDecl: 108} region.unprotectedFunction [3888..3930]
      virtual ~ClVMBaseEnv (void );


    //## Other Operations (specified)
      //## Operation: ReadWorkingDirectory%-472850620; C++
      //	============= Metodos static para Propiedades Especiales de Registry
      //	======================
      //	Lectura inicial del WorkDir para el Inicializador
      //	Entrada:	ai_pcPath = Rama Client o Server del Registry
      //	ai_iBufLen = Longitud del buffer
      //	Salida:	aio_acWorkingDirectory = Buffer con el registry leido
      //	return = M4_SUCCESS o M4_ERROR
      static m4return_t ReadWorkingDirectory (m4pchar_t ai_pcPath, m4pchar_t aio_acWorkingDirectory, m4int32_t ai_iBufLen, m4bool_t ai_bFullPath = M4_TRUE);

      //## Operation: GetIntegerTimeUnit%61424462; C++
      m4uint8_t GetIntegerTimeUnit (void ) const
      {
        //## begin ClVMBaseEnv::GetIntegerTimeUnit%61424462.body preserve=yes
        return( m_iTimeUnit ) ;
        //## end ClVMBaseEnv::GetIntegerTimeUnit%61424462.body
      }

      //## Operation: GetDateTimeUnit%1054274452; C++
      m4double_t GetDateTimeUnit (void ) const
      {
        //## begin ClVMBaseEnv::GetDateTimeUnit%1054274452.body preserve=yes
        return( m_dTimeUnit ) ;
        //## end ClVMBaseEnv::GetDateTimeUnit%1054274452.body
      }

      //## Operation: GetVersion%1186524157; C++
      m4pcchar_t GetVersion (void ) const
      {
        //## begin ClVMBaseEnv::GetVersion%1186524157.body preserve=yes
        return( m_acVersion ) ;
        //## end ClVMBaseEnv::GetVersion%1186524157.body
      }

      //## Operation: GetWorkingDirectory%-536460998; C++
      m4pcchar_t GetWorkingDirectory (void ) const
      {
        //## begin ClVMBaseEnv::GetWorkingDirectory%-536460998.body preserve=yes
        return( m_acWorkingDirectory ) ;
        //## end ClVMBaseEnv::GetWorkingDirectory%-536460998.body
      }

      //## Operation: GetDebugDirectory%-645679437; C++
      m4pcchar_t GetDebugDirectory (void ) const
      {
        //## begin ClVMBaseEnv::GetDebugDirectory%-645679437.body preserve=yes
        return( m_acDebugDirectory ) ;
        //## end ClVMBaseEnv::GetDebugDirectory%-645679437.body
      }

      //## Operation: GetTempDirectory%-470093833; C++
      m4pcchar_t GetTempDirectory (void ) const
      {
        //## begin ClVMBaseEnv::GetTempDirectory%-470093833.body preserve=yes
        return( m_acTempDirectory ) ;
        //## end ClVMBaseEnv::GetTempDirectory%-470093833.body
      }

      //## Operation: GetCacheDirectory%2096121747; C++
      m4pcchar_t GetCacheDirectory (void ) const
      {
        //## begin ClVMBaseEnv::GetCacheDirectory%2096121747.body preserve=yes
        return( m_acCacheDirectory ) ;
        //## end ClVMBaseEnv::GetCacheDirectory%2096121747.body
      }

      //## Operation: GetMDDiskCache%811450535; C++
      m4uint8_t GetMDDiskCache (void ) const
      {
        //## begin ClVMBaseEnv::GetMDDiskCache%811450535.body preserve=yes
        return( m_iMDDiskCache ) ;
        //## end ClVMBaseEnv::GetMDDiskCache%811450535.body
      }

      //## Operation: GetMDCacheMaxMemory%-1388592860; C++
      m4uint32_t GetMDCacheMaxMemory (void ) const
      {
        //## begin ClVMBaseEnv::GetMDCacheMaxMemory%-1388592860.body preserve=yes
        return( m_iMDCacheMaxMemory ) ;
        //## end ClVMBaseEnv::GetMDCacheMaxMemory%-1388592860.body
      }

      //## Operation: GetMDCacheMaxNumObjects%810022285; C++
      m4uint32_t GetMDCacheMaxNumObjects (void ) const
      {
        //## begin ClVMBaseEnv::GetMDCacheMaxNumObjects%810022285.body preserve=yes
        return( m_iMDCacheMaxNumObjects ) ;
        //## end ClVMBaseEnv::GetMDCacheMaxNumObjects%810022285.body
      }

      //## Operation: GetMDCacheRefreshRatio%774803057; C++
      m4uint32_t GetMDCacheRefreshRatio (void ) const
      {
        //## begin ClVMBaseEnv::GetMDCacheRefreshRatio%774803057.body preserve=yes
        return( m_iMDCacheRefreshRatio ) ;
        //## end ClVMBaseEnv::GetMDCacheRefreshRatio%774803057.body
      }

      //## Operation: GetMDCacheExpiryRatio%-572135795; C++
      m4double_t GetMDCacheExpiryRatio (void ) const
      {
        //## begin ClVMBaseEnv::GetMDCacheExpiryRatio%-572135795.body preserve=yes
        return( m_iMDCacheExpiryRatio ) ;
        //## end ClVMBaseEnv::GetMDCacheExpiryRatio%-572135795.body
      }

      //## Operation: GetMDCacheDefaultMaxPeriod%-1989105645; C++
      m4date_t GetMDCacheDefaultMaxPeriod (void ) const
      {
        //## begin ClVMBaseEnv::GetMDCacheDefaultMaxPeriod%-1989105645.body preserve=yes
        return( m_iMDCacheDefaultMaxPeriod ) ;
        //## end ClVMBaseEnv::GetMDCacheDefaultMaxPeriod%-1989105645.body
      }

      //## Operation: GetDataDiskCache%-56288781; C++
      m4uint8_t GetDataDiskCache (void ) const
      {
        //## begin ClVMBaseEnv::GetDataDiskCache%-56288781.body preserve=yes
        return( m_iDataDiskCache ) ;
        //## end ClVMBaseEnv::GetDataDiskCache%-56288781.body
      }

      //## Operation: GetDataCacheMaxMemory%1370992910; C++
      m4uint32_t GetDataCacheMaxMemory (void ) const
      {
        //## begin ClVMBaseEnv::GetDataCacheMaxMemory%1370992910.body preserve=yes
        return( m_iDataCacheMaxMemory ) ;
        //## end ClVMBaseEnv::GetDataCacheMaxMemory%1370992910.body
      }

      //## Operation: GetDataCacheMaxNumObjects%-1510124427; C++
      m4uint32_t GetDataCacheMaxNumObjects (void ) const
      {
        //## begin ClVMBaseEnv::GetDataCacheMaxNumObjects%-1510124427.body preserve=yes
        return( m_iDataCacheMaxNumObjects ) ;
        //## end ClVMBaseEnv::GetDataCacheMaxNumObjects%-1510124427.body
      }

      //## Operation: GetDataCacheRefreshRatio%931372986; C++
      m4uint32_t GetDataCacheRefreshRatio (void ) const
      {
        //## begin ClVMBaseEnv::GetDataCacheRefreshRatio%931372986.body preserve=yes
        return( m_iDataCacheRefreshRatio ) ;
        //## end ClVMBaseEnv::GetDataCacheRefreshRatio%931372986.body
      }

      //## Operation: GetDataCacheExpiryRatio%1766964384; C++
      m4double_t GetDataCacheExpiryRatio (void ) const
      {
        //## begin ClVMBaseEnv::GetDataCacheExpiryRatio%1766964384.body preserve=yes
        return( m_iDataCacheExpiryRatio ) ;
        //## end ClVMBaseEnv::GetDataCacheExpiryRatio%1766964384.body
      }

      //## Operation: GetDataCacheDefaultMaxPeriod%2104052278; C++
      m4date_t GetDataCacheDefaultMaxPeriod (void ) const
      {
        //## begin ClVMBaseEnv::GetDataCacheDefaultMaxPeriod%2104052278.body preserve=yes
        return( m_iDataCacheDefaultMaxPeriod ) ;
        //## end ClVMBaseEnv::GetDataCacheDefaultMaxPeriod%2104052278.body
      }

      //## Operation: GetSttEnable%-1690462815; C++
      m4uint8_t GetSttEnable (void ) const
      {
        //## begin ClVMBaseEnv::GetSttEnable%-1690462815.body preserve=yes
		return( 0 );
        //## end ClVMBaseEnv::GetSttEnable%-1690462815.body
      }

      //## Operation: GetSystemDebugEnable%-1970470590; C++
      m4uint8_t GetSystemDebugEnable (void ) const
      {
        //## begin ClVMBaseEnv::GetSystemDebugEnable%-1970470590.body preserve=yes
		return( m_iSystemDebugEnable ) ;
        //## end ClVMBaseEnv::GetSystemDebugEnable%-1970470590.body
      }

		//Detalle de trazas en ldbinsp.
	  m4uint32_t	GetSystemDebugDetailLevel( void ) const
	  {
		return( m_iSystemDebugDetailLevel ) ;
	  }

      //## Operation: GetUserLicense%-656782983; C++
      m4pcchar_t GetUserLicense (void ) const
      {
        //## begin ClVMBaseEnv::GetUserLicense%-656782983.body preserve=yes
		return( m_acUserLicence ) ;
        //## end ClVMBaseEnv::GetUserLicense%-656782983.body
      }

      //## Operation: SetSystemDebugEnable%1127170907; C++
      void SetSystemDebugEnable (m4uint8_t ai_iSystemDebugEnable)
      {
        //## begin ClVMBaseEnv::SetSystemDebugEnable%1127170907.body preserve=yes
		m_iSystemDebugEnable = ai_iSystemDebugEnable ;
        //## end ClVMBaseEnv::SetSystemDebugEnable%1127170907.body
      }

		//Detalle de trazas en ldbinsp.
	  void	SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel)
	  {
		m_iSystemDebugDetailLevel = ai_uiSystemDebugDetailLevel;
	  }

	  m4uint8_t GetUseFieldUnicodeTypes( void ) const
	  {
		return( m_iUseFieldUnicodeTypes );
	  }

	  m4uint8_t GetSMCacheRefresh( void ) const
	  {
		return( m_iSMCacheRefresh );
	  }

      //## Operation: GetConnectionString%1127930383; C++
      m4pcchar_t GetConnectionString (void) const
      {
        //## begin ClVMBaseEnv::GetConnectionString%1127930383.body preserve=yes
		return m_acConnectionString;
        //## end ClVMBaseEnv::GetConnectionString%1127930383.body
      }

      //## Operation: GetDBPassword%-965609599; C++
      m4pcchar_t GetDBPassword (void) const
      {
        //## begin ClVMBaseEnv::GetDBPassword%-965609599.body preserve=yes
		return m_acRegDBPwd;
        //## end ClVMBaseEnv::GetDBPassword%-965609599.body
      }

      //## Operation: GetDataBase%1336545684; C++
      m4char_t GetDataBase (void) const
      {
        //## begin ClVMBaseEnv::GetDataBase%1336545684.body preserve=yes
		return m_DataBase;
        //## end ClVMBaseEnv::GetDataBase%1336545684.body
      }

      //## Operation: GetMaxNumConn%811076172; C++
      m4int16_t GetMaxNumConn (void) const
      {
        //## begin ClVMBaseEnv::GetMaxNumConn%811076172.body preserve=yes
		return( m_numConn );
        //## end ClVMBaseEnv::GetMaxNumConn%811076172.body
      }

      //## Operation: GetBufferSize%-2111655230; C++
      m4int32_t GetBufferSize (void) const
      {
        //## begin ClVMBaseEnv::GetBufferSize%-2111655230.body preserve=yes
		return( m_iBufferSize );
        //## end ClVMBaseEnv::GetBufferSize%-2111655230.body
      }

      //## Operation: GetMultipleFilesTrace%-538918055; C++
      m4bool_t GetMultipleFilesTrace (void) const
      {
        //## begin ClVMBaseEnv::GetMultipleFilesTrace%-538918055.body preserve=yes
		return( m_bMultFiles );
        //## end ClVMBaseEnv::GetMultipleFilesTrace%-538918055.body
      }

      //## Operation: GetMaxNumCursor%859095675; C++
      m4int16_t GetMaxNumCursor (void) const
      {
        //## begin ClVMBaseEnv::GetMaxNumCursor%859095675.body preserve=yes
		return( m_numCursor );
        //## end ClVMBaseEnv::GetMaxNumCursor%859095675.body
      }

      //## Operation: GetMaxNumStmtCache%287454223; C++
      m4int16_t GetMaxNumStmtCache (void) const
      {
        //## begin ClVMBaseEnv::GetMaxNumStmtCache%287454223.body preserve=yes
		return( m_numStmtCache );
        //## end ClVMBaseEnv::GetMaxNumStmtCache%287454223.body
      }

      //## Operation: GetMaxNumObject%960827698; C++
      m4int16_t GetMaxNumObject (void) const
      {
        //## begin ClVMBaseEnv::GetMaxNumObject%960827698.body preserve=yes
		return( m_numObject );
        //## end ClVMBaseEnv::GetMaxNumObject%960827698.body
      }

      //## Operation: GetMaxNumObjectTrans%-1135415868; C++
      m4int16_t GetMaxNumObjectTrans (void) const
      {
        //## begin ClVMBaseEnv::GetMaxNumObjectTrans%-1135415868.body preserve=yes
		return( m_numObjectTrans );
        //## end ClVMBaseEnv::GetMaxNumObjectTrans%-1135415868.body
      }

      //## Operation: GetIsolationLevel%-321423889; C++
      m4int8_t GetIsolationLevel (void) const
      {
        //## begin ClVMBaseEnv::GetIsolationLevel%-321423889.body preserve=yes
		return( m_eILevel );
        //## end ClVMBaseEnv::GetIsolationLevel%-321423889.body
      }

      //## Operation: GetConExec%1536296868; C++
      m4pcchar_t GetConExec (void)
      {
        //## begin ClVMBaseEnv::GetConExec%1536296868.body preserve=yes
		return (m_acConExec);
        //## end ClVMBaseEnv::GetConExec%1536296868.body
      }

      //## Operation: GetTempConnString%-1206637786; C++
      m4pcchar_t GetTempConnString (void)
      {
        //## begin ClVMBaseEnv::GetTempConnString%-1206637786.body preserve=yes
		return (m_acTempConnString);
        //## end ClVMBaseEnv::GetTempConnString%-1206637786.body
      }

      //## Operation: GetLDBDiskCache%-1087613097; C++
      m4uint8_t GetLDBDiskCache (void) const
      {
        //## begin ClVMBaseEnv::GetLDBDiskCache%-1087613097.body preserve=yes
        return( m_iLDBDiskCache ) ;
        //## end ClVMBaseEnv::GetLDBDiskCache%-1087613097.body
      }

      //## Operation: GetMaxLDBInspSize%-402910063; C++
      m4int32_t GetMaxLDBInspSize (void) const
      {
        //## begin ClVMBaseEnv::GetMaxLDBInspSize%-402910063.body preserve=yes
        return( m_iMaxLDBInspSize ) ;
        //## end ClVMBaseEnv::GetMaxLDBInspSize%-402910063.body
      }

      //## Operation: GetDefaultCSTimeOut%850577700; C++
      virtual m4uint32_t GetDefaultCSTimeOut (void) const
      {
        //## begin ClVMBaseEnv::GetDefaultCSTimeOut%850577700.body preserve=yes
		  return 0; 
        //## end ClVMBaseEnv::GetDefaultCSTimeOut%850577700.body
      }

      //## Operation: GetNextCSTimeOut%42212808; C++
      virtual m4uint32_t GetNextCSTimeOut (void) const
      {
        //## begin ClVMBaseEnv::GetNextCSTimeOut%42212808.body preserve=yes
		  return 0; 
        //## end ClVMBaseEnv::GetNextCSTimeOut%42212808.body
      }

      //## Operation: GetGenCSTimeOut%-658843716; C++
      virtual m4uint32_t GetGenCSTimeOut (void) const
      {
        //## begin ClVMBaseEnv::GetGenCSTimeOut%-658843716.body preserve=yes
		  return 0; 
        //## end ClVMBaseEnv::GetGenCSTimeOut%-658843716.body
      }

      //## Operation: SetDefaultCSTimeOut%1988830628; C++
      virtual void SetDefaultCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClVMBaseEnv::SetDefaultCSTimeOut%1988830628.body preserve=yes
        //## end ClVMBaseEnv::SetDefaultCSTimeOut%1988830628.body
      }

      //## Operation: SetNextCSTimeOut%1326857287; C++
      virtual void SetNextCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClVMBaseEnv::SetNextCSTimeOut%1326857287.body preserve=yes
        //## end ClVMBaseEnv::SetNextCSTimeOut%1326857287.body
      }

      //## Operation: SetGenCSTimeOut%-1394553227; C++
      virtual void SetGenCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClVMBaseEnv::SetGenCSTimeOut%-1394553227.body preserve=yes
        //## end ClVMBaseEnv::SetGenCSTimeOut%-1394553227.body
      }

      //## Operation: ResetNextCSTimeOut%136832124; C++
      virtual void ResetNextCSTimeOut (void)
      {
        //## begin ClVMBaseEnv::ResetNextCSTimeOut%136832124.body preserve=yes
        //## end ClVMBaseEnv::ResetNextCSTimeOut%136832124.body
      }

      //## Operation: GetCsTraceLevel%480511590; C++
      virtual m4int8_t GetCsTraceLevel (void) const
      {
        //## begin ClVMBaseEnv::GetCsTraceLevel%480511590.body preserve=yes
		  return( 0 ) ; 
        //## end ClVMBaseEnv::GetCsTraceLevel%480511590.body
      }

      virtual m4int8_t GetVbTraceLevel (void) const
      {
		  return( 0 ) ; 
      }

      //## Operation: GetCsTraceCacheOperations%707066207; C++
      virtual m4bool_t GetCsTraceCacheOperations (void) const
      {
        //## begin ClVMBaseEnv::GetCsTraceCacheOperations%707066207.body preserve=yes
		  return( M4_FALSE ) ; 
        //## end ClVMBaseEnv::GetCsTraceCacheOperations%707066207.body
      }

      //## Operation: GetSmOltp%-910077738; C++
      virtual m4uint8_t GetSmOltp (void) const
      {
        //## begin ClVMBaseEnv::GetSmOltp%-910077738.body preserve=yes
		  return 0; 
        //## end ClVMBaseEnv::GetSmOltp%-910077738.body
      }

      //## Operation: Init%-844002019; C++
      //	-- {AddDecl: 69} region.unprotectedFunction [9930..9981]
      virtual m4return_t Init (void) = 0;

      //## Operation: IsInit%-649483335; C++
      virtual m4bool_t IsInit () const
      {
        //## begin ClVMBaseEnv::IsInit%-649483335.body preserve=yes
		  return m_bInit; 
        //## end ClVMBaseEnv::IsInit%-649483335.body
      }

      //## Operation: GetLogonDefaultMechanism%960822800
      virtual m4int32_t GetLogonDefaultMechanism ();

      //## Operation: GetLogonDefaultMechanismDllName%960822801
      virtual m4pcchar_t GetLogonDefaultMechanismDllName ();

      //## Operation: GetAuthenticateDllByMechanism%960822802
      virtual m4pcchar_t GetAuthenticateDllByMechanism ();

      //## Operation: GetShowData%969966573
      m4uint8_t GetShowData ();

      //## Operation: SetShowData%969966574
      void SetShowData (m4uint8_t ai_iShowData);

    // Additional Public Declarations
      //## begin ClVMBaseEnv%3912A1F501F4.public preserve=yes
      m4uint32_t GetObjDataCacheMaxMemory(void) const
      {
		  return m_iObjDataCacheMaxMemory;
      }

      m4uint32_t GetObjDataCacheMaxNumObjects(void) const
      {
		  return m_iObjDataCacheMaxNumObjects;
      }

      m4pcchar_t ClVMBaseEnv::GetDateSeparator(void) const
      {
		  return m_acDateSeparator;
      }

      m4pcchar_t ClVMBaseEnv::GetJavaHome(void) const
      {
		  return m_pcJavaHome;
      }

      m4pcchar_t ClVMBaseEnv::GetJavaClassPath(void) const
      {
		  return m_pcJavaClassPath;
      }

      m4pcchar_t ClVMBaseEnv::GetJavaArguments(void) const
      {
		  return m_pcJavaArguments;
      }

      m4pcchar_t ClVMBaseEnv::GetJavaTrace(void) const
      {
		  return m_pcJavaTrace;
      }

      m4bool_t ClVMBaseEnv::GetJavaAllowInProcess(void) const
      {
		  return m_bJavaAllowInProcess;
      }

      m4uint32_t ClVMBaseEnv::GetJavaTimeout(void) const
      {
		  return m_iJavaTimeout;
      }

      //## end ClVMBaseEnv%3912A1F501F4.public

      //## Operation: ReadRegistry%969966575
      static m4return_t ReadRegistry (m4pchar_t ai_pPath, m4pchar_t ai_pKey, m4return_t ai_iShowError, m4int32_t ai_iMinValue, m4int32_t ai_iMaxValue, m4int32_t ai_iDefValue, m4int32_t& ao_iValue);

  protected:
    //## Constructors (specified)
      //## Operation: ClVMBaseEnv%572991009; C++
      //	-- {AddDecl: 107} region.unprotectedFunction [3731..3887]
      //	============= clase base con Propiedades Comunes de Registry
      //	======================
      ClVMBaseEnv ();


    //## Other Operations (specified)
      //## Operation: Init%1511868948; C++
      //	Lectura del Registry
      //	-- {AddDecl: 109} region.unprotectedFunction [3931..4277]
      virtual m4return_t Init (m4pchar_t ai_pcPath);

      //## Operation: ReadRegistry%215321743; C++
      //	Lectura del Registry de la Cache
      //	-- {AddDecl: 114} region.unprotectedFunction [14001..24447]
      //	Lectura de todo el registry comun
      m4return_t ReadRegistry (m4pchar_t ai_pcPath);

      //## Operation: ReadCacheRegistry%1987159710; C++
      //	Path base del registro
      //	-- {AddDecl: 113} region.unprotectedFunction [6187..14000]
      //	Leemos el Registry de la Cache
      m4return_t ReadCacheRegistry (m4pchar_t ai_pcPath);

      //## Operation: SetPath%237450391; C++
      //	-- {AddDecl: 111} region.unprotectedFunction [5685..6092]
      m4return_t SetPath (m4pchar_t ai_pcPathBase);

      //## Operation: GetPath%-1437626576; C++
      //	-- {AddDecl: 112} region.unprotectedFunction [6093..6186]
      m4pchar_t GetPath ();

      //## Operation: Reset%-305520925; C++
      //	Reset object state
      //	-- {AddDecl: 110} region.unprotectedFunction [4278..5684]
      void Reset ();

    // Data Members for Class Attributes

      //## Attribute: m_acDefaultDllMechanism%3951EB1300DC
      //## begin ClVMBaseEnv::m_acDefaultDllMechanism%3951EB1300DC.attr preserve=no  protected: m4char_t {UA} 
      m4char_t m_acDefaultDllMechanism[ M4CH_MAX_TEMP_DIRECTORY + 1 ];
      //## end ClVMBaseEnv::m_acDefaultDllMechanism%3951EB1300DC.attr

      //## Attribute: m_iMechanism%3951ECB0000D
      //## begin ClVMBaseEnv::m_iMechanism%3951ECB0000D.attr preserve=no  protected: m4int32_t {UA} -1
      m4int32_t m_iMechanism;
      //## end ClVMBaseEnv::m_iMechanism%3951ECB0000D.attr

    // Data Members for Associations

      //## Association: m4objreg::<unnamed>%3912A1F50280
      //## Role: ClVMBaseEnv::m_bInit%3912A1F5029D
      //## begin ClVMBaseEnv::m_bInit%3912A1F5029D.role preserve=no  protected: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bInit;
      //## end ClVMBaseEnv::m_bInit%3912A1F5029D.role

      //## Association: m4objreg::<unnamed>%3912A1F5029F
      //## Role: ClVMBaseEnv::m_acTempDirectory%3912A1F502A1
      //	Temporal Dir:	CVM\TempDir
      //## begin ClVMBaseEnv::m_acTempDirectory%3912A1F502A1.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acTempDirectory[ M4CH_MAX_TEMP_DIRECTORY + 1 ];
      //## end ClVMBaseEnv::m_acTempDirectory%3912A1F502A1.role

      //## Association: m4objreg::<unnamed>%3912A1F502B1
      //## Role: ClVMBaseEnv::m_acVersion%3912A1F502B3
      //	Version:			CVM\Version
      //## begin ClVMBaseEnv::m_acVersion%3912A1F502B3.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acVersion[ M4CH_MAX_VERSION + 1 ];
      //## end ClVMBaseEnv::m_acVersion%3912A1F502B3.role

      //## Association: m4objreg::<unnamed>%3912A1F502B4
      //## Role: ClVMBaseEnv::m_acWorkingDirectory%3912A1F502B6
      //	Working	Dir:	CVM\WorkDir
      //	** CVM\CACHE **
      //## begin ClVMBaseEnv::m_acWorkingDirectory%3912A1F502B6.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acWorkingDirectory[M4CH_MAX_WORKING_DIRECTORY + 1 ];
      //## end ClVMBaseEnv::m_acWorkingDirectory%3912A1F502B6.role

      //## Association: m4objreg::<unnamed>%3912A1F502B7
      //## Role: ClVMBaseEnv::m_acCacheDirectory%3912A1F502B9
      //	Cache	Dir:	CVM\CACHE\CacheDir
      //	Caches de Datos
      //## begin ClVMBaseEnv::m_acCacheDirectory%3912A1F502B9.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acCacheDirectory[M4CH_MAX_WORKING_DIRECTORY + 1 ];
      //## end ClVMBaseEnv::m_acCacheDirectory%3912A1F502B9.role

      //## Association: m4objreg::<unnamed>%3912A1F502BB
      //## Role: ClVMBaseEnv::m_iDataCacheDefaultMaxPeriod%3912A1F502BD
      //	Max.Period:	CVM\CACHE\DataCacheDefaultMaxPeriod
      //## begin ClVMBaseEnv::m_iDataCacheDefaultMaxPeriod%3912A1F502BD.role preserve=no  protected: m4date_t {1 -> 1VHAPN}
      m4date_t m_iDataCacheDefaultMaxPeriod;
      //## end ClVMBaseEnv::m_iDataCacheDefaultMaxPeriod%3912A1F502BD.role

      //## Association: m4objreg::<unnamed>%3912A1F502BF
      //## Role: ClVMBaseEnv::m_iDataCacheExpiryRatio%3912A1F502C1
      //	Caducidad:	CVM\CACHE\DataCacheExpiryRatio
      //## begin ClVMBaseEnv::m_iDataCacheExpiryRatio%3912A1F502C1.role preserve=no  protected: m4double_t {1 -> 1VHAPN}
      m4double_t m_iDataCacheExpiryRatio;
      //## end ClVMBaseEnv::m_iDataCacheExpiryRatio%3912A1F502C1.role

      //## Association: m4objreg::<unnamed>%3912A1F502C3
      //## Role: ClVMBaseEnv::m_iDataCacheMaxMemory%3912A1F502C5
      //	Memoria:		CVM\CACHE\DataCacheMaxMemory
      //## begin ClVMBaseEnv::m_iDataCacheMaxMemory%3912A1F502C5.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iDataCacheMaxMemory;
      //## end ClVMBaseEnv::m_iDataCacheMaxMemory%3912A1F502C5.role

      //## Association: m4objreg::<unnamed>%3912A1F502C6
      //## Role: ClVMBaseEnv::m_iDataCacheMaxNumObjects%3912A1F502C8
      //	No.Objetos:	CVM\CACHE\DataCacheMaxNumObjects
      //## begin ClVMBaseEnv::m_iDataCacheMaxNumObjects%3912A1F502C8.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iDataCacheMaxNumObjects;
      //## end ClVMBaseEnv::m_iDataCacheMaxNumObjects%3912A1F502C8.role

      //## Association: m4objreg::<unnamed>%3912A1F502CA
      //## Role: ClVMBaseEnv::m_iDataDiskCache%3912A1F502CC
      //	Modo Cache:	CVM\CACHE\DataCacheMode
      //## begin ClVMBaseEnv::m_iDataDiskCache%3912A1F502CC.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iDataDiskCache;
      //## end ClVMBaseEnv::m_iDataDiskCache%3912A1F502CC.role

      //## Association: m4objreg::<unnamed>%3912A1F502CD
      //## Role: ClVMBaseEnv::m_iDataCacheRefreshRatio%3912A1F502CF
      //	Ratio Refr.:	CVM\CACHE\DataCacheRefreshRatio
      //	Caches de MetaDatos
      //## begin ClVMBaseEnv::m_iDataCacheRefreshRatio%3912A1F502CF.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iDataCacheRefreshRatio;
      //## end ClVMBaseEnv::m_iDataCacheRefreshRatio%3912A1F502CF.role

      //## Association: m4objreg::<unnamed>%3912A1F502D0
      //## Role: ClVMBaseEnv::m_iMDCacheDefaultMaxPeriod%3912A1F502D2
      //	Max.Period:	CVM\CACHE\MDCacheDefaultMaxPeriod
      //## begin ClVMBaseEnv::m_iMDCacheDefaultMaxPeriod%3912A1F502D2.role preserve=no  protected: m4date_t {1 -> 1VHAPN}
      m4date_t m_iMDCacheDefaultMaxPeriod;
      //## end ClVMBaseEnv::m_iMDCacheDefaultMaxPeriod%3912A1F502D2.role

      //## Association: m4objreg::<unnamed>%3912A1F502D3
      //## Role: ClVMBaseEnv::m_iMDCacheExpiryRatio%3912A1F502D5
      //	Caducidad:	CVM\CACHE\MDCacheExpiryRatio
      //## begin ClVMBaseEnv::m_iMDCacheExpiryRatio%3912A1F502D5.role preserve=no  protected: m4double_t {1 -> 1VHAPN}
      m4double_t m_iMDCacheExpiryRatio;
      //## end ClVMBaseEnv::m_iMDCacheExpiryRatio%3912A1F502D5.role

      //## Association: m4objreg::<unnamed>%3912A1F502D6
      //## Role: ClVMBaseEnv::m_iMDCacheMaxMemory%3912A1F502D8
      //	Memoria:		CVM\CACHE\MDCacheMaxMemory
      //## begin ClVMBaseEnv::m_iMDCacheMaxMemory%3912A1F502D8.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iMDCacheMaxMemory;
      //## end ClVMBaseEnv::m_iMDCacheMaxMemory%3912A1F502D8.role

      //## Association: m4objreg::<unnamed>%3912A1F502D9
      //## Role: ClVMBaseEnv::m_iMDCacheMaxNumObjects%3912A1F502DB
      //	No.Objetos:	CVM\CACHE\MDCacheMaxNumObjects
      //## begin ClVMBaseEnv::m_iMDCacheMaxNumObjects%3912A1F502DB.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iMDCacheMaxNumObjects;
      //## end ClVMBaseEnv::m_iMDCacheMaxNumObjects%3912A1F502DB.role

      //## Association: m4objreg::<unnamed>%3912A1F502DC
      //## Role: ClVMBaseEnv::m_iMDDiskCache%3912A1F502DE
      //	Modo Cache:	CVM\CACHE\MDCacheMode
      //## begin ClVMBaseEnv::m_iMDDiskCache%3912A1F502DE.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iMDDiskCache;
      //## end ClVMBaseEnv::m_iMDDiskCache%3912A1F502DE.role

      //## Association: m4objreg::<unnamed>%3912A1F502DF
      //## Role: ClVMBaseEnv::m_iMDCacheRefreshRatio%3912A1F502E1
      //	Ratio Refr.:	CVM\CACHE\MDCacheRefreshRatio
      //	** CVM\LOG **
      //## begin ClVMBaseEnv::m_iMDCacheRefreshRatio%3912A1F502E1.role preserve=no  protected: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iMDCacheRefreshRatio;
      //## end ClVMBaseEnv::m_iMDCacheRefreshRatio%3912A1F502E1.role

      //## Association: m4objreg::<unnamed>%3912A1F502E2
      //## Role: ClVMBaseEnv::m_acDebugDirectory%3912A1F502E4
      //	Debug	Dir:	CVM\LOG\DebugDir
      //## begin ClVMBaseEnv::m_acDebugDirectory%3912A1F502E4.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acDebugDirectory[ M4CH_MAX_DEBUG_DIRECTORY + 1 ];
      //## end ClVMBaseEnv::m_acDebugDirectory%3912A1F502E4.role

      //## Association: m4objreg::<unnamed>%3912A1F502E5
      //## Role: ClVMBaseEnv::m_iSystemDebugEnable%3912A1F502E7
      //	Debug Enable Level:		CVM\LOG\SystemDebugEnable
      //	** CVM\TIME **
      //## begin ClVMBaseEnv::m_iSystemDebugEnable%3912A1F502E7.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iSystemDebugEnable;
      //## end ClVMBaseEnv::m_iSystemDebugEnable%3912A1F502E7.role

	  m4uint32_t	m_iSystemDebugDetailLevel;

      //## Association: m4objreg::<unnamed>%3912A1F502E8
      //## Role: ClVMBaseEnv::m_dTimeCorrection%3912A1F502EA
      //	Local Time Correction:		CVM\TIME\LocalTimeCorrection
      //## begin ClVMBaseEnv::m_dTimeCorrection%3912A1F502EA.role preserve=no  protected: m4date_t {1 -> 1VHAPN}
      m4date_t m_dTimeCorrection;
      //## end ClVMBaseEnv::m_dTimeCorrection%3912A1F502EA.role

      //## Association: m4objreg::<unnamed>%3912A1F502EB
      //## Role: ClVMBaseEnv::m_iTimeUnit%3912A1F502ED
      //	Time Unit: DAY, HOUR, etc.:	CVM\TIME\TimeUnit
      //## begin ClVMBaseEnv::m_iTimeUnit%3912A1F502ED.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iTimeUnit;
      //## end ClVMBaseEnv::m_iTimeUnit%3912A1F502ED.role

      //## Association: m4objreg::<unnamed>%3912A1F502EE
      //## Role: ClVMBaseEnv::m_dTimeUnit%3912A1F502F0
      //	Time Unit pero como Date
      //	**** OTROS DATOS: ****
      //## begin ClVMBaseEnv::m_dTimeUnit%3912A1F502F0.role preserve=no  protected: m4double_t {1 -> 1VHAPN}
      m4double_t m_dTimeUnit;
      //## end ClVMBaseEnv::m_dTimeUnit%3912A1F502F0.role

      m4uint8_t m_iUseFieldUnicodeTypes;

      //## Association: m4objreg::<unnamed>%3912A1F502F1
      //## Role: ClVMBaseEnv::m_acUserLicence%3912A1F502F3
      //	Licencia de Usuario:	Password encriptada
      //## begin ClVMBaseEnv::m_acUserLicence%3912A1F502F3.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acUserLicence[ 2 * M4_MAX_LEN_MD5 + 1 ];
      //## end ClVMBaseEnv::m_acUserLicence%3912A1F502F3.role

      //## Association: m4objreg::<unnamed>%3912A1F502F4
      //## Role: ClVMBaseEnv::m_acConExec%3912A1F502F6
      //	Connection Exec:	CVM\LDB\ConExec
      //## begin ClVMBaseEnv::m_acConExec%3912A1F502F6.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acConExec [ M4CH_MAX_CON_EXEC + 1 ];
      //## end ClVMBaseEnv::m_acConExec%3912A1F502F6.role

      //## Association: m4objreg::<unnamed>%3912A1F502F7
      //## Role: ClVMBaseEnv::m_acConnectionString%3912A1F502F9
      //	Connection String for BDL:	CVM\LDB\ConnectionString
      //## begin ClVMBaseEnv::m_acConnectionString%3912A1F502F9.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acConnectionString[M4CH_MAX_CONNECTION_STRING + 1 ];
      //## end ClVMBaseEnv::m_acConnectionString%3912A1F502F9.role

      //## Association: m4objreg::<unnamed>%3912A1F502FA
      //## Role: ClVMBaseEnv::m_DataBase%3912A1F502FC
      //	DataBase type:					CVM\LDB\DataBase
      //## begin ClVMBaseEnv::m_DataBase%3912A1F502FC.role preserve=no  protected: m4char_t {1 -> 1VHAPN}
      m4char_t m_DataBase;
      //## end ClVMBaseEnv::m_DataBase%3912A1F502FC.role

      //## Association: m4objreg::<unnamed>%3912A1F502FD
      //## Role: ClVMBaseEnv::m_acRegDBPwd%3912A1F502FF
      //	DataBase Password: CVM\LDB\DBPassword
      //## begin ClVMBaseEnv::m_acRegDBPwd%3912A1F502FF.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acRegDBPwd[ 2 * M4CH_MAX_DB_PWD + 1 ];
      //## end ClVMBaseEnv::m_acRegDBPwd%3912A1F502FF.role

      //## Association: m4objreg::<unnamed>%3912A1F50301
      //## Role: ClVMBaseEnv::m_eILevel%3912A1F50303
      //	Isolation Level:					CVM\LDB\IsolationLevel
      //## begin ClVMBaseEnv::m_eILevel%3912A1F50303.role preserve=no  protected: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_eILevel;
      //## end ClVMBaseEnv::m_eILevel%3912A1F50303.role

      //## Association: m4objreg::<unnamed>%3912A1F50305
      //## Role: ClVMBaseEnv::m_numConn%3912A1F50307
      //	Max. Number Connections:			CVM\LDB\MaxNumConn
      //## begin ClVMBaseEnv::m_numConn%3912A1F50307.role preserve=no  protected: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_numConn;
      //## end ClVMBaseEnv::m_numConn%3912A1F50307.role

      //## Association: m4objreg::<unnamed>%3912A1F5030B
      //## Role: ClVMBaseEnv::m_numCursor%3912A1F5030D
      //	Max. Number Cursors:				CVM\LDB\MaxNumCursor
      //## begin ClVMBaseEnv::m_numCursor%3912A1F5030D.role preserve=no  protected: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_numCursor;
      //## end ClVMBaseEnv::m_numCursor%3912A1F5030D.role

      //## Association: m4objreg::<unnamed>%3912A1F5030E
      //## Role: ClVMBaseEnv::m_numObject%3912A1F50310
      //	Max. Number Objects:				CVM\LDB\MaxNumObject
      //## begin ClVMBaseEnv::m_numObject%3912A1F50310.role preserve=no  protected: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_numObject;
      //## end ClVMBaseEnv::m_numObject%3912A1F50310.role

      //## Association: m4objreg::<unnamed>%3912A1F50311
      //## Role: ClVMBaseEnv::m_numObjectTrans%3912A1F50313
      //	Max. Number Object Transactions:	CVM\LDB\MaxNumObjectTrans
      //## begin ClVMBaseEnv::m_numObjectTrans%3912A1F50313.role preserve=no  protected: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_numObjectTrans;
      //## end ClVMBaseEnv::m_numObjectTrans%3912A1F50313.role

      //## Association: m4objreg::<unnamed>%3912A1F50315
      //## Role: ClVMBaseEnv::m_iBufferSize%3912A1F50317
      //	Max. BlobBufferSize:				CVM\LDB\BlobBufferSize
      //## begin ClVMBaseEnv::m_iBufferSize%3912A1F50317.role preserve=no  protected: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iBufferSize;
      //## end ClVMBaseEnv::m_iBufferSize%3912A1F50317.role

      //## Association: m4objreg::<unnamed>%3912A1F50318
      //## Role: ClVMBaseEnv::m_numStmtCache%3912A1F5031A
      //	Max. Number Sentences Cache:		CVM\LDB\MaxNumStmtCache
      //## begin ClVMBaseEnv::m_numStmtCache%3912A1F5031A.role preserve=no  protected: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_numStmtCache;
      //## end ClVMBaseEnv::m_numStmtCache%3912A1F5031A.role

      //## Association: m4objreg::<unnamed>%3912A1F5031B
      //## Role: ClVMBaseEnv::m_acTempConnString%3912A1F5031D
      //	Temporal Connection String for BDL
      //	** CVM\LOG **
      //## begin ClVMBaseEnv::m_acTempConnString%3912A1F5031D.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acTempConnString[ M4CH_MAX_CONNECTION_STRING + 1 ];
      //## end ClVMBaseEnv::m_acTempConnString%3912A1F5031D.role

      //## Association: m4objreg::<unnamed>%3912A1F5031E
      //## Role: ClVMBaseEnv::m_bMultFiles%3912A1F50320
      //	Multiple Files trace:	CVM\LOG\MultipleFilesTrace
      //## begin ClVMBaseEnv::m_bMultFiles%3912A1F50320.role preserve=no  protected: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bMultFiles;
      //## end ClVMBaseEnv::m_bMultFiles%3912A1F50320.role

      //## Association: m4objreg::<unnamed>%3912A1F50321
      //## Role: ClVMBaseEnv::m_iMaxLDBInspSize%3912A1F50323
      //	Max.Tamaño para LDBInsp:	CVM\LOG\MaxLDBInspSize
      //	** CVM\CACHE **
      //## begin ClVMBaseEnv::m_iMaxLDBInspSize%3912A1F50323.role preserve=no  protected: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iMaxLDBInspSize;
      //## end ClVMBaseEnv::m_iMaxLDBInspSize%3912A1F50323.role

      //## Association: m4objreg::<unnamed>%3912A1F50324
      //## Role: ClVMBaseEnv::m_iLDBDiskCache%3912A1F50326
      //	LDB Cache Mode:	CVM\CACHE\LDBCacheMode
      //## begin ClVMBaseEnv::m_iLDBDiskCache%3912A1F50326.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iLDBDiskCache;
      //## end ClVMBaseEnv::m_iLDBDiskCache%3912A1F50326.role

      //## Association: m4objreg::<unnamed>%3912A1F5039C
      //## Role: ClVMBaseEnv::m_acPathBase%3912A1F5039E
      //## begin ClVMBaseEnv::m_acPathBase%3912A1F5039E.role preserve=no  protected: m4char_t {1 -> nVHAPN}
      m4char_t m_acPathBase[ M4CH_MAX_TEMP_BUFFER + 1 ];
      //## end ClVMBaseEnv::m_acPathBase%3912A1F5039E.role

    // Additional Protected Declarations
      //## begin ClVMBaseEnv%3912A1F501F4.protected preserve=yes
      m4uint32_t m_iObjDataCacheMaxMemory;
      m4uint32_t m_iObjDataCacheMaxNumObjects;
      //## end ClVMBaseEnv%3912A1F501F4.protected

      m4char_t m_acDateSeparator[ M4CH_MAX_DATE_SEPARATOR + 1 ];

	  m4pchar_t m_pcJavaHome;
	  m4pchar_t m_pcJavaClassPath;
	  m4pchar_t m_pcJavaArguments;
	  m4pchar_t m_pcJavaTrace;
	  m4bool_t	m_bJavaAllowInProcess;

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iShowData%39D0805801C0
      //## begin ClVMBaseEnv::m_iShowData%39D0805801C0.attr preserve=no  private: m4uint8_t {UA} 1
      m4uint8_t m_iShowData;
      //## end ClVMBaseEnv::m_iShowData%39D0805801C0.attr

  protected:

      m4uint8_t m_iSMCacheRefresh;

    // Additional Private Declarations
      //## begin ClVMBaseEnv%3912A1F501F4.private preserve=yes
      //## end ClVMBaseEnv%3912A1F501F4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClVMBaseEnv%3912A1F501F4.implementation preserve=yes
      //## end ClVMBaseEnv%3912A1F501F4.implementation

	m4uint32_t m_iJavaTimeout;
};

//## begin ClVMBaseEnv%3912A1F501F4.postscript preserve=yes
//## end ClVMBaseEnv%3912A1F501F4.postscript

//## begin ClM4ObjRegServer%3912A1F503A3.preface preserve=yes
//## end ClM4ObjRegServer%3912A1F503A3.preface

//## Class: ClM4ObjRegServer%3912A1F503A3
//	-- {AddDecl: 71} class [10422..10809]
//	============= clase con propiedades de Server ======================
//## Category: m4objreg%3912A1F501F3
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJREG ClM4ObjRegServer : public ClVMBaseEnv  //## Inherits: <unnamed>%3912A1F503AD
{
  //## begin ClM4ObjRegServer%3912A1F503A3.initialDeclarations preserve=yes
  //## end ClM4ObjRegServer%3912A1F503A3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4ObjRegServer%1356527309; C++
      //	-- {AddDecl: 122} region.unprotectedFunction [2463..2590]
      //	============= clase con propiedades de Server ======================
      ClM4ObjRegServer ();

    //## Destructor (specified)
      //## Operation: ~ClM4ObjRegServer%-37440012; C++
      //	-- {AddDecl: 123} region.unprotectedFunction [2591..2643]
      ~ClM4ObjRegServer ();


    //## Other Operations (specified)
      //## Operation: Init%-1705269378; C++
      //	-- {AddDecl: 125} region.unprotectedFunction [2703..3270]
      m4return_t Init (void );

    // Additional Public Declarations
      //## begin ClM4ObjRegServer%3912A1F503A3.public preserve=yes
      //## end ClM4ObjRegServer%3912A1F503A3.public

  protected:

    //## Other Operations (specified)
      //## Operation: ReadRegistry%-825915481; C++
      //	-- {AddDecl: 127} region.unprotectedFunction [4440..4674]
      m4return_t ReadRegistry (m4pchar_t ai_pcPath);

      //## Operation: ReadCacheRegistry%-1576364119; C++
      //	-- {AddDecl: 126} region.unprotectedFunction [3271..4439]
      m4return_t ReadCacheRegistry (m4pchar_t ai_pcPath);

      //## Operation: Reset%-2071759270; C++
      //	Reset object state
      //	-- {AddDecl: 124} region.unprotectedFunction [2644..2702]
      void Reset ();

    // Additional Protected Declarations
      //## begin ClM4ObjRegServer%3912A1F503A3.protected preserve=yes
      //## end ClM4ObjRegServer%3912A1F503A3.protected

  private:
    // Additional Private Declarations
      //## begin ClM4ObjRegServer%3912A1F503A3.private preserve=yes
      //## end ClM4ObjRegServer%3912A1F503A3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4ObjRegServer%3912A1F503A3.implementation preserve=yes
      //## end ClM4ObjRegServer%3912A1F503A3.implementation

};

//## begin ClM4ObjRegServer%3912A1F503A3.postscript preserve=yes
//## end ClM4ObjRegServer%3912A1F503A3.postscript

//## begin ClM4ObjRegClient%3912A1F503AE.preface preserve=yes
//## end ClM4ObjRegClient%3912A1F503AE.preface

//## Class: ClM4ObjRegClient%3912A1F503AE
//	-- {AddDecl: 73} class [10810..12835]
//	============= clase con propiedades de Cliente ======================
//## Category: m4objreg%3912A1F501F3
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJREG ClM4ObjRegClient : public ClVMBaseEnv  //## Inherits: <unnamed>%3912A1F6001A
{
  //## begin ClM4ObjRegClient%3912A1F503AE.initialDeclarations preserve=yes
  //## end ClM4ObjRegClient%3912A1F503AE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4ObjRegClient%-646752407; C++
      //	-- {AddDecl: 128} region.unprotectedFunction [4675..4802]
      //	============= clase con propiedades de Client ======================
      ClM4ObjRegClient ();

    //## Destructor (specified)
      //## Operation: ~ClM4ObjRegClient%-1439801433; C++
      //	-- {AddDecl: 129} region.unprotectedFunction [4803..4855]
      ~ClM4ObjRegClient ();


    //## Other Operations (specified)
      //## Operation: GetDefaultCSTimeOut%914953894; C++
      m4uint32_t GetDefaultCSTimeOut (void) const
      {
        //## begin ClM4ObjRegClient::GetDefaultCSTimeOut%914953894.body preserve=yes
		  return m_iDefaultCSTimeOut; 
        //## end ClM4ObjRegClient::GetDefaultCSTimeOut%914953894.body
      }

      //## Operation: GetNextCSTimeOut%1703089166; C++
      m4uint32_t GetNextCSTimeOut (void) const
      {
        //## begin ClM4ObjRegClient::GetNextCSTimeOut%1703089166.body preserve=yes
		  return m_iNextCSTimeOut; 
        //## end ClM4ObjRegClient::GetNextCSTimeOut%1703089166.body
      }

      //## Operation: GetGenCSTimeOut%-1937137702; C++
      m4uint32_t GetGenCSTimeOut (void) const
      {
        //## begin ClM4ObjRegClient::GetGenCSTimeOut%-1937137702.body preserve=yes
		  return m_iGenCSTimeOut; 
        //## end ClM4ObjRegClient::GetGenCSTimeOut%-1937137702.body
      }

      //## Operation: SetDefaultCSTimeOut%1046033119; C++
      void SetDefaultCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClM4ObjRegClient::SetDefaultCSTimeOut%1046033119.body preserve=yes
		  m_iDefaultCSTimeOut = ai_iTimeOut; 
        //## end ClM4ObjRegClient::SetDefaultCSTimeOut%1046033119.body
      }

      //## Operation: SetNextCSTimeOut%-1854646436; C++
      void SetNextCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClM4ObjRegClient::SetNextCSTimeOut%-1854646436.body preserve=yes
		  m_iNextCSTimeOut = ai_iTimeOut; 
        //## end ClM4ObjRegClient::SetNextCSTimeOut%-1854646436.body
      }

      //## Operation: SetGenCSTimeOut%-1265663602; C++
      void SetGenCSTimeOut (m4uint32_t ai_iTimeOut)
      {
        //## begin ClM4ObjRegClient::SetGenCSTimeOut%-1265663602.body preserve=yes
		  m_iGenCSTimeOut = ai_iTimeOut; 
        //## end ClM4ObjRegClient::SetGenCSTimeOut%-1265663602.body
      }

      //## Operation: ResetNextCSTimeOut%1916291304; C++
      void ResetNextCSTimeOut (void)
      {
        //## begin ClM4ObjRegClient::ResetNextCSTimeOut%1916291304.body preserve=yes
		  m_iNextCSTimeOut = m_iDefaultCSTimeOut; 
        //## end ClM4ObjRegClient::ResetNextCSTimeOut%1916291304.body
      }

      //## Operation: GetCsTraceLevel%682437428; C++
      m4int8_t GetCsTraceLevel (void) const
      {
        //## begin ClM4ObjRegClient::GetCsTraceLevel%682437428.body preserve=yes
		  return( m_iCsTraceLevel ) ;
	
        //## end ClM4ObjRegClient::GetCsTraceLevel%682437428.body
      }

	  m4int8_t GetVbTraceLevel (void) const
      {
          return( m_iVbTraceLevel ) ;
      }

      //## Operation: GetCsTraceCacheOperations%-811330844; C++
      m4bool_t GetCsTraceCacheOperations (void) const
      {
        //## begin ClM4ObjRegClient::GetCsTraceCacheOperations%-811330844.body preserve=yes
		return( m_bCsTraceCacheOperations ) ;
	
        //## end ClM4ObjRegClient::GetCsTraceCacheOperations%-811330844.body
      }

	  m4pcchar_t GetTransportProtocol (void) const
	  {
		  return (m_acTransportProtocol);
	  }

	  m4int32_t GetControlPort (void) const
	  {
		  return m_ControlPort;
	  }

	  m4int32_t GetCommWorkers (void) const
	  {
		  return m_CommWorkers;
	  }

      //## Operation: Init%1540314371; C++
      //	-- {AddDecl: 131} region.unprotectedFunction [5113..5680]
      m4return_t Init (void );

    // Additional Public Declarations
      //## begin ClM4ObjRegClient%3912A1F503AE.public preserve=yes
      //## end ClM4ObjRegClient%3912A1F503AE.public

  protected:

    //## Other Operations (specified)
      //## Operation: ReadRegistry%-802157592; C++
      //	-- {AddDecl: 133} region.unprotectedFunction [6851..9318]
      m4return_t ReadRegistry (m4pchar_t ai_pcPath);

      //## Operation: ReadCacheRegistry%306286329; C++
      //	-- {AddDecl: 132} region.unprotectedFunction [5681..6850]
      m4return_t ReadCacheRegistry (m4pchar_t ai_pcPath);

      //## Operation: Reset%1752870150; C++
      //	Reset object state
      //	-- {AddDecl: 130} region.unprotectedFunction [4856..5112]
      void Reset ();

    // Data Members for Associations

      //## Association: m4objreg::<unnamed>%3912A1F60008
      //## Role: ClM4ObjRegClient::m_iDefaultCSTimeOut%3912A1F6000A
      //	CS Time out:			CVM\CLIENTSERVER\CSDefaultTimeOut
      //## begin ClM4ObjRegClient::m_iDefaultCSTimeOut%3912A1F6000A.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
      static m4uint32_t m_iDefaultCSTimeOut;
      //## end ClM4ObjRegClient::m_iDefaultCSTimeOut%3912A1F6000A.role

      //## Association: m4objreg::<unnamed>%3912A1F6000B
      //## Role: ClM4ObjRegClient::m_iGenCSTimeOut%3912A1F6000D
      //	Generic CS Time Out:	CVM\CLIENTSERVER\CSGenericTimeOut
      //## begin ClM4ObjRegClient::m_iGenCSTimeOut%3912A1F6000D.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
      static m4uint32_t m_iGenCSTimeOut;
      //## end ClM4ObjRegClient::m_iGenCSTimeOut%3912A1F6000D.role

      //## Association: m4objreg::<unnamed>%3912A1F6000E
      //## Role: ClM4ObjRegClient::m_iNextCSTimeOut%3912A1F60010
      //## begin ClM4ObjRegClient::m_iNextCSTimeOut%3912A1F60010.role preserve=no  protected: static m4uint32_t {1 -> 1VHAPN}
      static m4uint32_t m_iNextCSTimeOut;
      //## end ClM4ObjRegClient::m_iNextCSTimeOut%3912A1F60010.role

      //## Association: m4objreg::<unnamed>%3912A1F60014
      //## Role: ClM4ObjRegClient::m_bCsTraceCacheOperations%3912A1F60016
      //	Operaciones de cache activadas en traza CS:	CVM\CLIENTSERVER\CSTraceShow
      //	CachedOperations
      //## begin ClM4ObjRegClient::m_bCsTraceCacheOperations%3912A1F60016.role preserve=no  protected: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bCsTraceCacheOperations;
      //## end ClM4ObjRegClient::m_bCsTraceCacheOperations%3912A1F60016.role

	  m4char_t m_acTransportProtocol[M4CH_MAX_TRANSPORT_PROTOCOL+1];
      m4int32_t m_ControlPort;
      m4int32_t m_CommWorkers;

    // Additional Protected Declarations
      //## begin ClM4ObjRegClient%3912A1F503AE.protected preserve=yes
      //## end ClM4ObjRegClient%3912A1F503AE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iCsTraceLevel%3944FDD6023A
      //## begin ClM4ObjRegClient::m_iCsTraceLevel%3944FDD6023A.attr preserve=no  private: m4int8_t {UA} 0
      m4int8_t m_iCsTraceLevel;
	  m4int8_t m_iVbTraceLevel;
	  //## end ClM4ObjRegClient::m_iCsTraceLevel%3944FDD6023A.attr

    // Additional Private Declarations
      //## begin ClM4ObjRegClient%3912A1F503AE.private preserve=yes
      //## end ClM4ObjRegClient%3912A1F503AE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4ObjRegClient%3912A1F503AE.implementation preserve=yes
      //## end ClM4ObjRegClient%3912A1F503AE.implementation
	  

};

//## begin ClM4ObjRegClient%3912A1F503AE.postscript preserve=yes
//## end ClM4ObjRegClient%3912A1F503AE.postscript

//## begin ClM4ObjRegSM%3912A1F6001B.preface preserve=yes
//## end ClM4ObjRegSM%3912A1F6001B.preface

//## Class: ClM4ObjRegSM%3912A1F6001B
//	-- {AddDecl: 86} class [12836..13449]
//	========== Registry Single Machine =================
//## Category: m4objreg%3912A1F501F3
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJREG ClM4ObjRegSM : public ClM4ObjRegClient  //## Inherits: <unnamed>%3912A1F6004C
{
  //## begin ClM4ObjRegSM%3912A1F6001B.initialDeclarations preserve=yes
  //## end ClM4ObjRegSM%3912A1F6001B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4ObjRegSM%96582462; C++
      //	-- {AddDecl: 134} region.unprotectedFunction [9319..9534]
      //	=============================================================================
      //	====
      //	============= clase Final con propiedades de SingleMachine
      //	======================
      ClM4ObjRegSM ();

    //## Destructor (specified)
      //## Operation: ~ClM4ObjRegSM%-1492792007; C++
      //	-- {AddDecl: 135} region.unprotectedFunction [9535..9579]
      ~ClM4ObjRegSM ();


    //## Other Operations (specified)
      //## Operation: GetSmOltp%-2022050306; C++
      m4uint8_t GetSmOltp (void) const
      {
        //## begin ClM4ObjRegSM::GetSmOltp%-2022050306.body preserve=yes
		return( m_iSmOltp );
	
        //## end ClM4ObjRegSM::GetSmOltp%-2022050306.body
      }

      //## Operation: Init%1551224479; C++
      //	CVM\CLIENTSERVER\SMOLTP
      //	-- {AddDecl: 88} region.unprotectedFunction [13170..13230]
      //	-- {AddDecl: 137} region.unprotectedFunction [9657..10213]
      m4return_t Init (void);

      //## Operation: ReadSMOLTP%1072811341; C++
      //	============= Metodos static para Propiedades Especiales de Registry
      //	======================
      //	Lectura inicial del SMOLTP para el Inicializador
      //	Salida:	ao_iSmOltp = Valor de la propiedad SMOLTP
      //	return = M4_SUCCESS o M4_ERROR
      static m4return_t ReadSMOLTP (m4uint8_t& ao_iSmOltp);

    // Additional Public Declarations
      //## begin ClM4ObjRegSM%3912A1F6001B.public preserve=yes
      //## end ClM4ObjRegSM%3912A1F6001B.public

  protected:

    //## Other Operations (specified)
      //## Operation: ReadRegistry%1447089992; C++
      //	-- {AddDecl: 139} region.unprotectedFunction [11458..11783]
      //	Lectura del Registry específico de Single Machine
      m4return_t ReadRegistry (m4pchar_t ai_pcPath);

      //## Operation: ReadCacheRegistry%594244098; C++
      //	-- {AddDecl: 138} region.unprotectedFunction [10214..11457]
      m4return_t ReadCacheRegistry (m4pchar_t ai_pcPath);

      //## Operation: Reset%-1566441361; C++
      //	Reset object state
      //	-- {AddDecl: 136} region.unprotectedFunction [9580..9656]
      void Reset ();

    // Data Members for Associations

      //## Association: m4objreg::<unnamed>%3912A1F60047
      //## Role: ClM4ObjRegSM::m_iSmOltp%3912A1F60049
      //	Single Machine:		CVM\CLIENTSERVER\SMOLTP
      //## begin ClM4ObjRegSM::m_iSmOltp%3912A1F60049.role preserve=no  protected: m4uint8_t {1 -> 1VHAPN}
      m4uint8_t m_iSmOltp;
      //## end ClM4ObjRegSM::m_iSmOltp%3912A1F60049.role

    // Additional Protected Declarations
      //## begin ClM4ObjRegSM%3912A1F6001B.protected preserve=yes
      //## end ClM4ObjRegSM%3912A1F6001B.protected

  private:
    // Additional Private Declarations
      //## begin ClM4ObjRegSM%3912A1F6001B.private preserve=yes
      //## end ClM4ObjRegSM%3912A1F6001B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4ObjRegSM%3912A1F6001B.implementation preserve=yes
      //## end ClM4ObjRegSM%3912A1F6001B.implementation

};

// singleton for debug environment
class ClMutex;
class M4_DECL_M4OBJREG ClVMDebugEnv {

private:
	m4int_t m_iDebugLevel;
	m4pchar_t m_pcDebugDir;

	static ClMutex *m_poMutex;
	
	// the main reference
	static ClVMDebugEnv *m_poThis;

	// reset
	void initDebugLevel(m4pcchar_t ai_pcPath);

	// singleton: private contructor
	ClVMDebugEnv();

public:
	// destructor
	virtual ~ClVMDebugEnv();
	
	// static accessor
	static ClVMDebugEnv* getInstance();

	// set / get
	m4int_t		getDebugLevel() const;
	m4return_t 	setDebugLevel(m4int_t ai_iLevel);

	m4int_t		getDebugMaxPeriod() const;
	m4return_t 	setDebugMaxPeriod(m4int_t ai_iMaxPeriod);

	m4pcchar_t	getDebugDir() const;
	void		setPath(m4pcchar_t ai_pcPath);
} ;

//## begin ClM4ObjRegSM%3912A1F6001B.postscript preserve=yes
//## end ClM4ObjRegSM%3912A1F6001B.postscript

//## begin module%3912A1F501C1.epilog preserve=yes









/*
	This is another possibility to relation classes :


			ClVMBaseEnv (abstracta)-------------
			  /			\						\
			/			 \						 \
		ClM4ObjRegBase	ClM4ObjRegServerBase	ClM4ObjRegClientBase
			\	\	  \	/_______________\_______ /____		 /
			 \	 \_____/________			   /	  \		/
			  \		  /	        \			 /			\  /
	Final:	ClM4ObjRegServer	ClM4ObjRegClient	ClM4ObjRegSM
*/



/*
// Interface Abstracto del Registry
// Mantenemos el nombre por compatibilidad hacia atrás
class  M4_DECL_M4OBJREG ClVMBaseEnv
{

public:
	// === Interface ===

	// CVM\WorkDir
    virtual m4pcchar_t  GetWorkingDirectory( void ) const = 0;
	// CVM\TempDir
    virtual m4pcchar_t  GetTempDirectory( void ) const = 0;
	// CVM\TIME\TimeUnit
    virtual m4uint8_t	GetIntegerTimeUnit( void ) const = 0;
	// CVM\TIME\TimeUnit
    virtual m4double_t  GetDateTimeUnit( void ) const = 0;

	// CVM\CACHE
    virtual m4pcchar_t  GetCacheDirectory( void ) const = 0;
	virtual m4uint8_t	GetMDDiskCache( void ) const = 0;
	virtual m4uint32_t	GetMDCacheMaxMemory( void ) const = 0;
	virtual m4uint32_t	GetMDCacheMaxNumObjects( void ) const = 0;
	virtual m4uint32_t	GetMDCacheRefreshRatio( void ) const = 0;
	virtual m4double_t	GetMDCacheExpiryRatio( void ) const = 0;
	virtual m4date_t	GetMDCacheDefaultMaxPeriod( void ) const = 0;
	virtual m4uint8_t	GetDataDiskCache( void ) const = 0;
	virtual m4uint32_t	GetDataCacheMaxMemory( void ) const = 0;
	virtual m4uint32_t	GetDataCacheMaxNumObjects( void ) const = 0;
	virtual m4uint32_t	GetDataCacheRefreshRatio( void ) const = 0;
	virtual m4double_t	GetDataCacheExpiryRatio( void ) const  = 0;
	virtual m4date_t	GetDataCacheDefaultMaxPeriod( void ) const  = 0;

	// CVM\LOG\DebugDir
    virtual m4pcchar_t  GetDebugDirectory( void ) const = 0;
	// CVM\LOG\SystemDebugEnable
	virtual m4uint8_t	GetSystemDebugEnable( void ) const = 0;

	// MACHINE\UserLicense
	virtual m4pcchar_t	GetUserLicense( void ) const = 0;

public:
	// ====================================
	// ========= Registry LDB   ===========
	// ====================================

	virtual m4pcchar_t  GetConnectionStringNoEncrypted( void ) = 0;

	virtual m4pcchar_t  GetConnectionString( void ) = 0;

    virtual m4char_t  GetDataBase( void ) const = 0;

	virtual m4int16_t GetMaxNumConn (void) const = 0;

	virtual m4int16_t GetMaxNumConnKit (void) const

	virtual m4bool_t GetMultipleFilesTrace (void) const = 0;
	
	virtual m4int16_t GetMaxNumCursor (void) const = 0;

	virtual m4int16_t GetMaxNumStmtCache (void) const = 0;

	virtual m4int16_t GetMaxNumObject (void) const = 0;

	virtual m4int16_t GetMaxNumObjectTrans (void) const = 0;

	virtual m4int8_t GetIsolationLevel (void) const = 0;

	virtual m4pcchar_t GetConExec (void) = 0;

	virtual m4pcchar_t GetTempConnString (void) = 0;
	
	virtual m4uint8_t	GetLDBDiskCache( void ) const = 0;

	// === end - Lectura de Registry para LDB

public:
	virtual m4return_t Init( m4pchar_t ai_pcPath ) = 0 ;
	virtual m4return_t Init( void ) = 0 ;
	virtual ~ClVMBaseEnv() { }

protected:
	virtual m4return_t ReadRegistry( m4pchar_t ai_pcPath ) = 0;
	virtual m4return_t ReadCacheRegistry( m4pchar_t ai_pcPath ) = 0;
};
*/



//## end module%3912A1F501C1.epilog


#endif
