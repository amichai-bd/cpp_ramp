#!/bin/bash

# $1 is the first argument passed to the script.
filename=$1

# Compile the code.
g++ -g $filename.cpp -o $filename.exe -IC:/msys64/SDL2-2.26.5/x86_64-w64-mingw32/include/SDL2 -LC:/msys64/SDL2-2.26.5/x86_64-w64-mingw32/lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -Wl,-rpath,C:/msys64/SDL2-2.26.5-win32-x64/api_ms_win
