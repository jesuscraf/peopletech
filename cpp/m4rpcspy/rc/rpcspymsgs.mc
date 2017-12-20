;#ifndef __RPCSPYMSGS_H__
;#define __RPCSPYMSGS_H__
;

LanguageNames =
(
English = 0x0409:rpcspymsgs_ENG
Spanish = 0x0C0A:rpcspymsgs_ESP
)


MessageId=0x0001
SymbolicName=IDS_DIALOGFAILURE
Language=Spanish
La creación del CommonDialog falló en la llamada a DialogBox()
.
Language=English
Creation of CD failed on call to DialogBox()
.

MessageId=0x0002
SymbolicName=IDS_STRUCTSIZE
Language=Spanish
Tamaño de estructura inválido pasado al CommonDialog
.
Language=English
Invalid structure size passed to CD
.

MessageId=0x0003
SymbolicName=IDS_INITIALIZATION
Language=Spanish
Fallo inicializando el CommonDialog, posiblemente debido a memoria insuficiente.
.
Language=English
Failure initializing CD. Possibly\n\r dued to insufficient memory.
.

MessageId=0x0004
SymbolicName=IDS_NOTEMPLATE
Language=Spanish
No se encontró la plantilla para el CommonDialog
.
Language=English
Failure finding custom template for CD
.

MessageId=0x0005
SymbolicName=IDS_NOHINSTANCE
Language=Spanish
No se pasó el handle de instancia al CommonDialog
.
Language=English
Instance handle not passed to CD
.

MessageId=0x0006
SymbolicName=IDS_LOADSTRFAILURE
Language=Spanish
Fallo cargando la cadena especificada
.
Language=English
Failure loading specified string
.

MessageId=0x0007
SymbolicName=IDS_FINDRESFAILURE
Language=Spanish
Fallo buscando el recurso
.
Language=English
Failure finding specified resource
.

MessageId=0x0008
SymbolicName=IDS_LOADRESFAILURE
Language=Spanish
Fallo cargando el recurso
.
Language=English
Failure loading specified resource
.

MessageId=0x0009
SymbolicName=IDS_LOCKRESFAILURE
Language=Spanish
Fallo bloqueando el recurso
.
Language=English
Failure locking specified resource
.

MessageId=0x000A
SymbolicName=IDS_MEMALLOCFAILURE
Language=Spanish
Fallo en la asignación de la memoria interna del CommonDialog
.
Language=English
Failure allocating memory for internal CD structure
.

MessageId=0x000B
SymbolicName=IDS_MEMLOCKFAILURE
Language=Spanish
Fallo bloqueando la memoria interna del CommonDialog
.
Language=English
Failure locking memory for internal CD structure
.

MessageId=0x000C
SymbolicName=IDS_NOHOOK
Language=Spanish
No se pasó ninguna función gancho al CommonDialog, sin embargo se especificó el marcador ENABLEHOOK
.
Language=English
No hook function passed to CD but ENABLEHOOK\n\r was passed as a flag
.

MessageId=0x000D
SymbolicName=IDS_SETUPFAILURE
Language=Spanish
Fallo estableciendo los recursos del CommonDialog
.
Language=English
Failure setting up resources for CD
.

MessageId=0x000E
SymbolicName=IDS_PARSEFAILURE
Language=Spanish
Fallo procesando las cadenas en la sección [devices] del archivo WIN.INI
.
Language=English
Failure parsing strings in [devices]\n\rsection of WIN.INI
.

MessageId=0x000F
SymbolicName=IDS_RETDEFFAILURE
Language=Spanish
Se especificó el marcador PD_RETURNDEFAULT pero bien los parámetros hDevMode o hDevNames no eran cero.
.
Language=English
PD_RETURNDEFAULT flag was set but either the \n\rhDevMode or hDevNames field was nonzero
.

MessageId=0x0010
SymbolicName=IDS_LOADDRVFAILURE
Language=Spanish
Fallo cargando el controlador de dispositivo para la impresora.
.
Language=English
Failure loading the printers device driver
.

MessageId=0x0011
SymbolicName=IDS_GETDEVMODEFAIL
Language=Spanish
El controlador de dispositivo de impresora fallo al intentar inicializar la estructura DEVMODE
.
Language=English
The printer driver failed to initialize a DEVMODE data structure
.

MessageId=0x0012
SymbolicName=IDS_INITFAILURE
Language=Spanish
La impresión del CD falló durante la inicialización
.
Language=English
Print CD failed during initialization
.

