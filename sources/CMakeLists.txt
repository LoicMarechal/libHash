
##########################
# BUILD THE STATIC LIBRARY
##########################

add_library (Hash.1 libhash.c)
install (FILES libhash.h DESTINATION include COMPONENT headers)
target_include_directories (Hash.1 PUBLIC $<INSTALL_INTERFACE:include>)
install (TARGETS Hash.1 EXPORT hash-target DESTINATION lib COMPONENT libraries)
install (EXPORT hash-target NAMESPACE ${PROJECT_NAME}::
         DESTINATION lib/cmake/${PROJECT_NAME})
install (TARGETS Hash.1 EXPORT libHash-target DESTINATION lib COMPONENT libraries)
install (EXPORT libHash-target DESTINATION lib/cmake/${PROJECT_NAME})
export  (PACKAGE libHash) 
