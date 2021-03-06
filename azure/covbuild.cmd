REM Script to build the Open Watcom tools
REM using the host platform's native C/C++ compiler or OW tools.
REM

mkdir %OWBINDIR%

cd %OWSRCDIR%\wmake
mkdir %OWOBJDIR%
cd %OWOBJDIR%
nmake -f ..\nmake
cd %OWSRCDIR%\builder
mkdir %OWOBJDIR%
cd %OWOBJDIR%
%OWBINDIR%\wmake -f ..\binmake bootstrap=1 builder.exe
cd %OWSRCDIR%
builder boot
builder build os_nt cpu_x64
