

################################
# GLOBAL CONFIGURATION VARIABLES
################################

cmake_minimum_required (VERSION 3.7.2)
set (libHash_VERSION_MAJOR 1)
set (libHash_VERSION_MINOR 40)
project(libHash VERSION ${libHash_VERSION_MAJOR}.${libHash_VERSION_MINOR} LANGUAGES C)

option(WITH_CPACK "Enable cpack target to generate a zip file containing binaries" OFF)
option(WITH_CTEST "Enable ctest with features and regressions test-cases"          OFF)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
   set (math_LIBRARIES "m")
endif ()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   if (WIN32)
      set(CMAKE_INSTALL_PREFIX "$ENV{HOMEPATH}/cmakebuilds" CACHE PATH "..." FORCE)
   else ()
      set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/cmakebuilds/$ENV{OSTYPE}-$ENV{MACHTYPE}" CACHE PATH "..." FORCE)
   endif()
endif ()

list (APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")


#######################################
# SET FILES AND DIRECTORIES TO BE BUILT
#######################################

include_directories (${PROJECT_SOURCE_DIR}/sources)
add_subdirectory (sources)

find_package (libMeshb 7)

if (libMeshb_FOUND)
   include_directories (${libMeshb_INCLUDE_DIRS})
   include_directories (${PROJECT_SOURCE_DIR}/examples)
   add_subdirectory (examples)
else ()
   message (WARNING "
   libMeshb.7 could not be found.
   Consequently, examples and utilities will not be built.
   You can download it from GitHUB at https://github.com/LoicMarechal/libMeshb")
endif ()

install (FILES LICENSE.txt copyright.txt DESTINATION share/libHash)
install (DIRECTORY sample_meshes DESTINATION share/libHash)

if(WITH_CTEST)
   enable_testing()
   include_directories(${PROJECT_SOURCE_DIR}/testing)
   add_subdirectory(testing)
endif()


######################################
# SET PACKAGE AND DEPLOYMENT VARIABLES
######################################

if (WITH_CPACK)
   include (InstallRequiredSystemLibraries)
   set (CPACK_GENERATOR TXZ)
   set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/copyright.txt)
   set (CPACK_PACKAGE_VERSION_MAJOR ${libHash_VERSION_MAJOR})
   set (CPACK_PACKAGE_VERSION_MINOR ${libHash_VERSION_MINOR})
   set (CPACK_COMPONENTS_ALL applications examples libraries headers)
   include (CPack)
endif ()


##################################
# EXPORT CMAKE PACKAGE INFORMATION
##################################

include(CMakePackageConfigHelpers)
file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
"include(\${CMAKE_CURRENT_LIST_DIR}/hash-target.cmake)
include(\${CMAKE_CURRENT_LIST_DIR}/libHash-target.cmake)
set(libHash_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(libHash_LIBRARIES Hash.1)
set(libHash_FOUND TRUE)
")
write_basic_package_version_file(
   "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion)
install(FILES
   "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
   "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
   DESTINATION lib/cmake/${PROJECT_NAME})


##################################
# PRINT OPERATIONS TO BE CONDUCTED
##################################

message("-- Build mode            : " ${CMAKE_BUILD_TYPE})
message("-- cpack target enabled  : " ${WITH_CPACK})
message("-- Install directory     : " ${CMAKE_INSTALL_PREFIX})
