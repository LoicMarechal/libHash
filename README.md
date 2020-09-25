## libHash version 1.40

## Overview
A hash table library that offers dynamicaly resizing tables and arbitrary number of hash keys insert and queries.


## Build

### Prerequisites for *Linux* or *macOS*
- Install [CMake](https://cmake.org/files/v3.7/cmake-3.7.2-win64-x64.msi)
- A valid C99 compiler
- Open a shell window

### Prerequisites for *Windows*
- You first need to install [CMake](https://cmake.org/files/v3.7/cmake-3.7.2-win64-x64.msi). Do not forget to choose "add cmake to the path for all users", from the install panel.
- Then you need a valid C compiler like the free [Visual Studio Community 2019](https://www.visualstudio.com/vs/visual-studio-express/)
- Open the x64 Native Tools Command Prompt for VS (or x86 if you need to build a 32-bit version)

### Build commands for all platforms
- unarchive the ZIP file
- `cd libHash-master`
- `mkdir build`
- `cd build`
- `cmake ..`
- `cmake --build . --target INSTALL`

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
