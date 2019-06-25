
##################
# BUILD C EXAMPLES
##################

add_executable(extract_boundary extract_boundary.c)
target_link_libraries(extract_boundary Meshb.7 Hash.1)
install (TARGETS extract_boundary DESTINATION share/libHash/examples COMPONENT examples)