MessageId=0x0013
SymbolicName=IDS_NODEVICES
Language=Spanish
No se encontró ningún controlador de impresora
.
Language=English
No printer device drivers were found
.

MessageId=0x0014
SymbolicName=IDS_NODEFAULTPRN
Language=Spanish
No hay ninguna impresora predeterminada
.
Language=English
No default printer was found
.

MessageId=0x0015
SymbolicName=IDS_DNDMMISMATCH
Language=Spanish
Los datos en DEVMODE contradicen aquellos en DEVNAMES
.
Language=English
Data in DEVMODE contradicts data in DEVNAMES
.

MessageId=0x0016
SymbolicName=IDS_CREATEICFAILURE
Language=Spanish
Fallo creando una IC
.
Language=English
Failure creating an IC
.

MessageId=0x0017
SymbolicName=IDS_PRINTERNOTFOUND
Language=Spanish
Impresora no encontrada
.
Language=English
Printer not found
.

MessageId=0x0018
SymbolicName=IDS_NOFONTS
Language=Spanish
No existen fuentes
.
Language=English
No fonts exist
.

MessageId=0x0019
SymbolicName=IDS_SUBCLASSFAILURE
Language=Spanish
Fallo en la subclasificación mientras se inicializaba el CD
.
Language=English
Failure subclassing during initialization of CD
.

MessageId=0x001A
SymbolicName=IDS_INVALIDFILENAME
Language=Spanish
Nombre de archivo invalido pasado a FileOpen o FileSave
.
Language=English
Invalid filename passed to FileOpen or FileSave
.

MessageId=0x001B
SymbolicName=IDS_BUFFERTOOSMALL
Language=Spanish
El buffer pasado al CommonDialog es demasido pequeño para acomodar la cadena
.
Language=English
Buffer passed to CD too small to accomodate string
.

MessageId=0x001C
SymbolicName=IDS_UNKNOWNERROR
Language=Spanish
Error desconocido.
.
Language=English
Unknown error.
.

MessageId=0x001D
SymbolicName=IDS_APPLY_WARN
Language=Spanish
Cambiar la información de de enlace para la llamadas a procedimientos remotos desconectará los clientes actualmente enganchados ¿Está seguro de que desea establecer los nuevos parámetros de enlace RPC?
.
Language=English
Changing remote procedure call binding information will disconnect currently binded clients. Are you sure that you want to set new RPC binding parameters?
.

MessageId=0x001E
SymbolicName=IDS_SPYTITLE
Language=Spanish
Espía de Eventos
.
Language=English
Event Spy
.

MessageId=0x001F
SymbolicName=IDS_TYPE
Language=Spanish
Tipo
.
Language=English
Type
.

MessageId=0x0020
SymbolicName=IDS_DATE
Language=Spanish
Fecha
.
Language=English
Date
.

MessageId=0x0021
SymbolicName=IDS_SOURCE
Language=Spanish
Fuente
.
Language=English
Source
.

MessageId=0x0022
SymbolicName=IDS_BRIEFING
Language=Spanish
Resumen
.
Language=English
Briefing
.

MessageId=0x0023
SymbolicName=IDS_SURECLR
Language=Spanish
¿Está seguro de que desea limpiar el registro de eventos?
.
Language=English
Are you sure that you want to clear the event log?
.

MessageId=0x0024
SymbolicName=IDS_TTCLEAR
Language=Spanish
Limpiar registro de eventos
.
Language=English
Clear event log
.

MessageId=0x0025
SymbolicName=IDS_TTDEL
Language=Spanish
Eliminar las filas seleccionadas
.
Language=English
Delete selected rows
.

MessageId=0x0026
SymbolicName=IDS_TTSAVE
Language=Spanish
Guardar registro en disco
.
Language=English
Save log to disk
.

MessageId=0x0027
SymbolicName=IDS_TTPRINT
Language=Spanish
Imprimir registro
.
Language=English
Print log
.

MessageId=0x0028
SymbolicName=IDS_TTOPTION
Language=Spanish
Opciones de conectividad
.
Language=English
Connectivity options
.

MessageId=0x0029
SymbolicName=IDM_FILE
Language=Spanish
&Archivo
.
Language=English
&File
.

MessageId=0x002A
SymbolicName=IDM_SAVE
Language=Spanish
&Guardar
.
Language=English
&Save
.

