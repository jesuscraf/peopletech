@echo off
:: ========================================================================
:: @(#) FileVersion: [M4_FILE_VERSION]
:: @(#) FileDescription: This script generates debuging info of a process.
:: @(#) CompanyName: Meta4 Spain, S.A.
:: @(#) LegalCopyright: (c) [M4_YEAR]
:: @(#) ProductName:  PeopleNet
:: ========================================================================

:: ------------------------------------------------------------------------
:: Check arguments.
:: ------------------------------------------------------------------------
if "%3" == "" goto Usage

:: ------------------------------------------------------------------------
:: Globals.
:: ------------------------------------------------------------------------
echo [%0] Collecting debug info.
echo [%0] Process %1 %2 %3

:: ------------------------------------------------------------------------
:: Creates the base directory for dumps.
:: ------------------------------------------------------------------------
if not exist m4dumps mkdir m4dumps
cd m4dumps

:: ------------------------------------------------------------------------
:: Gets the last dump info.
:: ------------------------------------------------------------------------
set LastPID=0
set LastThread=0
set LastTime=0

if exist m4dump.last call:ReadLastDumpInfo

:: ------------------------------------------------------------------------
:: Checks if this process is already dumped.
:: ------------------------------------------------------------------------
set /a ElapsedTime=(%3-LastTime)

if %ElapsedTime% leq 5 if %LastPID%==%1 if %LastThread%==%2 goto AlreadyCollected
goto NotYetCollected

:AlreadyCollected
echo [%0] Debug info already collected. Exiting.
cd..
goto exit

:NotYetCollected

:: ------------------------------------------------------------------------
:: Creates the output directory.
:: ------------------------------------------------------------------------
set Hour=%time:~0,2%
if "%Hour:~0,1%" == " " set Hour=0%Hour:~1,1%

set Min=%time:~3,2%
if "%Min:~0,1%" == " " set Min=0%Min:~1,1%

set Secs=%time:~6,2%
if "%Secs:~0,1%" == " " set Secs=0%Secs:~1,1%

set Year=%date:~-4%

set Month=%date:~3,2%
if "%Month:~0,1%" == " " set Month=0%Month:~1,1%

set Day=%date:~0,2%
if "%Day:~0,1%" == " " set Day=0%Day:~1,1%

set outDir=m4dump-%Year%-%Month%-%Day%-%Hour%-%Min%-%Secs%-%1-%2
mkdir %outDir%

:: ------------------------------------------------------------------------
:: Copy server traces.
:: ------------------------------------------------------------------------
xcopy    /C /Q /Y ..\temp\*.* %outDir%
xcopy /E /C /Q /Y ..\temp\m4debug %outDir%

:: ------------------------------------------------------------------------
:: Writes to disk the last dump info.
:: ------------------------------------------------------------------------
call:WriteLastDumpInfo %1 %2 %3

cd ..
echo [%0] Debug info output folder: %outDir%

goto exit

:: ------------------------------------------------------------------------
:: Usage.
:: ------------------------------------------------------------------------
:Usage
echo Usage:
echo   %0 ProcessPid ThreadId CrashTime
goto exit

:: ------------------------------------------------------------------------
:: Read last dump info.
:: ------------------------------------------------------------------------
:ReadLastDumpInfo
for /f "tokens=1-2 delims=:" %%a in (m4dump.last) do (
    if %%a==dpid set LastPID=%%b
	if %%a==dthread set LastThread=%%b
	if %%a==dtime set LastTime=%%b
)
goto:eof

:: ------------------------------------------------------------------------
:: Write last dump info.
:: ------------------------------------------------------------------------
:WriteLastDumpInfo
echo dpid:%~1>m4dump.last
echo dthread:%~2>>m4dump.last
echo dtime:%~3>>m4dump.last
goto:eof

:: ------------------------------------------------------------------------
:: Exits.
:: ------------------------------------------------------------------------
:exit
