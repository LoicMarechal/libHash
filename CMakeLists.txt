

################################
# GLOBAL CONFIGURATION VARIABLES
################################

cmake_minimum_required (VERSION 3.0.1)
set (libHash_VERSION_MAJOR 1)
set (libHash_VERSION_MINOR 40)

project(libHash VERSION "${libHash_VERSION_MAJOR}.${libHash_VERSION_MINOR}" LANGUAGES C)

include (CheckLanguage)
check_language (Fortran)
if(CMAKE_Fortran_COMPILER)
  enable_language(Fortran)
endif()

set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)


if (WIN32)
   set (CMAKE_C_FLAGS -DWIN32)
   set (CMAKE_CONFIGURATION_TYPES Release)
else ()
   set (CMAKE_C_FLAGS -O3)
   set (CMAKE_Fortran_FLAGS -O3)
endif ()

project (libHash VERSION ${libHash_VERSION_MAJOR}.${libHash_VERSION_MINOR} LANGUAGES C)


#######################################
# SET FILES AND DIRECTORIES TO BE BUILT
#######################################

include_directories (${PROJECT_SOURCE_DIR}/sources)
add_subdirectory (sources)
add_subdirectory (examples)

install (FILES LICENSE.txt copyright.txt DESTINATION share/libHash)
install (DIRECTORY sample_meshes DESTINATION share/libHash)


######################################
# SET PACKAGE AND DEPLOYMENT VARIABLES
######################################

include (InstallRequiredSystemLibraries)
set (CPACK_GENERATOR TXZ)
set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/copyright.txt)
set (CPACK_PACKAGE_VERSION_MAJOR ${libHash_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${libHash_VERSION_MINOR})
set(CPACK_COMPONENTS_ALL applications examples libraries headers)
include (CPack)


##################################
# EXPORT CMAKE PACKAGE INFORMATION
##################################
   
include(CMakePackageConfigHelpers)
file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
"include(\${CMAKE_CURRENT_LIST_DIR}/meshb-target.cmake)
include(\${CMAKE_CURRENT_LIST_DIR}/libHash-target.cmake)
set(libHash_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(libHash_LIBRARIES Hash.7)
set(libHash_Fortran_LIBRARIES Hashf.7)
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
