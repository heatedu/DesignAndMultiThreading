@echo off
echo ==========================================
echo  KAFKA PROJECT - BUILD AND RUN
echo ==========================================
echo.

REM Kill any locked processes
echo [1/5] Stopping any running processes...
taskkill /F /IM Kafka.exe 2>nul
taskkill /F /IM VBCSCompiler.exe 2>nul

REM Clean build artifacts
echo [2/5] Cleaning old build files...
if exist "Kafka\x64" rmdir /s /q "Kafka\x64"
if exist "x64" rmdir /s /q "x64"
if exist ".vs" rmdir /s /q ".vs"

REM Find MSBuild
echo [3/5] Finding Visual Studio build tools...
set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% (
    echo ERROR: MSBuild not found!
    echo Please build using Visual Studio instead.
    pause
    exit /b 1
)

REM Build the project
echo [4/5] Building project (Release mode)...
%MSBUILD% Kafka.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64 /v:minimal

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==========================================
    echo  BUILD FAILED!
    echo ==========================================
    echo Check the errors above.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo  BUILD SUCCESSFUL!
echo ==========================================
echo.

REM Run the program
echo [5/5] Running Kafka.exe...
echo.
echo ==========================================
echo.

if exist "x64\Release\Kafka.exe" (
    x64\Release\Kafka.exe
) else if exist "x64\Debug\Kafka.exe" (
    x64\Debug\Kafka.exe
) else (
    echo ERROR: Kafka.exe not found!
    echo Expected locations: x64\Release\Kafka.exe or x64\Debug\Kafka.exe
)

echo.
echo ==========================================
echo  PROGRAM FINISHED
echo ==========================================
pause

