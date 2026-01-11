@echo off
echo ==========================================
echo  KAFKA PROJECT - DEBUG BUILD
echo ==========================================
echo.

REM Kill ALL potentially locking processes
echo [1/6] Stopping all processes...
taskkill /F /IM Kafka.exe 2>nul
taskkill /F /IM devenv.exe 2>nul
taskkill /F /IM MSBuild.exe 2>nul
taskkill /F /IM VBCSCompiler.exe 2>nul
taskkill /F /IM vcpkgsrv.exe 2>nul

echo Waiting for processes to fully terminate...
timeout /t 3 /nobreak >nul

REM Clean ALL build artifacts thoroughly
echo [2/6] Cleaning ALL build files...
if exist "Kafka\x64" rmdir /s /q "Kafka\x64"
if exist "Kafka\Debug" rmdir /s /q "Kafka\Debug"
if exist "x64" rmdir /s /q "x64"
if exist "Debug" rmdir /s /q "Debug"
if exist ".vs" rmdir /s /q ".vs"

REM Double check specific problematic files are gone
if exist "Kafka\x64\Debug\vc143.pdb" del /f /q "Kafka\x64\Debug\vc143.pdb" 2>nul
if exist "Kafka\x64\Debug\Kafka.ilk" del /f /q "Kafka\x64\Debug\Kafka.ilk" 2>nul
if exist "x64\Debug\vc143.pdb" del /f /q "x64\Debug\vc143.pdb" 2>nul
if exist "x64\Debug\Kafka.ilk" del /f /q "x64\Debug\Kafka.ilk" 2>nul

echo All artifacts cleaned!

REM Find MSBuild
echo [3/6] Finding Visual Studio build tools...
set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
if not exist %MSBUILD% (
    echo ERROR: MSBuild not found!
    echo.
    echo Please ensure Visual Studio 2022 is installed with C++ tools.
    echo Or build using Visual Studio GUI instead.
    pause
    exit /b 1
)

echo Found MSBuild!

REM Clean via MSBuild
echo [4/6] Running MSBuild Clean...
%MSBUILD% Kafka.sln /t:Clean /p:Configuration=Debug /p:Platform=x64 /v:quiet

REM Build the project
echo [5/6] Building project in DEBUG mode...
echo This may take a minute...
echo.

%MSBUILD% Kafka.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /v:minimal

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==========================================
    echo  BUILD FAILED!
    echo ==========================================
    echo.
    echo Common fixes:
    echo 1. Close Visual Studio completely
    echo 2. Run this batch file again
    echo 3. If still fails, try BUILD_AND_RUN.bat (uses Release)
    echo.
    pause
    exit /b 1
)

echo.
echo ==========================================
echo  BUILD SUCCESSFUL!
echo ==========================================
echo.

REM Run the program
echo [6/6] Running Kafka.exe in DEBUG mode...
echo.
echo ==========================================
echo.

if exist "x64\Debug\Kafka.exe" (
    x64\Debug\Kafka.exe
) else (
    echo ERROR: Kafka.exe not found!
    echo Expected location: x64\Debug\Kafka.exe
    echo.
    echo The build reported success but executable is missing.
    echo Check the build output above for issues.
)

echo.
echo ==========================================
echo  PROGRAM FINISHED
echo ==========================================
echo.
echo TIP: If Debug build keeps failing, use Release mode:
echo   - Run BUILD_AND_RUN.bat (uses Release), or
echo   - In Visual Studio, set configuration to Release
echo.
pause

