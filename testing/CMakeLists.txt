
###########
# RUN TESTS
###########

# Unpack the test case mesh
if( NOT EXISTS "${CMAKE_BINARY_DIR}/sample_meshes/volume.meshb"
    AND EXISTS "${CMAKE_SOURCE_DIR}/testing/sample_meshes.tpxz" )
   message ("-- Unpacking sample_meshes.tpxz")
   file(ARCHIVE_EXTRACT INPUT "${CMAKE_SOURCE_DIR}/testing/sample_meshes.tpxz"
                        DESTINATION "${CMAKE_BINARY_DIR}")
endif()

add_test(NAME feature1 COMMAND extract_boundary 
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/examples")
