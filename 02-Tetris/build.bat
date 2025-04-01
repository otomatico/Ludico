@ECHO off
REM CLS
ECHO Si no sabe que hacer escriba:
ECHO build.bat main Tetris
ECHO -------------------------------------

ECHO   Setup required Environment
ECHO -------------------------------------
REM GCC
SET "COMPILER_PATH=C:\mingw64\bin"
SET "CC=%COMPILER_PATH%\g++.exe"
SET "ENV=RELEASE"
SET "CFLAGS2=-O3"
IF DEBUG == %ENV% SET "CFLAGS2=-Wall -Wextra -g3"

ECHO off
ECHO   Clean latest build
ECHO ------------------------
SET "FILENAME=main"
IF [%1] NEQ [] SET "FILENAME=%1"
SET "ROM_NAME=%FILENAME%"
IF [%2] NEQ [] SET "ROM_NAME=%2"
IF NOT EXIST output mkdir output
IF EXIST output\%ROM_NAME%.exe del /F output\%ROM_NAME%.exe

REM ECHO off
ECHO   Compile program
ECHO -----------------------
%CC% -o ./output/%ROM_NAME%.exe ./src/%FILENAME%.cpp %CFLAGS2%
@ECHO on

