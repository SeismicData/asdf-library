############################################################
### Exemple showing how to write ASDF files in C.        ###
############################################################

set (${HDF5_ENABLE_PARALLEL} 1)

# message (STATUS ${HDF5_LIBRARIES})
# message (STATUS ${HDF5_C_LIBRARIES})

add_executable(write_ASDF write_ASDF.c)
target_link_libraries(write_ASDF
                      asdf
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})
