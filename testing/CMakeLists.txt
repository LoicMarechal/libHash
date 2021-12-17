
###########
# RUN TESTS
###########

# Unpack the test case mesh
if( NOT EXISTS "${CMAKE_BINARY_DIR}/sample_meshes/volume.meshb" )
   if( EXISTS "${CMAKE_BINARY_DIR}/sample_meshes/volume.meshb.xz" )
   message ("-- Unpacking volume.meshb.xz")
   file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/sample_meshes/volume.meshb.xz"
                        DESTINATION "${CMAKE_BINARY_DIR}/sample_meshes/")
endif()

add_test(NAME feature1 COMMAND extract_boundary_faces 
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/examples")
