@echo off
SET PROJ_NAME=checkmate
SET MODE=%1
SET ARG=%2

REM -----------------------------
REM Check mode
REM -----------------------------
IF /I "%MODE%"=="check" (
    IF "%ARG%"=="" (
        echo You must provide a file to check!
        pause
        exit /b
    )
    echo Linting %ARG%...
    gcc -Wall -Wextra -Wpedantic -fsyntax-only %ARG%
    pause
    exit /b
)

REM -----------------------------
REM Compile mode
REM -----------------------------
IF /I "%MODE%"=="compile" (
    echo Compiling project...
    make
    pause
    exit /b
)

REM -----------------------------
REM Run mode
REM -----------------------------
IF /I "%MODE%"=="run" (
    IF NOT EXIST %PROJ_NAME%.exe (
        echo Error: %PROJ_NAME%.exe not found!
        pause
        exit /b
    )
    echo Running %PROJ_NAME%.exe...
    %PROJ_NAME%.exe %ARG%
    pause
    exit /b
)

REM -----------------------------
REM Build static library (.a)
REM -----------------------------
IF /I "%MODE%"=="liba" (
    echo Building static library lib%PROJ_NAME%.a...
    make slib
    pause
    exit /b
)

REM -----------------------------
REM Build Windows DLL
REM -----------------------------
IF /I "%MODE%"=="libdll" (
    echo Building Windows DLL checkmate.dll...
    make windlib
    pause
    exit /b
)


REM -----------------------------
REM Invalid mode
REM -----------------------------
echo Invalid mode! Use: check, compile, run, liba, libdll, or libso
pause
exit /b
