## libHash version 1.40

## Overview

## Build for *Linux* or *macOS*
Simply follow these steps:
- unarchive the ZIP file
- `cd libHash-master`
- `mkdir build`
- `cd build`
- `cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ../`
- `make`
- `make install`

## Build for *Windows*
- You first need to install [CMake](https://cmake.org/files/v3.7/cmake-3.7.2-win64-x64.msi). Do not forget to choose "add cmake to the path for all users", from the install panel.
- Then you need a valid C compiler like the free [Visual Studio Express 2015](https://www.visualstudio.com/vs/visual-studio-express/)
- unarchive the ZIP file
- open the windows shell
- `cd libHash-master`
- `mkdir build`
- `cd build`
- `cmake -DCMAKE_INSTALL_PREFIX=%HOMEPATH%\local ..\`
- `cmake --build . --config Release --target INSTALL`

## Usage
The **libHash** library is written in *ANSI C*.  
It is made of a single C file and a header file to be compiled and linked alongside the calling program.  
It may be used in C, C++, F77 and F90 programs (Fortran 77 and 90 APIs are provided).  
Tested on *Linux*, *macOS*, and *Windows 7->10*.


```C++
int i, j, vertex, ball[100];
int64_t LibIdx;

LibIdx = hsh_NewTable();

// Hash each tet with their four vertices
for(i=1;i<=NmbTet;i++)
   for(j=0;j<4;j++)
      hsh_AddItem(LibIdx, HshTet, TetTab[i][j], i, 1);

// Find and print the ball of vertex 150
vertex = 150;
degree = hsh_GetItem(LibIdx, 0, HshAny, 1, &vertex, &ball, NULL);
for(i=0;i<degree;i++)
   printf("%d\n", ball[i]);

hsh_FreeTable(HshIdx)
```
