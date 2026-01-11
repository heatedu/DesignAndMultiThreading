@echo off
echo Building Car Rental System...
cd CarRentalCursor
cl /EHsc /std:c++17 /W3 main.cpp /Fe:CarRental.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Running the program...
    echo.
    CarRental.exe
) else (
    echo.
    echo Build failed! Please open CarRentalCursor.sln in Visual Studio and build from there.
)
pause

