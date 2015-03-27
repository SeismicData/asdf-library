############################################################
### Exemple showing how to write ASDF files in C.        ###
############################################################
add_executable(write_ASDF write_ASDF.c)
target_link_libraries(write_ASDF
                      asdf
                      ${HDF5_C_LIBRARIES}
                      ${MPI_LIBRARIES})
