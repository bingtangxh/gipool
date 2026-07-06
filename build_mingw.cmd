@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "OUT_NAME=gipool_mingw.exe"
set "CSTD=c99"
set "CFLAGS=-std=%CSTD% -Wall -Wextra -g -finput-charset=UTF-8 -fexec-charset=UTF-8"
set "LDFLAGS="
set "SRCS="

for %%F in ("*.c") do (
    set "SRCS=!SRCS! "%%~fF""
)

if not defined SRCS (
    echo [build_mingw] No .c files found in %CD%
    exit /b 1
)

echo [build_mingw] Building %OUT_NAME%
gcc %CFLAGS% %SRCS% -o "%OUT_NAME%" %LDFLAGS%
exit /b %errorlevel%
