@echo off

call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"

cl /nologo /W4 /EHsc /std:c++17 /Zi /Od main.cpp ^
    /link user32.lib ^
    /link gdi32.lib ^
    /link Comctl32.lib ^
    /SUBSYSTEM:WINDOWS ^
    /DEBUG ^
    /OUT:.\main.exe

if %errorlevel% neq 0 (
    echo.
    echo Build failed.
    exit /b %errorlevel%
)

echo.
echo Build successful.
echo Executable: .\main.exe