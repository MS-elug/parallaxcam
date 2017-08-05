@echo off
REM http://landinghub.visualstudio.com/visual-cpp-build-tools
REM https://blogs.msdn.microsoft.com/vcblog/2016/10/24/building-your-c-application-with-visual-studio-code/

echo Initialize Microsoft CPP tools env
pushd %~dp0
call "C:\Program Files (x86)\Microsoft Visual C++ Build Tools\vcbuildtools.bat" x64
popd

echo Clean
if exist bin rmdir /S /Q bin
mkdir bin

echo Build application

set appName=parallax.exe
set compilerflags=/Od /Zi /EHsc
set includeflags=/I "%~dp0\..\lib\libusb-1.0.21\include" /I "%~dp0\..\lib\opencv-2.4.13.3\build\include"
set linkerflags=/OUT:bin/%appName% /LIBPATH:"%~dp0\..\lib\libusb-1.0.21\MS64\dll" /LIBPATH:"%~dp0\..\lib\opencv-2.4.13.3\build\x64\vc14\lib" libusb-1.0.lib opencv_core2413.lib opencv_highgui2413.lib
cl.exe %includeflags%  %compilerflags% src/main.cpp /link %linkerflags% %libFlags%

echo Copy DLL files needed to run %appName%
ROBOCOPY  "%~dp0\..\lib\libusb-1.0.21\MS64\dll" .\bin libusb-1.0.dll /NFL /NDL /NJH /NJS /nc /ns /np
ROBOCOPY  "%~dp0\..\lib\opencv-2.4.13.3\build\x64\vc14\bin" .\bin opencv_core2413.dll opencv_highgui2413.dll opencv_imgproc2413.dll /NFL /NDL /NJH /NJS /nc /ns /np