MessageId=0x002B
SymbolicName=IDM_PRINT
Language=Spanish
&Imprimir
.
Language=English
&Print
.

MessageId=0x002C
SymbolicName=IDM_EXIT
Language=Spanish
&Salir
.
Language=English
&Exit
.

MessageId=0x002E
SymbolicName=IDM_EDIT
Language=Spanish
&Editar
.
Language=English
&Edit
.

MessageId=0x002F
SymbolicName=IDM_DEL
Language=Spanish
&Borrar
.
Language=English
&Delete
.

MessageId=0x0030
SymbolicName=IDM_CLEARALL
Language=Spanish
Borrar &todo
.
Language=English
&Clear all
.

MessageId=0x0031
SymbolicName=IDM_OPTIONS
Language=Spanish
&Opciones
.
Language=English
&Options
.

MessageId=0x0032
SymbolicName=IDM_HELP
Language=Spanish
A&yuda
.
Language=English
&Help
.

MessageId=0x0033
SymbolicName=IDM_ABOUT
Language=Spanish
&Acerca de...
.
Language=English
&About
.

MessageId=0x0034
SymbolicName=IDC_OK
Language=Spanish
&Aceptar
.
Language=English
&Ok
.

MessageId=0x0035
SymbolicName=IDC_CANCEL
Language=Spanish
&Cancelar
.
Language=English
&Cancel
.

MessageId=0x0036
SymbolicName=IDC_APPLY
Language=Spanish
A&plicar
.
Language=English
&Apply
.

MessageId=0x0037
SymbolicName=IDC_ABOUT_CAPTION
Language=Spanish
Acerca del espía de Eventos
.
Language=English
About Spy
.

MessageId=0x0038
SymbolicName=IDC_ABOUT_TEXT
Language=Spanish
Aviso: Este programa sólo puede ser usado por Meta4 de acuerdo a los términos y condiciones de la licencia del contrato de uso de esta aplicación. El uso que se haga de este programa debe cumplir dicha licencia.
.
Language=English
Warning: This program may only be used by Meta4 in accordance with the terms and conditions in the License Contract for Use of this software application. The uses that are made of this program must comply with the permitted uses under said license.
.

MessageId=0x0039
SymbolicName=IDC_PRINT_CAPTION
Language=Spanish
Imprimiendo...
.
Language=English
Printing...
.

MessageId=0x003A
SymbolicName=IDC_OPTIONS_CAPTION
Language=Spanish
Opciones de conectividad del espía de eventos
.
Language=English
Remote Procedure Call Spy Options
.

MessageId=0x003B
SymbolicName=IDC_OPTIONS
Language=Spanish
Opciones de conectividad
.
Language=English
Connectivity options
.

MessageId=0x003C
SymbolicName=IDC_SEQUENCE
Language=Spanish
Secuencia de protocolo
.
Language=English
Protocol Sequence
.

MessageId=0x003D
SymbolicName=IDC_ENDPOINT
Language=Spanish
Punto de destino
.
Language=English
Endpoint
.

MessageId=0x003E
SymbolicName=IDC_LOG_LEVEL
Language=Spanish
Nivel de registro
.
Language=English
Log level
.

MessageId=0x003F
SymbolicName=IDC_MAX_ROWS
Language=Spanish
Máximo número de filas a mostrar
.
Language=English
Maximun number of rows to display
.

MessageId=0x0040
SymbolicName=IDC_TRACE
Language=Spanish
Registrar trazas, información, avisos y errores
.
Language=English
Log Traces, Info, Warnings && Errors
.

MessageId=0x0041
SymbolicName=IDC_EXCL
Language=Spanish
Registrar avisos y errores
.
Language=English
Log Warnings && Errors
.

MessageId=0x0042
SymbolicName=IDC_INFO
Language=Spanish
Registrar información, avisos y errores
.
Language=English
Log Info, Warnings && Errors
.

MessageId=0x0043
SymbolicName=IDC_CRIT
Language=Spanish
Registrar errores
.
Language=English
Log Errors
.

MessageId=0x0044
SymbolicName=IDC_QUEST
Language=Spanish
Deshabilitar registro
.
Language=English
Log disabled
.

MessageId=0x0045
SymbolicName=IDC_NO_64BITS
Language=Spanish
. Esta aplicación no está disponible en 64 bits!!
.
Language=English
. This apllication is not available for 64 bits!!
.





;
;#endif  //__RPCSPYMSGS_H__
;