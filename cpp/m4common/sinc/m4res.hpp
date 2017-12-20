//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              channel
// File:                m4res.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel
//
//
//==============================================================================


#ifndef __M4RES_HPP__
#define __M4RES_HPP__

//==============================================================================
//  Base para eventos
//==============================================================================

#define M4_EVENT_ERROR_BASE							0x00010000		//	65536
#define M4_AUDIT_ERROR_BASE							0x00020000		//	131072
#define M4_ACCESS_ERROR_BASE						0x00030000		//	196608


//==============================================================================
//  Bases para los errores de cada componente del canal
//==============================================================================

#define	M4_CH_MCR_ERROR_BASE			            0x00110000      // 1114112
#define	M4_MDFAC_ERROR_BASE				            0x00110000      // 1114112
#define	M4_CH_DM_ERROR_BASE			                0x00120000      // 1179648
#define	M4_CH_LDB_ERROR_BASE			            0x00130000      // 1245184
#define	M4_CH_VM_ERROR_BASE			                0x00140000      // 1310720
#define	M4_CH_WO_ERROR_BASE			                0x00150000      // 1376256
#define	M4_CH_DS_ERROR_BASE			                0x00160000      // 1441792
#define	M4_CH_CS_ERROR_BASE			                0x00170000      // 1507328
#define	M4_CH_TF_ERROR_BASE			                0x00180000      // 1572864
#define	M4_VARIANT_ERROR_BASE						0x00190000      // 1638400
#define	M4_MDRT_ERROR_BASE				            0x001A0000      // 1703936
#define	M4_OBJGLB_ERROR_BASE				        0x001B0000		// 1769472
#define M4_OBJREG_ERROR_BASE						0x001C0000		// 1835008
#define M4_HSPARSER_ERROR_BASE						0x001D0000		// 1900544
#define M4_CH_LICENSE_ERROR_BASE					0x001E0000		// 1966080

#define M4_RPT_ENGINE_ERROR_BASE                    0x00210000
#define M4_RPT_VIEWER_ERROR_BASE                    0x00220000
#define M4_RPT_ENGINE_ODBC_BASE                     0x00230000
#define M4_OBJ_OLE_BASE								0x00240000
#define M4_OBJ_OLEDSI								0x00240068
#define M4_OBJ_IDL									0x002400CD
#define M4_OBJ_LDAP									0x00240131
#define M4_OBJ_MAIL									0x00240195
#define M4_OBJ_CORBA_BASE							0x00250000

#define M4_SMCS_BASE								0x00260000      // 2490368
#define	M4_SERIAL_ERROR_BASE                        0x00270000      // 2555904
#define	M4_CH_PRES_ERROR_BASE                       0x00280000      // 2621440
#define	M4_CH_CACHEDIR_ERROR_BASE                   0x002A0000      // 2752512

#define M4_GENCONN_ERROR_BASE						0x002B0000		// 2818048
#define M4EAERROR_BASE								0x002C0000		// 2883584
#define M4SAPERROR_BASE								0x002D0000		// 2949120
#define M4SOAPERROR_BASE							0x002E0000		// 3014656

#define M4_DESIGN_CHANNEL_ERROR_BASE				0x002F0000		// 3080192


#define M4_RPT_FMETRICS_ERROR_BASE					0x00400000
#define M4_RPT_MIENV_ERROR_BASE                     0x00410000
#define M4_RPT_APISQL_ERROR_BASE                    0x00420000
#define M4MDTERROR_BASE								0x00430000		// 4390912
#define M4PRINTPDFERROR_BASE						0x00440000		// 4456448

#define M4_DOC_MANAG_ERROR_BASE						0x00780000		// 7864320

#define M4_CLI_JAVA_ERROR_BASE						0x00960000		// 9830400

#define M4_KERBEROS_ERROR_BASE						0x00441000		// 4460544

#define M4_EMIND_ERROR_BASE					        0x004434F0		// 4470000

#define M4JSOAPCLERROR_BASE							0x00450000		// 4521984

#define M4JAVAAPIERROR_BASE							0x00470000		// 4653056

//==============================================================================
//  Bases para los errores de cada componente del servidor
//==============================================================================

#define	M4_SRV_ERROR_BASE							0x00300000		// 3145728
#define M4_SRV_LM_ERROR_BASE						0x00310000		// 3211264
#define M4_SRV_RECEIVER_BASE						0x00320000		// 3276800
#define M4_SRV_SS_ERROR_BASE						0X00330000
#define M4_SRV_CACHE_ERROR_BASE						0x00340000
#define M4_SRV_JOB_SCHEDULER_ERROR					0x00350000		// 3473408
#define M4_SRV_API_CLIENTE_ERROR					0x00360000		// 3538944
#define M4_SRV_EXE_FOR_CHANNEL_BASE					0x00370000		// 3604480
#define M4_GEN_PROXY_BASE							0x00370000		// 3604480
#define M4_SRV_UUSS_ERROR_BASE						0x00380000		// 3670016
#define M4_SRV_LDBSUB_ERROR_BASE                    0x00390000      // 3735552 
#define M4_SRV_ADMINISTRATOR_ERROR_BASE				0x003A0000		// 3801088
#define M4_SRV_SERVERBASE_ERROR_BASE				0x003B0000		// 3866624
#define M4_SRV_SNMP_ERROR_BASE                      0x003C0000		// 3932160

//==============================================================================
//  Base para los mensajes de error del Dispatcher
//==============================================================================

#define M4_DPCH_ERROR_BASE							0x00600000		// 6291456
#define M4_APPCTL_ERROR_BASE						0x00610000		// 6356992


//==============================================================================
//  Base para los mensajes de Crystal Reports
//==============================================================================

#define M4_RPT_CRYSTAL_ERROR_BASE					0x008C0001		// 6291456


//==============================================================================
//  Base para los mensajes de Plantilla de tablas de valor
//==============================================================================

#define M4_VALUE_TABLE_TEMPLATE						0x00960000		// 9830400


//==============================================================================
//  Base para los mensajes de Transferencia de datos
//==============================================================================

#define M4_DATA_TRANSFER							0x00A00000		// 10485760


//==============================================================================
//  Base para los mensajes de Mapeos de tablas de valor
//==============================================================================

#define M4_VALUE_TABLE_MAPPINGS						0x00A10000		// 10551296

//==============================================================================
//  Base para los mensajes de Desktops
//==============================================================================

#define M4_DESKTOPS_ERROR_BASE						0x00A20000		// 10616832

//==============================================================================
//  Base para los mensajes de import export excel
//==============================================================================

#define M4_EXCEL_IO_ERROR_BASE						0x00A30000		// 10682368

//==============================================================================
//  Base para los mensajes de cambio de contraseña
//==============================================================================

#define M4_PWD_ERROR_BASE							0x00A40000		// 10747904

//==============================================================================
//  Base para los mensajes de transcodificación de datos
//==============================================================================

#define M4_DATA_TRANSCODIFICATION_ERROR_BASE		0x00A50000		// 10813440

//==============================================================================
//  Base para los mensajes de traducciones
//==============================================================================

#define M4_TRANSLATION_ERROR_BASE					0x00A60000		// 10878976

//==============================================================================
//  Base para los mensajes del organigrama
//==============================================================================

#define M4_ORG_CHART_ERROR_BASE						0x00A70000		// 10944512

//==============================================================================
//  Base para los mensajes del api Excel libxl
//==============================================================================

#define M4_EXCEL_LIBXL_ERROR_BASE					0x00A80000		// 11010048

#endif
