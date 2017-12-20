@echo off
::========================================================================
:: @(#) FileVersion: [M4_FILE_VERSION]
:: @(#) FileDescription: Windows Startup script for Meta4 Dispatcher .
:: @(#) CompanyName: Meta4 Spain, S.A.
:: @(#) LegalCopyright: (c) [M4_YEAR]
:: @(#) ProductName:  PeopleNet
::========================================================================

::------------------------------------------------------------------------
:: set environment variables only for this process.
::------------------------------------------------------------------------
setlocal

::------------------------------------------------------------------------
:: set app installation paths
::------------------------------------------------------------------------
set M4_APP_INSTALLATION_ROOT=..
set M4_APP_CONFIGURATION_ROOT=..

set M4_APP_BIN=%M4_APP_INSTALLATION_ROOT%\bin
set M4_APP_CONF=%M4_APP_CONFIGURATION_ROOT%\conf
set M4_APP_WORKDIR=%M4_APP_CONFIGURATION_ROOT%\bin

set M4_RUN_MODE=

set M4_APP_CONFIG_FILE=%M4_APP_CONF%\m4dspini.xml
set M4_APP_EXE=%M4_APP_BIN%\%M4_RUN_MODE%\m4dispatcher.exe 
set M4_APP_FULL_CMD=%M4_APP_EXE% -run %M4_APP_CONFIG_FILE%

set M4_LOGBASE_PATH=%M4_APP_INSTALLATION_ROOT%\conf\logbase.ini
set M4_LOGMSG_PATH=%M4_APP_INSTALLATION_ROOT%\conf\

rem ------------------------------------------------------------------------
rem app run paths
rem ------------------------------------------------------------------------
set M4_APP_PATH=%M4_APP_CONFIGURATION_ROOT%
set M4_APP_PATH=%M4_APP_PATH%;%M4_APP_INSTALLATION_ROOT%\bin\%M4_RUN_MODE%
set PATH=%M4_APP_PATH%;%PATH%

rem ------------------------------------------------------------------------
rem Console ouput trace level
rem ------------------------------------------------------------------------
set M4_SRV_COUT=0
set M4_SRV_TRACE_LEVEL=3

::------------------------------------------------------------------------
:: external applications
::------------------------------------------------------------------------
set DEPENDS=depends.exe
set MSDEV=msdev.exe

::------------------------------------------------------------------------
:: check input arguments
::------------------------------------------------------------------------
if /I "%1" == "-help" goto usage
if /I "%1" == "-h" goto usage
if /I "%1" == "/help" goto usage
if /I "%1" == "/h" goto usage

::------------------------------------------------------------------------
:: analize input commands
::------------------------------------------------------------------------
set PROGRAMNAME=%0
set COMMAND=%1
shift
if /I "%COMMAND%" == "" goto start
if /I "%COMMAND%" == "start" goto start
if /I "%COMMAND%" == "stop" goto stop
if /I "%COMMAND%" == "depends" goto depends
if /I "%COMMAND%" == "msdev" goto msdev
goto usage

::------------------------------------------------------------------------
:: startup application
::------------------------------------------------------------------------
:start
%M4_APP_FULL_CMD%
goto exit

::------------------------------------------------------------------------
:: shutdown application 
::------------------------------------------------------------------------
:stop
echo Not implemented yet.
goto exit

::------------------------------------------------------------------------
:: run dependency walker
::------------------------------------------------------------------------
:depends
start %DEPENDS% %M4_APP_FULL_CMD%
goto exit

::------------------------------------------------------------------------
:: run msdev
::------------------------------------------------------------------------
:msdev
start %MSDEV% %M4_APP_FULL_CMD%
goto exit

::------------------------------------------------------------------------
:: show the help
::------------------------------------------------------------------------
:usage
echo Usage:
echo   %PROGRAMNAME% start or stop           - default is start
echo   %PROGRAMNAME% depends                 - run dependency walker
echo   %PROGRAMNAME% msdev                   - run application in MsDev
goto exit

::------------------------------------------------------------------------
:: exit
::------------------------------------------------------------------------
:exit
endlocal

