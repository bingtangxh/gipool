@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "ARCH=%~1"
set "MODE=%~2"
if not defined ARCH set "ARCH=x64"
if not defined MODE set "MODE=release"

set "VCVARS_ARG=%ARCH%"
if /I "%ARCH%"=="x86" set "VCVARS_ARG=AMD64_x86"
if /I "%ARCH%"=="arm" set "VCVARS_ARG=AMD64_ARM"
if /I "%ARCH%"=="arm64" set "VCVARS_ARG=x64_ARM64"

set "SCRIPT_DIR=%~dp0"
set "OUT_NAME=gipool_clang_%ARCH%.exe"
set "PDB_NAME=gipool_clang_%ARCH%.pdb"
set "CLANG_FLAGS=/nologo /TC /utf-8 /W3 /GF /FS"
set "LINK_FLAGS=/OUT:%OUT_NAME%"
if /I "%MODE%"=="debug" (
    set "CLANG_FLAGS=%CLANG_FLAGS% /Z7 /Od"
    set "LINK_FLAGS=%LINK_FLAGS% /DEBUG /PDB:%PDB_NAME%"
) else (
    set "CLANG_FLAGS=%CLANG_FLAGS% /O2"
)

set "SRCS="
for %%F in ("%SCRIPT_DIR%*.c") do (
    set "SRCS=!SRCS! "%%~fF""
)
if not defined SRCS (
    echo [build_clang] No .c files found in %SCRIPT_DIR%
    exit /b 1
)

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [build_clang] vswhere.exe not found.
    exit /b 1
)

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%I"
if not defined VSINSTALL (
    echo [build_clang] Visual Studio with VC tools not found.
    exit /b 1
)

set "VCVARS=%VSINSTALL%\VC\Auxiliary\Build\vcvarsall.bat"
if not exist "%VCVARS%" (
    echo [build_clang] vcvarsall.bat not found: %VCVARS%
    exit /b 1
)

call "%VCVARS%" %VCVARS_ARG% >nul || exit /b 1

set "CLANG_CL="

for /f "delims=" %%I in ('where clang-cl.exe 2^>nul') do (
    set "CLANG_CL=%%I"
    goto :clang_found
)

set "CLANG_CL=%VSINSTALL%\VC\Tools\Llvm\bin\clang-cl.exe"

:clang_found
if not exist "%CLANG_CL%" (
    echo [build_clang] clang-cl.exe not found in PATH or Visual Studio LLVM tools.
    exit /b 1
)

echo [build_clang] Building %OUT_NAME% using %VCVARS_ARG% (%MODE%)
"%CLANG_CL%" %CLANG_FLAGS% %SRCS% /Fe:%OUT_NAME% /link %LINK_FLAGS%
exit /b %errorlevel%

