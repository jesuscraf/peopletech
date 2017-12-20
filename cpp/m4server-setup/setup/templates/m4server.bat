@echo off
:: ========================================================================
:: @(#) FileVersion: [M4_FILE_VERSION]
:: @(#) FileDescription: script to launch and stop Meta4 Application Server
:: @(#) CompanyName: Meta4 Spain, S.A.
:: @(#) LegalCopyright: (c) [M4_YEAR]
:: @(#) ProductName:  PeopleNet
:: ========================================================================

:: ------------------------------------------------------------------------
:: set environment variables only for this process.
:: ------------------------------------------------------------------------
setlocal

:: ========================================================================
:: General m4server settings (user customizable)
:: ========================================================================

:: ------------------------------------------------------------------------
:: set m4appserver installation paths
:: ------------------------------------------------------------------------
set M4_SERVER_CONFIGURATION_ROOT=c:\m4serversite-local\m4appserver-config
set M4_SERVER_INSTALLATION_ROOT=W:\peopletech\evolutivo\987.654.321\deploy\win32\m4serversite\m4server

:: ------------------------------------------------------------------------
:: set run (debug or nothing (release))
:: ------------------------------------------------------------------------
set M4_RUN_MODE=

:: ------------------------------------------------------------------------
:: set dbmode (oci or odbc, default is oci)
:: ------------------------------------------------------------------------
set M4_DB_MODE=oci

:: ------------------------------------------------------------------------
:: set extern doc management tables (1 or nothing (0))
:: ------------------------------------------------------------------------
set M4_SSR_EXTERN_DB=

:: ------------------------------------------------------------------------
:: external applications
:: ------------------------------------------------------------------------
set DEPENDS=depends.exe
set MSDEV=devenv.exe

:: ------------------------------------------------------------------------
:: check for help argument
:: ------------------------------------------------------------------------
if /I "%1" == "-help" goto usage
if /I "%1" == "-h" goto usage
if /I "%1" == "/help" goto usage
if /I "%1" == "/h" goto usage

:: ------------------------------------------------------------------------
::  set startup security (encrypted or blank (for normal)
:: ------------------------------------------------------------------------
set M4_STARTUP_SECURITY=encrypted

:: ------------------------------------------------------------------------
:: set m4appserver user and password (unused if startup security encrypted)
:: ------------------------------------------------------------------------
set M4_USER=TESTER
set M4_PASSWORD=TESTER

:: ------------------------------------------------------------------------
:: m4appserver run paths
:: ------------------------------------------------------------------------
:setpath
set M4SERVER_PATH=%M4_SERVER_CONFIGURATION_ROOT%
set M4SERVER_PATH=%M4SERVER_PATH%;%M4_SERVER_INSTALLATION_ROOT%\bin\%M4_RUN_MODE%
set M4SERVER_PATH=%M4SERVER_PATH%;%M4_SERVER_INSTALLATION_ROOT%\lib\%M4_RUN_MODE%
set M4SERVER_PATH=%M4SERVER_PATH%;%M4_SERVER_INSTALLATION_ROOT%\lib\%M4_DB_MODE%\%M4_RUN_MODE%

set M4SERVER_INTERNAL_PATH=%M4_SERVER_INSTALLATION_ROOT%\..\..\common\%M4_DB_MODE%\%M4_RUN_MODE%
set M4SERVER_INTERNAL_PATH=%M4SERVER_INTERNAL_PATH%;%M4_SERVER_INSTALLATION_ROOT%\..\..\common\%M4_RUN_MODE%

set M4SERVER_PATH=%M4SERVER_PATH%;%M4SERVER_INTERNAL_PATH%

:: delete " from path.
set M4SERVER_PATH=%M4SERVER_PATH:"=%

:: ------------------------------------------------------------------------
:: m4appserver window title
:: ------------------------------------------------------------------------
set M4_TITLE=Meta4 Application Server %M4_DB_MODE% %M4_RUN_MODE%

:: ------------------------------------------------------------------------
:: set m4appserver environment variables.
:: ------------------------------------------------------------------------
set PATH=%M4SERVER_PATH%;%PATH%
set M4_LOGBASE_PATH=%M4_SERVER_INSTALLATION_ROOT%\workdir\logbase.ini
set M4_LOGMSG_PATH=
set M4_USER_CREDENTIALS=%M4_SERVER_CONFIGURATION_ROOT%\ssc_appuser
if not _%M4_STARTUP_SECURITY%_==_encrypted_ set M4_USER_CREDENTIALS=%M4_USER% %M4_PASSWORD%
set M4_SERVER_EXE=m4server.exe %M4_SERVER_CONFIGURATION_ROOT%\startup.obl %M4_USER_CREDENTIALS%

:: ------------------------------------------------------------------------
:: set Java environment variables.
:: ------------------------------------------------------------------------

:: JRE or JDK Path - modify according to your current setup

set M4JAVAHOME=

:: JVM Classpath - no need to modify

set M4CLASSPATH=%M4_SERVER_INSTALLATION_ROOT%/lib/java/bcmail-jdk15-146.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/bcpg-jdk15on-149.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/bcprov-jdk15-146.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/m4documentsconverter.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/jetty-alpn-agent-2.0.6.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/m4alpnagent.jar
set M4CLASSPATH=%M4CLASSPATH%;%M4JAVAHOME%/lib/tools.jar 
set M4CLASSPATH=%M4CLASSPATH%;%M4_SERVER_INSTALLATION_ROOT%/lib/java/*

:: JVM Arguments separated by ";" - example: "-Xmx256m;-Xms128m"

set M4JAVAARGS=

if "%M4JAVAHOME%" == "" goto JavaCfgEnd
set PATH=%M4JAVAHOME%\bin\client;%M4JAVAHOME%\bin;%PATH%
:JavaCfgEnd

:: Java Trace

set M4JAVATRACE=0

:: Java Timeout

set M4JAVATIMEOUT=-1

:: Java In process activation

set M4JAVAALLOWINPROCESS=0

:: ------------------------------------------------------------------------
:: set hadoop environment variables.
:: ------------------------------------------------------------------------
set HADOOPSERVICES_HOME=%M4_SERVER_INSTALLATION_ROOT%\m4hadoopservices
set HADOOP_HOME=%HADOOPSERVICES_HOME%\hadoop
set HADOOP_BIN=%HADOOPSERVICES_HOME%\Hadoop\bin

:: set hadoop binaries to the path
set PATH=%HADOOP_BIN%;%PATH%

:: ------------------------------------------------------------------------
:: analize input commands
:: ------------------------------------------------------------------------
set PROGRAMNAME=%0
set COMMAND=%1
if /I "%COMMAND%" == "" goto start
if /I "%COMMAND%" == "start" goto start
if /I "%COMMAND%" == "batch" goto batch
if /I "%COMMAND%" == "stop" goto stop
if /I "%COMMAND%" == "kill" goto kill
if /I "%COMMAND%" == "depends" goto depends
if /I "%COMMAND%" == "msdev" goto msdev
goto siteStart

:: ------------------------------------------------------------------------
:: Startup the server configured in a meta4site. This is executed when this 
:: script is called from m4appctl. Extra arguments are passed to m4server exe.
:. After execution, exit from parent process to kill parent window.
:: ------------------------------------------------------------------------
:siteStart
set STARTUP_LOG_FILE=%M4_SERVER_CONFIGURATION_ROOT%\temp\m4debug\startuplog.txt
ECHO 	USER: %USERNAME%	DATE: %DATE%	TIME: %TIME%	ARGUMENTS: %*>> %STARTUP_LOG_FILE%

set ARGV=%*
cd /D %M4_SERVER_CONFIGURATION_ROOT%
call:removeTempFiles
"%M4_SERVER_INSTALLATION_ROOT%\bin\%M4_RUN_MODE%\m4server.exe" %M4_SERVER_CONFIGURATION_ROOT%\startup.obl %M4_USER_CREDENTIALS% %ARGV%
goto exit

:: ------------------------------------------------------------------------
:: startup the server
:: ------------------------------------------------------------------------
:start
set STARTUP_LOG_FILE=%M4_SERVER_CONFIGURATION_ROOT%\temp\m4debug\startuplog.txt
ECHO 	USER: %USERNAME%	DATE: %DATE%	TIME: %TIME%	ARGUMENTS: %*>> %STARTUP_LOG_FILE%

cd /D %M4_SERVER_CONFIGURATION_ROOT%
call:removeTempFiles
start "%M4_TITLE%" %M4_SERVER_EXE%
goto exit

:: ------------------------------------------------------------------------
:: startup the server in batch mode
:: ------------------------------------------------------------------------
:batch
set STARTUP_LOG_FILE=%M4_SERVER_CONFIGURATION_ROOT%\temp\m4debug\startuplog.txt
ECHO 	USER: %USERNAME%	DATE: %DATE%	TIME: %TIME%	ARGUMENTS: %*>> %STARTUP_LOG_FILE%

cd /D %M4_SERVER_CONFIGURATION_ROOT%
call:removeTempFiles
start /b %M4_SERVER_EXE% >> %2 2>&1
goto exit

:: ------------------------------------------------------------------------
:: shutdown the server 
:: ------------------------------------------------------------------------
:stop
set ARGV=%*
set M4_SHUTDOWN_EXE=m4sdown.exe -host localhost -obl %M4_SERVER_CONFIGURATION_ROOT%\startup.obl -file %M4_USER_CREDENTIALS% %ARGV%
cd /D %M4_SERVER_CONFIGURATION_ROOT% 
%M4_SHUTDOWN_EXE%
goto exit

:: ------------------------------------------------------------------------
:: kill the server 
:: ------------------------------------------------------------------------
:kill
cd /D %M4_SERVER_CONFIGURATION_ROOT%
if not exist pid echo File pid not found. Can't kill m4server. & goto exit
set /P FILE=<pid
for /F "tokens=1 delims= " %%F IN ("%FILE%") do set "PID=%%F"
echo Terminating %PID%...
taskkill /T /F /pid %PID%
goto exit

:: ------------------------------------------------------------------------
:: run dependency walker
:: ------------------------------------------------------------------------
:depends
start %DEPENDS% %M4_SERVER_INSTALLATION_ROOT%\bin\%M4_RUN_MODE%\m4server.exe
goto exit

:: ------------------------------------------------------------------------
:: run msdev
:: ------------------------------------------------------------------------
:msdev
cd /D %M4_SERVER_CONFIGURATION_ROOT%
start %MSDEV% /debugexe %M4_SERVER_INSTALLATION_ROOT%\bin\%M4_RUN_MODE%\%M4_SERVER_EXE%
goto exit

:: ------------------------------------------------------------------------
:: show the help
:: ------------------------------------------------------------------------
:usage
echo Usage:
echo   %PROGRAMNAME% start or stop           - default is start
echo   %PROGRAMNAME% depends                 - run dependency walker
echo   %PROGRAMNAME% msdev                   - run m4appserver in MsDev
echo   %PROGRAMNAME% help                    - this screen.
goto exit

:: ------------------------------------------------------------------------
:: remove temp files
:: ------------------------------------------------------------------------
:removeTempFiles
cd temp
dir /B /AD "." | findstr /V /I "m4cache m4debug m4reports JAR" > ".\remdir.lst"
for /F "delims=" %%f in (.\remdir.lst) do rmdir /S /Q "%%f"
del /F /Q .\*.*
:: Uncomment to clean reports temp files
:: rmdir /S /Q .\m4reports
:: mkdir m4reports
cd ..
goto:eof

:: ------------------------------------------------------------------------
:: exit.
:: ------------------------------------------------------------------------
:exit
endlocal
