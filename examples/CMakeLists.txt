
##################
# BUILD C EXAMPLES
##################

add_executable (extract_boundary extract_boundary_faces.c)
target_link_libraries (extract_boundary ${libMeshb_LIBRARIES} ${math_LIBRARIES} Hash.1)
install (TARGETS extract_boundary DESTINATION share/libHash/examples COMPONENT examples)
