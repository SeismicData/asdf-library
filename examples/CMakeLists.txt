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

enable_language (Fortran)
add_definitions(${MPI_Fortran_COMPILE_FLAGS})
include_directories(${MPI_Fortran_INCLUDE_PATH})
link_directories(${MPI_Fortran_LIBRARIES})
                    
#message(Status "MPI libs: " ${MPI_LIBRARIES})
#message(Status "MPI_Fortran libs: " ${MPI_Fortran_LIBRARIES})
#message(Status "HDF5 fort: " ${HDF5_C_LIBRARIES})
#message(Status "HDF5 fort: " "${HDF5_LIBRARIES}")

add_executable(write_example_f90 write_example.f90)
target_link_libraries(write_example_f90
                      asdf
                      ${MPI_Fortran_LINK_FLAGS}
                      ${MPI_LIBRARIES}
                      ${MPI_Fortran_LIBRARIES})

add_executable(read_example_f90 read_example.f90)
target_link_libraries(read_example_f90
                      asdf
                      ${MPI_Fortran_LINK_FLAGS}
                      ${MPI_LIBRARIES}
                      ${MPI_Fortran_LIBRARIES})
