############################################################
### Example showing how to write ASDF files in C.        ###
############################################################

set (${HDF5_ENABLE_PARALLEL} 1)
enable_language (Fortran)
add_definitions(${MPI_Fortran_COMPILE_FLAGS})
include_directories(${MPI_Fortran_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})
link_directories(${MPI_Fortran_LIBRARIES})

add_executable(write_ASDF write_ASDF.c)
target_link_libraries(write_ASDF
                      asdf
                      ${HDF5_LIBRARIES}
                      ${MPI_LIBRARIES})

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

set_property(TARGET write_example_f90 PROPERTY LINKER_LANGUAGE Fortran)
set_property(TARGET read_example_f90 PROPERTY LINKER_LANGUAGE Fortran)
install(FILES write_ASDF.c write_example.f90 read_example.f90
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/)
