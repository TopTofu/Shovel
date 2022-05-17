@echo off

Rem check if msvc needs to be set up
where cl >nul 2>nul
if %ERRORLEVEL% neq 0 call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul

set CommonCompilerFlags= -DDEV -Zi -nologo -EHsc -I "../Source" -I "../Library/include"
set CommonLinkerFlags= -out:Main.exe "opengl32.lib" "user32.lib" "gdi32.lib"

if not exist bin mkdir bin
pushd bin

cl %CommonCompilerFlags% "..\Main.cpp" /link %CommonLinkerFlags% >nul

popd

