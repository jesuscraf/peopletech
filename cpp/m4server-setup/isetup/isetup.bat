@echo off
rem ------------------------------------------------------------------------
rem @(#) FileVersion: [M4_FILE_VERSION]
rem @(#) CompanyName: Meta4 Spain, S.A.
rem @(#) LegalCopyright: (c) [M4_YEAR]
rem @(#) ProductName:  PeopleNet
rem ------------------------------------------------------------------------

rem ------------------------------------------------------------------------
rem check input arguments
rem ------------------------------------------------------------------------
set PROGRAMNAME=%0
if /I "%1" == "-help" goto usage
if /I "%1" == "-h" goto usage
if /I "%1" == "/help" goto usage
if /I "%1" == "/h" goto usage

rem ------------------------------------------------------------------------
rem set isetup variables.
set M4_APP_ROOT=..\
set ISETUP_PATH=%M4_APP_ROOT%\setup;%M4_APP_ROOT%\..\common;%M4_APP_ROOT%\m4server\lib;%M4_APP_ROOT%\m4server\bin
set ISETUP_PATH=%M4_APP_ROOT%\setup\debug;%M4_APP_ROOT%\..\common\debug;%M4_APP_ROOT%\m4server\lib\debug;%M4_APP_ROOT%\m4server\bin\debug;%ISETUP_PATH%
set ISETUP=isetup.pl
set ISETUP_INI=.\isetup.ini

rem ------------------------------------------------------------------------
rem get isetup ini file.
if not "%1" == "" set ISETUP_INI=%1

rem ------------------------------------------------------------------------
rem set script environment.
setlocal
set PATH=%ISETUP_PATH%;%PATH%

rem ------------------------------------------------------------------------
rem run the perl script.
%ISETUP% --ex=%ISETUP_INI%
goto exit

rem ------------------------------------------------------------------------
rem show the help
rem ------------------------------------------------------------------------
:usage
echo Usage:
echo   %PROGRAMNAME% --ex=full-path\isetup.ini
goto exit

rem ------------------------------------------------------------------------
rem end.
:exit
endlocal
